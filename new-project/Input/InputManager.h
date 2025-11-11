#pragma once

#include <unordered_map>
#include <cstdint>

namespace KBot::Input {

// 输入管理器类，用于处理键盘和鼠标输入
class InputManager {
public:
    // 单例模式
    static InputManager& GetInstance();
    static void DestroyInstance();
    
    // 初始化和清理
    bool Initialize();
    void Shutdown();
    
    // 键盘状态检查
    bool IsKeyPressed(int keyCode) const;
    
    // 鼠标状态检查
    bool IsMouseButtonPressed(int button) const;
    
    // 鼠标位置
    void GetMousePosition(int& x, int& y) const;
    void SetMousePosition(int x, int y);
    
    // 更新状态（每帧调用）
    void Update();
    
private:
    InputManager();
    ~InputManager();
    InputManager(const InputManager&) = delete;
    InputManager& operator=(const InputManager&) = delete;
    
    // 静态实例指针
    static InputManager* s_instance;
    
    // 初始化状态
    bool m_initialized = false;
};

} // namespace KBot::Input