#include "Core/OffsetValidationSystem.h"
#include "Core/MemoryManager.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::cout << "===== Offset Validation System =====" << std::endl;
    
    // 初始化内存管理器
    MemoryManager* memManager = MemoryManager::GetInstance();
    if (!memManager || !memManager->Initialize()) {
        std::cout << "Failed to initialize MemoryManager!" << std::endl;
        return 1;
    }
    
    // 初始化验证系统
    OffsetValidationSystem validationSystem;
    if (!validationSystem.Initialize()) {
        std::cout << "Failed to initialize OffsetValidationSystem!" << std::endl;
        return 1;
    }
    
    // 解析命令行参数
    std::string category = "";
    std::string outputFileName = "offset_validation_report.txt";
    
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--category" && i + 1 < argc) {
            category = argv[++i];
        } else if (arg == "--output" && i + 1 < argc) {
            outputFileName = argv[++i];
        } else if (arg == "--help") {
            std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --category <name>    Run only validations for a specific category" << std::endl;
            std::cout << "  --output <file>      Specify output file for the report" << std::endl;
            std::cout << "  --help               Show this help message" << std::endl;
            std::cout << std::endl;
            std::cout << "Available categories:" << std::endl;
            std::cout << "  Game                 Game-related validations" << std::endl;
            std::cout << "  GameObject           GameObject-related validations" << std::endl;
            std::cout << "  Hero                 Hero-related validations" << std::endl;
            std::cout << "  Minion               Minion-related validations" << std::endl;
            std::cout << "  Turret               Turret-related validations" << std::endl;
            std::cout << "  GameObjectManager    GameObjectManager-related validations" << std::endl;
            return 0;
        }
    }
    
    // 运行验证
    std::vector<ValidationResult> results;
    if (category.empty()) {
        std::cout << "Running all validations..." << std::endl;
        results = validationSystem.RunAllValidations();
    } else {
        std::cout << "Running validations for category: " << category << std::endl;
        results = validationSystem.RunCategoryValidations(category);
    }
    
    // 生成报告
    if (validationSystem.GenerateReport(results, outputFileName)) {
        std::cout << "Validation report generated: " << outputFileName << std::endl;
    } else {
        std::cout << "Failed to generate validation report!" << std::endl;
    }
    
    // 获取统计信息
    int total, passed, failed;
    validationSystem.GetStatistics(total, passed, failed);
    
    std::cout << std::endl;
    std::cout << "===== Validation Summary =====" << std::endl;
    std::cout << "Total: " << total << std::endl;
    std::cout << "Passed: " << passed << std::endl;
    std::cout << "Failed: " << failed << std::endl;
    std::cout << "Success Rate: " << std::fixed << std::setprecision(2) 
              << (total > 0 ? (float)passed / total * 100.0f : 0.0f) << "%" << std::endl;
    std::cout << "=============================" << std::endl;
    
    return 0;
}