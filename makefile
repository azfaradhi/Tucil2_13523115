# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2 -w

# Directories
SRC_DIR = src
MODULES_DIR = $(SRC_DIR)/modules
BIN_DIR = bin

# Source files
SRCS = $(SRC_DIR)/main.cpp \
       $(MODULES_DIR)/QuadTree.cpp \
       $(MODULES_DIR)/stb_image_impl.cpp

# Output executable
TARGET = $(BIN_DIR)/main

# Default rule
run: $(TARGET)

# Compile the project
$(TARGET): $(SRCS)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

# Clean the build
clean:
	rm -rf $(BIN_DIR)
