#pragma once

#include "../Core/MemoryManager.h"
#include "../Core/GameAPI.h"
#include "../Core/Offsets.h"
#include "../Game/Hero.h"
#include "../Utils/Vector.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdint>

namespace KBot::Tools {

// 基址管理工具类 - 用于管理通过外部工具找到的基址
class OffsetFinder {
public:
    // 单例模式
    static OffsetFinder& GetInstance();
    
    // 初始化和清理
    bool Initialize();
    void Shutdown();
    
    // 加载外部工具找到的基址
    bool LoadOffsetsFromFile(const std::string& filePath);
    
    // 验证基址是否有效
    bool ValidateOffsets();
    
    // 输出当前基址
    void PrintCurrentOffsets();
    
private:
    OffsetFinder() = default;
    ~OffsetFinder() = default;
    OffsetFinder(const OffsetFinder&) = delete;
    OffsetFinder& operator=(const OffsetFinder&) = delete;
    
    // 成员变量
    bool m_initialized = false;
    class MemoryManager* m_memoryManager = nullptr;
    class GameAPI* m_gameAPI = nullptr;
};

} // namespace KBot::Tools