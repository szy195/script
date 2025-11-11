#pragma once

#include <cstdint>
#include <chrono>

// 跨平台类型定义
#ifndef PLATFORM_WINDOWS
typedef uint32_t DWORD;
typedef void* HANDLE;
typedef const void* LPCVOID;
typedef void* LPVOID;
typedef size_t SIZE_T;
#endif

// 跨平台内存保护标志
#ifndef PAGE_EXECUTE_READWRITE
#define PAGE_EXECUTE_READWRITE 0x40
#endif

// 跨平台时间函数
inline uint32_t GetTickCount() {
    using namespace std::chrono;
    auto now = steady_clock::now();
    auto duration = now.time_since_epoch();
    return static_cast<uint32_t>(duration_cast<milliseconds>(duration).count());
}

// 跨平台模块句柄类型
#ifndef HMODULE
typedef void* HMODULE;
#endif

// 跨平台字符串类型
#ifndef LPCSTR
typedef const char* LPCSTR;
#endif

#ifndef LPSTR
typedef char* LPSTR;
#endif