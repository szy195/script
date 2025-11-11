#pragma once

// 确保标准库正确初始化
#ifndef _LIBCPP_VERSION
#define _LIBCPP_VERSION 1
#endif

#include <string>
#include <cmath>
#include <sstream>

// 2D向量类
class Vector2 {
public:
    float x, y;
    
    // 构造函数
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}
    Vector2(const Vector2& other) : x(other.x), y(other.y) {}
    
    // 赋值运算符
    Vector2& operator=(const Vector2& other) {
        x = other.x;
        y = other.y;
        return *this;
    }
    
    // 算术运算符
    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }
    
    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }
    
    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }
    
    Vector2 operator/(float scalar) const {
        return Vector2(x / scalar, y / scalar);
    }
    
    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vector2& operator-=(const Vector2& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Vector2& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    Vector2& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        return *this;
    }
    
    // 比较运算符
    bool operator==(const Vector2& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Vector2& other) const {
        return !(*this == other);
    }
    
    // 向量运算
    float Length() const {
        return std::sqrt(x * x + y * y);
    }
    
    float LengthSquared() const {
        return x * x + y * y;
    }
    
    float DistanceTo(const Vector2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    float DistanceSquaredTo(const Vector2& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return dx * dx + dy * dy;
    }
    
    float DotProduct(const Vector2& other) const {
        return x * other.x + y * other.y;
    }
    
    float CrossProduct(const Vector2& other) const {
        return x * other.y - y * other.x;
    }
    
    Vector2 Normalized() const {
        float len = Length();
        if (len > 0.0f) {
            return Vector2(x / len, y / len);
        }
        return Vector2();
    }
    
    Vector2& Normalize() {
        float len = Length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
        }
        return *this;
    }
    
    Vector2 Perpendicular() const {
        return Vector2(-y, x);
    }
    
    Vector2 Lerp(const Vector2& other, float t) const {
        return Vector2(x + (other.x - x) * t, y + (other.y - y) * t);
    }
    
    // 角度计算
    float Angle() const {
        return std::atan2(y, x);
    }
    
    float AngleTo(const Vector2& other) const {
        return std::atan2(other.y - y, other.x - x);
    }
    
    float AngleBetween(const Vector2& other) const {
        float dot = DotProduct(other);
        float det = CrossProduct(other);
        return std::atan2(det, dot);
    }
    
    // 旋转
    Vector2 Rotated(float angle) const {
        float cosAngle = std::cos(angle);
        float sinAngle = std::sin(angle);
        return Vector2(x * cosAngle - y * sinAngle, x * sinAngle + y * cosAngle);
    }
    
    Vector2& Rotate(float angle) {
        float cosAngle = std::cos(angle);
        float sinAngle = std::sin(angle);
        float newX = x * cosAngle - y * sinAngle;
        float newY = x * sinAngle + y * cosAngle;
        x = newX;
        y = newY;
        return *this;
    }
    
    // 工具方法
    bool IsZero() const {
        return x == 0.0f && y == 0.0f;
    }
    
    bool IsNearlyZero(float tolerance = 0.0001f) const {
        return std::abs(x) < tolerance && std::abs(y) < tolerance;
    }
    
    std::string ToString() const {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ")";
        return ss.str();
    }
    
    // 静态常量
    static const Vector2 Zero;
    static const Vector2 One;
    static const Vector2 UnitX;
    static const Vector2 UnitY;
};

// 3D向量类
class Vector3 {
public:
    float x, y, z;
    
    // 构造函数
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}
    Vector3(const Vector2& v2, float z = 0.0f) : x(v2.x), y(v2.y), z(z) {}
    
    // 赋值运算符
    Vector3& operator=(const Vector3& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
    
    // 算术运算符
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }
    
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }
    
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }
    
    Vector3 operator/(float scalar) const {
        return Vector3(x / scalar, y / scalar, z / scalar);
    }
    
    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }
    
    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }
    
    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    
    Vector3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    
    // 比较运算符
    bool operator==(const Vector3& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
    
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
    
    // 向量运算
    float Length() const {
        return std::sqrt(x * x + y * y + z * z);
    }
    
    float LengthSquared() const {
        return x * x + y * y + z * z;
    }
    
    float DistanceTo(const Vector3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return std::sqrt(dx * dx + dy * dy + dz * dz);
    }
    
    float DistanceSquaredTo(const Vector3& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        float dz = z - other.z;
        return dx * dx + dy * dy + dz * dz;
    }
    
    float DotProduct(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
    
    Vector3 CrossProduct(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }
    
    Vector3 Normalized() const {
        float len = Length();
        if (len > 0.0f) {
            return Vector3(x / len, y / len, z / len);
        }
        return Vector3();
    }
    
    Vector3& Normalize() {
        float len = Length();
        if (len > 0.0f) {
            x /= len;
            y /= len;
            z /= len;
        }
        return *this;
    }
    
    Vector3 Lerp(const Vector3& other, float t) const {
        return Vector3(
            x + (other.x - x) * t,
            y + (other.y - y) * t,
            z + (other.z - z) * t
        );
    }
    
    // 投影和反射
    Vector3 ProjectOnTo(const Vector3& other) const {
        float dot = DotProduct(other);
        float lenSq = other.LengthSquared();
        if (lenSq > 0.0f) {
            return other * (dot / lenSq);
        }
        return Vector3();
    }
    
    Vector3 Reflect(const Vector3& normal) const {
        return *this - normal * (2.0f * DotProduct(normal));
    }
    
    // 工具方法
    bool IsZero() const {
        return x == 0.0f && y == 0.0f && z == 0.0f;
    }
    
    bool IsNearlyZero(float tolerance = 0.0001f) const {
        return std::abs(x) < tolerance && std::abs(y) < tolerance && std::abs(z) < tolerance;
    }
    
    Vector2 ToVector2() const {
        return Vector2(x, y);
    }
    
    std::string ToString() const {
        std::stringstream ss;
        ss << "(" << x << ", " << y << ", " << z << ")";
        return ss.str();
    }
    
    // 静态常量
    static const Vector3 Zero;
    static const Vector3 One;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;
    static const Vector3 Up;
    static const Vector3 Down;
    static const Vector3 Forward;
    static const Vector3 Backward;
    static const Vector3 Left;
    static const Vector3 Right;
};

// 全局运算符
inline Vector2 operator*(float scalar, const Vector2& vector) {
    return vector * scalar;
}

inline Vector3 operator*(float scalar, const Vector3& vector) {
    return vector * scalar;
}

// 工具函数
inline float Distance(const Vector2& a, const Vector2& b) {
    return a.DistanceTo(b);
}

inline float Distance(const Vector3& a, const Vector3& b) {
    return a.DistanceTo(b);
}

inline float DotProduct(const Vector2& a, const Vector2& b) {
    return a.DotProduct(b);
}

inline float DotProduct(const Vector3& a, const Vector3& b) {
    return a.DotProduct(b);
}

inline Vector3 CrossProduct(const Vector3& a, const Vector3& b) {
    return a.CrossProduct(b);
}