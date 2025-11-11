#include "GameObject.h"
#include "../Core/MemoryManager.h"
#include "../Core/Offsets.h"
#include "../Utils/Math.h"
#include <iostream>

// 构造函数
GameObject::GameObject(uintptr_t address, GameObjectType type) 
    : address(address), type(type), team(GameObjectTeam::Unknown), isValid(true) {
    UpdateData();
}

// 析构函数
GameObject::~GameObject() {
}

// 获取内存地址
uintptr_t GameObject::GetAddress() const {
    return address;
}

// 获取类型
GameObjectType GameObject::GetType() const {
    return type;
}

// 获取团队
GameObjectTeam GameObject::GetTeam() const {
    return team;
}

// 获取名称
std::string GameObject::GetName() const {
    return name;
}

// 获取位置
Vector3 GameObject::GetPosition() const {
    return position;
}

// 获取2D位置
Vector2 GameObject::GetPosition2D() const {
    return Vector2(position.x, position.z);
}

// 是否有效
bool GameObject::IsValid() const {
    return isValid && address != 0;
}

// 设置有效状态
void GameObject::SetValid(bool valid) {
    isValid = valid;
}

// 更新数据
void GameObject::UpdateData() {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        isValid = false;
        return;
    }
    
    try {
        // 读取团队
        uint32_t teamValue;
        if (!memManager->Read(address + Offsets::GameObject::Team, teamValue)) {
            teamValue = 0;
        }
        team = static_cast<GameObjectTeam>(teamValue);
        
        // 读取名称
        uintptr_t namePtr;
        if (!memManager->Read(address + Offsets::GameObject::Name, namePtr)) {
            namePtr = 0;
        }
        if (namePtr != 0) {
            if (!memManager->ReadString(namePtr, name)) {
                name = "";
            }
        }
        
        // 读取位置
        float posX, posY, posZ;
        if (!memManager->Read(address + Offsets::GameObject::PosX, posX)) {
            posX = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::PosY, posY)) {
            posY = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::PosZ, posZ)) {
            posZ = 0.0f;
        }
        position.x = posX;
        position.y = posY;
        position.z = posZ;
        
        // 读取其他属性
        float healthVal, maxHealthVal, manaVal, maxManaVal, armorVal, magicResistVal, attackDamageVal, attackSpeedVal, movementSpeedVal;
        bool isVisibleVal, isTargetableVal, isInvulnerableVal, isDeadVal;
        
        if (!memManager->Read(address + Offsets::GameObject::Health, healthVal)) {
            healthVal = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::MaxHealth, maxHealthVal)) {
            maxHealthVal = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::Mana, manaVal)) {
            manaVal = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::MaxMana, maxManaVal)) {
            maxManaVal = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::Armor, armorVal)) {
            armorVal = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::MagicResist, magicResistVal)) {
            magicResistVal = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::AttackDamage, attackDamageVal)) {
            attackDamageVal = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::AttackSpeed, attackSpeedVal)) {
            attackSpeedVal = 0.0f;
        }
        if (!memManager->Read(address + Offsets::GameObject::MovementSpeed, movementSpeedVal)) {
            movementSpeedVal = 0.0f;
        }
        
        if (!memManager->Read(address + Offsets::GameObject::IsVisible, isVisibleVal)) {
            isVisibleVal = false;
        }
        if (!memManager->Read(address + Offsets::GameObject::IsTargetable, isTargetableVal)) {
            isTargetableVal = false;
        }
        if (!memManager->Read(address + Offsets::GameObject::IsInvulnerable, isInvulnerableVal)) {
            isInvulnerableVal = false;
        }
        if (!memManager->Read(address + Offsets::GameObject::IsDead, isDeadVal)) {
            isDeadVal = false;
        }
        
        health = healthVal;
        maxHealth = maxHealthVal;
        mana = manaVal;
        maxMana = maxManaVal;
        armor = armorVal;
        magicResist = magicResistVal;
        attackDamage = attackDamageVal;
        attackSpeed = attackSpeedVal;
        movementSpeed = movementSpeedVal;
        isVisible = isVisibleVal;
        isTargetable = isTargetableVal;
        isInvulnerable = isInvulnerableVal;
        isDead = isDeadVal;
        
    } catch (...) {
        isValid = false;
    }
}

// 获取生命值
float GameObject::GetHealth() const {
    return health;
}

// 获取最大生命值
float GameObject::GetMaxHealth() const {
    return maxHealth;
}

// 获取生命值百分比
float GameObject::GetHealthPercent() const {
    return maxHealth > 0 ? (health / maxHealth) * 100.0f : 0.0f;
}

// 获取法力值
float GameObject::GetMana() const {
    return mana;
}

// 获取最大法力值
float GameObject::GetMaxMana() const {
    return maxMana;
}

// 获取法力值百分比
float GameObject::GetManaPercent() const {
    return maxMana > 0 ? (mana / maxMana) * 100.0f : 0.0f;
}

// 获取护甲
float GameObject::GetArmor() const {
    return armor;
}

// 获取魔法抗性
float GameObject::GetMagicResist() const {
    return magicResist;
}

// 获取攻击力
float GameObject::GetAttackDamage() const {
    return attackDamage;
}

// 获取攻击速度
float GameObject::GetAttackSpeed() const {
    return attackSpeed;
}

// 获取移动速度
float GameObject::GetMovementSpeed() const {
    return movementSpeed;
}

// 是否可见
bool GameObject::IsVisible() const {
    return isVisible;
}

// 是否可目标
bool GameObject::IsTargetable() const {
    return isTargetable;
}

// 是否无敌
bool GameObject::IsInvulnerable() const {
    return isInvulnerable;
}

// 是否死亡
bool GameObject::IsDead() const {
    return isDead;
}

// 是否存活
bool GameObject::IsAlive() const {
    return !isDead && health > 0;
}

// 是否是敌人
bool GameObject::IsEnemy(GameObjectTeam otherTeam) const {
    return team != GameObjectTeam::Unknown && otherTeam != GameObjectTeam::Unknown && team != otherTeam;
}

// 是否是友方
bool GameObject::IsAlly(GameObjectTeam otherTeam) const {
    return team != GameObjectTeam::Unknown && otherTeam != GameObjectTeam::Unknown && team == otherTeam;
}

// 计算到另一个对象的距离
float GameObject::DistanceTo(const std::shared_ptr<GameObject>& other) const {
    if (!other) {
        return 0.0f;
    }
    return position.DistanceTo(other->GetPosition());
}

// 计算到某个位置的距离
float GameObject::DistanceTo(const Vector3& pos) const {
    return position.DistanceTo(pos);
}

// 计算到某个位置的距离（2D）
float GameObject::DistanceTo(const Vector2& pos) const {
    return GetPosition2D().DistanceTo(pos);
}

// 计算到另一个对象的距离（2D）
float GameObject::DistanceTo2D(const std::shared_ptr<GameObject>& other) const {
    if (!other) {
        return 0.0f;
    }
    return GetPosition2D().DistanceTo(other->GetPosition2D());
}

// 预测位置（基于移动速度和时间）
Vector3 GameObject::PredictPosition(float time) const {
    if (time <= 0.0f || movementSpeed <= 0.0f) {
        return position;
    }
    
    // 简单的线性预测
    // 在实际项目中，可能需要更复杂的预测算法
    return position; // 简化处理，返回当前位置
}

// 预测位置（基于移动方向和时间）
Vector3 GameObject::PredictPosition(const Vector3& direction, float time) const {
    if (time <= 0.0f || movementSpeed <= 0.0f) {
        return position;
    }
    
    Vector3 normalizedDir = direction.Normalized();
    float distance = movementSpeed * time;
    
    return position + normalizedDir * distance;
}

// 获取朝向
Vector3 GameObject::GetDirection() const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return Vector3::Forward;
    }
    
    // 读取朝向角度
    float angle;
    if (!memManager->Read(address + Offsets::GameObject::FacingAngle, angle)) {
        angle = 0.0f;
    }
    
    // 将角度转换为方向向量
    return Vector3(
        sin(angle),
        0.0f,
        cos(angle)
    ).Normalized();
}

// 获取朝向角度（弧度）
float GameObject::GetFacingAngle() const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return 0.0f;
    }
    
    float angle;
    if (!memManager->Read(address + Offsets::GameObject::FacingAngle, angle)) {
        angle = 0.0f;
    }
    return angle;
}

// 获取朝向角度（度数）
float GameObject::GetFacingAngleDegrees() const {
    return Math::RadiansToDegrees(GetFacingAngle());
}

// 设置朝向角度
void GameObject::SetFacingAngle(float angle) const {
    auto memManager = MemoryManager::GetInstance();
    if (!memManager || address == 0) {
        return;
    }
    
    memManager->Write(address + Offsets::GameObject::FacingAngle, angle);
}

// 设置朝向角度（度数）
void GameObject::SetFacingAngleDegrees(float degrees) const {
    SetFacingAngle(Math::DegreesToRadians(degrees));
}

// 设置朝向
void GameObject::SetFacing(const Vector3& direction) const {
    if (direction.IsZero()) {
        return;
    }
    
    Vector3 normalizedDir = direction.Normalized();
    float angle = atan2(normalizedDir.x, normalizedDir.z);
    
    SetFacingAngle(angle);
}

// 是否在范围内
bool GameObject::IsInRange(const Vector3& pos, float range) const {
    return DistanceTo(pos) <= range;
}

// 是否在范围内（2D）
bool GameObject::IsInRange(const Vector2& pos, float range) const {
    return DistanceTo(pos) <= range;
}

// 是否在另一个对象的范围内
bool GameObject::IsInRange(const std::shared_ptr<GameObject>& other, float range) const {
    return DistanceTo(other) <= range;
}

// 获取边界框
BoundingBox GameObject::GetBoundingBox() const {
    BoundingBox box;
    
    // 默认边界框大小
    float width = 50.0f;
    float height = 100.0f;
    
    // 根据对象类型调整边界框大小
    switch (type) {
        case GameObjectType::Hero:
            width = 60.0f;
            height = 180.0f;
            break;
        case GameObjectType::Minion:
            width = 40.0f;
            height = 80.0f;
            break;
        case GameObjectType::Turret:
            width = 120.0f;
            height = 200.0f;
            break;
        default:
            break;
    }
    
    box.min = Vector3(position.x - width / 2, position.y, position.z - width / 2);
    box.max = Vector3(position.x + width / 2, position.y + height, position.z + width / 2);
    
    return box;
}

// 获取碰撞半径
float GameObject::GetBoundingRadius() const {
    // 根据对象类型返回不同的碰撞半径
    switch (type) {
        case GameObjectType::Hero:
            return 65.0f;
        case GameObjectType::Minion:
            return 35.0f;
        case GameObjectType::Turret:
            return 100.0f;
        default:
            return 50.0f;
    }
}

// 获取选择半径
float GameObject::GetSelectionRadius() const {
    // 根据对象类型返回不同的选择半径
    switch (type) {
        case GameObjectType::Hero:
            return 120.0f;
        case GameObjectType::Minion:
            return 80.0f;
        case GameObjectType::Turret:
            return 150.0f;
        default:
            return 100.0f;
    }
}

// 是否与另一个对象碰撞
bool GameObject::IsCollidingWith(const std::shared_ptr<GameObject>& other) const {
    if (!other) {
        return false;
    }
    
    float distance = DistanceTo(other);
    float combinedRadius = GetBoundingRadius() + other->GetBoundingRadius();
    
    return distance <= combinedRadius;
}

// 是否与某个位置碰撞
bool GameObject::IsCollidingWith(const Vector3& pos, float radius) const {
    float distance = DistanceTo(pos);
    float combinedRadius = GetBoundingRadius() + radius;
    
    return distance <= combinedRadius;
}

// 获取到目标的路径（简化版）
std::vector<Vector3> GameObject::GetPathTo(const Vector3& target) const {
    std::vector<Vector3> path;
    
    // 简化处理，直接返回起点和终点
    // 在实际项目中，需要使用寻路算法
    path.push_back(position);
    path.push_back(target);
    
    return path;
}

// 获取到目标的路径（简化版）
std::vector<Vector3> GameObject::GetPathTo(const std::shared_ptr<GameObject>& target) const {
    if (!target) {
        return std::vector<Vector3>();
    }
    
    return GetPathTo(target->GetPosition());
}