#include <iostream>
#include "../Core/KBotCore.h"
#include "../Tools/OffsetFinder.h"

// 简单的控制台应用程序，用于管理通过外部工具找到的基址
int main() {
    std::cout << "=== KBot 基址管理工具 ===" << std::endl;
    std::cout << "用于管理通过CE和x64dbg等外部工具找到的基址" << std::endl;
    
    // 初始化KBot核心
    if (!KBotCore::GetInstance().Initialize()) {
        std::cout << "初始化KBot核心失败" << std::endl;
        return -1;
    }
    
    std::cout << "KBot核心初始化成功" << std::endl;
    
    // 初始化基址管理工具
    auto& offsetFinder = KBot::Tools::OffsetFinder::GetInstance();
    if (!offsetFinder.Initialize()) {
        std::cout << "初始化基址管理工具失败" << std::endl;
        return -1;
    }
    
    std::cout << "基址管理工具初始化成功" << std::endl;
    
    // 加载外部工具找到的基址
    if (offsetFinder.LoadOffsetsFromFile("external_offsets.txt")) {
        std::cout << "基址加载完成" << std::endl;
        
        // 验证基址
        if (offsetFinder.ValidateOffsets()) {
            std::cout << "基址验证通过" << std::endl;
        } else {
            std::cout << "基址验证失败，请检查基址是否正确" << std::endl;
        }
        
        // 打印当前基址
        offsetFinder.PrintCurrentOffsets();
    } else {
        std::cout << "基址加载失败" << std::endl;
    }
    
    // 清理资源
    offsetFinder.Shutdown();
    KBotCore::GetInstance().Shutdown();
    
    return 0;
}