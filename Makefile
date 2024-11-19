# Shell
SHELL=/bin/sh

# Directories already present
SRC_DIR=src
INC_DIR=include

# Directories that may not be present
OBJ_DIR=obj
BIN_DIR=bin

CC=clang
AS=clang
CXX=clang++
LD=ld.lld

# Compiler flags
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror
CFLAGS += -nostdinc
CFLAGS += -I$(INC_DIR)/

# Linker flags
LDFLAGS = -T $(LINKER_SCRIPT)

# Source files
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
CXX_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
ASM_SOURCES = $(wildcard $(SRC_DIR)/*.S)

# Object files
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CXX_SOURCES))
OBJECTS += $(patsubst $(SRC_DIR)/%.S, $(OBJ_DIR)/%.o, $(ASM_SOURCES))

# Compiling
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR) 
	$(CXX) $(CFLAGS) -c $< -o $@
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.S | $(OBJ_DIR) 
	$(AS) $(CFLAGS) -c $< -o $@

# Linking
$(BIN_DIR)/txapela: $(OBJECTS) | $(OBJ_DIR) $(BIN_DIR) 
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

$(BIN_DIR):
	mkdir -p $@
$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean build test

test: build
	@echo "TODO: test"

build:
	@echo "TODO: build"

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)
