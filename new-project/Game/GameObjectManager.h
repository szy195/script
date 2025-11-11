#pragma once

#include "GameObject.h"
#include "Hero.h"
#include "Minion.h"
#include "Turret.h"
#include "../Core/MemoryManager.h"
#include "../Core/Offsets.h"
#include "../Utils/Vector.h"
#include "../Utils/Math.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <functional>

// 游戏对象管理器
class GameObjectManager {
private:
    std::vector<std::shared_ptr<GameObject>> gameObjects;
    std::unordered_map<uintptr_t, std::shared_ptr<GameObject>> objectMap;
    std::shared_ptr<Hero> localPlayer;
    std::vector<std::shared_ptr<Hero>> heroes;
    std::vector<std::shared_ptr<Minion>> minions;
    std::vector<std::shared_ptr<Turret>> turrets;
    
    uintptr_t objectListPtr;
    uintptr_t localPlayerPtr;
    uint32_t objectCount;
    bool isInitialized;
    
public:
    // 构造函数
    GameObjectManager() : objectListPtr(0), localPlayerPtr(0), objectCount(0), isInitialized(false) {}
    
    // 析构函数
    ~GameObjectManager() {
        Clear();
    }
    
    // 初始化
    bool Initialize() {
        MemoryManager* memManager = MemoryManager::GetInstance();
        if (!memManager) {
            return false;
        }
        
        // 获取游戏模块基址
        uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
        if (gameBase == 0) {
            return false;
        }
        
        // 读取对象列表指针和本地玩家指针
        // 这些偏移量需要根据游戏版本进行调整
        if (!memManager->Read(gameBase + Offsets::GameObjectManager::ObjectListPtr, objectListPtr)) {
            objectListPtr = 0;
        }
        if (!memManager->Read(gameBase + Offsets::GameObjectManager::LocalPlayerPtr, localPlayerPtr)) {
            localPlayerPtr = 0;
        }
        
        if (objectListPtr == 0 || localPlayerPtr == 0) {
            return false;
        }
        
        isInitialized = true;
        return true;
    }
    
    // 更新所有游戏对象
    void Update() {
        if (!isInitialized) {
            Initialize();
            return;
        }
        
        Clear();
        
        // 更新本地玩家
        UpdateLocalPlayer();
        
        // 更新对象列表
        UpdateObjectList();
        
        // 分类对象
        CategorizeObjects();
    }
    
    // 获取本地玩家
    std::shared_ptr<Hero> GetLocalPlayer() const {
        return localPlayer;
    }
    
    // 获取所有英雄
    const std::vector<std::shared_ptr<Hero>>& GetHeroes() const {
        return heroes;
    }
    
    // 获取所有小兵
    const std::vector<std::shared_ptr<Minion>>& GetMinions() const {
        return minions;
    }
    
    // 获取所有防御塔
    const std::vector<std::shared_ptr<Turret>>& GetTurrets() const {
        return turrets;
    }
    
    // 获取所有游戏对象
    const std::vector<std::shared_ptr<GameObject>>& GetGameObjects() const {
        return gameObjects;
    }
    
    // 根据地址获取游戏对象
    std::shared_ptr<GameObject> GetGameObjectByAddress(uintptr_t address) const {
        auto it = objectMap.find(address);
        if (it != objectMap.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    // 根据名称获取游戏对象
    std::shared_ptr<GameObject> GetGameObjectByName(const std::string& name) const {
        for (const auto& obj : gameObjects) {
            if (obj->GetName() == name) {
                return obj;
            }
        }
        return nullptr;
    }
    
    // 根据类型获取游戏对象
    template<typename T>
    std::vector<std::shared_ptr<T>> GetGameObjectsByType() const {
        std::vector<std::shared_ptr<T>> result;
        for (const auto& obj : gameObjects) {
            std::shared_ptr<T> castedObj = std::dynamic_pointer_cast<T>(obj);
            if (castedObj) {
                result.push_back(castedObj);
            }
        }
        return result;
    }
    
    // 获取范围内的游戏对象
    std::vector<std::shared_ptr<GameObject>> GetGameObjectsInRange(const Vector3& position, float range) const {
        std::vector<std::shared_ptr<GameObject>> result;
        for (const auto& obj : gameObjects) {
            if (obj->IsInRange(position, range)) {
                result.push_back(obj);
            }
        }
        return result;
    }
    
    // 获取范围内的游戏对象（2D）
    std::vector<std::shared_ptr<GameObject>> GetGameObjectsInRange(const Vector2& position, float range) const {
        std::vector<std::shared_ptr<GameObject>> result;
        for (const auto& obj : gameObjects) {
            if (obj->IsInRange(position, range)) {
                result.push_back(obj);
            }
        }
        return result;
    }
    
    // 获取范围内的英雄
    std::vector<std::shared_ptr<Hero>> GetHeroesInRange(const Vector3& position, float range) const {
        std::vector<std::shared_ptr<Hero>> result;
        for (const auto& hero : heroes) {
            if (hero->IsInRange(position, range)) {
                result.push_back(hero);
            }
        }
        return result;
    }
    
    // 获取范围内的英雄（2D）
    std::vector<std::shared_ptr<Hero>> GetHeroesInRange(const Vector2& position, float range) const {
        std::vector<std::shared_ptr<Hero>> result;
        for (const auto& hero : heroes) {
            if (hero->IsInRange(position, range)) {
                result.push_back(hero);
            }
        }
        return result;
    }
    
    // 获取范围内的敌方英雄
    std::vector<std::shared_ptr<Hero>> GetEnemyHeroesInRange(const Vector3& position, float range, GameObjectTeam team) const {
        std::vector<std::shared_ptr<Hero>> result;
        for (const auto& hero : heroes) {
            if (hero->GetTeam() != team && hero->IsInRange(position, range)) {
                result.push_back(hero);
            }
        }
        return result;
    }
    
    // 获取范围内的敌方英雄（2D）
    std::vector<std::shared_ptr<Hero>> GetEnemyHeroesInRange(const Vector2& position, float range, GameObjectTeam team) const {
        std::vector<std::shared_ptr<Hero>> result;
        for (const auto& hero : heroes) {
            if (hero->GetTeam() != team && hero->IsInRange(position, range)) {
                result.push_back(hero);
            }
        }
        return result;
    }
    
    // 获取范围内的友方英雄
    std::vector<std::shared_ptr<Hero>> GetAllyHeroesInRange(const Vector3& position, float range, GameObjectTeam team) const {
        std::vector<std::shared_ptr<Hero>> result;
        for (const auto& hero : heroes) {
            if (hero->GetTeam() == team && hero->IsInRange(position, range)) {
                result.push_back(hero);
            }
        }
        return result;
    }
    
    // 获取范围内的友方英雄（2D）
    std::vector<std::shared_ptr<Hero>> GetAllyHeroesInRange(const Vector2& position, float range, GameObjectTeam team) const {
        std::vector<std::shared_ptr<Hero>> result;
        for (const auto& hero : heroes) {
            if (hero->GetTeam() == team && hero->IsInRange(position, range)) {
                result.push_back(hero);
            }
        }
        return result;
    }
    
    // 获取范围内的小兵
    std::vector<std::shared_ptr<Minion>> GetMinionsInRange(const Vector3& position, float range) const {
        std::vector<std::shared_ptr<Minion>> result;
        for (const auto& minion : minions) {
            if (minion->IsInRange(position, range)) {
                result.push_back(minion);
            }
        }
        return result;
    }
    
    // 获取范围内的小兵（2D）
    std::vector<std::shared_ptr<Minion>> GetMinionsInRange(const Vector2& position, float range) const {
        std::vector<std::shared_ptr<Minion>> result;
        for (const auto& minion : minions) {
            if (minion->IsInRange(position, range)) {
                result.push_back(minion);
            }
        }
        return result;
    }
    
    // 获取范围内的敌方小兵
    std::vector<std::shared_ptr<Minion>> GetEnemyMinionsInRange(const Vector3& position, float range, GameObjectTeam team) const {
        std::vector<std::shared_ptr<Minion>> result;
        for (const auto& minion : minions) {
            if (minion->GetOwnerTeam() != team && minion->IsInRange(position, range)) {
                result.push_back(minion);
            }
        }
        return result;
    }
    
    // 获取范围内的敌方小兵（2D）
    std::vector<std::shared_ptr<Minion>> GetEnemyMinionsInRange(const Vector2& position, float range, GameObjectTeam team) const {
        std::vector<std::shared_ptr<Minion>> result;
        for (const auto& minion : minions) {
            if (minion->GetOwnerTeam() != team && minion->IsInRange(position, range)) {
                result.push_back(minion);
            }
        }
        return result;
    }
    
    // 获取范围内的友方小兵
    std::vector<std::shared_ptr<Minion>> GetAllyMinionsInRange(const Vector3& position, float range, GameObjectTeam team) const {
        std::vector<std::shared_ptr<Minion>> result;
        for (const auto& minion : minions) {
            if (minion->GetOwnerTeam() == team && minion->IsInRange(position, range)) {
                result.push_back(minion);
            }
        }
        return result;
    }
    
    // 获取范围内的友方小兵（2D）
    std::vector<std::shared_ptr<Minion>> GetAllyMinionsInRange(const Vector2& position, float range, GameObjectTeam team) const {
        std::vector<std::shared_ptr<Minion>> result;
        for (const auto& minion : minions) {
            if (minion->GetOwnerTeam() == team && minion->IsInRange(position, range)) {
                result.push_back(minion);
            }
        }
        return result;
    }
    
    // 获取最近的敌人
    template<typename T>
    std::shared_ptr<T> GetClosestEnemy(const Vector3& position, GameObjectTeam team, float maxRange = FLT_MAX) const {
        std::shared_ptr<T> closest;
        float closestDistance = maxRange;
        
        for (const auto& obj : gameObjects) {
            std::shared_ptr<T> castedObj = std::dynamic_pointer_cast<T>(obj);
            if (castedObj && castedObj->GetTeam() != team) {
                float distance = position.DistanceTo(castedObj->GetPosition());
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closest = castedObj;
                }
            }
        }
        
        return closest;
    }
    
    // 获取最近的敌人（2D）
    template<typename T>
    std::shared_ptr<T> GetClosestEnemy(const Vector2& position, GameObjectTeam team, float maxRange = FLT_MAX) const {
        std::shared_ptr<T> closest;
        float closestDistance = maxRange;
        
        for (const auto& obj : gameObjects) {
            std::shared_ptr<T> castedObj = std::dynamic_pointer_cast<T>(obj);
            if (castedObj && castedObj->GetTeam() != team) {
                float distance = position.DistanceTo(castedObj->GetPosition2D());
                if (distance < closestDistance) {
                    closestDistance = distance;
                    closest = castedObj;
                }
            }
        }
        
        return closest;
    }
    
    // 根据条件筛选游戏对象
    std::vector<std::shared_ptr<GameObject>> FilterGameObjects(std::function<bool(const std::shared_ptr<GameObject>&)> predicate) const {
        std::vector<std::shared_ptr<GameObject>> result;
        for (const auto& obj : gameObjects) {
            if (predicate(obj)) {
                result.push_back(obj);
            }
        }
        return result;
    }
    
    // 清空所有游戏对象
    void Clear() {
        gameObjects.clear();
        objectMap.clear();
        localPlayer.reset();
        heroes.clear();
        minions.clear();
        turrets.clear();
    }
    
    // 获取对象数量
    size_t GetObjectCount() const {
        return gameObjects.size();
    }
    
    // 获取英雄数量
    size_t GetHeroCount() const {
        return heroes.size();
    }
    
    // 获取小兵数量
    size_t GetMinionCount() const {
        return minions.size();
    }
    
    // 获取防御塔数量
    size_t GetTurretCount() const {
        return turrets.size();
    }
    
private:
    // 更新本地玩家
    void UpdateLocalPlayer() {
        if (localPlayerPtr == 0) {
            return;
        }
        
        localPlayer = std::make_shared<Hero>(localPlayerPtr);
        localPlayer->UpdateData();
        
        if (localPlayer->GetAddress() != 0) {
            gameObjects.push_back(localPlayer);
            objectMap[localPlayer->GetAddress()] = localPlayer;
        }
    }
    
    // 更新对象列表
    void UpdateObjectList() {
        if (objectListPtr == 0) {
            return;
        }
        
        MemoryManager* memManager = MemoryManager::GetInstance();
        if (!memManager) {
            return;
        }
        
        // 读取对象数量
        if (!memManager->Read(objectListPtr + 0x10, objectCount)) { // 假设对象数量偏移量
            objectCount = 0;
        }
        
        // 读取对象列表
        uintptr_t nodePtr;
        if (!memManager->Read(objectListPtr + 0x8, nodePtr)) { // 假设链表头偏移量
            return;
        }
        
        for (uint32_t i = 0; i < objectCount && nodePtr != 0; i++) {
            uintptr_t objectPtr;
            if (!memManager->Read(nodePtr + 0x20, objectPtr)) { // 假设对象指针偏移量
                break;
            }
            
            if (objectPtr != 0 && objectPtr != localPlayerPtr) {
                // 读取对象类型
                uint32_t objectType;
                if (!memManager->Read(objectPtr + 0x14, objectType)) { // 假设对象类型偏移量
                    objectType = 0;
                }
                
                // 根据类型创建对象
                std::shared_ptr<GameObject> obj = CreateGameObjectByType(objectPtr, objectType);
                if (obj) {
                    obj->UpdateData();
                    
                    if (obj->GetAddress() != 0) {
                        gameObjects.push_back(obj);
                        objectMap[obj->GetAddress()] = obj;
                    }
                }
            }
            
            // 移动到下一个节点
            if (!memManager->Read(nodePtr + 0x0, nodePtr)) { // 假设下一个节点偏移量
                break;
            }
        }
    }
    
    // 根据类型创建游戏对象
    std::shared_ptr<GameObject> CreateGameObjectByType(uintptr_t address, uint32_t objectType) {
        switch (objectType) {
            case static_cast<uint32_t>(GameObjectType::AIHeroClient):
                return std::make_shared<Hero>(address);
            case static_cast<uint32_t>(GameObjectType::AIMinionClient):
                return std::make_shared<Minion>(address);
            case static_cast<uint32_t>(GameObjectType::AITurretClient):
                return std::make_shared<Turret>(address);
            default:
                return std::make_shared<GameObject>(address);
        }
    }
    
    // 分类对象
    void CategorizeObjects() {
        for (const auto& obj : gameObjects) {
            if (obj->GetType() == GameObjectType::AIHeroClient) {
                std::shared_ptr<Hero> hero = std::dynamic_pointer_cast<Hero>(obj);
                if (hero && hero != localPlayer) {
                    heroes.push_back(hero);
                }
            } else if (obj->GetType() == GameObjectType::AIMinionClient) {
                std::shared_ptr<Minion> minion = std::dynamic_pointer_cast<Minion>(obj);
                if (minion) {
                    minions.push_back(minion);
                }
            } else if (obj->GetType() == GameObjectType::AITurretClient) {
                std::shared_ptr<Turret> turret = std::dynamic_pointer_cast<Turret>(obj);
                if (turret) {
                    turrets.push_back(turret);
                }
            }
        }
    }
};