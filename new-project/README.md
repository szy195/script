# KBot Project

这是一个基于C++的游戏辅助工具项目，采用模块化架构设计，包含核心功能、游戏对象管理、渲染系统和UI界面。

## 项目结构

```
new-project/
├── Core/               # 核心系统
│   ├── KBotCore.h/cpp  # 核心管理器
│   ├── GameAPI.h/cpp   # 游戏API接口
│   ├── MemoryManager.h/cpp # 内存管理
│   └── Events.h/cpp    # 事件系统
├── Game/               # 游戏对象
│   ├── GameObject.h/cpp # 游戏对象基类
│   ├── Hero.h/cpp      # 英雄类
│   ├── Minion.h/cpp    # 小兵类
│   └── Turret.h/cpp    # 防御塔类
├── Renderer/           # 渲染系统
│   └── Renderer.h/cpp  # 渲染器
├── UI/                 # 用户界面
│   ├── UI.h/cpp        # UI系统核心
│   ├── Controls.h/cpp  # UI控件
│   └── UISample.h/cpp  # UI示例
├── Input/              # 输入系统
│   ├── InputManager.h/cpp # 输入管理器
├── Features/           # 功能模块
│   └── Orbwalker.h/cpp # 走砍功能
├── Tools/              # 工具类
│   ├── OffsetFinder.h/cpp # 基址查找工具
│   ├── OffsetFinderExample.cpp # 基址查找示例
│   └── OrbwalkerTest.cpp # 走砍功能测试
├── Utils/              # 工具类
│   ├── Math.h          # 数学工具
│   └── Vector.h        # 向量类
├── Security/           # 安全模块
├── main.cpp            # 主程序入口
├── ORBWALKER_GUIDE.md  # 走砍功能开发指南
├── CMakeLists.txt      # CMake构建配置
└── README.md           # 项目说明
```

## 核心功能

### 1. 核心系统 (Core)
- **KBotCore**: 核心管理器，负责初始化和协调各个子系统
- **GameAPI**: 游戏API接口，提供游戏状态查询和操作功能
- **MemoryManager**: 内存管理器，提供内存读写、搜索和钩子功能
- **Events**: 事件系统，支持事件监听、触发和队列处理

### 2. 游戏对象管理 (Game)
- **GameObject**: 游戏对象基类，提供通用属性和方法
- **Hero**: 英雄类，继承自GameObject，添加英雄特有功能
- **Minion**: 小兵类，继承自GameObject，添加小兵特有功能
- **Turret**: 防御塔类，继承自GameObject，添加防御塔特有功能
- **GameObjectManager**: 游戏对象管理器，负责管理所有游戏对象

### 3. 输入系统 (Input)
- **InputManager**: 输入管理器，负责处理键盘和鼠标输入
- 支持按键状态检测（按下、刚按下、刚释放）
- 支持鼠标状态检测和位置获取

### 4. 功能模块 (Features)
- **Orbwalker**: 走砍功能，允许英雄在攻击间隙进行移动
  - 多种走砍模式（补刀、清线、连招、逃跑）
  - 多种目标选择策略（最低血量、最近距离、靠近鼠标等）
  - 可自定义按键设置

### 5. 工具类 (Tools)
- **OffsetFinder**: 基址查找工具，帮助找到游戏内存中的关键基址
  - 自动查找攻击范围、攻击速度、攻击起手时间等基址
  - 基址有效性验证和保存功能

### 6. 渲染系统 (Renderer)
- **Renderer**: 渲染器，提供2D图形绘制功能，支持线条、矩形、圆形等基本图形
- 支持文本渲染和图像绘制
- 提供坐标转换功能

### 7. 用户界面 (UI)
- **UIElement**: UI元素基类，提供通用UI功能
- **UIManager**: UI管理器，负责UI元素管理和事件处理
- **UIRenderer**: UI渲染器，提供各类UI元素的渲染实现
- **Controls**: 各种UI控件，包括面板、按钮、标签、文本框、复选框等

## 构建说明

### 前置要求
- CMake 3.10 或更高版本
- C++17 兼容的编译器
- OpenGL
- GLFW

### 构建步骤

1. 创建构建目录：
```bash
mkdir build
cd build
```

2. 配置项目：
```bash
cmake ..
```

3. 编译项目：
```bash
cmake --build .
```

4. 运行程序：
```bash
./bin/KBot
```

## 使用说明

### 基本使用
1. 初始化KBot核心系统
2. 初始化渲染器和UI系统
3. 创建UI元素并添加到管理器
4. 在主循环中处理输入和渲染

### 走砍功能使用
1. 查找基址：
   ```cpp
   // 初始化基址查找工具
   auto& offsetFinder = KBot::Tools::OffsetFinder::GetInstance();
   offsetFinder.Initialize();
   
   // 查找走砍功能所需的基址
   offsetFinder.FindOrbwalkerOffsets();
   
   // 保存基址到文件
   offsetFinder.SaveOffsetsToFile("orbwalker_offsets.txt");
   ```

2. 使用走砍功能：
   ```cpp
   // 获取走砍功能实例
   auto orbwalker = KBotCore::GetInstance().GetModule<Features::Orbwalker>("Orbwalker");
   
   // 启用走砍功能
   orbwalker->SetEnabled(true);
   
   // 设置模式
   orbwalker->SetMode(Features::OrbwalkerMode::COMBO);
   ```

3. 按键设置：
   - 空格键：连招模式
   - C键：骚扰模式
   - V键：清线模式
   - X键：补刀模式
   - Z键：逃跑模式

### UI示例
项目包含一个UI示例，展示了如何使用各种UI控件：
- 窗口和面板
- 菜单和菜单项
- 按钮、文本框、复选框
- 滑块、进度条
- 组合框、列表框
- 标签页控件

### 扩展功能
1. 在Features目录下添加新功能模块
2. 在Security目录下添加安全相关功能
3. 扩展现有类以添加新功能

## 架构设计

### 设计原则
- 模块化设计，各模块职责明确
- 使用单例模式管理全局资源
- 基于事件的系统通信
- 面向对象的游戏对象模型

### 关键模式
- 单例模式：用于管理核心系统、内存管理器、渲染器等全局资源
- 观察者模式：用于事件系统
- 工厂模式：用于创建游戏对象
- 策略模式：用于不同的渲染和UI策略

## 注意事项

1. 本项目仅用于学习和研究目的
2. 使用时请遵守相关游戏的使用条款
3. 项目仍在开发中，部分功能可能不完整

## 贡献指南

1. Fork 项目
2. 创建功能分支
3. 提交更改
4. 推送到分支
5. 创建Pull Request

## 许可证

本项目采用MIT许可证，详见LICENSE文件。