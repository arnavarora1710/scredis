CXX = g++
CXXFLAGS = -Wall -Wextra -O2 -g -std=c++17

SRC_DIR = .
BUILD_DIR = .

SERVER_SRC = $(SRC_DIR)/server/init.cpp
CLIENT_SRC = $(SRC_DIR)/client/init.cpp

SERVER_BIN = $(BUILD_DIR)/server/init
CLIENT_BIN = $(BUILD_DIR)/client/init

all: $(SERVER_BIN) $(CLIENT_BIN)

$(SERVER_BIN): $(SERVER_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

$(CLIENT_BIN): $(CLIENT_SRC)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(SERVER_BIN) $(CLIENT_BIN)
	rm -rf $(BUILD_DIR)/server/init.dSYM $(BUILD_DIR)/client/init.dSYM
