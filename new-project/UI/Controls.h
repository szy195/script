#pragma once

#include "UI.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>

// 前向声明
class Panel;
class Button;
class Label;
class TextBox;
class CheckBox;
class RadioButton;
class Slider;
class ProgressBar;
class ComboBox;
class ListBox;
class TabControl;
class Window;
class Menu;
class MenuItem;
class Image;

// 面板类
class Panel : public UIElement {
public:
    // 构造函数
    Panel();
    
    // 析构函数
    virtual ~Panel();
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 设置自动大小
    void SetAutoSize(bool autoSize) { this->autoSize = autoSize; }
    bool IsAutoSize() const { return autoSize; }
    
protected:
    bool autoSize;
};

// 按钮类
class Button : public UIElement {
public:
    // 构造函数
    Button(const std::string& text = "");
    
    // 析构函数
    virtual ~Button();
    
    // 获取和设置文本
    const std::string& GetText() const { return text; }
    void SetText(const std::string& _text) { text = _text; OnTextChanged(); }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 点击事件回调类型
    using ClickCallback = std::function<void()>;
    
    // 设置点击回调
    void SetClickCallback(ClickCallback callback) { clickCallback = callback; }
    
protected:
    // 事件处理
    virtual void OnClick(const Vector2& position) override;
    
    // 文本改变事件
    virtual void OnTextChanged() {}
    
    // 成员变量
    std::string text;
    ClickCallback clickCallback;
};

// 标签类
class Label : public UIElement {
public:
    // 构造函数
    Label(const std::string& text = "");
    
    // 析构函数
    virtual ~Label();
    
    // 获取和设置文本
    const std::string& GetText() const { return text; }
    void SetText(const std::string& _text) { text = _text; OnTextChanged(); }
    
    // 获取和设置文本颜色
    const Color& GetTextColor() const { return textColor; }
    void SetTextColor(const Color& color) { textColor = color; }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
protected:
    // 文本改变事件
    virtual void OnTextChanged() {}
    
    // 成员变量
    std::string text;
    Color textColor;
};

// 文本框类
class TextBox : public UIElement {
public:
    // 构造函数
    TextBox(const std::string& text = "");
    
    // 析构函数
    virtual ~TextBox();
    
    // 获取和设置文本
    const std::string& GetText() const { return text; }
    void SetText(const std::string& _text) { text = _text; OnTextChanged(); }
    
    // 获取和设置占位符文本
    const std::string& GetPlaceholderText() const { return placeholderText; }
    void SetPlaceholderText(const std::string& text) { placeholderText = text; }
    
    // 获取和设置是否只读
    bool IsReadOnly() const { return readOnly; }
    void SetReadOnly(bool _readOnly) { readOnly = _readOnly; }
    
    // 获取和设置是否多行
    bool IsMultiline() const { return multiline; }
    void SetMultiline(bool _multiline) { multiline = _multiline; }
    
    // 获取和设置最大字符数
    int GetMaxLength() const { return maxLength; }
    void SetMaxLength(int length) { maxLength = length; }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnKeyPress(int keyCode, bool shift, bool ctrl, bool alt) override;
    
    // 文本改变事件回调类型
    using TextChangedCallback = std::function<void(const std::string&)>;
    
    // 设置文本改变回调
    void SetTextChangedCallback(TextChangedCallback callback) { textChangedCallback = callback; }
    
protected:
    // 文本改变事件
    virtual void OnTextChanged();
    
    // 成员变量
    std::string text;
    std::string placeholderText;
    bool readOnly;
    bool multiline;
    int maxLength;
    int cursorPosition;
    TextChangedCallback textChangedCallback;
};

// 复选框类
class CheckBox : public UIElement {
public:
    // 构造函数
    CheckBox(const std::string& text = "", bool checked = false);
    
    // 析构函数
    virtual ~CheckBox();
    
    // 获取和设置文本
    const std::string& GetText() const { return text; }
    void SetText(const std::string& _text) { text = _text; OnTextChanged(); }
    
    // 获取和设置选中状态
    bool IsChecked() const { return checked; }
    void SetChecked(bool _checked) { checked = _checked; OnCheckedChanged(); }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnClick(const Vector2& position) override;
    
    // 选中状态改变事件回调类型
    using CheckedChangedCallback = std::function<void(bool)>;
    
    // 设置选中状态改变回调
    void SetCheckedChangedCallback(CheckedChangedCallback callback) { checkedChangedCallback = callback; }
    
protected:
    // 文本改变事件
    virtual void OnTextChanged() {}
    
    // 选中状态改变事件
    virtual void OnCheckedChanged();
    
    // 成员变量
    std::string text;
    bool checked;
    CheckedChangedCallback checkedChangedCallback;
};

// 单选按钮类
class RadioButton : public UIElement {
public:
    // 构造函数
    RadioButton(const std::string& text = "", bool checked = false);
    
    // 析构函数
    virtual ~RadioButton();
    
    // 获取和设置文本
    const std::string& GetText() const { return text; }
    void SetText(const std::string& _text) { text = _text; OnTextChanged(); }
    
    // 获取和设置选中状态
    bool IsChecked() const { return checked; }
    void SetChecked(bool _checked) { checked = _checked; OnCheckedChanged(); }
    
    // 获取和设置组名
    const std::string& GetGroupName() const { return groupName; }
    void SetGroupName(const std::string& name) { groupName = name; }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnClick(const Vector2& position) override;
    
    // 选中状态改变事件回调类型
    using CheckedChangedCallback = std::function<void(bool)>;
    
    // 设置选中状态改变回调
    void SetCheckedChangedCallback(CheckedChangedCallback callback) { checkedChangedCallback = callback; }
    
protected:
    // 文本改变事件
    virtual void OnTextChanged() {}
    
    // 选中状态改变事件
    virtual void OnCheckedChanged();
    
    // 成员变量
    std::string text;
    bool checked;
    std::string groupName;
    CheckedChangedCallback checkedChangedCallback;
};

// 滑块类
class Slider : public UIElement {
public:
    // 构造函数
    Slider(float minValue = 0.0f, float maxValue = 100.0f, float value = 50.0f);
    
    // 析构函数
    virtual ~Slider();
    
    // 获取和设置最小值
    float GetMinValue() const { return minValue; }
    void SetMinValue(float value) { minValue = value; OnRangeChanged(); }
    
    // 获取和设置最大值
    float GetMaxValue() const { return maxValue; }
    void SetMaxValue(float value) { maxValue = value; OnRangeChanged(); }
    
    // 获取和设置当前值
    float GetValue() const { return currentValue; }
    void SetValue(float value) { currentValue = value; OnValueChanged(); }
    
    // 获取和设置是否垂直
    bool IsVertical() const { return vertical; }
    void SetVertical(bool _vertical) { vertical = _vertical; }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnMouseDown(const Vector2& position, int button) override;
    virtual void OnMouseMove(const Vector2& position) override;
    virtual void OnMouseUp(const Vector2& position, int button) override;
    
    // 值改变事件回调类型
    using ValueChangedCallback = std::function<void(float)>;
    
    // 设置值改变回调
    void SetValueChangedCallback(ValueChangedCallback callback) { valueChangedCallback = callback; }
    
protected:
    // 范围改变事件
    virtual void OnRangeChanged() {}
    
    // 值改变事件
    virtual void OnValueChanged();
    
    // 计算滑块位置
    float CalculateSliderPosition() const;
    
    // 计算滑块值
    float CalculateSliderValue(float position) const;
    
    // 成员变量
    float minValue;
    float maxValue;
    float currentValue;
    bool vertical;
    bool dragging;
    ValueChangedCallback valueChangedCallback;
};

// 进度条类
class ProgressBar : public UIElement {
public:
    // 构造函数
    ProgressBar(float minValue = 0.0f, float maxValue = 100.0f, float value = 0.0f);
    
    // 析构函数
    virtual ~ProgressBar();
    
    // 获取和设置最小值
    float GetMinValue() const { return minValue; }
    void SetMinValue(float value) { minValue = value; OnRangeChanged(); }
    
    // 获取和设置最大值
    float GetMaxValue() const { return maxValue; }
    void SetMaxValue(float value) { maxValue = value; OnRangeChanged(); }
    
    // 获取和设置当前值
    float GetValue() const { return currentValue; }
    void SetValue(float value) { currentValue = value; OnValueChanged(); }
    
    // 获取和设置是否显示文本
    bool IsShowText() const { return showText; }
    void SetShowText(bool show) { showText = show; }
    
    // 获取和设置文本格式
    const std::string& GetTextFormat() const { return textFormat; }
    void SetTextFormat(const std::string& format) { textFormat = format; }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
protected:
    // 范围改变事件
    virtual void OnRangeChanged() {}
    
    // 值改变事件
    virtual void OnValueChanged() {}
    
    // 格式化文本
    std::string FormatText() const;
    
    // 成员变量
    float minValue;
    float maxValue;
    float currentValue;
    bool showText;
    std::string textFormat;
};

// 组合框类
class ComboBox : public UIElement {
public:
    // 构造函数
    ComboBox();
    
    // 析构函数
    virtual ~ComboBox();
    
    // 获取和设置项目列表
    const std::vector<std::string>& GetItems() const { return items; }
    void SetItems(const std::vector<std::string>& _items) { items = _items; OnItemsChanged(); }
    
    // 添加项目
    void AddItem(const std::string& item) { items.push_back(item); OnItemsChanged(); }
    
    // 移除项目
    void RemoveItem(const std::string& item);
    void RemoveItem(int index);
    
    // 获取和设置选中索引
    int GetSelectedIndex() const { return selectedIndex; }
    void SetSelectedIndex(int index) { selectedIndex = index; OnSelectedIndexChanged(); }
    
    // 获取和设置选中项
    std::string GetSelectedItem() const;
    void SetSelectedItem(const std::string& item);
    
    // 获取和设置是否可编辑
    bool IsEditable() const { return editable; }
    void SetEditable(bool _editable) { editable = _editable; }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnClick(const Vector2& position) override;
    
    // 选中索引改变事件回调类型
    using SelectedIndexChangedCallback = std::function<void(int)>;
    
    // 设置选中索引改变回调
    void SetSelectedIndexChangedCallback(SelectedIndexChangedCallback callback) { selectedIndexChangedCallback = callback; }
    
protected:
    // 项目改变事件
    virtual void OnItemsChanged() {}
    
    // 选中索引改变事件
    virtual void OnSelectedIndexChanged();
    
    // 成员变量
    std::vector<std::string> items;
    int selectedIndex;
    bool editable;
    bool dropdownOpen;
    SelectedIndexChangedCallback selectedIndexChangedCallback;
};

// 列表框类
class ListBox : public UIElement {
public:
    // 构造函数
    ListBox();
    
    // 析构函数
    virtual ~ListBox();
    
    // 获取和设置项目列表
    const std::vector<std::string>& GetItems() const { return items; }
    void SetItems(const std::vector<std::string>& _items) { items = _items; OnItemsChanged(); }
    
    // 添加项目
    void AddItem(const std::string& item) { items.push_back(item); OnItemsChanged(); }
    
    // 移除项目
    void RemoveItem(const std::string& item);
    void RemoveItem(int index);
    
    // 获取和设置选中索引
    int GetSelectedIndex() const { return selectedIndex; }
    void SetSelectedIndex(int index) { selectedIndex = index; OnSelectedIndexChanged(); }
    
    // 获取和设置选中项
    std::string GetSelectedItem() const;
    void SetSelectedItem(const std::string& item);
    
    // 获取和设置是否允许多选
    bool IsMultiSelect() const { return multiSelect; }
    void SetMultiSelect(bool _multiSelect) { multiSelect = _multiSelect; }
    
    // 获取和设置选中索引列表
    const std::vector<int>& GetSelectedIndices() const { return selectedIndices; }
    void SetSelectedIndices(const std::vector<int>& indices) { selectedIndices = indices; OnSelectedIndicesChanged(); }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnClick(const Vector2& position) override;
    
    // 选中索引改变事件回调类型
    using SelectedIndexChangedCallback = std::function<void(int)>;
    
    // 设置选中索引改变回调
    void SetSelectedIndexChangedCallback(SelectedIndexChangedCallback callback) { selectedIndexChangedCallback = callback; }
    
protected:
    // 项目改变事件
    virtual void OnItemsChanged() {}
    
    // 选中索引改变事件
    virtual void OnSelectedIndexChanged();
    
    // 选中索引列表改变事件
    virtual void OnSelectedIndicesChanged() {}
    
    // 成员变量
    std::vector<std::string> items;
    int selectedIndex;
    bool multiSelect;
    std::vector<int> selectedIndices;
    SelectedIndexChangedCallback selectedIndexChangedCallback;
};

// 标签页控件类
class TabControl : public UIElement {
public:
    // 构造函数
    TabControl();
    
    // 析构函数
    virtual ~TabControl();
    
    // 获取和设置标签页列表
    const std::vector<std::shared_ptr<UIElement>>& GetTabs() const { return tabs; }
    
    // 添加标签页
    void AddTab(const std::string& title, std::shared_ptr<UIElement> content);
    
    // 移除标签页
    void RemoveTab(const std::string& title);
    void RemoveTab(int index);
    
    // 获取和设置选中索引
    int GetSelectedIndex() const { return selectedIndex; }
    void SetSelectedIndex(int index) { selectedIndex = index; OnSelectedIndexChanged(); }
    
    // 获取和设置选中的标签页
    std::shared_ptr<UIElement> GetSelectedTab() const;
    void SetSelectedTab(const std::string& title);
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnClick(const Vector2& position) override;
    
    // 选中索引改变事件回调类型
    using SelectedIndexChangedCallback = std::function<void(int)>;
    
    // 设置选中索引改变回调
    void SetSelectedIndexChangedCallback(SelectedIndexChangedCallback callback) { selectedIndexChangedCallback = callback; }
    
protected:
    // 选中索引改变事件
    virtual void OnSelectedIndexChanged();
    
    // 成员变量
    std::vector<std::shared_ptr<UIElement>> tabs;
    int selectedIndex;
    SelectedIndexChangedCallback selectedIndexChangedCallback;
};

// 窗口类
class Window : public UIElement {
public:
    // 构造函数
    Window(const std::string& title = "");
    
    // 析构函数
    virtual ~Window();
    
    // 获取和设置标题
    const std::string& GetTitle() const { return title; }
    void SetTitle(const std::string& _title) { title = _title; OnTitleChanged(); }
    
    // 获取和设置是否可调整大小
    bool IsResizable() const { return resizable; }
    void SetResizable(bool _resizable) { resizable = _resizable; }
    
    // 获取和设置是否可移动
    bool IsMovable() const { return movable; }
    void SetMovable(bool _movable) { movable = _movable; }
    
    // 获取和设置是否有关闭按钮
    bool HasCloseButton() const { return closeButton; }
    void SetHasCloseButton(bool has) { closeButton = has; }
    
    // 获取和设置是否最小化
    bool IsMinimized() const { return minimized; }
    void SetMinimized(bool _minimized) { minimized = _minimized; OnMinimizedChanged(); }
    
    // 获取和设置是否最大化
    bool IsMaximized() const { return maximized; }
    void SetMaximized(bool _maximized) { maximized = _maximized; OnMaximizedChanged(); }
    
    // 获取和设置内容区域
    std::shared_ptr<UIElement> GetContent() const { return content; }
    void SetContent(std::shared_ptr<UIElement> _content) { content = _content; OnContentChanged(); }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnMouseDown(const Vector2& position, int button) override;
    virtual void OnMouseMove(const Vector2& position) override;
    virtual void OnMouseUp(const Vector2& position, int button) override;
    
    // 关闭事件回调类型
    using CloseCallback = std::function<void()>;
    
    // 设置关闭回调
    void SetCloseCallback(CloseCallback callback) { closeCallback = callback; }
    
protected:
    // 标题改变事件
    virtual void OnTitleChanged() {}
    
    // 最小化改变事件
    virtual void OnMinimizedChanged() {}
    
    // 最大化改变事件
    virtual void OnMaximizedChanged() {}
    
    // 内容改变事件
    virtual void OnContentChanged() {}
    
    // 成员变量
    std::string title;
    bool resizable;
    bool movable;
    bool closeButton;
    bool minimized;
    bool maximized;
    bool dragging;
    bool resizing;
    Vector2 dragOffset;
    Vector2 resizeStartSize;
    Vector2 resizeStartPosition;
    std::shared_ptr<UIElement> content;
    CloseCallback closeCallback;
};

// 菜单类
class Menu : public UIElement {
public:
    // 构造函数
    Menu();
    
    // 析构函数
    virtual ~Menu();
    
    // 获取和设置菜单项列表
    const std::vector<std::shared_ptr<UIElement>>& GetItems() const { return items; }
    
    // 添加菜单项
    void AddItem(std::shared_ptr<UIElement> item) { items.push_back(item); OnItemsChanged(); }
    
    // 移除菜单项
    void RemoveItem(std::shared_ptr<UIElement> item);
    void RemoveItem(const std::string& text);
    
    // 获取和设置是否打开
    bool IsOpen() const { return open; }
    void SetOpen(bool _open) { open = _open; OnOpenChanged(); }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnClick(const Vector2& position) override;
    
protected:
    // 项目改变事件
    virtual void OnItemsChanged() {}
    
    // 打开改变事件
    virtual void OnOpenChanged() {}
    
    // 成员变量
    std::vector<std::shared_ptr<UIElement>> items;
    bool open;
};

// 菜单项类
class MenuItem : public UIElement {
public:
    // 构造函数
    MenuItem(const std::string& text = "");
    
    // 析构函数
    virtual ~MenuItem();
    
    // 获取和设置文本
    const std::string& GetText() const { return text; }
    void SetText(const std::string& _text) { text = _text; OnTextChanged(); }
    
    // 获取和设置是否启用
    bool IsEnabled() const { return enabled; }
    void SetEnabled(bool _enabled) { enabled = _enabled; OnEnabledChanged(); }
    
    // 获取和设置是否选中
    bool IsChecked() const { return checked; }
    void SetChecked(bool _checked) { checked = _checked; OnCheckedChanged(); }
    
    // 获取和设置是否有子菜单
    bool HasSubMenu() const { return hasSubMenu; }
    void SetHasSubMenu(bool _hasSubMenu) { hasSubMenu = _hasSubMenu; }
    
    // 获取和设置子菜单
    std::shared_ptr<Menu> GetSubMenu() const { return subMenu; }
    void SetSubMenu(std::shared_ptr<Menu> menu) { subMenu = menu; hasSubMenu = (menu != nullptr); }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
    // 事件处理
    virtual void OnClick(const Vector2& position) override;
    
    // 点击事件回调类型
    using ClickCallback = std::function<void()>;
    
    // 设置点击回调
    void SetClickCallback(ClickCallback callback) { clickCallback = callback; }
    
protected:
    // 文本改变事件
    virtual void OnTextChanged() {}
    
    // 启用改变事件
    virtual void OnEnabledChanged() {}
    
    // 选中改变事件
    virtual void OnCheckedChanged() {}
    
    // 成员变量
    std::string text;
    bool enabled;
    bool checked;
    bool hasSubMenu;
    std::shared_ptr<Menu> subMenu;
    ClickCallback clickCallback;
};

// 图像类
class Image : public UIElement {
public:
    // 构造函数
    Image(const std::string& texturePath = "");
    
    // 析构函数
    virtual ~Image();
    
    // 获取和设置纹理路径
    const std::string& GetTexturePath() const { return texturePath; }
    void SetTexturePath(const std::string& path) { texturePath = path; OnTextureChanged(); }
    
    // 获取和设置是否保持宽高比
    bool IsKeepAspectRatio() const { return keepAspectRatio; }
    void SetKeepAspectRatio(bool keep) { keepAspectRatio = keep; }
    
    // 获取和设置拉伸模式
    int GetStretchMode() const { return stretchMode; }
    void SetStretchMode(int mode) { stretchMode = mode; }
    
    // 渲染
    virtual void Render(UIRenderer* renderer) override;
    
protected:
    // 纹理改变事件
    virtual void OnTextureChanged() {}
    
    // 成员变量
    std::string texturePath;
    bool keepAspectRatio;
    int stretchMode;
};