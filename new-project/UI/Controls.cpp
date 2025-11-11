#include "Controls.h"
#include "../Utils/Vector.h"
#include "../Utils/Math.h"
#include <algorithm>
#include <iostream>

// Panel实现

Panel::Panel() : UIElement(UIElementType::Panel), autoSize(false) {
    SetSize(200, 150);
}

Panel::~Panel() {
}

void Panel::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 渲染背景
    renderer->RenderBackground(shared_from_this());
    
    // 渲染边框
    renderer->RenderBorder(shared_from_this());
    
    // 渲染子元素
    for (const auto& child : children) {
        child->Render(renderer);
    }
}

// Button实现

Button::Button(const std::string& _text) : UIElement(UIElementType::Button), text(_text) {
    SetSize(100, 30);
    SetTag(text);
}

Button::~Button() {
}

void Button::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的按钮渲染方法
    renderer->RenderButton(shared_from_this());
}

void Button::OnClick(const Vector2& position) {
    UIElement::OnClick(position);
    
    if (clickCallback) {
        clickCallback();
    }
}

// Label实现

Label::Label(const std::string& _text) : UIElement(UIElementType::Label), text(_text), textColor(Color::Black()) {
    SetSize(100, 20);
    SetTag(text);
}

Label::~Label() {
}

void Label::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的标签渲染方法
    renderer->RenderLabel(shared_from_this());
}

// TextBox实现

TextBox::TextBox(const std::string& _text) 
    : UIElement(UIElementType::TextBox), text(_text), readOnly(false), 
      multiline(false), maxLength(0), cursorPosition(0) {
    SetSize(200, 25);
    SetTag(text);
}

TextBox::~TextBox() {
}

void TextBox::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的文本框渲染方法
    renderer->RenderTextBox(shared_from_this());
}

void TextBox::OnKeyPress(int keyCode, bool shift, bool ctrl, bool alt) {
    UIElement::OnKeyPress(keyCode, shift, ctrl, alt);
    
    if (readOnly) return;
    
    // 处理键盘输入
    if (keyCode == 8) { // Backspace
        if (cursorPosition > 0) {
            text.erase(cursorPosition - 1, 1);
            cursorPosition--;
            OnTextChanged();
        }
    } else if (keyCode == 46) { // Delete
        if (cursorPosition < text.length()) {
            text.erase(cursorPosition, 1);
            OnTextChanged();
        }
    } else if (keyCode == 37) { // Left arrow
        if (cursorPosition > 0) {
            cursorPosition--;
        }
    } else if (keyCode == 39) { // Right arrow
        if (cursorPosition < text.length()) {
            cursorPosition++;
        }
    } else if (keyCode == 36) { // Home
        cursorPosition = 0;
    } else if (keyCode == 35) { // End
        cursorPosition = text.length();
    } else if (keyCode >= 32 && keyCode <= 126) { // Printable characters
        if (maxLength <= 0 || text.length() < maxLength) {
            text.insert(cursorPosition, 1, static_cast<char>(keyCode));
            cursorPosition++;
            OnTextChanged();
        }
    }
}

void TextBox::OnTextChanged() {
    SetTag(text);
    
    if (textChangedCallback) {
        textChangedCallback(text);
    }
}

// CheckBox实现

CheckBox::CheckBox(const std::string& _text, bool _checked) 
    : UIElement(UIElementType::CheckBox), text(_text), checked(_checked) {
    SetSize(120, 20);
    SetTag(text);
    
    if (checked) {
        SetState(UIState::Selected);
    }
}

CheckBox::~CheckBox() {
}

void CheckBox::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的复选框渲染方法
    renderer->RenderCheckBox(shared_from_this());
}

void CheckBox::OnClick(const Vector2& position) {
    UIElement::OnClick(position);
    
    // 切换选中状态
    checked = !checked;
    SetState(checked ? UIState::Selected : UIState::Normal);
    OnCheckedChanged();
}

void CheckBox::OnCheckedChanged() {
    if (checkedChangedCallback) {
        checkedChangedCallback(checked);
    }
}

// RadioButton实现

RadioButton::RadioButton(const std::string& _text, bool _checked) 
    : UIElement(UIElementType::RadioButton), text(_text), checked(_checked) {
    SetSize(120, 20);
    SetTag(text);
    
    if (checked) {
        SetState(UIState::Selected);
    }
}

RadioButton::~RadioButton() {
}

void RadioButton::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的单选按钮渲染方法
    renderer->RenderRadioButton(shared_from_this());
}

void RadioButton::OnClick(const Vector2& position) {
    UIElement::OnClick(position);
    
    if (!checked) {
        checked = true;
        SetState(UIState::Selected);
        
        // 取消同一组中其他单选按钮的选中状态
        // 这里需要访问UIManager来获取同一组的其他单选按钮
        // 暂时简化处理
        
        OnCheckedChanged();
    }
}

void RadioButton::OnCheckedChanged() {
    if (checkedChangedCallback) {
        checkedChangedCallback(checked);
    }
}

// Slider实现

Slider::Slider(float _minValue, float _maxValue, float _value) 
    : UIElement(UIElementType::Slider), minValue(_minValue), maxValue(_maxValue), 
      currentValue(_value), vertical(false), dragging(false) {
    SetSize(200, 20);
    UpdateValue();
}

Slider::~Slider() {
}

void Slider::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的滑块渲染方法
    renderer->RenderSlider(shared_from_this());
}

void Slider::OnMouseDown(const Vector2& position, int button) {
    UIElement::OnMouseDown(position, button);
    
    if (button == 0) { // 左键
        dragging = true;
        SetValue(CalculateValueFromPosition(position));
    }
}

void Slider::OnMouseMove(const Vector2& position) {
    UIElement::OnMouseMove(position);
    
    if (dragging) {
        SetValue(CalculateValueFromPosition(position));
    }
}

void Slider::OnMouseUp(const Vector2& position, int button) {
    UIElement::OnMouseUp(position, button);
    
    if (button == 0) { // 左键
        dragging = false;
    }
}

void Slider::SetValue(float value) {
    value = Math::Clamp(value, minValue, maxValue);
    
    if (currentValue != value) {
        currentValue = value;
        OnValueChanged();
    }
}

void Slider::OnValueChanged() {
    if (valueChangedCallback) {
        valueChangedCallback(currentValue);
    }
}

void Slider::UpdateValue() {
    currentValue = Math::Clamp(currentValue, minValue, maxValue);
}

float Slider::CalculateValueFromPosition(const Vector2& position) {
    UIRect bounds = GetScreenBounds();
    
    if (vertical) {
        float relativeY = position.y - bounds.y;
        float percentage = 1.0f - (relativeY / bounds.height);
        return minValue + percentage * (maxValue - minValue);
    } else {
        float relativeX = position.x - bounds.x;
        float percentage = relativeX / bounds.width;
        return minValue + percentage * (maxValue - minValue);
    }
}

float Slider::CalculateSliderPosition() {
    if (vertical) {
        UIRect bounds = GetScreenBounds();
        float percentage = (currentValue - minValue) / (maxValue - minValue);
        return bounds.y + (1.0f - percentage) * bounds.height;
    } else {
        UIRect bounds = GetScreenBounds();
        float percentage = (currentValue - minValue) / (maxValue - minValue);
        return bounds.x + percentage * bounds.width;
    }
}

// ProgressBar实现

ProgressBar::ProgressBar(float _minValue, float _maxValue, float _value) 
    : UIElement(UIElementType::ProgressBar), minValue(_minValue), maxValue(_maxValue), 
      currentValue(_value), showText(true), textFormat("{0}%") {
    SetSize(200, 25);
    UpdateProgress();
}

ProgressBar::~ProgressBar() {
}

void ProgressBar::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的进度条渲染方法
    renderer->RenderProgressBar(shared_from_this());
    
    // 如果需要显示文本，渲染文本
    if (showText) {
        UIRect bounds = GetScreenBounds();
        std::string text = FormatText();
        renderer->DrawText(text.c_str(), bounds.x + bounds.width * 0.5f, bounds.y + bounds.height * 0.5f, 
                          style.foregroundColor, style.font);
    }
}

void ProgressBar::SetValue(float value) {
    value = Math::Clamp(value, minValue, maxValue);
    
    if (currentValue != value) {
        currentValue = value;
        UpdateProgress();
    }
}

void ProgressBar::UpdateProgress() {
    currentValue = Math::Clamp(currentValue, minValue, maxValue);
}

std::string ProgressBar::FormatText() {
    float percentage = (maxValue - minValue) > 0 ? 
                      ((currentValue - minValue) / (maxValue - minValue)) * 100.0f : 0.0f;
    
    // 简单的字符串格式化，实际项目中可能需要更复杂的实现
    char buffer[64];
    sprintf(buffer, "%.1f%%", percentage);
    return std::string(buffer);
}

// ComboBox实现

ComboBox::ComboBox() : UIElement(UIElementType::ComboBox), selectedIndex(-1), editable(false), droppedDown(false) {
    SetSize(150, 25);
}

ComboBox::~ComboBox() {
}

void ComboBox::AddItem(const std::string& item) {
    items.push_back(item);
    
    // 如果没有选中项，选中第一项
    if (selectedIndex < 0 && !items.empty()) {
        SetSelectedIndex(0);
    }
}

void ComboBox::RemoveItem(const std::string& item) {
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        int index = static_cast<int>(it - items.begin());
        items.erase(it);
        
        // 调整选中索引
        if (selectedIndex == index) {
            selectedIndex = items.empty() ? -1 : 0;
            OnSelectedIndexChanged();
        } else if (selectedIndex > index) {
            selectedIndex--;
        }
    }
}

void ComboBox::RemoveItemAt(int index) {
    if (index >= 0 && index < static_cast<int>(items.size())) {
        items.erase(items.begin() + index);
        
        // 调整选中索引
        if (selectedIndex == index) {
            selectedIndex = items.empty() ? -1 : 0;
            OnSelectedIndexChanged();
        } else if (selectedIndex > index) {
            selectedIndex--;
        }
    }
}

const std::string& ComboBox::GetItemAt(int index) const {
    static std::string empty;
    return (index >= 0 && index < static_cast<int>(items.size())) ? items[index] : empty;
}

void ComboBox::SetSelectedIndex(int index) {
    if (index >= -1 && index < static_cast<int>(items.size()) && selectedIndex != index) {
        selectedIndex = index;
        
        if (selectedIndex >= 0) {
            SetTag(items[selectedIndex]);
        } else {
            SetTag("");
        }
        
        OnSelectedIndexChanged();
    }
}

const std::string& ComboBox::GetSelectedItem() const {
    static std::string empty;
    return (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size())) ? 
           items[selectedIndex] : empty;
}

void ComboBox::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的组合框渲染方法
    renderer->RenderComboBox(shared_from_this());
}

void ComboBox::OnClick(const Vector2& position) {
    UIElement::OnClick(position);
    
    // 切换下拉状态
    droppedDown = !droppedDown;
}

void ComboBox::OnSelectedIndexChanged() {
    if (selectedIndexChangedCallback) {
        selectedIndexChangedCallback(selectedIndex);
    }
}

// ListBox实现

ListBox::ListBox() : UIElement(UIElementType::ListBox), selectedIndex(-1), multiSelect(false) {
    SetSize(150, 100);
}

ListBox::~ListBox() {
}

void ListBox::AddItem(const std::string& item) {
    items.push_back(item);
    
    // 如果没有选中项，选中第一项
    if (selectedIndex < 0 && !items.empty()) {
        SetSelectedIndex(0);
    }
}

void ListBox::RemoveItem(const std::string& item) {
    auto it = std::find(items.begin(), items.end(), item);
    if (it != items.end()) {
        int index = static_cast<int>(it - items.begin());
        items.erase(it);
        
        // 调整选中索引
        if (selectedIndex == index) {
            selectedIndex = items.empty() ? -1 : 0;
            OnSelectedIndexChanged();
        } else if (selectedIndex > index) {
            selectedIndex--;
        }
        
        // 调整多选索引
        auto selectedIt = std::find(selectedIndices.begin(), selectedIndices.end(), index);
        if (selectedIt != selectedIndices.end()) {
            selectedIndices.erase(selectedIt);
            
            // 调整大于被删除索引的所有选中索引
            for (auto& idx : selectedIndices) {
                if (idx > index) {
                    idx--;
                }
            }
        }
    }
}

void ListBox::RemoveItemAt(int index) {
    if (index >= 0 && index < static_cast<int>(items.size())) {
        items.erase(items.begin() + index);
        
        // 调整选中索引
        if (selectedIndex == index) {
            selectedIndex = items.empty() ? -1 : 0;
            OnSelectedIndexChanged();
        } else if (selectedIndex > index) {
            selectedIndex--;
        }
        
        // 调整多选索引
        auto selectedIt = std::find(selectedIndices.begin(), selectedIndices.end(), index);
        if (selectedIt != selectedIndices.end()) {
            selectedIndices.erase(selectedIt);
            
            // 调整大于被删除索引的所有选中索引
            for (auto& idx : selectedIndices) {
                if (idx > index) {
                    idx--;
                }
            }
        }
    }
}

const std::string& ListBox::GetItemAt(int index) const {
    static std::string empty;
    return (index >= 0 && index < static_cast<int>(items.size())) ? items[index] : empty;
}

void ListBox::SetSelectedIndex(int index) {
    if (index >= -1 && index < static_cast<int>(items.size())) {
        if (multiSelect) {
            // 多选模式
            if (index >= 0) {
                auto it = std::find(selectedIndices.begin(), selectedIndices.end(), index);
                if (it == selectedIndices.end()) {
                    selectedIndices.push_back(index);
                } else {
                    selectedIndices.erase(it);
                }
            } else {
                selectedIndices.clear();
            }
        } else {
            // 单选模式
            if (selectedIndex != index) {
                selectedIndex = index;
                OnSelectedIndexChanged();
            }
        }
    }
}

const std::string& ListBox::GetSelectedItem() const {
    static std::string empty;
    return (selectedIndex >= 0 && selectedIndex < static_cast<int>(items.size())) ? 
           items[selectedIndex] : empty;
}

void ListBox::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的列表框渲染方法
    renderer->RenderListBox(shared_from_this());
}

void ListBox::OnClick(const Vector2& position) {
    UIElement::OnClick(position);
    
    // 计算点击的项索引
    UIRect bounds = GetScreenBounds();
    float itemHeight = 20.0f; // 假设每项高度为20像素
    int clickedIndex = static_cast<int>((position.y - bounds.y) / itemHeight);
    
    if (clickedIndex >= 0 && clickedIndex < static_cast<int>(items.size())) {
        SetSelectedIndex(clickedIndex);
    }
}

void ListBox::OnSelectedIndexChanged() {
    if (selectedIndexChangedCallback) {
        selectedIndexChangedCallback(selectedIndex);
    }
}

// TabControl实现

TabControl::TabControl() : UIElement(UIElementType::TabControl), selectedIndex(-1), tabHeight(30.0f) {
    SetSize(400, 300);
}

TabControl::~TabControl() {
}

void TabControl::AddTab(const std::string& text, std::shared_ptr<UIElement> content) {
    Tab tab;
    tab.text = text;
    tab.content = content;
    
    tabs.push_back(tab);
    
    // 如果没有选中项，选中第一项
    if (selectedIndex < 0 && !tabs.empty()) {
        SetSelectedIndex(0);
    }
}

void TabControl::RemoveTab(const std::string& text) {
    auto it = std::find_if(tabs.begin(), tabs.end(), 
                          [&text](const Tab& tab) { return tab.text == text; });
    
    if (it != tabs.end()) {
        int index = static_cast<int>(it - tabs.begin());
        tabs.erase(it);
        
        // 调整选中索引
        if (selectedIndex == index) {
            selectedIndex = tabs.empty() ? -1 : 0;
            OnSelectedIndexChanged();
        } else if (selectedIndex > index) {
            selectedIndex--;
        }
    }
}

void TabControl::RemoveTabAt(int index) {
    if (index >= 0 && index < static_cast<int>(tabs.size())) {
        tabs.erase(tabs.begin() + index);
        
        // 调整选中索引
        if (selectedIndex == index) {
            selectedIndex = tabs.empty() ? -1 : 0;
            OnSelectedIndexChanged();
        } else if (selectedIndex > index) {
            selectedIndex--;
        }
    }
}

void TabControl::SetSelectedIndex(int index) {
    if (index >= -1 && index < static_cast<int>(tabs.size()) && selectedIndex != index) {
        selectedIndex = index;
        OnSelectedIndexChanged();
    }
}

std::shared_ptr<UIElement> TabControl::GetTabContent(int index) const {
    return (index >= 0 && index < static_cast<int>(tabs.size())) ? tabs[index].content : nullptr;
}

void TabControl::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 渲染标签页
    for (int i = 0; i < static_cast<int>(tabs.size()); i++) {
        UIRect tabArea = CalculateTabArea(i);
        
        // 设置标签页样式
        Color bgColor = (i == selectedIndex) ? Color::LightGray() : Color::White();
        Color textColor = Color::Black();
        
        renderer->DrawFilledRectangle(tabArea.x, tabArea.y, tabArea.width, tabArea.height, bgColor);
        renderer->DrawRectangle(tabArea.x, tabArea.y, tabArea.width, tabArea.height, Color::Black(), 1.0f);
        renderer->DrawText(tabs[i].text.c_str(), tabArea.x + 5, tabArea.y + tabArea.height * 0.5f, 
                         textColor, style.font);
    }
    
    // 渲染内容区域
    if (selectedIndex >= 0 && selectedIndex < static_cast<int>(tabs.size())) {
        UIRect contentArea = CalculateContentArea();
        auto content = tabs[selectedIndex].content;
        
        if (content) {
            // 设置内容位置和大小
            content->SetPosition(contentArea.GetPosition());
            content->SetSize(contentArea.GetSize());
            
            // 渲染内容
            content->Render(renderer);
        }
    }
}

void TabControl::OnClick(const Vector2& position) {
    UIElement::OnClick(position);
    
    // 检查点击的标签页
    for (int i = 0; i < static_cast<int>(tabs.size()); i++) {
        UIRect tabArea = CalculateTabArea(i);
        
        if (tabArea.Contains(position)) {
            SetSelectedIndex(i);
            break;
        }
    }
}

void TabControl::OnSelectedIndexChanged() {
    if (selectedIndexChangedCallback) {
        selectedIndexChangedCallback(selectedIndex);
    }
}

UIRect TabControl::CalculateTabArea(int index) const {
    UIRect bounds = GetScreenBounds();
    float tabWidth = bounds.width / tabs.size();
    
    return UIRect(bounds.x + index * tabWidth, bounds.y, tabWidth, tabHeight);
}

UIRect TabControl::CalculateContentArea() const {
    UIRect bounds = GetScreenBounds();
    
    return UIRect(bounds.x, bounds.y + tabHeight, bounds.width, bounds.height - tabHeight);
}

// Window实现

Window::Window(const std::string& _title) 
    : UIElement(UIElementType::Window), title(_title), resizable(true), movable(true), 
      showCloseButton(true), modal(false), active(false), dragging(false), resizing(false) {
    SetSize(400, 300);
    SetTag(title);
}

Window::~Window() {
}

void Window::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的窗口渲染方法
    renderer->RenderWindow(shared_from_this());
}

void Window::OnMouseDown(const Vector2& position, int button) {
    UIElement::OnMouseDown(position, button);
    
    if (button == 0) { // 左键
        UIRect titleBarArea = CalculateTitleBarArea();
        UIRect closeButtonArea = CalculateCloseButtonArea();
        UIRect resizeArea = CalculateResizeArea();
        
        if (closeButtonArea.Contains(position) && showCloseButton) {
            // 点击关闭按钮
            if (closeCallback) {
                closeCallback();
            }
        } else if (titleBarArea.Contains(position) && movable) {
            // 开始拖动窗口
            dragging = true;
            dragOffset = position - GetScreenPosition();
        } else if (resizeArea.Contains(position) && resizable) {
            // 开始调整窗口大小
            resizing = true;
            resizeStartSize = GetSize();
            resizeStartPosition = position;
        }
        
        // 设置为活动窗口
        SetActive(true);
    }
}

void Window::OnMouseMove(const Vector2& position) {
    UIElement::OnMouseMove(position);
    
    if (dragging) {
        // 拖动窗口
        Vector2 newPosition = position - dragOffset;
        SetPosition(newPosition);
    } else if (resizing) {
        // 调整窗口大小
        Vector2 delta = position - resizeStartPosition;
        Vector2 newSize = resizeStartSize + Vector2(delta.x, delta.y);
        
        // 确保窗口大小不小于最小值
        newSize.x = Math::Max(newSize.x, 200.0f);
        newSize.y = Math::Max(newSize.y, 150.0f);
        
        SetSize(newSize);
    }
}

void Window::OnMouseUp(const Vector2& position, int button) {
    UIElement::OnMouseUp(position, button);
    
    if (button == 0) { // 左键
        dragging = false;
        resizing = false;
    }
}

UIRect Window::CalculateTitleBarArea() const {
    UIRect bounds = GetScreenBounds();
    float titleBarHeight = 30.0f;
    
    return UIRect(bounds.x, bounds.y, bounds.width, titleBarHeight);
}

UIRect Window::CalculateCloseButtonArea() const {
    UIRect bounds = GetScreenBounds();
    float titleBarHeight = 30.0f;
    float closeButtonSize = 20.0f;
    
    return UIRect(bounds.x + bounds.width - closeButtonSize - 5, 
                 bounds.y + (titleBarHeight - closeButtonSize) * 0.5f,
                 closeButtonSize, closeButtonSize);
}

UIRect Window::CalculateResizeArea() const {
    UIRect bounds = GetScreenBounds();
    float resizeSize = 15.0f;
    
    return UIRect(bounds.x + bounds.width - resizeSize, 
                 bounds.y + bounds.height - resizeSize,
                 resizeSize, resizeSize);
}

// Menu实现

Menu::Menu() : UIElement(UIElementType::Menu), isOpen(false) {
    SetSize(150, 25);
}

Menu::~Menu() {
}

void Menu::AddMenuItem(std::shared_ptr<MenuItem> item) {
    if (item) {
        menuItems.push_back(item);
        item->SetParent(shared_from_this());
    }
}

void Menu::RemoveMenuItem(std::shared_ptr<MenuItem> item) {
    auto it = std::find(menuItems.begin(), menuItems.end(), item);
    if (it != menuItems.end()) {
        menuItems.erase(it);
        item->SetParent(std::weak_ptr<UIElement>());
    }
}

void Menu::RemoveMenuItem(const std::string& text) {
    auto it = std::find_if(menuItems.begin(), menuItems.end(), 
                          [&text](const std::shared_ptr<MenuItem>& item) {
                              return item->GetText() == text;
                          });
    
    if (it != menuItems.end()) {
        (*it)->SetParent(std::weak_ptr<UIElement>());
        menuItems.erase(it);
    }
}

std::shared_ptr<MenuItem> Menu::GetMenuItem(int index) const {
    return (index >= 0 && index < static_cast<int>(menuItems.size())) ? menuItems[index] : nullptr;
}

std::shared_ptr<MenuItem> Menu::GetMenuItem(const std::string& text) const {
    auto it = std::find_if(menuItems.begin(), menuItems.end(), 
                          [&text](const std::shared_ptr<MenuItem>& item) {
                              return item->GetText() == text;
                          });
    
    return (it != menuItems.end()) ? *it : nullptr;
}

void Menu::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的菜单渲染方法
    renderer->RenderMenu(shared_from_this());
    
    // 如果菜单打开，渲染菜单项
    if (isOpen) {
        UIRect bounds = GetScreenBounds();
        float itemHeight = 25.0f;
        
        for (int i = 0; i < static_cast<int>(menuItems.size()); i++) {
            auto item = menuItems[i];
            if (item && item->IsVisible()) {
                // 设置菜单项位置
                item->SetPosition(Vector2(bounds.x, bounds.y + bounds.height + i * itemHeight));
                item->SetSize(Vector2(bounds.width, itemHeight));
                
                // 渲染菜单项
                item->Render(renderer);
            }
        }
    }
}

void Menu::OnClick(const Vector2& position) {
    UIElement::OnClick(position);
    
    // 切换菜单打开状态
    isOpen = !isOpen;
}

void Menu::OnMouseMove(const Vector2& position) {
    UIElement::OnMouseMove(position);
    
    // 如果菜单打开，处理菜单项的鼠标移动
    if (isOpen) {
        for (const auto& item : menuItems) {
            if (item && item->ContainsPoint(position)) {
                item->OnMouseMove(position);
            }
        }
    }
}

// MenuItem实现

MenuItem::MenuItem(const std::string& _text) 
    : UIElement(UIElementType::MenuItem), text(_text), enabled(true), checked(false), 
      separator(false), subMenuOpen(false) {
    SetSize(150, 25);
    SetTag(text);
}

MenuItem::~MenuItem() {
}

void MenuItem::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的菜单项渲染方法
    renderer->RenderMenuItem(shared_from_this());
}

void MenuItem::OnClick(const Vector2& position) {
    UIElement::OnClick(position);
    
    if (enabled && !separator) {
        // 切换选中状态
        if (checked) {
            checked = false;
            SetState(UIState::Normal);
        } else {
            checked = true;
            SetState(UIState::Selected);
        }
        
        OnCheckedChanged();
        
        // 触发点击回调
        if (clickCallback) {
            clickCallback();
        }
    }
}

void MenuItem::OnMouseMove(const Vector2& position) {
    UIElement::OnMouseMove(position);
    
    // 如果有子菜单，处理子菜单的打开/关闭
    if (!subMenuItems.empty()) {
        subMenuOpen = ContainsPoint(position);
    }
}

void MenuItem::AddSubMenuItem(std::shared_ptr<MenuItem> item) {
    if (item) {
        subMenuItems.push_back(item);
        item->SetParent(shared_from_this());
    }
}

void MenuItem::RemoveSubMenuItem(std::shared_ptr<MenuItem> item) {
    auto it = std::find(subMenuItems.begin(), subMenuItems.end(), item);
    if (it != subMenuItems.end()) {
        subMenuItems.erase(it);
        item->SetParent(std::weak_ptr<UIElement>());
    }
}

void MenuItem::RemoveSubMenuItem(const std::string& text) {
    auto it = std::find_if(subMenuItems.begin(), subMenuItems.end(), 
                          [&text](const std::shared_ptr<MenuItem>& item) {
                              return item->GetText() == text;
                          });
    
    if (it != subMenuItems.end()) {
        (*it)->SetParent(std::weak_ptr<UIElement>());
        subMenuItems.erase(it);
    }
}

std::shared_ptr<MenuItem> MenuItem::GetSubMenuItem(int index) const {
    return (index >= 0 && index < static_cast<int>(subMenuItems.size())) ? subMenuItems[index] : nullptr;
}

std::shared_ptr<MenuItem> MenuItem::GetSubMenuItem(const std::string& text) const {
    auto it = std::find_if(subMenuItems.begin(), subMenuItems.end(), 
                          [&text](const std::shared_ptr<MenuItem>& item) {
                              return item->GetText() == text;
                          });
    
    return (it != subMenuItems.end()) ? *it : nullptr;
}

// Image实现

Image::Image() : UIElement(UIElementType::Image), keepAspectRatio(true), scaleMode(0) {
    SetSize(100, 100);
}

Image::~Image() {
}

void Image::Render(UIRenderer* renderer) {
    if (!visible || !renderer) return;
    
    // 使用UIRenderer的图像渲染方法
    renderer->RenderImage(shared_from_this());
}

void Image::UpdateImageSize() {
    // 这里应该根据图像实际大小和缩放模式更新控件大小
    // 暂时简化处理
}

void Image::OnImageChanged() {
    UpdateImageSize();
}