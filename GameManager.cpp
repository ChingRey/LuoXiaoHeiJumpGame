#include "GameManager.h"
#include "Platform.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cwchar>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

// =====================================================================
//  Constructor / Destructor
// =====================================================================
// 透明图片绘制函数（支持PNG透明）
inline void putimage_alpha(int x, int y, const IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

GameManager::GameManager()
	: state(START_MENU), cameraTop(0)
	, score(0), highScore(0), platformsJumped(0)
	, lastGeneratedY(0), fallFrames(0), startBgLoaded(false)
	, winTriggered(false), bgCacheReady(false), bgmLoaded(false)
{
	srand((unsigned)time(nullptr));

	// Define button rectangles (screen coords)
	// Start button at bottom, above footer area
	startBtn   = { 88, 470, 312, 630};
	pauseBtn   = { SCREEN_WIDTH - 70, 3, SCREEN_WIDTH - 3, 58 };
	restartBtn = { SCREEN_WIDTH / 2 - 90, 455, SCREEN_WIDTH / 2 + 70, 590 };
	endBtn     = { SCREEN_WIDTH / 2 - 140, 520, SCREEN_WIDTH / 2 + 140, 610 };

	LoadHighScore();
	// Only load background for start menu (small image), not for gameplay
	if (loadimage(&startBg, L"assets\\ui\\start_bg_2.png",
		SCREEN_WIDTH, SCREEN_HEIGHT, true) == 0) {
		startBgLoaded = true;
	}

	// 背景图片在 ResetGame 中延迟加载，避免启动黑屏
}

GameManager::~GameManager() {
	mciSendString(L"close bgm", NULL, 0, NULL);
}

// =====================================================================
//  Main Loop
// =====================================================================

void GameManager::Run() {
	initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetWindowText(GetHWnd(), L"Luo Xiao Hei Jump Adventure");
	BeginBatchDraw();

	ResetGame();
	state = START_MENU;

	while (true) {
		// Check window still alive
		if (!IsWindow(GetHWnd())) break;

		// -------- Input --------
		bool leftKey  = (GetAsyncKeyState(VK_LEFT)  & 0x8000) != 0;
		bool rightKey = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;

		// Mouse clicks
		while (MouseHit()) {
			MOUSEMSG m = GetMouseMsg();
			if (m.uMsg != WM_LBUTTONDOWN) continue;

			int mx = m.x, my = m.y;

			switch (state) {
			case START_MENU:
				if (mx >= startBtn.left  && mx <= startBtn.right &&
				    my >= startBtn.top   && my <= startBtn.bottom) {
					ResetGame();
					state = PLAYING;
				}
				break;

			case PLAYING:
				if (mx >= pauseBtn.left  && mx <= pauseBtn.right &&
				    my >= pauseBtn.top   && my <= pauseBtn.bottom) {
					state = PAUSED;
					mciSendString(L"pause bgm", NULL, 0, NULL);
				}
				break;

			case PAUSED:
				if (mx >= pauseBtn.left  && mx <= pauseBtn.right &&
				    my >= pauseBtn.top   && my <= pauseBtn.bottom) {
					state = PLAYING;
					mciSendString(L"resume bgm", NULL, 0, NULL);
				}
				break;

			case GAME_OVER:
				if (mx >= restartBtn.left  && mx <= restartBtn.right &&
				    my >= restartBtn.top   && my <= restartBtn.bottom) {
					ResetGame();
					state = PLAYING;
				}
				break;

			case WIN:
				if (mx >= endBtn.left  && mx <= endBtn.right &&
				    my >= endBtn.top   && my <= endBtn.bottom) {
					ResetGame();
					state = PLAYING;
				}
				break;
			}
		}

		// -------- Update --------
		if (state == PLAYING) {
			player.Update(leftKey, rightKey);
			UpdateCamera();
			CheckCollisions();
			GeneratePlatforms();
			CleanPlatforms();
			CheckGameOver();
			CheckWin();  // 检查是否通关
		}

		// -------- Draw --------
		cleardevice();
		DrawBackground();

		// Reset drawing settings before each frame
		setlinestyle(PS_SOLID);
		setbkmode(OPAQUE);

		switch (state) {
		case START_MENU:
			DrawStartMenu();
			break;

		case PLAYING:
			for (auto& p : platforms) p.Draw(cameraTop);
			for (auto& d : deadImages) d.Draw(cameraTop);
			player.Draw(cameraTop);
			DrawPlayingUI();
			break;

		case PAUSED:
			for (auto& p : platforms) p.Draw(cameraTop);
			for (auto& d : deadImages) d.Draw(cameraTop);
			player.Draw(cameraTop);
			DrawPlayingUI();
			DrawPauseOverlay();
			break;

		case GAME_OVER:
			DrawGameOverScreen();
			break;

		case WIN:
			// 通关时也绘制背景和平台（使用最后的 cameraTop）
			for (auto& p : platforms) p.Draw(cameraTop);
			player.Draw(cameraTop);
			DrawWinScreen();
			break;
		}

		FlushBatchDraw();
		Sleep(16);   // ~60 FPS
	}

	EndBatchDraw();
	closegraph();
}

// =====================================================================
//  ResetGame  -  fresh state for a new round
// =====================================================================

void GameManager::ResetGame() {
	// 音乐只加载一次，之后持续循环播放，不中断
	if (!bgmLoaded) {
		mciSendString(L"open \"assets\\bgm\\hx.mp3\" alias bgm", NULL, 0, NULL);
		mciSendString(L"play bgm repeat", NULL, 0, NULL);
		bgmLoaded = true;
	} else {
		mciSendString(L"resume bgm", NULL, 0, NULL);
	}

	player.Reset(
		(float)(SCREEN_WIDTH / 2),
		(float)(SCREEN_HEIGHT - 150),
		SCREEN_WIDTH
	);
	cameraTop       = 0;
	score           = 0;
	platformsJumped = 0;
	fallFrames      = 0;
	winTriggered   = false;
	lastPlatformY = SCREEN_HEIGHT - 150 + Player::RADIUS + 5;  // 初始化为起始平台y坐标
	platforms.clear();
	deadImages.clear();

	// 延迟加载背景图片（仅首次）
	if (!bgCacheReady) {
		for (int i = 0; i < BG_COUNT; i++) {
			wchar_t path[64];
			swprintf_s(path, 64, L"assets/bg/bg%d.png", i + 1);
			// 不指定尺寸，直接加载原图
			loadimage(&bgImages[i], path);
		}
		bgCacheReady = true;
	}

	// 加载死亡图片和跳跃板图片
	Platform::LoadDeathImage();
	Platform::LoadJumpImage();

	// Safe starter platform right under the player's feet
	int safeX = SCREEN_WIDTH / 2 - 45;
	int safeY = (int)player.y + Player::RADIUS + 5;
	platforms.emplace_back(safeX, safeY, 90, 12);

	// Pre-fill platforms upwards well past the visible area
	lastGeneratedY = safeY;
	int wMin, wMax, gMin, gMax;
	GetStageParams(wMin, wMax, gMin, gMax);

	// 先生成所有木板
	while (lastGeneratedY > WIN_PLATFORM_Y - 100) {
		int w   = wMin + rand() % (wMax - wMin + 1);
		int px  = 5 + rand() % (SCREEN_WIDTH - w - 10);
		int gap = gMin + rand() % (gMax - gMin + 1);
		lastGeneratedY -= gap;
		platforms.emplace_back(px, lastGeneratedY, w, 12, PLATFORM_NORMAL);
	}

	// 在第1阶段选1个、第2阶段选2个普通木板改为跳跃板（共3个）
	{
		std::vector<int> stage1Idx, stage2Idx;
		for (int i = 0; i < (int)platforms.size(); i++) {
			if (platforms[i].type != PLATFORM_NORMAL) continue;
			if (platforms[i].y > BG2_START_Y)       // stage 1: y > -650
				stage1Idx.push_back(i);
			else if (platforms[i].y > BG3_START_Y)  // stage 2: -1300 < y <= -650
				stage2Idx.push_back(i);
		}
		if (!stage1Idx.empty())
			platforms[stage1Idx[rand() % stage1Idx.size()]].type = PLATFORM_JUMP;
		// stage 2 选2个（不重复）
		if (stage2Idx.size() >= 2) {
			int i1 = rand() % stage2Idx.size();
			int i2 = rand() % (stage2Idx.size() - 1);
			if (i2 >= i1) i2++;
			platforms[stage2Idx[i1]].type = PLATFORM_JUMP;
			platforms[stage2Idx[i2]].type = PLATFORM_JUMP;
		} else if (!stage2Idx.empty()) {
			platforms[stage2Idx[rand() % stage2Idx.size()]].type = PLATFORM_JUMP;
		}
	}

	// 在第2、3阶段各生成1个悬空危险图片
	for (int s = 2; s <= 3; s++) {
		int stageTop = -s * BG_HEIGHT;
		int deadPlaced = 0;

		for (int retry = 0; retry < 200 && deadPlaced < 1; retry++) {
			int deadX = 5 + rand() % (SCREEN_WIDTH - 80);
			int deadY = stageTop + 30 + rand() % (BG_HEIGHT - 100);

			bool overlap = false;
			for (const auto& p : platforms) {
				if (deadX + 75 > p.x && deadX < p.x + p.width &&
				    deadY + 70 > p.y && deadY < p.y + p.height) {
					overlap = true;
					break;
				}
			}
			if (!overlap) {
				deadImages.emplace_back(deadX, deadY, 75, 70, PLATFORM_DEAD);
				deadPlaced++;

				// 在致死图片旁边生成一块木板（同行，左右随机选一侧）
				int plankW = 70, plankH = 12;
				int plankGap = 8;
				bool placeLeft = (rand() % 2 == 0);
				int plankX, plankY = deadY;
				if (placeLeft)
					plankX = deadX - plankGap - plankW;
				else
					plankX = deadX + 75 + plankGap;

				if (plankX < 5) plankX = 5;
				if (plankX + plankW > SCREEN_WIDTH - 5)
					plankX = SCREEN_WIDTH - 5 - plankW;

				bool plankOverlap = false;
				for (const auto& p : platforms) {
					if (plankX + plankW > p.x && plankX < p.x + p.width &&
					    plankY + plankH > p.y && plankY < p.y + p.height) {
						plankOverlap = true;
						break;
					}
				}
				if (!plankOverlap && plankX >= 5 && plankX + plankW <= SCREEN_WIDTH - 5) {
					platforms.emplace_back(plankX, plankY, plankW, plankH, PLATFORM_NORMAL);
				}
			}
		}
	}
}

// =====================================================================
//  TriggerGameOver  -  unified settlement point
// =====================================================================

void GameManager::TriggerGameOver() {
	state = GAME_OVER;
	mciSendString(L"pause bgm", NULL, 0, NULL);
	if (score > highScore) {
		highScore = score;
		SaveHighScore();
	}
}

// =====================================================================
//  Per-frame Logic
// =====================================================================

void GameManager::UpdateCamera() {
	// bg4 铺满屏幕后，锁定镜头，不再跟随玩家移动
	if (cameraTop <= BG4_START_Y) {
		return;
	}

	// Keep player in upper-middle portion of screen.
	// Camera only pans UP, never down.
	float targetTop = player.y - SCREEN_HEIGHT * 0.4f;
	if ((float)cameraTop > targetTop) {
		cameraTop = (int)targetTop;
	}
}

void GameManager::CheckCollisions() {
	for (auto& p : platforms) {
		if (p.CheckLanding((int)player.x, (int)player.y,
		                    Player::RADIUS, (int)player.vy)) {
			// 只有下落时碰到木板才起跳
			if (player.vy > 0) {
				if (p.type == PLATFORM_JUMP)
					player.SuperJump();
				else
					player.Jump();
			}

			// 只有第一次踩到时才加分
			if (!p.scored) {
				int heightDiff = lastPlatformY - p.y;
				if (heightDiff > 0) {
					score += heightDiff;
				}
				lastPlatformY = p.y;
				p.scored = true;
				platformsJumped++;
			}
			break;
		}
	}

	// 检查是否撞到悬空的危险图片（任何方向碰到都会死）
	for (auto& d : deadImages) {
		if (d.CheckCollision((int)player.x, (int)player.y, Player::RADIUS)) {
			TriggerGameOver();
			return;
		}
	}
}

void GameManager::CheckGameOver() {
	int screenY = player.GetScreenY(cameraTop);
	if (screenY > SCREEN_HEIGHT) {
		fallFrames++;
		if (fallFrames > 20) {   // ~330 ms grace below screen
			TriggerGameOver();
		}
	} else {
		fallFrames = 0;
	}
}

// =====================================================================
//  CheckWin  -  检查是否通关（到达 bg4 最上方台阶）
// =====================================================================

void GameManager::CheckWin() {
	if (winTriggered) return;

	// 玩家碰到世界顶端即通关
	if (player.y < WIN_PLATFORM_Y) {
		winTriggered = true;
		TriggerWin();
	}
}

// =====================================================================
//  TriggerWin  -  触发通关胜利
// =====================================================================

void GameManager::TriggerWin() {
	state = WIN;
	mciSendString(L"pause bgm", NULL, 0, NULL);
	if (score > highScore) {
		highScore = score;
		SaveHighScore();
	}
}

void GameManager::GeneratePlatforms() {
	// 镜头到达 bg4（最顶层）后不再生成新平台，让顶端木板固定
	if (cameraTop <= BG4_START_Y) return;

	int wMin, wMax, gMin, gMax;
	GetStageParams(wMin, wMax, gMin, gMax);

	// 统计当前阶段已有的悬空危险图片
	int currentY = lastGeneratedY;
	int stageY;
	if (currentY >= BG2_START_Y) stageY = 1;
	else if (currentY >= BG3_START_Y) stageY = 2;
	else stageY = 3;

	int stageBaseY = -BG_HEIGHT * (stageY - 1);
	int stageTopY = stageBaseY - BG_HEIGHT;
	int stageBottomY = stageBaseY + 100;

	int currentDeadCount = 0;
	int currentJumpCount = 0;
	for (const auto& d : deadImages) {
		if (d.type == PLATFORM_DEAD && d.y > stageTopY && d.y < stageBottomY) {
			currentDeadCount++;
		}
	}
	for (const auto& p : platforms) {
		if (p.type == PLATFORM_JUMP && p.y > stageTopY && p.y < stageBottomY) {
			currentJumpCount++;
		}
	}

	// Keep ~400 px of platforms above the visible area at all times
	while (lastGeneratedY > cameraTop - 400) {
		int w   = wMin + rand() % (wMax - wMin + 1);
		int px  = 5 + rand() % (SCREEN_WIDTH - w - 10);
		int gap = gMin + rand() % (gMax - gMin + 1);
		lastGeneratedY -= gap;

		// 生成木板（第1阶段1个跳跃板，第2阶段2个跳跃板）
		PlatformType pt = PLATFORM_NORMAL;
		if (stageY == 1 && currentJumpCount < 1) {
			pt = PLATFORM_JUMP;
			currentJumpCount++;
		} else if (stageY == 2 && currentJumpCount < 2) {
			pt = PLATFORM_JUMP;
			currentJumpCount++;
		}
		platforms.emplace_back(px, lastGeneratedY, w, 12, pt);

		// 生成悬空危险图片（在两个木板中间，不重叠）
		int newY = lastGeneratedY;
		int newStageY;
		if (newY >= BG2_START_Y) newStageY = 1;
		else if (newY >= BG3_START_Y) newStageY = 2;
		else newStageY = 3;

		if (newStageY != stageY) {
			stageY = newStageY;
			stageBaseY = -BG_HEIGHT * (stageY - 1);
			stageTopY = stageBaseY - BG_HEIGHT;
			stageBottomY = stageBaseY + 100;
			currentDeadCount = 0;
			currentJumpCount = 0;
		}

		// 在第2、3阶段各生成1个悬空危险图片
		if ((stageY == 2 || stageY == 3) && currentDeadCount < 1) {
			for (int retry = 0; retry < 200; retry++) {
				int deadImgX = 5 + rand() % (SCREEN_WIDTH - 80);
				int deadImgY = stageTopY + 30 + rand() % (BG_HEIGHT - 100);

				bool overlap = false;
				for (const auto& p : platforms) {
					if (deadImgX + 75 > p.x && deadImgX < p.x + p.width &&
					    deadImgY + 70 > p.y && deadImgY < p.y + p.height) {
						overlap = true;
						break;
					}
				}
				if (!overlap) {
					deadImages.emplace_back(deadImgX, deadImgY, 75, 70, PLATFORM_DEAD);
					currentDeadCount++;

					// 在致死图片旁边生成一块木板（同行，左右随机选一侧）
					int plankW = 70, plankH = 12;
					int plankGap = 8;
					bool placeLeft = (rand() % 2 == 0);
					int plankX, plankY = deadImgY;
					if (placeLeft)
						plankX = deadImgX - plankGap - plankW;
					else
						plankX = deadImgX + 75 + plankGap;

					if (plankX < 5) plankX = 5;
					if (plankX + plankW > SCREEN_WIDTH - 5)
						plankX = SCREEN_WIDTH - 5 - plankW;

					bool plankOverlap = false;
					for (const auto& p : platforms) {
						if (plankX + plankW > p.x && plankX < p.x + p.width &&
						    plankY + plankH > p.y && plankY < p.y + p.height) {
							plankOverlap = true;
							break;
						}
					}
					if (!plankOverlap && plankX >= 5 && plankX + plankW <= SCREEN_WIDTH - 5) {
						platforms.emplace_back(plankX, plankY, plankW, plankH, PLATFORM_NORMAL);
					}
				}
			}
		}
	}
}

void GameManager::CleanPlatforms() {
	for (auto it = platforms.begin(); it != platforms.end(); ) {
		if (it->y - cameraTop > SCREEN_HEIGHT + 100) {
			it = platforms.erase(it);
		} else {
			++it;
		}
	}

	// 同时清理超出屏幕的危险图片
	for (auto it = deadImages.begin(); it != deadImages.end(); ) {
		if (it->y - cameraTop > SCREEN_HEIGHT + 100) {
			it = deadImages.erase(it);
		} else {
			++it;
		}
	}
}

// =====================================================================
//  Stage & Scoring
// =====================================================================

int GameManager::GetStage() const {
	if (platformsJumped < 10) return 1;
	if (platformsJumped < 25) return 2;
	return 3;
}

int GameManager::GetScorePerPlatform() const {
	switch (GetStage()) {
	case 1: return 20;
	case 2: return 40;
	case 3: return 60;
	default: return 20;
	}
}

void GameManager::GetStageParams(int& wMin, int& wMax,
                                  int& gMin, int& gMax) const {
	// 木板统一宽度70，间距统一约100（三阶段从多到少）
	switch (GetStage()) {
	case 1:
		// Stage 1: 间距小=数量多
		wMin = 70; wMax = 70;
		gMin = 60; gMax = 60;
		break;
	case 2:
		// Stage 2: 间距中等
		wMin = 70; wMax = 70;
		gMin = 75; gMax = 75;
		break;
	case 3:
	default:
		// Stage 3: 间距大=数量少
		wMin = 70; wMax = 70;
		gMin = 75; gMax = 75;
		break;
	}
}

// =====================================================================
//  Drawing helpers
// =====================================================================

void GameManager::DrawBackground() {
	int idx = (-cameraTop) / BG_HEIGHT;
	int offset = (-cameraTop) % BG_HEIGHT;
	if (offset < 0) offset = -offset;

	// 到了最后一张bg4，锁定不滚动
	if (idx >= BG_COUNT - 1) {
		idx = BG_COUNT - 1;
		offset = 0;
		if (bgImages[idx].getwidth() > 0) {
			putimage(0, 0, &bgImages[idx]);
		}
		return;
	}

	// 屏幕上方：idx+1的图片
	if (offset > 0) {
		putimage(0, 0, SCREEN_WIDTH, offset, &bgImages[idx + 1], 0, BG_HEIGHT - offset);
	}

	// 屏幕下方：idx的图片
	if (bgImages[idx].getwidth() > 0) {
		putimage(0, offset, SCREEN_WIDTH, BG_HEIGHT - offset, &bgImages[idx], 0, 0);
	}
}

void GameManager::DrawButton(const RECT& r, const wchar_t* text,
	COLORREF bgColor, COLORREF textColor) const {

	// 圆角半径（数字越大越圆，10～15最好看）
	int radius = 12;

	// 原木色边框
	setfillcolor(RGB(150, 110, 65));
	setlinecolor(RGB(80, 55, 35));
	fillroundrect(r.left, r.top, r.right, r.bottom, radius, radius);

	// 原木顶部高光（浅木亮色）
	setlinecolor(RGB(180, 150, 120));
	line(r.left + 2, r.top + 2, r.right - 2, r.top + 2);

	// Centre text
	settextstyle(25, 0, L"PingFang SC");
	settextcolor(textColor);
	setbkmode(TRANSPARENT);
	int tw = textwidth(text);
	int th = textheight(text);
	int tx = r.left + (r.right - r.left - tw) / 2;
	int ty = r.top + (r.bottom - r.top - th) / 2;
	outtextxy(tx, ty, text);
}

// =====================================================================
//  Start Menu Screen
// =====================================================================

void GameManager::DrawStartMenu() const
{
	// 1. 背景图（铺满）
	if (startBgLoaded) {
		putimage(0, 0, &startBg);
	}
	else {
		setfillcolor(RGB(255, 200, 100));
		solidrectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	// 2. 开始按钮（在标题和图片中间）
	// 开始按钮 - 图片版（旧按钮不显示）
	// 开始按钮 - 图片版（旧按钮不显示）
	// 开始按钮（透明图片）
	IMAGE startBtnImg;
	loadimage(&startBtnImg, L"assets\\ui\\start_btn.png",
		startBtn.right - startBtn.left,
		startBtn.bottom - startBtn.top, true);
	putimage_alpha(startBtn.left, startBtn.top, &startBtnImg);

	// 3. 最高分（底部居中）
	//wchar_t buf[64];
	//swprintf_s(buf, 64, L"最高分：%d", highScore);
	//settextstyle(20, 0, L"PingFang SC");

	//int hw = textwidth(buf);
	//int y = SCREEN_HEIGHT - 35;


	//settextcolor(RGB(0, 180, 0));
	//outtextxy((SCREEN_WIDTH - hw) / 2, y, buf);
}
// =====================================================================
//  In-game HUD
// =====================================================================

void GameManager::DrawPlayingUI() const {

	// 暂停按钮 透明图片
	IMAGE pauseBtnImg;
	loadimage(&pauseBtnImg, L"assets\\ui\\pause_btn.png",
		pauseBtn.right - pauseBtn.left,
		pauseBtn.bottom - pauseBtn.top, true);
	putimage_alpha(pauseBtn.left, pauseBtn.top, &pauseBtnImg);

	// HUD text (top-left) with a subtle shadow for readability
	setbkmode(TRANSPARENT);
	settextstyle(14, 0, L"Arial");

	wchar_t buf[64];
	int stage = GetStage();

	// Shadow first
	//settextcolor(RGB(0, 0, 0));
	//swprintf_s(buf, 64, L"Score: %d", score);
	//outtextxy(11, 11, buf);
	//swprintf_s(buf, 64, L"Stage: %d", stage);
	//outtextxy(11, 31, buf);
	//swprintf_s(buf, 64, L"Jumps: %d", platformsJumped);
	//outtextxy(11, 51, buf);
	//swprintf_s(buf, 64, L"Best:  %d", (highScore > score) ? highScore : score);
	//outtextxy(11, 71, buf);

	// Then bright text on top
	//settextcolor(RGB(255, 255, 255));
	//swprintf_s(buf, 64, L"Score: %d", score);
	//outtextxy(10, 10, buf);
	//swprintf_s(buf, 64, L"Stage: %d", stage);
	//outtextxy(10, 30, buf);
	//swprintf_s(buf, 64, L"Jumps: %d", platformsJumped);
	//outtextxy(10, 50, buf);
	//swprintf_s(buf, 64, L"Best:  %d", (highScore > score) ? highScore : score);
	//outtextxy(10, 70, buf);
}

// =====================================================================
//  Pause Overlay
// =====================================================================

void GameManager::DrawPauseOverlay() const {
	// Dim the screen with alternating lines
	for (int y = 0; y < SCREEN_HEIGHT; y += 3) {
		setlinecolor(RGB(0, 0, 0));
		line(0, y, SCREEN_WIDTH, y);
	}

	// Title
	settextstyle(34, 0, L"Arial");
	settextcolor(RGB(255, 255, 255));
	setbkmode(TRANSPARENT);
	const wchar_t* pause = L"PAUSED";
	int pw = textwidth(pause);
	outtextxy((SCREEN_WIDTH - pw) / 2, SCREEN_HEIGHT / 2 - 40, pause);

	// Hint
	settextstyle(15, 0, L"Arial");
	const wchar_t* hint = L"Click pause button (top-right) to resume";
	int hw = textwidth(hint);
	outtextxy((SCREEN_WIDTH - hw) / 2, SCREEN_HEIGHT / 2 + 15, hint);
}

// =====================================================================
//  Game Over Screen
// =====================================================================



void GameManager::DrawGameOverScreen() const {

	// ===================== 【和开始页完全一样的图片加载】 =====================
	IMAGE gameOverBg;
	bool gameOverBgLoaded = false;

	// 路径和开始页一样：assets\\ui\\xxx.png
	if (loadimage(&gameOverBg, L"assets\\ui\\gameover.png",
		SCREEN_WIDTH, SCREEN_HEIGHT, true) == 0)
	{
		gameOverBgLoaded = true;
	}

	// 画背景（和开始页完全相同逻辑）
	if (gameOverBgLoaded)
	{
		putimage(0, 0, &gameOverBg);
	}
	else
	{
		setfillcolor(RGB(30, 30, 45));
		solidrectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	// ========================================================================


	settextstyle(34, 0, L"Arial");

	if (score >= highScore && score > 0) {
		settextstyle(20, 0, L"Arial");
		settextcolor(RGB(255, 255, 60));
		const wchar_t* nhs = L"*** NEW HIGH SCORE! ***";
		int nhw = textwidth(nhs);
		outtextxy((SCREEN_WIDTH - nhw) / 2, 300, nhs);
	}

	// 再玩一次按钮 - 图片版（旧按钮不显示）
	// 再玩一次按钮 - 图片版（旧按钮不显示）
	// 再玩一次按钮（透明图片）
	IMAGE restartBtnImg;
	loadimage(&restartBtnImg, L"assets\\ui\\restart_btn.png",
		restartBtn.right - restartBtn.left,
		restartBtn.bottom - restartBtn.top, true);
	putimage_alpha(restartBtn.left, restartBtn.top, &restartBtnImg);
}

// =====================================================================
//  Win Screen  -  通关胜利界面
// =====================================================================

void GameManager::DrawWinScreen() const {
	// 通关背景图
	IMAGE endBg;
	bool endBgLoaded = false;
	if (loadimage(&endBg, L"assets\\ui\\end.png",
		SCREEN_WIDTH, SCREEN_HEIGHT, true) == 0)
	{
		endBgLoaded = true;
	}

	if (endBgLoaded)
	{
		putimage(0, 0, &endBg);
	}
	else
	{
		setfillcolor(RGB(30, 30, 45));
		solidrectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	// 再玩一次按钮（透明图片，底端居中）
	IMAGE endBtnImg;
	loadimage(&endBtnImg, L"assets\\ui\\end_btn.png",
		endBtn.right - endBtn.left,
		endBtn.bottom - endBtn.top, true);
	putimage_alpha(endBtn.left, endBtn.top, &endBtnImg);
}

// =====================================================================
//  High-score persistence
// =====================================================================

void GameManager::LoadHighScore() {
	std::ifstream file("highscore.txt");
	if (file.is_open()) {
		file >> highScore;
		file.close();
	} else {
		highScore = 0;
	}
}

void GameManager::SaveHighScore() {
	std::ofstream file("highscore.txt");
	if (file.is_open()) {
		file << highScore;
		file.close();
	}
}
