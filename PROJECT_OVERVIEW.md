# 罗小黑战记·森林双跳游戏 - 项目完整文档

## 📦 项目文件总览

```
LuoXiaoHeiJumpGame/
├── 源代码文件
│   ├── main.cpp                     # 游戏入口点
│   ├── Role.h / Role.cpp            # 角色类实现
│   ├── Platform.h / Platform.cpp    # 平台类实现
│   ├── GameManager.h / GameManager.cpp  # 游戏管理器实现
│
├── 项目配置
│   └── LuoXiaoHeiJumpGame.vcxproj   # Visual Studio 项目文件
│
├── 文档说明
│   ├── README.md                    # 完整功能说明（中文）
│   ├── QUICK_START.md               # 快速启动指南（中文）
│   ├── DEBUG_GUIDE.md               # 调试指南（中文）
│   ├── CONFIG_EXAMPLES.md           # 配置参数示例（中文）
│   └── PROJECT_OVERVIEW.md          # 本文件
│
└── 运行时文件
    └── highscore.txt                # 最高分记录（自动生成）
```

## ✨ 项目亮点

### 核心玩法
- ✅ **完整双跳系统**：地面可跳2次，空中可跳1次
- ✅ **动态难度**：随分数自动提升难度
- ✅ **精准碰撞检测**：只有从平台顶部踩到才算落地
- ✅ **屏幕环绕机制**：水平移出屏幕从另一侧出现

### 技术特性
- ✅ **模块化设计**：清晰的类划分和职责
- ✅ **60FPS稳定帧率**：流畅的游戏体验
- ✅ **本地数据存储**：高分自动保存
- ✅ **完全可配置**：所有参数都可自定义调整

### 罗小黑主题
- ✅ **治愈系画风**：清新绿色背景和森林场景
- ✅ **角色设计**：黑色圆形+黄色五官的简化罗小黑
- ✅ **主题文案**：游戏结束提示贴合角色特色
- ✅ **素材预留**：所有绘制位置都有替换注释

## 🎮 快速上手（5分钟）

### 1. 编译（2分钟）
```powershell
# Visual Studio 中按 Ctrl+Shift+B
# 或命令行：
cd LuoXiaoHeiJumpGame
cl.exe /EHsc Role.cpp Platform.cpp GameManager.cpp main.cpp /link graphics.lib user32.lib
```

### 2. 运行（1分钟）
```powershell
# Visual Studio 中按 Ctrl+F5
# 或直接运行 .exe 文件
```

### 3. 游戏操作
| 操作 | 按键 |
|------|------|
| 左移 | ← |
| 右移 | → |
| 跳跃 | Space |
| 暂停 | 点击右上角按钮 |

## 📊 代码架构

### 类关系图
```
GameManager (游戏管理器)
    ├── Role (玩家角色) x1
    ├── Platform (游戏平台) x多个
    └── UI & Input & Physics
```

### 核心类说明

#### Role 类（角色模块）
**文件**: Role.h / Role.cpp

| 成员 | 说明 |
|------|------|
| `m_x, m_y` | 角色位置坐标 |
| `m_vx, m_vy` | 速度向量 |
| `m_jumpCount` | 当前可跳跃次数 |
| `m_gravity` | 重力加速度 |

| 方法 | 说明 |
|------|------|
| `Update()` | 更新物理（重力、速度、位置） |
| `Move(dir)` | 处理水平移动 |
| `Jump()` | 执行跳跃逻辑 |
| `CheckLandOnPlatform()` | 碰撞检测 |
| `Draw()` | 绘制角色 |

#### Platform 类（平台模块）
**文件**: Platform.h / Platform.cpp

| 成员 | 说明 |
|------|------|
| `m_x, m_y` | 平台中心坐标 |
| `m_width, m_height` | 平台尺寸 |
| `m_speed` | 下落速度 |

| 方法 | 说明 |
|------|------|
| `Update()` | 平台向下移动 |
| `Draw()` | 绘制平台 |
| `IsOutOfScreen()` | 检查超出屏幕 |

#### GameManager 类（游戏管理器）
**文件**: GameManager.h / GameManager.cpp

| 关键方法 | 说明 |
|---------|------|
| `Init()` | 初始化游戏 |
| `Run()` | 主游戏循环 |
| `HandleInput()` | 处理输入 |
| `Update()` | 更新逻辑 |
| `Draw()` | 绘制画面 |
| `GeneratePlatform()` | 生成新平台 |
| `GetDifficultyMultiplier()` | 计算难度倍数 |

## 🎯 游戏流程

```
初始化 Init()
    ↓
创建游戏窗口
    ↓
加载最高分
    ↓
生成初始平台
    ↓
主循环 Run()
    ├→ HandleInput() - 处理按键
    ├→ Update() - 更新逻辑
    │   ├→ 更新角色物理
    │   ├→ 更新平台位置
    │   ├→ 碰撞检测
    │   ├→ 分数计算
    │   └→ 检查游戏结束
    ├→ Draw() - 绘制画面
    │   ├→ 背景
    │   ├→ 平台
    │   ├→ 角色
    │   └→ UI
    └→ 控制帧率 Sleep(16ms)
```

## 🔧 参数调整指南

### 快速调整难度

**简单模式**（新手推荐）
```cpp
m_platformSpeed = 2;        // 平台移动速度
m_platformWidth = 100;      // 平台宽度
m_platformGapMin = 60;      // 最小间隙
m_platformGapMax = 120;     // 最大间隙
```

**困难模式**（高手挑战）
```cpp
m_platformSpeed = 4.5;
m_platformWidth = 60;
m_platformGapMin = 30;
m_platformGapMax = 80;
```

### 调整操作感

**灵敏型**（快速反应）
```cpp
m_moveAccel = 1.2;      // 加速快
m_maxMoveSpeed = 10;    // 速度快
m_jumpPower = -22;      // 跳跃高
m_gravity = 0.5;        // 下落慢
```

**沉重型**（需要预判）
```cpp
m_moveAccel = 0.5;      // 加速慢
m_maxMoveSpeed = 6;     // 速度慢
m_jumpPower = -15;      // 跳跃低
m_gravity = 0.8;        // 下落快
```

详见 `CONFIG_EXAMPLES.md` 获取完整配置方案。

## 🎨 素材替换位置

所有素材预留位置都标记了中文注释，便于查找：

### 1. 角色替换 (Role.cpp 中 Draw() 函数)
```cpp
// ========= 下方替换为罗小黑立绘 =========
// 改为加载图片并绘制
// 示例：loadimage(&img, _T("luoxiaohei.png"));
```

### 2. 平台替换 (Platform.cpp 中 Draw() 函数)
```cpp
// ========= 下方替换为森林树干/石板纹理 =========
// 改为加载平台纹理
// 示例：loadimage(&platformImg, _T("platform.png"));
```

### 3. 背景替换 (GameManager.cpp 中 DrawBackground() 函数)
```cpp
// ========= 下方替换为森林背景纹理 =========
// 改为加载背景图片
// 示例：loadimage(&bgImg, _T("background.png"));
```

## 📈 难度递进机制

```
分数 0-500:    难度倍数 0.0 - 1.0 (线性增长)
分数 > 500:    难度倍数 1.0 (达到最高难度)

平台宽度随难度变化:
    adjustedWidth = m_platformWidth * (1.0 - difficultyMultiplier * 0.3)

    例如：
    - 分数 0:   宽度 = 80 * (1.0 - 0.0 * 0.3) = 80
    - 分数 250: 宽度 = 80 * (1.0 - 0.5 * 0.3) = 68
    - 分数 500: 宽度 = 80 * (1.0 - 1.0 * 0.3) = 56
```

## 🐛 常见问题排查

### 编译错误
- **缺少 graphics.h**: 检查 EasyX 是否正确安装
- **链接错误**: 确保项目属性中添加了 EasyX 的库目录

### 运行时错误
- **无法显示窗口**: 以管理员权限运行
- **角色无法跳跃**: 检查键盘驱动和输入处理

### 游戏性问题
- **无法踩中平台**: 调整碰撞检测范围（Role.cpp 第 68-70 行）
- **下落过快/过慢**: 修改 `m_gravity` 参数
- **移动不灵敏**: 增加 `m_moveAccel` 值

详见 `DEBUG_GUIDE.md` 获取完整的调试指南。

## 📚 学习资源

### C++ 特性使用
- 面向对象设计（类和继承）
- STL 容器（vector）
- 指针和内存管理
- 文件 I/O 操作

### 游戏开发概念
- 游戏循环架构
- 物理引擎基础
- 碰撞检测算法
- UI 事件处理

### EasyX 图形库
- 图形绘制函数
- 事件处理系统
- 字符串和文本显示
- 颜色和样式管理

## 🚀 扩展方向

### 短期扩展（易实现）
1. **添加道具系统**：减速、加分、护盾
2. **敌人系统**：妖灵敌人和 Boss
3. **角色皮肤**：无限、阿根等角色
4. **音效系统**：跳跃、着地、游戏结束声音

### 中期扩展（中等难度）
1. **等级系统**：多个关卡和场景
2. **排行榜**：在线分享最高分
3. **成就系统**：解锁不同成就
4. **特效系统**：粒子效果和屏幕抖动

### 长期扩展（高难度）
1. **联网对战**：多人在线竞技
2. **自定义编辑器**：玩家创建关卡
3. **移动端适配**：安卓 / iOS 移植
4. **AI 对手**：单机对抗模式

## 📋 项目检查清单

- [x] 完整的双跳系统实现
- [x] 动态难度递进机制
- [x] 分数和最高分记录
- [x] 暂停和游戏结束逻辑
- [x] 罗小黑主题视觉设计
- [x] 碰撞检测和物理模拟
- [x] 模块化代码结构
- [x] 详细中文注释
- [x] 素材替换预留位置
- [x] 完整项目文档
- [x] 调试指南和配置示例
- [x] 成功编译运行

## 📞 技术支持

### 在线资源
- EasyX 官网：http://www.easyx.cn
- EasyX 文档：http://www.easyx.cn/help/
- C++ 参考：https://cppreference.com

### 文档查询顺序
1. **快速问题** → 查看 QUICK_START.md
2. **参数调整** → 查看 CONFIG_EXAMPLES.md  
3. **代码调试** → 查看 DEBUG_GUIDE.md
4. **功能说明** → 查看 README.md

## 📄 许可证

本项目为学习教育用途开发。

---

## 版本历史

| 版本 | 日期 | 更新内容 |
|------|------|---------|
| 1.0 | 2024 | 初始发布版本 |

---

**感谢使用罗小黑战记·森林双跳游戏！** 🎮✨

祝您开发愉快！如有任何问题，欢迎参考相关文档或查看代码注释。

