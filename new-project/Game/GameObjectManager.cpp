#include "GameObjectManager.h"
#include "../Core/MemoryManager.h"
#include "../Core/GameAPI.h"
#include "../Core/Offsets.h"
#include "../Utils/Vector.h"
#include "../Utils/Math.h"
#include <algorithm>
#include <iostream>

// 构造函数
GameObjectManager::GameObjectManager() : memManager(nullptr), gameAPI(nullptr), isInitialized(false) {
    // 预分配容器空间
    gameObjects.reserve(1000);
    heroes.reserve(20);
    minions.reserve(200);
    turrets.reserve(30);
    localPlayer = nullptr;
}

// 析构函数
GameObjectManager::~GameObjectManager() {
    Shutdown();
}

// 初始化
bool GameObjectManager::Initialize() {
    if (isInitialized) {
        return true;
    }
    
    memManager = MemoryManager::GetInstance();
    gameAPI = GameAPI::GetInstance();
    
    if (!memManager || !gameAPI) {
        return false;
    }
    
    isInitialized = true;
    return true;
}

// 关闭
void GameObjectManager::Shutdown() {
    Clear();
    isInitialized = false;
}

// 清空所有游戏对象
void GameObjectManager::Clear() {
    gameObjects.clear();
    heroes.clear();
    minions.clear();
    turrets.clear();
    localPlayer = nullptr;
}

// 更新游戏对象
void GameObjectManager::Update() {
    if (!isInitialized) {
        return;
    }
    
    // 获取游戏对象列表
    std::vector<uintptr_t> objectList = GetGameObjectList();
    
    // 处理已存在的对象
    UpdateExistingObjects(objectList);
    
    // 处理新对象
    ProcessNewObjects(objectList);
    
    // 处理已删除的对象
    ProcessDeletedObjects(objectList);
    
    // 更新本地玩家
    UpdateLocalPlayer();
}

// 获取游戏对象列表
std::vector<uintptr_t> GameObjectManager::GetGameObjectList() {
    std::vector<uintptr_t> objectList;
    
    if (!memManager) {
        return objectList;
    }
    
    // 获取游戏模块基址
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (gameBase == 0) {
        return objectList;
    }
    
    // 获取对象管理器指针
    uintptr_t objManagerPtr;
    if (!memManager->Read(gameBase + Offsets::GameObjectManager::ObjectManager, objManagerPtr) || objManagerPtr == 0) {
        return objectList;
    }
    
    // 获取对象列表指针
    uintptr_t objListPtr;
    if (!memManager->Read(objManagerPtr + Offsets::GameObjectManager::ObjectList, objListPtr) || objListPtr == 0) {
        return objectList;
    }
    
    // 获取对象数量
    uint32_t objCount;
    if (!memManager->Read(objManagerPtr + Offsets::GameObjectManager::ObjectCount, objCount)) {
        objCount = 0;
    }
    
    // 读取对象列表
    objectList.reserve(objCount);
    for (uint32_t i = 0; i < objCount; ++i) {
        uintptr_t objPtr;
        if (memManager->Read(objListPtr + i * sizeof(uintptr_t), objPtr) && objPtr != 0) {
            objectList.push_back(objPtr);
        }
    }
    
    return objectList;
}

// 更新已存在的对象
void GameObjectManager::UpdateExistingObjects(const std::vector<uintptr_t>& currentObjects) {
    for (auto& obj : gameObjects) {
        if (!obj) {
            continue;
        }
        
        uintptr_t address = obj->GetAddress();
        
        // 检查对象是否仍在列表中
        auto it = std::find(currentObjects.begin(), currentObjects.end(), address);
        if (it == currentObjects.end()) {
            // 对象已被删除，标记为无效
            obj->SetValid(false);
            continue;
        }
        
        // 更新对象数据
        obj->UpdateData();
    }
}

// 处理新对象
void GameObjectManager::ProcessNewObjects(const std::vector<uintptr_t>& currentObjects) {
    for (uintptr_t address : currentObjects) {
        // 检查对象是否已存在
        bool exists = false;
        for (const auto& obj : gameObjects) {
            if (obj && obj->GetAddress() == address) {
                exists = true;
                break;
            }
        }
        
        if (exists) {
            continue;
        }
        
        // 创建新对象
        std::shared_ptr<GameObject> newObject = CreateGameObject(address);
        if (newObject) {
            gameObjects.push_back(newObject);
            
            // 根据类型添加到相应列表
            GameObjectType type = newObject->GetType();
            if (type == GameObjectType::Hero) {
                heroes.push_back(std::static_pointer_cast<Hero>(newObject));
            } else if (type == GameObjectType::Minion) {
                minions.push_back(std::static_pointer_cast<Minion>(newObject));
            } else if (type == GameObjectType::Turret) {
                turrets.push_back(std::static_pointer_cast<Turret>(newObject));
            }
        }
    }
}

// 处理已删除的对象
void GameObjectManager::ProcessDeletedObjects(const std::vector<uintptr_t>& currentObjects) {
    // 从游戏对象列表中移除无效对象
    gameObjects.erase(
        std::remove_if(gameObjects.begin(), gameObjects.end(),
            [](const std::shared_ptr<GameObject>& obj) { return !obj || !obj->IsValid(); }),
        gameObjects.end()
    );
    
    // 从英雄列表中移除无效对象
    heroes.erase(
        std::remove_if(heroes.begin(), heroes.end(),
            [](const std::shared_ptr<Hero>& obj) { return !obj || !obj->IsValid(); }),
        heroes.end()
    );
    
    // 从小兵列表中移除无效对象
    minions.erase(
        std::remove_if(minions.begin(), minions.end(),
            [](const std::shared_ptr<Minion>& obj) { return !obj || !obj->IsValid(); }),
        minions.end()
    );
    
    // 从防御塔列表中移除无效对象
    turrets.erase(
        std::remove_if(turrets.begin(), turrets.end(),
            [](const std::shared_ptr<Turret>& obj) { return !obj || !obj->IsValid(); }),
        turrets.end()
    );
}

// 更新本地玩家
void GameObjectManager::UpdateLocalPlayer() {
    if (!memManager) {
        return;
    }
    
    // 获取游戏模块基址
    uintptr_t gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (gameBase == 0) {
        return;
    }
    
    // 获取本地玩家指针
    uintptr_t localPlayerPtr;
    if (!memManager->Read(gameBase + Offsets::GameObjectManager::LocalPlayerPtr, localPlayerPtr) || localPlayerPtr == 0) {
        localPlayer = nullptr;
        return;
    }
    
    // 查找本地玩家对象
    for (const auto& hero : heroes) {
        if (hero && hero->GetAddress() == localPlayerPtr) {
            localPlayer = hero;
            return;
        }
    }
    
    // 如果没找到，说明本地玩家可能是其他类型
    for (const auto& obj : gameObjects) {
        if (obj && obj->GetAddress() == localPlayerPtr) {
            localPlayer = std::dynamic_pointer_cast<Hero>(obj);
            return;
        }
    }
    
    localPlayer = nullptr;
}

// 创建游戏对象
std::shared_ptr<GameObject> GameObjectManager::CreateGameObject(uintptr_t address) {
    if (!memManager || address == 0) {
        return nullptr;
    }
    
    // 读取对象类型
    uint32_t typeValue;
    if (!memManager->Read(address + Offsets::GameObject::Type, typeValue)) {
        typeValue = 0;
    }
    
    GameObjectType type = static_cast<GameObjectType>(typeValue);
    
    // 根据类型创建对象
    switch (type) {
        case GameObjectType::Hero:
            return std::make_shared<Hero>(address);
        case GameObjectType::Minion:
            return std::make_shared<Minion>(address);
        case GameObjectType::Turret:
            return std::make_shared<Turret>(address);
        default:
            return std::make_shared<GameObject>(address, type);
    }
}

// 获取本地玩家
std::shared_ptr<Hero> GameObjectManager::GetLocalPlayer() const {
    return localPlayer;
}

// 获取所有游戏对象
const std::vector<std::shared_ptr<GameObject>>& GameObjectManager::GetGameObjects() const {
    return gameObjects;
}

// 获取所有英雄
const std::vector<std::shared_ptr<Hero>>& GameObjectManager::GetHeroes() const {
    return heroes;
}

// 获取所有小兵
const std::vector<std::shared_ptr<Minion>>& GameObjectManager::GetMinions() const {
    return minions;
}

// 获取所有防御塔
const std::vector<std::shared_ptr<Turret>>& GameObjectManager::GetTurrets() const {
    return turrets;
}

// 获取范围内的游戏对象
std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectsInRange(const Vector3& position, float range) const {
    std::vector<std::shared_ptr<GameObject>> result;
    
    for (const auto& obj : gameObjects) {
        if (!obj || !obj->IsValid()) {
            continue;
        }
        
        if (obj->GetPosition().DistanceTo(position) <= range) {
            result.push_back(obj);
        }
    }
    
    return result;
}

// 获取范围内的游戏对象（2D）
std::vector<std::shared_ptr<GameObject>> GameObjectManager::GetGameObjectsInRange(const Vector2& position, float range) const {
    std::vector<std::shared_ptr<GameObject>> result;
    
    for (const auto& obj : gameObjects) {
        if (!obj || !obj->IsValid()) {
            continue;
        }
        
        Vector3 objPos = obj->GetPosition();
        Vector2 objPos2D(objPos.x, objPos.z);
        
        if (objPos2D.DistanceTo(position) <= range) {
            result.push_back(obj);
        }
    }
    
    return result;
}

// 获取范围内的英雄
std::vector<std::shared_ptr<Hero>> GameObjectManager::GetHeroesInRange(const Vector3& position, float range) const {
    std::vector<std::shared_ptr<Hero>> result;
    
    for (const auto& hero : heroes) {
        if (!hero || !hero->IsValid()) {
            continue;
        }
        
        if (hero->GetPosition().DistanceTo(position) <= range) {
            result.push_back(hero);
        }
    }
    
    return result;
}

// 获取范围内的英雄（2D）
std::vector<std::shared_ptr<Hero>> GameObjectManager::GetHeroesInRange(const Vector2& position, float range) const {
    std::vector<std::shared_ptr<Hero>> result;
    
    for (const auto& hero : heroes) {
        if (!hero || !hero->IsValid()) {
            continue;
        }
        
        Vector3 heroPos = hero->GetPosition();
        Vector2 heroPos2D(heroPos.x, heroPos.z);
        
        if (heroPos2D.DistanceTo(position) <= range) {
            result.push_back(hero);
        }
    }
    
    return result;
}

// 获取范围内的敌方英雄
std::vector<std::shared_ptr<Hero>> GameObjectManager::GetEnemyHeroesInRange(const Vector3& position, GameObjectTeam team, float range) const {
    std::vector<std::shared_ptr<Hero>> result;
    
    for (const auto& hero : heroes) {
        if (!hero || !hero->IsValid()) {
            continue;
        }
        
        if (hero->GetTeam() != team && hero->GetPosition().DistanceTo(position) <= range) {
            result.push_back(hero);
        }
    }
    
    return result;
}

// 获取范围内的敌方英雄（2D）
std::vector<std::shared_ptr<Hero>> GameObjectManager::GetEnemyHeroesInRange(const Vector2& position, GameObjectTeam team, float range) const {
    std::vector<std::shared_ptr<Hero>> result;
    
    for (const auto& hero : heroes) {
        if (!hero || !hero->IsValid()) {
            continue;
        }
        
        Vector3 heroPos = hero->GetPosition();
        Vector2 heroPos2D(heroPos.x, heroPos.z);
        
        if (hero->GetTeam() != team && heroPos2D.DistanceTo(position) <= range) {
            result.push_back(hero);
        }
    }
    
    return result;
}

// 获取范围内的友方英雄
std::vector<std::shared_ptr<Hero>> GameObjectManager::GetAllyHeroesInRange(const Vector3& position, GameObjectTeam team, float range) const {
    std::vector<std::shared_ptr<Hero>> result;
    
    for (const auto& hero : heroes) {
        if (!hero || !hero->IsValid()) {
            continue;
        }
        
        if (hero->GetTeam() == team && hero->GetPosition().DistanceTo(position) <= range) {
            result.push_back(hero);
        }
    }
    
    return result;
}

// 获取范围内的友方英雄（2D）
std::vector<std::shared_ptr<Hero>> GameObjectManager::GetAllyHeroesInRange(const Vector2& position, GameObjectTeam team, float range) const {
    std::vector<std::shared_ptr<Hero>> result;
    
    for (const auto& hero : heroes) {
        if (!hero || !hero->IsValid()) {
            continue;
        }
        
        Vector3 heroPos = hero->GetPosition();
        Vector2 heroPos2D(heroPos.x, heroPos.z);
        
        if (hero->GetTeam() == team && heroPos2D.DistanceTo(position) <= range) {
            result.push_back(hero);
        }
    }
    
    return result;
}

// 获取范围内的小兵
std::vector<std::shared_ptr<Minion>> GameObjectManager::GetMinionsInRange(const Vector3& position, float range) const {
    std::vector<std::shared_ptr<Minion>> result;
    
    for (const auto& minion : minions) {
        if (!minion || !minion->IsValid()) {
            continue;
        }
        
        if (minion->GetPosition().DistanceTo(position) <= range) {
            result.push_back(minion);
        }
    }
    
    return result;
}

// 获取范围内的小兵（2D）
std::vector<std::shared_ptr<Minion>> GameObjectManager::GetMinionsInRange(const Vector2& position, float range) const {
    std::vector<std::shared_ptr<Minion>> result;
    
    for (const auto& minion : minions) {
        if (!minion || !minion->IsValid()) {
            continue;
        }
        
        Vector3 minionPos = minion->GetPosition();
        Vector2 minionPos2D(minionPos.x, minionPos.z);
        
        if (minionPos2D.DistanceTo(position) <= range) {
            result.push_back(minion);
        }
    }
    
    return result;
}

// 获取范围内的敌方小兵
std::vector<std::shared_ptr<Minion>> GameObjectManager::GetEnemyMinionsInRange(const Vector3& position, GameObjectTeam team, float range) const {
    std::vector<std::shared_ptr<Minion>> result;
    
    for (const auto& minion : minions) {
        if (!minion || !minion->IsValid()) {
            continue;
        }
        
        if (minion->GetTeam() != team && minion->GetPosition().DistanceTo(position) <= range) {
            result.push_back(minion);
        }
    }
    
    return result;
}

// 获取范围内的敌方小兵（2D）
std::vector<std::shared_ptr<Minion>> GameObjectManager::GetEnemyMinionsInRange(const Vector2& position, GameObjectTeam team, float range) const {
    std::vector<std::shared_ptr<Minion>> result;
    
    for (const auto& minion : minions) {
        if (!minion || !minion->IsValid()) {
            continue;
        }
        
        Vector3 minionPos = minion->GetPosition();
        Vector2 minionPos2D(minionPos.x, minionPos.z);
        
        if (minion->GetTeam() != team && minionPos2D.DistanceTo(position) <= range) {
            result.push_back(minion);
        }
    }
    
    return result;
}

// 获取范围内的友方小兵
std::vector<std::shared_ptr<Minion>> GameObjectManager::GetAllyMinionsInRange(const Vector3& position, GameObjectTeam team, float range) const {
    std::vector<std::shared_ptr<Minion>> result;
    
    for (const auto& minion : minions) {
        if (!minion || !minion->IsValid()) {
            continue;
        }
        
        if (minion->GetTeam() == team && minion->GetPosition().DistanceTo(position) <= range) {
            result.push_back(minion);
        }
    }
    
    return result;
}

// 获取范围内的友方小兵（2D）
std::vector<std::shared_ptr<Minion>> GameObjectManager::GetAllyMinionsInRange(const Vector2& position, GameObjectTeam team, float range) const {
    std::vector<std::shared_ptr<Minion>> result;
    
    for (const auto& minion : minions) {
        if (!minion || !minion->IsValid()) {
            continue;
        }
        
        Vector3 minionPos = minion->GetPosition();
        Vector2 minionPos2D(minionPos.x, minionPos.z);
        
        if (minion->GetTeam() == team && minionPos2D.DistanceTo(position) <= range) {
            result.push_back(minion);
        }
    }
    
    return result;
}

// 获取最近的敌人
template<typename T>
std::shared_ptr<T> GameObjectManager::GetClosestEnemy(const Vector3& position, GameObjectTeam team, float maxRange) const {
    std::shared_ptr<T> closestEnemy = nullptr;
    float closestDistance = maxRange;
    
    for (const auto& obj : gameObjects) {
        if (!obj || !obj->IsValid()) {
            continue;
        }
        
        auto castedObj = std::dynamic_pointer_cast<T>(obj);
        if (!castedObj || castedObj->GetTeam() == team) {
            continue;
        }
        
        float distance = castedObj->GetPosition().DistanceTo(position);
        if (distance < closestDistance) {
            closestDistance = distance;
            closestEnemy = castedObj;
        }
    }
    
    return closestEnemy;
}

// 获取最近的敌人（2D）
template<typename T>
std::shared_ptr<T> GameObjectManager::GetClosestEnemy(const Vector2& position, GameObjectTeam team, float maxRange) const {
    std::shared_ptr<T> closestEnemy = nullptr;
    float closestDistance = maxRange;
    
    for (const auto& obj : gameObjects) {
        if (!obj || !obj->IsValid()) {
            continue;
        }
        
        auto castedObj = std::dynamic_pointer_cast<T>(obj);
        if (!castedObj || castedObj->GetTeam() == team) {
            continue;
        }
        
        Vector3 objPos = castedObj->GetPosition();
        Vector2 objPos2D(objPos.x, objPos.z);
        
        float distance = objPos2D.DistanceTo(position);
        if (distance < closestDistance) {
            closestDistance = distance;
            closestEnemy = castedObj;
        }
    }
    
    return closestEnemy;
}