# KBot功能开发所需基址列表

本文档详细说明了开发各个功能模块所需的基址偏移量，按照功能优先级排序。

## 1. 走砍 (Orbwalker)

### 基本功能基址
- **本地玩家基址**: `Game::LocalPlayer` - 获取玩家对象
- **移动函数**: `Game::MoveFunction` - 执行移动指令
- **攻击函数**: `Game::AttackFunction` - 执行攻击指令
- **游戏时间**: `Game::GameTime` - 计算攻击间隔和移动时机
- **游戏时钟**: `Game::GameTickCount` - 同步游戏状态

### 玩家状态基址
- **位置坐标**: `GameObject::PositionX/Y/Z` - 获取当前位置
- **攻击速度**: `GameObject::AttackSpeed` - 计算攻击间隔
- **移动速度**: `GameObject::MovementSpeed` - 计算移动时间
- **攻击范围**: `Hero::AttackRange` - 判断目标是否在范围内
- **当前血量**: `GameObject::Health` - 判断是否需要撤退
- **是否死亡**: `GameObject::IsDead` - 避免死亡时操作

### 目标选择基址
- **对象管理器**: `Game::ObjectManager` - 遍历所有游戏对象
- **对象列表**: `GameObjectManager::ObjectListPtr` - 获取对象列表
- **对象数量**: `GameObjectManager::ObjectCount` - 获取对象数量
- **对象类型**: `GameObject::Type` - 识别目标类型
- **团队信息**: `GameObject::Team` - 区分敌我
- **是否可见**: `GameObject::IsVisible` - 只攻击可见目标
- **是否可选中**: `GameObject::IsTargetable` - 只攻击可选中目标

## 2. 目标选择器 (Target Selector)

### 目标筛选基址
- **对象位置**: `GameObject::PositionX/Y/Z` - 计算距离
- **目标血量**: `GameObject::Health` - 优先选择低血量目标
- **目标最大血量**: `GameObject::MaxHealth` - 计算血量百分比
- **目标护甲**: `GameObject::Armor` - 计算物理伤害减免
- **目标魔抗**: `GameObject::MagicResist` - 计算魔法伤害减免

### 威胁评估基址
- **目标攻击力**: `GameObject::AttackDamage` - 评估物理威胁
- **目标法强**: `Hero::AbilityPower` - 评估魔法威胁
- **目标技能等级**: `Hero::SpellLevel` - 评估技能威胁
- **目标装备**: `Hero::Inventory` - 评估装备威胁
- **目标状态**: `Hero::StatusFlags` - 判断目标状态(如眩晕、沉默等)

## 3. 技能CD计时 (Spell Cooldown Tracker)

### 技能信息基址
- **技能书**: `Hero::SpellBook` - 获取技能信息
- **技能槽**: `Hero::SpellSlot` - 获取具体技能
- **技能等级**: `Hero::SpellLevel` - 判断技能是否学习
- **技能冷却**: `Hero::SpellCooldown` - 获取当前冷却时间
- **技能总冷却**: `Hero::SpellTotalCooldown` - 获取总冷却时间
- **技能法力消耗**: `Hero::SpellManaCost` - 判断是否有足够法力
- **技能施法时间**: `Hero::SpellCastTime` - 计算施法时间
- **技能范围**: `Hero::SpellRange` - 判断技能是否在范围内
- **技能伤害**: `Hero::SpellDamage` - 计算技能伤害

### 召唤师技能基址
- **召唤师技能信息**: `Hero::SummonerSpellInfo` - 获取召唤师技能
- **技能充能**: `Hero::SpellCharges` - 获取技能充能数
- **最大充能**: `Hero::SpellMaxCharges` - 获取最大充能数
- **充能计时**: `Hero::SpellChargeTimer` - 获取充能计时

## 4. 躲避系统 (Evade System)

### 弹道检测基址
- **弹道对象**: 需要新增 `Missile` 类的基址
- **弹道起始位置**: `Missile::StartPosition` - 获取弹道起点
- **弹道当前位置**: `Missile::CurrentPosition` - 获取弹道当前位置
- **弹道结束位置**: `Missile::EndPosition` - 获取弹道终点
- **弹道速度**: `Missile::Speed` - 计算弹道到达时间
- **弹道施法者**: `Missile::Caster` - 识别弹道来源
- **弹道目标**: `Missile::Target` - 识别弹道目标

### 技能检测基址
- **技能施放事件**: 需要新增 `SpellCastEvent` 相关基址
- **技能名称**: `SpellCastEvent::SpellName` - 识别技能类型
- **技能起始位置**: `SpellCastEvent::StartPosition` - 获取技能起点
- **技能结束位置**: `SpellCastEvent::EndPosition` - 获取技能终点

### 躲避计算基址
- **玩家位置**: `GameObject::PositionX/Y/Z` - 计算躲避路径
- **玩家移动速度**: `GameObject::MovementSpeed` - 计算躲避时间
- **玩家方向**: `GameObject::Direction` - 计算躲避方向
- **玩家状态**: `Hero::StatusFlags` - 判断是否可以移动

## 5. 活化剂使用 (Cleanse/Item Usage)

### 物品系统基址
- **物品栏**: `Hero::Inventory` - 获取物品列表
- **物品ID**: `Hero::ItemId` - 识别物品类型
- **物品数量**: `Hero::ItemStacks` - 获取物品数量
- **物品充能**: `Hero::ItemCharges` - 获取物品充能数
- **物品冷却**: `Hero::ItemCooldown` - 获取物品冷却时间

### 状态检测基址
- **玩家状态**: `Hero::StatusFlags` - 检测控制效果
- **玩家血量**: `GameObject::Health` - 判断是否需要使用治疗
- **玩家法力**: `GameObject::Mana` - 判断是否需要使用法力药水

### 物品使用基址
- **使用物品函数**: 需要新增 `Game::UseItemFunction` - 执行物品使用

## 6. 英雄技能连招 (Champion Combos)

### 英雄特定基址
- **英雄类型**: 根据英雄名称识别不同英雄
- **英雄等级**: `Hero::Level` - 判断技能是否可用
- **英雄经验**: `Hero::Experience` - 计算等级进度

### 连招系统基址
- **技能信息**: `Hero::SpellInfo` - 获取技能详细信息
- **技能范围**: `Hero::SpellRange` - 判断技能是否在范围内
- **技能宽度**: `Hero::SpellWidth` - 计算技能覆盖范围
- **技能速度**: `Hero::SpellSpeed` - 计算技能飞行时间
- **技能施法时间**: `Hero::SpellCastTime` - 计算连招时间

### 目标信息基址
- **目标位置**: `GameObject::PositionX/Y/Z` - 计算技能命中
- **目标移动速度**: `GameObject::MovementSpeed` - 预测目标位置
- **目标状态**: `Hero::StatusFlags` - 判断目标是否易受攻击
- **目标护甲**: `GameObject::Armor` - 计算物理伤害
- **目标魔抗**: `GameObject::MagicResist` - 计算魔法伤害

## 新增基址需求

根据现有基址列表，以下功能需要新增基址：

### 弹道系统
```cpp
namespace Missile {
    constexpr uintptr_t Type = 0x??;          // 弹道类型
    constexpr uintptr_t Caster = 0x??;        // 施法者
    constexpr uintptr_t Target = 0x??;         // 目标
    constexpr uintptr_t StartPositionX = 0x??; // 起始位置X
    constexpr uintptr_t StartPositionY = 0x??; // 起始位置Y
    constexpr uintptr_t StartPositionZ = 0x??; // 起始位置Z
    constexpr uintptr_t CurrentPositionX = 0x??; // 当前位置X
    constexpr uintptr_t CurrentPositionY = 0x??; // 当前位置Y
    constexpr uintptr_t CurrentPositionZ = 0x??; // 当前位置Z
    constexpr uintptr_t EndPositionX = 0x??;   // 结束位置X
    constexpr uintptr_t EndPositionY = 0x??;   // 结束位置Y
    constexpr uintptr_t EndPositionZ = 0x??;   // 结束位置Z
    constexpr uintptr_t Speed = 0x??;          // 弹道速度
    constexpr uintptr_t SpellId = 0x??;        // 技能ID
}
```

### 物品使用函数
```cpp
namespace Game {
    constexpr uintptr_t UseItemFunction = 0x??; // 使用物品函数
}
```

### 技能详细信息
```cpp
namespace Hero {
    constexpr uintptr_t SpellWidth = 0x??;      // 技能宽度
    constexpr uintptr_t SpellSpeed = 0x??;      // 技能速度
    constexpr uintptr_t SpellInfoBase = 0x??;  // 技能信息基址
    constexpr uintptr_t SummonerSpellBase = 0x??; // 召唤师技能基址
}
```

### 状态标志
```cpp
namespace Hero {
    constexpr uintptr_t StatusFlags = 0x??;     // 状态标志
}
```

## 开发优先级建议

1. **走砍系统**: 基础功能，优先开发
2. **目标选择器**: 走砍系统的核心组件
3. **技能CD计时**: 为连招系统提供基础
4. **躲避系统**: 提高生存能力
5. **活化剂使用**: 辅助功能，提高容错率
6. **英雄技能连招**: 高级功能，最后开发

## 注意事项

1. 基址可能会随游戏更新而变化，需要定期更新
2. 部分基址可能需要通过指针链访问，如 `[Base + Offset1] + Offset2`
3. 某些功能可能需要Hook游戏函数来获取更多信息
4. 开发时应考虑性能影响，避免频繁读取内存
5. 部分基址可能在不同游戏版本中有所不同，需要版本检测机制