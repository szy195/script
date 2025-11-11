#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <sstream>
#include <iostream>
#include <chrono>
#include <iomanip>
#include "../Core/Platform.h"

namespace Log {
    // 日志级别枚举
    enum class Level {
        Trace = 0,
        Debug = 1,
        Info = 2,
        Warning = 3,
        Error = 4,
        Critical = 5
    };

    // 日志配置结构
    struct LogConfig {
        Level minLevel = Level::Info;
        bool consoleOutput = true;
        bool fileOutput = true;
        std::string logFilePath = "KBot.log";
        size_t maxFileSize = 10 * 1024 * 1024; // 10MB
        size_t maxBackupFiles = 5;
        bool timestampEnabled = true;
        bool threadIdEnabled = false;
    };

    // 日志器类
    class Logger {
    public:
        // 辅助函数，用于将各种类型转换为C字符串
        template<typename T>
        static auto ToString(T&& value) -> decltype(std::to_string(value)) {
            return std::to_string(value);
        }
        
        // 对于字符串类型，直接返回
        static const char* ToString(const std::string& str) {
            return str.c_str();
        }
        
        // 对于C字符串，直接返回
        static const char* ToString(const char* str) {
            return str;
        }
        
        // 对于字符，返回其地址
        static const char* ToString(char ch) {
            static char buffer[2] = {0};
            buffer[0] = ch;
            return buffer;
        }
        
        static Logger& GetInstance();
        
        // 初始化日志系统
        bool Initialize(const LogConfig& config = LogConfig{});
        
        // 关闭日志系统
        void Shutdown();
        
        // 设置日志级别
        void SetMinLevel(Level level);
        
        // 获取当前日志级别
        Level GetMinLevel() const;
        
        // 记录日志
        void Log(Level level, const char* format, ...);
        void Log(Level level, const std::string& message);
        
        // 格式化日志记录
        template<typename... Args>
        void Trace(const char* format, Args... args);
        template<typename... Args>
        void Debug(const char* format, Args... args);
        template<typename... Args>
        void Info(const char* format, Args... args);
        template<typename... Args>
        void Warning(const char* format, Args... args);
        template<typename... Args>
        void Error(const char* format, Args... args);
        template<typename... Args>
        void Critical(const char* format, Args... args);
        
        // 获取日志配置
        const LogConfig& GetConfig() const;
        
        // 设置日志配置
        void SetConfig(const LogConfig& config);
        
        // 刷新日志缓冲区
        void Flush();
        
        // 获取日志内容
        std::vector<std::string> GetRecentLogs(size_t count = 100) const;
        
        // 清除日志
        void ClearLogs();

    private:
        Logger() = default;
        ~Logger();
        
        // 禁止拷贝和赋值
        Logger(const Logger&) = delete;
        Logger& operator=(const Logger&) = delete;
        
        // 内部实现方法
        void WriteToFile(Level level, const std::string& message);
        void WriteToConsole(Level level, const std::string& message);
        std::string FormatMessage(Level level, const std::string& message);
        std::string LevelToString(Level level) const;
        std::string GetCurrentTimestamp() const;
        void CheckFileRotation();
        std::string FormatVarArgs(const char* format, va_list args);
        
        // 成员变量
        LogConfig m_config;
        std::ofstream m_logFile;
        mutable std::mutex m_mutex;
        std::vector<std::string> m_recentLogs;
        size_t m_maxRecentLogs = 1000;
        bool m_initialized = false;
    };

    // 便捷的全局日志函数
    template<typename... Args>
    inline void Trace(const char* format, Args... args) {
        Logger::GetInstance().Trace(format, Logger::ToString(args)...);
    }
    
    template<typename... Args>
    inline void Debug(const char* format, Args... args) {
        Logger::GetInstance().Debug(format, Logger::ToString(args)...);
    }
    
    template<typename... Args>
    inline void Info(const char* format, Args... args) {
        Logger::GetInstance().Info(format, Logger::ToString(args)...);
    }
    
    template<typename... Args>
    inline void Warning(const char* format, Args... args) {
        Logger::GetInstance().Warning(format, Logger::ToString(args)...);
    }
    
    template<typename... Args>
    inline void Error(const char* format, Args... args) {
        Logger::GetInstance().Error(format, Logger::ToString(args)...);
    }
    
    template<typename... Args>
    inline void Critical(const char* format, Args... args) {
        Logger::GetInstance().Critical(format, Logger::ToString(args)...);
    }

    // RAII日志作用域类，用于临时更改日志级别
    class ScopedLogLevel {
    public:
        ScopedLogLevel(Level newLevel);
        ~ScopedLogLevel();
        
    private:
        Level m_previousLevel;
    };

    // 日志宏定义，用于条件编译和性能优化
    #define LOG_TRACE(...) Log::Trace(__VA_ARGS__)
    #define LOG_DEBUG(...) Log::Debug(__VA_ARGS__)
    #define LOG_INFO(...) Log::Info(__VA_ARGS__)
    #define LOG_WARNING(...) Log::Warning(__VA_ARGS__)
    #define LOG_ERROR(...) Log::Error(__VA_ARGS__)
    #define LOG_CRITICAL(...) Log::Critical(__VA_ARGS__)
}

// 模板函数实现
namespace Log {
    template<typename... Args>
    void Logger::Trace(const char* format, Args... args) {
        if (Level::Trace >= m_config.minLevel) {
            char buffer[1024];
            snprintf(buffer, sizeof(buffer), format, Logger::ToString(args)...);
            Log(Level::Trace, buffer);
        }
    }
    
    template<typename... Args>
    void Logger::Debug(const char* format, Args... args) {
        if (Level::Debug >= m_config.minLevel) {
            char buffer[1024];
            snprintf(buffer, sizeof(buffer), format, Logger::ToString(args)...);
            Log(Level::Debug, buffer);
        }
    }
    
    template<typename... Args>
    void Logger::Info(const char* format, Args... args) {
        if (Level::Info >= m_config.minLevel) {
            char buffer[1024];
            snprintf(buffer, sizeof(buffer), format, Logger::ToString(args)...);
            Log(Level::Info, buffer);
        }
    }
    
    template<typename... Args>
    void Logger::Warning(const char* format, Args... args) {
        if (Level::Warning >= m_config.minLevel) {
            char buffer[1024];
            snprintf(buffer, sizeof(buffer), format, Logger::ToString(args)...);
            Log(Level::Warning, buffer);
        }
    }
    
    template<typename... Args>
    void Logger::Error(const char* format, Args... args) {
        if (Level::Error >= m_config.minLevel) {
            char buffer[1024];
            snprintf(buffer, sizeof(buffer), format, Logger::ToString(args)...);
            Log(Level::Error, buffer);
        }
    }
    
    template<typename... Args>
    void Logger::Critical(const char* format, Args... args) {
        if (Level::Critical >= m_config.minLevel) {
            char buffer[1024];
            snprintf(buffer, sizeof(buffer), format, Logger::ToString(args)...);
            Log(Level::Critical, buffer);
        }
    }
}