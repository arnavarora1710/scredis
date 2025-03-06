CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra
OPTFLAGS = -O3
ARCH = -march=armv8-a+simd
INCLUDES = -I$(SRC_DIR)

# Directories
SRC_DIR = src
BUILD_DIR = build
DS_DIR = $(SRC_DIR)/ds
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin
TEST_DIR = tests
TEST_BIN_DIR = $(BUILD_DIR)/tests

# Source files
DS_SRC = $(wildcard $(DS_DIR)/*.cpp)
DS_OBJ = $(DS_SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Test files
TEST_SRC = $(TEST_DIR)/script.cpp
TEST_OBJ = $(TEST_SRC:$(TEST_DIR)/%.cpp=$(OBJ_DIR)/test/%.o)
TEST_BIN = $(TEST_BIN_DIR)/test_runner

# Default target
all: dirs $(TEST_BIN)

# Create necessary directories
dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR) $(TEST_BIN_DIR) $(OBJ_DIR)/ds $(OBJ_DIR)/test

# Compile data structure source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(ARCH) $(INCLUDES) -c $< -o $@

# Compile test source files
$(OBJ_DIR)/test/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(ARCH) $(INCLUDES) -c $< -o $@

# Link test executable
$(TEST_BIN): $(DS_OBJ) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(ARCH) -o $@ $^

# Run tests
test: $(TEST_BIN)
	@./$(TEST_BIN)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all dirs debug run clean test 