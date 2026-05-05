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
	GAME_OVER
};

class GameManager {
public:
	static constexpr int SCREEN_WIDTH  = 400;
	static constexpr int SCREEN_HEIGHT = 650;

	GameState              state;
	Player                 player;
	std::vector<Platform>  platforms;
	int cameraTop;           // World Y at top of screen (only moves up)
	int score;
	int highScore;
	int platformsJumped;     // Total successful platform landings

	// Button hit-test rectangles (screen coordinates)
	RECT startBtn;
	RECT pauseBtn;
	RECT restartBtn;

	GameManager();
	~GameManager();

	void Run();

private:
private:

	int lastGeneratedY;      // Highest world Y where we last placed a platform
	int fallFrames;          // Consecutive frames player has been below screen

	IMAGE startBg;           // Start-menu background image
	bool  startBgLoaded;     // Whether the image loaded successfully


	// --- Lifecycle ---
	void ResetGame();
	void TriggerGameOver();

	// --- Per-frame logic ---
	void UpdateCamera();
	void CheckCollisions();
	void CheckGameOver();
	void GeneratePlatforms();
	void CleanPlatforms();

	// --- Stage & scoring ---
	int  GetStage()              const;
	int  GetScorePerPlatform()   const;
	void GetStageParams(int& wMin, int& wMax, int& gMin, int& gMax) const;

	// --- Drawing ---
	void DrawBackground()        const;
	void DrawStartMenu()         const;
	void DrawPlayingUI()         const;
	void DrawPauseOverlay()      const;
	void DrawGameOverScreen()    const;
	void DrawButton(const RECT& r, const wchar_t* text,
	                COLORREF bgColor, COLORREF textColor) const;

	// --- High-score persistence ---
	void LoadHighScore();
	void SaveHighScore();
};
