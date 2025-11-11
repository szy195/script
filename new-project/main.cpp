// 确保标准库正确初始化
#include <exception>
#include <stdexcept>

#include "UI/UISample.h"
#include "Core/KBotCore.h"
#include "Renderer/Renderer.h"
#include <iostream>

using namespace KBot;

int main() {
    // 初始化KBot核心
    auto& core = KBotCore::GetInstance();
    if (!core.Initialize()) {
        std::cerr << "Failed to initialize KBot core" << std::endl;
        return -1;
    }
    
    // 初始化渲染器
    auto* renderer = Renderer::GetInstance();
    if (!renderer->Initialize()) {
        std::cerr << "Failed to initialize renderer" << std::endl;
        return -1;
    }
    
    // 创建UI示例
    UISample uiSample;
    uiSample.Initialize();
    
    // 主循环
    bool running = true;
    while (running) {
        // 处理输入
        uiSample.HandleInput();
        
        // 开始渲染
        renderer->Begin();
        
        // 清除屏幕
        renderer->Clear();
        
        // 渲染UI
        uiSample.Render();
        
        // 结束渲染
        renderer->End();
        
        // 简单的退出条件（实际项目中可能需要更复杂的处理）
        // 这里可以添加窗口消息处理等
        
        // 临时退出条件，实际项目中应该根据窗口消息或其他条件
        // 这里为了演示，我们假设运行一段时间后退出
        static int frameCount = 0;
        frameCount++;
        if (frameCount > 10000) { // 运行10000帧后退出
            running = false;
        }
    }
    
    // 清理资源
    uiSample.Cleanup();
    renderer->Shutdown();
    core.Shutdown();
    
    return 0;
}