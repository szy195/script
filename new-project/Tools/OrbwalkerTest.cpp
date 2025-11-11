#include <iostream>
#include <unistd.h>
#include "../Core/KBotCore.h"
#include "../Features/Orbwalker.h"

// 简单的控制台应用程序，用于测试走砍功能
int main() {
    std::cout << "=== KBot 走砍功能测试程序 ===" << std::endl;
    std::cout << "请确保游戏已运行并且KBot已注入" << std::endl;
    
    // 初始化KBot核心
    if (!KBotCore::GetInstance().Initialize()) {
        std::cout << "初始化KBot核心失败" << std::endl;
        return -1;
    }
    
    std::cout << "KBot核心初始化成功" << std::endl;
    
    // 获取走砍功能实例
    auto orbwalker = KBotCore::GetInstance().GetModule<KBot::Features::Orbwalker>("Orbwalker");
    if (!orbwalker) {
        std::cout << "获取走砍功能实例失败" << std::endl;
        return -1;
    }
    
    std::cout << "走砍功能初始化成功" << std::endl;
    
    // 设置走砍功能
    orbwalker->SetEnabled(true);
    orbwalker->SetMode(KBot::Features::OrbwalkerMode::COMBO);
    orbwalker->SetTargetSelector(KBot::Features::TargetSelector::LOWEST_HEALTH);
    
    std::cout << "走砍功能已启用" << std::endl;
    std::cout << "模式: 连招模式" << std::endl;
    std::cout << "目标选择器: 最低血量" << std::endl;
    std::cout << "按键设置:" << std::endl;
    std::cout << "  空格键: 连招模式" << std::endl;
    std::cout << "  C键: 骚扰模式" << std::endl;
    std::cout << "  V键: 清线模式" << std::endl;
    std::cout << "  X键: 补刀模式" << std::endl;
    std::cout << "  Z键: 逃跑模式" << std::endl;
    
    std::cout << "程序将运行10秒后自动退出..." << std::endl;
    
    // 主循环 - 简化为固定时间运行
    for (int i = 0; i < 100; ++i) {
        // 输出当前状态
        std::cout << "\r" << "=== KBot 走砍功能测试程序 ===" << std::flush;
        
        // 休眠100毫秒
        usleep(100000);
    }
    
    std::cout << std::endl;
    
    // 清理资源
    KBotCore::GetInstance().Shutdown();
    
    std::cout << "程序已退出" << std::endl;
    return 0;
}