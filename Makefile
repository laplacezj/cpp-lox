# 编译器和编译选项
CXX := g++
CXXFLAGS := -Wall -ggdb -Wextra -std=c++17 -Iinclude
CPPFLAGS := -MMD  # 启用生成依赖文件

# 指定中间文件目录
BUILD_DIR = build

# 目标可执行文件名称
TARGET = $(BUILD_DIR)/cpp-lox

# 源文件目录
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# 生成的目标文件和依赖文件
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(OBJS:.o=.d)

# 链接所有目标文件生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

# 编译规则：将.cpp文件编译成.o文件，并生成依赖文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# 包含所有依赖文件
-include $(DEPS)

# 清理生成的文件
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/*.d $(TARGET)

# 伪目标
.PHONY: clean


.PHONY: test-lexing
test-lexing:
	@make >/dev/null
	@echo "testing cpp-lox with test-lexing.lox ..."
	@./$(BUILD_DIR)/cpp-lox tests/test-lexing.lox | diff -u --color tests/test-lexing.lox.expected -;


.PHONY: test-lexing2
test-lexing2:
	@make >/dev/null
	@echo "testing cpp-lox with test-lexing2.lox ..."
	@./$(BUILD_DIR)/cpp-lox tests/test-lexing2.lox | diff -u --color tests/test-lexing2.lox.expected -;

.PHONY: test-parsing
test-parsing:
	@make >/dev/null
	@echo "testing cpp-lox with test-parsing.lox ..."
	@./$(BUILD_DIR)/cpp-lox tests/test-parsing.lox | diff -u --color tests/test-parsing.lox.expected -;