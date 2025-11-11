#include "UISample.h"
#include "UI.h"
#include "../Renderer/Renderer.h"
#include <iostream>

namespace KBot {

UISample::UISample() : initialized(false), showSettings(false), showAbout(false) {
}

UISample::~UISample() {
    Cleanup();
}

void UISample::Initialize() {
    if (initialized) return;
    
    // 初始化UI管理器
    auto& uiManager = UIManager::GetInstance();
    auto& renderer = Renderer::GetInstance();
    uiManager.Initialize(&renderer);
    
    // 创建UI元素
    CreateMainWindow();
    CreateMenuBar();
    CreateToolBar();
    CreateStatusBar();
    CreateMainPanel();
    CreateSettingsPanel();
    CreateAboutDialog();
    
    initialized = true;
}

void UISample::Render() {
    if (!initialized) return;
    
    // 获取渲染器
    auto& renderer = Renderer::GetInstance();
    
    // 渲染主窗口
    if (mainWindow && mainWindow->IsVisible()) {
        mainWindow->Render(UIManager::GetInstance().GetUIRenderer());
    }
    
    // 渲染设置面板
    if (showSettings && settingsPanel && settingsPanel->IsVisible()) {
        settingsPanel->Render(UIManager::GetInstance().GetUIRenderer());
    }
    
    // 渲染关于对话框
    if (showAbout && aboutDialog && aboutDialog->IsVisible()) {
        aboutDialog->Render(UIManager::GetInstance().GetUIRenderer());
    }
    
    // 渲染菜单
    if (fileMenu && fileMenu->IsVisible()) {
        fileMenu->Render(UIManager::GetInstance().GetUIRenderer());
    }
    
    if (helpMenu && helpMenu->IsVisible()) {
        helpMenu->Render(UIManager::GetInstance().GetUIRenderer());
    }
}

void UISample::HandleInput() {
    if (!initialized) return;
    
    // 处理UI输入
    auto& uiManager = UIManager::GetInstance();
    // 这里应该根据实际输入情况调用相应的UI输入处理方法
    // 例如：uiManager.OnMouseMove(mousePosition);
    // 由于没有具体的输入信息，这里暂时不处理
}

void UISample::Cleanup() {
    if (!initialized) return;
    
    // 清理UI元素
    mainWindow.reset();
    fileMenu.reset();
    helpMenu.reset();
    mainPanel.reset();
    settingsPanel.reset();
    aboutDialog.reset();
    
    testButton.reset();
    testSlider.reset();
    testCheckBox.reset();
    testTextBox.reset();
    testComboBox.reset();
    testTabControl.reset();
    testListBox.reset();
    testProgressBar.reset();
    testImage.reset();
    statusLabel.reset();
    
    // 清理UI管理器
    auto& uiManager = UIManager::GetInstance();
    uiManager.Cleanup();
    
    initialized = false;
}

void UISample::CreateMainWindow() {
    // 创建主窗口
    mainWindow = std::make_shared<Window>("KBot UI Sample");
    mainWindow->SetPosition(Vector2(100, 100));
    mainWindow->SetSize(Vector2(800, 600));
    
    // 设置关闭回调
    mainWindow->SetCloseCallback([this]() {
        mainWindow->SetVisible(false);
    });
    
    // 添加到UI管理器
    auto& uiManager = UIManager::GetInstance();
    uiManager.AddElement(mainWindow);
}

void UISample::CreateMenuBar() {
    // 创建文件菜单
    fileMenu = std::make_shared<Menu>();
    fileMenu->SetPosition(Vector2(10, 35));
    fileMenu->SetSize(Vector2(60, 25));
    
    // 添加菜单项
    auto newItem = std::make_shared<MenuItem>("New");
    newItem->SetClickCallback([this]() { OnMenuFileNew(); });
    fileMenu->AddItem(newItem);
    
    auto openItem = std::make_shared<MenuItem>("Open");
    openItem->SetClickCallback([this]() { OnMenuFileOpen(); });
    fileMenu->AddItem(openItem);
    
    auto saveItem = std::make_shared<MenuItem>("Save");
    saveItem->SetClickCallback([this]() { OnMenuFileSave(); });
    fileMenu->AddItem(saveItem);
    
    // 添加到UI管理器
    auto& uiManager = UIManager::GetInstance();
    uiManager.AddElement(fileMenu);
    
    // 创建帮助菜单
    helpMenu = std::make_shared<Menu>();
    helpMenu->SetPosition(Vector2(80, 35));
    helpMenu->SetSize(Vector2(60, 25));
    
    // 添加菜单项
    auto aboutItem = std::make_shared<MenuItem>("About");
    aboutItem->SetClickCallback([this]() { OnMenuHelpAbout(); });
    helpMenu->AddItem(aboutItem);
    
    // 添加到UI管理器
    uiManager.AddElement(helpMenu);
}

void UISample::CreateToolBar() {
    // 工具栏可以是一个面板，包含多个按钮
    // 这里简化处理，实际实现可能更复杂
}

void UISample::CreateStatusBar() {
    // 创建状态栏标签
    statusLabel = std::make_shared<Label>("Ready");
    statusLabel->SetPosition(Vector2(10, 580));
    statusLabel->SetSize(Vector2(780, 20));
    statusLabel->SetTextColor(Color::Black());
    
    // 添加到主窗口
    if (mainWindow) {
        mainWindow->AddChild(statusLabel);
    }
}

void UISample::CreateMainPanel() {
    // 创建主面板
    mainPanel = std::make_shared<Panel>();
    mainPanel->SetPosition(Vector2(10, 60));
    mainPanel->SetSize(Vector2(780, 510));
    
    // 创建测试按钮
    testButton = std::make_shared<Button>("Test Button");
    testButton->SetPosition(Vector2(20, 20));
    testButton->SetSize(Vector2(120, 30));
    testButton->SetClickCallback([this]() { OnButtonClick(); });
    mainPanel->AddChild(testButton);
    
    // 创建测试滑块
    testSlider = std::make_shared<Slider>(0.0f, 100.0f, 50.0f);
    testSlider->SetPosition(Vector2(20, 60));
    testSlider->SetSize(Vector2(200, 20));
    testSlider->SetValueChangedCallback([this](float value) { OnSliderValueChanged(value); });
    mainPanel->AddChild(testSlider);
    
    // 创建测试复选框
    testCheckBox = std::make_shared<CheckBox>("Test CheckBox", true);
    testCheckBox->SetPosition(Vector2(20, 90));
    testCheckBox->SetSize(Vector2(150, 20));
    testCheckBox->SetCheckedChangedCallback([this](bool checked) { OnCheckBoxChanged(checked); });
    mainPanel->AddChild(testCheckBox);
    
    // 创建测试文本框
    testTextBox = std::make_shared<TextBox>("Test Text");
    testTextBox->SetPosition(Vector2(20, 120));
    testTextBox->SetSize(Vector2(200, 25));
    testTextBox->SetTextChangedCallback([this](const std::string& text) { OnTextBoxChanged(text); });
    mainPanel->AddChild(testTextBox);
    
    // 创建测试组合框
    testComboBox = std::make_shared<ComboBox>();
    testComboBox->SetPosition(Vector2(20, 150));
    testComboBox->SetSize(Vector2(150, 25));
    testComboBox->AddItem("Option 1");
    testComboBox->AddItem("Option 2");
    testComboBox->AddItem("Option 3");
    testComboBox->SetSelectedIndexChangedCallback([this](int index) { OnComboBoxChanged(index); });
    mainPanel->AddChild(testComboBox);
    
    // 创建测试标签页控件
    testTabControl = std::make_shared<TabControl>();
    testTabControl->SetPosition(Vector2(250, 20));
    testTabControl->SetSize(Vector2(500, 300));
    testTabControl->SetSelectedIndexChangedCallback([this](int index) { OnTabChanged(index); });
    
    // 添加标签页
    auto tab1Panel = std::make_shared<Panel>();
    UIStyle tab1Style;
    tab1Style.backgroundColor = Color::LightGray();
    tab1Panel->SetStyle(tab1Style);
    testTabControl->AddTab("Tab 1", tab1Panel);
    
    auto tab2Panel = std::make_shared<Panel>();
    UIStyle tab2Style;
    tab2Style.backgroundColor = Color(0.7f, 0.85f, 1.0f); // 浅蓝色
    tab2Panel->SetStyle(tab2Style);
    testTabControl->AddTab("Tab 2", tab2Panel);
    
    auto tab3Panel = std::make_shared<Panel>();
    UIStyle tab3Style;
    tab3Style.backgroundColor = Color(0.7f, 1.0f, 0.7f); // 浅绿色
    tab3Panel->SetStyle(tab3Style);
    testTabControl->AddTab("Tab 3", tab3Panel);
    
    mainPanel->AddChild(testTabControl);
    
    // 创建测试列表框
    testListBox = std::make_shared<ListBox>();
    testListBox->SetPosition(Vector2(250, 340));
    testListBox->SetSize(Vector2(200, 150));
    testListBox->AddItem("Item 1");
    testListBox->AddItem("Item 2");
    testListBox->AddItem("Item 3");
    testListBox->AddItem("Item 4");
    testListBox->AddItem("Item 5");
    mainPanel->AddChild(testListBox);
    
    // 创建测试进度条
    testProgressBar = std::make_shared<ProgressBar>(0.0f, 100.0f, 75.0f);
    testProgressBar->SetPosition(Vector2(470, 340));
    testProgressBar->SetSize(Vector2(280, 25));
    testProgressBar->SetShowText(true);
    mainPanel->AddChild(testProgressBar);
    
    // 创建测试图像
    testImage = std::make_shared<Image>();
    testImage->SetPosition(Vector2(470, 380));
    testImage->SetSize(Vector2(280, 110));
    // 这里应该设置图像，但暂时简化处理
    mainPanel->AddChild(testImage);
    
    // 添加到主窗口
    if (mainWindow) {
        mainWindow->AddChild(mainPanel);
    }
}

void UISample::CreateSettingsPanel() {
    // 创建设置面板
    settingsPanel = std::make_shared<Panel>();
    settingsPanel->SetPosition(Vector2(200, 150));
    settingsPanel->SetSize(Vector2(400, 300));
    settingsPanel->SetVisible(false); // 默认隐藏
    
    // 添加设置控件...
    // 这里简化处理
    
    // 添加到UI管理器
    auto& uiManager = UIManager::GetInstance();
    uiManager.AddElement(settingsPanel);
}

void UISample::CreateAboutDialog() {
    // 创建关于对话框
    aboutDialog = std::make_shared<Window>("About KBot");
    aboutDialog->SetPosition(Vector2(300, 200));
    aboutDialog->SetSize(Vector2(300, 200));
    aboutDialog->SetVisible(false); // 默认隐藏
    // aboutDialog->SetModal(true); // 注释掉，因为Window类没有SetModal方法
    
    // 添加关于内容
    auto aboutLabel = std::make_shared<Label>("KBot UI Sample\n\nVersion 1.0.0\n\nA sample UI for KBot");
    aboutLabel->SetPosition(Vector2(20, 40));
    aboutLabel->SetSize(Vector2(260, 120));
    aboutLabel->SetTextColor(Color::Black());
    aboutDialog->AddChild(aboutLabel);
    
    // 添加关闭按钮
    auto closeButton = std::make_shared<Button>("Close");
    closeButton->SetPosition(Vector2(110, 160));
    closeButton->SetSize(Vector2(80, 30));
    closeButton->SetClickCallback([this]() {
        showAbout = false;
        aboutDialog->SetVisible(false);
    });
    aboutDialog->AddChild(closeButton);
    
    // 添加到UI管理器
    auto& uiManager = UIManager::GetInstance();
    uiManager.AddElement(aboutDialog);
}

// 事件处理函数实现

void UISample::OnMenuFileNew() {
    if (statusLabel) {
        statusLabel->SetText("File > New clicked");
    }
}

void UISample::OnMenuFileOpen() {
    if (statusLabel) {
        statusLabel->SetText("File > Open clicked");
    }
}

void UISample::OnMenuFileSave() {
    if (statusLabel) {
        statusLabel->SetText("File > Save clicked");
    }
}

void UISample::OnMenuFileExit() {
    if (statusLabel) {
        statusLabel->SetText("File > Exit clicked");
    }
    
    // 隐藏主窗口
    if (mainWindow) {
        mainWindow->SetVisible(false);
    }
}

void UISample::OnMenuHelpAbout() {
    showAbout = true;
    if (aboutDialog) {
        aboutDialog->SetVisible(true);
    }
}

void UISample::OnButtonClick() {
    if (statusLabel) {
        statusLabel->SetText("Test button clicked");
    }
}

void UISample::OnSliderValueChanged(float value) {
    if (statusLabel) {
        char buffer[64];
        sprintf(buffer, "Slider value: %.2f", value);
        statusLabel->SetText(buffer);
    }
}

void UISample::OnCheckBoxChanged(bool checked) {
    if (statusLabel) {
        statusLabel->SetText(checked ? "CheckBox checked" : "CheckBox unchecked");
    }
}

void UISample::OnTextBoxChanged(const std::string& text) {
    if (statusLabel) {
        statusLabel->SetText("TextBox text: " + text);
    }
}

void UISample::OnComboBoxChanged(int index) {
    if (statusLabel && testComboBox) {
        statusLabel->SetText("ComboBox selected: " + testComboBox->GetSelectedItem());
    }
}

void UISample::OnTabChanged(int index) {
    if (statusLabel) {
        char buffer[64];
        sprintf(buffer, "Tab changed to index: %d", index);
        statusLabel->SetText(buffer);
    }
}

} // namespace KBot