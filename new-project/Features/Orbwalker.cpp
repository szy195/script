#include "Orbwalker.h"
#include "../Core/KBotCore.h"
#include "../Core/MemoryManager.h"
#include "../Core/Offsets.h"
#include "../Game/Minion.h"
#include "../Input/InputManager.h"
#include <algorithm>
#ifdef _WIN32
#include <Windows.h>
#else
#include "../Core/Platform.h"
#endif

namespace KBot::Features {

Orbwalker::Orbwalker() {
    m_gameAPI = &KBotCore::GetInstance().GetGameAPI();
    m_events = &KBotCore::GetInstance().GetEvents();
}

Orbwalker::~Orbwalker() {
    Shutdown();
}

bool Orbwalker::Initialize() {
    if (m_initialized) {
        return true;
    }
    
    // 注册事件监听器
    m_gameUpdateListenerId = m_events->AddListener(EventType::GAME_UPDATE, 
        [this](const std::shared_ptr<EventData>& eventData) {
            OnGameUpdate(eventData);
        });
    
    m_drawListenerId = m_events->AddListener(EventType::DRAW, 
        [this](const std::shared_ptr<EventData>& eventData) {
            OnDraw(eventData);
        });
    
    m_initialized = true;
    return true;
}

void Orbwalker::Shutdown() {
    if (!m_initialized) {
        return;
    }
    
    // 移除事件监听器
    if (!m_gameUpdateListenerId.empty()) {
        m_events->RemoveListener(EventType::GAME_UPDATE, m_gameUpdateListenerId);
        m_gameUpdateListenerId.clear();
    }
    
    if (!m_drawListenerId.empty()) {
        m_events->RemoveListener(EventType::DRAW, m_drawListenerId);
        m_drawListenerId.clear();
    }
    
    m_initialized = false;
}

bool Orbwalker::IsInitialized() const {
    return m_initialized;
}

void Orbwalker::Update() {
    if (!m_enabled || !m_initialized) {
        return;
    }
    
    // 获取本地玩家
    Hero* localPlayer = m_gameAPI->GetLocalPlayer();
    if (!localPlayer || localPlayer->IsDead()) {
        return;
    }
    
    // 检查按键状态，设置模式
    if (InputManager::IsKeyPressed(m_comboKey)) {
        m_mode = OrbwalkerMode::COMBO;
    } else if (InputManager::IsKeyPressed(m_harassKey)) {
        m_mode = OrbwalkerMode::LANE_CLEAR; // 暂时用清线模式代替骚扰
    } else if (InputManager::IsKeyPressed(m_laneClearKey)) {
        m_mode = OrbwalkerMode::LANE_CLEAR;
    } else if (InputManager::IsKeyPressed(m_lastHitKey)) {
        m_mode = OrbwalkerMode::LAST_HIT;
    } else if (InputManager::IsKeyPressed(m_fleeKey)) {
        m_mode = OrbwalkerMode::FLEE;
    } else {
        m_mode = OrbwalkerMode::NONE;
    }
    
    // 如果没有模式，不执行走砍
    if (m_mode == OrbwalkerMode::NONE) {
        return;
    }
    
    // 根据模式执行走砍
    switch (m_mode) {
        case OrbwalkerMode::COMBO:
        case OrbwalkerMode::LANE_CLEAR:
        case OrbwalkerMode::LAST_HIT:
            // 选择目标并执行走砍
            m_currentTarget = SelectTarget();
            Orbwalk(m_currentTarget);
            break;
            
        case OrbwalkerMode::FLEE:
            // 逃跑模式只移动，不攻击
            Orbwalk(nullptr);
            break;
            
        default:
            break;
    }
}

void Orbwalker::SetMode(OrbwalkerMode mode) {
    m_mode = mode;
}

OrbwalkerMode Orbwalker::GetMode() const {
    return m_mode;
}

void Orbwalker::SetTargetSelector(TargetSelector selector) {
    m_targetSelector = selector;
}

TargetSelector Orbwalker::GetTargetSelector() const {
    return m_targetSelector;
}

void Orbwalker::SetEnabled(bool enabled) {
    m_enabled = enabled;
}

bool Orbwalker::IsEnabled() const {
    return m_enabled;
}

void Orbwalker::SetComboKey(int keyCode) {
    m_comboKey = keyCode;
}

void Orbwalker::SetHarassKey(int keyCode) {
    m_harassKey = keyCode;
}

void Orbwalker::SetLaneClearKey(int keyCode) {
    m_laneClearKey = keyCode;
}

void Orbwalker::SetLastHitKey(int keyCode) {
    m_lastHitKey = keyCode;
}

void Orbwalker::SetFleeKey(int keyCode) {
    m_fleeKey = keyCode;
}

bool Orbwalker::CanAttack() const {
    DWORD currentTime = GetTickCount();
    return (m_lastAttackTick + GetAttackDelay()) < currentTime;
}

bool Orbwalker::CanMove() const {
    DWORD currentTime = GetTickCount();
    return (m_lastMoveTick) < currentTime;
}

Hero* Orbwalker::GetTarget() const {
    return m_currentTarget;
}

float Orbwalker::GetAttackDelay() const {
    return 1000.0f / GetAttackSpeed();
}

float Orbwalker::GetWindupTime() const {
    return GetAttackCastTime() * 1000.0f;
}

void Orbwalker::Attack(Hero* target) {
    if (!target || !m_gameAPI) {
        return;
    }
    
    // 执行攻击
    m_gameAPI->AttackUnit(target);
    
    // 更新时间戳
    m_lastAttackTick = GetTickCount();
    m_lastMoveTick = GetTickCount() + GetWindupTime();
}

void Orbwalker::Move(const Vector3& position) {
    if (!m_gameAPI) {
        return;
    }
    
    // 执行移动
    m_gameAPI->MoveTo(position);
    
    // 更新时间戳
    m_lastMoveTick = GetTickCount() + 50; // 添加小延迟防止卡顿
}

void Orbwalker::Orbwalk(Hero* target, const Vector3& position) {
    // 如果有目标且可以攻击，则攻击
    if (target && CanAttack() && IsInRange(target)) {
        Attack(target);
        return;
    }
    
    // 如果可以移动，则移动
    if (CanMove()) {
        Vector3 movePos = position;
        
        // 如果没有指定移动位置，则移动到鼠标位置
        if (movePos.IsZero()) {
            movePos = m_gameAPI->GetMouseWorldPosition();
        }
        
        Move(movePos);
    }
}

void Orbwalker::OnGameUpdate(const std::shared_ptr<EventData>& eventData) {
    Update();
}

void Orbwalker::OnDraw(const std::shared_ptr<EventData>& eventData) {
    // 可以在这里绘制走砍相关的UI
}

Hero* Orbwalker::SelectTarget() const {
    Hero* localPlayer = m_gameAPI->GetLocalPlayer();
    if (!localPlayer) {
        return nullptr;
    }
    
    // 获取所有敌方英雄
    std::vector<Hero*> enemies = m_gameAPI->GetEnemyHeroes();
    if (enemies.empty()) {
        return nullptr;
    }
    
    // 过滤掉死亡或不可见的目标
    std::vector<Hero*> validTargets;
    for (Hero* enemy : enemies) {
        if (enemy && !enemy->IsDead() && enemy->IsVisible() && IsInRange(enemy)) {
            validTargets.push_back(enemy);
        }
    }
    
    if (validTargets.empty()) {
        return nullptr;
    }
    
    // 根据目标选择器选择目标
    Hero* selectedTarget = nullptr;
    float highestPriority = -1.0f;
    
    for (Hero* target : validTargets) {
        float priority = GetTargetPriority(target);
        if (priority > highestPriority) {
            highestPriority = priority;
            selectedTarget = target;
        }
    }
    
    return selectedTarget;
}

float Orbwalker::GetTargetPriority(Hero* target) const {
    if (!target) {
        return -1.0f;
    }
    
    Hero* localPlayer = m_gameAPI->GetLocalPlayer();
    if (!localPlayer) {
        return -1.0f;
    }
    
    float priority = 0.0f;
    
    switch (m_targetSelector) {
        case TargetSelector::LOWEST_HEALTH:
            // 最低血量优先
            priority = 1.0f - (target->GetHealth() / target->GetMaxHealth());
            break;
            
        case TargetSelector::CLOSEST:
            // 最近距离优先
            float distance = localPlayer->GetDistance(target);
            priority = 1.0f - (distance / GetAttackRange());
            break;
            
        case TargetSelector::NEAR_MOUSE:
            // 靠近鼠标优先
            Vector3 mousePos = m_gameAPI->GetMouseWorldPosition();
            float mouseDistance = target->GetDistance(mousePos);
            priority = 1.0f - (mouseDistance / 1000.0f); // 假设1000为最大距离
            break;
            
        case TargetSelector::LESS_ATTACKS:
            // 最少攻击次数（需要额外数据支持）
            priority = 0.5f; // 暂时返回中等优先级
            break;
            
        case TargetSelector::HIGHEST_AD:
            // 最高攻击力优先
            priority = target->GetAttackDamage() / 500.0f; // 假设500为最大攻击力
            break;
            
        default:
            priority = 0.5f;
            break;
    }
    
    return priority;
}

float Orbwalker::GetAttackCastTime() const {
    Hero* localPlayer = m_gameAPI->GetLocalPlayer();
    if (!localPlayer) {
        return 0.3f; // 默认值
    }
    
    // 这里应该从游戏数据中获取攻击起手时间
    // 暂时使用默认值
    return 0.3f;
}

float Orbwalker::GetAttackSpeed() const {
    Hero* localPlayer = m_gameAPI->GetLocalPlayer();
    if (!localPlayer) {
        return 1.0f; // 默认值
    }
    
    return localPlayer->GetAttackSpeed();
}

float Orbwalker::GetAttackRange() const {
    Hero* localPlayer = m_gameAPI->GetLocalPlayer();
    if (!localPlayer) {
        return 500.0f; // 默认值
    }
    
    // 这里应该从游戏数据中获取攻击范围
    // 暂时使用默认值
    return 500.0f;
}

bool Orbwalker::IsInRange(Hero* target) const {
    Hero* localPlayer = m_gameAPI->GetLocalPlayer();
    if (!localPlayer || !target) {
        return false;
    }
    
    float distance = localPlayer->GetDistance(target);
    float attackRange = GetAttackRange();
    
    return distance <= attackRange;
}

} // namespace KBot::Features