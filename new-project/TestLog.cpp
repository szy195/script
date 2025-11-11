#include "Utils/Log.h"
#include "Core/Events.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "Testing KBot Log System..." << std::endl;
    
    // 初始化日志系统
    Log::LogConfig config;
    config.minLevel = Log::Level::Debug;
    config.consoleOutput = true;
    config.fileOutput = true;
    config.logFilePath = "test_log.txt";
    Log::Logger::GetInstance().Initialize(config);
    
    // 测试基本日志功能
    Log::Trace("%s", "This is a trace message");
    Log::Debug("%s", "This is a debug message");
    Log::Info("%s", "This is an info message");
    Log::Warning("%s", "This is a warning message");
    Log::Error("%s", "This is an error message");
    Log::Critical("%s", "This is a critical message");
    
    // 测试格式化日志
    Log::Info("Formatted message: %d + %d = %d", 10, 20, 30);
    Log::Error("Error code: %d, Message: %s", 404, "Not Found");
    
    // 测试事件系统
    Events events;
    events.Initialize();
    
    // 启用事件日志
    events.EnableEventLogging(true);
    
    // 创建并触发一些测试事件
    auto gameUpdateEvent = std::make_shared<GameUpdateEvent>();
    gameUpdateEvent->gameTime = 12345.67f;
    gameUpdateEvent->gameTick = 100;
    events.FireEvent(gameUpdateEvent);
    
    auto objectCreateEvent = std::make_shared<ObjectCreateEvent>();
    objectCreateEvent->object = (void*)1001;
    objectCreateEvent->objectType = "TestObject";
    events.FireEvent(objectCreateEvent);
    
    auto spellCastEvent = std::make_shared<SpellCastEvent>();
    spellCastEvent->caster = (void*)1001;
    spellCastEvent->spellSlot = 1;
    spellCastEvent->spellName = "TestSpell";
    spellCastEvent->startPosition = Vector3(0, 0, 0);
    spellCastEvent->endPosition = Vector3(100, 0, 0);
    events.FireEvent(spellCastEvent);
    
    // 处理事件队列
    events.ProcessEventQueue();
    
    // 获取事件日志
    auto eventLog = events.GetEventLog();
    std::cout << "Event log entries: " << eventLog.size() << std::endl;
    for (const auto& entry : eventLog) {
        std::cout << entry << std::endl;
    }
    
    // 测试日志级别作用域
    {
        Log::ScopedLogLevel scope(Log::Level::Warning);
        Log::Trace("%s", "This trace message should not appear");
        Log::Debug("%s", "This debug message should not appear");
        Log::Info("%s", "This info message should not appear");
        Log::Warning("%s", "This warning message should appear");
        Log::Error("%s", "This error message should appear");
    }
    
    // 恢复日志级别
    Log::Info("%s", "This message should appear again");
    
    // 关闭事件系统
    events.Shutdown();
    
    // 关闭日志系统
    Log::Logger::GetInstance().Shutdown();
    
    std::cout << "Log system test completed. Check test_log.txt for file output." << std::endl;
    return 0;
}