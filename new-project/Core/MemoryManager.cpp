#include "MemoryManager.h"
#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#include <TlHelp32.h>
#else
#include "Platform.h"
#include <sys/mman.h>
#include <unistd.h>
#include <dlfcn.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <mach/mach.h>
#include <mach/task.h>
#include <libproc.h>
#endif

// 获取单例实例
MemoryManager* MemoryManager::GetInstance() {
    static MemoryManager instance;
    return &instance;
}

// 构造函数
MemoryManager::MemoryManager() : processHandle(nullptr), isInitialized(false) {
#ifdef _WIN32
    processId = GetCurrentProcessId();
#else
    processId = getpid();
#endif
}

// 析构函数
MemoryManager::~MemoryManager() {
    Shutdown();
}

// 初始化
bool MemoryManager::Initialize() {
    if (isInitialized) {
        return true;
    }
    
#ifdef _WIN32
    // 获取进程句柄
    processHandle = GetCurrentProcess();
    if (processHandle == nullptr) {
        return false;
    }
    
    // 获取模块信息
    RefreshModuleList();
    
    // 获取内存区域
    RefreshMemoryRegions();
#else
    // macOS实现
    processHandle = (void*)1; // 简化处理，使用当前进程
    if (processHandle == nullptr) {
        return false;
    }
    
    // 获取模块信息
    RefreshModuleList();
    
    // 获取内存区域
    RefreshMemoryRegions();
#endif
    
    isInitialized = true;
    return true;
}

// 关闭
void MemoryManager::Shutdown() {
#ifdef _WIN32
    if (processHandle && processHandle != GetCurrentProcess()) {
        CloseHandle(processHandle);
        processHandle = nullptr;
    }
#else
    // macOS实现
    processHandle = nullptr;
#endif
    
    modules.clear();
    memoryRegions.clear();
    isInitialized = false;
}

// 读取内存
bool MemoryManager::ReadMemory(uintptr_t address, void* buffer, size_t size) const {
    if (!isInitialized || !processHandle || !buffer || size == 0) {
        return false;
    }
    
#ifdef _WIN32
    SIZE_T bytesRead = 0;
    return ReadProcessMemory(processHandle, (LPCVOID)address, buffer, size, &bytesRead) == TRUE && bytesRead == size;
#else
    // macOS实现 - 简化版本，直接内存访问
    // 注意：在实际应用中，需要更复杂的实现来处理内存保护
    try {
        memcpy(buffer, (void*)address, size);
        return true;
    } catch (...) {
        return false;
    }
#endif
}

// 写入内存
bool MemoryManager::WriteMemory(uintptr_t address, const void* buffer, size_t size) const {
    if (!isInitialized || !processHandle || !buffer || size == 0) {
        return false;
    }
    
#ifdef _WIN32
    // 更改内存保护
    DWORD oldProtect;
    if (!VirtualProtectEx(processHandle, (LPVOID)address, size, PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return false;
    }
    
    SIZE_T bytesWritten = 0;
    bool result = WriteProcessMemory(processHandle, (LPVOID)address, buffer, size, &bytesWritten) == TRUE && bytesWritten == size;
    
    // 恢复内存保护
    VirtualProtectEx(processHandle, (LPVOID)address, size, oldProtect, &oldProtect);
    
    return result;
#else
    // macOS实现 - 简化版本
    // 更改内存保护
    vm_prot_t oldProtection;
    kern_return_t kr = vm_protect(mach_task_self(), address, size, 0, VM_PROT_READ | VM_PROT_WRITE | VM_PROT_EXECUTE);
    if (kr != KERN_SUCCESS) {
        return false;
    }
    
    // 写入内存
    try {
        memcpy((void*)address, buffer, size);
        return true;
    } catch (...) {
        return false;
    }
#endif
}

// 读取字节
uint8_t MemoryManager::Read<uint8_t>(uintptr_t address) const {
    uint8_t value = 0;
    ReadMemory(address, &value, sizeof(value));
    return value;
}

// 读取16位整数
uint16_t MemoryManager::Read<uint16_t>(uintptr_t address) const {
    uint16_t value = 0;
    ReadMemory(address, &value, sizeof(value));
    return value;
}

// 读取32位整数
uint32_t MemoryManager::Read<uint32_t>(uintptr_t address) const {
    uint32_t value = 0;
    ReadMemory(address, &value, sizeof(value));
    return value;
}

// 读取64位整数
uint64_t MemoryManager::Read<uint64_t>(uintptr_t address) const {
    uint64_t value = 0;
    ReadMemory(address, &value, sizeof(value));
    return value;
}

// 读取浮点数
float MemoryManager::Read<float>(uintptr_t address) const {
    float value = 0.0f;
    ReadMemory(address, &value, sizeof(value));
    return value;
}

// 读取双精度浮点数
double MemoryManager::Read<double>(uintptr_t address) const {
    double value = 0.0;
    ReadMemory(address, &value, sizeof(value));
    return value;
}

// 读取指针
uintptr_t MemoryManager::Read<uintptr_t>(uintptr_t address) const {
    uintptr_t value = 0;
    ReadMemory(address, &value, sizeof(value));
    return value;
}

// 写入字节
bool MemoryManager::Write(uintptr_t address, uint8_t value) const {
    return WriteMemory(address, &value, sizeof(value));
}

// 写入16位整数
bool MemoryManager::Write(uintptr_t address, uint16_t value) const {
    return WriteMemory(address, &value, sizeof(value));
}

// 写入32位整数
bool MemoryManager::Write(uintptr_t address, uint32_t value) const {
    return WriteMemory(address, &value, sizeof(value));
}

// 写入64位整数
bool MemoryManager::Write(uintptr_t address, uint64_t value) const {
    return WriteMemory(address, &value, sizeof(value));
}

// 写入浮点数
bool MemoryManager::Write(uintptr_t address, float value) const {
    return WriteMemory(address, &value, sizeof(value));
}

// 写入双精度浮点数
bool MemoryManager::Write(uintptr_t address, double value) const {
    return WriteMemory(address, &value, sizeof(value));
}

// 写入指针
bool MemoryManager::Write(uintptr_t address, uintptr_t value) const {
    return WriteMemory(address, &value, sizeof(value));
}

// 写入字符串
bool MemoryManager::WriteString(uintptr_t address, const std::string& str) const {
    return WriteMemory(address, str.c_str(), str.length() + 1);
}

// 读取字符串
std::string MemoryManager::ReadString(uintptr_t address, size_t maxLength) const {
    std::string result;
    result.reserve(maxLength);
    
    for (size_t i = 0; i < maxLength; ++i) {
        char c = Read<char>(address + i);
        if (c == '\0') {
            break;
        }
        result += c;
    }
    
    return result;
}

// 读取宽字符串
std::wstring MemoryManager::ReadWideString(uintptr_t address, size_t maxLength) const {
    std::wstring result;
    result.reserve(maxLength);
    
    for (size_t i = 0; i < maxLength; ++i) {
        wchar_t c = Read<wchar_t>(address + i * sizeof(wchar_t));
        if (c == L'\0') {
            break;
        }
        result += c;
    }
    
    return result;
}

// 搜索模式
std::vector<uintptr_t> MemoryManager::FindPattern(const std::vector<uint8_t>& pattern, const std::vector<uint8_t>& mask, uintptr_t startAddress, size_t size) const {
    std::vector<uintptr_t> results;
    
    if (pattern.empty() || mask.empty() || pattern.size() != mask.size()) {
        return results;
    }
    
    // 获取内存区域
    std::vector<MemoryRegion> regions = GetMemoryRegions();
    
    for (const auto& region : regions) {
        // 检查区域是否可读
        if (!(region.protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))) {
            continue;
        }
        
        // 计算搜索范围
        uintptr_t regionStart = std::max(region.base, startAddress);
        uintptr_t regionEnd = region.base + region.size;
        
        if (regionStart >= regionEnd) {
            continue;
        }
        
        // 读取内存区域
        std::vector<uint8_t> buffer(regionEnd - regionStart);
        if (!ReadMemory(regionStart, buffer.data(), buffer.size())) {
            continue;
        }
        
        // 搜索模式
        for (size_t i = 0; i <= buffer.size() - pattern.size(); ++i) {
            bool found = true;
            for (size_t j = 0; j < pattern.size(); ++j) {
                if (mask[j] != 0x00 && buffer[i + j] != pattern[j]) {
                    found = false;
                    break;
                }
            }
            
            if (found) {
                results.push_back(regionStart + i);
            }
        }
    }
    
    return results;
}

// 搜索模式（字符串）
std::vector<uintptr_t> MemoryManager::FindPattern(const std::string& pattern, const std::string& mask, uintptr_t startAddress, size_t size) const {
    std::vector<uint8_t> patternBytes;
    std::vector<uint8_t> maskBytes;
    
    // 解析模式字符串
    for (size_t i = 0; i < pattern.length(); i += 2) {
        if (i + 1 >= pattern.length()) {
            break;
        }
        
        // 处理通配符
        if (pattern[i] == '?' && pattern[i + 1] == '?') {
            patternBytes.push_back(0x00);
            maskBytes.push_back(0x00);
        } else {
            try {
                unsigned int byte;
                std::string byteStr = pattern.substr(i, 2);
                byte = std::stoul(byteStr, nullptr, 16);
                patternBytes.push_back(static_cast<uint8_t>(byte));
                maskBytes.push_back(0xFF);
            } catch (...) {
                patternBytes.push_back(0x00);
                maskBytes.push_back(0x00);
            }
        }
    }
    
    return FindPattern(patternBytes, maskBytes, startAddress, size);
}

// 获取模块基址
uintptr_t MemoryManager::GetModuleBase(const std::string& moduleName) const {
    for (const auto& module : modules) {
        if (module.name == moduleName) {
            return module.base;
        }
    }
    return 0;
}

// 获取模块大小
size_t MemoryManager::GetModuleSize(const std::string& moduleName) const {
    for (const auto& module : modules) {
        if (module.name == moduleName) {
            return module.size;
        }
    }
    return 0;
}

// 获取模块信息
ModuleInfo MemoryManager::GetModuleInfo(const std::string& moduleName) const {
    for (const auto& module : modules) {
        if (module.name == moduleName) {
            return module;
        }
    }
    return ModuleInfo();
}

// 获取所有模块
const std::vector<ModuleInfo>& MemoryManager::GetModules() const {
    return modules;
}

// 刷新模块列表
void MemoryManager::RefreshModuleList() {
    modules.clear();
    
#ifdef _WIN32
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
    if (hSnapshot == INVALID_HANDLE_VALUE) {
        return;
    }
    
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);
    
    if (Module32First(hSnapshot, &me32)) {
        do {
            ModuleInfo module;
            module.base = reinterpret_cast<uintptr_t>(me32.modBaseAddr);
            module.size = me32.modBaseSize;
            module.name = me32.szModule;
            module.path = me32.szExePath;
            
            modules.push_back(module);
        } while (Module32Next(hSnapshot, &me32));
    }
    
    CloseHandle(hSnapshot);
#else
    // macOS实现 - 简化版本
    // 获取主程序信息
    ModuleInfo mainModule;
    mainModule.base = 0x100000000; // 典型的macOS 64位程序基址
    mainModule.size = 0x10000000;  // 假设大小
    mainModule.name = "League of Legends.exe";
    mainModule.path = "/Applications/League of Legends.app/Contents/LoL/League of Legends.exe";
    
    modules.push_back(mainModule);
#endif
}

// 获取内存区域
std::vector<MemoryRegion> MemoryManager::GetMemoryRegions() const {
    return memoryRegions;
}

// 获取指定地址的内存区域
MemoryRegion MemoryManager::GetMemoryRegion(uintptr_t address) const {
    for (const auto& region : memoryRegions) {
        if (address >= region.base && address < region.base + region.size) {
            return region;
        }
    }
    return MemoryRegion();
}

// 刷新内存区域
void MemoryManager::RefreshMemoryRegions() {
    memoryRegions.clear();
    
#ifdef _WIN32
    MEMORY_BASIC_INFORMATION mbi;
    uintptr_t currentAddress = 0;
    
    while (VirtualQueryEx(processHandle, (LPCVOID)currentAddress, &mbi, sizeof(mbi)) == sizeof(mbi)) {
        MemoryRegion region;
        region.base = reinterpret_cast<uintptr_t>(mbi.BaseAddress);
        region.size = mbi.RegionSize;
        region.state = mbi.State;
        region.protect = mbi.Protect;
        region.type = mbi.Type;
        
        memoryRegions.push_back(region);
        
        currentAddress = region.base + region.size;
        
        // 防止无限循环
        if (currentAddress >= 0x7FFFFFFF0000) {
            break;
        }
    }
#else
    // macOS实现 - 简化版本
    // 创建一个基本的内存区域
    MemoryRegion region;
    region.base = 0x100000000; // 典型的macOS 64位程序基址
    region.size = 0x10000000;  // 假设大小
    region.state = 0x1000;    // MEM_COMMIT
    region.protect = 0x40;     // PAGE_EXECUTE_READWRITE
    region.type = 0x1000000;  // MEM_IMAGE
    
    memoryRegions.push_back(region);
#endif
}

// 分配内存
uintptr_t MemoryManager::AllocateMemory(size_t size, uint32_t protection) const {
    if (!isInitialized || !processHandle || size == 0) {
        return 0;
    }
    
    return reinterpret_cast<uintptr_t>(VirtualAllocEx(processHandle, nullptr, size, MEM_COMMIT | MEM_RESERVE, protection));
}

// 释放内存
bool MemoryManager::FreeMemory(uintptr_t address, size_t size) const {
    if (!isInitialized || !processHandle || address == 0) {
        return false;
    }
    
    return VirtualFreeEx(processHandle, (LPVOID)address, size, MEM_RELEASE) == TRUE;
}

// 更改内存保护
bool MemoryManager::ProtectMemory(uintptr_t address, size_t size, uint32_t protection, uint32_t& oldProtection) const {
    if (!isInitialized || !processHandle || address == 0 || size == 0) {
        return false;
    }
    
    return VirtualProtectEx(processHandle, (LPVOID)address, size, protection, reinterpret_cast<DWORD*>(&oldProtection)) == TRUE;
}

// 注入代码
bool MemoryManager::InjectCode(uintptr_t address, const std::vector<uint8_t>& code) const {
    if (!isInitialized || !processHandle || address == 0 || code.empty()) {
        return false;
    }
    
    // 更改内存保护
    DWORD oldProtect;
    if (!VirtualProtectEx(processHandle, (LPVOID)address, code.size(), PAGE_EXECUTE_READWRITE, &oldProtect)) {
        return false;
    }
    
    // 写入代码
    SIZE_T bytesWritten = 0;
    bool result = WriteProcessMemory(processHandle, (LPVOID)address, code.data(), code.size(), &bytesWritten) == TRUE && bytesWritten == code.size();
    
    // 恢复内存保护
    VirtualProtectEx(processHandle, (LPVOID)address, code.size(), oldProtect, &oldProtect);
    
    return result;
}

// 创建钩子
bool MemoryManager::CreateHook(uintptr_t targetAddress, uintptr_t detourAddress, uintptr_t& originalAddress) const {
    if (!isInitialized || !processHandle || targetAddress == 0 || detourAddress == 0) {
        return false;
    }
    
    // 分配跳转代码空间
    uintptr_t jumpCode = AllocateMemory(16, PAGE_EXECUTE_READWRITE);
    if (jumpCode == 0) {
        return false;
    }
    
    // 保存原始指令
    std::vector<uint8_t> originalBytes(16);
    if (!ReadMemory(targetAddress, originalBytes.data(), originalBytes.size())) {
        FreeMemory(jumpCode, 16);
        return false;
    }
    
    // 创建跳转到detour的代码
    std::vector<uint8_t> jumpBytes;
    jumpBytes.push_back(0xE9); // JMP rel32
    uint32_t relOffset = static_cast<uint32_t>(detourAddress - targetAddress - 5);
    jumpBytes.push_back(static_cast<uint8_t>(relOffset & 0xFF));
    jumpBytes.push_back(static_cast<uint8_t>((relOffset >> 8) & 0xFF));
    jumpBytes.push_back(static_cast<uint8_t>((relOffset >> 16) & 0xFF));
    jumpBytes.push_back(static_cast<uint8_t>((relOffset >> 24) & 0xFF));
    
    // 填充剩余字节
    while (jumpBytes.size() < 16) {
        jumpBytes.push_back(0x90); // NOP
    }
    
    // 创建跳转回原始代码的代码
    std::vector<uint8_t> trampolineBytes;
    
    // 复制原始指令
    size_t instructionSize = 5; // 简化处理，假设原始指令长度为5字节
    for (size_t i = 0; i < instructionSize; ++i) {
        trampolineBytes.push_back(originalBytes[i]);
    }
    
    // 添加跳转回原始代码的指令
    trampolineBytes.push_back(0xE9); // JMP rel32
    relOffset = static_cast<uint32_t>((targetAddress + instructionSize) - (jumpCode + trampolineBytes.size()) - 5);
    trampolineBytes.push_back(static_cast<uint8_t>(relOffset & 0xFF));
    trampolineBytes.push_back(static_cast<uint8_t>((relOffset >> 8) & 0xFF));
    trampolineBytes.push_back(static_cast<uint8_t>((relOffset >> 16) & 0xFF));
    trampolineBytes.push_back(static_cast<uint8_t>((relOffset >> 24) & 0xFF));
    
    // 填充剩余字节
    while (trampolineBytes.size() < 16) {
        trampolineBytes.push_back(0x90); // NOP
    }
    
    // 写入跳转代码
    if (!InjectCode(jumpCode, trampolineBytes)) {
        FreeMemory(jumpCode, 16);
        return false;
    }
    
    // 写入钩子代码
    if (!InjectCode(targetAddress, jumpBytes)) {
        FreeMemory(jumpCode, 16);
        return false;
    }
    
    originalAddress = jumpCode;
    return true;
}

// 移除钩子
bool MemoryManager::RemoveHook(uintptr_t targetAddress, uintptr_t originalAddress) const {
    if (!isInitialized || !processHandle || targetAddress == 0 || originalAddress == 0) {
        return false;
    }
    
    // 恢复原始指令
    std::vector<uint8_t> originalBytes(16);
    if (!ReadMemory(originalAddress, originalBytes.data(), originalBytes.size())) {
        return false;
    }
    
    // 写入原始指令
    return InjectCode(targetAddress, originalBytes);
}