#include "Player.h"
#pragma comment(lib, "msimg32.lib")

// 【新加】透明PNG专用函数，只加在这里就行
inline void putimage_alpha(int x, int y, const IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

Player::Player() : x(0), y(0), vy(0), screenW(400), facingRight(true), onGround(false) {
	loadimage(&img_up_left, L"assets/player/upl.png", UP_WIDTH, UP_HEIGHT);
	loadimage(&img_up_right, L"assets/player/upr.png", UP_WIDTH, UP_HEIGHT);
	loadimage(&img_down_left, L"assets/player/downl.png", DOWN_WIDTH, DOWN_HEIGHT);
	loadimage(&img_down_right, L"assets/player/downr.png", DOWN_WIDTH, DOWN_HEIGHT);
}

void Player::Reset(float startX, float startY, int screenWidth) {
	x = startX;
	y = startY;
	vy = 0.0f;
	screenW = screenWidth;
	facingRight = true;
	onGround = false;
}

void Player::Update(bool leftKey, bool rightKey) {
	// 左右移动（与跳跃无关）
	if (leftKey)  x -= MOVE_SPEED;
	if (rightKey) x += MOVE_SPEED;
	if (leftKey)  facingRight = false;
	if (rightKey) facingRight = true;
	if (x < -RADIUS)               x = (float)screenW + RADIUS;
	if (x > (float)screenW + RADIUS) x = -RADIUS;

	// 物理
	vy += GRAVITY;
	y += vy;
	// onGround 由 CheckCollisions 设置
}

void Player::Jump() {
	vy = JUMP_VELOCITY;
}

void Player::SuperJump() {
	vy = SUPER_JUMP_VEL;
}

// 【全部改成透明绘制，去掉 SRCAND】
void Player::Draw(int cameraTop) const {
	int screenX = (int)x;
	int screenY = (int)(y - cameraTop);

	if (vy < 0) {
		if (facingRight) {
			putimage_alpha(screenX - UP_WIDTH / 2, screenY - UP_HEIGHT / 2, &img_up_right);
		}
		else {
			putimage_alpha(screenX - UP_WIDTH / 2, screenY - UP_HEIGHT / 2, &img_up_left);
		}
	}
	else {
		if (facingRight) {
			putimage_alpha(screenX - DOWN_WIDTH / 2, screenY - DOWN_HEIGHT / 2, &img_down_right);
		}
		else {
			putimage_alpha(screenX - DOWN_WIDTH / 2, screenY - DOWN_HEIGHT / 2, &img_down_left);
		}
	}
}

int Player::GetScreenY(int cameraTop) const {
	return (int)(y - cameraTop);
}

/*#include "Player.h"

Player::Player() : x(0), y(0), vy(0), screenW(400), facingRight(true), onGround(false) {
	loadimage(&img_up_left, L"assets/player/upl.png", UP_WIDTH, UP_HEIGHT);
	loadimage(&img_up_right, L"assets/player/upr.png", UP_WIDTH, UP_HEIGHT);
	loadimage(&img_down_left, L"assets/player/downl.png", DOWN_WIDTH, DOWN_HEIGHT);
	loadimage(&img_down_right, L"assets/player/downr.png", DOWN_WIDTH, DOWN_HEIGHT);
}

inline void putimage_alpha(int x, int y, IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

void Player::Reset(float startX, float startY, int screenWidth) {
	x       = startX;
	y       = startY;
	vy      = 0.0f;
	screenW = screenWidth;
	facingRight = true;
	onGround = false;
}

void Player::Update(bool leftKey, bool rightKey) {
	// 左右移动（与跳跃无关）
	if (leftKey)  x -= MOVE_SPEED;
	if (rightKey) x += MOVE_SPEED;
	if (leftKey)  facingRight = false;
	if (rightKey) facingRight = true;
	if (x < -RADIUS)               x = (float)screenW + RADIUS;
	if (x > (float)screenW + RADIUS) x = -RADIUS;

	// 物理
	vy += GRAVITY;
	y  += vy;
	// onGround 由 CheckCollisions 设置
}

void Player::Jump() {
	vy = JUMP_VELOCITY;
}

void Player::Draw(int cameraTop) const {
	int screenX = (int)x;
	int screenY = (int)(y - cameraTop);

	if (vy < 0) {
		if (facingRight) {
			putimage(screenX - UP_WIDTH/2, screenY - UP_HEIGHT/2, UP_WIDTH, UP_HEIGHT, &img_up_right, 0, 0);
		} else {
			putimage(screenX - UP_WIDTH/2, screenY - UP_HEIGHT/2, UP_WIDTH, UP_HEIGHT, &img_up_left, 0, 0);
		}
	} else {
		if (facingRight) {
			putimage(screenX - DOWN_WIDTH/2, screenY - DOWN_HEIGHT/2, DOWN_WIDTH, DOWN_HEIGHT, &img_down_right, 0, 0);
		} else {
			putimage(screenX - DOWN_WIDTH/2, screenY - DOWN_HEIGHT/2, DOWN_WIDTH, DOWN_HEIGHT, &img_down_left, 0, 0);
		}
	}
}

int Player::GetScreenY(int cameraTop) const {
	return (int)(y - cameraTop);
}*/