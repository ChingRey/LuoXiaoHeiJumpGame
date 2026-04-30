# 罗小黑战记·森林双跳游戏 - 快速启动指南

## ⚡ 5分钟快速上手

### 第一步：环境准备（2分钟）

#### 方案A：Visual Studio 2019/2022（推荐）

1. **安装 EasyX 图形库**
   - 官网下载：http://www.easyx.cn
   - 选择 EasyX 2024 最新版本
   - 安装时勾选"集成 Visual Studio"

2. **创建项目**
   ```
   文件 → 新建 → 项目 → Visual C++ → 空项目
   项目名称：LuoXiaoHeiJumpGame
   ```

3. **添加文件**
   - 将所有 `.cpp` 和 `.h` 文件复制到项目目录
   - 在 Visual Studio 中将这些文件添加到项目

#### 方案B：Code::Blocks

1. 安装 EasyX 库
2. 创建新项目，选择 C++ 控制台应用
3. 在项目选项中添加 EasyX 包含和库目录

#### 方案C：CLion

1. 确保已安装 EasyX
2. 配置 CMakeLists.txt 文件以支持 EasyX 库

### 第二步：编译（2分钟）

**Visual Studio**:
```
Ctrl + Shift + B  # 生成解决方案
```

**命令行编译**（PowerShell）:
```powershell
cd LuoXiaoHeiJumpGame
cl.exe /EHsc Role.cpp Platform.cpp GameManager.cpp main.cpp /link graphics.lib user32.lib
```

### 第三步：运行（1分钟）

**Visual Studio**:
```
Ctrl + F5  # 开始执行（不调试）
```

或直接双击生成的 `.exe` 文件

---

## 🎮 游戏操作

| 操作 | 按键 | 说明 |
|------|------|------|
| **向左移动** | ← | 持续按住向左加速 |
| **向右移动** | → | 持续按住向右加速 |
| **跳跃** | Space | 短按触发跳跃，可连续跳2次 |
| **暂停** | 点击右上角按钮 | 暂停所有游戏逻辑 |

---

## 📊 游戏界面

```
┌─────────────────────────────────────────────────────────┐
│ 分数: 150       时间: 10s  [暂停]                        │
│ 最高: 500                                               │
│                                                         │
│                    ⭕ 罗小黑                             │
│             (黑色圆形+黄色五官)                        │
│                                                         │
│          ▁▁▁ (平台1)      ▁▁▁ (平台2)                   │
│                                                         │
│                 ▁▁▁ (平台3)                             │
│                                                         │
│          ▁▁▁▁▁▁▁▁▁▁ (宽平台)                          │
│                                                         │
│  [◀] 左键      [▶] 右键                               │
└─────────────────────────────────────────────────────────┘
```

---

## 🔧 快速配置

### 修改难度

**文件**: `GameManager.cpp` 第 119-123 行

```cpp
// 当前配置（中等难度）
m_platformSpeed = 3;          // 平台下落速度
m_platformWidth = 80;         // 平台宽度
m_platformHeight = 15;        // 平台高度
m_platformGapMin = 40;        // 最小间隙
m_platformGapMax = 100;       // 最大间隙

// 简单难度建议值
// m_platformSpeed = 2;
// m_platformWidth = 100;
// m_platformGapMin = 60;
// m_platformGapMax = 120;

// 困难难度建议值
// m_platformSpeed = 4.5;
// m_platformWidth = 60;
// m_platformGapMin = 30;
// m_platformGapMax = 80;
```

修改后重新编译即可生效。

### 修改角色物理

**文件**: `Role.cpp` 第 10-18 行

```cpp
// 当前配置（平衡型）
m_moveAccel = 0.8;        // 移动加速度
m_maxMoveSpeed = 8;       // 最大移动速度
m_jumpPower = -18;        // 跳跃力度（负数表示向上）
m_gravity = 0.6;          // 重力加速度

// 敏捷型建议值（快速反应）
// m_moveAccel = 1.2;
// m_maxMoveSpeed = 10;
// m_jumpPower = -22;
// m_gravity = 0.5;

// 沉重型建议值（需要预判）
// m_moveAccel = 0.5;
// m_maxMoveSpeed = 6;
// m_jumpPower = -15;
// m_gravity = 0.8;
```

### 修改窗口大小

**文件**: `main.cpp` 第 5 行

```cpp
// 当前配置：800x600
GameManager game(800, 600);

// 其他常用分辨率
// GameManager game(640, 480);   // 小窗口
// GameManager game(1024, 768);  // 大窗口
// GameManager game(480, 800);   // 竖屏（移动模拟）
```

---

## 🐛 常见问题

### Q1: 编译出现"找不到 graphics.h"

**解决方案**：
1. 检查 EasyX 是否正确安装
2. 在 Visual Studio 项目属性中添加 EasyX 包含路径：
   - 项目属性 → VC++ 目录 → 包含目录
   - 添加：`C:\Program Files\EasyX\include`
3. 在库目录中添加：`C:\Program Files\EasyX\lib`

### Q2: 窗口无法创建

**解决方案**：
1. 确认 EasyX 库文件已正确链接
2. 检查项目配置是否为 x86（32位）或 x64（64位）
3. 尝试以管理员身份运行程序

### Q3: 角色无法跳跃

**解决方案**：
1. 确认已按下空格键
2. 检查键盘是否正常工作
3. 尝试重新启动程序
4. 查看 `DEBUG_GUIDE.md` 中的调试建议

### Q4: 最高记录无法保存

**解决方案**：
1. 确认程序有写入权限
2. 检查项目目录下是否有 `highscore.txt` 文件
3. 手动创建 `highscore.txt` 文件（内容为 `0`）

### Q5: 游戏运行卡顿

**解决方案**：
1. 关闭其他应用释放系统资源
2. 在 `GameManager::Run()` 中将 `Sleep(16)` 改为 `Sleep(33)` 以降低目标帧率
3. 检查是否有无限循环或内存泄漏

---

## 📝 代码快速浏览

### 核心游戏循环

**文件**: `GameManager.cpp`

```cpp
void GameManager::Run()
{
    BeginBatchDraw();

    while (true)
    {
        HandleInput();      // 处理按键输入

        if (m_gameState == PLAYING)
            Update();       // 更新游戏逻辑

        Draw();            // 绘制画面

        FlushBatchDraw();
        Sleep(16);         // 60 FPS
    }

    EndBatchDraw();
}
```

### 双跳机制实现

**文件**: `Role.cpp`

```cpp
void Role::Jump()
{
    if (m_jumpCount > 0)
    {
        m_vy = m_jumpPower;      // 设置向上速度
        m_jumpCount--;           // 递减跳跃次数
        m_isOnGround = false;    // 标记离地
    }
}

// 落地重置
bool Role::CheckLandOnPlatform(...)
{
    if (碰撞检测成功)
    {
        m_jumpCount = 2;         // 重置为2次可跳
        m_isOnGround = true;     // 标记着地
        m_vy = 0;               // 停止下落
        return true;
    }
    return false;
}
```

### 平台生成

**文件**: `GameManager.cpp`

```cpp
void GameManager::GeneratePlatform()
{
    // 获取难度倍数（0.0 - 1.0）
    double difficultyMultiplier = GetDifficultyMultiplier();

    // 随机生成平台
    double gap = m_platformGapMin + rand() % (m_platformGapMax - m_platformGapMin);
    double newY = m_lastPlatformY - gap;
    double newX = 随机X位置;

    // 难度越高，平台越窄
    double adjustedWidth = m_platformWidth * (1.0 - difficultyMultiplier * 0.3);

    Platform* platform = new Platform(newX, newY, adjustedWidth, 
                                     m_platformHeight, m_platformSpeed);
    m_platforms.push_back(platform);
}
```

---

## 🎨 后期素材替换

所有素材位置都有中文注释标记，搜索以下关键字快速找到：

- `// ========= 下方替换为罗小黑立绘 =========` (Role.cpp)
- `// ========= 下方替换为森林树干/石板纹理 =========` (Platform.cpp)
- `// ========= 下方替换为森林背景纹理 =========` (GameManager.cpp)

替换示例：

```cpp
// 替换罗小黑角色
IMAGE img;
loadimage(&img, _T("luoxiaohei.png"));
putimage(m_x - m_radius, m_y - m_radius, &img);

// 替换平台纹理
IMAGE platformImg;
loadimage(&platformImg, _T("platform.png"));
putimage(m_x - m_width/2, m_y - m_height/2, &platformImg);

// 替换背景
IMAGE bgImg;
loadimage(&bgImg, _T("background.png"), m_screenWidth, m_screenHeight);
putimage(0, m_backgroundScrollY, &bgImg);
```

---

## 📚 详细文档

- **README.md** - 完整项目说明和功能介绍
- **DEBUG_GUIDE.md** - 调试指南和性能优化
- **CONFIG_EXAMPLES.md** - 配置示例和参数说明

---

## ✨ 游戏特色总结

✅ **完整的双跳机制** - 经典玩法完美实现  
✅ **动态难度递进** - 随分数自动提升挑战  
✅ **治愈系主题** - 罗小黑风格美术设计  
✅ **本地高分记录** - 分数自动保存  
✅ **模块化代码** - 易于修改和扩展  
✅ **无素材可运行** - 纯代码绘制，可直接运行  

---

## 🚀 下一步

1. **编译运行**游戏，熟悉玩法
2. **调整参数**找到最佳游戏感
3. **添加素材**替换默认图形
4. **扩展功能**添加道具或敌人系统

**祝您游戏开发愉快！** 🎮

---

**文档版本**: v1.0  
**最后更新**: 2024年  
