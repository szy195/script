#pragma once

// 包含必要的C标准库头文件
#include <cmath>
#include <ctime>
#include <cstdlib>

// 包含Vector类的定义
#include "Vector.h"

// 使用C标准库中的数学函数
extern "C" {
    float sqrtf(float);
    float powf(float, float);
    float sinf(float);
    float cosf(float);
    float tanf(float);
    float asinf(float);
    float acosf(float);
    float atanf(float);
    float atan2f(float, float);
    float floorf(float);
    float ceilf(float);
    float roundf(float);
    float fmodf(float, float);
}

// 数学工具类
class Math {
public:
    // 数学常量
    static constexpr float PI = 3.14159265358979323846f;
    static constexpr float TWO_PI = 2.0f * PI;
    static constexpr float HALF_PI = PI / 2.0f;
    static constexpr float QUARTER_PI = PI / 4.0f;
    static constexpr float INV_PI = 1.0f / PI;
    static constexpr float DEG_TO_RAD = PI / 180.0f;
    static constexpr float RAD_TO_DEG = 180.0f / PI;
    static constexpr float EPSILON = 1e-6f;
    
    // 基础数学函数
    template<typename T>
    static T Min(T a, T b) {
        return a < b ? a : b;
    }
    
    template<typename T>
    static T Max(T a, T b) {
        return a > b ? a : b;
    }
    
    template<typename T>
    static T Clamp(T value, T min, T max) {
        return (value < min) ? min : (value > max) ? max : value;
    }
    
    template<typename T>
    static T Abs(T value) {
        return value < 0 ? -value : value;
    }
    
    static float Sqrt(float value) {
        return sqrtf(value);
    }
    
    static float Pow(float base, float exp) {
        return powf(base, exp);
    }
    
    static float Sin(float radians) {
        return sinf(radians);
    }
    
    static float Cos(float radians) {
        return cosf(radians);
    }
    
    static float Tan(float radians) {
        return tanf(radians);
    }
    
    static float Asin(float value) {
        return asinf(value);
    }
    
    static float Acos(float value) {
        return acosf(value);
    }
    
    static float Atan(float value) {
        return atanf(value);
    }
    
    static float Atan2(float y, float x) {
        return atan2f(y, x);
    }
    
    static float Floor(float value) {
        return floorf(value);
    }
    
    static float Ceil(float value) {
        return ceilf(value);
    }
    
    static float Round(float value) {
        return roundf(value);
    }
    
    template<typename T>
    static T Sign(T value) {
        return (value > 0) ? 1 : ((value < 0) ? -1 : 0);
    }
    
    template<typename T>
    static void Swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }
    
    // 简单的冒泡排序实现
    template<typename T>
    static void Sort(T* array, size_t size) {
        for (size_t i = 0; i < size - 1; ++i) {
            for (size_t j = 0; j < size - i - 1; ++j) {
                if (array[j] > array[j + 1]) {
                    Swap(array[j], array[j + 1]);
                }
            }
        }
    }
    
    // 简单数组操作
    template<typename T>
    static T Sum(T* array, size_t size) {
        T sum = 0;
        for (size_t i = 0; i < size; ++i) {
            sum += array[i];
        }
        return sum;
    }
    
    template<typename T>
    static T Average(T* array, size_t size) {
        if (size == 0) return 0;
        return Sum(array, size) / static_cast<T>(size);
    }
    
    // 浮点数比较
    static bool IsNearlyEqual(float a, float b, float tolerance = EPSILON) {
        return Abs(a - b) < tolerance;
    }
    
    static bool IsNearlyZero(float value, float tolerance = EPSILON) {
        return Abs(value) < tolerance;
    }
    
    // 角度转换
    static float DegreesToRadians(float degrees) {
        return degrees * DEG_TO_RAD;
    }
    
    static float RadiansToDegrees(float radians) {
        return radians * RAD_TO_DEG;
    }
    
    // 角度规范化
    static float NormalizeDegrees(float degrees) {
        degrees = fmodf(degrees, 360.0f);
        if (degrees < 0.0f) {
            degrees += 360.0f;
        }
        return degrees;
    }
    
    static float NormalizeRadians(float radians) {
        radians = fmodf(radians, TWO_PI);
        if (radians < 0.0f) {
            radians += TWO_PI;
        }
        return radians;
    }
    
    // 角度差值
    static float DegreesDifference(float a, float b) {
        float diff = NormalizeDegrees(b - a);
        if (diff > 180.0f) {
            diff -= 360.0f;
        }
        return diff;
    }
    
    static float RadiansDifference(float a, float b) {
        float diff = NormalizeRadians(b - a);
        if (diff > PI) {
            diff -= TWO_PI;
        }
        return diff;
    }
    
    // 线性插值
    template<typename T>
    static T Lerp(T a, T b, float t) {
        return a + (b - a) * Clamp(t, 0.0f, 1.0f);
    }
    
    // 反向线性插值
    template<typename T>
    static float InverseLerp(T a, T b, T value) {
        if (IsNearlyEqual(a, b)) {
            return 0.0f;
        }
        return Clamp((value - a) / (b - a), 0.0f, 1.0f);
    }
    
    // 平滑插值
    template<typename T>
    static T SmoothStep(T a, T b, float t) {
        t = Clamp(t, 0.0f, 1.0f);
        t = t * t * (3.0f - 2.0f * t);
        return Lerp(a, b, t);
    }
    
    // 随机数生成
    static float RandomFloat(float min = 0.0f, float max = 1.0f) {
        // 使用C标准库的rand函数
        float normalized = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        return min + normalized * (max - min);
    }
    
    static float RandomRange(float min, float max) {
        return RandomFloat(min, max);
    }
    
    static int RandomInt(int min = 0, int max = 100) {
        // 使用C标准库的rand函数
        return min + (rand() % (max - min + 1));
    }
    
    static Vector2 RandomVector2() {
        return Vector2(RandomFloat(), RandomFloat());
    }
    
    static Vector2 RandomVector2InRange(float min, float max) {
        return Vector2(RandomRange(min, max), RandomRange(min, max));
    }
    
    static Vector3 RandomVector3() {
        return Vector3(RandomFloat(), RandomFloat(), RandomFloat());
    }
    
    static Vector3 RandomVector3InRange(float min, float max) {
        return Vector3(RandomRange(min, max), RandomRange(min, max), RandomRange(min, max));
    }
    
    static Vector3 RandomDirection() {
        float theta = RandomRange(0.0f, TWO_PI);
        float phi = RandomRange(0.0f, PI);
        float sinPhi = Math::Sin(phi);
        
        return Vector3(
            sinPhi * Math::Cos(theta),
            sinPhi * Math::Sin(theta),
            Math::Cos(phi)
        );
    }
    
    // 距离计算
    static float PointToLineDistance(const Vector2& point, const Vector2& lineStart, const Vector2& lineEnd) {
        Vector2 lineVec = lineEnd - lineStart;
        Vector2 pointVec = point - lineStart;
        float lineLen = lineVec.Length();
        
        if (lineLen < EPSILON) {
            return pointVec.Length();
        }
        
        float t = Clamp(pointVec.DotProduct(lineVec) / (lineLen * lineLen), 0.0f, 1.0f);
        Vector2 projection = lineStart + lineVec * t;
        
        return point.DistanceTo(projection);
    }
    
    static float PointToLineDistance(const Vector3& point, const Vector3& lineStart, const Vector3& lineEnd) {
        Vector3 lineVec = lineEnd - lineStart;
        Vector3 pointVec = point - lineStart;
        float lineLen = lineVec.Length();
        
        if (lineLen < EPSILON) {
            return pointVec.Length();
        }
        
        float t = Clamp(pointVec.DotProduct(lineVec) / (lineLen * lineLen), 0.0f, 1.0f);
        Vector3 projection = lineStart + lineVec * t;
        
        return point.DistanceTo(projection);
    }
    
    static float PointToSegmentDistance(const Vector2& point, const Vector2& segmentStart, const Vector2& segmentEnd) {
        Vector2 segmentVec = segmentEnd - segmentStart;
        Vector2 pointVec = point - segmentStart;
        float segmentLen = segmentVec.Length();
        
        if (segmentLen < EPSILON) {
            return pointVec.Length();
        }
        
        float t = pointVec.DotProduct(segmentVec) / (segmentLen * segmentLen);
        
        if (t < 0.0f) {
            return point.DistanceTo(segmentStart);
        } else if (t > 1.0f) {
            return point.DistanceTo(segmentEnd);
        } else {
            Vector2 projection = segmentStart + segmentVec * t;
            return point.DistanceTo(projection);
        }
    }
    
    static float PointToSegmentDistance(const Vector3& point, const Vector3& segmentStart, const Vector3& segmentEnd) {
        Vector3 segmentVec = segmentEnd - segmentStart;
        Vector3 pointVec = point - segmentStart;
        float segmentLen = segmentVec.Length();
        
        if (segmentLen < EPSILON) {
            return pointVec.Length();
        }
        
        float t = pointVec.DotProduct(segmentVec) / (segmentLen * segmentLen);
        
        if (t < 0.0f) {
            return point.DistanceTo(segmentStart);
        } else if (t > 1.0f) {
            return point.DistanceTo(segmentEnd);
        } else {
            Vector3 projection = segmentStart + segmentVec * t;
            return point.DistanceTo(projection);
        }
    }
    
    // 圆形与矩形相交检测
    static bool CircleRectIntersection(
        const Vector2& circleCenter, float circleRadius,
        const Vector2& rectMin, const Vector2& rectMax
    ) {
        // 找到矩形上离圆心最近的点
        Vector2 closestPoint(
            Clamp(circleCenter.x, rectMin.x, rectMax.x),
            Clamp(circleCenter.y, rectMin.y, rectMax.y)
        );
        
        // 计算圆心到最近点的距离
        float distanceSquared = circleCenter.DistanceSquaredTo(closestPoint);
        
        // 如果距离小于等于半径的平方，则相交
        return distanceSquared <= (circleRadius * circleRadius);
    }
    
    // 线段相交检测
    static bool LineSegmentIntersection(
        const Vector2& a1, const Vector2& a2,
        const Vector2& b1, const Vector2& b2,
        Vector2& outIntersectionPoint
    ) {
        Vector2 a = a2 - a1;
        Vector2 b = b2 - b1;
        
        float cross = a.CrossProduct(b);
        
        // 平行线
        if (IsNearlyZero(cross)) {
            return false;
        }
        
        Vector2 c = b1 - a1;
        float t = c.CrossProduct(b) / cross;
        float u = c.CrossProduct(a) / cross;
        
        if (t >= 0.0f && t <= 1.0f && u >= 0.0f && u <= 1.0f) {
            outIntersectionPoint = a1 + a * t;
            return true;
        }
        
        return false;
    }
    
    // 多边形包含点检测
    static bool PointInPolygon(const Vector2& point, const Vector2* polygon, size_t polygonSize) {
        if (polygonSize < 3) {
            return false;
        }
        
        bool inside = false;
        size_t j = polygonSize - 1;
        
        for (size_t i = 0; i < polygonSize; i++) {
            if (((polygon[i].y > point.y) != (polygon[j].y > point.y)) &&
                (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
                inside = !inside;
            }
            j = i;
        }
        
        return inside;
    }
    
    // 圆形包含点检测
    static bool PointInCircle(const Vector2& point, const Vector2& circleCenter, float circleRadius) {
        return point.DistanceSquaredTo(circleCenter) <= (circleRadius * circleRadius);
    }
    
    // 矩形包含点检测
    static bool PointInRect(const Vector2& point, const Vector2& rectMin, const Vector2& rectMax) {
        return point.x >= rectMin.x && point.x <= rectMax.x &&
               point.y >= rectMin.y && point.y <= rectMax.y;
    }
    
    // 时间工具
    static uint64_t GetCurrentTimeMillis() {
        return static_cast<uint64_t>(clock() * 1000 / CLOCKS_PER_SEC);
    }
    
    static void SeedRandom(uint32_t seed) {
        srand(seed);
    }
    
    static float GetDeltaTime(uint64_t lastTime) {
        uint64_t currentTime = GetCurrentTimeMillis();
        return static_cast<float>(currentTime - lastTime) / 1000.0f;
    }
};

// 包含Vector类的定义
#include "Vector.h"