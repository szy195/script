#pragma once

#include "Controls.h"
#include "../Core/GameAPI.h"
#include <memory>

namespace KBot {

class UISample {
public:
    UISample();
    ~UISample();
    
    // 初始化UI示例
    void Initialize();
    
    // 渲染UI
    void Render();
    
    // 处理输入
    void HandleInput();
    
    // 清理资源
    void Cleanup();
    
private:
    // 创建主窗口
    void CreateMainWindow();
    
    // 创建菜单栏
    void CreateMenuBar();
    
    // 创建工具栏
    void CreateToolBar();
    
    // 创建状态栏
    void CreateStatusBar();
    
    // 创建主面板
    void CreateMainPanel();
    
    // 创建设置面板
    void CreateSettingsPanel();
    
    // 创建关于对话框
    void CreateAboutDialog();
    
    // 事件处理函数
    void OnMenuFileNew();
    void OnMenuFileOpen();
    void OnMenuFileSave();
    void OnMenuFileExit();
    void OnMenuHelpAbout();
    
    void OnButtonClick();
    void OnSliderValueChanged(float value);
    void OnCheckBoxChanged(bool checked);
    void OnTextBoxChanged(const std::string& text);
    void OnComboBoxChanged(int index);
    void OnTabChanged(int index);
    
    // UI元素
    std::shared_ptr<Window> mainWindow;
    std::shared_ptr<Menu> fileMenu;
    std::shared_ptr<Menu> helpMenu;
    std::shared_ptr<Panel> mainPanel;
    std::shared_ptr<Panel> settingsPanel;
    std::shared_ptr<Window> aboutDialog;
    
    // 控件
    std::shared_ptr<Button> testButton;
    std::shared_ptr<Slider> testSlider;
    std::shared_ptr<CheckBox> testCheckBox;
    std::shared_ptr<TextBox> testTextBox;
    std::shared_ptr<ComboBox> testComboBox;
    std::shared_ptr<TabControl> testTabControl;
    std::shared_ptr<ListBox> testListBox;
    std::shared_ptr<ProgressBar> testProgressBar;
    std::shared_ptr<Image> testImage;
    std::shared_ptr<Label> statusLabel;
    
    // 状态变量
    bool initialized;
    bool showSettings;
    bool showAbout;
};

} // namespace KBot