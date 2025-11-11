#pragma once

#include "../Core/GameAPI.h"
#include "../Game/Hero.h"
#include "../Game/GameObject.h"
#include "../Utils/Vector.h"
#include "../Core/Events.h"
#include <memory>
#include <vector>
#include <chrono>
#include <cstdint>

namespace KBot::Features {

// 走砍模式枚举
enum class OrbwalkerMode {
    NONE = 0,
    LAST_HIT = 1,  // 补刀模式
    LANE_CLEAR = 2, // 清线模式
    COMBO = 3,     // 连招模式
    FLEE = 4       // 逃跑模式
};

// 目标选择器枚举
enum class TargetSelector {
    LESS_ATTACKS = 0,  // 最少攻击次数
    LOWEST_HEALTH = 1,  // 最低血量
    HIGHEST_AD = 2,    // 最高攻击力
    CLOSEST = 3,       // 最近距离
    NEAR_MOUSE = 4     // 靠近鼠标
};

// 走砍功能类
class Orbwalker {
public:
    Orbwalker();
    ~Orbwalker();
    
    // 初始化和清理
    bool Initialize();
    void Shutdown();
    bool IsInitialized() const;
    
    // 更新
    void Update();
    
    // 模式控制
    void SetMode(OrbwalkerMode mode);
    OrbwalkerMode GetMode() const;
    
    // 目标选择器控制
    void SetTargetSelector(TargetSelector selector);
    TargetSelector GetTargetSelector() const;
    
    // 启用/禁用
    void SetEnabled(bool enabled);
    bool IsEnabled() const;
    
    // 按键设置
    void SetComboKey(int keyCode);
    void SetHarassKey(int keyCode);
    void SetLaneClearKey(int keyCode);
    void SetLastHitKey(int keyCode);
    void SetFleeKey(int keyCode);
    
    // 状态查询
    bool CanAttack() const;
    bool CanMove() const;
    Hero* GetTarget() const;
    float GetAttackDelay() const;
    float GetWindupTime() const;
    
    // 动作执行
    void Attack(Hero* target);
    void Move(const Vector3& position);
    void Orbwalk(Hero* target = nullptr, const Vector3& position = Vector3());
    
private:
    // 事件处理
    void OnGameUpdate(const std::shared_ptr<EventData>& eventData);
    void OnDraw(const std::shared_ptr<EventData>& eventData);
    
    // 目标选择
    Hero* SelectTarget() const;
    float GetTargetPriority(Hero* target) const;
    
    // 计算方法
    float GetAttackCastTime() const;
    float GetAttackSpeed() const;
    float GetAttackRange() const;
    bool IsInRange(Hero* target) const;
    
    // 成员变量
    bool m_initialized = false;
    bool m_enabled = true;
    
    // 模式和设置
    OrbwalkerMode m_mode = OrbwalkerMode::NONE;
    TargetSelector m_targetSelector = TargetSelector::LOWEST_HEALTH;
    
    // 按键设置
    int m_comboKey = 32;     // 空格键
    int m_harassKey = 67;    // C键
    int m_laneClearKey = 86; // V键
    int m_lastHitKey = 88;   // X键
    int m_fleeKey = 90;      // Z键
    
    // 时间记录
    uint32_t m_lastAttackTick = 0;
    uint32_t m_lastMoveTick = 0;
    float m_lastAttackCastTime = 0.0f;
    
    // 目标
    Hero* m_currentTarget = nullptr;
    
    // 游戏API
    GameAPI* m_gameAPI = nullptr;
    Events* m_events = nullptr;
    
    // 事件监听器ID
    std::string m_gameUpdateListenerId;
    std::string m_drawListenerId;
};

} // namespace KBot::Features