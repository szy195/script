#pragma once

#ifdef _WIN32
#include <windows.h>
#else
#include "Platform.h"
#include <sys/mman.h>
#include <unistd.h>
#include <dlfcn.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cstdint>
#include <cwchar>
#include <locale>

// 内存访问权限
enum class MemoryAccess {
    READ = 1,
    WRITE = 2,
    EXECUTE = 4,
    ALL = READ | WRITE | EXECUTE
};

// 内存区域信息
struct MemoryRegion {
    uintptr_t baseAddress;
    size_t size;
    DWORD protection;
    std::string moduleName;
    bool isReadable;
    bool isWritable;
    bool isExecutable;
};

// 内存扫描结果
struct MemoryScanResult {
    uintptr_t address;
    std::vector<uint8_t> bytes;
    std::string pattern;
};

// 内存管理器类
class MemoryManager {
public:
    MemoryManager();
    ~MemoryManager();
    
    // 初始化和清理
    bool Initialize();
    void Shutdown();
    bool IsInitialized() const;
    
    // 基础内存操作
    template<typename T>
    bool Read(uintptr_t address, T& value);
    
    template<typename T>
    bool Write(uintptr_t address, const T& value);
    
    bool ReadBytes(uintptr_t address, void* buffer, size_t size);
    bool WriteBytes(uintptr_t address, const void* buffer, size_t size);
    
    // 字符串操作
    std::string ReadString(uintptr_t address, size_t maxLength = 256);
    bool WriteString(uintptr_t address, const std::string& str);
    
    std::wstring ReadWString(uintptr_t address, size_t maxLength = 256);
    bool WriteWString(uintptr_t address, const std::wstring& str);
    
    // 内存扫描
    std::vector<MemoryScanResult> ScanPattern(const std::string& pattern, const std::string& mask, uintptr_t startAddress = 0, size_t size = 0);
    std::vector<uintptr_t> FindPattern(const std::string& pattern, const std::string& mask, uintptr_t startAddress = 0, size_t size = 0);
    std::vector<uintptr_t> FindString(const std::string& str, bool caseSensitive = true, uintptr_t startAddress = 0, size_t size = 0);
    std::vector<uintptr_t> FindValue(uint32_t value, uintptr_t startAddress = 0, size_t size = 0);
    
    // 内存区域管理
    std::vector<MemoryRegion> GetMemoryRegions();
    std::vector<MemoryRegion> GetModuleRegions(const std::string& moduleName);
    MemoryRegion GetMemoryRegion(uintptr_t address);
    
    // 模块信息
    uintptr_t GetModuleBase(const std::string& moduleName);
    size_t GetModuleSize(const std::string& moduleName);
    std::string GetModulePath(uintptr_t address);
    std::vector<std::string> GetModuleList();
    
    // 内存保护
    bool ProtectMemory(uintptr_t address, size_t size, DWORD newProtection, DWORD& oldProtection);
    bool UnprotectMemory(uintptr_t address, size_t size, DWORD& oldProtection);
    
    // 内存分配
    uintptr_t AllocateMemory(size_t size, DWORD protection = PAGE_EXECUTE_READWRITE);
    void FreeMemory(uintptr_t address);
    
    // 代码注入
    bool WriteJump(uintptr_t target, uintptr_t destination);
    bool WriteCall(uintptr_t target, uintptr_t destination);
    bool WriteNop(uintptr_t address, size_t count);
    bool WriteBytes(uintptr_t address, const std::vector<uint8_t>& bytes);
    
    // 钩子管理
    bool CreateHook(uintptr_t target, uintptr_t detour, void** original = nullptr);
    bool RemoveHook(uintptr_t target);
    void* GetOriginalFunction(uintptr_t target);
    
    // 偏移管理
    bool SetOffset(const std::string& name, uintptr_t offset);
    uintptr_t GetOffset(const std::string& name);
    bool LoadOffsets(const std::string& filePath);
    bool SaveOffsets(const std::string& filePath);
    
    // 高级功能
    uintptr_t FollowPointerChain(uintptr_t baseAddress, const std::vector<uintptr_t>& offsets);
    std::vector<uintptr_t> FindAllPointersTo(uintptr_t address);
    bool IsValidPointer(uintptr_t address);
    
    // 调试辅助
    void DumpMemory(uintptr_t address, size_t size, const std::string& filePath = "");
    void DumpMemoryRegion(const MemoryRegion& region, const std::string& filePath = "");
    std::string HexDump(uintptr_t address, size_t size, size_t bytesPerLine = 16);
    
private:
    // 内部辅助方法
    bool EnsureProcessHandle();
    bool IsAddressValid(uintptr_t address);
    std::vector<uint8_t> PatternToBytes(const std::string& pattern);
    bool MatchPattern(const uint8_t* data, const uint8_t* pattern, const char* mask, size_t size);
    
    // 成员变量
    bool m_initialized = false;
    HANDLE m_processHandle = nullptr;
    DWORD m_processId = 0;
    
    // 缓存
    std::map<std::string, uintptr_t> m_moduleBases;
    std::map<std::string, size_t> m_moduleSizes;
    std::map<std::string, uintptr_t> m_offsets;
    std::map<uintptr_t, void*> m_hooks;
    
    // 统计
    size_t m_readCount = 0;
    size_t m_writeCount = 0;
    size_t m_scanCount = 0;
};

// 模板实现
template<typename T>
bool MemoryManager::Read(uintptr_t address, T& value) {
    if (!IsAddressValid(address)) {
        return false;
    }
    
    EnsureProcessHandle();
    
    SIZE_T bytesRead = 0;
    bool result = ReadProcessMemory(m_processHandle, (LPCVOID)address, &value, sizeof(T), &bytesRead);
    
    if (result) {
        m_readCount++;
    }
    
    return result && bytesRead == sizeof(T);
}

template<typename T>
bool MemoryManager::Write(uintptr_t address, const T& value) {
    if (!IsAddressValid(address)) {
        return false;
    }
    
    EnsureProcessHandle();
    
    DWORD oldProtection;
    if (!ProtectMemory(address, sizeof(T), PAGE_EXECUTE_READWRITE, oldProtection)) {
        return false;
    }
    
    SIZE_T bytesWritten = 0;
    bool result = WriteProcessMemory(m_processHandle, (LPVOID)address, &value, sizeof(T), &bytesWritten);
    
    // 恢复原始保护
    ProtectMemory(address, sizeof(T), oldProtection, oldProtection);
    
    if (result) {
        m_writeCount++;
    }
    
    return result && bytesWritten == sizeof(T);
}