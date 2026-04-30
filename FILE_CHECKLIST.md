# 💾 项目文件清单 & 使用说明

## 📋 完整文件列表

### 源代码文件（必需）
```
✅ main.cpp                 # 游戏主函数入口
✅ Role.h / Role.cpp        # 角色类：管理玩家移动、跳跃、碰撞
✅ Platform.h / Platform.cpp # 平台类：管理平台生成和移动
✅ GameManager.h / GameManager.cpp # 游戏管理器：控制游戏流程
```

### 项目配置文件
```
✅ LuoXiaoHeiJumpGame.vcxproj    # Visual Studio 项目配置文件
```

### 文档文件
```
✅ README.md              # 完整功能说明和使用指南
✅ QUICK_START.md         # 5分钟快速上手指南
✅ DEBUG_GUIDE.md         # 调试建议和常见问题
✅ CONFIG_EXAMPLES.md     # 参数配置示例
✅ PROJECT_OVERVIEW.md    # 项目架构和扩展指南
✅ FILE_CHECKLIST.md      # 本文件
```

### 运行时生成文件
```
⚙️ highscore.txt          # 最高分记录（自动生成）
```

---

## 📖 文档导航指南

### 根据您的需求选择对应文档

#### 🆕 初次接触项目？
**推荐阅读顺序**：
1. **QUICK_START.md** (5 分钟快速上手)
   - 5分钟完成编译运行
   - 快速了解游戏操作
   - 基本参数调整方法

2. **README.md** (完整功能说明)
   - 全面了解游戏机制
   - 代码模块说明
   - 后期素材替换方法

3. **PROJECT_OVERVIEW.md** (架构和扩展)
   - 了解项目结构
   - 类关系和设计模式
   - 未来扩展方向

#### 🔧 想要调整游戏参数？
**推荐阅读**：
1. **CONFIG_EXAMPLES.md** - 快速查找参数配置
   - 预设的难度配置（简单/中等/困难）
   - 角色物理参数（敏捷/平衡/沉重型）
   - 视觉效果参数

2. **README.md** - "难度调整指南" 部分
   - 详细参数说明
   - 推荐调整范围

#### 🐛 遇到问题或想深入学习？
**推荐阅读**：
1. **DEBUG_GUIDE.md** - 调试和优化
   - 常见问题和解决方案
   - 调试技巧和断点设置
   - 性能优化建议

2. **README.md** - "常见问题" 部分
   - 快速参考表

#### 🎨 想要添加素材（图片、音效等）？
**推荐阅读**：
1. **README.md** - "素材替换指南" 部分
2. **PROJECT_OVERVIEW.md** - "素材替换位置" 部分
3. 代码注释 - 搜索 "========="

---

## 🚀 快速启动流程

### 第一次使用

```
1. 检查前置条件
   ├─ Visual Studio 2019/2022
   └─ EasyX 2024 已安装

2. 复制项目文件到本地
   └─ 所有 .cpp .h .vcxproj 文件

3. 打开项目
   └─ 使用 Visual Studio 打开 .vcxproj 文件

4. 编译
   └─ Ctrl+Shift+B (或 Build > Build Solution)

5. 运行
   └─ Ctrl+F5 (或 Debug > Run Without Debugging)

✅ 游戏启动！
```

### 第二次及以后

```
1. 打开 VS 项目文件
2. Ctrl+F5 直接运行
3. 或根据需求修改代码后重新编译运行
```

---

## 💡 常用操作快速参考

### Visual Studio 快捷键
| 操作 | 快捷键 |
|------|--------|
| 编译 | Ctrl+Shift+B |
| 运行 | Ctrl+F5 |
| 调试运行 | F5 |
| 停止调试 | Shift+F5 |
| 查找文件 | Ctrl+T |
| 查找引用 | Ctrl+K Ctrl+R |
| 转到定义 | F12 |

### 代码查找

**查找双跳逻辑**：
- 文件：Role.cpp
- 函数：Jump() 和 Update()
- 关键词：m_jumpCount

**查找碰撞检测**：
- 文件：Role.cpp
- 函数：CheckLandOnPlatform()
- 关键词：碰撞

**查找难度调整**：
- 文件：GameManager.cpp
- 函数：GeneratePlatform() 和 GetDifficultyMultiplier()
- 关键词：difficulty

**查找素材位置**：
- 关键词：========= （在代码中搜索）

---

## 🔍 文件用途速查表

| 文件 | 用途 | 何时查看 |
|------|------|---------|
| main.cpp | 程序入口点 | 修改窗口大小 |
| Role.h/cpp | 角色逻辑 | 调整跳跃/移动 |
| Platform.h/cpp | 平台逻辑 | 调整平台外观 |
| GameManager.h/cpp | 游戏核心逻辑 | 调整难度/分数 |
| QUICK_START.md | 快速上手 | 第一次使用 |
| README.md | 完整说明 | 需要详细了解 |
| DEBUG_GUIDE.md | 调试指南 | 遇到问题 |
| CONFIG_EXAMPLES.md | 参数配置 | 调整游戏参数 |
| PROJECT_OVERVIEW.md | 项目架构 | 深入学习/扩展 |

---

## 🎯 按任务查找对应部分

### 任务 1: "我想立即玩到游戏"
```
查看: QUICK_START.md → 第一章 "5分钟快速上手"
时间: 5-10 分钟
```

### 任务 2: "我想修改游戏难度"
```
查看: CONFIG_EXAMPLES.md → "难度配置" 部分
或    GameManager.cpp → 第 119-123 行
时间: 2-5 分钟
```

### 任务 3: "我想调整角色操作感"
```
查看: CONFIG_EXAMPLES.md → "角色物理配置" 部分
或    Role.cpp → 第 10-18 行
时间: 3-5 分钟
```

### 任务 4: "我想添加自己的角色图片"
```
查看: README.md → "素材替换指南" → "替换罗小黑角色"
或    Role.cpp → Draw() 函数中的注释
时间: 10-20 分钟
```

### 任务 5: "我想为游戏添加音效"
```
查看: PROJECT_OVERVIEW.md → "扩展方向" → "短期扩展"
时间: 1-2 小时
```

### 任务 6: "游戏运行出现问题"
```
查看: DEBUG_GUIDE.md → "常见BUG及修复"
或    QUICK_START.md → "常见问题"
时间: 5-30 分钟（取决于问题复杂度）
```

### 任务 7: "我想深入理解代码架构"
```
查看: PROJECT_OVERVIEW.md → "项目架构" 部分
或    README.md → "代码模块说明" 部分
时间: 30-60 分钟
```

### 任务 8: "我想扩展游戏功能"
```
查看: PROJECT_OVERVIEW.md → "扩展方向" 部分
或    DEBUG_GUIDE.md → "扩展功能预留" 部分
时间: 根据扩展内容而定
```

---

## 📊 文件关系图

```
main.cpp (入口)
    ↓
GameManager (游戏管理器)
    ├── Role (角色)
    │   ├── Role.h (接口)
    │   └── Role.cpp (实现)
    │
    ├── Platform (平台)
    │   ├── Platform.h (接口)
    │   └── Platform.cpp (实现)
    │
    └── GameManager.h/cpp (核心逻辑)
        ├── 游戏循环
        ├── 输入处理
        ├── 物理更新
        ├── 碰撞检测
        └── UI 绘制

高分记录
    ↓
highscore.txt (自动生成)
```

---

## ✅ 使用检查清单

### 初始设置
- [ ] 安装了 EasyX 图形库
- [ ] Visual Studio 已配置好 EasyX 路径
- [ ] 所有源文件已复制到项目目录
- [ ] 项目文件已在 VS 中打开

### 编译前
- [ ] 确认项目配置为 Debug 或 Release
- [ ] 确认平台为 Win32 或 x64
- [ ] 检查 EasyX 库文件是否正确链接

### 编译后
- [ ] 编译成功（无错误）
- [ ] 可执行文件已生成

### 运行时
- [ ] 游戏窗口正常显示
- [ ] 能够接收键盘输入
- [ ] 角色能够移动和跳跃
- [ ] 分数能够正常计算

### 扩展前
- [ ] 已理解代码架构
- [ ] 已准备好需要的资源（图片、音效等）
- [ ] 已备份原始代码（如需要）

---

## 🆘 获取帮助的方法

### 遇到编译错误？
1. 查看 QUICK_START.md → "常见问题" → "Q1-Q2"
2. 查看 DEBUG_GUIDE.md → "常见BUG" 部分

### 游戏无法运行？
1. 查看 QUICK_START.md → "常见问题" → "Q2"
2. 检查 Visual Studio 输出窗口的错误信息

### 游戏逻辑有问题？
1. 查看 DEBUG_GUIDE.md → "调试注释和关键位置"
2. 在代码中添加 printf 输出调试信息

### 想要修改参数？
1. 打开 CONFIG_EXAMPLES.md
2. 找到对应的参数配置
3. 复制相应代码到源文件中

### 想要添加新功能？
1. 查看 PROJECT_OVERVIEW.md → "扩展方向"
2. 查看 DEBUG_GUIDE.md → "扩展功能预留"
3. 学习现有的类设计模式后进行扩展

---

## 💾 文件备份建议

建议保持以下结构的备份：

```
LuoXiaoHeiJumpGame_Backup/
├── src/                    # 原始源代码
│   ├── main.cpp
│   ├── Role.h / Role.cpp
│   ├── Platform.h / Platform.cpp
│   └── GameManager.h / GameManager.cpp
├── docs/                   # 所有文档
│   ├── README.md
│   ├── QUICK_START.md
│   └── ... 其他文档
└── README_BACKUP.txt       # 备份说明
```

这样可以在需要时快速恢复原始版本。

---

## 📞 获取最新信息

- **EasyX 最新版本**：http://www.easyx.cn
- **文档更新**：本项目文件夹内的各 .md 文件
- **示例代码**：代码注释中的示例

---

## 🎓 推荐学习路径

### C++ 基础不足？
1. 学习 C++ 基本语法（变量、函数、类）
2. 学习指针和动态内存
3. 学习 STL 容器（vector）
4. 回到本项目

### EasyX 不熟悉？
1. 查看 EasyX 官方文档：http://www.easyx.cn/help/
2. 学习基础绘图函数
3. 学习事件处理
4. 在本项目中应用

### 游戏开发入门？
1. 理解游戏循环概念
2. 学习碰撞检测基础
3. 学习简单物理模拟
4. 通过本项目实践

---

## ✨ 项目成就

**已完成**：
- ✅ 完整的游戏逻辑实现
- ✅ 模块化代码结构
- ✅ 详细的中文文档
- ✅ 可配置的参数系统
- ✅ 素材替换预留位置
- ✅ 调试指南和示例

**正在进行**：
- 🚀 等待您的创意扩展！

**未来计划**：
- 📱 移动端适配
- 🌐 联网功能
- 🎵 音效系统
- 🏆 排行榜系统

---

**文档版本**：v1.0  
**最后更新**：2024年  
**所有文档已完成**：✅

祝您使用愉快！🎮✨

