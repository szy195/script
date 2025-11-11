#pragma once

#include "../Core/MemoryManager.h"
#include "../Core/Offsets.h"
#include "../Utils/Vector.h"
#include "../Utils/Math.h"
#include <string>
#include <vector>

// 游戏对象类型枚举
enum class GameObjectType {
    Unknown = 0,
    Player = 1,
    Minion = 2,
    Turret = 3,
    Inhibitor = 4,
    Nexus = 5,
    Jungle = 6,
    Missile = 7,
    Shop = 8,
    AttackableUnit = 9,
    Building = 10,
    AIHeroClient = 11,
    AIMinionClient = 12,
    AITurretClient = 13,
    LevelProp = 14,
    GrassObject = 15,
    Obj_AI_Base = 16
};

// 游戏对象团队枚举
enum class GameObjectTeam {
    Unknown = 0,
    Blue = 100,
    Red = 200,
    Neutral = 300
};

// 游戏对象基类
class GameObject {
protected:
    uintptr_t address;
    std::string name;
    GameObjectType type;
    GameObjectTeam team;
    Vector3 position;
    Vector3 direction;
    bool isVisible;
    bool isAlive;
    float health;
    float maxHealth;
    float mana;
    float maxMana;
    float attackRange;
    float movementSpeed;
    
public:
    // 构造函数
    GameObject(uintptr_t addr = 0) : address(addr), type(GameObjectType::Unknown), 
        team(GameObjectTeam::Unknown), isVisible(false), isAlive(false),
        health(0.0f), maxHealth(0.0f), mana(0.0f), maxMana(0.0f),
        attackRange(0.0f), movementSpeed(0.0f) {}
    
    // 虚析构函数
    virtual ~GameObject() {}
    
    // 获取内存地址
    uintptr_t GetAddress() const {
        return address;
    }
    
    // 设置内存地址
    void SetAddress(uintptr_t addr) {
        address = addr;
        if (addr != 0) {
            UpdateData();
        }
    }
    
    // 更新对象数据
    virtual void UpdateData() {
        if (address == 0) {
            return;
        }
        
        // 读取基本属性
        MemoryManager* memManager = MemoryManager::GetInstance();
        if (!memManager) {
            return;
        }
        
        // 读取位置
        position = memManager->Read<Vector3>(address + Offsets::GameObject::Position);
        
        // 读取可见性和存活状态
        isVisible = memManager->Read<bool>(address + Offsets::GameObject::IsVisible);
        isAlive = memManager->Read<bool>(address + Offsets::GameObject::IsAlive);
        
        // 读取生命值
        health = memManager->Read<float>(address + Offsets::GameObject::Health);
        maxHealth = memManager->Read<float>(address + Offsets::GameObject::MaxHealth);
        
        // 读取法力值
        mana = memManager->Read<float>(address + Offsets::GameObject::Mana);
        maxMana = memManager->Read<float>(address + Offsets::GameObject::MaxMana);
        
        // 读取移动速度
        movementSpeed = memManager->Read<float>(address + Offsets::GameObject::MovementSpeed);
        
        // 读取名称
        char nameBuffer[64];
        uintptr_t namePtr = memManager->Read<uintptr_t>(address + Offsets::GameObject::Name);
        if (namePtr != 0) {
            memManager->ReadMemory(namePtr, nameBuffer, sizeof(nameBuffer));
            name = std::string(nameBuffer);
        }
    }
    
    // 获取对象名称
    const std::string& GetName() const {
        return name;
    }
    
    // 获取对象类型
    GameObjectType GetType() const {
        return type;
    }
    
    // 设置对象类型
    void SetType(GameObjectType objType) {
        type = objType;
    }
    
    // 获取团队
    GameObjectTeam GetTeam() const {
        return team;
    }
    
    // 设置团队
    void SetTeam(GameObjectTeam objTeam) {
        team = objTeam;
    }
    
    // 获取位置
    const Vector3& GetPosition() const {
        return position;
    }
    
    // 获取2D位置
    Vector2 GetPosition2D() const {
        return position.ToVector2();
    }
    
    // 获取方向
    const Vector3& GetDirection() const {
        return direction;
    }
    
    // 是否可见
    bool IsVisible() const {
        return isVisible;
    }
    
    // 是否存活
    bool IsAlive() const {
        return isAlive;
    }
    
    // 获取生命值
    float GetHealth() const {
        return health;
    }
    
    // 获取最大生命值
    float GetMaxHealth() const {
        return maxHealth;
    }
    
    // 获取生命值百分比
    float GetHealthPercent() const {
        if (maxHealth <= 0.0f) {
            return 0.0f;
        }
        return health / maxHealth;
    }
    
    // 获取法力值
    float GetMana() const {
        return mana;
    }
    
    // 获取最大法力值
    float GetMaxMana() const {
        return maxMana;
    }
    
    // 获取法力值百分比
    float GetManaPercent() const {
        if (maxMana <= 0.0f) {
            return 0.0f;
        }
        return mana / maxMana;
    }
    
    // 获取攻击范围
    float GetAttackRange() const {
        return attackRange;
    }
    
    // 设置攻击范围
    void SetAttackRange(float range) {
        attackRange = range;
    }
    
    // 获取移动速度
    float GetMovementSpeed() const {
        return movementSpeed;
    }
    
    // 计算到另一个对象的距离
    float DistanceTo(const GameObject& other) const {
        return position.DistanceTo(other.GetPosition());
    }
    
    // 计算到另一个对象的2D距离
    float DistanceTo2D(const GameObject& other) const {
        return GetPosition2D().DistanceTo(other.GetPosition2D());
    }
    
    // 计算到某个位置的距离
    float DistanceTo(const Vector3& pos) const {
        return position.DistanceTo(pos);
    }
    
    // 计算到某个位置的2D距离
    float DistanceTo(const Vector2& pos) const {
        return GetPosition2D().DistanceTo(pos);
    }
    
    // 是否在指定范围内
    bool IsInRange(const GameObject& other, float range) const {
        return DistanceTo(other) <= range;
    }
    
    // 是否在指定范围内(2D)
    bool IsInRange2D(const GameObject& other, float range) const {
        return DistanceTo2D(other) <= range;
    }
    
    // 是否在指定范围内(位置)
    bool IsInRange(const Vector3& pos, float range) const {
        return DistanceTo(pos) <= range;
    }
    
    // 是否在指定范围内(2D位置)
    bool IsInRange(const Vector2& pos, float range) const {
        return DistanceTo(pos) <= range;
    }
    
    // 获取指向目标的方向向量
    Vector3 GetDirectionTo(const GameObject& target) const {
        Vector3 dir = target.GetPosition() - position;
        return dir.Normalized();
    }
    
    // 获取指向目标的方向向量(2D)
    Vector2 GetDirectionTo2D(const GameObject& target) const {
        Vector2 dir = target.GetPosition2D() - GetPosition2D();
        return dir.Normalized();
    }
    
    // 获取指向位置的方向向量
    Vector3 GetDirectionTo(const Vector3& pos) const {
        Vector3 dir = pos - position;
        return dir.Normalized();
    }
    
    // 获取指向位置的方向向量(2D)
    Vector2 GetDirectionTo(const Vector2& pos) const {
        Vector2 dir = pos - GetPosition2D();
        return dir.Normalized();
    }
    
    // 计算预测位置
    Vector3 PredictPosition(float time, const Vector3& targetVelocity = Vector3::Zero) const {
        return position + targetVelocity * time;
    }
    
    // 计算预测位置(2D)
    Vector2 PredictPosition2D(float time, const Vector2& targetVelocity = Vector2::Zero) const {
        return GetPosition2D() + targetVelocity * time;
    }
    
    // 获取字符串表示
    virtual std::string ToString() const {
        std::string result = "GameObject[";
        result += "Name: " + name;
        result += ", Type: " + std::to_string(static_cast<int>(type));
        result += ", Team: " + std::to_string(static_cast<int>(team));
        result += ", Position: " + position.ToString();
        result += ", Health: " + std::to_string(health) + "/" + std::to_string(maxHealth);
        result += ", Visible: " + (isVisible ? "true" : "false");
        result += ", Alive: " + (isAlive ? "true" : "false");
        result += "]";
        return result;
    }
    
    // 比较运算符
    bool operator==(const GameObject& other) const {
        return address == other.address;
    }
    
    bool operator!=(const GameObject& other) const {
        return !(*this == other);
    }
    
    bool operator<(const GameObject& other) const {
        return address < other.address;
    }
    
    // 静态工具函数
    static GameObjectType GetObjectTypeFromString(const std::string& typeStr) {
        if (typeStr == "AIHeroClient") return GameObjectType::AIHeroClient;
        if (typeStr == "AIMinionClient") return GameObjectType::AIMinionClient;
        if (typeStr == "AITurretClient") return GameObjectType::AITurretClient;
        if (typeStr == "AttackableUnit") return GameObjectType::AttackableUnit;
        if (typeStr == "Building") return GameObjectType::Building;
        if (typeStr == "LevelProp") return GameObjectType::LevelProp;
        if (typeStr == "GrassObject") return GameObjectType::GrassObject;
        if (typeStr == "Obj_AI_Base") return GameObjectType::Obj_AI_Base;
        return GameObjectType::Unknown;
    }
    
    static GameObjectTeam GetTeamFromId(int teamId) {
        if (teamId == 100) return GameObjectTeam::Blue;
        if (teamId == 200) return GameObjectTeam::Red;
        if (teamId == 300) return GameObjectTeam::Neutral;
        return GameObjectTeam::Unknown;
    }
};