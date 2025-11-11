#include "UI.h"
#include "../Utils/Vector.h"
#include "../Utils/Math.h"
#include <algorithm>
#include <iostream>

// UIElement实现

UIElement::UIElement(UIElementType _type) 
    : type(_type), position(0, 0), size(100, 30), visible(true), enabled(true), 
      focused(false), state(UIState::Normal), data(nullptr) {
}

UIElement::~UIElement() {
    children.clear();
    eventCallbacks.clear();
}

void UIElement::AddChild(std::shared_ptr<UIElement> child) {
    if (!child) return;
    
    // 如果子元素已经有父元素，先从原父元素中移除
    auto currentParent = child->GetParent();
    if (currentParent) {
        currentParent->RemoveChild(child);
    }
    
    // 添加到子元素列表
    children.push_back(child);
    child->SetParent(shared_from_this());
}

void UIElement::RemoveChild(std::shared_ptr<UIElement> child) {
    if (!child) return;
    
    auto it = std::find(children.begin(), children.end(), child);
    if (it != children.end()) {
        children.erase(it);
        child->SetParent(std::weak_ptr<UIElement>());
    }
}

void UIElement::RemoveChild(const std::string& childID) {
    auto it = std::find_if(children.begin(), children.end(), 
                          [&childID](const std::shared_ptr<UIElement>& child) {
                              return child->GetID() == childID;
                          });
    
    if (it != children.end()) {
        (*it)->SetParent(std::weak_ptr<UIElement>());
        children.erase(it);
    }
}

std::shared_ptr<UIElement> UIElement::FindChild(const std::string& childID) const {
    auto it = std::find_if(children.begin(), children.end(), 
                          [&childID](const std::shared_ptr<UIElement>& child) {
                              return child->GetID() == childID;
                          });
    
    return (it != children.end()) ? *it : nullptr;
}

std::shared_ptr<UIElement> UIElement::FindChildRecursive(const std::string& childID) const {
    // 先在直接子元素中查找
    auto child = FindChild(childID);
    if (child) return child;
    
    // 递归在子元素的子元素中查找
    for (const auto& c : children) {
        child = c->FindChildRecursive(childID);
        if (child) return child;
    }
    
    return nullptr;
}

bool UIElement::ContainsPoint(const Vector2& point) const {
    return GetScreenBounds().Contains(point);
}

Vector2 UIElement::GetScreenPosition() const {
    Vector2 screenPos = position;
    
    // 加上父元素的位置
    auto parent = GetParent();
    if (parent) {
        screenPos += parent->GetScreenPosition();
    }
    
    return screenPos;
}

UIRect UIElement::GetScreenBounds() const {
    Vector2 screenPos = GetScreenPosition();
    return UIRect(screenPos.x, screenPos.y, size.x, size.y);
}

void UIElement::OnMouseEnter(const Vector2& position) {
    if (state != UIState::Disabled) {
        SetState(UIState::Hover);
        UIEvent event(UIEventType::MouseEnter, shared_from_this());
        event.mousePosition = position;
        TriggerEvent(event);
    }
}

void UIElement::OnMouseLeave(const Vector2& position) {
    if (state != UIState::Disabled) {
        SetState(UIState::Normal);
        UIEvent event(UIEventType::MouseLeave, shared_from_this());
        event.mousePosition = position;
        TriggerEvent(event);
    }
}

void UIElement::OnMouseDown(const Vector2& position, int button) {
    if (enabled) {
        SetState(UIState::Pressed);
        UIEvent event(UIEventType::MouseDown, shared_from_this());
        event.mousePosition = position;
        TriggerEvent(event);
    }
}

void UIElement::OnMouseUp(const Vector2& position, int button) {
    if (enabled) {
        if (state == UIState::Pressed) {
            OnClick(position);
        }
        
        SetState(ContainsPoint(position) ? UIState::Hover : UIState::Normal);
        
        UIEvent event(UIEventType::MouseUp, shared_from_this());
        event.mousePosition = position;
        TriggerEvent(event);
    }
}

void UIElement::OnClick(const Vector2& position) {
    if (enabled) {
        UIEvent event(UIEventType::Click, shared_from_this());
        event.mousePosition = position;
        TriggerEvent(event);
    }
}

void UIElement::OnDoubleClick(const Vector2& position) {
    if (enabled) {
        UIEvent event(UIEventType::DoubleClick, shared_from_this());
        event.mousePosition = position;
        TriggerEvent(event);
    }
}

void UIElement::OnKeyPress(int keyCode, bool shift, bool ctrl, bool alt) {
    if (enabled && focused) {
        UIEvent event(UIEventType::KeyPress, shared_from_this());
        event.keyCode = keyCode;
        event.shiftPressed = shift;
        event.ctrlPressed = ctrl;
        event.altPressed = alt;
        TriggerEvent(event);
    }
}

void UIElement::OnKeyRelease(int keyCode, bool shift, bool ctrl, bool alt) {
    if (enabled && focused) {
        UIEvent event(UIEventType::KeyRelease, shared_from_this());
        event.keyCode = keyCode;
        event.shiftPressed = shift;
        event.ctrlPressed = ctrl;
        event.altPressed = alt;
        TriggerEvent(event);
    }
}

void UIElement::OnFocus() {
    if (enabled) {
        focused = true;
        UIEvent event(UIEventType::Focus, shared_from_this());
        TriggerEvent(event);
    }
}

void UIElement::OnBlur() {
    focused = false;
    UIEvent event(UIEventType::Blur, shared_from_this());
    TriggerEvent(event);
}

void UIElement::OnValueChange() {
    UIEvent event(UIEventType::ValueChange, shared_from_this());
    TriggerEvent(event);
}

void UIElement::OnResize(const Vector2& oldSize, const Vector2& newSize) {
    UIEvent event(UIEventType::Resize, shared_from_this());
    TriggerEvent(event);
}

void UIElement::OnMove(const Vector2& oldPosition, const Vector2& newPosition) {
    UIEvent event(UIEventType::Move, shared_from_this());
    TriggerEvent(event);
}

void UIElement::OnShow() {
    visible = true;
    UIEvent event(UIEventType::Show, shared_from_this());
    TriggerEvent(event);
}

void UIElement::OnHide() {
    visible = false;
    UIEvent event(UIEventType::Hide, shared_from_this());
    TriggerEvent(event);
}

void UIElement::Render(UIRenderer* renderer) {
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

void UIElement::Update(float deltaTime) {
    // 子元素更新
    for (const auto& child : children) {
        child->Update(deltaTime);
    }
}

void UIElement::AddEventListener(UIEventType eventType, UIEventCallback callback) {
    eventCallbacks[eventType] = callback;
}

void UIElement::RemoveEventListener(UIEventType eventType) {
    eventCallbacks.erase(eventType);
}

void UIElement::TriggerEvent(const UIEvent& event) {
    auto it = eventCallbacks.find(event.type);
    if (it != eventCallbacks.end()) {
        it->second(event);
    }
}

// UIRenderer实现

UIRenderer::UIRenderer(Renderer* _renderer) : renderer(_renderer) {
}

UIRenderer::~UIRenderer() {
}

void UIRenderer::RenderElement(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    switch (element->GetType()) {
        case UIElementType::Panel:
            RenderPanel(element);
            break;
        case UIElementType::Button:
            RenderButton(element);
            break;
        case UIElementType::Label:
            RenderLabel(element);
            break;
        case UIElementType::TextBox:
            RenderTextBox(element);
            break;
        case UIElementType::CheckBox:
            RenderCheckBox(element);
            break;
        case UIElementType::RadioButton:
            RenderRadioButton(element);
            break;
        case UIElementType::Slider:
            RenderSlider(element);
            break;
        case UIElementType::ProgressBar:
            RenderProgressBar(element);
            break;
        case UIElementType::ComboBox:
            RenderComboBox(element);
            break;
        case UIElementType::ListBox:
            RenderListBox(element);
            break;
        case UIElementType::TabControl:
            RenderTabControl(element);
            break;
        case UIElementType::Window:
            RenderWindow(element);
            break;
        case UIElementType::Menu:
            RenderMenu(element);
            break;
        case UIElementType::MenuItem:
            RenderMenuItem(element);
            break;
        case UIElementType::Image:
            RenderImage(element);
            break;
        default:
            // 默认渲染
            RenderBackground(element);
            RenderBorder(element);
            break;
    }
    
    // 渲染子元素
    for (const auto& child : element->GetChildren()) {
        RenderElement(child);
    }
}

void UIRenderer::RenderPanel(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    // 渲染背景
    RenderBackground(element);
    
    // 渲染边框
    RenderBorder(element);
}

void UIRenderer::RenderButton(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    // 根据状态调整颜色
    Color bgColor = element->GetStyle().backgroundColor;
    
    switch (element->GetState()) {
        case UIState::Hover:
            bgColor = Color(bgColor.r * 0.9f, bgColor.g * 0.9f, bgColor.b * 0.9f, bgColor.a);
            break;
        case UIState::Pressed:
            bgColor = Color(bgColor.r * 0.8f, bgColor.g * 0.8f, bgColor.b * 0.8f, bgColor.a);
            break;
        case UIState::Disabled:
            bgColor = Color(bgColor.r * 0.6f, bgColor.g * 0.6f, bgColor.b * 0.6f, bgColor.a);
            break;
        default:
            break;
    }
    
    // 渲染背景
    UIRect bounds = element->GetScreenBounds();
    renderer->DrawFilledRectangle(bounds.x, bounds.y, bounds.width, bounds.height, bgColor);
    
    // 渲染边框
    RenderBorder(element);
    
    // 渲染文本（如果有）
    // 这里假设元素的标签是文本内容
    const std::string& text = element->GetTag();
    if (!text.empty()) {
        RenderText(element, text, Vector2(bounds.x + bounds.width * 0.5f, bounds.y + bounds.height * 0.5f));
    }
}

void UIRenderer::RenderLabel(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    // 渲染文本
    const std::string& text = element->GetTag();
    if (!text.empty()) {
        UIRect bounds = element->GetScreenBounds();
        RenderText(element, text, Vector2(bounds.x, bounds.y + bounds.height * 0.5f));
    }
}

void UIRenderer::RenderTextBox(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    // 渲染背景
    RenderBackground(element);
    
    // 渲染边框
    RenderBorder(element);
    
    // 渲染文本
    const std::string& text = element->GetTag();
    if (!text.empty()) {
        UIRect bounds = element->GetScreenBounds();
        RenderText(element, text, Vector2(bounds.x + 5, bounds.y + bounds.height * 0.5f));
    }
    
    // 如果有焦点，渲染光标
    if (element->IsFocused()) {
        UIRect bounds = element->GetScreenBounds();
        Vector2 textSize = CalculateTextSize(element->GetTag(), element->GetStyle().font);
        float cursorX = bounds.x + 5 + textSize.x;
        renderer->DrawLine(cursorX, bounds.y + 5, cursorX, bounds.y + bounds.height - 5, Color::Black(), 1.0f);
    }
}

void UIRenderer::RenderCheckBox(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    UIRect bounds = element->GetScreenBounds();
    
    // 渲染复选框
    float checkBoxSize = 16.0f;
    float checkBoxX = bounds.x;
    float checkBoxY = bounds.y + (bounds.height - checkBoxSize) * 0.5f;
    
    renderer->DrawRectangle(checkBoxX, checkBoxY, checkBoxSize, checkBoxSize, element->GetStyle().borderColor, 1.0f);
    
    // 如果选中，渲染勾选标记
    if (element->GetState() == UIState::Selected) {
        renderer->DrawLine(checkBoxX + 3, checkBoxY + 8, checkBoxX + 6, checkBoxY + 11, element->GetStyle().foregroundColor, 2.0f);
        renderer->DrawLine(checkBoxX + 6, checkBoxY + 11, checkBoxX + 13, checkBoxY + 3, element->GetStyle().foregroundColor, 2.0f);
    }
    
    // 渲染文本
    const std::string& text = element->GetTag();
    if (!text.empty()) {
        RenderText(element, text, Vector2(checkBoxX + checkBoxSize + 5, bounds.y + bounds.height * 0.5f));
    }
}

void UIRenderer::RenderRadioButton(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    UIRect bounds = element->GetScreenBounds();
    
    // 渲染单选按钮
    float radioSize = 16.0f;
    float radioX = bounds.x + radioSize * 0.5f;
    float radioY = bounds.y + bounds.height * 0.5f;
    
    renderer->DrawCircle(radioX, radioY, radioSize * 0.5f, element->GetStyle().borderColor, 1.0f);
    
    // 如果选中，渲染内部圆点
    if (element->GetState() == UIState::Selected) {
        renderer->DrawFilledCircle(radioX, radioY, radioSize * 0.25f, element->GetStyle().foregroundColor);
    }
    
    // 渲染文本
    const std::string& text = element->GetTag();
    if (!text.empty()) {
        RenderText(element, text, Vector2(bounds.x + radioSize + 5, bounds.y + bounds.height * 0.5f));
    }
}

void UIRenderer::RenderSlider(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    UIRect bounds = element->GetScreenBounds();
    
    // 渲染滑轨
    float trackY = bounds.y + bounds.height * 0.5f;
    renderer->DrawLine(bounds.x, trackY, bounds.x + bounds.width, trackY, element->GetStyle().borderColor, 2.0f);
    
    // 渲染滑块
    float sliderWidth = 10.0f;
    float sliderHeight = 20.0f;
    float sliderX = bounds.x; // 这里应该根据滑块的值来计算位置
    float sliderY = bounds.y + (bounds.height - sliderHeight) * 0.5f;
    
    renderer->DrawFilledRectangle(sliderX, sliderY, sliderWidth, sliderHeight, element->GetStyle().foregroundColor);
}

void UIRenderer::RenderProgressBar(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    UIRect bounds = element->GetScreenBounds();
    
    // 渲染背景
    renderer->DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, element->GetStyle().borderColor, 1.0f);
    
    // 渲染进度
    float progress = 0.5f; // 这里应该从元素数据中获取实际进度值
    float progressWidth = bounds.width * progress;
    renderer->DrawFilledRectangle(bounds.x, bounds.y, progressWidth, bounds.height, element->GetStyle().foregroundColor);
}

void UIRenderer::RenderComboBox(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    UIRect bounds = element->GetScreenBounds();
    
    // 渲染背景
    RenderBackground(element);
    
    // 渲染边框
    RenderBorder(element);
    
    // 渲染文本
    const std::string& text = element->GetTag();
    if (!text.empty()) {
        RenderText(element, text, Vector2(bounds.x + 5, bounds.y + bounds.height * 0.5f));
    }
    
    // 渲染下拉箭头
    float arrowSize = 8.0f;
    float arrowX = bounds.x + bounds.width - arrowSize - 5;
    float arrowY = bounds.y + (bounds.height - arrowSize) * 0.5f;
    
    renderer->DrawLine(arrowX, arrowY, arrowX + arrowSize * 0.5f, arrowY + arrowSize, element->GetStyle().foregroundColor, 1.0f);
    renderer->DrawLine(arrowX + arrowSize * 0.5f, arrowY + arrowSize, arrowX + arrowSize, arrowY, element->GetStyle().foregroundColor, 1.0f);
}

void UIRenderer::RenderListBox(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    // 渲染背景
    RenderBackground(element);
    
    // 渲染边框
    RenderBorder(element);
    
    // 渲染列表项
    // 这里应该遍历列表项并渲染每一项
}

void UIRenderer::RenderTabControl(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    // 渲染标签页
    // 这里应该遍历标签页并渲染每一个标签
}

void UIRenderer::RenderWindow(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    // 渲染背景
    RenderBackground(element);
    
    // 渲染边框
    RenderBorder(element);
    
    // 渲染标题栏
    UIRect bounds = element->GetScreenBounds();
    float titleBarHeight = 30.0f;
    renderer->DrawFilledRectangle(bounds.x, bounds.y, bounds.width, titleBarHeight, Color(100, 100, 100, 255));
    
    // 渲染标题文本
    const std::string& text = element->GetTag();
    if (!text.empty()) {
        RenderText(element, text, Vector2(bounds.x + 10, bounds.y + titleBarHeight * 0.5f));
    }
    
    // 渲染关闭按钮
    float closeButtonSize = 20.0f;
    float closeButtonX = bounds.x + bounds.width - closeButtonSize - 5;
    float closeButtonY = bounds.y + (titleBarHeight - closeButtonSize) * 0.5f;
    renderer->DrawRectangle(closeButtonX, closeButtonY, closeButtonSize, closeButtonSize, Color::Red(), 1.0f);
    renderer->DrawLine(closeButtonX + 5, closeButtonY + 5, closeButtonX + closeButtonSize - 5, closeButtonY + closeButtonSize - 5, Color::Red(), 1.0f);
    renderer->DrawLine(closeButtonX + closeButtonSize - 5, closeButtonY + 5, closeButtonX + 5, closeButtonY + closeButtonSize - 5, Color::Red(), 1.0f);
}

void UIRenderer::RenderMenu(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    // 渲染背景
    RenderBackground(element);
    
    // 渲染边框
    RenderBorder(element);
    
    // 渲染菜单项
    // 这里应该遍历菜单项并渲染每一项
}

void UIRenderer::RenderMenuItem(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    UIRect bounds = element->GetScreenBounds();
    
    // 如果是悬停状态，渲染高亮背景
    if (element->GetState() == UIState::Hover) {
        renderer->DrawFilledRectangle(bounds.x, bounds.y, bounds.width, bounds.height, Color(200, 200, 200, 255));
    }
    
    // 渲染文本
    const std::string& text = element->GetTag();
    if (!text.empty()) {
        RenderText(element, text, Vector2(bounds.x + 5, bounds.y + bounds.height * 0.5f));
    }
}

void UIRenderer::RenderImage(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    UIRect bounds = element->GetScreenBounds();
    
    // 渲染图像
    // 这里应该从元素数据中获取图像并渲染
    renderer->DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, element->GetStyle().borderColor, 1.0f);
}

void UIRenderer::RenderBackground(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    UIRect bounds = element->GetScreenBounds();
    const UIStyle& style = element->GetStyle();
    
    if (style.cornerRadius > 0) {
        // 渲染圆角矩形
        renderer->DrawFilledRectangle(bounds.x, bounds.y, bounds.width, bounds.height, style.backgroundColor);
    } else {
        // 渲染普通矩形
        renderer->DrawFilledRectangle(bounds.x, bounds.y, bounds.width, bounds.height, style.backgroundColor);
    }
}

void UIRenderer::RenderBorder(std::shared_ptr<UIElement> element) {
    if (!element || !renderer) return;
    
    const UIStyle& style = element->GetStyle();
    if (style.borderWidth <= 0) return;
    
    UIRect bounds = element->GetScreenBounds();
    
    if (style.cornerRadius > 0) {
        // 渲染圆角矩形边框
        renderer->DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, style.borderColor, style.borderWidth);
    } else {
        // 渲染普通矩形边框
        renderer->DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, style.borderColor, style.borderWidth);
    }
}

void UIRenderer::RenderText(std::shared_ptr<UIElement> element, const std::string& text, const Vector2& position) {
    if (!element || !renderer || text.empty()) return;
    
    const UIStyle& style = element->GetStyle();
    Color textColor = GetStateColor(element, style.foregroundColor);
    
    // 根据对齐方式调整文本位置
    Vector2 textPos = position;
    Vector2 textSize = CalculateTextSize(text, style.font);
    
    switch (style.horizontalAlignment) {
        case UIAlignment::Center:
            textPos.x -= textSize.x * 0.5f;
            break;
        case UIAlignment::Right:
            textPos.x -= textSize.x;
            break;
        default:
            break;
    }
    
    switch (style.verticalAlignment) {
        case UIAlignment::Center:
            textPos.y -= textSize.y * 0.5f;
            break;
        case UIAlignment::Bottom:
            textPos.y -= textSize.y;
            break;
        default:
            break;
    }
    
    // 渲染文本
    renderer->DrawText(text.c_str(), textPos.x, textPos.y, textColor, style.font);
}

Vector2 UIRenderer::CalculateTextSize(const std::string& text, const Font& font) {
    if (!renderer) return Vector2(0, 0);
    
    // 这里应该使用渲染器的文本计算功能
    // 暂时返回一个估算值
    return Vector2(text.length() * 8.0f, 16.0f);
}

Color UIRenderer::GetStateColor(std::shared_ptr<UIElement> element, const Color& normalColor) {
    if (!element) return normalColor;
    
    switch (element->GetState()) {
        case UIState::Disabled:
            return Color(normalColor.r * 0.5f, normalColor.g * 0.5f, normalColor.b * 0.5f, normalColor.a);
        case UIState::Hover:
            return Color(normalColor.r * 0.8f, normalColor.g * 0.8f, normalColor.b * 0.9f, normalColor.a);
        case UIState::Pressed:
            return Color(normalColor.r * 0.7f, normalColor.g * 0.7f, normalColor.b * 0.8f, normalColor.a);
        default:
            return normalColor;
    }
}

// UIManager实现

UIManager* UIManager::instance = nullptr;

UIManager* UIManager::GetInstance() {
    if (!instance) {
        instance = new UIManager();
    }
    return instance;
}

UIManager::UIManager() : uiRenderer(nullptr), mousePosition(0, 0), initialized(false) {
}

UIManager::~UIManager() {
    Cleanup();
}

void UIManager::Initialize(Renderer* renderer) {
    if (initialized) return;
    
    uiRenderer = new UIRenderer(renderer);
    initialized = true;
}

void UIManager::Cleanup() {
    if (!initialized) return;
    
    elements.clear();
    focusedElement.reset();
    hoveredElement.reset();
    pressedElement.reset();
    
    if (uiRenderer) {
        delete uiRenderer;
        uiRenderer = nullptr;
    }
    
    initialized = false;
}

void UIManager::Update(float deltaTime) {
    if (!initialized) return;
    
    // 更新所有UI元素
    for (const auto& element : elements) {
        element->Update(deltaTime);
    }
}

void UIManager::Render() {
    if (!initialized || !uiRenderer) return;
    
    // 渲染所有UI元素
    for (const auto& element : elements) {
        if (element->IsVisible()) {
            uiRenderer->RenderElement(element);
        }
    }
}

void UIManager::OnMouseDown(const Vector2& position, int button) {
    if (!initialized) return;
    
    mousePosition = position;
    
    // 查找鼠标下的元素
    auto element = GetElementAtPosition(position);
    if (element && element->IsEnabled()) {
        // 设置为按下状态
        element->OnMouseDown(position, button);
        pressedElement = element;
        
        // 设置焦点
        SetFocusedElement(element);
    }
}

void UIManager::OnMouseUp(const Vector2& position, int button) {
    if (!initialized) return;
    
    mousePosition = position;
    
    // 处理之前按下的元素
    auto pressed = pressedElement.lock();
    if (pressed) {
        pressed->OnMouseUp(position, button);
        pressedElement.reset();
    }
    
    // 查找鼠标下的元素
    auto element = GetElementAtPosition(position);
    if (element && element->IsEnabled()) {
        // 如果是同一个元素，触发点击事件
        if (element == pressed) {
            element->OnClick(position);
        }
    }
}

void UIManager::OnMouseMove(const Vector2& position) {
    if (!initialized) return;
    
    mousePosition = position;
    
    // 查找鼠标下的元素
    auto element = GetElementAtPosition(position);
    
    // 处理之前悬停的元素
    auto hovered = hoveredElement.lock();
    if (hovered && hovered != element) {
        hovered->OnMouseLeave(position);
        hoveredElement.reset();
    }
    
    // 处理当前悬停的元素
    if (element && element->IsEnabled()) {
        if (element != hovered) {
            element->OnMouseEnter(position);
            hoveredElement = element;
        }
    }
}

void UIManager::OnMouseWheel(const Vector2& position, int delta) {
    if (!initialized) return;
    
    // 查找鼠标下的元素
    auto element = GetElementAtPosition(position);
    if (element && element->IsEnabled()) {
        // 可以添加鼠标滚轮事件处理
    }
}

void UIManager::OnKeyPress(int keyCode, bool shift, bool ctrl, bool alt) {
    if (!initialized) return;
    
    // 发送给焦点元素
    auto focused = focusedElement.lock();
    if (focused && focused->IsEnabled()) {
        focused->OnKeyPress(keyCode, shift, ctrl, alt);
    }
}

void UIManager::OnKeyRelease(int keyCode, bool shift, bool ctrl, bool alt) {
    if (!initialized) return;
    
    // 发送给焦点元素
    auto focused = focusedElement.lock();
    if (focused && focused->IsEnabled()) {
        focused->OnKeyRelease(keyCode, shift, ctrl, alt);
    }
}

void UIManager::AddElement(std::shared_ptr<UIElement> element) {
    if (!element) return;
    
    elements.push_back(element);
}

void UIManager::RemoveElement(std::shared_ptr<UIElement> element) {
    if (!element) return;
    
    auto it = std::find(elements.begin(), elements.end(), element);
    if (it != elements.end()) {
        elements.erase(it);
    }
    
    // 如果是焦点元素，清除焦点
    if (focusedElement.lock() == element) {
        focusedElement.reset();
    }
    
    // 如果是悬停元素，清除悬停
    if (hoveredElement.lock() == element) {
        hoveredElement.reset();
    }
    
    // 如果是按下元素，清除按下
    if (pressedElement.lock() == element) {
        pressedElement.reset();
    }
}

void UIManager::RemoveElement(const std::string& elementID) {
    auto it = std::find_if(elements.begin(), elements.end(), 
                          [&elementID](const std::shared_ptr<UIElement>& element) {
                              return element->GetID() == elementID;
                          });
    
    if (it != elements.end()) {
        RemoveElement(*it);
    }
}

std::shared_ptr<UIElement> UIManager::FindElement(const std::string& elementID) const {
    auto it = std::find_if(elements.begin(), elements.end(), 
                          [&elementID](const std::shared_ptr<UIElement>& element) {
                              return element->GetID() == elementID;
                          });
    
    return (it != elements.end()) ? *it : nullptr;
}

std::shared_ptr<UIElement> UIManager::FindElementRecursive(const std::string& elementID) const {
    // 先在顶级元素中查找
    auto element = FindElement(elementID);
    if (element) return element;
    
    // 递归在子元素中查找
    for (const auto& e : elements) {
        element = e->FindChildRecursive(elementID);
        if (element) return element;
    }
    
    return nullptr;
}

std::shared_ptr<UIElement> UIManager::GetElementAtPosition(const Vector2& position) const {
    // 从后往前遍历，因为后添加的元素在上层
    for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
        auto element = *it;
        if (element->IsVisible() && element->ContainsPoint(position)) {
            // 检查子元素
            auto childElement = GetChildElementAtPosition(element, position);
            return childElement ? childElement : element;
        }
    }
    
    return nullptr;
}

std::shared_ptr<UIElement> UIManager::GetChildElementAtPosition(std::shared_ptr<UIElement> parent, const Vector2& position) const {
    if (!parent) return nullptr;
    
    // 从后往前遍历子元素
    const auto& children = parent->GetChildren();
    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        auto child = *it;
        if (child->IsVisible() && child->ContainsPoint(position)) {
            // 递归检查子元素的子元素
            auto grandChild = GetChildElementAtPosition(child, position);
            return grandChild ? grandChild : child;
        }
    }
    
    return nullptr;
}

void UIManager::SetFocusedElement(std::shared_ptr<UIElement> element) {
    auto currentFocused = focusedElement.lock();
    if (currentFocused == element) return;
    
    // 清除当前焦点
    if (currentFocused) {
        currentFocused->OnBlur();
    }
    
    // 设置新焦点
    focusedElement = element;
    if (element) {
        element->OnFocus();
    }
}