#pragma once
#include <graphics.h>
#include <vector>
#include <string>
#include "Role.h"
#include "Platform.h"

// 游戏管理器
class GameManager
{
public:
    // 构造函数
    GameManager(int screenWidth, int screenHeight);

    // 析构函数
    ~GameManager();

    // 初始化游戏
    void Init();

    // 主游戏循环
    void Run();

private:
    // 游戏状态枚举
    enum GameState
    {
        PLAYING,   // 游戏进行中
        PAUSED,    // 游戏暂停
        GAME_OVER  // 游戏结束
    };

    // 处理输入
    void HandleInput();

    // 更新游戏逻辑
    void Update();

    // 绘制游戏画面
    void Draw();

    // 生成新平台
    void GeneratePlatform();
    void GeneratePlatformBelow(double startY);

    // 获取难度参数（随分数增加）
    double GetDifficultyMultiplier();

    // 绘制UI界面
    void DrawUI();

    // 绘制暂停界面
    void DrawPauseScreen();

    // 绘制游戏结束界面
    void DrawGameOverScreen();

    // 绘制背景（森林场景）
    void DrawBackground();

    // 检查暂停按钮是否被点击
    bool CheckPauseButtonClick(int x, int y);

    // 检查"重新开始"按钮是否被点击
    bool CheckRestartButtonClick(int x, int y);

    // 加载最高记录
    void LoadHighScore();

    // 保存最高记录
    void SaveHighScore();

    // ===================== 成员变量 =====================
    int m_screenWidth;
    int m_screenHeight;

    Role* m_player;
    std::vector<Platform*> m_platforms;

    GameState m_gameState;

    int m_score;           // 当前分数
    int m_highScore;       // 最高记录
    double m_gameTime;     // 游戏时间（秒）
    int m_frameCount;      // 帧计数器

    double m_platformSpeed;        // 平台下落速度
    double m_platformWidth;        // 平台宽度
    double m_platformHeight;       // 平台高度
    int m_platformGapMin;          // 平台间隙最小值
    int m_platformGapMax;          // 平台间隙最大值
    double m_lastPlatformY;        // 最后一个平台的Y坐标（用于生成下一个）
    double m_lastPlatformBelowY;   // 下方最后一个平台的Y坐标

    // 摄像机系统
    double m_cameraY;              // 摄像机在世界坐标中的Y位置
    int m_fixedPlayerScreenY;      // 玩家在屏幕上的固定Y坐标

    // 按钮位置
    int m_pauseButtonX;
    int m_pauseButtonY;
    int m_pauseButtonRadius;

    // 方向键按钮位置
    int m_leftButtonX;
    int m_leftButtonY;
    int m_rightButtonX;
    int m_rightButtonY;
    int m_buttonRadius;

    // 背景滚动
    double m_backgroundScrollY;

    // 玩家跳跃状态（用于背景滚动控制）
    bool m_wasJumping;            // 上一帧是否在跳跃
    double m_scrollOffset;        // 累积的滚动偏移量
};
