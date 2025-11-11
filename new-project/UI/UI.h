#pragma once

#include "../Renderer/Renderer.h"
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <unordered_map>
#include "../Utils/Vector.h"
#include "../Utils/Math.h"

// 前向声明
class UIElement;
class UIRenderer;
class UIManager;

// UI元素类型枚举
enum class UIElementType {
    Unknown,
    Panel,
    Button,
    Label,
    TextBox,
    CheckBox,
    RadioButton,
    Slider,
    ProgressBar,
    ComboBox,
    ListBox,
    TabControl,
    Window,
    Menu,
    MenuItem,
    Image,
    Custom
};

// UI对齐方式枚举
enum class UIAlignment {
    None,
    Left,
    Right,
    Center,
    Top,
    Bottom,
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

// UI状态枚举
enum class UIState {
    Normal,
    Hover,
    Pressed,
    Disabled,
    Selected,
    Focused
};

// UI事件类型枚举
enum class UIEventType {
    MouseEnter,
    MouseLeave,
    MouseDown,
    MouseUp,
    Click,
    DoubleClick,
    KeyPress,
    KeyRelease,
    Focus,
    Blur,
    ValueChange,
    Resize,
    Move,
    Show,
    Hide,
    Custom
};

// UI事件结构体
struct UIEvent {
    UIEventType type;
    std::shared_ptr<UIElement> source;
    Vector2 mousePosition;
    int keyCode;
    bool shiftPressed;
    bool ctrlPressed;
    bool altPressed;
    void* data;
    
    UIEvent(UIEventType _type, std::shared_ptr<UIElement> _source) 
        : type(_type), source(_source), mousePosition(0, 0), keyCode(0), 
          shiftPressed(false), ctrlPressed(false), altPressed(false), data(nullptr) {}
};

// UI事件回调函数类型
using UIEventCallback = std::function<void(const UIEvent&)>;

// UI边距结构体
struct UIMargin {
    float left;
    float top;
    float right;
    float bottom;
    
    UIMargin() : left(0), top(0), right(0), bottom(0) {}
    UIMargin(float all) : left(all), top(all), right(all), bottom(all) {}
    UIMargin(float _left, float _top, float _right, float _bottom) 
        : left(_left), top(_top), right(_right), bottom(_bottom) {}
};

// UI大小结构体
struct UISize {
    float width;
    float height;
    
    UISize() : width(0), height(0) {}
    UISize(float _width, float _height) : width(_width), height(_height) {}
    
    bool IsEmpty() const { return width <= 0 || height <= 0; }
};

// UI矩形结构体
struct UIRect {
    float x;
    float y;
    float width;
    float height;
    
    UIRect() : x(0), y(0), width(0), height(0) {}
    UIRect(float _x, float _y, float _width, float _height) 
        : x(_x), y(_y), width(_width), height(_height) {}
    
    Vector2 GetPosition() const { return Vector2(x, y); }
    Vector2 GetSize() const { return Vector2(width, height); }
    Vector2 GetCenter() const { return Vector2(x + width * 0.5f, y + height * 0.5f); }
    
    bool Contains(const Vector2& point) const {
        return point.x >= x && point.x < x + width && 
               point.y >= y && point.y < y + height;
    }
    
    bool Intersects(const UIRect& rect) const {
        return !(x + width < rect.x || rect.x + rect.width < x || 
                 y + height < rect.y || rect.y + rect.height < y);
    }
    
    UIRect Inflate(float amount) const {
        return UIRect(x - amount, y - amount, width + amount * 2, height + amount * 2);
    }
    
    UIRect Deflate(float amount) const {
        return UIRect(x + amount, y + amount, width - amount * 2, height - amount * 2);
    }
};

// UI样式结构体
struct UIStyle {
    Color backgroundColor;
    Color foregroundColor;
    Color borderColor;
    float borderWidth;
    float cornerRadius;
    Font font;
    UIMargin margin;
    UIMargin padding;
    UIAlignment horizontalAlignment;
    UIAlignment verticalAlignment;
    
    UIStyle() : backgroundColor(Color::White()), foregroundColor(Color::Black()), 
                borderColor(Color::Gray()), borderWidth(1.0f), cornerRadius(0.0f),
                horizontalAlignment(UIAlignment::Left), verticalAlignment(UIAlignment::Top) {}
};

// UI元素基类
class UIElement : public std::enable_shared_from_this<UIElement> {
public:
    // 构造函数
    UIElement(UIElementType type = UIElementType::Unknown);
    
    // 析构函数
    virtual ~UIElement();
    
    // 获取元素类型
    UIElementType GetType() const { return type; }
    
    // 获取和设置ID
    const std::string& GetID() const { return id; }
    void SetID(const std::string& _id) { id = _id; }
    
    // 获取和设置父元素
    std::shared_ptr<UIElement> GetParent() const { return parent.lock(); }
    void SetParent(std::weak_ptr<UIElement> _parent) { parent = _parent; }
    
    // 获取子元素
    const std::vector<std::shared_ptr<UIElement>>& GetChildren() const { return children; }
    
    // 添加子元素
    void AddChild(std::shared_ptr<UIElement> child);
    
    // 移除子元素
    void RemoveChild(std::shared_ptr<UIElement> child);
    void RemoveChild(const std::string& childID);
    
    // 查找子元素
    std::shared_ptr<UIElement> FindChild(const std::string& childID) const;
    std::shared_ptr<UIElement> FindChildRecursive(const std::string& childID) const;
    
    // 获取和设置位置
    Vector2 GetPosition() const { return position; }
    void SetPosition(const Vector2& _position) { position = _position; OnPositionChanged(); }
    void SetPosition(float x, float y) { position = Vector2(x, y); OnPositionChanged(); }
    
    // 获取和设置大小
    Vector2 GetSize() const { return size; }
    void SetSize(const Vector2& _size) { size = _size; OnSizeChanged(); }
    void SetSize(float width, float height) { size = Vector2(width, height); OnSizeChanged(); }
    
    // 获取边界矩形
    UIRect GetBounds() const { return UIRect(position.x, position.y, size.x, size.y); }
    void SetBounds(const UIRect& bounds) { position = Vector2(bounds.x, bounds.y); size = Vector2(bounds.width, bounds.height); OnPositionChanged(); OnSizeChanged(); }
    
    // 获取和设置可见性
    bool IsVisible() const { return visible; }
    void SetVisible(bool _visible) { visible = _visible; OnVisibilityChanged(); }
    
    // 获取和设置启用状态
    bool IsEnabled() const { return enabled; }
    void SetEnabled(bool _enabled) { enabled = _enabled; OnEnabledChanged(); }
    
    // 获取和设置焦点状态
    bool IsFocused() const { return focused; }
    void SetFocused(bool _focused) { focused = _focused; OnFocusChanged(); }
    
    // 获取和设置样式
    const UIStyle& GetStyle() const { return style; }
    void SetStyle(const UIStyle& _style) { style = _style; OnStyleChanged(); }
    
    // 获取和设置状态
    UIState GetState() const { return state; }
    void SetState(UIState _state) { state = _state; OnStateChanged(); }
    
    // 获取和设置标签
    const std::string& GetTag() const { return tag; }
    void SetTag(const std::string& _tag) { tag = _tag; }
    
    // 获取和设置数据
    void* GetData() const { return data; }
    void SetData(void* _data) { data = _data; }
    
    // 检查点是否在元素内
    virtual bool ContainsPoint(const Vector2& point) const;
    
    // 获取元素的屏幕位置
    virtual Vector2 GetScreenPosition() const;
    
    // 获取元素的屏幕边界
    virtual UIRect GetScreenBounds() const;
    
    // 事件处理
    virtual void OnMouseEnter(const Vector2& position);
    virtual void OnMouseLeave(const Vector2& position);
    virtual void OnMouseDown(const Vector2& position, int button);
    virtual void OnMouseUp(const Vector2& position, int button);
    virtual void OnClick(const Vector2& position);
    virtual void OnDoubleClick(const Vector2& position);
    virtual void OnKeyPress(int keyCode, bool shift, bool ctrl, bool alt);
    virtual void OnKeyRelease(int keyCode, bool shift, bool ctrl, bool alt);
    virtual void OnFocus();
    virtual void OnBlur();
    virtual void OnValueChange();
    virtual void OnResize(const Vector2& oldSize, const Vector2& newSize);
    virtual void OnMove(const Vector2& oldPosition, const Vector2& newPosition);
    virtual void OnShow();
    virtual void OnHide();
    
    // 渲染
    virtual void Render(UIRenderer* renderer);
    
    // 更新
    virtual void Update(float deltaTime);
    
    // 事件回调管理
    void AddEventListener(UIEventType eventType, UIEventCallback callback);
    void RemoveEventListener(UIEventType eventType);
    void TriggerEvent(const UIEvent& event);
    
protected:
    // 事件回调
    virtual void OnPositionChanged() {}
    virtual void OnSizeChanged() {}
    virtual void OnVisibilityChanged() {}
    virtual void OnEnabledChanged() {}
    virtual void OnFocusChanged() {}
    virtual void OnStyleChanged() {}
    virtual void OnStateChanged() {}
    
    // 成员变量
    UIElementType type;
    std::string id;
    std::weak_ptr<UIElement> parent;
    std::vector<std::shared_ptr<UIElement>> children;
    Vector2 position;
    Vector2 size;
    bool visible;
    bool enabled;
    bool focused;
    UIStyle style;
    UIState state;
    std::string tag;
    void* data;
    
    // 事件回调
    std::unordered_map<UIEventType, UIEventCallback> eventCallbacks;
};

// UI渲染器类
class UIRenderer {
public:
    // 构造函数
    UIRenderer(Renderer* renderer);
    
    // 析构函数
    ~UIRenderer();
    
    // 渲染UI元素
    void RenderElement(std::shared_ptr<UIElement> element);
    
    // 渲染面板
    void RenderPanel(std::shared_ptr<UIElement> element);
    
    // 渲染按钮
    void RenderButton(std::shared_ptr<UIElement> element);
    
    // 渲染标签
    void RenderLabel(std::shared_ptr<UIElement> element);
    
    // 渲染文本框
    void RenderTextBox(std::shared_ptr<UIElement> element);
    
    // 渲染复选框
    void RenderCheckBox(std::shared_ptr<UIElement> element);
    
    // 渲染单选按钮
    void RenderRadioButton(std::shared_ptr<UIElement> element);
    
    // 渲染滑块
    void RenderSlider(std::shared_ptr<UIElement> element);
    
    // 渲染进度条
    void RenderProgressBar(std::shared_ptr<UIElement> element);
    
    // 渲染组合框
    void RenderComboBox(std::shared_ptr<UIElement> element);
    
    // 渲染列表框
    void RenderListBox(std::shared_ptr<UIElement> element);
    
    // 渲染标签页控件
    void RenderTabControl(std::shared_ptr<UIElement> element);
    
    // 渲染窗口
    void RenderWindow(std::shared_ptr<UIElement> element);
    
    // 渲染菜单
    void RenderMenu(std::shared_ptr<UIElement> element);
    
    // 渲染菜单项
    void RenderMenuItem(std::shared_ptr<UIElement> element);
    
    // 渲染图像
    void RenderImage(std::shared_ptr<UIElement> element);
    
    // 获取渲染器
    Renderer* GetRenderer() const { return renderer; }
    
private:
    // 渲染器
    Renderer* renderer;
    
    // 渲染背景
    void RenderBackground(std::shared_ptr<UIElement> element);
    
    // 渲染边框
    void RenderBorder(std::shared_ptr<UIElement> element);
    
    // 渲染文本
    void RenderText(std::shared_ptr<UIElement> element, const std::string& text, const Vector2& position);
    
    // 计算文本大小
    Vector2 CalculateTextSize(const std::string& text, const Font& font);
    
    // 获取状态颜色
    Color GetStateColor(std::shared_ptr<UIElement> element, const Color& normalColor);
};

// UI管理器类
class UIManager {
public:
    // 获取单例实例
    static UIManager* GetInstance();
    
    // 构造函数
    UIManager();
    
    // 析构函数
    ~UIManager();
    
    // 初始化
    void Initialize(Renderer* renderer);
    
    // 清理
    void Cleanup();
    
    // 更新
    void Update(float deltaTime);
    
    // 渲染
    void Render();
    
    // 处理鼠标输入
    void OnMouseDown(const Vector2& position, int button);
    void OnMouseUp(const Vector2& position, int button);
    void OnMouseMove(const Vector2& position);
    void OnMouseWheel(const Vector2& position, int delta);
    
    // 处理键盘输入
    void OnKeyPress(int keyCode, bool shift, bool ctrl, bool alt);
    void OnKeyRelease(int keyCode, bool shift, bool ctrl, bool alt);
    
    // 添加UI元素
    void AddElement(std::shared_ptr<UIElement> element);
    
    // 移除UI元素
    void RemoveElement(std::shared_ptr<UIElement> element);
    void RemoveElement(const std::string& elementID);
    
    // 查找UI元素
    std::shared_ptr<UIElement> FindElement(const std::string& elementID) const;
    std::shared_ptr<UIElement> FindElementRecursive(const std::string& elementID) const;
    
    // 获取所有UI元素
    const std::vector<std::shared_ptr<UIElement>>& GetElements() const { return elements; }
    
    // 获取鼠标下的元素
    std::shared_ptr<UIElement> GetElementAtPosition(const Vector2& position) const;
    
    // 设置焦点元素
    void SetFocusedElement(std::shared_ptr<UIElement> element);
    
    // 获取焦点元素
    std::shared_ptr<UIElement> GetFocusedElement() const { return focusedElement.lock(); }
    
    // 获取鼠标悬停的元素
    std::shared_ptr<UIElement> GetHoveredElement() const { return hoveredElement.lock(); }
    
    // 获取鼠标按下的元素
    std::shared_ptr<UIElement> GetPressedElement() const { return pressedElement.lock(); }
    
    // 获取鼠标位置
    Vector2 GetMousePosition() const { return mousePosition; }
    
    // 获取渲染器
    UIRenderer* GetUIRenderer() const { return uiRenderer; }
    
private:
    // 单例实例
    static UIManager* instance;
    
    // UI渲染器
    UIRenderer* uiRenderer;
    
    // UI元素列表
    std::vector<std::shared_ptr<UIElement>> elements;
    
    // 焦点元素
    std::weak_ptr<UIElement> focusedElement;
    
    // 鼠标悬停的元素
    std::weak_ptr<UIElement> hoveredElement;
    
    // 鼠标按下的元素
    std::weak_ptr<UIElement> pressedElement;
    
    // 鼠标位置
    Vector2 mousePosition;
    
    // 初始化标志
    bool initialized;
};