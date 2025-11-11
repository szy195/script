#pragma once

#include "MemoryManager.h"
#include "../Utils/Vector.h"
#include "../Game/GameObjectManager.h"
#ifdef _WIN32
#include <windows.h>
#else
#include "Platform.h"
#endif
#include <string>
#include <vector>
#include <memory>
#include <map>

// 前向声明
class GameObject;
class Hero;
class Minion;
class Missile;
class Turret;

// 游戏状态
enum class GameState {
    UNKNOWN,
    IN_GAME,
    IN_LOBBY,
    IN_CHAMP_SELECT,
    IN_POST_GAME,
    LOADING,
    DISCONNECTED
};

// 游戏对象类型
enum class ObjectType {
    UNKNOWN,
    HERO,
    MINION,
    TURRET,
    MISSILE,
    INHIBITOR,
    NEXUS,
    SHOP,
    ATTACKABLE_UNIT
};

// 游戏API类
class GameAPI {
public:
    GameAPI();
    ~GameAPI();
    
    // 初始化和清理
    bool Initialize();
    void Shutdown();
    bool IsInitialized() const;
    
    // 游戏状态
    bool IsGameActive() const;
    GameState GetGameState() const;
    float GetGameTime() const;
    int GetGameTickCount() const;
    
    // 本地玩家信息
    Hero* GetLocalPlayer() const;
    int GetLocalPlayerTeam() const;
    bool IsLocalPlayerDead() const;
    Vector3 GetLocalPlayerPosition() const;
    float GetLocalPlayerHealth() const;
    float GetLocalPlayerMaxHealth() const;
    float GetLocalPlayerMana() const;
    float GetLocalPlayerMaxMana() const;
    float GetLocalPlayerAttackRange() const;
    
    // 游戏对象管理
    ObjectManager* GetObjectManager() const;
    std::vector<Hero*> GetHeroes() const;
    std::vector<Hero*> GetAllyHeroes() const;
    std::vector<Hero*> GetEnemyHeroes() const;
    std::vector<Minion*> GetMinions() const;
    std::vector<Minion*> GetAllyMinions() const;
    std::vector<Minion*> GetEnemyMinions() const;
    std::vector<Turret*> GetTurrets() const;
    std::vector<Missile*> GetMissiles() const;
    
    // 游戏对象查找
    GameObject* GetObjectByNetworkId(int networkId) const;
    GameObject* GetObjectByAddress(uintptr_t address) const;
    Hero* GetHeroByName(const std::string& name) const;
    Hero* GetHeroByChampionName(const std::string& championName) const;
    
    // 游戏世界信息
    Vector3 GetWorldToScreen(const Vector3& worldPos) const;
    Vector3 GetScreenToWorld(const Vector3& screenPos) const;
    bool IsOnScreen(const Vector3& worldPos) const;
    float GetDistance(const Vector3& pos1, const Vector3& pos2) const;
    
    // 游戏设置
    float GetPing() const;
    int GetFPS() const;
    Vector2 GetScreenSize() const;
    Vector2 GetMousePosition() const;
    bool IsCursorVisible() const;
    
    // 游戏事件
    bool IsChatOpen() const;
    bool IsShopOpen() const;
    bool IsScoreboardOpen() const;
    std::string GetChatMessage() const;
    void SendChatMessage(const std::string& message) const;
    
    // 游戏控制
    void MoveTo(const Vector3& position) const;
    void Attack(GameObject* target) const;
    void CastSpell(int slot, const Vector3& position = Vector3()) const;
    void CastSpell(int slot, GameObject* target) const;
    void UseItem(int slot) const;
    void Recall() const;
    
    // 游戏信息
    int GetGold() const;
    int GetLevel() const;
    float GetExperience() const;
    std::vector<int> GetInventory() const;
    float GetAbilityPower() const;
    float GetAttackDamage() const;
    float GetArmor() const;
    float GetMagicResist() const;
    float GetCooldownReduction() const;
    float GetAttackSpeed() const;
    float GetMovementSpeed() const;
    float GetLifeSteal() const;
    float GetSpellVamp() const;
    
    // 技能信息
    bool IsSpellReady(int slot) const;
    float GetSpellCooldown(int slot) const;
    float GetSpellRange(int slot) const;
    float GetSpellDamage(int slot, GameObject* target) const;
    std::string GetSpellName(int slot) const;
    std::string GetSpellIcon(int slot) const;
    
    // 游戏更新
    void UpdateObjects();
    void UpdateLocalPlayer();
    void UpdateGameState();
    void UpdateGameSettings();
    
    // 偏移量管理
    bool LoadOffsets(const std::string& filePath);
    bool SaveOffsets(const std::string& filePath);
    uintptr_t GetOffset(const std::string& name) const;
    
    // 调试信息
    void PrintGameInfo() const;
    void PrintObjectList() const;
    void PrintLocalPlayerInfo() const;
    
private:
    // 内部方法
    bool InitializeOffsets();
    bool InitializeHooks();
    void ShutdownHooks();
    
    // 游戏对象创建
    Hero* CreateHero(uintptr_t address) const;
    Minion* CreateMinion(uintptr_t address) const;
    Turret* CreateTurret(uintptr_t address) const;
    Missile* CreateMissile(uintptr_t address) const;
    GameObject* CreateGameObject(uintptr_t address) const;
    
    // 游戏状态检测
    GameState DetectGameState() const;
    bool IsInGame() const;
    bool IsInLobby() const;
    bool IsInChampionSelect() const;
    
    // 成员变量
    bool m_initialized = false;
    MemoryManager* m_memoryManager = nullptr;
    std::unique_ptr<ObjectManager> m_objectManager;
    
    // 缓存
    Hero* m_localPlayer = nullptr;
    GameState m_gameState = GameState::UNKNOWN;
    float m_gameTime = 0.0f;
    int m_gameTickCount = 0;
    Vector2 m_screenSize;
    float m_ping = 0.0f;
    int m_fps = 0;
    
    // 偏移量
    std::map<std::string, uintptr_t> m_offsets;
    
    // 钩子
    std::vector<uintptr_t> m_hooks;
};