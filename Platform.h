#pragma once
#include <graphics.h>

enum PlatformType {
	PLATFORM_NORMAL = 0
	// Reserved for future expansion:
	// PLATFORM_MOVING, PLATFORM_BREAKABLE, PLATFORM_BOOST, PLATFORM_SPRING
};

class Platform {
public:
	int x, y;
	int width, height;
	PlatformType type;
	bool scored;       // Already scored this landing? (prevents double-counting)

	Platform() : x(0), y(0), width(60), height(12), type(PLATFORM_NORMAL), scored(false) {}

	Platform(int x_, int y_, int w_, int h_, PlatformType t = PLATFORM_NORMAL)
		: x(x_), y(y_), width(w_), height(h_), type(t), scored(false) {}

	// Draw platform relative to camera
	void Draw(int cameraTop) const {
		int screenY = y - cameraTop;
		if (screenY < -20 || screenY > 700) return;

		// Drop shadow
		setfillcolor(RGB(55, 110, 55));
		solidrectangle(x + 2, screenY + 2, x + width + 2, screenY + height + 2);

		// Main body
		setfillcolor(RGB(95, 175, 95));
		setlinecolor(RGB(65, 145, 65));
		fillrectangle(x, screenY, x + width, screenY + height);

		// Top highlight for slight 3D effect
		setfillcolor(RGB(135, 205, 135));
		solidrectangle(x + 2, screenY + 1, x + width - 2, screenY + 3);
	}

	// Check if player lands on this platform this frame.
	// playerVy must be positive (falling) and the player's foot must
	// have crossed the platform top from above.
	bool CheckLanding(int playerX, int playerY, int playerRadius, int playerVy) const {
		if (playerVy <= 0) return false;
		if (scored) return false;

		int footY     = playerY + playerRadius;
		int prevFootY = footY - playerVy;
		int left      = playerX - playerRadius;
		int right     = playerX + playerRadius;

		// Foot crossed platform surface from above
		if (footY >= y && prevFootY <= y + 8) {
			// Horizontal overlap (with small margin so edges count)
			if (right > x + 5 && left < x + width - 5) {
				return true;
			}
		}
		return false;
	}
};
