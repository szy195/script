#include "Renderer.h"
#include "../Core/MemoryManager.h"
#include "../Core/GameAPI.h"

#include <iostream>
#include <cstring>

// 单例实例
Renderer* Renderer::instance = nullptr;

// 获取单例实例
Renderer* Renderer::GetInstance() {
    if (!instance) {
        instance = new Renderer();
    }
    return instance;
}

// 构造函数
Renderer::Renderer() : renderDevice(nullptr), vertexBuffer(nullptr), gradientTexture(nullptr), 
                       initialized(false), rendering(false), screenWidth(0), screenHeight(0),
                       savedRenderStates(nullptr), savedViewMatrix(nullptr), 
                       savedProjectionMatrix(nullptr), savedWorldMatrix(nullptr) {
    // 跨平台初始化
}

// 析构函数
Renderer::~Renderer() {
    Shutdown();
}

// 初始化渲染资源
bool Renderer::InitializeResources() {
    if (!initialized) {
        return false;
    }
    
    // 跨平台渲染资源初始化
    // 这里应该初始化具体的图形API资源
    
    return true;
}

// 初始化渲染器
bool Renderer::Initialize() {
    if (initialized) {
        return false;
    }
    
    // 获取屏幕尺寸
    screenWidth = 1920; // 默认值，实际应从系统获取
    screenHeight = 1080; // 默认值，实际应从系统获取
    
    // 初始化渲染资源
    if (!InitializeResources()) {
        Shutdown();
        return false;
    }
    
    initialized = true;
    return true;
}

// 清理渲染器
void Renderer::Shutdown() {
    if (!initialized) {
        return;
    }
    
    CleanupResources();
    
    if (renderDevice) {
        renderDevice = nullptr;
    }
    
    initialized = false;
}

// 清理渲染资源
void Renderer::CleanupResources() {
    if (!initialized) {
        return;
    }
    
    // 跨平台渲染资源清理
    // 这里应该释放具体的图形API资源
    
    // 清理保存的渲染状态
    if (savedRenderStates) {
        // 释放保存的渲染状态内存
        savedRenderStates = nullptr;
    }
    
    if (savedViewMatrix) {
        // 释放保存的视图矩阵内存
        savedViewMatrix = nullptr;
    }
    
    if (savedProjectionMatrix) {
        // 释放保存的投影矩阵内存
        savedProjectionMatrix = nullptr;
    }
    
    if (savedWorldMatrix) {
        // 释放保存的世界矩阵内存
        savedWorldMatrix = nullptr;
    }
}

// 清除屏幕
void Renderer::Clear(const Color& color) {
    if (!initialized) {
        return;
    }
    
    // 跨平台实现清除屏幕
    // 这里应该调用具体的图形API清除屏幕
}

// 开始渲染
void Renderer::Begin() {
    if (!initialized || rendering) {
        return;
    }
    
    rendering = true;
    
    // 保存当前渲染状态
    // 跨平台实现保存渲染状态
    
    // 设置新的渲染状态
    SetRenderStates();
}

// 结束渲染
void Renderer::End() {
    if (!initialized || !rendering) {
        return;
    }
    
    // 恢复渲染状态
    RestoreRenderStates();
    
    rendering = false;
}

// 绘制线条
void Renderer::DrawLine(const Vector2& start, const Vector2& end, const Color& color, float thickness) {
    if (!initialized || !rendering) {
        return;
    }
    
    // 创建线条顶点
    Vector2 direction = (end - start).Normalized();
    Vector2 normal = Vector2(-direction.y, direction.x);
    
    Vector2 p1 = start - normal * (thickness * 0.5f);
    Vector2 p2 = start + normal * (thickness * 0.5f);
    Vector2 p3 = end + normal * (thickness * 0.5f);
    Vector2 p4 = end - normal * (thickness * 0.5f);
    
    // 创建顶点
    Vertex vertices[4] = {
        Vertex(p1.x, p1.y, color.ToARGB()),
        Vertex(p2.x, p2.y, color.ToARGB()),
        Vertex(p3.x, p3.y, color.ToARGB()),
        Vertex(p4.x, p4.y, color.ToARGB())
    };
    
    // 绘制矩形
    DrawPrimitive(5, 2, vertices, 4, sizeof(Vertex));
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, const Color& color, float thickness) {
    DrawLine(Vector2(x1, y1), Vector2(x2, y2), color, thickness);
}

// 绘制矩形
void Renderer::DrawRectangle(const Vector2& position, const Vector2& size, const Color& color, float thickness) {
    if (!initialized || !rendering) {
        return;
    }
    
    // 绘制四条边
    Vector2 p1 = position;
    Vector2 p2 = position + Vector2(size.x, 0);
    Vector2 p3 = position + size;
    Vector2 p4 = position + Vector2(0, size.y);
    
    DrawLine(p1, p2, color, thickness);
    DrawLine(p2, p3, color, thickness);
    DrawLine(p3, p4, color, thickness);
    DrawLine(p4, p1, color, thickness);
}

void Renderer::DrawRectangle(float x, float y, float width, float height, const Color& color, float thickness) {
    DrawRectangle(Vector2(x, y), Vector2(width, height), color, thickness);
}

void Renderer::DrawFilledRectangle(const Vector2& position, const Vector2& size, const Color& color) {
    if (!initialized || !rendering) {
        return;
    }
    
    // 创建顶点
    Vertex vertices[4] = {
        Vertex(position.x, position.y, color.ToARGB()),
        Vertex(position.x + size.x, position.y, color.ToARGB()),
        Vertex(position.x + size.x, position.y + size.y, color.ToARGB()),
        Vertex(position.x, position.y + size.y, color.ToARGB())
    };
    
    // 绘制矩形
    DrawPrimitive(5, 2, vertices, 4, sizeof(Vertex));
}

void Renderer::DrawFilledRectangle(float x, float y, float width, float height, const Color& color) {
    DrawFilledRectangle(Vector2(x, y), Vector2(width, height), color);
}

// 绘制圆形
void Renderer::DrawCircle(const Vector2& center, float radius, const Color& color, float thickness, int segments) {
    if (!initialized || !rendering || segments < 3) {
        return;
    }
    
    // 计算角度步长
    float angleStep = Math::TWO_PI / segments;
    
    // 绘制圆的边
    for (int i = 0; i < segments; ++i) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        
        Vector2 p1 = center + Vector2(cos(angle1) * radius, sin(angle1) * radius);
        Vector2 p2 = center + Vector2(cos(angle2) * radius, sin(angle2) * radius);
        
        DrawLine(p1, p2, color, thickness);
    }
}

void Renderer::DrawCircle(float x, float y, float radius, const Color& color, float thickness, int segments) {
    DrawCircle(Vector2(x, y), radius, color, thickness, segments);
}

void Renderer::DrawFilledCircle(const Vector2& center, float radius, const Color& color, int segments) {
    if (!initialized || !rendering || segments < 3) {
        return;
    }
    
    // 计算角度步长
    float angleStep = Math::TWO_PI / segments;
    
    // 创建顶点数组
    std::vector<Vertex> vertices(segments + 1);
    
    // 中心点
    vertices[0] = Vertex(center.x, center.y, color.ToARGB());
    
    // 圆周上的点
    for (int i = 0; i < segments; ++i) {
        float angle = i * angleStep;
        float x = center.x + cos(angle) * radius;
        float y = center.y + sin(angle) * radius;
        vertices[i + 1] = Vertex(x, y, color.ToARGB());
    }
    
    // 绘制三角形扇形
    DrawPrimitive(5, segments, vertices.data(), segments + 1, sizeof(Vertex));
}

void Renderer::DrawFilledCircle(float x, float y, float radius, const Color& color, int segments) {
    DrawFilledCircle(Vector2(x, y), radius, color, segments);
}

// 绘制椭圆
void Renderer::DrawEllipse(const Vector2& center, float radiusX, float radiusY, const Color& color, float thickness, int segments) {
    if (!initialized || !rendering || segments < 3) {
        return;
    }
    
    // 计算角度步长
    float angleStep = Math::TWO_PI / segments;
    
    // 绘制椭圆的边
    for (int i = 0; i < segments; ++i) {
        float angle1 = i * angleStep;
        float angle2 = (i + 1) * angleStep;
        
        Vector2 p1 = center + Vector2(cos(angle1) * radiusX, sin(angle1) * radiusY);
        Vector2 p2 = center + Vector2(cos(angle2) * radiusX, sin(angle2) * radiusY);
        
        DrawLine(p1, p2, color, thickness);
    }
}

void Renderer::DrawEllipse(float x, float y, float radiusX, float radiusY, const Color& color, float thickness, int segments) {
    DrawEllipse(Vector2(x, y), radiusX, radiusY, color, thickness, segments);
}

void Renderer::DrawFilledEllipse(const Vector2& center, float radiusX, float radiusY, const Color& color, int segments) {
    if (!initialized || !rendering || segments < 3) {
        return;
    }
    
    // 计算角度步长
    float angleStep = Math::TWO_PI / segments;
    
    // 创建顶点数组
    std::vector<Vertex> vertices(segments + 1);
    
    // 中心点
    vertices[0] = Vertex(center.x, center.y, color.ToARGB());
    
    // 椭圆周上的点
    for (int i = 0; i < segments; ++i) {
        float angle = i * angleStep;
        float x = center.x + cos(angle) * radiusX;
        float y = center.y + sin(angle) * radiusY;
        vertices[i + 1] = Vertex(x, y, color.ToARGB());
    }
    
    // 绘制三角形扇形
    DrawPrimitive(5, segments, vertices.data(), segments + 1, sizeof(Vertex));
}

void Renderer::DrawFilledEllipse(float x, float y, float radiusX, float radiusY, const Color& color, int segments) {
    DrawFilledEllipse(Vector2(x, y), radiusX, radiusY, color, segments);
}

// 绘制三角形
void Renderer::DrawTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3, const Color& color, float thickness) {
    if (!initialized || !rendering) {
        return;
    }
    
    // 绘制三条边
    DrawLine(p1, p2, color, thickness);
    DrawLine(p2, p3, color, thickness);
    DrawLine(p3, p1, color, thickness);
}

void Renderer::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Color& color, float thickness) {
    DrawTriangle(Vector2(x1, y1), Vector2(x2, y2), Vector2(x3, y3), color, thickness);
}

void Renderer::DrawFilledTriangle(const Vector2& p1, const Vector2& p2, const Vector2& p3, const Color& color) {
    if (!initialized || !rendering) {
        return;
    }
    
    // 创建顶点
    Vertex vertices[3] = {
        Vertex(p1.x, p1.y, color.ToARGB()),
        Vertex(p2.x, p2.y, color.ToARGB()),
        Vertex(p3.x, p3.y, color.ToARGB())
    };
    
    // 绘制三角形
    DrawPrimitive(3, 1, vertices, 3, sizeof(Vertex));
}

void Renderer::DrawFilledTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Color& color) {
    DrawFilledTriangle(Vector2(x1, y1), Vector2(x2, y2), Vector2(x3, y3), color);
}

// 绘制多边形
void Renderer::DrawPolygon(const std::vector<Vector2>& points, const Color& color, float thickness) {
    if (!initialized || !rendering || points.size() < 3) {
        return;
    }
    
    // 绘制多边形的边
    for (size_t i = 0; i < points.size(); ++i) {
        size_t nextIndex = (i + 1) % points.size();
        DrawLine(points[i], points[nextIndex], color, thickness);
    }
}

void Renderer::DrawFilledPolygon(const std::vector<Vector2>& points, const Color& color) {
    if (!initialized || !rendering || points.size() < 3) {
        return;
    }
    
    // 创建顶点数组
    std::vector<Vertex> vertices(points.size());
    
    // 复制顶点
    for (size_t i = 0; i < points.size(); ++i) {
        vertices[i] = Vertex(points[i].x, points[i].y, color.ToARGB());
    }
    
    // 绘制三角形扇形
    DrawPrimitive(5, points.size() - 2, vertices.data(), points.size(), sizeof(Vertex));
}

// 绘制文本
void Renderer::DrawText(const std::string& text, const Vector2& position, const Color& color, const Font& font) {
    if (!initialized || !rendering || text.empty()) {
        return;
    }
    
    // 跨平台实现文本绘制
    // 这里应该调用具体的图形API绘制文本
    // text: 要绘制的文本
    // position: 文本位置
    // color: 文本颜色
    // font: 字体信息
}

void Renderer::DrawText(const std::string& text, float x, float y, const Color& color, const Font& font) {
    DrawText(text, Vector2(x, y), color, font);
}

// 绘制带阴影的文本
void Renderer::DrawTextShadow(const std::string& text, const Vector2& position, const Color& color, const Color& shadowColor, const Font& font) {
    if (!initialized || !rendering || text.empty()) {
        return;
    }
    
    // 绘制阴影
    DrawText(text, position + Vector2(1, 1), shadowColor, font);
    
    // 绘制文本
    DrawText(text, position, color, font);
}

void Renderer::DrawTextShadow(const std::string& text, float x, float y, const Color& color, const Color& shadowColor, const Font& font) {
    DrawTextShadow(text, Vector2(x, y), color, shadowColor, font);
}

// 绘制带轮廓的文本
void Renderer::DrawTextOutline(const std::string& text, const Vector2& position, const Color& color, const Color& outlineColor, const Font& font) {
    if (!initialized || !rendering || text.empty()) {
        return;
    }
    
    // 绘制轮廓
    DrawText(text, position + Vector2(-1, -1), outlineColor, font);
    DrawText(text, position + Vector2(0, -1), outlineColor, font);
    DrawText(text, position + Vector2(1, -1), outlineColor, font);
    DrawText(text, position + Vector2(1, 0), outlineColor, font);
    DrawText(text, position + Vector2(1, 1), outlineColor, font);
    DrawText(text, position + Vector2(0, 1), outlineColor, font);
    DrawText(text, position + Vector2(-1, 1), outlineColor, font);
    DrawText(text, position + Vector2(-1, 0), outlineColor, font);
    
    // 绘制文本
    DrawText(text, position, color, font);
}

void Renderer::DrawTextOutline(const std::string& text, float x, float y, const Color& color, const Color& outlineColor, const Font& font) {
    DrawTextOutline(text, Vector2(x, y), color, outlineColor, font);
}

// 绘制图像
void Renderer::DrawImage(void* texture, const Vector2& position, const Vector2& size) {
    if (!initialized || !rendering || !texture) {
        return;
    }
    
    // 跨平台实现图像绘制
    // 这里应该调用具体的图形API绘制图像
    // texture: 纹理指针
    // position: 图像位置
    // size: 图像尺寸
}

void Renderer::DrawImage(void* texture, float x, float y, float width, float height) {
    DrawImage(texture, Vector2(x, y), Vector2(width, height));
}

void Renderer::DrawImage(void* texture, const Vector2& destPos, const Vector2& destSize, const Vector2& srcPos, const Vector2& srcSize) {
    if (!initialized || !rendering || !texture) {
        return;
    }
    
    // 跨平台实现图像绘制
    // 这里应该调用具体的图形API绘制图像
    // texture: 纹理指针
    // destPos: 目标位置
    // destSize: 目标尺寸
    // srcPos: 源位置
    // srcSize: 源尺寸
}

void Renderer::DrawImage(void* texture, float destX, float destY, float destWidth, float destHeight, float srcX, float srcY, float srcWidth, float srcHeight) {
    DrawImage(texture, Vector2(destX, destY), Vector2(destWidth, destHeight), Vector2(srcX, srcY), Vector2(srcWidth, srcHeight));
}

// 绘制圆角矩形
void Renderer::DrawRoundedRectangle(const Vector2& position, const Vector2& size, float radius, const Color& color, float thickness) {
    if (!initialized || !rendering) {
        return;
    }
    
    // 确保半径不超过矩形尺寸的一半
    float r = Math::Min(radius, Math::Min(size.x, size.y) * 0.5f);
    
    // 绘制四个角
    Vector2 topLeft = position + Vector2(r, r);
    Vector2 topRight = position + Vector2(size.x - r, r);
    Vector2 bottomLeft = position + Vector2(r, size.y - r);
    Vector2 bottomRight = position + Vector2(size.x - r, size.y - r);
    
    // 绘制四个角的圆弧
    DrawCircle(topLeft, r, color, thickness, 8);
    DrawCircle(topRight, r, color, thickness, 8);
    DrawCircle(bottomLeft, r, color, thickness, 8);
    DrawCircle(bottomRight, r, color, thickness, 8);
    
    // 绘制四条边
    DrawLine(topLeft + Vector2(r, 0), topRight + Vector2(-r, 0), color, thickness);
    DrawLine(topRight + Vector2(0, r), bottomRight + Vector2(0, -r), color, thickness);
    DrawLine(bottomRight + Vector2(-r, 0), bottomLeft + Vector2(r, 0), color, thickness);
    DrawLine(bottomLeft + Vector2(0, -r), topLeft + Vector2(0, r), color, thickness);
}

void Renderer::DrawRoundedRectangle(float x, float y, float width, float height, float radius, const Color& color, float thickness) {
    DrawRoundedRectangle(Vector2(x, y), Vector2(width, height), radius, color, thickness);
}

void Renderer::DrawFilledRoundedRectangle(const Vector2& position, const Vector2& size, float radius, const Color& color) {
    if (!initialized || !rendering) {
        return;
    }
    
    // 确保半径不超过矩形尺寸的一半
    float r = Math::Min(radius, Math::Min(size.x, size.y) * 0.5f);
    
    // 绘制中心矩形
    Vector2 centerPos = position + Vector2(r, 0);
    Vector2 centerSize = size - Vector2(r * 2, 0);
    DrawFilledRectangle(centerPos, centerSize, color);
    
    // 绘制左右两个矩形
    Vector2 leftPos = position + Vector2(0, r);
    Vector2 leftSize = Vector2(r, size.y - r * 2);
    DrawFilledRectangle(leftPos, leftSize, color);
    
    Vector2 rightPos = position + Vector2(size.x - r, r);
    DrawFilledRectangle(rightPos, leftSize, color);
    
    // 绘制四个角的圆角
    Vector2 topLeft = position + Vector2(r, r);
    Vector2 topRight = position + Vector2(size.x - r, r);
    Vector2 bottomLeft = position + Vector2(r, size.y - r);
    Vector2 bottomRight = position + Vector2(size.x - r, size.y - r);
    
    DrawFilledCircle(topLeft, r, color, 8);
    DrawFilledCircle(topRight, r, color, 8);
    DrawFilledCircle(bottomLeft, r, color, 8);
    DrawFilledCircle(bottomRight, r, color, 8);
}

void Renderer::DrawFilledRoundedRectangle(float x, float y, float width, float height, float radius, const Color& color) {
    DrawFilledRoundedRectangle(Vector2(x, y), Vector2(width, height), radius, color);
}

// 创建字体
void* Renderer::CreateFont(const char* fontName, int size, bool bold, bool italic) {
    if (!initialized) {
        return nullptr;
    }
    
    // 跨平台字体创建
    // 这里应该调用具体的图形API创建字体
    // fontName: 字体名称
    // size: 字体大小
    // bold: 是否粗体
    // italic: 是否斜体
    // 返回字体对象指针
    
    return nullptr; // 临时返回nullptr，需要在具体平台实现
}

// 创建纹理
void* Renderer::CreateTexture(const std::string& filePath) {
    if (!initialized) {
        return nullptr;
    }
    
    // 跨平台纹理创建
    // 这里应该调用具体的图形API从文件创建纹理
    // filePath: 纹理文件路径
    // 返回纹理对象指针
    
    return nullptr; // 临时返回nullptr，需要在具体平台实现
}

void* Renderer::CreateTexture(int width, int height, Color color) {
    if (!initialized) {
        return nullptr;
    }
    
    // 跨平台纹理创建
    // 这里应该调用具体的图形API创建纯色纹理
    // width: 纹理宽度
    // height: 纹理高度
    // color: 纹理颜色
    // 返回纹理对象指针
    
    return nullptr; // 临时返回nullptr，需要在具体平台实现
}

void* Renderer::CreateTextureFromMemory(const void* data, size_t size) {
    if (!initialized || !data || size == 0) {
        return nullptr;
    }
    
    // 跨平台纹理创建
    // 这里应该调用具体的图形API从内存创建纹理
    // data: 纹理数据
    // size: 数据大小
    // 返回纹理对象指针
    
    return nullptr; // 临时返回nullptr，需要在具体平台实现
}

void* Renderer::CreateTextureFromResource(void* hModule, const char* resourceType, const char* resourceName) {
    if (!initialized) {
        return nullptr;
    }
    
    // 跨平台纹理创建
    // 这里应该调用具体的图形API从资源创建纹理
    // hModule: 资源模块 (跨平台: void*类型)
    // resourceType: 资源类型
    // resourceName: 资源名称
    // 返回纹理对象指针
    
    return nullptr; // 临时返回nullptr，需要在具体平台实现
}

// 获取屏幕尺寸
Vector2 Renderer::GetScreenSize() const {
    return Vector2(static_cast<float>(screenWidth), static_cast<float>(screenHeight));
}

// 世界坐标转屏幕坐标
Vector2 Renderer::WorldToScreen(const Vector3& worldPos) {
    auto gameAPI = GameAPI::GetInstance();
    if (!gameAPI) {
        return Vector2::Zero;
    }
    
    return gameAPI->WorldToScreen(worldPos);
}

// 屏幕坐标转世界坐标
Vector3 Renderer::ScreenToWorld(const Vector2& screenPos, float z) {
    auto gameAPI = GameAPI::GetInstance();
    if (!gameAPI) {
        return Vector3::Zero;
    }
    
    return gameAPI->ScreenToWorld(screenPos, z);
}

// 检查点是否在屏幕内
bool Renderer::IsOnScreen(const Vector2& position) const {
    return position.x >= 0 && position.x < screenWidth && 
           position.y >= 0 && position.y < screenHeight;
}

bool Renderer::IsOnScreen(float x, float y) const {
    return IsOnScreen(Vector2(x, y));
}

// 初始化渲染资源
bool Renderer::InitializeRenderResources() {
    if (!initialized) {
        return false;
    }
    
    // 跨平台渲染资源初始化
    // 这里应该初始化具体的图形API资源
    
    return true;
}

// 清理渲染资源
void Renderer::CleanupRenderResources() {
    // 跨平台渲染资源清理
    // 这里应该清理具体的图形API资源
}

// 创建顶点缓冲区
bool Renderer::CreateVertexBuffer() {
    if (!initialized) {
        return false;
    }
    
    // 跨平台顶点缓冲区创建
    // 这里应该调用具体的图形API创建顶点缓冲区
    
    return true;
}

// 设置渲染状态
void Renderer::SetRenderStates() {
    if (!initialized) {
        return;
    }
    
    // 跨平台渲染状态设置
    // 这里应该调用具体的图形API设置渲染状态
}

// 恢复渲染状态
void Renderer::RestoreRenderStates() {
    if (!initialized) {
        return;
    }
    
    // 跨平台渲染状态恢复
    // 这里应该调用具体的图形API恢复渲染状态
}

// 绘制图元
void Renderer::DrawPrimitive(int primitiveType, unsigned int primitiveCount, const void* vertices, unsigned int vertexCount, unsigned int vertexStride) {
    if (!initialized || !rendering || !vertices) {
        return;
    }
    
    // 跨平台图元绘制
    // primitiveType: 图元类型 (3=三角形列表, 5=三角形扇形)
    // primitiveCount: 图元数量
    // vertices: 顶点数据
    // vertexCount: 顶点数量
    // vertexStride: 顶点步长
    
    // 这里应该调用具体的图形API绘制图元
}