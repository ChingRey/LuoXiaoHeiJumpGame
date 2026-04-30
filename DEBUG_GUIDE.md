# 罗小黑战记·森林双跳游戏 - 开发调试指南

## 调试注释和关键位置

### 1. 角色双跳机制调试

**文件**: `Role.cpp`

```cpp
// ======== 关键调试点 ========
// Role::Jump() - 第43行
void Role::Jump()
{
    if (m_jumpCount > 0)
    {
        m_vy = m_jumpPower;  // 修改 m_jumpPower 调整跳跃高度
        m_jumpCount--;       // 这里递减跳跃次数
        m_isOnGround = false;
    }
    // 调试技巧：输出 m_jumpCount 值确保双跳逻辑正确
    // printf("Jump Count: %d\n", m_jumpCount);
}

// Role::Update() - 第19行
// 检查这段逻辑确保离地时跳跃次数从2变为1
if (!m_isOnGround && m_jumpCount == 2)
{
    m_jumpCount = 1;  // 这里设定空中跳跃次数为1
}
```

**调试步骤**：
1. 如果无法进行第二段跳跃，检查 `m_jumpCount` 是否正确递减
2. 如果在地面上无法跳跃，检查 `m_isOnGround` 标志是否被正确设置
3. 添加调试输出到 `Draw()` 函数显示当前跳跃次数

### 2. 碰撞检测调试

**文件**: `Role.cpp`

```cpp
// ======== 关键碰撞检测 ========
// Role::CheckLandOnPlatform() - 第58行
bool Role::CheckLandOnPlatform(double platformY, double platformX, 
                               double platformWidth, double platformHeight)
{
    // 水平范围检查
    if (m_x < platformX - platformWidth / 2 || m_x > platformX + platformWidth / 2)
        return false;  // 未在平台水平范围内

    // 竖直范围检查 - 只有从上方落下时才算踩到
    if (m_y + m_radius >= platformY - platformHeight / 2 &&
        m_y + m_radius <= platformY - platformHeight / 2 + 10 &&
        m_vy > 0)  // 关键：必须是向下运动
    {
        // 如果无法踩中平台，检查上面四个条件中哪个不满足
        return true;
    }
    return false;
}
```

**调试步骤**：
1. 如果经常穿过平台，增大碰撞盒：将 `+10` 改为 `+15`
2. 如果踩不到平台，检查平台Y坐标和角色Y坐标的计算
3. 如果平台侧面无法安全通过，检查水平范围的 `+/- platformWidth/2` 值

**添加可视化调试**（在 `GameManager::Draw()` 中）：
```cpp
// 调试：绘制角色碰撞盒
setlinecolor(RED);
circle(m_player->GetX(), m_player->GetY(), m_player->GetRadius());

// 调试：绘制平台碰撞盒
for (auto p : m_platforms)
{
    setlinecolor(BLUE);
    rectangle(p->GetX() - p->GetWidth()/2, p->GetY() - p->GetHeight()/2,
              p->GetX() + p->GetWidth()/2, p->GetY() + p->GetHeight()/2);
}
```

### 3. 难度递进调试

**文件**: `GameManager.cpp`

```cpp
// ======== 难度倍数计算 ========
// GameManager::GetDifficultyMultiplier() - 第225行
double GameManager::GetDifficultyMultiplier()
{
    // 难度倍数 = min(score / 500.0, 1.0)
    // 在分数达到500时，难度达到最高
    // 修改 500 可改变难度增长速度：
    // - 越小，难度增长越快
    // - 越大，难度增长越慢
    double multiplier = std::min(1.0, m_score / 500.0);
    return multiplier;
}

// GameManager::GeneratePlatform() - 第196行
// 获取难度倍数
double difficultyMultiplier = GetDifficultyMultiplier();

// 平台宽度随难度减小
// 修改系数 0.3：
// - 越大，平台变窄速度越快
// - 越小，变化越缓慢
double adjustedWidth = m_platformWidth * (1.0 - difficultyMultiplier * 0.3);
adjustedWidth = std::max(40.0, adjustedWidth);  // 最小宽度保证可玩性
```

**调试技巧**：
```cpp
// 在 DrawUI() 中显示当前难度倍数
double diff = GetDifficultyMultiplier();
std::wstring diffText = L"难度: " + std::to_wstring((int)(diff * 100)) + L"%";
outtextxy(m_screenWidth/2 - 80, 20, diffText.c_str());
```

### 4. 物理参数调试

**文件**: `Role.h` (构造函数初始化列表)

```cpp
// ======== 物理参数 ========
Role::Role(double x, double y)
    : ... ,
      m_moveAccel(0.8),         // 水平加速度（越小越敏感）
      m_moveDecel(0.6),         // 水平减速度
      m_maxMoveSpeed(8),        // 最大水平速度
      m_jumpPower(-18),         // 跳跃初速度（负值向上，绝对值越大跳越高）
      m_gravity(0.6),           // 重力加速度（越大下落越快）
      ...
{
}
```

**快速调整方案**：

| 问题 | 调整方案 |
|------|---------|
| 角色移动太快 | 减小 `m_maxMoveSpeed` (例如8→5) |
| 角色移动太慢 | 增大 `m_maxMoveSpeed` (例如8→12) |
| 角色不灵敏 | 增大 `m_moveAccel` (例如0.8→1.2) |
| 跳得不够高 | 增大 `m_jumpPower` 绝对值 (例如-18→-25) |
| 下落太快 | 减小 `m_gravity` (例如0.6→0.4) |
| 空中停留太久 | 增大 `m_gravity` (例如0.6→0.8) |

### 5. 帧率和性能调试

**文件**: `GameManager.cpp` - `Run()` 函数

```cpp
void GameManager::Run()
{
    BeginBatchDraw();

    while (true)
    {
        HandleInput();

        if (m_gameState == PLAYING)
            Update();

        Draw();

        FlushBatchDraw();
        Sleep(16);  // 16ms = 60fps

        // 性能调试：
        // 如果帧数不稳定，改为 Sleep(33) = 30fps
        // 如果希望更流畅，改为 Sleep(10) = 100fps（需要更好的硬件）
    }

    EndBatchDraw();
}
```

## 常见BUG及修复

### BUG 1: 角色无法进行第二段跳跃

**症状**: 按空格只能跳一次，即使在地面上也无法连续跳跃

**检查清单**:
1. ✅ 检查 `Role::CheckLandOnPlatform()` 是否正确调用
2. ✅ 检查 `m_jumpCount` 是否在落地时重置为 2
3. ✅ 检查输入处理中的 `spacePressed` 标志逻辑

**修复代码**:
```cpp
// HandleInput() 中的跳跃输入处理
if (GetAsyncKeyState(VK_SPACE) & 0x8000)
{
    static bool spacePressed = false;
    if (!spacePressed)
    {
        m_player->Jump();
        spacePressed = true;
    }
    // BUG: 这里需要在释放时重置
}
else
{
    spacePressed = false;  // 空格释放时重置标志
}
```

### BUG 2: 角色卡在平台上无法继续跳跃

**症状**: 踩中平台后无法继续跳跃或跳跃无效

**原因**: 碰撞检测范围过大导致角色多次触发落地

**修复**:
```cpp
// 在 Role::Update() 中添加防护
bool Role::CheckLandOnPlatform(double platformY, double platformX, 
                               double platformWidth, double platformHeight)
{
    // ... 碰撞检测逻辑 ...

    if (m_y + m_radius >= platformY - platformHeight / 2 &&
        m_y + m_radius <= platformY - platformHeight / 2 + 10 &&
        m_vy > 0 &&
        !m_isOnGround)  // 添加这个条件防止重复触发
    {
        m_vy = 0;
        m_y = platformY - platformHeight / 2 - m_radius;
        m_jumpCount = 2;
        m_isOnGround = true;
        return true;
    }
    return false;
}
```

### BUG 3: 游戏卡顿或掉帧

**症状**: 游戏运行不流畅，帧数波动

**原因**: 可能是平台生成过多或内存泄漏

**检查步骤**:
```cpp
// 在 GameManager::Update() 中监测平台数量
static int maxPlatforms = 0;
if (m_platforms.size() > maxPlatforms)
{
    maxPlatforms = m_platforms.size();
    // 输出到调试器
    printf("Max platforms: %zu\n", m_platforms.size());
}

// 如果平台数超过100，需要优化生成策略
if (m_platforms.size() > 100)
{
    // 移除屏幕外的平台
}
```

### BUG 4: 分数计算不正确

**症状**: 分数增长不稳定或显示错误

**检查点**:
```cpp
// GameManager::Update() 中的分数更新
if (m_frameCount % 60 == 0)  // 每秒（60帧）
{
    m_gameTime += 1.0;
    m_score += 1;  // 每秒加1分

    // 可以添加踩中平台的加分：
    // m_score += 5;  // 踩中一个平台额外加5分
}
```

## 性能优化建议

### 1. 减少绘制调用

**优化前**:
```cpp
void Draw()
{
    // 多次清屏
    cleardevice();
    DrawBackground();
    // ... 绘制每个平台
    for (auto p : m_platforms) p->Draw();
    // ...
}
```

**优化后** (使用批量绘制):
```cpp
BeginBatchDraw();
while (gameRunning)
{
    cleardevice();
    // ... 绘制所有元素
    FlushBatchDraw();
    Sleep(16);
}
EndBatchDraw();  // 这已在代码中实现
```

### 2. 优化碰撞检测

**当前方案**: O(n) - 检查每个平台

**未来优化**: 使用网格划分或四叉树 (当平台数量 > 500 时考虑)

```cpp
// 简单网格优化（伪代码）
struct Grid {
    vector<Platform*> cells[10][10];

    void AddPlatform(Platform* p) {
        int gridX = (p->GetX() / screenWidth) * 10;
        int gridY = (p->GetY() / screenHeight) * 10;
        cells[gridX][gridY].push_back(p);
    }

    vector<Platform*> GetNearby(Role* player) {
        int gridX = (player->GetX() / screenWidth) * 10;
        int gridY = (player->GetY() / screenHeight) * 10;
        // 返回同网格及相邻网格的平台
    }
};
```

### 3. 内存管理

**当前实现**:
```cpp
// 正确的内存释放
~GameManager()
{
    if (m_player) delete m_player;
    for (auto p : m_platforms)
        delete p;
    m_platforms.clear();
}
```

**改进建议**: 使用智能指针
```cpp
// 未来：使用 unique_ptr 自动管理
std::unique_ptr<Role> m_player;
std::vector<std::unique_ptr<Platform>> m_platforms;
```

## 单元测试建议

### 测试1: 双跳机制

```cpp
void TestDoubleJump()
{
    Role player(400, 500);

    // 初始状态：应该可以跳跃
    assert(player.m_jumpCount == 2);

    // 第一次跳跃
    player.Jump();
    assert(player.m_jumpCount == 1);
    assert(player.GetVelocityY() < 0);  // 向上

    // 第二次跳跃（在空中）
    player.Jump();
    assert(player.m_jumpCount == 0);

    // 落地后重置
    player.CheckLandOnPlatform(600, 400, 80, 15);
    assert(player.m_jumpCount == 2);
}
```

### 测试2: 碰撞检测

```cpp
void TestCollision()
{
    Role player(400, 300);
    Platform platform(400, 400, 80, 15, 3);

    // 角色从平台上方落下
    player.m_vy = 5;  // 向下
    bool result = player.CheckLandOnPlatform(
        platform.GetY(), 
        platform.GetX(), 
        platform.GetWidth(), 
        platform.GetHeight()
    );

    assert(result == true);  // 应该踩中
    assert(player.m_jumpCount == 2);  // 重置跳跃
}
```

## 性能基准

在标准硬件上（Intel i5, 8GB RAM）的预期表现：

| 指标 | 目标值 |
|------|--------|
| 帧率 | 60 FPS |
| 平台数量 | < 50 |
| 内存占用 | < 50 MB |
| CPU 使用率 | < 10% |

---

**更新日期**: 2024年  
**维护者**: GitHub Copilot
q