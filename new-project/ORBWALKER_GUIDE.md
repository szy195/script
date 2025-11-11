# KBot 走砍功能开发指南

## 概述

本指南将帮助您使用KBot的走砍功能，并使用基址查找工具找到所需的基址。

## 目录

1. [功能概述](#功能概述)
2. [基址查找](#基址查找)
3. [走砍功能使用](#走砍功能使用)
4. [常见问题](#常见问题)

## 功能概述

走砍功能是KBot的核心功能之一，它允许英雄在攻击的间隙进行移动，从而提高战斗效率。走砍功能包括以下组件：

- **Orbwalker**: 走砍功能的核心类，负责处理攻击和移动逻辑
- **InputManager**: 输入管理器，负责处理键盘和鼠标输入
- **OffsetFinder**: 基址查找工具，帮助找到游戏内存中的关键基址

## 基址查找

### 1. 初始化基址查找工具

在使用基址查找工具之前，请确保游戏已经运行并且KBot已经成功注入。

```cpp
#include "../Tools/OffsetFinder.h"

// 初始化基址查找工具
KBot::Tools::OffsetFinder& offsetFinder = KBot::Tools::OffsetFinder::GetInstance();
offsetFinder.Initialize();
```

### 2. 查找走砍功能所需的基址

```cpp
// 查找所有走砍功能所需的基址
bool success = offsetFinder.FindOrbwalkerOffsets();

// 打印找到的基址
offsetFinder.PrintFoundOffsets();

// 保存基址到文件
offsetFinder.SaveOffsetsToFile("orbwalker_offsets.txt");
```

### 3. 手动验证基址

自动查找的基址可能不完全准确，需要手动验证。以下是验证方法：

1. **攻击范围**:
   - 让英雄移动到不同位置
   - 观察基址值是否随距离变化
   - 正确的攻击范围应该在200-2000之间

2. **攻击速度**:
   - 购买攻击速度装备
   - 观察基址值是否增加
   - 正确的攻击速度应该在0.1-3.0之间

3. **攻击起手时间**:
   - 更换英雄
   - 观察基址值是否变化
   - 正确的攻击起手时间应该在0.1-1.0之间

### 4. 更新基址到Offsets.h

将验证后的基址添加到`Core/Offsets.h`文件中：

```cpp
namespace Hero {
    // 攻击范围
    static const uintptr_t AttackRange = 0x12345678;
    
    // 攻击速度
    static const uintptr_t AttackSpeed = 0x12345678;
    
    // 攻击起手时间
    static const uintptr_t AttackCastTime = 0x12345678;
    
    // 状态标志
    static const uintptr_t StatusFlags = 0x12345678;
}
```

## 走砍功能使用

### 1. 启用走砍功能

走砍功能在KBot初始化时自动启用，您可以通过以下方式控制：

```cpp
// 获取走砍功能实例
auto orbwalker = KBotCore::GetInstance().GetModule<Features::Orbwalker>("Orbwalker");

// 启用/禁用走砍功能
orbwalker->SetEnabled(true);

// 设置走砍模式
orbwalker->SetMode(Features::OrbwalkerMode::COMBO);
```

### 2. 按键设置

走砍功能默认使用以下按键：

- **空格键**: 连招模式
- **C键**: 骚扰模式
- **V键**: 清线模式
- **X键**: 补刀模式
- **Z键**: 逃跑模式

您可以通过以下方式自定义按键：

```cpp
orbwalker->SetComboKey(VK_SPACE);  // 设置连招键为空格键
orbwalker->SetHarassKey(0x43);      // 设置骚扰键为C键
orbwalker->SetLaneClearKey(0x56);   // 设置清线键为V键
orbwalker->SetLastHitKey(0x58);     // 设置补刀键为X键
orbwalker->SetFleeKey(0x5A);        // 设置逃跑键为Z键
```

### 3. 目标选择器设置

走砍功能支持多种目标选择策略：

```cpp
// 设置目标选择器
orbwalker->SetTargetSelector(Features::TargetSelector::LOWEST_HEALTH);  // 最低血量
orbwalker->SetTargetSelector(Features::TargetSelector::CLOSEST);         // 最近距离
orbwalker->SetTargetSelector(Features::TargetSelector::NEAR_MOUSE);      // 靠近鼠标
```

## 常见问题

### Q: 走砍功能不工作怎么办？

A: 请检查以下几点：

1. 确保游戏正在运行并且KBot已成功注入
2. 检查基址是否正确，可以使用OffsetFinder工具重新查找
3. 确保走砍功能已启用
4. 检查按键设置是否正确

### Q: 如何找到正确的基址？

A: 使用OffsetFinder工具可以自动查找可能的基址，但需要手动验证：

1. 运行游戏并注入KBot
2. 调用`FindOrbwalkerOffsets()`方法查找基址
3. 打印或保存找到的基址
4. 手动验证每个基址的正确性
5. 将正确的基址添加到Offsets.h文件中

### Q: 走砍功能导致游戏卡顿怎么办？

A: 可能是以下原因：

1. 更新频率过高，可以调整休眠时间
2. 内存读取错误，检查基址是否正确
3. 多线程冲突，确保线程安全

### Q: 如何自定义走砍逻辑？

A: 您可以通过继承Orbwalker类或修改现有方法来自定义走砍逻辑：

```cpp
class CustomOrbwalker : public Features::Orbwalker {
public:
    // 重写Orbwalk方法
    void Orbwalk(Hero* target = nullptr, const Vector3& position = Vector3()) override {
        // 自定义走砍逻辑
    }
};
```

## 下一步

完成走砍功能后，您可以继续开发以下功能：

1. 目标选择器
2. 技能CD计时
3. 躲避系统
4. 活化剂使用
5. 英雄连招

每个功能都需要相应的基址支持，您可以使用OffsetFinder工具或其他方法找到这些基址。