#pragma once

#include <string>
#include <vector>
#include <memory>
#include "../Utils/Vector.h"
#include "../Utils/Math.h"

// 颜色结构体
struct Color {
    float r, g, b, a;
    
    Color() : r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
    Color(float _r, float _g, float _b, float _a = 1.0f) : r(_r), g(_g), b(_b), a(_a) {}
    Color(uint32_t argb) {
        a = ((argb >> 24) & 0xFF) / 255.0f;
        r = ((argb >> 16) & 0xFF) / 255.0f;
        g = ((argb >> 8) & 0xFF) / 255.0f;
        b = (argb & 0xFF) / 255.0f;
    }
    
    uint32_t ToARGB() const {
        return 
            (static_cast<uint32_t>(a * 255.0f) << 24) |
            (static_cast<uint32_t>(r * 255.0f) << 16) |
            (static_cast<uint32_t>(g * 255.0f) << 8) |
            static_cast<uint32_t>(b * 255.0f);
    }
    
    static Color Red() { return Color(1.0f, 0.0f, 0.0f); }
    static Color Green() { return Color(0.0f, 1.0f, 0.0f); }
    static Color Blue() { return Color(0.0f, 0.0f, 1.0f); }
    static Color White() { return Color(1.0f, 1.0f, 1.0f); }
    static Color Black() { return Color(0.0f, 0.0f, 0.0f); }
    static Color Yellow() { return Color(1.0f, 1.0f, 0.0f); }
    static Color Cyan() { return Color(0.0f, 1.0f, 1.0f); }
    static Color Magenta() { return Color(1.0f, 0.0f, 1.0f); }
    static Color Orange() { return Color(1.0f, 0.5f, 0.0f); }
    static Color Purple() { return Color(0.5f, 0.0f, 1.0f); }
    static Color Gray() { return Color(0.5f, 0.5f, 0.5f); }
    static Color LightGray() { return Color(0.75f, 0.75f, 0.75f); }
    static Color DarkGray() { return Color(0.25f, 0.25f, 0.25f); }
};

// 字体结构体
struct Font {
    std::string name;
    int size;
    bool bold;
    bool italic;
    void* fontHandle; // 跨平台字体句柄
    
    Font() : size(12), bold(false), italic(false), fontHandle(nullptr) {}
    Font(const std::string& _name, int _size, bool _bold = false, bool _italic = false) 
        : name(_name), size(_size), bold(_bold), italic(_italic), fontHandle(nullptr) {}
};

// 渲染器类
class Renderer {
public:
    // 获取单例实例
    static Renderer* GetInstance();
    
    // 初始化渲染器
    bool Initialize();
    
    // 清理渲染器
    void Shutdown();
    
    // 开始渲染
    void Begin();
    
    // 结束渲染
    void End();
    
    // 清除屏幕
    void Clear(const Color& color = Color(0.1f, 0.1f, 0.1f, 1.0f));
    
    // 绘制线条
    void DrawLine(const Vector2& start, const Vector2& end, const Color& color, float thickness = 1.0f);
    void DrawLine(float x1, float y1, float x2, float y2, const Color& color, float thickness = 1.0f);
    
    // 绘制矩形
    void DrawRectangle(const Vector2& position, const Vector2& size, const Color& color, float thickness = 1.0f);
    void DrawRectangle(float x, float y, float width, float height, const Color& color, float thickness = 1.0f);
    void DrawFilledRectangle(const Vector2& position, const Vector2& size, const Color& color);
    void DrawFilledRectangle(float x, float y, float width, float height, const Color& color);
    
    // 绘制圆形
    void DrawCircle(const Vector2& center, float radius, const Color& color, float thickness = 1.0f, int segments = 32);
    void DrawCircle(float x, float y, float radius, const Color& color, float thickness = 1.0f, int segments = 32);
    void DrawFilledCircle(const Vector2& center, float radius, const Color& color, int segments = 32);
    void DrawFilledCircle(float x, float y, float radius, const Color& color, int segments = 32);
    
    // 绘制椭圆
    void DrawEllipse(const Vector2& center, float radiusX, float radiusY, const Color& color, float thickness = 1.0f, int segments = 32);
    void DrawEllipse(float x, float y, float radiusX, float radiusY, const Color& color, float thickness = 1.0f, int segments = 32);
    void DrawFilledEllipse(const Vector2& center, float radiusX, float radiusY, const Color& color, int segments = 32);
    void DrawFilledEllipse(float x, float y, float radiusX, float radiusY, const Color& color, int segments = 32);
    
    // 绘制三角形
    void DrawTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3, const Color& color, float thickness = 1.0f);
    void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Color& color, float thickness = 1.0f);
    void DrawFilledTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3, const Color& color);
    void DrawFilledTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Color& color);
    
    // 绘制多边形
    void DrawPolygon(const std::vector<Vector2>& points, const Color& color, float thickness = 1.0f);
    void DrawFilledPolygon(const std::vector<Vector2>& points, const Color& color);
    
    // 绘制文本
    void DrawText(const std::string& text, const Vector2& position, const Color& color, const Font& font = Font());
    void DrawText(const std::string& text, float x, float y, const Color& color, const Font& font = Font());
    
    // 绘制带阴影的文本
    void DrawTextShadow(const std::string& text, const Vector2& position, const Color& color, const Color& shadowColor, const Font& font = Font());
    void DrawTextShadow(const std::string& text, float x, float y, const Color& color, const Color& shadowColor, const Font& font = Font());
    
    // 绘制带轮廓的文本
    void DrawTextOutline(const std::string& text, const Vector2& position, const Color& color, const Color& outlineColor, const Font& font = Font());
    void DrawTextOutline(const std::string& text, float x, float y, const Color& color, const Color& outlineColor, const Font& font = Font());
    
    // 绘制图像
    void DrawImage(void* texture, const Vector2& position, const Vector2& size = Vector2(0, 0));
    void DrawImage(void* texture, float x, float y, float width = 0, float height = 0);
    void DrawImage(void* texture, const Vector2& destPos, const Vector2& destSize, const Vector2& srcPos, const Vector2& srcSize);
    void DrawImage(void* texture, float destX, float destY, float destWidth, float destHeight, float srcX, float srcY, float srcWidth, float srcHeight);
    
    // 绘制圆角矩形
    void DrawRoundedRectangle(const Vector2& position, const Vector2& size, float radius, const Color& color, float thickness = 1.0f);
    void DrawRoundedRectangle(float x, float y, float width, float height, float radius, const Color& color, float thickness = 1.0f);
    void DrawFilledRoundedRectangle(const Vector2& position, const Vector2& size, float radius, const Color& color);
    void DrawFilledRoundedRectangle(float x, float y, float width, float height, float radius, const Color& color);
    
    // 创建字体
    void* CreateFont(const char* fontName, int size, bool bold = false, bool italic = false);
    
    // 创建纹理
    void* CreateTexture(const std::string& filePath);
    void* CreateTextureFromMemory(const void* data, size_t size);
    void* CreateTextureFromResource(void* module, const char* resourceType, const char* resourceName);
    
    // 获取屏幕尺寸
    Vector2 GetScreenSize() const;
    
    // 世界坐标转屏幕坐标
    Vector2 WorldToScreen(const Vector3& worldPos);
    
    // 屏幕坐标转世界坐标
    Vector3 ScreenToWorld(const Vector2& screenPos, float z = 0.0f);
    
    // 检查点是否在屏幕内
    bool IsOnScreen(const Vector2& position) const;
    bool IsOnScreen(float x, float y) const;
    
    // 获取渲染状态
    bool IsInitialized() const { return initialized; }
    
private:
    // 构造函数和析构函数
    Renderer();
    ~Renderer();
    
    // 禁用拷贝构造和赋值
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    
    // 初始化渲染资源
    bool InitializeResources();
    
    // 清理渲染资源
    void CleanupResources();
    
    // 创建顶点缓冲区
    bool CreateVertexBuffer();
    
    // 绘制原始图元
    void DrawPrimitive(int primitiveType, unsigned int primitiveCount, const void* vertices, unsigned int vertexCount, unsigned int vertexStride);
    
    // 设置渲染状态
    void SetRenderStates();
    
    // 恢复渲染状态
    void RestoreRenderStates();
    
    // 单例实例
    static Renderer* instance;
    
    // 渲染资源
    void* renderDevice;
    void* vertexBuffer;
    void* gradientTexture;
    
    // 渲染状态
    bool initialized;
    bool rendering;
    
    // 保存的渲染状态
    void* savedRenderStates;
    void* savedViewMatrix;
    void* savedProjectionMatrix;
    void* savedWorldMatrix;
    
    // 屏幕尺寸
    int screenWidth;
    int screenHeight;
};

// 顶点结构体
struct Vertex {
    float x, y, z;
    uint32_t color;
    float u, v;
    
    Vertex() : x(0), y(0), z(0), color(0xFFFFFFFF), u(0), v(0) {}
    Vertex(float _x, float _y, uint32_t _color = 0xFFFFFFFF, float _u = 0, float _v = 0) 
        : x(_x), y(_y), z(0), color(_color), u(_u), v(_v) {}
    Vertex(float _x, float _y, float _z, uint32_t _color = 0xFFFFFFFF, float _u = 0, float _v = 0) 
        : x(_x), y(_y), z(_z), color(_color), u(_u), v(_v) {}
};

// 跨平台顶点格式定义
// 具体格式取决于使用的图形API