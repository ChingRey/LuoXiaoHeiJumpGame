#include "Player.h"

Player::Player() : x(0), y(0), vy(0), screenW(400) {}

void Player::Reset(float startX, float startY, int screenWidth) {
	x       = startX;
	y       = startY;
	vy      = 0.0f;
	screenW = screenWidth;
}

void Player::Update(bool leftKey, bool rightKey) {
	// Horizontal movement
	if (leftKey)  x -= MOVE_SPEED;
	if (rightKey) x += MOVE_SPEED;

	// Screen wrapping
	if (x < -RADIUS)               x = (float)screenW + RADIUS;
	if (x > (float)screenW + RADIUS) x = -RADIUS;

	// Gravity
	vy += GRAVITY;
	y  += vy;
}

void Player::Jump() {
	vy = JUMP_VELOCITY;
}

void Player::Draw(int cameraTop) const {
	int screenX = (int)x;
	int screenY = (int)(y - cameraTop);

	// --- Body: dark circle (Luo Xiao Hei placeholder) ---
	setfillcolor(RGB(45, 45, 45));
	setlinecolor(RGB(25, 25, 25));
	fillcircle(screenX, screenY, RADIUS);

	// --- Ears ---
	setfillcolor(RGB(50, 50, 50));
	setlinecolor(RGB(25, 25, 25));
	int earR = 7;
	fillcircle(screenX - 12, screenY - 15, earR);
	fillcircle(screenX + 12, screenY - 15, earR);

	// Inner ear
	setfillcolor(RGB(95, 65, 65));
	fillcircle(screenX - 12, screenY - 15, 3);
	fillcircle(screenX + 12, screenY - 15, 3);

	// --- Eyes (white) ---
	setfillcolor(RGB(255, 255, 255));
	int eyeR = 4;
	fillcircle(screenX - 6, screenY - 3, eyeR);
	fillcircle(screenX + 6, screenY - 3, eyeR);

	// --- Pupils ---
	setfillcolor(RGB(0, 0, 0));
	fillcircle(screenX - 5, screenY - 3, 2);
	fillcircle(screenX + 7, screenY - 3, 2);

	// --- Mouth ---
	setlinecolor(RGB(100, 100, 100));
	line(screenX - 3, screenY + 5, screenX,     screenY + 7);
	line(screenX,     screenY + 7, screenX + 3, screenY + 5);
}

int Player::GetScreenY(int cameraTop) const {
	return (int)(y - cameraTop);
}
