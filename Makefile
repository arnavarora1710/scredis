CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra
OPTFLAGS = -O3
ARCH = -march=armv8-a+simd

# Directories
SRC_DIR = src
BUILD_DIR = build
DS_DIR = $(SRC_DIR)/ds
BIN_DIR = $(BUILD_DIR)/bin

# Source files and binaries
ST_B_TREE_SRC = $(DS_DIR)/st_b_tree.cpp
ST_B_TREE_BIN = $(BIN_DIR)/st_b_tree

# Default target
all: dirs $(ST_B_TREE_BIN)

# Create necessary directories
dirs:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Compile st_b_tree
$(ST_B_TREE_BIN): $(ST_B_TREE_SRC)
	$(CXX) $(CXXFLAGS) $(OPTFLAGS) $(ARCH) -o $@ $<

# Run the st_b_tree program
run: $(ST_B_TREE_BIN)
	@./$(ST_B_TREE_BIN)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all dirs debug run clean 