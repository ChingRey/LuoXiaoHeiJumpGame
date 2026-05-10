#pragma once
#include <graphics.h>
inline void putimage_alpha(int x, int y, const IMAGE* img);

enum PlatformType {
	PLATFORM_NORMAL = 0,
	PLATFORM_DEAD = 1,
	PLATFORM_JUMP = 2
};

class Platform {
public:
	int x, y;
	int width, height;
	PlatformType type;
	bool scored;

	static IMAGE deathImgSmall;  // 预缩放好的45x45图片
	static bool deathImgLoaded;
	static IMAGE jumpImg;        // 跳跃板图片
	static bool jumpImgLoaded;

	Platform()
		: x(0), y(0), width(60), height(12), type(PLATFORM_NORMAL), scored(false) {
	}

	Platform(int x_, int y_, int w_, int h_, PlatformType t = PLATFORM_NORMAL)
		: x(x_), y(y_), width(w_), height(h_), type(t), scored(false) {
	}

	static void LoadDeathImage();
	static void LoadJumpImage();

	void Draw(int cameraTop) const {
		// --------------------------
		// 第一步：先算在屏幕的位置
		// --------------------------
		int screenY = y - cameraTop;

		// --------------------------
		// 第二步：只对普通木板画绿色木板
		// --------------------------
		if (type == PLATFORM_NORMAL || type == PLATFORM_JUMP) {
			// 黑色底板
			setfillcolor(RGB(30, 30, 30));
			// 红边框
			setlinecolor(RGB(30, 30, 30));
			fillroundrect(x, screenY, x + width, screenY + height, 10, 10);

			// 跳跃板：在木板上方居中显示蓝色跳跃图片
			if (type == PLATFORM_JUMP && jumpImgLoaded) {
				int imgW = jumpImg.getwidth();
				int imgH = jumpImg.getheight();
				int imgX = x + (width - imgW) / 2;
				int imgY = screenY - imgH;  // 图片底部贴在木板顶部
				putimage_alpha(imgX, imgY, &jumpImg);
			}
		}
		else if (type == PLATFORM_DEAD && deathImgLoaded) {
			putimage_alpha(x, screenY, &deathImgSmall);
		}
	}

	bool CheckLanding(int playerX, int playerY, int playerRadius, int playerVy) const {
		if (playerVy <= 0) return false;

		int footY = playerY + playerRadius;
		int prevFootY = footY - playerVy;
		int left = playerX - playerRadius;
		int right = playerX + playerRadius;

		if (footY >= y && prevFootY <= y + 8) {
			if (right > x + 5 && left < x + width - 5) {
				return true;
			}
		}
		return false;
	}

	// 检测是否碰到危险图片（任何方向碰撞）
	bool CheckCollision(int playerX, int playerY, int playerRadius) const {
		if (type != PLATFORM_DEAD) return false;

		// 死亡图片碰撞框向内缩 15px，边缘蹭到不算，必须踩进来才死
		const int margin = 15;
		int imgLeft   = x + margin;
		int imgRight  = x + width - margin;
		int imgTop    = y + margin;
		int imgBottom = y + height - margin;
		if (imgLeft >= imgRight || imgTop >= imgBottom) return false;

		int left   = playerX - playerRadius;
		int right  = playerX + playerRadius;
		int top    = playerY - playerRadius;
		int bottom = playerY + playerRadius;

		if (right > imgLeft && left < imgRight && bottom > imgTop && top < imgBottom) {
			return true;
		}
		return false;
	}
};