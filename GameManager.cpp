#include "GameManager.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cwchar>

// =====================================================================
//  Constructor / Destructor
// =====================================================================

GameManager::GameManager()
	: state(START_MENU), cameraTop(0)
	, score(0), highScore(0), platformsJumped(0)
	, lastGeneratedY(0), fallFrames(0), startBgLoaded(false)
{
	srand((unsigned)time(nullptr));

	// Define button rectangles (screen coords)
	// Start button at bottom, above footer area
	startBtn   = { 125, 250, 275, 300  };
	pauseBtn   = { SCREEN_WIDTH - 55, 10, SCREEN_WIDTH - 5, 40 };
	restartBtn = { SCREEN_WIDTH / 2 - 70, 410, SCREEN_WIDTH / 2 + 70, 455 };

	LoadHighScore();
	// Only load background for start menu (small image), not for gameplay
	if (loadimage(&startBg, L"assets\\ui\\start_bg_2.png",
		SCREEN_WIDTH, SCREEN_HEIGHT, true) == 0) {
		startBgLoaded = true;
	}
	
}

GameManager::~GameManager() {}

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
				}
				break;

			case PAUSED:
				if (mx >= pauseBtn.left  && mx <= pauseBtn.right &&
				    my >= pauseBtn.top   && my <= pauseBtn.bottom) {
					state = PLAYING;
				}
				break;

			case GAME_OVER:
				if (mx >= restartBtn.left  && mx <= restartBtn.right &&
				    my >= restartBtn.top   && my <= restartBtn.bottom) {
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
			player.Draw(cameraTop);
			DrawPlayingUI();
			break;

		case PAUSED:
			for (auto& p : platforms) p.Draw(cameraTop);
			player.Draw(cameraTop);
			DrawPlayingUI();
			DrawPauseOverlay();
			break;

		case GAME_OVER:
			DrawGameOverScreen();
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
	player.Reset(
		(float)(SCREEN_WIDTH / 2),
		(float)(SCREEN_HEIGHT - 150),
		SCREEN_WIDTH
	);
	cameraTop       = 0;
	score           = 0;
	platformsJumped = 0;
	fallFrames      = 0;
	platforms.clear();

	// Safe starter platform right under the player's feet
	int safeX = SCREEN_WIDTH / 2 - 45;
	int safeY = (int)player.y + Player::RADIUS + 5;
	platforms.emplace_back(safeX, safeY, 90, 12);

	// Pre-fill platforms upwards well past the visible area
	lastGeneratedY = (int)player.y - 30;
	int wMin, wMax, gMin, gMax;
	GetStageParams(wMin, wMax, gMin, gMax);

	while (lastGeneratedY > -400) {
		int w   = wMin + rand() % (wMax - wMin + 1);
		int px  = 5 + rand() % (SCREEN_WIDTH - w - 10);
		int gap = gMin + rand() % (gMax - gMin + 1);
		lastGeneratedY -= gap;
		platforms.emplace_back(px, lastGeneratedY, w, 12);
	}
}

// =====================================================================
//  TriggerGameOver  -  unified settlement point
// =====================================================================

void GameManager::TriggerGameOver() {
	state = GAME_OVER;
	if (score > highScore) {
		highScore = score;
		SaveHighScore();
	}
}

// =====================================================================
//  Per-frame Logic
// =====================================================================

void GameManager::UpdateCamera() {
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
			player.Jump();
			p.scored = true;
			score += GetScorePerPlatform();
			platformsJumped++;
			break;   // One landing per frame is enough
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

void GameManager::GeneratePlatforms() {
	int wMin, wMax, gMin, gMax;
	GetStageParams(wMin, wMax, gMin, gMax);

	// Keep ~400 px of platforms above the visible area at all times
	while (lastGeneratedY > cameraTop - 400) {
		int w   = wMin + rand() % (wMax - wMin + 1);
		int px  = 5 + rand() % (SCREEN_WIDTH - w - 10);
		int gap = gMin + rand() % (gMax - gMin + 1);
		lastGeneratedY -= gap;
		platforms.emplace_back(px, lastGeneratedY, w, 12);
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
	// Jump height ≈ 100 px (9.5^2 / 0.9).
	// Gaps increase evenly across stages; the limit is that max gap
	// always stays slightly below jump height.
	switch (GetStage()) {
	case 1:
		// Stage 1 ( 0- 9): comfortable — easier for beginners
		wMin = 85;  wMax = 110;
		gMin = 28;  gMax = 42;    // smaller gap ~35 px
		break;
	case 2:
		// Stage 2 (10-24): moderate — spacing widens
		wMin = 65;  wMax = 88;
		gMin = 56;  gMax = 74;    // midpoint ~65 px, ~65% of jump
		break;
	case 3:
	default:
		// Stage 3 (25+  ): challenging — gap just under jump ceiling
		wMin = 45;  wMax = 68;
		gMin = 70;  gMax = 92;    // midpoint ~81 px, ~81% of jump (< 100)
		break;
	}
}

// =====================================================================
//  Drawing helpers
// =====================================================================

void GameManager::DrawBackground() const {
	// Sky-to-forest gradient
	for (int i = 0; i < SCREEN_HEIGHT; i++) {
		int r = 160 + i * 40  / SCREEN_HEIGHT;
		int g = 210 + i * 30  / SCREEN_HEIGHT;
		int b = 230 - i * 50  / SCREEN_HEIGHT;
		setlinecolor(RGB(r, g, b));
		line(0, i, SCREEN_WIDTH, i);
	}
}

void GameManager::DrawButton(const RECT& r, const wchar_t* text,
                              COLORREF bgColor, COLORREF textColor) const {



	// 圆角半径（数字越大越圆，10～15最好看）
	int radius = 12;

	// Body → 改成圆角矩形
	setfillcolor(bgColor);
	setlinecolor(RGB(40, 100, 40));
	fillroundrect(r.left, r.top, r.right, r.bottom, radius, radius); // 圆角

	// Highlight line at top（保留你原来的高光）
	setlinecolor(RGB(150, 200, 150));
	line(r.left + 2, r.top + 2, r.right - 2, r.top + 2);

	// Centre text - 完全保留你原来的字体样式
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
	DrawButton(startBtn, L"开始游戏", RGB(80, 160, 80), RGB(255, 255, 255));

	// 3. 最高分（底部居中）
	wchar_t buf[64];
	swprintf_s(buf, 64, L"最高分：%d", highScore);
	settextstyle(20, 0, L"PingFang SC");

	int hw = textwidth(buf);
	int y = SCREEN_HEIGHT - 35;


	settextcolor(RGB(0, 180, 0));
	outtextxy((SCREEN_WIDTH - hw) / 2, y, buf);
}
// =====================================================================
//  In-game HUD
// =====================================================================

void GameManager::DrawPlayingUI() const {
	// Pause button (top-right)
	DrawButton(pauseBtn, L"II", RGB(190, 190, 190), RGB(30, 30, 30));

	// HUD text (top-left) with a subtle shadow for readability
	setbkmode(TRANSPARENT);
	settextstyle(14, 0, L"Arial");

	wchar_t buf[64];
	int stage = GetStage();

	// Shadow first
	settextcolor(RGB(0, 0, 0));
	swprintf_s(buf, 64, L"Score: %d", score);
	outtextxy(11, 11, buf);
	swprintf_s(buf, 64, L"Stage: %d", stage);
	outtextxy(11, 31, buf);
	swprintf_s(buf, 64, L"Jumps: %d", platformsJumped);
	outtextxy(11, 51, buf);
	swprintf_s(buf, 64, L"Best:  %d", (highScore > score) ? highScore : score);
	outtextxy(11, 71, buf);

	// Then bright text on top
	settextcolor(RGB(255, 255, 255));
	swprintf_s(buf, 64, L"Score: %d", score);
	outtextxy(10, 10, buf);
	swprintf_s(buf, 64, L"Stage: %d", stage);
	outtextxy(10, 30, buf);
	swprintf_s(buf, 64, L"Jumps: %d", platformsJumped);
	outtextxy(10, 50, buf);
	swprintf_s(buf, 64, L"Best:  %d", (highScore > score) ? highScore : score);
	outtextxy(10, 70, buf);
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
	// Dark backdrop
	for (int y = 0; y < SCREEN_HEIGHT; y += 3) {
		setlinecolor(RGB(18, 18, 38));
		line(0, y, SCREEN_WIDTH, y);
	}

	// "GAME OVER" heading
	settextstyle(34, 0, L"Arial");
	settextcolor(RGB(255, 70, 70));
	setbkmode(TRANSPARENT);
	const wchar_t* go = L"GAME OVER";
	int gow = textwidth(go);
	outtextxy((SCREEN_WIDTH - gow) / 2, 60, go);

	// Stats
	settextstyle(17, 0, L"Arial");
	settextcolor(RGB(255, 255, 255));

	wchar_t buf[64];
	swprintf_s(buf, 64, L"Score:  %d", score);
	int bw = textwidth(buf);
	outtextxy((SCREEN_WIDTH - bw) / 2, 145, buf);

	swprintf_s(buf, 64, L"High Score:  %d", highScore);
	bw = textwidth(buf);
	outtextxy((SCREEN_WIDTH - bw) / 2, 180, buf);

	int stage = GetStage();
	swprintf_s(buf, 64, L"Stage Reached:  %d", stage);
	bw = textwidth(buf);
	outtextxy((SCREEN_WIDTH - bw) / 2, 215, buf);

	swprintf_s(buf, 64, L"Platforms Jumped:  %d", platformsJumped);
	bw = textwidth(buf);
	outtextxy((SCREEN_WIDTH - bw) / 2, 250, buf);

	// New high score badge
	if (score >= highScore && score > 0) {
		settextstyle(20, 0, L"Arial");
		settextcolor(RGB(255, 255, 60));
		const wchar_t* nhs = L"*** NEW HIGH SCORE! ***";
		int nhw = textwidth(nhs);
		outtextxy((SCREEN_WIDTH - nhw) / 2, 300, nhs);
	}

	// Restart button
	DrawButton(restartBtn, L"Play Again", RGB(80, 180, 80), RGB(255, 255, 255));
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
