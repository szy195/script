#pragma once

#include "GameObject.h"
#include "../Core/Offsets.h"
#include "../Utils/Math.h"
#include <string>

// 小兵类型枚举
enum class MinionType {
    Unknown = 0,
    Melee = 1,      // 近战小兵
    Ranged = 2,     // 远程小兵
    Siege = 3,      // 攻城车
    Super = 4,      // 超级兵
    Pet = 5,        // 宠物（如安妮的熊、女枪的鸟等）
    Jungle = 6,     // 野怪
    Ward = 7        // 守卫
};

// 小兵类
class Minion : public GameObject {
private:
    MinionType minionType;
    GameObjectTeam ownerTeam;
    bool isWard;
    bool isPet;
    bool isJungle;
    float attackDamage;
    float attackSpeed;
    float armor;
    float magicResist;
    float healthRegen;
    float manaRegen;
    float visionRadius;
    
public:
    // 构造函数
    Minion(uintptr_t addr = 0) : GameObject(addr), minionType(MinionType::Unknown),
        ownerTeam(GameObjectTeam::Unknown), isWard(false), isPet(false), isJungle(false),
        attackDamage(0.0f), attackSpeed(0.0f), armor(0.0f), magicResist(0.0f),
        healthRegen(0.0f), manaRegen(0.0f), visionRadius(0.0f) {
        
        // 设置默认类型为小兵
        SetType(GameObjectType::AIMinionClient);
    }
    
    // 更新小兵数据
    void UpdateData() override {
        GameObject::UpdateData();
        
        if (address == 0) {
            return;
        }
        
        MemoryManager* memManager = MemoryManager::GetInstance();
        if (!memManager) {
            return;
        }
        
        // 读取小兵特有属性
        if (!memManager->Read(address + Offsets::Minion::AttackDamage, attackDamage)) {
            attackDamage = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Minion::AttackSpeed, attackSpeed)) {
            attackSpeed = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Minion::Armor, armor)) {
            armor = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Minion::MagicResist, magicResist)) {
            magicResist = 0.0f;
        }
        
        // 读取其他属性
        if (!memManager->Read(address + Offsets::Minion::HealthRegen, healthRegen)) {
            healthRegen = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Minion::ManaRegen, manaRegen)) {
            manaRegen = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Minion::VisionRadius, visionRadius)) {
            visionRadius = 0.0f;
        }
        
        // 根据名称确定小兵类型
        minionType = GetMinionTypeFromName(name);
        ownerTeam = GetTeamFromName(name);
        
        // 设置特殊标志
        isWard = (minionType == MinionType::Ward);
        isPet = (minionType == MinionType::Pet);
        isJungle = (minionType == MinionType::Jungle);
        
        // 根据类型设置攻击范围
        SetAttackRangeByType();
    }
    
    // 获取小兵类型
    MinionType GetMinionType() const {
        return minionType;
    }
    
    // 获取小兵类型名称
    std::string GetMinionTypeName() const {
        return GetMinionTypeName(minionType);
    }
    
    // 获取所属队伍
    GameObjectTeam GetOwnerTeam() const {
        return ownerTeam;
    }
    
    // 是否是守卫
    bool IsWard() const {
        return isWard;
    }
    
    // 是否是宠物
    bool IsPet() const {
        return isPet;
    }
    
    // 是否是野怪
    bool IsJungle() const {
        return isJungle;
    }
    
    // 是否是敌方小兵
    bool IsEnemy(const GameObject& other) const {
        return ownerTeam != other.GetTeam() && ownerTeam != GameObjectTeam::Neutral;
    }
    
    // 是否是友方小兵
    bool IsAlly(const GameObject& other) const {
        return ownerTeam == other.GetTeam() && ownerTeam != GameObjectTeam::Neutral;
    }
    
    // 获取攻击力
    float GetAttackDamage() const {
        return attackDamage;
    }
    
    // 获取攻击速度
    float GetAttackSpeed() const {
        return attackSpeed;
    }
    
    // 获取护甲
    float GetArmor() const {
        return armor;
    }
    
    // 获取魔抗
    float GetMagicResist() const {
        return magicResist;
    }
    
    // 获取生命回复
    float GetHealthRegen() const {
        return healthRegen;
    }
    
    // 获取法力回复
    float GetManaRegen() const {
        return manaRegen;
    }
    
    // 获取视野范围
    float GetVisionRadius() const {
        return visionRadius;
    }
    
    // 是否是最后一个小兵
    bool IsLastMinion() const {
        // 这里需要根据游戏状态判断是否是最后一个小兵
        // 在实际项目中，需要更复杂的逻辑
        return false;
    }
    
    // 是否是重要目标（如攻城车）
    bool IsPriorityTarget() const {
        return minionType == MinionType::Siege || minionType == MinionType::Super;
    }
    
    // 是否应该被攻击
    bool ShouldAttack(const Hero& attacker) const {
        // 守卫不应该被攻击
        if (isWard) {
            return false;
        }
        
        // 宠物通常不应该被攻击
        if (isPet) {
            return false;
        }
        
        // 敌方小兵应该被攻击
        if (IsEnemy(attacker)) {
            return true;
        }
        
        // 野怪应该被攻击
        if (isJungle) {
            return true;
        }
        
        return false;
    }
    
    // 获取字符串表示
    std::string ToString() const {
        std::string result = "Minion[";
        result += GameObject::ToString();
        result += ", Type: " + GetMinionTypeName();
        result += ", Team: " + std::to_string(static_cast<int>(ownerTeam));
        result += ", AD: " + std::to_string(attackDamage);
        result += "]";
        return result;
    }
    
private:
    // 根据名称获取小兵类型
    static MinionType GetMinionTypeFromName(const std::string& name) {
        // 近战小兵
        if (name.find("SRU_OrderMinionMelee") != std::string::npos ||
            name.find("SRU_ChaosMinionMelee") != std::string::npos) {
            return MinionType::Melee;
        }
        
        // 远程小兵
        if (name.find("SRU_OrderMinionRanged") != std::string::npos ||
            name.find("SRU_ChaosMinionRanged") != std::string::npos) {
            return MinionType::Ranged;
        }
        
        // 攻城车
        if (name.find("SRU_OrderMinionSiege") != std::string::npos ||
            name.find("SRU_ChaosMinionSiege") != std::string::npos) {
            return MinionType::Siege;
        }
        
        // 超级兵
        if (name.find("SRU_OrderMinionSuper") != std::string::npos ||
            name.find("SRU_ChaosMinionSuper") != std::string::npos) {
            return MinionType::Super;
        }
        
        // 守卫
        if (name.find("SightWard") != std::string::npos ||
            name.find("VisionWard") != std::string::npos ||
            name.find("YellowTrinket") != std::string::npos ||
            name.find("BlueTrinket") != std::string::npos) {
            return MinionType::Ward;
        }
        
        // 宠物
        if (name.find("AnnieTibbers") != std::string::npos ||
            name.find("JhinCait") != std::string::npos ||
            name.find("ShacoBox") != std::string::npos ||
            name.find("MalzaharVoidling") != std::string::npos) {
            return MinionType::Pet;
        }
        
        // 野怪
        if (name.find("SRU_") != std::string::npos) {
            return MinionType::Jungle;
        }
        
        return MinionType::Unknown;
    }
    
    // 根据名称获取队伍
    static GameObjectTeam GetTeamFromName(const std::string& name) {
        if (name.find("Order") != std::string::npos) {
            return GameObjectTeam::Blue;
        }
        
        if (name.find("Chaos") != std::string::npos) {
            return GameObjectTeam::Red;
        }
        
        if (name.find("Neutral") != std::string::npos) {
            return GameObjectTeam::Neutral;
        }
        
        return GameObjectTeam::Unknown;
    }
    
    // 获取小兵类型名称
    static std::string GetMinionTypeName(MinionType type) {
        switch (type) {
            case MinionType::Melee: return "Melee";
            case MinionType::Ranged: return "Ranged";
            case MinionType::Siege: return "Siege";
            case MinionType::Super: return "Super";
            case MinionType::Pet: return "Pet";
            case MinionType::Jungle: return "Jungle";
            case MinionType::Ward: return "Ward";
            default: return "Unknown";
        }
    }
    
    // 根据类型设置攻击范围
    void SetAttackRangeByType() {
        switch (minionType) {
            case MinionType::Melee:
                SetAttackRange(125.0f); // 近战小兵攻击范围
                break;
            case MinionType::Ranged:
                SetAttackRange(300.0f); // 远程小兵攻击范围
                break;
            case MinionType::Siege:
                SetAttackRange(350.0f); // 攻城车攻击范围
                break;
            case MinionType::Super:
                SetAttackRange(150.0f); // 超级兵攻击范围
                break;
            case MinionType::Jungle:
                SetAttackRange(300.0f); // 野怪攻击范围
                break;
            case MinionType::Ward:
                SetAttackRange(0.0f); // 守卫不攻击
                break;
            case MinionType::Pet:
                SetAttackRange(250.0f); // 宠物攻击范围
                break;
            default:
                SetAttackRange(200.0f); // 默认攻击范围
                break;
        }
    }
};