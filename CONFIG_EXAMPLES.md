// 配置示例文件 - 用于快速调整游戏参数
// 复制下列参数到相应代码位置即可快速改变游戏特性

// ==================== 难度配置 ====================

// 配置1：简单模式
// 在 GameManager 构造函数中使用以下参数
// 平台移动速度
// const double m_platformSpeed = 2.0;
// 初始平台宽度
// const double m_platformWidth = 100;
// 平台高度
// const double m_platformHeight = 20;
// 平台间隙范围
// const int m_platformGapMin = 60;
// const int m_platformGapMax = 120;
// 难度增长阈值
// const double DIFFICULTY_THRESHOLD = 1000.0;  // 需要1000分才能达到最高难度

// 配置2：中等模式（默认）
// m_platformSpeed = 3.0;
// m_platformWidth = 80;
// m_platformHeight = 15;
// m_platformGapMin = 40;
// m_platformGapMax = 100;
// DIFFICULTY_THRESHOLD = 500.0;

// 配置3：困难模式
// m_platformSpeed = 4.5;
// m_platformWidth = 60;
// m_platformHeight = 12;
// m_platformGapMin = 30;
// m_platformGapMax = 80;
// DIFFICULTY_THRESHOLD = 300.0;

// 配置4：极难模式
// m_platformSpeed = 6.0;
// m_platformWidth = 50;
// m_platformHeight = 10;
// m_platformGapMin = 25;
// m_platformGapMax = 60;
// DIFFICULTY_THRESHOLD = 200.0;

// ==================== 角色物理配置 ====================

// 配置1：敏捷型（快速反应，容易操作）
// m_moveAccel = 1.2;       // 加速快
// m_moveDecel = 0.7;       // 减速慢
// m_maxMoveSpeed = 10;     // 移动速度快
// m_jumpPower = -22;       // 跳跃高
// m_gravity = 0.5;         // 下落慢

// 配置2：平衡型（默认）
// m_moveAccel = 0.8;
// m_moveDecel = 0.6;
// m_maxMoveSpeed = 8;
// m_jumpPower = -18;
// m_gravity = 0.6;

// 配置3：沉重型（需要预判，难度高）
// m_moveAccel = 0.5;       // 加速慢
// m_moveDecel = 0.5;       // 减速快
// m_maxMoveSpeed = 6;      // 移动速度慢
// m_jumpPower = -15;       // 跳跃低
// m_gravity = 0.8;         // 下落快

// 配置4：超级敏捷（竞技模式）
// m_moveAccel = 1.5;
// m_moveDecel = 0.8;
// m_maxMoveSpeed = 12;
// m_jumpPower = -25;
// m_gravity = 0.4;

// ==================== 窗口和UI配置 ====================

// 配置1：小窗口（移动设备模拟）
// initgraph(480, 800);  // 竖屏

// 配置2：标准窗口（默认）
// initgraph(800, 600);  // 横屏

// 配置3：大窗口（高分辨率）
// initgraph(1024, 768);

// 配置4：超宽屏
// initgraph(1280, 720);

// ==================== 视觉效果配置 ====================

// 变更背景颜色深度（在 DrawBackground 中）

// 配置1：浅绿色背景（轻松氛围）
// int greenBase = 200;  // 绿色基数越大越浅
// setfillcolor(RGB(150, greenBase, 120));

// 配置2：中等绿色背景（默认）
// int greenBase = 180 - (int)(30 * difficultyMultiplier);
// setfillcolor(RGB(150, greenBase, 120));

// 配置3：深绿色背景（压抑氛围）
// int greenBase = 150 - (int)(40 * difficultyMultiplier);
// setfillcolor(RGB(130, greenBase, 100));

// ==================== 分数系统配置 ====================

// 配置1：简单加分（只加时间分）
// if (m_frameCount % 60 == 0) {
//     m_gameTime += 1.0;
//     m_score += 1;  // 每秒加1分
// }

// 配置2：加强加分（踩中平台额外加分）
// 在 Update() 中记录上一帧的平台数量，比较当前帧
// if (踩中新平台) {
//     m_score += 5;  // 踩中平台额外加5分
// }

// 配置3：超级加分（时间+平台双重加分）
// if (m_frameCount % 60 == 0) {
//     m_gameTime += 1.0;
//     m_score += 2;  // 每秒加2分
// }
// if (踩中平台) {
//     m_score += 10;  // 踩中平台加10分
// }

// ==================== 暂停和UI按钮位置 ====================

// 配置1：标准按钮布局（默认）
// m_pauseButtonX = m_screenWidth - 50;
// m_pauseButtonY = 40;
// m_pauseButtonRadius = 20;
// m_leftButtonX = 50;
// m_leftButtonY = m_screenHeight - 50;
// m_rightButtonX = 110;
// m_rightButtonY = m_screenHeight - 50;

// 配置2：按钮放大（便于手机端点击）
// m_pauseButtonRadius = 30;
// m_buttonRadius = 35;

// 配置3：按钮缩小（桌面端）
// m_pauseButtonRadius = 15;
// m_buttonRadius = 20;

// ==================== 速度调试参数 ====================

// 帧率控制（在 Run() 中的 Sleep 值）

// 30 FPS（流畅）
// Sleep(33);

// 60 FPS（默认，推荐）
// Sleep(16);

// 120 FPS（超流畅，可能需要高性能硬件）
// Sleep(8);

// ==================== 快速配置应用示例 ====================

/*
// 要应用简单模式，修改 GameManager 构造函数如下：

GameManager::GameManager(int screenWidth, int screenHeight)
    : m_screenWidth(screenWidth), m_screenHeight(screenHeight),
      m_player(nullptr), m_gameState(GAME_OVER), m_score(0), m_highScore(0),
      m_gameTime(0), m_frameCount(0), m_platformSpeed(2.0),      // 简单模式
      m_platformWidth(100),                                       // 简单模式
      m_platformHeight(20),                                       // 简单模式
      m_platformGapMin(60),                                       // 简单模式
      m_platformGapMax(120),                                      // 简单模式
      m_lastPlatformY(screenHeight - 50),
      m_pauseButtonX(screenWidth - 50), m_pauseButtonY(40), m_pauseButtonRadius(20),
      m_leftButtonX(50), m_leftButtonY(screenHeight - 50),
      m_rightButtonX(110), m_rightButtonY(screenHeight - 50),
      m_buttonRadius(25), m_backgroundScrollY(0)
{
    srand(static_cast<unsigned>(time(nullptr)));
}

// 要应用敏捷型角色物理，修改 Role 构造函数如下：

Role::Role(double x, double y)
    : m_x(x), m_y(y), m_vx(0), m_vy(0), m_radius(15),
      m_moveAccel(1.2),         // 敏捷型
      m_moveDecel(0.7),         // 敏捷型
      m_maxMoveSpeed(10),       // 敏捷型
      m_jumpPower(-22),         // 敏捷型
      m_gravity(0.5),           // 敏捷型
      m_jumpCount(2), m_isOnGround(true), m_targetVx(0)
{
}
*/

// ==================== 调试输出配置 ====================

// 在任何函数中添加以下代码以输出调试信息

// 输出当前分数和难度倍数
// printf("Score: %d, Difficulty: %.2f\n", m_score, GetDifficultyMultiplier());

// 输出平台数量
// printf("Platforms: %zu\n", m_platforms.size());

// 输出角色位置和速度
// printf("Player: X=%.1f, Y=%.1f, VY=%.1f\n", m_player->GetX(), m_player->GetY(), m_player->GetVelocityY());

// 输出FPS（每1秒输出一次）
// static int fpsCounter = 0;
// static int lastTime = 0;
// int currentTime = GetTickCount();
// if (currentTime - lastTime >= 1000) {
//     printf("FPS: %d\n", fpsCounter);
//     fpsCounter = 0;
//     lastTime = currentTime;
// }
// fpsCounter++;

// ==================== 扩展功能预留 ====================

// 道具系统预留结构
/*
struct Prop
{
    double x, y;
    int type;  // 0=减速, 1=加分, 2=护盾
    double speed;
    bool active;

    void Update() { y += speed; }
    void Draw() { 
        // 绘制道具
    }
    bool CheckCollision(Role* player) { 
        // 碰撞检测
        return false;
    }
};

std::vector<Prop*> m_props;
*/

// 敌人系统预留结构
/*
struct Enemy
{
    double x, y;
    double vx, vy;
    double radius;

    void Update() { x += vx; y += vy; }
    void Draw() { 
        // 绘制敌人
    }
    bool CheckCollision(Role* player) { 
        // 碰撞检测
        return false;
    }
};

std::vector<Enemy*> m_enemies;
*/

// 粒子系统预留结构
/*
struct Particle
{
    double x, y;
    double vx, vy;
    int life;  // 剩余帧数

    void Update() { 
        x += vx; 
        y += vy; 
        life--;
    }
    void Draw() { 
        // 绘制粒子
    }
    bool IsAlive() { return life > 0; }
};

std::vector<Particle*> m_particles;
*/

// ==================== 配置文件使用说明 ====================

/*
1. 选择您想要的配置（简单/中等/困难等）
2. 复制相应配置的参数值
3. 粘贴到对应的源代码位置
4. 重新编译运行
5. 根据游戏感受进一步微调参数

所有参数都可以动态调整，建议先测试预设配置，再根据需求微调。

关键调整顺序建议：
1. 先调整难度（平台参数）
2. 再调整角色物理（操作感）
3. 最后微调视觉效果（颜色、大小）
*/
