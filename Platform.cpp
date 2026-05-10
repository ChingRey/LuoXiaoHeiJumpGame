#include "Platform.h"
#pragma comment(lib,"msimg32.lib")

// 透明贴图函数 → 只写在这里，不写在.h里！
inline void putimage_alpha(int x, int y, const IMAGE* img)
{
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(
		GetImageHDC(NULL), x, y, w, h,
		GetImageHDC((IMAGE*)img), 0, 0, w, h,
		{ AC_SRC_OVER, 0, 255, AC_SRC_ALPHA }
	);
}

// 静态成员初始化
IMAGE Platform::deathImgSmall;
bool Platform::deathImgLoaded = false;
IMAGE Platform::jumpImg;
bool Platform::jumpImgLoaded = false;

// 加载死亡图片（直接缩放到45x45）
void Platform::LoadDeathImage() {
	if (!deathImgLoaded) {
		// 直接加载并缩放到45x45
		loadimage(&deathImgSmall, L"assets\\ui\\death.png", 75, 70);
		deathImgLoaded = true;
	}
}

// 加载跳跃板图片（缩放到60x30，居中放在木板上）
void Platform::LoadJumpImage() {
	if (!jumpImgLoaded) {
		loadimage(&jumpImg, L"assets\\ui\\bluepan.png", 55, 55);
		jumpImgLoaded = true;
	}
}