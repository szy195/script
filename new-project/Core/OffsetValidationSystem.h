#pragma once

#include <string>
#include <vector>
#include <functional>
#include <map>

// 前向声明
class MemoryManager;

// 验证结果结构体
struct ValidationResult {
    std::string name;           // 验证项名称
    std::string category;       // 类别
    std::string description;    // 描述
    bool isValid;               // 是否有效
    std::string actualValue;    // 实际值
    std::string expectedValue;  // 预期值
    std::string errorMessage;   // 错误信息（如果无效）
};

// 验证函数类型
using ValidationFunction = std::function<ValidationResult(MemoryManager*)>;

// 基址验证系统
class OffsetValidationSystem {
public:
    OffsetValidationSystem();
    ~OffsetValidationSystem();
    
    // 初始化验证系统
    bool Initialize();
    
    // 注册验证函数
    void RegisterValidator(const std::string& name, const std::string& category, 
                          const std::string& description, ValidationFunction func);
    
    // 运行所有验证
    std::vector<ValidationResult> RunAllValidations();
    
    // 运行特定类别的验证
    std::vector<ValidationResult> RunCategoryValidations(const std::string& category);
    
    // 运行特定验证
    ValidationResult RunValidation(const std::string& name);
    
    // 生成验证报告
    bool GenerateReport(const std::vector<ValidationResult>& results, const std::string& filePath);
    
    // 获取验证统计信息
    void GetStatistics(int& total, int& passed, int& failed);
    
    // 添加自定义验证规则
    void AddCustomRule(const std::string& name, const std::string& rule);
    
    // 检查验证规则
    bool CheckRule(const std::string& name, const std::string& value);
    
private:
    // 注册默认验证函数
    void RegisterDefaultValidators();
    
    // 游戏基址验证函数
    ValidationResult ValidateGameBase(MemoryManager* memManager);
    ValidationResult ValidateGameTime(MemoryManager* memManager);
    ValidationResult ValidateGameState(MemoryManager* memManager);
    ValidationResult ValidateObjectManager(MemoryManager* memManager);
    ValidationResult ValidateLocalPlayer(MemoryManager* memManager);
    
    // GameObject验证函数
    ValidationResult ValidateGameObjectType(MemoryManager* memManager);
    ValidationResult ValidateGameObjectTeam(MemoryManager* memManager);
    ValidationResult ValidateGameObjectName(MemoryManager* memManager);
    ValidationResult ValidateGameObjectPosition(MemoryManager* memManager);
    ValidationResult ValidateGameObjectHealth(MemoryManager* memManager);
    ValidationResult ValidateGameObjectMana(MemoryManager* memManager);
    ValidationResult ValidateGameObjectVisibility(MemoryManager* memManager);
    ValidationResult ValidateGameObjectMovementSpeed(MemoryManager* memManager);
    
    // Hero验证函数
    ValidationResult ValidateHeroLevel(MemoryManager* memManager);
    ValidationResult ValidateHeroExperience(MemoryManager* memManager);
    ValidationResult ValidateHeroGold(MemoryManager* memManager);
    ValidationResult ValidateHeroStats(MemoryManager* memManager);
    ValidationResult ValidateHeroSpells(MemoryManager* memManager);
    
    // Minion验证函数
    ValidationResult ValidateMinionType(MemoryManager* memManager);
    ValidationResult ValidateMinionStats(MemoryManager* memManager);
    
    // Turret验证函数
    ValidationResult ValidateTurretType(MemoryManager* memManager);
    ValidationResult ValidateTurretStats(MemoryManager* memManager);
    
    // GameObjectManager验证函数
    ValidationResult ValidateObjectList(MemoryManager* memManager);
    ValidationResult ValidateObjectCount(MemoryManager* memManager);
    
    // 辅助函数
    std::string ValueToString(uintptr_t value);
    std::string ValueToString(float value);
    std::string ValueToString(int value);
    std::string ValueToString(bool value);
    std::string ValueToString(const std::string& value);
    
    bool IsValidPointer(uintptr_t address);
    bool IsValidFloat(float value, float min, float max);
    bool IsValidInt(int value, int min, int max);
    bool IsValidString(const std::string& value);
    
    // 成员变量
    std::map<std::string, std::pair<std::string, ValidationFunction>> m_validators; // name -> (category, function)
    std::map<std::string, std::string> m_descriptions; // name -> description
    std::map<std::string, std::string> m_customRules; // name -> rule
    
    int m_totalValidations;
    int m_passedValidations;
    int m_failedValidations;
    
    bool m_initialized;
};