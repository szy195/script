#include "Events.h"
#include "../Utils/Log.h"
#include <algorithm>
#include <iostream>

// 构造函数
Events::Events() : m_initialized(false) {
    // 初始化事件队列
    m_eventQueue.reserve(1000);
}

// 析构函数
Events::~Events() {
    Shutdown();
}

// 初始化
bool Events::Initialize() {
    if (m_initialized) {
        return true;
    }
    
    m_initialized = true;
    return true;
}

// 关闭
void Events::Shutdown() {
    // 清空事件队列
    std::lock_guard<std::mutex> lock(m_mutex);
    m_eventQueue.clear();
    
    // 清空监听器
    m_listeners.clear();
    
    // 清空过滤器
    m_filters.clear();
    
    // 重置统计
    m_eventCounts.clear();
    m_totalEventCount = 0;
    
    // 清空日志
    m_eventLog.clear();
    
    m_initialized = false;
}

// 是否已初始化
bool Events::IsInitialized() const {
    return m_initialized;
}

// 添加事件监听器
std::string Events::AddListener(const std::string& eventType, EventCallback callback, bool persistent) {
    if (!m_initialized || !callback) {
        return "";
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // 生成唯一ID
    std::string id = GenerateListenerId();
    
    // 创建新监听器
    auto listener = std::make_shared<EventListener>(id, eventType, callback, persistent);
    
    // 添加到列表
    m_listeners[eventType].push_back(listener);
    
    return id;
}

// 移除事件监听器
bool Events::RemoveListener(const std::string& listenerId) {
    if (!m_initialized) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    for (auto& pair : m_listeners) {
        auto& listeners = pair.second;
        auto it = std::find_if(listeners.begin(), listeners.end(),
            [&listenerId](const std::shared_ptr<EventListener>& l) { return l->id == listenerId; });
        
        if (it != listeners.end()) {
            listeners.erase(it);
            return true;
        }
    }
    
    return false;
}

// 移除指定类型的所有监听器
bool Events::RemoveListeners(const std::string& eventType) {
    if (!m_initialized) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto it = m_listeners.find(eventType);
    if (it != m_listeners.end()) {
        m_listeners.erase(it);
        return true;
    }
    
    return false;
}

// 清空所有监听器
void Events::ClearAllListeners() {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_listeners.clear();
}

// 触发事件
void Events::FireEvent(const std::shared_ptr<EventData>& eventData) {
    if (!m_initialized || !eventData) {
        return;
    }
    
    // 记录事件日志
    LogEvent(eventData);
    
    // 更新统计
    std::string eventType = eventData->GetEventType();
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_eventCounts[eventType]++;
        m_totalEventCount++;
    }
    
    // 检查过滤器
    if (!ShouldProcessEvent(eventData)) {
        return;
    }
    
    // 处理事件
    ProcessEvent(eventData);
}

// 触发事件（仅类型）
void Events::FireEvent(const std::string& eventType) {
    FireEvent(eventType, "");
}

// 触发事件（类型和数据）
void Events::FireEvent(const std::string& eventType, const std::string& data) {
    // 记录事件日志
    Log::Debug("Firing event: %s", eventType.c_str());
    
    // 更新统计
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_eventCounts[eventType]++;
        m_totalEventCount++;
    }
    
    // 记录事件数据
    if (!data.empty()) {
        Log::Debug("Event data: %s", data.c_str());
    }
    
    // 由于没有具体的事件数据，这里只记录日志
    Log::Debug("Event fired: %s", eventType.c_str());
}

// 处理事件队列
void Events::ProcessEventQueue() {
    if (!m_initialized) {
        return;
    }
    
    // 复制事件队列以避免长时间锁定
    std::vector<std::shared_ptr<EventData>> eventsToProcess;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        eventsToProcess = m_eventQueue;
        m_eventQueue.clear();
    }
    
    // 处理事件
    for (const auto& event : eventsToProcess) {
        FireEvent(event);
    }
}

// 清空事件队列
void Events::ClearEventQueue() {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_eventQueue.clear();
}

// 获取队列中的事件数量
size_t Events::GetEventQueueSize() const {
    if (!m_initialized) {
        return 0;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_eventQueue.size();
}

// 设置事件过滤器
void Events::SetEventFilter(const std::string& eventType, std::function<bool(const std::shared_ptr<EventData>&)> filter) {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_filters[eventType] = filter;
}

// 移除事件过滤器
void Events::RemoveEventFilter(const std::string& eventType) {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_filters.erase(eventType);
}

// 清空所有事件过滤器
void Events::ClearEventFilters() {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_filters.clear();
}

// 获取指定类型的监听器数量
size_t Events::GetListenerCount(const std::string& eventType) const {
    if (!m_initialized) {
        return 0;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto it = m_listeners.find(eventType);
    if (it != m_listeners.end()) {
        return it->second.size();
    }
    
    return 0;
}

// 获取总监听器数量
size_t Events::GetTotalListenerCount() const {
    if (!m_initialized) {
        return 0;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    size_t count = 0;
    for (const auto& pair : m_listeners) {
        count += pair.second.size();
    }
    
    return count;
}

// 获取指定类型的事件数量
size_t Events::GetEventCount(const std::string& eventType) const {
    if (!m_initialized) {
        return 0;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto it = m_eventCounts.find(eventType);
    if (it != m_eventCounts.end()) {
        return it->second;
    }
    
    return 0;
}

// 获取总事件数量
size_t Events::GetTotalEventCount() const {
    if (!m_initialized) {
        return 0;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_totalEventCount;
}

// 重置事件计数器
void Events::ResetEventCounters() {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_eventCounts.clear();
    m_totalEventCount = 0;
}

// 启用事件日志
void Events::EnableEventLogging(bool enable) {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_eventLoggingEnabled = enable;
    
    if (enable) {
        Log::Info("%s", "Event logging enabled");
    } else {
        Log::Info("%s", "Event logging disabled");
    }
}

// 检查事件日志是否启用
bool Events::IsEventLoggingEnabled() const {
    if (!m_initialized) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    return m_eventLoggingEnabled;
}

// 获取事件日志
std::vector<std::string> Events::GetEventLog(size_t maxEntries) const {
    if (!m_initialized) {
        return {};
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    if (maxEntries >= m_eventLog.size()) {
        return m_eventLog;
    }
    
    // 返回最新的条目
    return std::vector<std::string>(m_eventLog.end() - maxEntries, m_eventLog.end());
}

// 清空事件日志
void Events::ClearEventLog() {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_eventLog.clear();
    Log::Info("%s", "Event log cleared");
}

// 打印事件信息
void Events::PrintEventInfo() const {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    Log::Info("%s", "=== Event System Info ===");
    Log::Info("Total events processed: %zu", m_totalEventCount);
    Log::Info("Event queue size: %zu", m_eventQueue.size());
    Log::Info("Active listeners: %zu", GetTotalListenerCount());
    
    Log::Info("%s", "Event counts by type:");
    for (const auto& pair : m_eventCounts) {
        Log::Info("  %s: %zu", pair.first.c_str(), pair.second);
    }
}

// 打印监听器信息
void Events::PrintListenerInfo() const {
    if (!m_initialized) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    Log::Info("%s", "=== Event Listeners Info ===");
    for (const auto& pair : m_listeners) {
        Log::Info("Event type: %s, Listeners: %zu", pair.first.c_str(), pair.second.size());
    }
}

// 内部方法：处理事件
void Events::ProcessEvent(const std::shared_ptr<EventData>& eventData) {
    std::string eventType = eventData->GetEventType();
    
    // 复制监听器列表以避免在回调中修改列表
    std::vector<std::shared_ptr<EventListener>> listeners;
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        auto it = m_listeners.find(eventType);
        if (it != m_listeners.end()) {
            listeners = it->second;
        }
    }
    
    // 释放锁后再调用回调
    for (const auto& listener : listeners) {
        try {
            listener->callback(eventData);
        } catch (const std::exception& e) {
            Log::Error("Exception in event listener '%s': %s", listener->id.c_str(), e.what());
        }
    }
}

// 内部方法：检查是否应该处理事件
bool Events::ShouldProcessEvent(const std::shared_ptr<EventData>& eventData) {
    std::string eventType = eventData->GetEventType();
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto it = m_filters.find(eventType);
    if (it != m_filters.end()) {
        return it->second(eventData);
    }
    
    return true;
}

// 内部方法：记录事件日志
void Events::LogEvent(const std::shared_ptr<EventData>& eventData) {
    if (!m_eventLoggingEnabled) {
        return;
    }
    
    std::string eventType = eventData->GetEventType();
    
    // 获取当前时间
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    // 格式化时间
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    
    // 创建日志条目
    std::string logEntry = ss.str() + " [" + eventType + "] Event fired";
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    // 添加到日志
    m_eventLog.push_back(logEntry);
    
    // 限制日志条目数量
    if (m_eventLog.size() > m_maxLogEntries) {
        m_eventLog.erase(m_eventLog.begin());
    }
    
    // 记录到日志系统
    Log::Debug("Event: %s", eventType.c_str());
}

// 内部方法：生成监听器ID
std::string Events::GenerateListenerId() {
    std::stringstream ss;
    ss << "listener_" << m_nextListenerId++;
    return ss.str();
}