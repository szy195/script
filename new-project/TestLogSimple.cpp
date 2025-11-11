#include "Utils/Log.h"
#include <iostream>

int main() {
    std::cout << "Testing KBot Log System (Simple)..." << std::endl;
    
    // 初始化日志系统
    Log::LogConfig config;
    config.minLevel = Log::Level::Debug;
    config.consoleOutput = true;
    config.fileOutput = false;  // 禁用文件输出以简化测试
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
    
    // 关闭日志系统
    Log::Logger::GetInstance().Shutdown();
    
    std::cout << "Simple log system test completed." << std::endl;
    return 0;
}