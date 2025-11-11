#include "GameAPI.h"
#include "Offsets.h"
#include "GameObjectManager.h"
#include "../Utils/Math.h"
#ifdef _WIN32
#include <Windows.h>
#else
#include "Platform.h"
#endif
#include <iostream>

// 单例实例
GameAPI* GameAPI::instance = nullptr;

// 获取单例实例
GameAPI* GameAPI::GetInstance() {
    if (instance == nullptr) {
        instance = new GameAPI();
    }
    return instance;
}

// 销毁单例实例
void GameAPI::DestroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

// 构造函数
GameAPI::GameAPI() : gameManager(nullptr), memManager(nullptr), isInitialized(false), gameState(GameState::Unknown) {
    memManager = MemoryManager::GetInstance();
    gameManager = new GameObjectManager();
}

// 析构函数
GameAPI::~GameAPI() {
    Shutdown();
    if (gameManager != nullptr) {
        delete gameManager;
        gameManager = nullptr;
    }
}

// 初始化
bool GameAPI::Initialize() {
    if (isInitialized) {
        return true;
    }
    
    // 初始化内存管理器
    if (!memManager || !memManager->Initialize()) {
        return false;
    }
    
    // 初始化游戏对象管理器
    if (!gameManager || !gameManager->Initialize()) {
        return false;
    }
    
    // 获取游戏模块基址
    gameBase = memManager->GetModuleBase("League of Legends.exe");
    if (gameBase == 0) {
        return false;
    }
    
    // 初始化游戏状态
    UpdateGameState();
    
    isInitialized = true;
    return true;
}

// 关闭
void GameAPI::Shutdown() {
    if (gameManager) {
        gameManager->Clear();
    }
    
    isInitialized = false;
    gameState = GameState::Unknown;
}

// 更新
void GameAPI::Update() {
    if (!isInitialized) {
        return;
    }
    
    // 更新游戏状态
    UpdateGameState();
    
    // 更新游戏对象
    if (gameManager) {
        gameManager->Update();
    }
}

// 获取游戏状态
GameState GameAPI::GetGameState() const {
    return gameState;
}

// 是否在游戏中
bool GameAPI::IsInGame() const {
    return gameState == GameState::InProgress;
}

// 是否在加载界面
bool GameAPI::IsInLoadingScreen() const {
    return gameState == GameState::Loading;
}

// 是否在英雄选择界面
bool GameAPI::IsInChampionSelect() const {
    return gameState == GameState::ChampionSelect;
}

// 是否在主界面
bool GameAPI::IsInMainMenu() const {
    return gameState == GameState::MainMenu;
}

// 获取本地玩家
std::shared_ptr<Hero> GameAPI::GetLocalPlayer() const {
    if (!gameManager) {
        return nullptr;
    }
    
    return gameManager->GetLocalPlayer();
}

// 获取所有英雄
const std::vector<std::shared_ptr<Hero>>& GameAPI::GetHeroes() const {
    if (!gameManager) {
        static std::vector<std::shared_ptr<Hero>> empty;
        return empty;
    }
    
    return gameManager->GetHeroes();
}

// 获取所有小兵
const std::vector<std::shared_ptr<Minion>>& GameAPI::GetMinions() const {
    if (!gameManager) {
        static std::vector<std::shared_ptr<Minion>> empty;
        return empty;
    }
    
    return gameManager->GetMinions();
}

// 获取所有防御塔
const std::vector<std::shared_ptr<Turret>>& GameAPI::GetTurrets() const {
    if (!gameManager) {
        static std::vector<std::shared_ptr<Turret>> empty;
        return empty;
    }
    
    return gameManager->GetTurrets();
}

// 获取所有游戏对象
const std::vector<std::shared_ptr<GameObject>>& GameAPI::GetGameObjects() const {
    if (!gameManager) {
        static std::vector<std::shared_ptr<GameObject>> empty;
        return empty;
    }
    
    return gameManager->GetGameObjects();
}

// 获取范围内的游戏对象
std::vector<std::shared_ptr<GameObject>> GameAPI::GetGameObjectsInRange(const Vector3& position, float range) const {
    if (!gameManager) {
        return std::vector<std::shared_ptr<GameObject>>();
    }
    
    return gameManager->GetGameObjectsInRange(position, range);
}

// 获取范围内的游戏对象（2D）
std::vector<std::shared_ptr<GameObject>> GameAPI::GetGameObjectsInRange(const Vector2& position, float range) const {
    if (!gameManager) {
        return std::vector<std::shared_ptr<GameObject>>();
    }
    
    return gameManager->GetGameObjectsInRange(position, range);
}

// 获取范围内的英雄
std::vector<std::shared_ptr<Hero>> GameAPI::GetHeroesInRange(const Vector3& position, float range) const {
    if (!gameManager) {
        return std::vector<std::shared_ptr<Hero>>();
    }
    
    return gameManager->GetHeroesInRange(position, range);
}

// 获取范围内的英雄（2D）
std::vector<std::shared_ptr<Hero>> GameAPI::GetHeroesInRange(const Vector2& position, float range) const {
    if (!gameManager) {
        return std::vector<std::shared_ptr<Hero>>();
    }
    
    return gameManager->GetHeroesInRange(position, range);
}

// 获取范围内的敌方英雄
std::vector<std::shared_ptr<Hero>> GameAPI::GetEnemyHeroesInRange(const Vector3& position, float range) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return std::vector<std::shared_ptr<Hero>>();
    }
    
    return gameManager->GetEnemyHeroesInRange(position, range, localPlayer->GetTeam());
}

// 获取范围内的敌方英雄（2D）
std::vector<std::shared_ptr<Hero>> GameAPI::GetEnemyHeroesInRange(const Vector2& position, float range) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return std::vector<std::shared_ptr<Hero>>();
    }
    
    return gameManager->GetEnemyHeroesInRange(position, range, localPlayer->GetTeam());
}

// 获取范围内的友方英雄
std::vector<std::shared_ptr<Hero>> GameAPI::GetAllyHeroesInRange(const Vector3& position, float range) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return std::vector<std::shared_ptr<Hero>>();
    }
    
    return gameManager->GetAllyHeroesInRange(position, range, localPlayer->GetTeam());
}

// 获取范围内的友方英雄（2D）
std::vector<std::shared_ptr<Hero>> GameAPI::GetAllyHeroesInRange(const Vector2& position, float range) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return std::vector<std::shared_ptr<Hero>>();
    }
    
    return gameManager->GetAllyHeroesInRange(position, range, localPlayer->GetTeam());
}

// 获取范围内的小兵
std::vector<std::shared_ptr<Minion>> GameAPI::GetMinionsInRange(const Vector3& position, float range) const {
    if (!gameManager) {
        return std::vector<std::shared_ptr<Minion>>();
    }
    
    return gameManager->GetMinionsInRange(position, range);
}

// 获取范围内的小兵（2D）
std::vector<std::shared_ptr<Minion>> GameAPI::GetMinionsInRange(const Vector2& position, float range) const {
    if (!gameManager) {
        return std::vector<std::shared_ptr<Minion>>();
    }
    
    return gameManager->GetMinionsInRange(position, range);
}

// 获取范围内的敌方小兵
std::vector<std::shared_ptr<Minion>> GameAPI::GetEnemyMinionsInRange(const Vector3& position, float range) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return std::vector<std::shared_ptr<Minion>>();
    }
    
    return gameManager->GetEnemyMinionsInRange(position, range, localPlayer->GetTeam());
}

// 获取范围内的敌方小兵（2D）
std::vector<std::shared_ptr<Minion>> GameAPI::GetEnemyMinionsInRange(const Vector2& position, float range) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return std::vector<std::shared_ptr<Minion>>();
    }
    
    return gameManager->GetEnemyMinionsInRange(position, range, localPlayer->GetTeam());
}

// 获取范围内的友方小兵
std::vector<std::shared_ptr<Minion>> GameAPI::GetAllyMinionsInRange(const Vector3& position, float range) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return std::vector<std::shared_ptr<Minion>>();
    }
    
    return gameManager->GetAllyMinionsInRange(position, range, localPlayer->GetTeam());
}

// 获取范围内的友方小兵（2D）
std::vector<std::shared_ptr<Minion>> GameAPI::GetAllyMinionsInRange(const Vector2& position, float range) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return std::vector<std::shared_ptr<Minion>>();
    }
    
    return gameManager->GetAllyMinionsInRange(position, range, localPlayer->GetTeam());
}

// 获取最近的敌人
std::shared_ptr<Hero> GameAPI::GetClosestEnemyHero(const Vector3& position, float maxRange) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return nullptr;
    }
    
    return gameManager->GetClosestEnemy<Hero>(position, localPlayer->GetTeam(), maxRange);
}

// 获取最近的敌人（2D）
std::shared_ptr<Hero> GameAPI::GetClosestEnemyHero(const Vector2& position, float maxRange) const {
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer || !gameManager) {
        return nullptr;
    }
    
    return gameManager->GetClosestEnemy<Hero>(position, localPlayer->GetTeam(), maxRange);
}

// 获取游戏时间
float GameAPI::GetGameTime() const {
    if (!memManager || gameBase == 0) {
        return 0.0f;
    }
    
    return memManager->Read<float>(gameBase + Offsets::Game::GameTime);
}

// 获取游戏TickCount
uint32_t GameAPI::GetGameTick() const {
    if (!memManager || gameBase == 0) {
        return 0;
    }
    
    return memManager->Read<uint32_t>(gameBase + Offsets::Game::GameTickCount);
}

// 世界坐标转屏幕坐标
Vector2 GameAPI::WorldToScreen(const Vector3& worldPos) const {
    if (!memManager || gameBase == 0) {
        return Vector2::Zero;
    }
    
    // 读取视图矩阵
    uintptr_t viewMatrixPtr = memManager->Read<uintptr_t>(gameBase + Offsets::Game::ViewMatrixPtr);
    if (viewMatrixPtr == 0) {
        return Vector2::Zero;
    }
    
    float viewMatrix[16];
    memManager->ReadMemory(viewMatrixPtr, viewMatrix, sizeof(viewMatrix));
    
    // 读取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    // 世界坐标转屏幕坐标
    float w = viewMatrix[3] * worldPos.x + viewMatrix[7] * worldPos.y + viewMatrix[11] * worldPos.z + viewMatrix[15];
    
    if (w < 0.1f) {
        return Vector2::Zero; // 在摄像机后面
    }
    
    float x = (viewMatrix[0] * worldPos.x + viewMatrix[4] * worldPos.y + viewMatrix[8] * worldPos.z + viewMatrix[12]) / w;
    float y = (viewMatrix[1] * worldPos.x + viewMatrix[5] * worldPos.y + viewMatrix[9] * worldPos.z + viewMatrix[13]) / w;
    
    // 转换为屏幕坐标
    x = (x + 1.0f) * 0.5f * screenWidth;
    y = (1.0f - y) * 0.5f * screenHeight;
    
    return Vector2(x, y);
}

// 屏幕坐标转世界坐标
Vector3 GameAPI::ScreenToWorld(const Vector2& screenPos) const {
    if (!memManager || gameBase == 0) {
        return Vector3::Zero;
    }
    
    // 读取视图矩阵
    uintptr_t viewMatrixPtr = memManager->Read<uintptr_t>(gameBase + Offsets::Game::ViewMatrixPtr);
    if (viewMatrixPtr == 0) {
        return Vector3::Zero;
    }
    
    float viewMatrix[16];
    memManager->ReadMemory(viewMatrixPtr, viewMatrix, sizeof(viewMatrix));
    
    // 读取屏幕尺寸
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    
    // 屏幕坐标转标准化设备坐标
    float x = (2.0f * screenPos.x / screenWidth) - 1.0f;
    float y = 1.0f - (2.0f * screenPos.y / screenHeight);
    
    // 获取本地玩家位置作为参考点
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer) {
        return Vector3::Zero;
    }
    
    Vector3 playerPos = localPlayer->GetPosition();
    
    // 简化的屏幕坐标转世界坐标（假设在相同高度）
    // 在实际项目中，需要更复杂的计算，可能需要射线与地面平面的交点
    Vector3 worldPos = playerPos;
    
    // 这里简化处理，实际需要根据视图矩阵逆变换计算
    // 由于计算复杂，这里只返回一个近似值
    return worldPos;
}

// 发送聊天消息
void GameAPI::SendChatMessage(const std::string& message) const {
    if (!memManager || gameBase == 0) {
        return;
    }
    
    // 获取聊天函数地址
    uintptr_t chatFunction = memManager->Read<uintptr_t>(gameBase + Offsets::Game::ChatFunction);
    if (chatFunction == 0) {
        return;
    }
    
    // 创建函数调用
    typedef void(__cdecl* ChatFunction)(const char*);
    ChatFunction func = reinterpret_cast<ChatFunction>(chatFunction);
    
    // 调用函数发送消息
    func(message.c_str());
}

// 发送移动指令
void GameAPI::IssueMoveOrder(const Vector3& position) const {
    if (!memManager || gameBase == 0) {
        return;
    }
    
    // 获取移动函数地址
    uintptr_t moveFunction = memManager->Read<uintptr_t>(gameBase + Offsets::Game::MoveFunction);
    if (moveFunction == 0) {
        return;
    }
    
    // 获取本地玩家地址
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer) {
        return;
    }
    
    // 创建函数调用
    typedef void(__cdecl* MoveFunction)(uintptr_t, float, float, float);
    MoveFunction func = reinterpret_cast<MoveFunction>(moveFunction);
    
    // 调用函数发送移动指令
    func(localPlayer->GetAddress(), position.x, position.y, position.z);
}

// 发送攻击指令
void GameAPI::IssueAttackOrder(uintptr_t target) const {
    if (!memManager || gameBase == 0 || target == 0) {
        return;
    }
    
    // 获取攻击函数地址
    uintptr_t attackFunction = memManager->Read<uintptr_t>(gameBase + Offsets::Game::AttackFunction);
    if (attackFunction == 0) {
        return;
    }
    
    // 获取本地玩家地址
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer) {
        return;
    }
    
    // 创建函数调用
    typedef void(__cdecl* AttackFunction)(uintptr_t, uintptr_t);
    AttackFunction func = reinterpret_cast<AttackFunction>(attackFunction);
    
    // 调用函数发送攻击指令
    func(localPlayer->GetAddress(), target);
}

// 发送技能指令
void GameAPI::IssueSpellOrder(SpellSlot slot, const Vector3& position, uintptr_t target) const {
    if (!memManager || gameBase == 0) {
        return;
    }
    
    // 获取技能函数地址
    uintptr_t spellFunction = memManager->Read<uintptr_t>(gameBase + Offsets::Game::SpellFunction);
    if (spellFunction == 0) {
        return;
    }
    
    // 获取本地玩家地址
    auto localPlayer = GetLocalPlayer();
    if (!localPlayer) {
        return;
    }
    
    // 创建函数调用
    typedef void(__cdecl* SpellFunction)(uintptr_t, int, float, float, float, uintptr_t);
    SpellFunction func = reinterpret_cast<SpellFunction>(spellFunction);
    
    // 调用函数发送技能指令
    int slotInt = static_cast<int>(slot);
    func(localPlayer->GetAddress(), slotInt, position.x, position.y, position.z, target);
}

// 发送技能指令（无目标）
void GameAPI::IssueSpellOrder(SpellSlot slot) const {
    IssueSpellOrder(slot, Vector3::Zero, 0);
}

// 发送技能指令（位置目标）
void GameAPI::IssueSpellOrder(SpellSlot slot, const Vector3& position) const {
    IssueSpellOrder(slot, position, 0);
}

// 发送技能指令（单位目标）
void GameAPI::IssueSpellOrder(SpellSlot slot, uintptr_t target) const {
    IssueSpellOrder(slot, Vector3::Zero, target);
}

// 发送技能指令（游戏对象目标）
void GameAPI::IssueSpellOrder(SpellSlot slot, const std::shared_ptr<GameObject>& target) const {
    if (!target) {
        return;
    }
    IssueSpellOrder(slot, Vector3::Zero, target->GetAddress());
}

// 更新游戏状态
void GameAPI::UpdateGameState() {
    if (!memManager || gameBase == 0) {
        gameState = GameState::Unknown;
        return;
    }
    
    // 读取游戏状态
    uint32_t stateValue = memManager->Read<uint32_t>(gameBase + Offsets::Game::GameState);
    
    switch (stateValue) {
        case 0:
            gameState = GameState::MainMenu;
            break;
        case 1:
            gameState = GameState::ChampionSelect;
            break;
        case 2:
            gameState = GameState::Loading;
            break;
        case 3:
            gameState = GameState::InProgress;
            break;
        case 4:
            gameState = GameState::GameEnd;
            break;
        default:
            gameState = GameState::Unknown;
            break;
    }
}