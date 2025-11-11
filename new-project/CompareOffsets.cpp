#include <iostream>
#include <string>
#include "Core/OffsetComparator.h"

int main(int argc, char* argv[]) {
    std::cout << "===== League of Legends Offset Comparison Tool =====" << std::endl;
    
    if (argc < 3) {
        std::cout << "Usage: OffsetComparator <old_offsets_file> <new_offsets_file>" << std::endl;
        std::cout << "Example: OffsetComparator old_offsets.h new_offsets.h" << std::endl;
        system("pause");
        return 1;
    }
    
    std::string oldFile = argv[1];
    std::string newFile = argv[2];
    
    // 创建偏移量比较器
    OffsetComparator comparator;
    
    // 比较偏移量
    std::cout << "Comparing offsets..." << std::endl;
    auto differences = comparator.CompareFiles(oldFile, newFile);
    
    // 生成比较报告
    std::string reportPath = "offset_comparison_report.txt";
    comparator.GenerateComparisonReport(differences, reportPath);
    
    std::cout << std::endl;
    std::cout << "Comparison completed!" << std::endl;
    std::cout << "Total differences: " << differences.size() << std::endl;
    std::cout << "Report saved to: " << reportPath << std::endl;
    
    // 打印前10个差异
    if (!differences.empty()) {
        std::cout << std::endl;
        std::cout << "First 10 differences:" << std::endl;
        int count = 0;
        for (const auto& diff : differences) {
            if (count >= 10) break;
            std::cout << diff << std::endl;
            count++;
        }
        
        if (differences.size() > 10) {
            std::cout << "... and " << (differences.size() - 10) << " more differences." << std::endl;
        }
    }
    
    system("pause");
    return 0;
}