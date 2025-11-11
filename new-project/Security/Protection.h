#pragma once

#include <string>
#include <memory>

namespace KBot {

class Protection {
public:
    Protection() = default;
    ~Protection() = default;
    
    // 初始化和清理
    bool Initialize();
    void Shutdown();
    
    // 保护功能
    bool EnableAntiDetection();
    bool DisableAntiDetection();
    
    // 检测功能
    bool IsBeingDetected() const;
    bool IsGameProtected() const;
    
    // 安全检查
    bool PerformSecurityCheck();
    bool IsEnvironmentSafe() const;
    
    // 调试保护
    bool EnableDebugProtection();
    bool DisableDebugProtection();
    
    // 内存保护
    bool EnableMemoryProtection();
    bool DisableMemoryProtection();
    
    // 获取状态
    bool IsInitialized() const;
    bool IsAntiDetectionEnabled() const;
    bool IsDebugProtectionEnabled() const;
    bool IsMemoryProtectionEnabled() const;
    
    // 获取保护级别
    int GetProtectionLevel() const;
    
    // 设置保护级别
    void SetProtectionLevel(int level);
    
    // 获取错误信息
    const char* GetLastError() const;

private:
    // 私有实现方法
    bool InitializeInternal();
    void ShutdownInternal();
    
    // 检测方法
    bool CheckForDebuggers() const;
    bool CheckForVirtualization() const;
    bool CheckForAnalysisTools() const;
    
    // 保护方法
    bool ApplyAntiDebugging();
    bool ApplyAntiAnalysis();
    bool ApplyMemoryObfuscation();
    
    // 成员变量
    bool m_initialized = false;
    bool m_antiDetectionEnabled = false;
    bool m_debugProtectionEnabled = false;
    bool m_memoryProtectionEnabled = false;
    int m_protectionLevel = 1;
    std::string m_lastError;
};

} // namespace KBot