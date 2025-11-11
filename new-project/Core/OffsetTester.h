#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <memory>
#include <cstdint>
#include <iostream>
#include <iomanip>

#include "../Core/MemoryManager.h"
#include "../Core/Offsets.h"
#include "../Game/GameObjectManager.h"
#include "../Game/Hero.h"

// 测试结果结构体
struct OffsetTestResult {
    std::string offsetName;
    std::string category;
    uintptr_t offset;
    bool isValid;
    std::string description;
    std::string actualValue;
    std::string expectedValue;
};

// 基址测试器类
class OffsetTester {
public:
    OffsetTester();
    ~OffsetTester();
    
    // 初始化测试器
    bool Initialize();
    
    // 运行所有测试
    std::vector<OffsetTestResult> RunAllTests();
    
    // 运行特定类别的测试
    std::vector<OffsetTestResult> RunCategoryTests(const std::string& category);
    
    // 测试游戏基址
    std::vector<OffsetTestResult> TestGameOffsets();
    
    // 测试GameObject偏移量
    std::vector<OffsetTestResult> TestGameObjectOffsets();
    
    // 测试Hero偏移量
    std::vector<OffsetTestResult> TestHeroOffsets();
    
    // 测试Minion偏移量
    std::vector<OffsetTestResult> TestMinionOffsets();
    
    // 测试Turret偏移量
    std::vector<OffsetTestResult> TestTurretOffsets();
    
    // 测试GameObjectManager偏移量
    std::vector<OffsetTestResult> TestGameObjectManagerOffsets();
    
    // 验证本地玩家对象
    bool ValidateLocalPlayer();
    
    // 验证对象列表
    bool ValidateObjectList();
    
    // 生成测试报告
    bool GenerateReport(const std::vector<OffsetTestResult>& results, const std::string& filePath);
    
    // 保存当前偏移量配置
    bool SaveCurrentOffsets(const std::string& filePath);
    
    // 加载偏移量配置
    bool LoadOffsets(const std::string& filePath);
    
    // 比较两组偏移量
    std::vector<std::string> CompareOffsets(const std::string& oldFile, const std::string& newFile);
    
private:
    // 辅助方法
    std::string ValueToString(uintptr_t value);
    std::string ValueToString(float value);
    std::string ValueToString(int value);
    std::string ValueToString(bool value);
    std::string ValueToString(const std::string& value);
    
    // 验证指针是否有效
    bool IsValidPointer(uintptr_t address);
    
    // 验证字符串是否有效
    bool IsValidString(uintptr_t address);
    
    // 验证浮点数是否在合理范围内
    bool IsValidFloat(float value, float min = -10000.0f, float max = 10000.0f);
    
    // 验证整数是否在合理范围内
    bool IsValidInt(int value, int min = -100000, int max = 100000);
    
    // 成员变量
    MemoryManager* m_memManager;
    GameObjectManager* m_gameObjectManager;
    bool m_initialized;
    
    // 测试统计
    int m_totalTests;
    int m_passedTests;
    int m_failedTests;
    
    // 偏移量映射
    std::map<std::string, uintptr_t> m_currentOffsets;
};