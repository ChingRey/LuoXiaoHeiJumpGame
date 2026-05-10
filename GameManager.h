#pragma once
#define RGBA(r,g,b,a) ((COLORREF)(((BYTE)(a)<<24)|((BYTE)(r)<<16)|((BYTE)(g)<<8)|(BYTE)(b)))
#include <graphics.h>
#include <vector>
#include "Platform.h"
#include "Player.h"

enum GameState {
	START_MENU,
	PLAYING,
	PAUSED,
	GAME_OVER,
	WIN  // 通关胜利状态
};

class GameManager {
public:
	static constexpr int SCREEN_WIDTH  = 400;
	static constexpr int SCREEN_HEIGHT = 650;

	// 背景图片配置
	static constexpr int BG_COUNT = 4;
	static constexpr int BG_HEIGHT = 650;  // 每张背景显示的高度（屏幕高度）
	static constexpr int BG1_START_Y = 0;
	static constexpr int BG2_START_Y = -BG_HEIGHT;
	static constexpr int BG3_START_Y = -BG_HEIGHT * 2;
	static constexpr int BG4_START_Y = -BG_HEIGHT * 3;
	// bg4 最上方台阶的 Y 坐标（世界坐标，负值）
	static constexpr int WIN_PLATFORM_Y = -BG_HEIGHT * 3;

	GameState              state;
	Player                 player;
	std::vector<Platform>  platforms;
	std::vector<Platform>  deadImages;  // 悬空的危险图片，不贴木板
	int cameraTop;           // World Y at top of screen (only moves up)
	int score;
	int highScore;
	int platformsJumped;     // Total successful platform landings

	// Button hit-test rectangles (screen coordinates)
	RECT startBtn;
	RECT pauseBtn;
	RECT restartBtn;
	RECT endBtn;

	GameManager();
	~GameManager();

	void Run();

private:

	int lastGeneratedY;      // Highest world Y where we last placed a platform
	int fallFrames;          // Consecutive frames player has been below screen
	bool winTriggered;      // 是否已触发通关（防止重复触发）
	int lastPlatformY;      // 上一次踩到的平台y坐标（用于按高度差记分）

	IMAGE startBg;           // Start-menu background image
	bool  startBgLoaded;  // Whether the image loaded successfully

	// 游玩背景图片
	IMAGE bgImages[BG_COUNT];
	bool  bgLoaded[BG_COUNT];
	bool  bgCacheReady;  // 背景图片是否已加载
	bool  bgmLoaded;     // 背景音乐是否已加载


	// --- Lifecycle ---
	void ResetGame();
	void TriggerGameOver();
	void TriggerWin();      // 触发通关胜利

	// --- Per-frame logic ---
	void UpdateCamera();
	void CheckCollisions();
	void CheckGameOver();
	void CheckWin();        // 检查是否通关（到达 bg4 最上方台阶）
	void GeneratePlatforms();
	void CleanPlatforms();

	// --- Stage & scoring ---
	int  GetStage()              const;
	int  GetScorePerPlatform()   const;
	void GetStageParams(int& wMin, int& wMax, int& gMin, int& gMax) const;

	// --- Drawing ---
	void DrawBackground();
	void DrawStartMenu()         const;
	void DrawPlayingUI()         const;
	void DrawPauseOverlay()      const;
	void DrawGameOverScreen()    const;
	void DrawWinScreen()        const;  // 通关胜利界面
	void DrawButton(const RECT& r, const wchar_t* text,
	                COLORREF bgColor, COLORREF textColor) const;

	// --- High-score persistence ---
	void LoadHighScore();
	void SaveHighScore();
};
