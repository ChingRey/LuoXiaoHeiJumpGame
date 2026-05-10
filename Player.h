#pragma once
#include <graphics.h>

class Player {
public:
	static constexpr int   RADIUS        = 17;
	static constexpr float GRAVITY          = 2.2f;
	static constexpr float JUMP_VELOCITY    = -19.0f;
	static constexpr float SUPER_JUMP_VEL   = -38.0f;
	static constexpr float MOVE_SPEED       = 20.0f;

	// 缩放后的目标尺寸
	static constexpr int   UP_WIDTH    = 55;
	static constexpr int   UP_HEIGHT   = 55;
	static constexpr int   DOWN_WIDTH  = 55;
	static constexpr int   DOWN_HEIGHT = 44;

	// 图片（上升/下降 + 左/右朝向）
	IMAGE img_up_left;    // 上升向左
	IMAGE img_up_right;  // 上升向右
	IMAGE img_down_left;  // 下降向左
	IMAGE img_down_right; // 下降向右

	float x, y;       // World-space center position
	float vy;          // Vertical velocity (positive = down)
	int   screenW;     // Screen width for wrapping

	// 是否在木板上（可以起跳）
	bool onGround;

	// 设置/获取是否在地面
	void SetGround(bool g) { onGround = g; }
	bool IsOnGround() const { return onGround; }

	// 朝向：true=向右，false=向左
	bool facingRight;

	Player();
	void Reset (float startX, float startY, int screenWidth);
	void Update(bool leftKey, bool rightKey);
	void Jump  ();
	void SuperJump();
	void Draw  (int cameraTop) const;
	int  GetScreenY(int cameraTop) const;
};
