#pragma once

#include "GameObject.h"
#include "../Core/Offsets.h"
#include "../Utils/Math.h"
#include <string>
#include <vector>

// 防御塔类型枚举
enum class TurretType {
    Unknown = 0,
    Outer = 1,      // 外塔
    Inner = 2,      // 内塔
    Inhibitor = 3,  // 水晶塔
    Nexus = 4,      // 基地塔
    Fountain = 5    // 泉水塔
};

// 防御塔类
class Turret : public GameObject {
private:
    TurretType turretType;
    GameObjectTeam ownerTeam;
    float attackDamage;
    float attackSpeed;
    float armor;
    float magicResist;
    float healthRegen;
    float visionRadius;
    bool isInvulnerable;
    bool isTargetable;
    
public:
    // 构造函数
    Turret(uintptr_t addr = 0) : GameObject(addr), turretType(TurretType::Unknown),
        ownerTeam(GameObjectTeam::Unknown), attackDamage(0.0f), attackSpeed(0.0f),
        armor(0.0f), magicResist(0.0f), healthRegen(0.0f), visionRadius(0.0f),
        isInvulnerable(false), isTargetable(false) {
        
        // 设置默认类型为防御塔
        SetType(GameObjectType::AITurretClient);
    }
    
    // 更新防御塔数据
    void UpdateData() override {
        GameObject::UpdateData();
        
        if (address == 0) {
            return;
        }
        
        MemoryManager* memManager = MemoryManager::GetInstance();
        if (!memManager) {
            return;
        }
        
        // 读取防御塔特有属性
        if (!memManager->Read(address + Offsets::Turret::AttackDamage, attackDamage)) {
            attackDamage = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Turret::AttackSpeed, attackSpeed)) {
            attackSpeed = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Turret::Armor, armor)) {
            armor = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Turret::MagicResist, magicResist)) {
            magicResist = 0.0f;
        }
        
        // 读取其他属性
        if (!memManager->Read(address + Offsets::Turret::HealthRegen, healthRegen)) {
            healthRegen = 0.0f;
        }
        if (!memManager->Read(address + Offsets::Turret::VisionRadius, visionRadius)) {
            visionRadius = 0.0f;
        }
        
        // 读取状态标志
        if (!memManager->Read(address + Offsets::Turret::IsInvulnerable, isInvulnerable)) {
            isInvulnerable = false;
        }
        if (!memManager->Read(address + Offsets::Turret::IsTargetable, isTargetable)) {
            isTargetable = false;
        }
        
        // 根据名称确定防御塔类型
        turretType = GetTurretTypeFromName(name);
        ownerTeam = GetTeamFromName(name);
        
        // 根据类型设置攻击范围
        SetAttackRangeByType();
    }
    
    // 获取防御塔类型
    TurretType GetTurretType() const {
        return turretType;
    }
    
    // 获取防御塔类型名称
    std::string GetTurretTypeName() const {
        return GetTurretTypeName(turretType);
    }
    
    // 获取所属队伍
    GameObjectTeam GetOwnerTeam() const {
        return ownerTeam;
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
    
    // 获取视野范围
    float GetVisionRadius() const {
        return visionRadius;
    }
    
    // 是否无敌
    bool IsInvulnerable() const {
        return isInvulnerable;
    }
    
    // 是否可选中
    bool IsTargetable() const {
        return isTargetable;
    }
    
    // 是否是敌方防御塔
    bool IsEnemy(const GameObject& other) const {
        return ownerTeam != other.GetTeam() && ownerTeam != GameObjectTeam::Neutral;
    }
    
    // 是否是友方防御塔
    bool IsAlly(const GameObject& other) const {
        return ownerTeam == other.GetTeam() && ownerTeam != GameObjectTeam::Neutral;
    }
    
    // 是否可以攻击
    bool CanAttack() const {
        return isAlive && !isInvulnerable && isTargetable;
    }
    
    // 是否可以被攻击
    bool CanBeAttacked() const {
        return isAlive && !isInvulnerable && isTargetable;
    }
    
    // 是否在攻击范围内
    bool IsTargetInRange(const GameObject& target) const {
        if (!CanAttack()) {
            return false;
        }
        
        return IsInRange(target, GetAttackRange());
    }
    
    // 获取优先级（用于目标选择）
    int GetPriority() const {
        switch (turretType) {
            case TurretType::Nexus: return 4;
            case TurretType::Inhibitor: return 3;
            case TurretType::Inner: return 2;
            case TurretType::Outer: return 1;
            case TurretType::Fountain: return 0; // 泉水塔通常不作为目标
            default: return 0;
        }
    }
    
    // 是否应该被攻击
    bool ShouldAttack(const Hero& attacker) const {
        // 不能攻击的防御塔
        if (!CanBeAttacked()) {
            return false;
        }
        
        // 敌方防御塔应该被攻击
        if (IsEnemy(attacker)) {
            return true;
        }
        
        // 友方防御塔不应该被攻击
        return false;
    }
    
    // 获取字符串表示
    std::string ToString() const {
        std::string result = "Turret[";
        result += GameObject::ToString();
        result += ", Type: " + GetTurretTypeName();
        result += ", Team: " + std::to_string(static_cast<int>(ownerTeam));
        result += ", AD: " + std::to_string(attackDamage);
        result += ", Invulnerable: " + (isInvulnerable ? "true" : "false");
        result += "]";
        return result;
    }
    
private:
    // 根据名称获取防御塔类型
    static TurretType GetTurretTypeFromName(const std::string& name) {
        // 外塔
        if (name.find("Turret_OrderTurretShrine_A") != std::string::npos ||
            name.find("Turret_OrderTurretShrine_B") != std::string::npos ||
            name.find("Turret_OrderTurretShrine_C") != std::string::npos ||
            name.find("Turret_ChaosTurretShrine_A") != std::string::npos ||
            name.find("Turret_ChaosTurretShrine_B") != std::string::npos ||
            name.find("Turret_ChaosTurretShrine_C") != std::string::npos) {
            return TurretType::Outer;
        }
        
        // 内塔
        if (name.find("Turret_OrderTurret2_A") != std::string::npos ||
            name.find("Turret_OrderTurret2_B") != std::string::npos ||
            name.find("Turret_OrderTurret2_C") != std::string::npos ||
            name.find("Turret_ChaosTurret2_A") != std::string::npos ||
            name.find("Turret_ChaosTurret2_B") != std::string::npos ||
            name.find("Turret_ChaosTurret2_C") != std::string::npos) {
            return TurretType::Inner;
        }
        
        // 水晶塔
        if (name.find("Turret_OrderTurret3_A") != std::string::npos ||
            name.find("Turret_OrderTurret3_B") != std::string::npos ||
            name.find("Turret_OrderTurret3_C") != std::string::npos ||
            name.find("Turret_ChaosTurret3_A") != std::string::npos ||
            name.find("Turret_ChaosTurret3_B") != std::string::npos ||
            name.find("Turret_ChaosTurret3_C") != std::string::npos) {
            return TurretType::Inhibitor;
        }
        
        // 基地塔
        if (name.find("Turret_OrderTurret4") != std::string::npos ||
            name.find("Turret_ChaosTurret4") != std::string::npos) {
            return TurretType::Nexus;
        }
        
        // 泉水塔
        if (name.find("Turret_OrderTurretFountain") != std::string::npos ||
            name.find("Turret_ChaosTurretFountain") != std::string::npos) {
            return TurretType::Fountain;
        }
        
        return TurretType::Unknown;
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
    
    // 获取防御塔类型名称
    static std::string GetTurretTypeName(TurretType type) {
        switch (type) {
            case TurretType::Outer: return "Outer";
            case TurretType::Inner: return "Inner";
            case TurretType::Inhibitor: return "Inhibitor";
            case TurretType::Nexus: return "Nexus";
            case TurretType::Fountain: return "Fountain";
            default: return "Unknown";
        }
    }
    
    // 根据类型设置攻击范围
    void SetAttackRangeByType() {
        switch (turretType) {
            case TurretType::Outer:
                SetAttackRange(750.0f); // 外塔攻击范围
                break;
            case TurretType::Inner:
                SetAttackRange(750.0f); // 内塔攻击范围
                break;
            case TurretType::Inhibitor:
                SetAttackRange(750.0f); // 水晶塔攻击范围
                break;
            case TurretType::Nexus:
                SetAttackRange(750.0f); // 基地塔攻击范围
                break;
            case TurretType::Fountain:
                SetAttackRange(850.0f); // 泉水塔攻击范围
                break;
            default:
                SetAttackRange(750.0f); // 默认攻击范围
                break;
        }
    }
};