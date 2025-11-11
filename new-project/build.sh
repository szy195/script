#!/bin/bash

# KBot项目构建脚本

# 设置颜色输出
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# 打印带颜色的消息
print_info() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# 检查命令是否存在
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# 检测操作系统
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        OS="Linux"
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macOS"
    elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
        OS="Windows"
    else
        OS="Unknown"
    fi
    print_info "检测到操作系统: $OS"
}

# 安装依赖
install_dependencies() {
    print_info "检查并安装依赖..."
    
    if [[ "$OS" == "Linux" ]]; then
        # 检查包管理器
        if command_exists apt-get; then
            print_info "使用apt-get安装依赖..."
            sudo apt-get update
            sudo apt-get install -y build-essential cmake libglfw3-dev libgl1-mesa-dev
        elif command_exists yum; then
            print_info "使用yum安装依赖..."
            sudo yum groupinstall -y "Development Tools"
            sudo yum install -y cmake glfw-devel mesa-libGL-devel
        elif command_exists pacman; then
            print_info "使用pacman安装依赖..."
            sudo pacman -Syu --noconfirm
            sudo pacman -S --noconfirm base-devel cmake glfw mesa
        else
            print_warning "未找到支持的包管理器，请手动安装依赖"
        fi
    elif [[ "$OS" == "macOS" ]]; then
        # 检查Homebrew
        if command_exists brew; then
            print_info "使用Homebrew安装依赖..."
            brew update
            brew install cmake glfw
        else
            print_warning "未找到Homebrew，请手动安装依赖"
        fi
    elif [[ "$OS" == "Windows" ]]; then
        print_warning "Windows系统请手动安装Visual Studio、CMake和GLFW"
    fi
}

# 使用CMake构建
build_with_cmake() {
    print_info "使用CMake构建项目..."
    
    # 创建构建目录
    mkdir -p build
    cd build
    
    # 配置项目
    print_info "配置项目..."
    if command_exists cmake; then
        cmake .. -DCMAKE_BUILD_TYPE=Release
        if [ $? -ne 0 ]; then
            print_error "CMake配置失败"
            exit 1
        fi
        
        # 编译项目
        print_info "编译项目..."
        cmake --build . --config Release
        if [ $? -ne 0 ]; then
            print_error "编译失败"
            exit 1
        fi
        
        print_info "构建完成，可执行文件位于: build/bin/KBot"
    else
        print_error "未找到CMake，请先安装CMake"
        exit 1
    fi
    
    cd ..
}

# 使用Makefile构建
build_with_make() {
    print_info "使用Makefile构建项目..."
    
    if command_exists make; then
        make release
        if [ $? -ne 0 ]; then
            print_error "编译失败"
            exit 1
        fi
        
        print_info "构建完成，可执行文件位于: bin/KBot"
    else
        print_error "未找到make，请先安装make"
        exit 1
    fi
}

# 清理构建文件
clean_build() {
    print_info "清理构建文件..."
    
    if [ -d "build" ]; then
        rm -rf build
        print_info "已删除build目录"
    fi
    
    if [ -d "bin" ]; then
        rm -rf bin
        print_info "已删除bin目录"
    fi
    
    if [ -f "Makefile" ]; then
        make clean 2>/dev/null
    fi
}

# 运行项目
run_project() {
    print_info "运行项目..."
    
    if [ -f "build/bin/KBot" ]; then
        ./build/bin/KBot
    elif [ -f "bin/KBot" ]; then
        ./bin/KBot
    else
        print_error "未找到可执行文件，请先构建项目"
        exit 1
    fi
}

# 显示帮助信息
show_help() {
    echo "KBot项目构建脚本"
    echo ""
    echo "用法: $0 [选项]"
    echo ""
    echo "选项:"
    echo "  install     安装依赖"
    echo "  cmake       使用CMake构建项目"
    echo "  make        使用Makefile构建项目"
    echo "  clean       清理构建文件"
    echo "  run         运行项目"
    echo "  help        显示此帮助信息"
    echo ""
    echo "示例:"
    echo "  $0 install  # 安装依赖"
    echo "  $0 cmake    # 使用CMake构建"
    echo "  $0 run      # 运行项目"
}

# 主函数
main() {
    # 检测操作系统
    detect_os
    
    # 处理命令行参数
    case "${1:-}" in
        "install")
            install_dependencies
            ;;
        "cmake")
            build_with_cmake
            ;;
        "make")
            build_with_make
            ;;
        "clean")
            clean_build
            ;;
        "run")
            run_project
            ;;
        "help"|"--help"|"-h")
            show_help
            ;;
        "")
            print_warning "未指定操作，显示帮助信息"
            show_help
            ;;
        *)
            print_error "未知选项: $1"
            show_help
            exit 1
            ;;
    esac
}

# 执行主函数
main "$@"