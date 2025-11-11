#include <iostream>
#include "Core/OffsetTester.h"
#include "Core/MemoryManager.h"

int main() {
    std::cout << "===== League of Legends Offset Testing Tool =====" << std::endl;
    
    // 初始化内存管理器
    if (!MemoryManager::GetInstance()->Initialize()) {
        std::cout << "Failed to initialize MemoryManager" << std::endl;
        system("pause");
        return 1;
    }
    
    // 创建偏移量测试器
    OffsetTester tester;
    if (!tester.Initialize()) {
        std::cout << "Failed to initialize OffsetTester" << std::endl;
        system("pause");
        return 1;
    }
    
    std::cout << "OffsetTester initialized successfully!" << std::endl;
    std::cout << std::endl;
    
    // 运行所有测试
    std::cout << "Running all offset tests..." << std::endl;
    auto results = tester.RunAllTests();
    
    // 生成报告
    std::string reportPath = "offset_test_report.txt";
    tester.GenerateReport(results, reportPath);
    
    // 保存当前偏移量
    std::string offsetsPath = "current_offsets.h";
    tester.SaveCurrentOffsets(offsetsPath);
    
    std::cout << std::endl;
    std::cout << "Testing completed!" << std::endl;
    std::cout << "Report saved to: " << reportPath << std::endl;
    std::cout << "Offsets saved to: " << offsetsPath << std::endl;
    
    system("pause");
    return 0;
}