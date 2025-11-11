#pragma once

// STL
#include <memory>
#include <string>
#include <vector>
#include <map>

// 项目头文件
#include "../Utils/Config.h"
#include "../Utils/Log.h"
#include "../Core/MemoryManager.h"
#include "../Core/GameAPI.h"
#include "../Core/Events.h"
#include "../Renderer/Renderer.h"
#include "../Security/Protection.h"
#include "../Input/InputManager.h"

// 版本信息
#define KBOT_VERSION_MAJOR 1
#define KBOT_VERSION_MINOR 0
#define KBOT_VERSION_PATCH 0
#define KBOT_VERSION_STRING "1.0.0"

// 导出函数
#define KBOT_API

// 全局变量
extern void* g_hModule;
extern uint32_t g_dwThreadId;
extern void* g_hThread;

// 核心管理器类
class KBotCore {
public:
    // 单例模式
    static KBotCore& GetInstance();
    
    // 初始化和清理
    bool Initialize();
    void Shutdown();
    bool IsInitialized() const;
    
    // 模块访问
    class MemoryManager* GetMemoryManager() const;
    class GameAPI* GetGameAPI() const;
    class Events* GetEvents() const;
    class Renderer* GetRenderer() const;
    class Config* GetConfig() const;
    class Protection* GetProtection() const;
    
    // 游戏状态
    bool IsGameActive() const;
    void SetGameActive(bool active);
    
    // 功能模块管理
    template<typename T>
    void RegisterModule(const std::string& name);
    
    template<typename T>
    T* GetModule(const std::string& name);
    
    void UnregisterModule(const std::string& name);
    
    // 线程管理
    void StartMainThread();
    void StopMainThread();
    
private:
    KBotCore() = default;
    ~KBotCore() = default;
    KBotCore(const KBotCore&) = delete;
    KBotCore& operator=(const KBotCore&) = delete;
    
    // 初始化步骤
    bool InitializeProtection();
    bool InitializeMemory();
    bool InitializeGameAPI();
    bool InitializeRenderer();
    bool InitializeModules();
    
    // 清理步骤
    void ShutdownModules();
    void ShutdownRenderer();
    void ShutdownGameAPI();
    void ShutdownMemory();
    void ShutdownProtection();
    
    // 成员变量
    bool m_initialized = false;
    bool m_gameActive = false;
    
    // 核心模块
    std::unique_ptr<class MemoryManager> m_memoryManager;
    std::unique_ptr<class GameAPI> m_gameAPI;
    std::unique_ptr<class Events> m_events;
    std::unique_ptr<class Renderer> m_renderer;
    std::unique_ptr<class Config> m_config;
    std::unique_ptr<class Protection> m_protection;
    
    // 功能模块
    std::map<std::string, std::unique_ptr<void>> m_modules;
    
    // 线程管理
    void* m_mainThread = nullptr;
    uint32_t m_mainThreadId = 0;
    bool m_mainThreadRunning = false;
};

// 主线程函数
void* MainThread(void* param);

// 导出函数
extern "C" {
    // 获取版本信息
    KBOT_API const char* GetKBotVersion();
    
    // 初始化和清理
    KBOT_API bool InitializeKBot();
    KBOT_API void ShutdownKBot();
    KBOT_API bool IsKBotInitialized();
    
    // 配置管理
    KBOT_API bool LoadConfig(const char* configPath);
    KBOT_API bool SaveConfig(const char* configPath);
    KBOT_API void SetConfigValue(const char* key, const char* value);
    KBOT_API const char* GetConfigValue(const char* key);
    
    // 功能控制
    KBOT_API void EnableFeature(const char* featureName, bool enable);
    KBOT_API bool IsFeatureEnabled(const char* featureName);
    
    // 状态查询
    KBOT_API bool IsGameActive();
    KBOT_API const char* GetStatus();
}