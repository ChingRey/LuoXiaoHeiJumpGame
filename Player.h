#pragma once
#include <graphics.h>

class Player {
public:
	static constexpr int   RADIUS        = 18;
	static constexpr float GRAVITY       = 0.45f;
	static constexpr float JUMP_VELOCITY = -9.5f;
	static constexpr float MOVE_SPEED    = 5.0f;

	float x, y;       // World-space center position
	float vy;          // Vertical velocity (positive = down)
	int   screenW;     // Screen width for wrapping

	Player();
	void Reset (float startX, float startY, int screenWidth);
	void Update(bool leftKey, bool rightKey);
	void Jump  ();
	void Draw  (int cameraTop) const;
	int  GetScreenY(int cameraTop) const;
};
