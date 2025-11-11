# KBot 走砍功能实现总结

## 完成的工作

1. **创建了走砍功能模块**：
   - `Features/Orbwalker.h` - 走砍功能头文件，定义了走砍功能的接口和实现
   - `Features/Orbwalker.cpp` - 走砍功能实现文件，包含走砍逻辑的具体实现

2. **创建了输入管理器**：
   - `Input/InputManager.h` - 输入管理器头文件，用于处理键盘和鼠标输入
   - `Input/InputManager.cpp` - 输入管理器实现文件，用于处理键盘和鼠标输入

3. **创建了基址查找工具**：
   - `Tools/OffsetFinder.h` - 基址查找工具头文件，用于查找走砍功能所需的基址
   - `Tools/OffsetFinder.cpp` - 基址查找工具实现文件，用于查找走砍功能所需的基址
   - `Tools/OffsetFinderExample.cpp` - 基址查找工具示例程序
   - `Tools/OrbwalkerTest.cpp` - 走砍功能测试程序

4. **修改了核心系统**：
   - 修改了 `Core/KBotCore.cpp`，初始化走砍功能并注释掉其他功能
   - 修改了 `Core/KBotCore.h`，添加了输入管理器的包含

5. **创建了文档**：
   - `ORBWALKER_GUIDE.md` - 走砍功能开发指南，帮助用户使用基址查找工具和走砍功能
   - 更新了 `README.md`，添加了走砍功能的相关信息

## 走砍功能的主要特性

1. **多种走砍模式**：
   - 补刀模式 (LAST_HIT)
   - 清线模式 (LANE_CLEAR)
   - 连招模式 (COMBO)
   - 逃跑模式 (FLEE)

2. **多种目标选择策略**：
   - 最少攻击次数 (LESS_ATTACKS)
   - 最低血量 (LOWEST_HEALTH)
   - 最高攻击力 (HIGHEST_AD)
   - 最近距离 (CLOSEST)
   - 靠近鼠标 (NEAR_MOUSE)

3. **可自定义按键设置**：
   - 空格键：连招模式
   - C键：骚扰模式
   - V键：清线模式
   - X键：补刀模式
   - Z键：逃跑模式

4. **精确的攻击和移动时机控制**：
   - 攻击延迟计算
   - 攻击起手时间计算
   - 移动时机控制

## 基址查找工具的主要特性

1. **自动查找关键基址**：
   - 攻击范围
   - 攻击速度
   - 攻击起手时间
   - 英雄状态标志
   - 技能冷却时间
   - 游戏时间

2. **基址有效性验证**：
   - 检查内存可读性
   - 验证基址值的合理性

3. **基址保存功能**：
   - 打印找到的基址
   - 保存基址到文件

## 使用方法

1. **查找基址**：
   ```cpp
   // 初始化基址查找工具
   auto& offsetFinder = KBot::Tools::OffsetFinder::GetInstance();
   offsetFinder.Initialize();
   
   // 查找走砍功能所需的基址
   offsetFinder.FindOrbwalkerOffsets();
   
   // 保存基址到文件
   offsetFinder.SaveOffsetsToFile("orbwalker_offsets.txt");
   ```

2. **使用走砍功能**：
   ```cpp
   // 获取走砍功能实例
   auto orbwalker = KBotCore::GetInstance().GetModule<Features::Orbwalker>("Orbwalker");
   
   // 启用走砍功能
   orbwalker->SetEnabled(true);
   
   // 设置模式
   orbwalker->SetMode(Features::OrbwalkerMode::COMBO);
   ```

## 后续工作

1. **完善基址查找**：
   - 手动验证找到的基址
   - 更新Offsets.h文件

2. **优化走砍逻辑**：
   - 改进目标选择算法
   - 优化攻击和移动时机

3. **添加更多功能**：
   - 目标选择器
   - 技能CD计时
   - 躲避系统
   - 活化剂使用
   - 英雄连招

## 注意事项

1. **基址验证**：
   - 自动查找的基址可能不完全准确
   - 需要手动验证每个基址的正确性
   - 不同游戏版本可能需要不同的基址

2. **性能优化**：
   - 走砍功能需要频繁读取游戏内存
   - 注意优化内存读取频率
   - 适当添加休眠时间，减少CPU使用率

3. **安全性**：
   - 使用基址查找工具时注意游戏反作弊系统
   - 避免频繁修改游戏内存
   - 建议在测试环境中使用

## 总结

我们已经成功实现了KBot的走砍功能，并提供了基址查找工具，帮助用户找到游戏内存中的关键基址。走砍功能支持多种模式和目标选择策略，并提供了可自定义的按键设置。基址查找工具可以自动查找攻击范围、攻击速度、攻击起手时间等关键基址，并提供基址有效性验证和保存功能。

用户可以通过查看`ORBWALKER_GUIDE.md`文件获取更详细的使用指南，并通过运行`Tools/OffsetFinderExample.cpp`和`Tools/OrbwalkerTest.cpp`来测试功能。