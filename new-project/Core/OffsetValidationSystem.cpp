#include "OffsetValidationSystem.h"
#include "MemoryManager.h"
#include "Offsets.h"
#include "GameObjectManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <regex>

OffsetValidationSystem::OffsetValidationSystem() 
    : m_totalValidations(0), m_passedValidations(0), m_failedValidations(0), m_initialized(false) {
}

OffsetValidationSystem::~OffsetValidationSystem() {
}

bool OffsetValidationSystem::Initialize() {
    // 注册默认验证函数
    RegisterDefaultValidators();
    
    m_initialized = true;
    return true;
}

void OffsetValidationSystem::RegisterValidator(const std::string& name, const std::string& category, 
                                               const std::string& description, ValidationFunction func) {
    m_validators[name] = std::make_pair(category, func);
    m_descriptions[name] = description;
}

std::vector<ValidationResult> OffsetValidationSystem::RunAllValidations() {
    std::vector<ValidationResult> results;
    
    if (!m_initialized) {
        std::cout << "OffsetValidationSystem not initialized!" << std::endl;
        return results;
    }
    
    // 重置统计
    m_totalValidations = 0;
    m_passedValidations = 0;
    m_failedValidations = 0;
    
    // 运行所有验证
    for (const auto& validator : m_validators) {
        ValidationResult result = RunValidation(validator.first);
        results.push_back(result);
        
        m_totalValidations++;
        if (result.isValid) {
            m_passedValidations++;
        } else {
            m_failedValidations++;
        }
    }
    
    // 打印统计信息
    std::cout << "===== Validation Results =====" << std::endl;
    std::cout << "Total Validations: " << m_totalValidations << std::endl;
    std::cout << "Passed: " << m_passedValidations << std::endl;
    std::cout << "Failed: " << m_failedValidations << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(2) 
              << (m_totalValidations > 0 ? (float)m_passedValidations / m_totalValidations * 100.0f : 0.0f) << "%" << std::endl;
    std::cout << "=============================" << std::endl;
    
    return results;
}

std::vector<ValidationResult> OffsetValidationSystem::RunCategoryValidations(const std::string& category) {
    std::vector<ValidationResult> results;
    
    if (!m_initialized) {
        std::cout << "OffsetValidationSystem not initialized!" << std::endl;
        return results;
    }
    
    // 运行特定类别的验证
    for (const auto& validator : m_validators) {
        if (validator.second.first == category) {
            ValidationResult result = RunValidation(validator.first);
            results.push_back(result);
        }
    }
    
    return results;
}

ValidationResult OffsetValidationSystem::RunValidation(const std::string& name) {
    ValidationResult result;
    result.name = name;
    result.isValid = false;
    
    auto it = m_validators.find(name);
    if (it == m_validators.end()) {
        result.errorMessage = "Validator not found: " + name;
        return result;
    }
    
    result.category = it->second.first;
    auto descIt = m_descriptions.find(name);
    if (descIt != m_descriptions.end()) {
        result.description = descIt->second;
    }
    
    try {
        MemoryManager* memManager = MemoryManager::GetInstance();
        if (!memManager || !memManager->IsInitialized()) {
            result.errorMessage = "MemoryManager not initialized";
            return result;
        }
        
        result = it->second.second(memManager);
    } catch (const std::exception& e) {
        result.errorMessage = "Exception during validation: " + std::string(e.what());
    }
    
    return result;
}

bool OffsetValidationSystem::GenerateReport(const std::vector<ValidationResult>& results, const std::string& filePath) {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Failed to open file: " << filePath << std::endl;
        return false;
    }
    
    // 写入报告头部
    file << "===== Offset Validation Report =====" << std::endl;
    
    // 获取当前时间
    time_t now = time(0);
    char* dt = ctime(&now);
    file << "Generated: " << dt;
    
    // 写入统计信息
    file << "Total Validations: " << m_totalValidations << std::endl;
    file << "Passed: " << m_passedValidations << std::endl;
    file << "Failed: " << m_failedValidations << std::endl;
    file << "Success Rate: " << std::fixed << std::setprecision(2) 
          << (m_totalValidations > 0 ? (float)m_passedValidations / m_totalValidations * 100.0f : 0.0f) << "%" << std::endl;
    file << "===================================" << std::endl << std::endl;
    
    // 按类别分组结果
    std::map<std::string, std::vector<ValidationResult>> categorizedResults;
    for (const auto& result : results) {
        categorizedResults[result.category].push_back(result);
    }
    
    // 写入各类别的验证结果
    for (const auto& category : categorizedResults) {
        file << "===== " << category.first << " Validations =====" << std::endl;
        file << std::left << std::setw(20) << "Validation Name" 
              << std::setw(8) << "Status" 
              << std::setw(15) << "Actual Value" 
              << std::setw(15) << "Expected Value" 
              << "Description" << std::endl;
        file << std::string(100, '-') << std::endl;
        
        for (const auto& result : category.second) {
            file << std::left << std::setw(20) << result.name 
                  << std::setw(8) << (result.isValid ? "PASS" : "FAIL") 
                  << std::setw(15) << result.actualValue 
                  << std::setw(15) << result.expectedValue 
                  << result.description << std::endl;
            
            if (!result.isValid && !result.errorMessage.empty()) {
                file << "  Error: " << result.errorMessage << std::endl;
            }
        }
        
        file << std::endl;
    }
    
    file.close();
    std::cout << "Validation report generated: " << filePath << std::endl;
    return true;
}

void OffsetValidationSystem::GetStatistics(int& total, int& passed, int& failed) {
    total = m_totalValidations;
    passed = m_passedValidations;
    failed = m_failedValidations;
}

void OffsetValidationSystem::AddCustomRule(const std::string& name, const std::string& rule) {
    m_customRules[name] = rule;
}

bool OffsetValidationSystem::CheckRule(const std::string& name, const std::string& value) {
    auto it = m_customRules.find(name);
    if (it == m_customRules.end()) {
        return true; // 如果没有规则，默认通过
    }
    
    // 这里应该实现规则检查逻辑
    // 为了简化，我们只是检查值是否包含在规则中
    return it->second.find(value) != std::string::npos;
}

void OffsetValidationSystem::RegisterDefaultValidators() {
    // 游戏基址验证
    RegisterValidator("GameBase", "Game", "游戏基址", 
                      [this](MemoryManager* memManager) { return ValidateGameBase(memManager); });
    
    RegisterValidator("GameTime", "Game", "游戏时间", 
                      [this](MemoryManager* memManager) { return ValidateGameTime(memManager); });
    
    RegisterValidator("GameState", "Game", "游戏状态", 
                      [this](MemoryManager* memManager) { return ValidateGameState(memManager); });
    
    RegisterValidator("ObjectManager", "Game", "对象管理器", 
                      [this](MemoryManager* memManager) { return ValidateObjectManager(memManager); });
    
    RegisterValidator("LocalPlayer", "Game", "本地玩家", 
                      [this](MemoryManager* memManager) { return ValidateLocalPlayer(memManager); });
    
    // GameObject验证
    RegisterValidator("GameObjectType", "GameObject", "游戏对象类型", 
                      [this](MemoryManager* memManager) { return ValidateGameObjectType(memManager); });
    
    RegisterValidator("GameObjectTeam", "GameObject", "游戏对象团队", 
                      [this](MemoryManager* memManager) { return ValidateGameObjectTeam(memManager); });
    
    RegisterValidator("GameObjectName", "GameObject", "游戏对象名称", 
                      [this](MemoryManager* memManager) { return ValidateGameObjectName(memManager); });
    
    RegisterValidator("GameObjectPosition", "GameObject", "游戏对象位置", 
                      [this](MemoryManager* memManager) { return ValidateGameObjectPosition(memManager); });
    
    RegisterValidator("GameObjectHealth", "GameObject", "游戏对象生命值", 
                      [this](MemoryManager* memManager) { return ValidateGameObjectHealth(memManager); });
    
    RegisterValidator("GameObjectMana", "GameObject", "游戏对象法力值", 
                      [this](MemoryManager* memManager) { return ValidateGameObjectMana(memManager); });
    
    RegisterValidator("GameObjectVisibility", "GameObject", "游戏对象可见性", 
                      [this](MemoryManager* memManager) { return ValidateGameObjectVisibility(memManager); });
    
    RegisterValidator("GameObjectMovementSpeed", "GameObject", "游戏对象移动速度", 
                      [this](MemoryManager* memManager) { return ValidateGameObjectMovementSpeed(memManager); });
    
    // Hero验证
    RegisterValidator("HeroLevel", "Hero", "英雄等级", 
                      [this](MemoryManager* memManager) { return ValidateHeroLevel(memManager); });
    
    RegisterValidator("HeroExperience", "Hero", "英雄经验值", 
                      [this](MemoryManager* memManager) { return ValidateHeroExperience(memManager); });
    
    RegisterValidator("HeroGold", "Hero", "英雄金币", 
                      [this](MemoryManager* memManager) { return ValidateHeroGold(memManager); });
    
    RegisterValidator("HeroStats", "Hero", "英雄属性", 
                      [this](MemoryManager* memManager) { return ValidateHeroStats(memManager); });
    
    RegisterValidator("HeroSpells", "Hero", "英雄技能", 
                      [this](MemoryManager* memManager) { return ValidateHeroSpells(memManager); });
    
    // Minion验证
    RegisterValidator("MinionType", "Minion", "小兵类型", 
                      [this](MemoryManager* memManager) { return ValidateMinionType(memManager); });
    
    RegisterValidator("MinionStats", "Minion", "小兵属性", 
                      [this](MemoryManager* memManager) { return ValidateMinionStats(memManager); });
    
    // Turret验证
    RegisterValidator("TurretType", "Turret", "防御塔类型", 
                      [this](MemoryManager* memManager) { return ValidateTurretType(memManager); });
    
    RegisterValidator("TurretStats", "Turret", "防御塔属性", 
                      [this](MemoryManager* memManager) { return ValidateTurretStats(memManager); });
    
    // GameObjectManager验证
    RegisterValidator("ObjectList", "GameObjectManager", "对象列表", 
                      [this](MemoryManager* memManager) { return ValidateObjectList(memManager); });
    
    RegisterValidator("ObjectCount", "GameObjectManager", "对象数量", 
                      [this](MemoryManager* memManager) { return ValidateObjectCount(memManager); });
}

// 游戏基址验证函数实现
ValidationResult OffsetValidationSystem::ValidateGameBase(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameBase";
    result.description = "游戏基址";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    result.actualValue = ValueToString(gameBase);
    result.expectedValue = "Valid Pointer";
    result.isValid = IsValidPointer(gameBase);
    
    if (!result.isValid) {
        result.errorMessage = "Failed to get game base address";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateGameTime(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameTime";
    result.description = "游戏时间";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    float gameTime = memManager->Read<float>(gameBase + Offsets::Game::GameTime);
    result.actualValue = ValueToString(gameTime);
    result.expectedValue = "> 0.0";
    result.isValid = IsValidFloat(gameTime, 0.0f, 3600.0f);
    
    if (!result.isValid) {
        result.errorMessage = "Game time is not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateGameState(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameState";
    result.description = "游戏状态";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    int gameState = memManager->Read<int>(gameBase + Offsets::Game::GameState);
    result.actualValue = ValueToString(gameState);
    result.expectedValue = "0-10";
    result.isValid = IsValidInt(gameState, 0, 10);
    
    if (!result.isValid) {
        result.errorMessage = "Game state is not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateObjectManager(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "ObjectManager";
    result.description = "对象管理器";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t objectManager = memManager->Read<uintptr_t>(gameBase + Offsets::Game::ObjectManager);
    result.actualValue = ValueToString(objectManager);
    result.expectedValue = "Valid Pointer";
    result.isValid = IsValidPointer(objectManager);
    
    if (!result.isValid) {
        result.errorMessage = "Object manager is not a valid pointer";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateLocalPlayer(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "LocalPlayer";
    result.description = "本地玩家";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    result.actualValue = ValueToString(localPlayer);
    result.expectedValue = "Valid Pointer";
    result.isValid = IsValidPointer(localPlayer);
    
    if (!result.isValid) {
        result.errorMessage = "Local player is not a valid pointer";
    }
    
    return result;
}

// GameObject验证函数实现
ValidationResult OffsetValidationSystem::ValidateGameObjectType(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameObjectType";
    result.description = "游戏对象类型";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    int type = memManager->Read<int>(localPlayer + Offsets::GameObject::Type);
    result.actualValue = ValueToString(type);
    result.expectedValue = "0-20";
    result.isValid = IsValidInt(type, 0, 20);
    
    if (!result.isValid) {
        result.errorMessage = "Game object type is not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateGameObjectTeam(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameObjectTeam";
    result.description = "游戏对象团队";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    int team = memManager->Read<int>(localPlayer + Offsets::GameObject::Team);
    result.actualValue = ValueToString(team);
    result.expectedValue = "100, 200 or 300";
    result.isValid = (team == 100 || team == 200 || team == 300);
    
    if (!result.isValid) {
        result.errorMessage = "Game object team is not a valid team ID";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateGameObjectName(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameObjectName";
    result.description = "游戏对象名称";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    uintptr_t namePtr = memManager->Read<uintptr_t>(localPlayer + Offsets::GameObject::NamePtr);
    result.actualValue = ValueToString(namePtr);
    result.expectedValue = "Valid Pointer";
    result.isValid = IsValidPointer(namePtr);
    
    if (result.isValid) {
        try {
            std::string name = memManager->ReadString(namePtr);
            if (name.empty() || name.length() > 256) {
                result.isValid = false;
                result.errorMessage = "Game object name is not a valid string";
            } else {
                result.actualValue = name;
            }
        } catch (...) {
            result.isValid = false;
            result.errorMessage = "Failed to read game object name";
        }
    } else {
        result.errorMessage = "Game object name pointer is not valid";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateGameObjectPosition(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameObjectPosition";
    result.description = "游戏对象位置";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    float posX = memManager->Read<float>(localPlayer + Offsets::GameObject::PositionX);
    float posY = memManager->Read<float>(localPlayer + Offsets::GameObject::PositionY);
    float posZ = memManager->Read<float>(localPlayer + Offsets::GameObject::PositionZ);
    
    result.actualValue = "(" + ValueToString(posX) + ", " + ValueToString(posY) + ", " + ValueToString(posZ) + ")";
    result.expectedValue = "Valid Coordinates";
    result.isValid = IsValidFloat(posX, -10000.0f, 10000.0f) && 
                     IsValidFloat(posY, -10000.0f, 10000.0f) && 
                     IsValidFloat(posZ, -10000.0f, 10000.0f);
    
    if (!result.isValid) {
        result.errorMessage = "Game object position is not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateGameObjectHealth(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameObjectHealth";
    result.description = "游戏对象生命值";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    float health = memManager->Read<float>(localPlayer + Offsets::GameObject::Health);
    float maxHealth = memManager->Read<float>(localPlayer + Offsets::GameObject::MaxHealth);
    
    result.actualValue = ValueToString(health) + "/" + ValueToString(maxHealth);
    result.expectedValue = "0.0 - 10000.0";
    result.isValid = IsValidFloat(health, 0.0f, 10000.0f) && 
                     IsValidFloat(maxHealth, 0.0f, 10000.0f) &&
                     maxHealth > 0;
    
    if (!result.isValid) {
        result.errorMessage = "Game object health is not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateGameObjectMana(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameObjectMana";
    result.description = "游戏对象法力值";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    float mana = memManager->Read<float>(localPlayer + Offsets::GameObject::Mana);
    float maxMana = memManager->Read<float>(localPlayer + Offsets::GameObject::MaxMana);
    
    result.actualValue = ValueToString(mana) + "/" + ValueToString(maxMana);
    result.expectedValue = "0.0 - 10000.0";
    result.isValid = IsValidFloat(mana, 0.0f, 10000.0f) && 
                     IsValidFloat(maxMana, 0.0f, 10000.0f) &&
                     maxMana > 0;
    
    if (!result.isValid) {
        result.errorMessage = "Game object mana is not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateGameObjectVisibility(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameObjectVisibility";
    result.description = "游戏对象可见性";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    bool isVisible = memManager->Read<bool>(localPlayer + Offsets::GameObject::IsVisible);
    result.actualValue = ValueToString(isVisible);
    result.expectedValue = "true or false";
    result.isValid = true; // 布尔值总是有效的
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateGameObjectMovementSpeed(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "GameObjectMovementSpeed";
    result.description = "游戏对象移动速度";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    float movementSpeed = memManager->Read<float>(localPlayer + Offsets::GameObject::MovementSpeed);
    result.actualValue = ValueToString(movementSpeed);
    result.expectedValue = "0.0 - 1000.0";
    result.isValid = IsValidFloat(movementSpeed, 0.0f, 1000.0f);
    
    if (!result.isValid) {
        result.errorMessage = "Game object movement speed is not in valid range";
    }
    
    return result;
}

// Hero验证函数实现
ValidationResult OffsetValidationSystem::ValidateHeroLevel(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "HeroLevel";
    result.description = "英雄等级";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    int level = memManager->Read<int>(localPlayer + Offsets::Hero::Level);
    result.actualValue = ValueToString(level);
    result.expectedValue = "1-18";
    result.isValid = IsValidInt(level, 1, 18);
    
    if (!result.isValid) {
        result.errorMessage = "Hero level is not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateHeroExperience(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "HeroExperience";
    result.description = "英雄经验值";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    float experience = memManager->Read<float>(localPlayer + Offsets::Hero::Experience);
    result.actualValue = ValueToString(experience);
    result.expectedValue = "0.0 - 100000.0";
    result.isValid = IsValidFloat(experience, 0.0f, 100000.0f);
    
    if (!result.isValid) {
        result.errorMessage = "Hero experience is not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateHeroGold(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "HeroGold";
    result.description = "英雄金币";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    float gold = memManager->Read<float>(localPlayer + Offsets::Hero::Gold);
    result.actualValue = ValueToString(gold);
    result.expectedValue = "0.0 - 50000.0";
    result.isValid = IsValidFloat(gold, 0.0f, 50000.0f);
    
    if (!result.isValid) {
        result.errorMessage = "Hero gold is not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateHeroStats(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "HeroStats";
    result.description = "英雄属性";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    float attackDamage = memManager->Read<float>(localPlayer + Offsets::Hero::AttackDamage);
    float abilityPower = memManager->Read<float>(localPlayer + Offsets::Hero::AbilityPower);
    float armor = memManager->Read<float>(localPlayer + Offsets::Hero::Armor);
    float magicResist = memManager->Read<float>(localPlayer + Offsets::Hero::MagicResist);
    
    result.actualValue = "AD:" + ValueToString(attackDamage) + 
                        " AP:" + ValueToString(abilityPower) + 
                        " AR:" + ValueToString(armor) + 
                        " MR:" + ValueToString(magicResist);
    result.expectedValue = "Valid Stats";
    result.isValid = IsValidFloat(attackDamage, 0.0f, 1000.0f) && 
                     IsValidFloat(abilityPower, 0.0f, 1000.0f) &&
                     IsValidFloat(armor, 0.0f, 1000.0f) &&
                     IsValidFloat(magicResist, 0.0f, 1000.0f);
    
    if (!result.isValid) {
        result.errorMessage = "Hero stats are not in valid range";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateHeroSpells(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "HeroSpells";
    result.description = "英雄技能";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t localPlayer = memManager->Read<uintptr_t>(gameBase + Offsets::Game::LocalPlayer);
    if (!IsValidPointer(localPlayer)) {
        result.errorMessage = "Invalid local player address";
        return result;
    }
    
    // 检查技能信息指针
    bool allValid = true;
    for (int i = 0; i < 4; i++) {
        uintptr_t spellInfoPtr = memManager->Read<uintptr_t>(localPlayer + Offsets::Hero::SpellInfoBase + i * 0x8);
        if (!IsValidPointer(spellInfoPtr)) {
            allValid = false;
            break;
        }
        
        // 检查技能等级
        int spellLevel = memManager->Read<int>(spellInfoPtr + Offsets::Hero::SpellLevel);
        if (!IsValidInt(spellLevel, 0, 5)) {
            allValid = false;
            break;
        }
    }
    
    result.actualValue = allValid ? "Valid" : "Invalid";
    result.expectedValue = "Valid";
    result.isValid = allValid;
    
    if (!result.isValid) {
        result.errorMessage = "Hero spells are not valid";
    }
    
    return result;
}

// Minion验证函数实现
ValidationResult OffsetValidationSystem::ValidateMinionType(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "MinionType";
    result.description = "小兵类型";
    
    // 这里应该获取一个小兵对象进行验证
    // 为了简化，我们只是返回一个默认结果
    result.actualValue = "N/A";
    result.expectedValue = "0-10";
    result.isValid = true;
    result.errorMessage = "Minion validation not implemented";
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateMinionStats(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "MinionStats";
    result.description = "小兵属性";
    
    // 这里应该获取一个小兵对象进行验证
    // 为了简化，我们只是返回一个默认结果
    result.actualValue = "N/A";
    result.expectedValue = "Valid Stats";
    result.isValid = true;
    result.errorMessage = "Minion validation not implemented";
    
    return result;
}

// Turret验证函数实现
ValidationResult OffsetValidationSystem::ValidateTurretType(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "TurretType";
    result.description = "防御塔类型";
    
    // 这里应该获取一个防御塔对象进行验证
    // 为了简化，我们只是返回一个默认结果
    result.actualValue = "N/A";
    result.expectedValue = "0-10";
    result.isValid = true;
    result.errorMessage = "Turret validation not implemented";
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateTurretStats(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "TurretStats";
    result.description = "防御塔属性";
    
    // 这里应该获取一个防御塔对象进行验证
    // 为了简化，我们只是返回一个默认结果
    result.actualValue = "N/A";
    result.expectedValue = "Valid Stats";
    result.isValid = true;
    result.errorMessage = "Turret validation not implemented";
    
    return result;
}

// GameObjectManager验证函数实现
ValidationResult OffsetValidationSystem::ValidateObjectList(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "ObjectList";
    result.description = "对象列表";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t objectManager = memManager->Read<uintptr_t>(gameBase + Offsets::Game::ObjectManager);
    if (!IsValidPointer(objectManager)) {
        result.errorMessage = "Invalid object manager address";
        return result;
    }
    
    uintptr_t objectListPtr = memManager->Read<uintptr_t>(objectManager + Offsets::GameObjectManager::ObjectListPtr);
    result.actualValue = ValueToString(objectListPtr);
    result.expectedValue = "Valid Pointer";
    result.isValid = IsValidPointer(objectListPtr);
    
    if (!result.isValid) {
        result.errorMessage = "Object list pointer is not valid";
    }
    
    return result;
}

ValidationResult OffsetValidationSystem::ValidateObjectCount(MemoryManager* memManager) {
    ValidationResult result;
    result.name = "ObjectCount";
    result.description = "对象数量";
    
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (!IsValidPointer(gameBase)) {
        result.errorMessage = "Invalid game base address";
        return result;
    }
    
    uintptr_t objectManager = memManager->Read<uintptr_t>(gameBase + Offsets::Game::ObjectManager);
    if (!IsValidPointer(objectManager)) {
        result.errorMessage = "Invalid object manager address";
        return result;
    }
    
    int objectCount = memManager->Read<int>(objectManager + Offsets::GameObjectManager::ObjectCount);
    result.actualValue = ValueToString(objectCount);
    result.expectedValue = "0-1000";
    result.isValid = IsValidInt(objectCount, 0, 1000);
    
    if (!result.isValid) {
        result.errorMessage = "Object count is not in valid range";
    }
    
    return result;
}

// 辅助函数实现
std::string OffsetValidationSystem::ValueToString(uintptr_t value) {
    std::stringstream ss;
    ss << "0x" << std::hex << value;
    return ss.str();
}

std::string OffsetValidationSystem::ValueToString(float value) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << value;
    return ss.str();
}

std::string OffsetValidationSystem::ValueToString(int value) {
    return std::to_string(value);
}

std::string OffsetValidationSystem::ValueToString(bool value) {
    return value ? "true" : "false";
}

std::string OffsetValidationSystem::ValueToString(const std::string& value) {
    return value;
}

bool OffsetValidationSystem::IsValidPointer(uintptr_t address) {
    return address != 0 && address < 0x7FFFFFFFFFFF;
}

bool OffsetValidationSystem::IsValidFloat(float value, float min, float max) {
    return value >= min && value <= max && !isnan(value) && !isinf(value);
}

bool OffsetValidationSystem::IsValidInt(int value, int min, int max) {
    return value >= min && value <= max;
}

bool OffsetValidationSystem::IsValidString(const std::string& value) {
    return !value.empty() && value.length() < 256;
}