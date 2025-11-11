# Makefile for KBot Project

# 编译器设置
CXX = clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -stdlib=libc++ -D_LIBCPP_DEBUG=0 -D_LIBCPP_DISABLE_VISIBILITY_ANNOTATIONS
DEBUG_FLAGS = -g -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

# 目录设置
SRC_DIR = .
BUILD_DIR = build
BIN_DIR = bin

# 包含目录
INCLUDES = -I$(SRC_DIR) -I$(SRC_DIR)/Core -I$(SRC_DIR)/Game -I$(SRC_DIR)/Renderer -I$(SRC_DIR)/UI -I$(SRC_DIR)/Utils

# 链接库
LIBS = -lGL -lglfw

# 源文件
CORE_SOURCES = $(wildcard Core/*.cpp)
GAME_SOURCES = $(wildcard Game/*.cpp)
RENDERER_SOURCES = $(wildcard Renderer/*.cpp)
UI_SOURCES = $(wildcard UI/*.cpp)
UTILS_SOURCES = $(wildcard Utils/*.cpp)
GAMEOBJECT_SOURCES = $(wildcard GameObject/*.cpp)
MAIN_SOURCES = main.cpp

# 所有源文件
ALL_SOURCES = $(MAIN_SOURCES) $(CORE_SOURCES) $(GAME_SOURCES) $(RENDERER_SOURCES) $(UI_SOURCES) $(UTILS_SOURCES) $(GAMEOBJECT_SOURCES)

# 目标文件
OBJECTS = $(ALL_SOURCES:%.cpp=$(BUILD_DIR)/%.o)

# 可执行文件
TARGET = $(BIN_DIR)/KBot

# 偏移量测试工具
TEST_OFFSETS_SOURCES = TestOffsets.cpp $(CORE_SOURCES) $(GAMEOBJECT_SOURCES)
TEST_OFFSETS_OBJECTS = $(TEST_OFFSETS_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
TEST_OFFSETS_TARGET = $(BIN_DIR)/TestOffsets

# 偏移量比较工具
COMPARE_OFFSETS_SOURCES = CompareOffsets.cpp $(CORE_SOURCES)
COMPARE_OFFSETS_OBJECTS = $(COMPARE_OFFSETS_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
COMPARE_OFFSETS_TARGET = $(BIN_DIR)/CompareOffsets

# 偏移量验证工具
VALIDATE_OFFSETS_SOURCES = ValidateOffsets.cpp $(CORE_SOURCES)
VALIDATE_OFFSETS_OBJECTS = $(VALIDATE_OFFSETS_SOURCES:%.cpp=$(BUILD_DIR)/%.o)
VALIDATE_OFFSETS_TARGET = $(BIN_DIR)/ValidateOffsets

# 默认目标
all: $(TARGET) $(TEST_OFFSETS_TARGET) $(COMPARE_OFFSETS_TARGET) $(VALIDATE_OFFSETS_TARGET)

# 创建目录
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/Core $(BUILD_DIR)/Game $(BUILD_DIR)/Renderer $(BUILD_DIR)/UI $(BUILD_DIR)/Utils $(BUILD_DIR)/GameObject

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 编译目标文件
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 链接可执行文件
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LIBS) -o $@

# 偏移量测试工具
$(TEST_OFFSETS_TARGET): $(TEST_OFFSETS_OBJECTS) | $(BIN_DIR)
	$(CXX) $(TEST_OFFSETS_OBJECTS) -o $@

# 偏移量比较工具
$(COMPARE_OFFSETS_TARGET): $(COMPARE_OFFSETS_OBJECTS) | $(BIN_DIR)
	$(CXX) $(COMPARE_OFFSETS_OBJECTS) -o $@

# 偏移量验证工具
$(VALIDATE_OFFSETS_TARGET): $(VALIDATE_OFFSETS_OBJECTS) | $(BIN_DIR)
	$(CXX) $(VALIDATE_OFFSETS_OBJECTS) -o $@

# 调试版本
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: clean all

# 发布版本
release: CXXFLAGS += $(RELEASE_FLAGS)
release: clean all

# 清理
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# 安装（可选）
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/

# 运行
run: $(TARGET)
	./$(TARGET)

# 依赖关系
$(BUILD_DIR)/main.o: main.cpp UI/UISample.h Core/KBotCore.h Renderer/Renderer.h
$(BUILD_DIR)/Core/%.o: Core/%.cpp Core/%.h Utils/Math.h Utils/Vector.h
$(BUILD_DIR)/Game/%.o: Game/%.cpp Game/%.h Core/GameAPI.h Utils/Math.h Utils/Vector.h
$(BUILD_DIR)/Renderer/%.o: Renderer/%.cpp Renderer/%.h Utils/Math.h Utils/Vector.h
$(BUILD_DIR)/UI/%.o: UI/%.cpp UI/%.h Core/Events.h Renderer/Renderer.h Utils/Math.h Utils/Vector.h

# 帮助信息
help:
	@echo "Available targets:"
	@echo "  all           - Build the project and tools (default)"
	@echo "  debug         - Build debug version"
	@echo "  release       - Build release version"
	@echo "  clean         - Remove build files"
	@echo "  install       - Install to /usr/local/bin"
	@echo "  run           - Build and run the project"
	@echo "  test-offsets  - Build the offset testing tool"
	@echo "  compare-offsets - Build the offset comparison tool"
	@echo "  validate-offsets - Build the offset validation tool"
	@echo "  help          - Show this help message"

# 单独构建偏移量测试工具
test-offsets: $(TEST_OFFSETS_TARGET)

# 单独构建偏移量比较工具
compare-offsets: $(COMPARE_OFFSETS_TARGET)

# 单独构建偏移量验证工具
validate-offsets: $(VALIDATE_OFFSETS_TARGET)

.PHONY: all debug release clean install run help test-offsets compare-offsets validate-offsets