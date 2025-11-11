#pragma once

#include <string>
#include <vector>
#include <map>

struct OffsetInfo {
    std::string name;
    std::string category;
    uintptr_t offset;
    std::string description;
};

class OffsetComparator {
public:
    OffsetComparator();
    ~OffsetComparator();
    
    // 比较两个偏移量文件
    std::vector<std::string> CompareFiles(const std::string& oldFile, const std::string& newFile);
    
    // 比较两个偏移量映射
    std::vector<std::string> CompareMaps(const std::map<std::string, OffsetInfo>& oldOffsets, 
                                        const std::map<std::string, OffsetInfo>& newOffsets);
    
    // 从文件加载偏移量
    std::map<std::string, OffsetInfo> LoadOffsetsFromFile(const std::string& filePath);
    
    // 保存偏移量到文件
    bool SaveOffsetsToFile(const std::map<std::string, OffsetInfo>& offsets, const std::string& filePath);
    
    // 生成比较报告
    bool GenerateComparisonReport(const std::vector<std::string>& differences, const std::string& filePath);
    
    // 获取当前偏移量（从Offsets.h）
    std::map<std::string, OffsetInfo> GetCurrentOffsets();
    
    // 自动检测偏移量变化
    std::vector<std::string> DetectChanges(const std::string& oldOffsetsFile);
    
private:
    // 解析偏移量行
    bool ParseOffsetLine(const std::string& line, std::string& category, std::string& name, uintptr_t& offset);
    
    // 格式化偏移量差异
    std::string FormatDifference(const std::string& category, const std::string& name, 
                                uintptr_t oldOffset, uintptr_t newOffset);
    
    // 获取偏移量的唯一键
    std::string GetOffsetKey(const std::string& category, const std::string& name);
};