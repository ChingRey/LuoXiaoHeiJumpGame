# 罗小黑战记·森林双跳游戏

## 项目概述
这是一个使用 C++ 和 EasyX 图形库开发的经典双跳（Double Jump）风格跳跃闯关游戏，融合了《罗小黑战记》治愈系森林世界观的设计元素。

## 核心玩法

### 基础机制
- **双跳系统**：角色在地面/平台上可连续触发 2 次跳跃（第一次起跳、第二次空中补跳），离开平台后仅能触发 1 次空中跳跃
- **平台闯关**：平台从屏幕顶部向下移动，玩家需要通过左右移动和跳跃踩在平台上，不断向上攀爬
- **避免坠落**：如果掉出屏幕底部，游戏结束
- **难度递进**：随着分数提升，平台会变窄、间距变大、下落速度加快

### 控制方式
| 控制 | 方式 |
|------|------|
| **左移** | ← 方向键 |
| **右移** | → 方向键 |
| **跳跃** | 空格键 |
| **暂停** | 点击右上角暂停按钮 |

### 游戏结束条件
- 角色掉落出屏幕底部时游戏结束
- 系统自动保存最高记录到本地文件 `highscore.txt`

## 项目结构

```
LuoXiaoHeiJumpGame/
├── main.cpp                  # 游戏入口
├── Role.h / Role.cpp         # 角色类：控制、跳跃、碰撞、绘制
├── Platform.h / Platform.cpp # 平台类：生成、移动、绘制
├── GameManager.h / GameManager.cpp  # 游戏管理器：主循环、逻辑、UI
└── LuoXiaoHeiJumpGame.vcxproj       # Visual Studio 项目文件
```

## 代码模块说明

### 1. Role 类（角色模块）
**核心职责**：管理玩家角色的位置、速度、跳跃、重力、碰撞、绘制

**关键成员**：
- `m_x, m_y`：角色中心坐标
- `m_vx, m_vy`：水平和竖直速度
- `m_jumpCount`：当前可跳跃次数（0=不可跳，1=可跳1次，2=可跳2次）
- `m_isOnGround`：是否在地面/平台上

**关键函数**：
- `Update()`：更新物理状态（重力、速度、位置）
- `Move(direction)`：处理水平移动输入
- `Jump()`：执行跳跃逻辑
- `CheckLandOnPlatform(...)`：碰撞检测，与平台顶部接触时重置跳跃次数
- `Draw()`：绘制角色（目前为黑色圆形+简化五官，后期替换为罗小黑立绘）

**物理参数**（可调）：
```cpp
m_moveAccel = 0.8;        // 移动加速度
m_maxMoveSpeed = 8;       // 最大水平速度
m_jumpPower = -18;        // 跳跃力度（负值表示向上）
m_gravity = 0.6;          // 重力加速度
```

### 2. Platform 类（平台模块）
**核心职责**：管理平台的位置、大小、移动、绘制

**关键成员**：
- `m_x, m_y`：平台中心坐标
- `m_width, m_height`：平台尺寸
- `m_speed`：下落速度

**关键函数**：
- `Update()`：平台向下移动
- `Draw()`：绘制平台（目前为深棕色矩形，后期替换为森林树干/石板纹理）
- `IsOutOfScreen(...)`：检查平台是否超出屏幕

### 3. GameManager 类（游戏管理器模块）
**核心职责**：主游戏循环、输入处理、物理更新、碰撞检测、UI绘制、分数计算

**游戏状态枚举**：
```cpp
enum GameState { PLAYING, PAUSED, GAME_OVER };
```

**关键函数**：
- `Init()`：初始化游戏窗口、加载最高记录、生成初始平台
- `Run()`：主游戏循环（60帧/秒）
- `HandleInput()`：处理键盘和鼠标输入
- `Update()`：更新角色、平台、碰撞、分数
- `Draw()`：绘制所有游戏元素
- `GeneratePlatform()`：根据难度生成新平台
- `GetDifficultyMultiplier()`：计算难度倍数（0.0-1.0）
- `DrawUI()`：绘制分数、时间、暂停按钮
- `DrawPauseScreen()`：绘制暂停界面
- `DrawGameOverScreen()`：绘制游戏结束界面
- `DrawBackground()`：绘制森林背景

**难度参数**（随分数变化）：
```cpp
// 难度倍数 = min(score / 500, 1.0)
// 平台宽度 = m_platformWidth * (1.0 - 难度倍数 * 0.3)
// 背景色 = 随难度加深
```

## 编译与运行

### 前置要求
1. **Visual Studio 2019** 或更高版本
2. **EasyX 图形库 2024** 版本
   - 下载地址：http://www.easyx.cn
   - 安装时选择集成到 Visual Studio

### 编译步骤
1. 打开 Visual Studio
2. 创建新项目或打开现有项目文件夹
3. 将所有 `.cpp` 和 `.h` 文件放在项目目录
4. 在项目属性中配置 EasyX 包含目录和库目录
5. 点击 **构建** > **生成解决方案** 或按 `Ctrl+Shift+B`
6. 点击 **调试** > **开始执行（不调试）** 或按 `Ctrl+F5` 运行

### 快速编译命令（PowerShell）
```powershell
# 进入项目目录
cd LuoXiaoHeiJumpGame

# 使用 cl.exe 编译（需要配置 EasyX 路径）
cl.exe /EHsc Role.cpp Platform.cpp GameManager.cpp main.cpp /link graphics.lib user32.lib
```

## 素材替换指南

### 1. 替换罗小黑角色 (Role.cpp)
在 `Draw()` 函数中找到注释：
```cpp
// ========= 下方替换为罗小黑立绘 =========
```

**替换方式**：
```cpp
// 方法1：加载图片并绘制
IMAGE img;
loadimage(&img, _T("luoxiaohei.png"));
putimage(m_x - 15, m_y - 15, &img);

// 方法2：绘制更复杂的图形组合
```

### 2. 替换平台纹理 (Platform.cpp)
在 `Draw()` 函数中找到注释：
```cpp
// ========= 下方替换为森林树干/石板纹理 =========
```

**替换方式**：
```cpp
// 加载平台纹理
IMAGE platformImg;
loadimage(&platformImg, _T("platform.png"));
putimage(m_x - m_width/2, m_y - m_height/2, &platformImg);
```

### 3. 替换背景场景 (GameManager.cpp)
在 `DrawBackground()` 函数中找到注释：
```cpp
// ========= 下方替换为森林背景纹理 =========
```

**替换方式**：
```cpp
// 绘制背景图片
IMAGE bg;
loadimage(&bg, _T("background.png"), m_screenWidth, m_screenHeight);
putimage(0, m_backgroundScrollY, &bg);
putimage(0, m_backgroundScrollY - m_screenHeight, &bg);
```

## 难度调整指南

### 调整平台难度
在 `GameManager::GeneratePlatform()` 中修改：
```cpp
// 随机间隙
double gap = m_platformGapMin + rand() % (m_platformGapMax - m_platformGapMin);
// 可修改 m_platformGapMin 和 m_platformGapMax 的初始值

// 平台宽度随难度变化
double adjustedWidth = m_platformWidth * (1.0 - difficultyMultiplier * 0.3);
// 可修改系数 0.3 (越大，难度增长越快)
```

### 调整角色物理
在 `Role::Role()` 构造函数中修改：
```cpp
m_moveAccel = 0.8;       // 移动加速度（越小越灵敏）
m_maxMoveSpeed = 8;      // 最大水平速度
m_jumpPower = -18;       // 跳跃力度（绝对值越大，跳得越高）
m_gravity = 0.6;         // 重力（越大下落越快）
```

### 调整游戏难度阈值
在 `GameManager::GetDifficultyMultiplier()` 中修改：
```cpp
double multiplier = std::min(1.0, m_score / 500.0);
// 改为 m_score / 300.0 则难度增长更快
```

## 游戏特色

### 罗小黑主题元素
- 🎨 **治愈系森林风格**：清新的绿色背景，营造妖灵森林氛围
- 🐱 **角色形象**：黑色圆形+黄色五官，简化版罗小黑设计
- 📝 **主题文案**：游戏结束时提示"小黑掉下去啦！再试一次吧～"
- 🌳 **场景装饰**：背景树木、云雾，增强沉浸感
- 📈 **难度递进**：背景色随难度变深，呼应森林深处冒险的设定

### 游戏机制亮点
- ✨ **双跳系统**：经典双跳游戏的核心机制，增加跳跃自由度
- 🎯 **精准碰撞检测**：只有从平台顶部踩到才算落地
- 📊 **分数系统**：每秒+1分，踩中平台有额外奖励（可扩展）
- 💾 **本地存储**：最高记录自动保存和加载
- ⏸️ **暂停功能**：随时暂停游戏
- 🎮 **键盘+鼠标双控制**：支持方向键和按钮点击

## 扩展建议

### 立即可实现的扩展
1. **添加道具系统**：
   - 减速道具：减慢平台下落速度
   - 加分道具：增加当前分数
   - 护盾道具：死亡时复活一次

2. **添加敌人系统**：
   - 妖灵敌人：移动的障碍物，碰撞即死亡
   - Boss关卡：在特定分数达到时出现

3. **角色皮肤**：
   - 罗小黑原皮肤（可通过积分解锁）
   - 无限皮肤
   - 阿根皮肤

4. **音效系统**：
   - 跳跃音效
   - 踩中平台音效
   - 游戏结束音效
   - 背景音乐

### 代码预留接口
- `Role::Draw()` 中有素材替换注释
- `Platform::Draw()` 中有纹理替换注释
- `GameManager::DrawBackground()` 中有背景替换注释
- `GameManager::GeneratePlatform()` 支持难度参数灵活调整

## 常见问题

### Q: 如何修改窗口大小？
A: 在 `main.cpp` 的 `GameManager game(800, 600)` 中修改参数即可

### Q: 游戏运行缓慢？
A: 调整帧率控制，在 `GameManager::Run()` 中修改 `Sleep(16)` 的值

### Q: 最高记录保存在哪里？
A: 项目目录下的 `highscore.txt` 文件

### Q: 如何调整难度？
A: 详见上方"难度调整指南"部分

## 技术细节

### 物理系统
- 使用简单的欧拉积分模拟重力和速度
- 速度平滑过渡实现流畅的加速/减速
- 屏幕环绕效果使角色从左边移出时从右边出现

### 碰撞检测
- 矩形-圆形碰撞：检查角色与平台的接触
- 只有从上方踩到才算落地（`m_vy > 0`）
- 防止卡顶或穿透的处理

### 渲染
- 使用 EasyX 批量绘制 (`BeginBatchDraw` / `EndBatchDraw`)
- 背景、平台、角色、UI 分层绘制
- 支持半透明效果（暂停/结束遮罩）

### 事件系统
- 键盘异步输入（`GetAsyncKeyState`）
- 鼠标事件处理（`MouseHit`、`GetMouseMsg`）
- 暂停按钮碰撞检测

## 许可证
本项目为学习用途开发，仅供参考。

## 联系与反馈
如有问题或建议，欢迎提出！

---

**最后更新**：2024年  
**开发环境**：Visual Studio 2019+ / EasyX 2024  
**语言**：C++11/14/17  
