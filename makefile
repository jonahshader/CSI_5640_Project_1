# Compiler settings
CXX = g++
CXXFLAGS = -std=c++20 \
           -fopenacc \
           -foffload=nvptx-none \
           -fno-stack-protector \
           -fcf-protection=none \
           -foffload-options=-fno-stack-protector \
           -foffload-options=-fcf-protection=none \
           -foffload-options=-misa=sm_80 \
           -march=native \
           -ffast-math \
           -fopt-info-optimized-omp \
           -fopt-info-note-omp

# Build type flags
RELEASE_FLAGS = -O3 -DNDEBUG
DEBUG_FLAGS = -g -O0 -DDEBUG

# Default to Release build
CXXFLAGS += $(RELEASE_FLAGS)

# Directory structure
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Source files
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/systems/benchmark.cpp \
          $(SRC_DIR)/systems/json_helper.cpp \
          $(SRC_DIR)/systems/json.cpp \
          $(SRC_DIR)/systems/run_benchmarks.cpp \
          $(SRC_DIR)/systems/types.cpp \
          $(SRC_DIR)/systems/update_state.cpp

# Object files (maintain directory structure in build directory)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Binary name
TARGET = $(BIN_DIR)/cellular_automata

# Include directories
INCLUDES = -I$(SRC_DIR)

# Phony targets
.PHONY: all clean debug dirs

# Default target
all: dirs $(TARGET)

# Debug build
debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: clean all

# Create necessary directories
dirs:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)/systems

# Linking
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(CXXFLAGS)

# Compilation (maintain directory structure)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)