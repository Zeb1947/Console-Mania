# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Source file
SRC = src/main.cpp

# Output directory and executable name
BIN_DIR = bin
TARGET = a

# Full path to the output executable
OUT = $(BIN_DIR)/$(TARGET)

# Default rule
all: $(OUT)

# Rule to build the executable
$(OUT): $(SRC) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Ensure the output directory exists
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Clean rule
clean:
	rm -rf $(BIN_DIR)/$(TARGET)

.PHONY: all clean

