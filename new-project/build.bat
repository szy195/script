@echo off
REM KBot项目构建脚本 (Windows)

setlocal enabledelayedexpansion

REM 检查参数
if "%1"=="" (
    echo 未指定操作，显示帮助信息
    goto :show_help
)

REM 处理命令行参数
if "%1"=="install" goto :install_dependencies
if "%1"=="cmake" goto :build_with_cmake
if "%1"=="make" goto :build_with_make
if "%1"=="clean" goto :clean_build
if "%1"=="run" goto :run_project
if "%1"=="help" goto :show_help
if "%1"=="--help" goto :show_help
if "%1"=="-h" goto :show_help

echo 未知选项: %1
goto :show_help

:install_dependencies
echo [INFO] 检查并安装依赖...
echo [WARNING] Windows系统请手动安装Visual Studio、CMake和GLFW
echo [INFO] 下载地址:
echo [INFO] Visual Studio: https://visualstudio.microsoft.com/
echo [INFO] CMake: https://cmake.org/download/
echo [INFO] GLFW: https://www.glfw.org/download.html
goto :end

:build_with_cmake
echo [INFO] 使用CMake构建项目...

REM 检查CMake是否存在
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] 未找到CMake，请先安装CMake
    exit /b 1
)

REM 创建构建目录
if not exist build mkdir build
cd build

REM 配置项目
echo [INFO] 配置项目...
cmake .. -DCMAKE_BUILD_TYPE=Release
if %errorlevel% neq 0 (
    echo [ERROR] CMake配置失败
    cd ..
    exit /b 1
)

REM 编译项目
echo [INFO] 编译项目...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo [ERROR] 编译失败
    cd ..
    exit /b 1
)

echo [INFO] 构建完成，可执行文件位于: build\bin\Release\KBot.exe
cd ..
goto :end

:build_with_make
echo [INFO] 使用Makefile构建项目...

REM 检查make是否存在
where make >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] 未找到make，请先安装make或使用CMake构建
    exit /b 1
)

make release
if %errorlevel% neq 0 (
    echo [ERROR] 编译失败
    exit /b 1
)

echo [INFO] 构建完成，可执行文件位于: bin\KBot.exe
goto :end

:clean_build
echo [INFO] 清理构建文件...

if exist build (
    rmdir /s /q build
    echo [INFO] 已删除build目录
)

if exist bin (
    rmdir /s /q bin
    echo [INFO] 已删除bin目录
)

if exist Makefile (
    make clean 2>nul
)

goto :end

:run_project
echo [INFO] 运行项目...

if exist build\bin\Release\KBot.exe (
    start "" "build\bin\Release\KBot.exe"
) else if exist bin\KBot.exe (
    start "" "bin\KBot.exe"
) else (
    echo [ERROR] 未找到可执行文件，请先构建项目
    exit /b 1
)

goto :end

:show_help
echo KBot项目构建脚本 (Windows)
echo.
echo 用法: %0 [选项]
echo.
echo 选项:
echo   install     安装依赖
echo   cmake       使用CMake构建项目
echo   make        使用Makefile构建项目
echo   clean       清理构建文件
echo   run         运行项目
echo   help        显示此帮助信息
echo.
echo 示例:
echo   %0 install  # 安装依赖
echo   %0 cmake    # 使用CMake构建
echo   %0 run      # 运行项目

:end
endlocal