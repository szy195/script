cmake_minimum_required(VERSION 3.10)
project(KBot)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/Core)
include_directories(${CMAKE_SOURCE_DIR}/Game)
include_directories(${CMAKE_SOURCE_DIR}/Renderer)
include_directories(${CMAKE_SOURCE_DIR}/UI)
include_directories(${CMAKE_SOURCE_DIR}/Utils)

# 收集所有源文件
file(GLOB_RECURSE CORE_SOURCES "Core/*.cpp")
file(GLOB_RECURSE GAME_SOURCES "Game/*.cpp")
file(GLOB_RECURSE RENDERER_SOURCES "Renderer/*.cpp")
file(GLOB_RECURSE UI_SOURCES "UI/*.cpp")
file(GLOB_RECURSE UTILS_SOURCES "Utils/*.cpp")

# 主程序源文件
set(MAIN_SOURCES "main.cpp")

# 创建可执行文件
add_executable(KBot 
    ${MAIN_SOURCES}
    ${CORE_SOURCES}
    ${GAME_SOURCES}
    ${RENDERER_SOURCES}
    ${UI_SOURCES}
    ${UTILS_SOURCES}
)

# 查找并链接必要的库
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

# 链接库
target_link_libraries(KBot 
    ${OPENGL_LIBRARIES}
    glfw
)

# Windows特定设置
if(WIN32)
    # 添加Windows特定的库
    target_link_libraries(KBot 
        opengl32
        gdi32
        user32
        kernel32
        shell32
    )
    
    # 设置子系统为Windows（如果需要GUI应用程序）
    # set_target_properties(KBot PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

# 编译选项
if(MSVC)
    # Visual Studio特定选项
    target_compile_options(KBot PRIVATE /W4)
else()
    # GCC/Clang特定选项
    target_compile_options(KBot PRIVATE -Wall -Wextra -pedantic -D_LIBCPP_ENABLE_CXX17_REMOVED_AUTO_PTR -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS)
endif()

# 预处理器定义
target_compile_definitions(KBot PRIVATE 
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

# 设置调试信息
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set_target_properties(KBot PROPERTIES
        DEBUG_POSTFIX "_d"
    )
endif()

# 复制资源文件（如果有）
# file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}/bin)