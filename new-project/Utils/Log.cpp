#include "Log.h"
#include <cstdarg>
#include <filesystem>
#include <algorithm>
#include <thread>

namespace Log {
    // Logger单例实现
    Logger& Logger::GetInstance() {
        static Logger instance;
        return instance;
    }

    Logger::~Logger() {
        Shutdown();
    }

    bool Logger::Initialize(const LogConfig& config) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        m_config = config;
        
        // 打开日志文件
        if (m_config.fileOutput) {
            // 确保日志目录存在
            std::filesystem::path logPath(m_config.logFilePath);
            std::filesystem::path logDir = logPath.parent_path();
            
            if (!logDir.empty() && !std::filesystem::exists(logDir)) {
                std::error_code ec;
                if (!std::filesystem::create_directories(logDir, ec)) {
                    if (m_config.consoleOutput) {
                        std::cerr << "Failed to create log directory: " << ec.message() << std::endl;
                    }
                    return false;
                }
            }
            
            // 检查文件大小，如果超过限制则轮转
            if (std::filesystem::exists(logPath)) {
                auto fileSize = std::filesystem::file_size(logPath);
                if (fileSize > m_config.maxFileSize) {
                    CheckFileRotation();
                }
            }
            
            m_logFile.open(m_config.logFilePath, std::ios::app);
            if (!m_logFile.is_open()) {
                if (m_config.consoleOutput) {
                    std::cerr << "Failed to open log file: " << m_config.logFilePath << std::endl;
                }
                return false;
            }
        }
        
        m_initialized = true;
        
        // 记录初始化消息（直接输出，避免死锁）
        std::string initMsg = "Log system initialized with minimum level: " + LevelToString(m_config.minLevel);
        if (m_config.consoleOutput) {
            std::cout << "[" << GetCurrentTimestamp() << "] [INFO] " << initMsg << std::endl;
        }
        if (m_config.fileOutput && m_logFile.is_open()) {
            m_logFile << "[" << GetCurrentTimestamp() << "] [INFO] " << initMsg << std::endl;
        }
        
        return true;
    }

    void Logger::Shutdown() {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (m_initialized) {
            // 记录关闭消息（直接输出，避免死锁）
            std::string shutdownMsg = "Log system shutting down";
            if (m_config.consoleOutput) {
                std::cout << "[" << GetCurrentTimestamp() << "] [INFO] " << shutdownMsg << std::endl;
            }
            if (m_config.fileOutput && m_logFile.is_open()) {
                m_logFile << "[" << GetCurrentTimestamp() << "] [INFO] " << shutdownMsg << std::endl;
            }
            
            if (m_logFile.is_open()) {
                m_logFile.close();
            }
            
            m_initialized = false;
        }
    }

    void Logger::SetMinLevel(Level level) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_config.minLevel = level;
    }

    Level Logger::GetMinLevel() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_config.minLevel;
    }

    void Logger::Log(Level level, const char* format, ...) {
        if (level < m_config.minLevel) {
            return;
        }
        
        va_list args;
        va_start(args, format);
        std::string message = FormatVarArgs(format, args);
        va_end(args);
        
        Log(level, message);
    }

    void Logger::Log(Level level, const std::string& message) {
        if (level < m_config.minLevel) {
            return;
        }
        
        std::lock_guard<std::mutex> lock(m_mutex);
        
        std::string formattedMessage = FormatMessage(level, message);
        
        // 输出到控制台
        if (m_config.consoleOutput) {
            WriteToConsole(level, formattedMessage);
        }
        
        // 输出到文件
        if (m_config.fileOutput && m_logFile.is_open()) {
            WriteToFile(level, formattedMessage);
        }
        
        // 添加到最近日志列表
        m_recentLogs.push_back(formattedMessage);
        if (m_recentLogs.size() > m_maxRecentLogs) {
            m_recentLogs.erase(m_recentLogs.begin());
        }
    }

    const LogConfig& Logger::GetConfig() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_config;
    }

    void Logger::SetConfig(const LogConfig& config) {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        bool wasFileOutput = m_config.fileOutput;
        std::string oldLogPath = m_config.logFilePath;
        
        m_config = config;
        
        // 如果文件输出配置发生变化，重新打开文件
        if (wasFileOutput != m_config.fileOutput || oldLogPath != m_config.logFilePath) {
            if (m_logFile.is_open()) {
                m_logFile.close();
            }
            
            if (m_config.fileOutput) {
                m_logFile.open(m_config.logFilePath, std::ios::app);
                if (!m_logFile.is_open() && m_config.consoleOutput) {
                    std::cerr << "Failed to open log file: " << m_config.logFilePath << std::endl;
                }
            }
        }
    }

    void Logger::Flush() {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (m_logFile.is_open()) {
            m_logFile.flush();
        }
    }

    std::vector<std::string> Logger::GetRecentLogs(size_t count) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        
        if (count >= m_recentLogs.size()) {
            return m_recentLogs;
        }
        
        return std::vector<std::string>(m_recentLogs.end() - count, m_recentLogs.end());
    }

    void Logger::ClearLogs() {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_recentLogs.clear();
    }

    void Logger::WriteToFile(Level level, const std::string& message) {
        if (m_logFile.is_open()) {
            m_logFile << message << std::endl;
            
            // 检查文件大小，如果超过限制则轮转
            auto currentPos = m_logFile.tellp();
            if (currentPos > static_cast<std::streampos>(m_config.maxFileSize)) {
                m_logFile.close();
                CheckFileRotation();
                m_logFile.open(m_config.logFilePath, std::ios::app);
            }
        }
    }

    void Logger::WriteToConsole(Level level, const std::string& message) {
        // 根据日志级别选择输出流
        if (level >= Level::Error) {
            std::cerr << message << std::endl;
        } else {
            std::cout << message << std::endl;
        }
    }

    std::string Logger::FormatMessage(Level level, const std::string& message) {
        std::stringstream ss;
        
        // 添加时间戳
        if (m_config.timestampEnabled) {
            ss << "[" << GetCurrentTimestamp() << "] ";
        }
        
        // 添加线程ID
        if (m_config.threadIdEnabled) {
            ss << "[Thread:" << std::this_thread::get_id() << "] ";
        }
        
        // 添加日志级别
        ss << "[" << LevelToString(level) << "] ";
        
        // 添加消息内容
        ss << message;
        
        return ss.str();
    }

    std::string Logger::LevelToString(Level level) const {
        switch (level) {
            case Level::Trace: return "TRACE";
            case Level::Debug: return "DEBUG";
            case Level::Info: return "INFO";
            case Level::Warning: return "WARNING";
            case Level::Error: return "ERROR";
            case Level::Critical: return "CRITICAL";
            default: return "UNKNOWN";
        }
    }

    std::string Logger::GetCurrentTimestamp() const {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        
        return ss.str();
    }

    void Logger::CheckFileRotation() {
        std::filesystem::path logPath(m_config.logFilePath);
        
        // 删除最老的备份文件
        for (size_t i = m_config.maxBackupFiles; i > 1; --i) {
            std::filesystem::path backupPath = logPath;
            backupPath += std::to_string(i - 1);
            
            if (std::filesystem::exists(backupPath)) {
                std::filesystem::path newPath = logPath;
                newPath += std::to_string(i);
                
                std::error_code ec;
                std::filesystem::rename(backupPath, newPath, ec);
            }
        }
        
        // 重命名当前日志文件为备份文件
        if (std::filesystem::exists(logPath)) {
            std::filesystem::path backupPath = logPath;
            backupPath += "1";
            
            std::error_code ec;
            std::filesystem::rename(logPath, backupPath, ec);
        }
    }

    std::string Logger::FormatVarArgs(const char* format, va_list args) {
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), format, args);
        return std::string(buffer);
    }

    // ScopedLogLevel实现
    ScopedLogLevel::ScopedLogLevel(Level newLevel) {
        Logger& logger = Logger::GetInstance();
        m_previousLevel = logger.GetMinLevel();
        logger.SetMinLevel(newLevel);
    }

    ScopedLogLevel::~ScopedLogLevel() {
        Logger::GetInstance().SetMinLevel(m_previousLevel);
    }
}