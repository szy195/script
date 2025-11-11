#include "Minion.h"
#include "../Core/MemoryManager.h"
#include "../Core/Offsets.h"
#include "../Utils/Math.h"
#include <iostream>

// 构造函数
Minion::Minion(uintptr_t address) : GameObject(address, GameObjectType::Minion) {
    minionType = MinionType::Unknown;
    isMelee = false;
    isCaster = false;
    isSiege = false;
    isSuper = false;
    isWard = false;
    isPet = false;
    isClone = false;
    isJungleMonster = false;
    isEpicMonster = false;
    isDragon = false;
    isBaron = false;
    isRiftHerald = false;
    isRedBuff = false;
    isBlueBuff = false;
    isScuttleCrab = false;
    isKrug = false;
    isRaptor = false;
    isWolf = false;
    isGromp = false;
    
    // 更新小兵特有数据
    UpdateMinionData();
}

// 析构函数
Minion::~Minion() {
}

// 更新数据
void Minion::UpdateData() {
    // 更新基类数据
    GameObject::UpdateData();
    
    // 更新小兵特有数据
    UpdateMinionData();
}

// 更新小兵特有数据
void Minion::UpdateMinionData() {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return;
    }
    
    try {
        // 读取小兵类型
        uint32_t typeValue;
        if (!memManager->Read(address + Offsets::Minion::Type, typeValue)) {
            typeValue = 0; // 默认为近战小兵
        }
        
        // 根据类型值设置小兵类型
        switch (typeValue) {
            case 0:
                minionType = MinionType::Melee;
                isMelee = true;
                break;
            case 1:
                minionType = MinionType::Caster;
                isCaster = true;
                break;
            case 2:
                minionType = MinionType::Siege;
                isSiege = true;
                break;
            case 3:
                minionType = MinionType::Super;
                isSuper = true;
                break;
            case 4:
                minionType = MinionType::Ward;
                isWard = true;
                break;
            case 5:
                minionType = MinionType::Pet;
                isPet = true;
                break;
            case 6:
                minionType = MinionType::Clone;
                isClone = true;
                break;
            case 7:
                minionType = MinionType::JungleMonster;
                isJungleMonster = true;
                break;
            default:
                minionType = MinionType::Unknown;
                break;
        }
        
        // 根据名称判断野怪类型
        std::string name = GetName();
        if (!name.empty()) {
            if (name.find("Dragon") != std::string::npos) {
                isDragon = true;
                isEpicMonster = true;
            } else if (name.find("Baron") != std::string::npos) {
                isBaron = true;
                isEpicMonster = true;
            } else if (name.find("Herald") != std::string::npos) {
                isRiftHerald = true;
                isEpicMonster = true;
            } else if (name.find("Red") != std::string::npos || name.find("Razorbeak") != std::string::npos) {
                isRedBuff = true;
            } else if (name.find("Blue") != std::string::npos || name.find("Sentinel") != std::string::npos) {
                isBlueBuff = true;
            } else if (name.find("Scuttle") != std::string::npos) {
                isScuttleCrab = true;
            } else if (name.find("Krug") != std::string::npos) {
                isKrug = true;
            } else if (name.find("Raptor") != std::string::npos) {
                isRaptor = true;
            } else if (name.find("Wolf") != std::string::npos) {
                isWolf = true;
            } else if (name.find("Gromp") != std::string::npos) {
                isGromp = true;
            }
        }
        
        // 读取小兵特有属性
        float attackDamageValue;
        if (!memManager->Read(address + Offsets::Minion::AttackDamage, attackDamageValue)) {
            attackDamageValue = 0.0f;
        }
        attackDamage = attackDamageValue;
        
        float attackSpeedValue;
        if (!memManager->Read(address + Offsets::Minion::AttackSpeed, attackSpeedValue)) {
            attackSpeedValue = 1.0f;
        }
        attackSpeed = attackSpeedValue;
        
        float attackRangeValue;
        if (!memManager->Read(address + Offsets::Minion::AttackRange, attackRangeValue)) {
            attackRangeValue = 0.0f;
        }
        attackRange = attackRangeValue;
        
        uint32_t goldBountyValue;
        if (!memManager->Read(address + Offsets::Minion::GoldBounty, goldBountyValue)) {
            goldBountyValue = 0;
        }
        goldBounty = goldBountyValue;
        
        uint32_t experienceBountyValue;
        if (!memManager->Read(address + Offsets::Minion::ExperienceBounty, experienceBountyValue)) {
            experienceBountyValue = 0;
        }
        experienceBounty = experienceBountyValue;
        
        float respawnTimeValue;
        if (!memManager->Read(address + Offsets::Minion::RespawnTime, respawnTimeValue)) {
            respawnTimeValue = 0.0f;
        }
        respawnTime = respawnTimeValue;
        
        // 读取路径信息
        uint32_t pathIndexValue;
        if (!memManager->Read(address + Offsets::Minion::PathIndex, pathIndexValue)) {
            pathIndexValue = 0;
        }
        pathIndex = pathIndexValue;
        
        uint32_t pathLengthValue;
        if (!memManager->Read(address + Offsets::Minion::PathLength, pathLengthValue)) {
            pathLengthValue = 0;
        }
        pathLength = pathLengthValue;
        
        // 读取目标信息
        if (!memManager->Read(address + Offsets::Minion::TargetAddress, targetAddress)) {
            targetAddress = 0;
        }
        
    } catch (...) {
        // 忽略异常
    }
}

// 获取小兵类型
MinionType Minion::GetMinionType() const {
    return minionType;
}

// 是否是近战小兵
bool Minion::IsMeleeMinion() const {
    return isMelee;
}

// 是否是远程小兵
bool Minion::IsCasterMinion() const {
    return isCaster;
}

// 是否是炮车
bool Minion::IsSiegeMinion() const {
    return isSiege;
}

// 是否是超级兵
bool Minion::IsSuperMinion() const {
    return isSuper;
}

// 是否是守卫
bool Minion::IsWard() const {
    return isWard;
}

// 是否是宠物
bool Minion::IsPet() const {
    return isPet;
}

// 是否是克隆
bool Minion::IsClone() const {
    return isClone;
}

// 是否是野怪
bool Minion::IsJungleMonster() const {
    return isJungleMonster;
}

// 是否是史诗级野怪
bool Minion::IsEpicMonster() const {
    return isEpicMonster;
}

// 是否是巨龙
bool Minion::IsDragon() const {
    return isDragon;
}

// 是否是纳什男爵
bool Minion::IsBaron() const {
    return isBaron;
}

// 是否是峡谷先锋
bool Minion::IsRiftHerald() const {
    return isRiftHerald;
}

// 是否是红Buff
bool Minion::IsRedBuff() const {
    return isRedBuff;
}

// 是否是蓝Buff
bool Minion::IsBlueBuff() const {
    return isBlueBuff;
}

// 是否是河道蟹
bool Minion::IsScuttleCrab() const {
    return isScuttleCrab;
}

// 是否是石头人
bool Minion::IsKrug() const {
    return isKrug;
}

// 是否是锋喙鸟
bool Minion::IsRaptor() const {
    return isRaptor;
}

// 是否是魔狼
bool Minion::IsWolf() const {
    return isWolf;
}

// 是否是蛤蟆
bool Minion::IsGromp() const {
    return isGromp;
}

// 是否是线上小兵
bool Minion::IsLaneMinion() const {
    return isMelee || isCaster || isSiege || isSuper;
}

// 是否是普通野怪
bool Minion::IsNormalJungleMonster() const {
    return isJungleMonster && !isEpicMonster;
}

// 获取攻击力
float Minion::GetAttackDamage() const {
    return attackDamage;
}

// 获取攻击速度
float Minion::GetAttackSpeed() const {
    return attackSpeed;
}

// 获取攻击范围
float Minion::GetAttackRange() const {
    return attackRange;
}

// 获取金币奖励
uint32_t Minion::GetGoldBounty() const {
    return goldBounty;
}

// 获取经验奖励
uint32_t Minion::GetExperienceBounty() const {
    return experienceBounty;
}

// 获取重生时间
float Minion::GetRespawnTime() const {
    return respawnTime;
}

// 获取路径索引
uint32_t Minion::GetPathIndex() const {
    return pathIndex;
}

// 获取路径长度
uint32_t Minion::GetPathLength() const {
    return pathLength;
}

// 获取目标地址
uintptr_t Minion::GetTargetAddress() const {
    return targetAddress;
}

// 获取目标对象
std::shared_ptr<GameObject> Minion::GetTarget() const {
    if (targetAddress == 0) {
        return nullptr;
    }
    
    // 这里需要从游戏对象管理器中查找目标对象
    // 在实际实现中，需要调用GameObjectManager的FindObjectByAddress方法
    return nullptr;
}

// 是否在攻击范围内
bool Minion::IsTargetInRange(const std::shared_ptr<GameObject>& target) const {
    if (!target) {
        return false;
    }
    
    return DistanceTo(target) <= attackRange;
}

// 是否可以攻击
bool Minion::CanAttack() const {
    return !IsDead() && !IsStunned() && !IsRooted();
}

// 是否应该攻击目标
bool Minion::ShouldAttack(const std::shared_ptr<GameObject>& target) const {
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
float Minion::GetNextAttackTime() const {
    // 假设基础攻击时间为1.0秒
    float baseAttackTime = 1.0f;
    
    // 根据攻击速度计算攻击时间
    float attackTime = baseAttackTime / attackSpeed;
    
    // 获取当前游戏时间
    // 这里需要从游戏API获取当前时间
    float currentTime = 0.0f; // 假设当前时间为0
    
    // 计算下次攻击时间
    return currentTime + attackTime;
}

// 是否正在攻击
bool Minion::IsAttacking() const {
    return targetAddress != 0;
}

// 是否正在移动
bool Minion::IsMoving() const {
    return !IsAttacking() && !IsStunned() && !IsRooted();
}

// 预测小兵位置
Vector3 Minion::PredictPosition(float time) const {
    if (time <= 0.0f) {
        return GetPosition();
    }
    
    // 如果小兵正在移动，预测其位置
    if (IsMoving()) {
        Vector3 currentPos = GetPosition();
        Vector3 velocity = GetVelocity();
        
        return currentPos + velocity * time;
    }
    
    // 如果小兵正在攻击，预测其位置不变
    if (IsAttacking()) {
        return GetPosition();
    }
    
    // 否则，预测其位置不变
    return GetPosition();
}

// 获取小兵价值
float Minion::GetValue() const {
    float value = 0.0f;
    
    // 金币价值
    value += goldBounty * 1.0f;
    
    // 经验价值
    value += experienceBounty * 0.5f;
    
    // 史诗级野怪有额外价值
    if (isEpicMonster) {
        value += 500.0f;
    }
    
    // 龙和男爵有更高价值
    if (isDragon) {
        value += 300.0f;
    }
    
    if (isBaron) {
        value += 500.0f;
    }
    
    if (isRiftHerald) {
        value += 400.0f;
    }
    
    return value;
}

// 是否应该优先攻击
bool Minion::ShouldPrioritize() const {
    // 优先级：史诗级野怪 > 龙/男爵/峡谷先锋 > 炮车 > 远程小兵 > 近战小兵 > 其他野怪
    if (isEpicMonster) {
        return true;
    }
    
    if (isDragon || isBaron || isRiftHerald) {
        return true;
    }
    
    if (isSiege) {
        return true;
    }
    
    if (isCaster) {
        return true;
    }
    
    if (isMelee) {
        return true;
    }
    
    return false;
}

// 获取小兵威胁等级
float Minion::GetThreatLevel() const {
    float threat = 0.0f;
    
    // 基于攻击力的威胁
    threat += attackDamage * 0.5f;
    
    // 基于攻击速度的威胁
    threat += attackSpeed * 10.0f;
    
    // 基于攻击范围的威胁
    threat += attackRange * 0.1f;
    
    // 史诗级野怪威胁更高
    if (isEpicMonster) {
        threat += 100.0f;
    }
    
    // 龙和男爵威胁更高
    if (isDragon) {
        threat += 50.0f;
    }
    
    if (isBaron) {
        threat += 100.0f;
    }
    
    if (isRiftHerald) {
        threat += 75.0f;
    }
    
    return threat;
}