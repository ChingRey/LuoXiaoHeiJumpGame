#include <graphics.h>
#include "GameManager.h"

int main()
{
    // 创建游戏管理器（800x600窗口）
    GameManager game(800, 600);

    // 初始化游戏
    game.Init();

    // 运行游戏
    game.Run();

    return 0;
}
