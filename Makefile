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
TEST_BASIC = basic_tests
TEST_CORRECTNESS = correctness_tests
TEST_BENCHMARK = benchmark_tests

TEST_SRCS = $(TEST_BASIC).cpp $(TEST_CORRECTNESS).cpp $(TEST_BENCHMARK).cpp
TEST_BINS = $(TEST_SRCS:%.cpp=$(TEST_BIN_DIR)/%)
TEST_OBJS = $(TEST_SRCS:%.cpp=$(OBJ_DIR)/test/%.o)

# Default target
all: dirs $(TEST_BINS)

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

# Link test executables
$(TEST_BIN_DIR)/%: $(OBJ_DIR)/test/%.o $(DS_OBJ)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(ARCH) -o $@ $^

# Test targets
test-basic: $(TEST_BIN_DIR)/$(TEST_BASIC)
	@echo "Running basic tests..."
	@./$(TEST_BIN_DIR)/$(TEST_BASIC)

test-correctness: $(TEST_BIN_DIR)/$(TEST_CORRECTNESS)
	@echo "Running correctness tests..."
	@./$(TEST_BIN_DIR)/$(TEST_CORRECTNESS)

test-benchmark: $(TEST_BIN_DIR)/$(TEST_BENCHMARK)
	@echo "Running benchmark tests..."
	@./$(TEST_BIN_DIR)/$(TEST_BENCHMARK)

# Run all tests
test: test-basic test-correctness test-benchmark

# Run small tests (basic + correctness)
test-small: test-basic test-correctness

# Run large tests (benchmark)
test-large: test-benchmark

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all dirs test test-basic test-correctness test-benchmark test-small test-large clean 