#include "KBotCore.h"
#include "../Utils/Log.h"
#include "../Features/Orbwalker.h"
#include "../Input/InputManager.h"
#include <thread>
#include <chrono>

// 全局变量
void* g_hModule = nullptr;
uint32_t g_dwThreadId = 0;
void* g_hThread = nullptr;

// KBotCore实现
KBotCore& KBotCore::GetInstance() {
    static KBotCore instance;
    return instance;
}

bool KBotCore::Initialize() {
    if (m_initialized) {
        return true;
    }
    
    // 初始化日志系统
    Log::LogConfig config;
    config.minLevel = Log::Level::Info;
    config.consoleOutput = true;
    config.fileOutput = true;
    config.logFilePath = "KBot.log";
    Log::Logger::GetInstance().Initialize(config);
    
    Log::Info("Initializing KBot Core v" KBOT_VERSION_STRING);
    
    // 初始化保护机制
    if (!InitializeProtection()) {
        Log::Error("Failed to initialize protection");
        return false;
    }
    
    // 初始化内存管理
    if (!InitializeMemory()) {
        Log::Error("Failed to initialize memory manager");
        return false;
    }
    
    // 初始化游戏API
    if (!InitializeGameAPI()) {
        Log::Error("Failed to initialize game API");
        return false;
    }
    
    // 初始化渲染器
    if (!InitializeRenderer()) {
        Log::Error("Failed to initialize renderer");
        return false;
    }
    
    // 初始化功能模块
    if (!InitializeModules()) {
        Log::Error("Failed to initialize modules");
        return false;
    }
    
    // 启动主线程
    StartMainThread();
    
    m_initialized = true;
    Log::Info("KBot Core initialized successfully");
    return true;
}

void KBotCore::Shutdown() {
    if (!m_initialized) {
        return;
    }
    
    Log::Info("Shutting down KBot Core");
    
    // 停止主线程
    StopMainThread();
    
    // 清理功能模块
    ShutdownModules();
    
    // 清理渲染器
    ShutdownRenderer();
    
    // 清理游戏API
    ShutdownGameAPI();
    
    // 清理内存管理
    ShutdownMemory();
    
    // 清理保护机制
    ShutdownProtection();
    
    m_initialized = false;
    Log::Info("KBot Core shutdown complete");
    
    // 关闭日志系统
    Log::Logger::GetInstance().Shutdown();
}

bool KBotCore::IsInitialized() const {
    return m_initialized;
}

MemoryManager* KBotCore::GetMemoryManager() const {
    return m_memoryManager.get();
}

GameAPI* KBotCore::GetGameAPI() const {
    return m_gameAPI.get();
}

Events* KBotCore::GetEvents() const {
    return m_events.get();
}

Renderer* KBotCore::GetRenderer() const {
    return m_renderer.get();
}

Config* KBotCore::GetConfig() const {
    return m_config.get();
}

Protection* KBotCore::GetProtection() const {
    return m_protection.get();
}

bool KBotCore::IsGameActive() const {
    return m_gameActive;
}

void KBotCore::SetGameActive(bool active) {
    if (m_gameActive != active) {
        m_gameActive = active;
        if (active) {
            Log::Info("Game is now active");
            GetEvents()->FireEvent("GameActive");
        } else {
            Log::Info("Game is now inactive");
            GetEvents()->FireEvent("GameInactive");
        }
    }
}

void KBotCore::StartMainThread() {
    if (m_mainThreadRunning) {
        return;
    }
    
    m_mainThreadRunning = true;
    
    // 使用std::thread替代Windows API
    std::thread threadObj(MainThread, this);
    threadObj.detach();
    m_mainThread = reinterpret_cast<void*>(threadObj.native_handle());
    
    Log::Info("Main thread started");
}

void KBotCore::StopMainThread() {
    if (!m_mainThreadRunning) {
        return;
    }
    
    m_mainThreadRunning = false;
    
    // 等待线程结束
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    m_mainThread = nullptr;
    
    Log::Info("Main thread stopped");
}

bool KBotCore::InitializeProtection() {
    m_protection = std::make_unique<Protection>();
    if (!m_protection->Initialize()) {
        return false;
    }
    return true;
}

bool KBotCore::InitializeMemory() {
    m_memoryManager = std::make_unique<MemoryManager>();
    if (!m_memoryManager->Initialize()) {
        return false;
    }
    return true;
}

bool KBotCore::InitializeGameAPI() {
    m_gameAPI = std::make_unique<GameAPI>();
    m_events = std::make_unique<Events>();
    
    if (!m_gameAPI->Initialize()) {
        return false;
    }
    
    if (!m_events->Initialize()) {
        return false;
    }
    
    return true;
}

bool KBotCore::InitializeRenderer() {
    m_renderer = std::make_unique<Renderer>();
    if (!m_renderer->Initialize()) {
        return false;
    }
    return true;
}

bool KBotCore::InitializeModules() {
    try {
        // 初始化配置
        m_config = std::make_unique<Config>();
        if (!m_config->Initialize()) {
            return false;
        }

        // 初始化输入管理器
        InputManager::GetInstance().Initialize();

        // 初始化走砍功能
        auto orbwalker = std::make_shared<Features::Orbwalker>();
        if (!orbwalker->Initialize()) {
            return false;
        }
        RegisterModule("Orbwalker", orbwalker);

        // TODO: 初始化其他功能模块（暂时注释掉，专注走砍功能）
        /*
        // 初始化目标选择器
        auto targetSelector = std::make_shared<Features::TargetSelector>();
        if (!targetSelector->Initialize()) {
            return false;
        }
        RegisterModule("TargetSelector", targetSelector);

        // 初始化技能CD计时器
        auto spellTracker = std::make_shared<Features::SpellTracker>();
        if (!spellTracker->Initialize()) {
            return false;
        }
        RegisterModule("SpellTracker", spellTracker);

        // 初始化躲避系统
        auto evade = std::make_shared<Features::Evade>();
        if (!evade->Initialize()) {
            return false;
        }
        RegisterModule("Evade", evade);

        // 初始化活化剂使用
        auto potionTracker = std::make_shared<Features::PotionTracker>();
        if (!potionTracker->Initialize()) {
            return false;
        }
        RegisterModule("PotionTracker", potionTracker);

        // 初始化英雄连招
        auto championScripts = std::make_shared<Features::ChampionScripts>();
        if (!championScripts->Initialize()) {
            return false;
        }
        RegisterModule("ChampionScripts", championScripts);
        */

        return true;
    }
    catch (const std::exception& e) {
        // 记录错误
        return false;
    }
}

void KBotCore::ShutdownModules() {
    // 清理输入管理器
    InputManager::GetInstance().Shutdown();
    
    m_config.reset();
    m_modules.clear();
}

void KBotCore::ShutdownRenderer() {
    if (m_renderer) {
        m_renderer->Shutdown();
        m_renderer.reset();
    }
}

void KBotCore::ShutdownGameAPI() {
    if (m_events) {
        m_events->Shutdown();
        m_events.reset();
    }
    
    if (m_gameAPI) {
        m_gameAPI->Shutdown();
        m_gameAPI.reset();
    }
}

void KBotCore::ShutdownMemory() {
    if (m_memoryManager) {
        m_memoryManager->Shutdown();
        m_memoryManager.reset();
    }
}

void KBotCore::ShutdownProtection() {
    if (m_protection) {
        m_protection->Shutdown();
        m_protection.reset();
    }
}

// 主线程函数
void* MainThread(void* param) {
    KBotCore* core = static_cast<KBotCore*>(param);
    
    Log::Info("Main thread started");
    
    while (core->m_mainThreadRunning) {
        try {
            // 更新输入管理器
            InputManager::GetInstance().Update();
            
            // 检查游戏状态
            bool gameActive = core->GetGameAPI()->IsGameActive();
            core->SetGameActive(gameActive);
            
            if (gameActive) {
                // 更新游戏对象
                core->GetGameAPI()->UpdateObjects();
                
                // 触发游戏更新事件
                core->GetEvents()->FireEvent("GameUpdate");
                
                // 渲染
                core->GetRenderer()->Render();
            }
            
            // 短暂休眠以减少CPU使用率
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        catch (const std::exception& e) {
            Log::Error("Exception in main thread: %s", e.what());
        }
    }
    
    Log::Info("Main thread exiting");
    return nullptr;
}

// 初始化和清理函数
void InitializeLibrary() {
    Log::Info("KBot library initializing");
}

void ShutdownLibrary() {
    Log::Info("KBot library shutting down");
    if (KBotCore::GetInstance().IsInitialized()) {
        KBotCore::GetInstance().Shutdown();
    }
}

// 公共API函数实现
const char* GetKBotVersion() {
    return KBOT_VERSION_STRING;
}

bool InitializeKBot() {
    return KBotCore::GetInstance().Initialize();
}

void ShutdownKBot() {
    KBotCore::GetInstance().Shutdown();
}

bool IsKBotInitialized() {
    return KBotCore::GetInstance().IsInitialized();
}

bool LoadConfig(const char* configPath) {
    return KBotCore::GetInstance().GetConfig()->Load(configPath);
}

bool SaveConfig(const char* configPath) {
    return KBotCore::GetInstance().GetConfig()->Save(configPath);
}
    
    void SetConfigValue(const char* key, const char* value) {
    KBotCore::GetInstance().GetConfig()->SetValue(key, value);
}

const char* GetConfigValue(const char* key) {
    return KBotCore::GetInstance().GetConfig()->GetValue(key);
}

void EnableFeature(const char* featureName, bool enable) {
    // TODO: 实现功能控制
}

bool IsFeatureEnabled(const char* featureName) {
    // TODO: 实现功能状态查询
    return false;
}

bool IsGameActive() {
    return KBotCore::GetInstance().IsGameActive();
}

const char* GetStatus() {
    static std::string status;
    if (KBotCore::GetInstance().IsInitialized()) {
        if (KBotCore::GetInstance().IsGameActive()) {
            status = "Running";
        } else {
            status = "Waiting for game";
        }
    } else {
        status = "Not initialized";
    }
    return status.c_str();
}