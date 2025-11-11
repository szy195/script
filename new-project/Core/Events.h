#pragma once

#include <functional>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include "../Utils/Vector.h"

// 事件数据基类
class EventData {
public:
    virtual ~EventData() = default;
    virtual std::string GetEventType() const = 0;
};

// 游戏更新事件
class GameUpdateEvent : public EventData {
public:
    std::string GetEventType() const override { return "GameUpdate"; }
    float gameTime;
    int gameTick;
};

// 游戏对象创建事件
class ObjectCreateEvent : public EventData {
public:
    std::string GetEventType() const override { return "ObjectCreate"; }
    void* object;
    std::string objectType;
};

// 游戏对象销毁事件
class ObjectDestroyEvent : public EventData {
public:
    std::string GetEventType() const override { return "ObjectDestroy"; }
    void* object;
    std::string objectType;
};

// 技能施放事件
class SpellCastEvent : public EventData {
public:
    std::string GetEventType() const override { return "SpellCast"; }
    void* caster;
    int spellSlot;
    std::string spellName;
    Vector3 startPosition;
    Vector3 endPosition;
};

// 技能弹道事件
class MissileCreateEvent : public EventData {
public:
    std::string GetEventType() const override { return "MissileCreate"; }
    void* missile;
    void* caster;
    void* target;
    Vector3 startPosition;
    Vector3 currentPosition;
    Vector3 endPosition;
    float speed;
};

// 游戏状态变化事件
class GameStateChangeEvent : public EventData {
public:
    std::string GetEventType() const override { return "GameStateChange"; }
    std::string oldState;
    std::string newState;
};

// 按键事件
class KeyEvent : public EventData {
public:
    std::string GetEventType() const override { return "Key"; }
    int keyCode;
    bool isPressed;
    bool isRepeat;
};

// 鼠标事件
class MouseEvent : public EventData {
public:
    std::string GetEventType() const override { return "Mouse"; }
    int x;
    int y;
    int button;
    bool isPressed;
    bool isDoubleClick;
};

// 绘制事件
class DrawEvent : public EventData {
public:
    std::string GetEventType() const override { return "Draw"; }
    void* renderer;
};

// 事件回调函数类型
using EventCallback = std::function<void(const std::shared_ptr<EventData>&)>;

// 事件监听器
struct EventListener {
    std::string id;
    std::string eventType;
    EventCallback callback;
    bool persistent;  // 是否持久监听，false表示触发一次后自动移除
    
    EventListener(const std::string& id, const std::string& type, EventCallback cb, bool persist = true)
        : id(id), eventType(type), callback(cb), persistent(persist) {}
};

// 事件系统类
class Events {
public:
    Events();
    ~Events();
    
    // 初始化和清理
    bool Initialize();
    void Shutdown();
    bool IsInitialized() const;
    
    // 事件监听
    std::string AddListener(const std::string& eventType, EventCallback callback, bool persistent = true);
    bool RemoveListener(const std::string& listenerId);
    bool RemoveListeners(const std::string& eventType);
    void ClearAllListeners();
    
    // 事件触发
    void FireEvent(const std::shared_ptr<EventData>& eventData);
    void FireEvent(const std::string& eventType);
    void FireEvent(const std::string& eventType, const std::string& data = "");
    
    // 事件队列
    void ProcessEventQueue();
    void ClearEventQueue();
    size_t GetEventQueueSize() const;
    
    // 事件过滤
    void SetEventFilter(const std::string& eventType, std::function<bool(const std::shared_ptr<EventData>&)> filter);
    void RemoveEventFilter(const std::string& eventType);
    void ClearEventFilters();
    
    // 事件统计
    size_t GetListenerCount(const std::string& eventType) const;
    size_t GetTotalListenerCount() const;
    size_t GetEventCount(const std::string& eventType) const;
    size_t GetTotalEventCount() const;
    void ResetEventCounters();
    
    // 事件日志
    void EnableEventLogging(bool enable);
    bool IsEventLoggingEnabled() const;
    std::vector<std::string> GetEventLog(size_t maxEntries = 100) const;
    void ClearEventLog();
    
    // 调试
    void PrintEventInfo() const;
    void PrintListenerInfo() const;
    
private:
    // 内部方法
    void ProcessEvent(const std::shared_ptr<EventData>& eventData);
    bool ShouldProcessEvent(const std::shared_ptr<EventData>& eventData);
    void LogEvent(const std::shared_ptr<EventData>& eventData);
    std::string GenerateListenerId();
    
    // 成员变量
    bool m_initialized = false;
    mutable std::mutex m_mutex;
    
    // 事件监听器
    std::map<std::string, std::vector<std::shared_ptr<EventListener>>> m_listeners;
    
    // 事件队列
    std::vector<std::shared_ptr<EventData>> m_eventQueue;
    
    // 事件过滤器
    std::map<std::string, std::function<bool(const std::shared_ptr<EventData>&)>> m_filters;
    
    // 统计
    std::map<std::string, size_t> m_eventCounts;
    size_t m_totalEventCount = 0;
    
    // 日志
    bool m_eventLoggingEnabled = false;
    std::vector<std::string> m_eventLog;
    size_t m_maxLogEntries = 1000;
    
    // ID生成
    size_t m_nextListenerId = 0;
};

// 事件管理器单例
class EventManager {
public:
    static Events& GetInstance() {
        static Events instance;
        return instance;
    }
    
private:
    EventManager() = default;
    ~EventManager() = default;
    EventManager(const EventManager&) = delete;
    EventManager& operator=(const EventManager&) = delete;
};

// 便捷宏
#define ADD_LISTENER(eventType, callback) \
    Events::GetInstance().AddListener(eventType, callback)

#define REMOVE_LISTENER(listenerId) \
    Events::GetInstance().RemoveListener(listenerId)

#define FIRE_EVENT(eventData) \
    Events::GetInstance().FireEvent(eventData)

#define FIRE_EVENT_TYPE(eventType) \
    Events::GetInstance().FireEvent(eventType)