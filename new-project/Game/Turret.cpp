#include "Turret.h"
#include "../Core/MemoryManager.h"
#include "../Core/Offsets.h"
#include "../Utils/Math.h"
#include <iostream>

// 构造函数
Turret::Turret(uintptr_t address) : GameObject(address, GameObjectType::Turret) {
    turretType = TurretType::Unknown;
    isOuter = false;
    isInner = false;
    isInhibitor = false;
    isNexus = false;
    isBase = false;
    isDead = false;
    
    // 更新防御塔特有数据
    UpdateTurretData();
}

// 析构函数
Turret::~Turret() {
}

// 更新数据
void Turret::UpdateData() {
    // 更新基类数据
    GameObject::UpdateData();
    
    // 更新防御塔特有数据
    UpdateTurretData();
}

// 更新防御塔特有数据
void Turret::UpdateTurretData() {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return;
    }
    
    try {
        // 读取防御塔类型
        uint32_t typeValue;
        if (!memManager->Read(address + Offsets::Turret::Type, typeValue)) {
            typeValue = 0; // 默认为外塔
        }
        
        // 根据类型值设置防御塔类型
        switch (typeValue) {
            case 0:
                turretType = TurretType::Outer;
                isOuter = true;
                break;
            case 1:
                turretType = TurretType::Inner;
                isInner = true;
                break;
            case 2:
                turretType = TurretType::Inhibitor;
                isInhibitor = true;
                break;
            case 3:
                turretType = TurretType::Nexus;
                isNexus = true;
                break;
            case 4:
                turretType = TurretType::Base;
                isBase = true;
                break;
            default:
                turretType = TurretType::Unknown;
                break;
        }
        
        // 根据名称判断防御塔类型
        std::string name = GetName();
        if (!name.empty()) {
            if (name.find("Outer") != std::string::npos || name.find("Turret_ShieldA") != std::string::npos || name.find("Turret_ShieldB") != std::string::npos) {
                isOuter = true;
                turretType = TurretType::Outer;
            } else if (name.find("Inner") != std::string::npos || name.find("Turret_OrderA") != std::string::npos || name.find("Turret_OrderB") != std::string::npos) {
                isInner = true;
                turretType = TurretType::Inner;
            } else if (name.find("Inhibitor") != std::string::npos || name.find("Turret_ChaosA") != std::string::npos || name.find("Turret_ChaosB") != std::string::npos) {
                isInhibitor = true;
                turretType = TurretType::Inhibitor;
            } else if (name.find("Nexus") != std::string::npos) {
                isNexus = true;
                turretType = TurretType::Nexus;
            } else if (name.find("Base") != std::string::npos) {
                isBase = true;
                turretType = TurretType::Base;
            }
        }
        
        // 读取防御塔特有属性
        float attackDamageValue;
        if (!memManager->Read(address + Offsets::Turret::AttackDamage, attackDamageValue)) {
            attackDamageValue = 0.0f;
        }
        attackDamage = attackDamageValue;
        
        float attackSpeedValue;
        if (!memManager->Read(address + Offsets::Turret::AttackSpeed, attackSpeedValue)) {
            attackSpeedValue = 1.0f;
        }
        attackSpeed = attackSpeedValue;
        
        float attackRangeValue;
        if (!memManager->Read(address + Offsets::Turret::AttackRange, attackRangeValue)) {
            attackRangeValue = 0.0f;
        }
        attackRange = attackRangeValue;
        
        uint32_t goldBountyValue;
        if (!memManager->Read(address + Offsets::Turret::GoldBounty, goldBountyValue)) {
            goldBountyValue = 0;
        }
        goldBounty = goldBountyValue;
        
        // 读取目标信息
        if (!memManager->Read(address + Offsets::Turret::TargetAddress, targetAddress)) {
            targetAddress = 0;
        }
        
        // 读取状态信息
        bool isDeadValue;
        if (!memManager->Read(address + Offsets::Turret::IsDead, isDeadValue)) {
            isDeadValue = false;
        }
        isDead = isDeadValue;
        
    } catch (...) {
        // 忽略异常
    }
}

// 获取防御塔类型
TurretType Turret::GetTurretType() const {
    return turretType;
}

// 是否是外塔
bool Turret::IsOuterTurret() const {
    return isOuter;
}

// 是否是内塔
bool Turret::IsInnerTurret() const {
    return isInner;
}

// 是否是水晶塔
bool Turret::IsInhibitorTurret() const {
    return isInhibitor;
}

// 是否是基地塔
bool Turret::IsNexusTurret() const {
    return isNexus;
}

// 是否是基地防御塔
bool Turret::IsBaseTurret() const {
    return isBase;
}

// 是否已摧毁
bool Turret::IsDestroyed() const {
    return isDead || IsDead();
}

// 获取攻击力
float Turret::GetAttackDamage() const {
    return attackDamage;
}

// 获取攻击速度
float Turret::GetAttackSpeed() const {
    return attackSpeed;
}

// 获取攻击范围
float Turret::GetAttackRange() const {
    return attackRange;
}

// 获取金币奖励
uint32_t Turret::GetGoldBounty() const {
    return goldBounty;
}

// 获取目标地址
uintptr_t Turret::GetTargetAddress() const {
    return targetAddress;
}

// 获取目标对象
std::shared_ptr<GameObject> Turret::GetTarget() const {
    if (targetAddress == 0) {
        return nullptr;
    }
    
    // 这里需要从游戏对象管理器中查找目标对象
    // 在实际实现中，需要调用GameObjectManager的FindObjectByAddress方法
    return nullptr;
}

// 是否在攻击范围内
bool Turret::IsTargetInRange(const std::shared_ptr<GameObject>& target) const {
    if (!target) {
        return false;
    }
    
    return DistanceTo(target) <= attackRange;
}

// 是否可以攻击
bool Turret::CanAttack() const {
    return !IsDead() && !IsDestroyed();
}

// 是否应该攻击目标
bool Turret::ShouldAttack(const std::shared_ptr<GameObject>& target) const {
    if (!target || !CanAttack()) {
        return false;
    }
    
    // 检查是否在攻击范围内
    if (!IsTargetInRange(target)) {
        return false;
    }
    
    // 检查目标是否有效
    if (target->IsDead() || target->IsInvulnerable()) {
        return false;
    }
    
    // 检查目标是否是敌人
    if (IsAlly(target)) {
        return false;
    }
    
    return true;
}

// 获取下一个攻击时间
float Turret::GetNextAttackTime() const {
    // 假设基础攻击时间为1.5秒
    float baseAttackTime = 1.5f;
    
    // 根据攻击速度计算攻击时间
    float attackTime = baseAttackTime / attackSpeed;
    
    // 获取当前游戏时间
    // 这里需要从游戏API获取当前时间
    float currentTime = 0.0f; // 假设当前时间为0
    
    // 计算下次攻击时间
    return currentTime + attackTime;
}

// 是否正在攻击
bool Turret::IsAttacking() const {
    return targetAddress != 0;
}

// 获取防御塔价值
float Turret::GetValue() const {
    float value = 0.0f;
    
    // 基础金币价值
    value += goldBounty * 1.0f;
    
    // 根据防御塔类型增加价值
    switch (turretType) {
        case TurretType::Outer:
            value += 50.0f;
            break;
        case TurretType::Inner:
            value += 100.0f;
            break;
        case TurretType::Inhibitor:
            value += 150.0f;
            break;
        case TurretType::Nexus:
            value += 200.0f;
            break;
        case TurretType::Base:
            value += 75.0f;
            break;
        default:
            break;
    }
    
    return value;
}

// 获取防御塔威胁等级
float Turret::GetThreatLevel() const {
    float threat = 0.0f;
    
    // 基于攻击力的威胁
    threat += attackDamage * 0.5f;
    
    // 基于攻击速度的威胁
    threat += attackSpeed * 10.0f;
    
    // 基于攻击范围的威胁
    threat += attackRange * 0.1f;
    
    // 根据防御塔类型增加威胁
    switch (turretType) {
        case TurretType::Outer:
            threat += 25.0f;
            break;
        case TurretType::Inner:
            threat += 50.0f;
            break;
        case TurretType::Inhibitor:
            threat += 75.0f;
            break;
        case TurretType::Nexus:
            threat += 100.0f;
            break;
        case TurretType::Base:
            threat += 40.0f;
            break;
        default:
            break;
    }
    
    return threat;
}

// 是否应该优先攻击
bool Turret::ShouldPrioritize() const {
    // 优先级：基地塔 > 水晶塔 > 内塔 > 外塔 > 其他
    if (isNexus) {
        return true;
    }
    
    if (isInhibitor) {
        return true;
    }
    
    if (isInner) {
        return true;
    }
    
    if (isOuter) {
        return true;
    }
    
    return false;
}

// 预测防御塔位置
Vector3 Turret::PredictPosition(float time) const {
    // 防御塔是固定不动的，所以预测位置就是当前位置
    return GetPosition();
}

// 获取防御塔等级
uint32_t Turret::GetLevel() const {
    // 防御塔通常没有等级，但可以根据类型返回一个值
    switch (turretType) {
        case TurretType::Outer:
            return 1;
        case TurretType::Inner:
            return 2;
        case TurretType::Inhibitor:
            return 3;
        case TurretType::Nexus:
            return 4;
        case TurretType::Base:
            return 2;
        default:
            return 1;
    }
}

// 获取防御塔护甲
float Turret::GetArmor() const {
    // 防御塔通常有较高的护甲
    float baseArmor = 50.0f;
    
    // 根据防御塔类型增加护甲
    switch (turretType) {
        case TurretType::Outer:
            return baseArmor;
        case TurretType::Inner:
            return baseArmor + 20.0f;
        case TurretType::Inhibitor:
            return baseArmor + 40.0f;
        case TurretType::Nexus:
            return baseArmor + 60.0f;
        case TurretType::Base:
            return baseArmor + 30.0f;
        default:
            return baseArmor;
    }
}

// 获取防御塔魔法抗性
float Turret::GetMagicResist() const {
    // 防御塔通常有较高的魔法抗性
    float baseMagicResist = 50.0f;
    
    // 根据防御塔类型增加魔法抗性
    switch (turretType) {
        case TurretType::Outer:
            return baseMagicResist;
        case TurretType::Inner:
            return baseMagicResist + 20.0f;
        case TurretType::Inhibitor:
            return baseMagicResist + 40.0f;
        case TurretType::Nexus:
            return baseMagicResist + 60.0f;
        case TurretType::Base:
            return baseMagicResist + 30.0f;
        default:
            return baseMagicResist;
    }
}

// 计算对目标的伤害
float Turret::CalculateDamage(const std::shared_ptr<GameObject>& target) const {
    if (!target) {
        return 0.0f;
    }
    
    float damage = attackDamage;
    
    // 计算护甲减免
    float effectiveArmor = Math::Max(0.0f, target->GetArmor());
    damage *= 100.0f / (100.0f + effectiveArmor);
    
    // 防御塔对小兵有额外伤害
    if (target->GetType() == GameObjectType::Minion) {
        damage *= 1.5f;
    }
    
    // 防御塔对英雄有额外伤害（根据英雄等级）
    if (target->GetType() == GameObjectType::Hero) {
        // 这里需要获取英雄等级，假设有方法GetLevel
        // damage *= (1.0f + 0.04f * heroLevel);
        damage *= 1.2f; // 简化处理
    }
    
    return damage;
}