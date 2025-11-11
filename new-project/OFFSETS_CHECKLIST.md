# KBot功能开发基址需求清单

## 1. 走砍系统 (Orbwalker)

### 已有基址
- [x] `Game::LocalPlayer` - 本地玩家
- [x] `Game::MoveFunction` - 移动函数
- [x] `Game::AttackFunction` - 攻击函数
- [x] `Game::GameTime` - 游戏时间
- [x] `GameObject::PositionX/Y/Z` - 位置坐标
- [x] `GameObject::AttackSpeed` - 攻击速度
- [x] `GameObject::MovementSpeed` - 移动速度
- [x] `GameObject::IsDead` - 死亡状态
- [x] `GameObject::Team` - 团队信息
- [x] `GameObject::IsVisible` - 可见状态
- [x] `GameObject::IsTargetable` - 可选中状态
- [x] `GameObjectManager::ObjectListPtr` - 对象列表
- [x] `GameObjectManager::ObjectCount` - 对象数量

### 需要补充
- [ ] `Hero::AttackRange` - 攻击范围 (当前Offsets.h中未定义)

## 2. 目标选择器 (Target Selector)

### 已有基址
- [x] `GameObject::Health` - 当前血量
- [x] `GameObject::MaxHealth` - 最大血量
- [x] `GameObject::Armor` - 护甲
- [x] `GameObject::MagicResist` - 魔抗
- [x] `GameObject::AttackDamage` - 攻击力
- [x] `Hero::AbilityPower` - 法强
- [x] `Hero::Inventory` - 装备栏

### 需要补充
- [ ] `Hero::StatusFlags` - 状态标志 (当前Offsets.h中未定义)

## 3. 技能CD计时 (Spell Cooldown Tracker)

### 已有基址
- [x] `Hero::SpellBook` - 技能书
- [x] `Hero::SpellLevel` - 技能等级
- [x] `Hero::SpellCooldown` - 技能冷却
- [x] `Hero::SpellTotalCooldown` - 技能总冷却
- [x] `Hero::SpellManaCost` - 技能法力消耗
- [x] `Hero::SpellCastTime` - 技能施法时间
- [x] `Hero::SpellRange` - 技能范围
- [x] `Hero::SpellDamage` - 技能伤害
- [x] `Hero::SpellCharges` - 技能充能
- [x] `Hero::SpellMaxCharges` - 最大充能
- [x] `Hero::SpellChargeTimer` - 充能计时
- [x] `Hero::SummonerSpellInfo` - 召唤师技能信息

## 4. 躲避系统 (Evade System)

### 已有基址
- [x] `GameObject::PositionX/Y/Z` - 位置坐标
- [x] `GameObject::MovementSpeed` - 移动速度
- [x] `GameObject::Direction` - 移动方向

### 需要补充
- [ ] 整个 `Missile` 命名空间的所有基址
- [ ] `SpellCastEvent` 相关基址
- [ ] `Hero::StatusFlags` - 状态标志

## 5. 活化剂使用 (Cleanse/Item Usage)

### 已有基址
- [x] `Hero::Inventory` - 物品栏
- [x] `Hero::ItemId` - 物品ID
- [x] `Hero::ItemStacks` - 物品数量
- [x] `Hero::ItemCharges` - 物品充能
- [x] `Hero::ItemCooldown` - 物品冷却
- [x] `GameObject::Health` - 血量
- [x] `GameObject::Mana` - 法力值

### 需要补充
- [ ] `Game::UseItemFunction` - 使用物品函数
- [ ] `Hero::StatusFlags` - 状态标志

## 6. 英雄技能连招 (Champion Combos)

### 已有基址
- [x] `Hero::Level` - 英雄等级
- [x] `Hero::Experience` - 英雄经验
- [x] `Hero::SpellCooldown` - 技能冷却
- [x] `Hero::SpellRange` - 技能范围
- [x] `Hero::SpellCastTime` - 技能施法时间
- [x] `GameObject::PositionX/Y/Z` - 目标位置
- [x] `GameObject::MovementSpeed` - 目标移动速度
- [x] `GameObject::Armor` - 目标护甲
- [x] `GameObject::MagicResist` - 目标魔抗

### 需要补充
- [ ] `Hero::SpellWidth` - 技能宽度
- [ ] `Hero::SpellSpeed` - 技能速度
- [ ] `Hero::StatusFlags` - 目标状态标志

## 关键缺失基址总结

1. `Hero::AttackRange` - 攻击范围
2. `Hero::StatusFlags` - 状态标志 (多个功能需要)
3. `Game::UseItemFunction` - 使用物品函数
4. `Hero::SpellWidth` - 技能宽度
5. `Hero::SpellSpeed` - 技能速度
6. 整个 `Missile` 命名空间的所有基址 (躲避系统核心)

## 优先级建议

1. **第一优先级**: 补充 `Hero::AttackRange` 和 `Hero::StatusFlags` (走砍和目标选择必需)
2. **第二优先级**: 补充 `Game::UseItemFunction` (活化剂使用必需)
3. **第三优先级**: 补充 `Hero::SpellWidth` 和 `Hero::SpellSpeed` (连招系统增强)
4. **第四优先级**: 补充整个 `Missile` 命名空间 (躲避系统)

## 基址查找技巧

1. 使用CE (Cheat Engine) 搜索已知值 (如血量、法力值)
2. 通过指针链查找基址
3. 分析游戏汇编代码找到关键函数
4. 使用项目中的 `OffsetComparator` 和 `OffsetTester` 工具
5. 参考其他开源项目的基址 (注意版本差异)