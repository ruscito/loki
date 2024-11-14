# Detect OS
ifeq ($(OS),Windows_NT)
    # MSYS2 Windows settings
    CC = gcc
    CFLAGS = -g -Wall -Wextra -std=c11
    INCLUDES = -Iinclude \
               -I./extern/cglm/include \
               -I./extern \
               -I./extern/glad/include \
               -I/mingw64/include
    LDFLAGS = $(shell pkg-config --libs glfw3) -lopengl32 -lgdi32
    TARGET = $(BIN_DIR)/loki.exe
    RM = rm -rf
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        # macOS settings
        CC = clang
        CFLAGS = -g -Wall -Wextra -std=c11
        INCLUDES = -Iinclude \
                  -I./extern/cglm/include \
                  -I./extern \
                  -I./extern/glad/include \
                  -I/usr/local/include
        LDFLAGS = -L/usr/local/lib -lglfw -framework OpenGL -ldl
        TARGET = $(BIN_DIR)/loki
        RM = rm -rf
    else
        # Linux settings
        CC = gcc
        CFLAGS = -g -Wall -Wextra -std=c11
        INCLUDES = -Iinclude \
                  -I./extern/cglm/include \
                  -I./extern \
                  -I./extern/glad/include \
                  -I/usr/include
        LDFLAGS = -lglfw -lGL -ldl -lm
        TARGET = $(BIN_DIR)/loki
        RM = rm -rf
    endif
endif

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
RES_DIR = res

# Create directories
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Source files and objects
SRCS := $(shell find $(SRC_DIR) -name '*.c')
GLAD_SRC := ./extern/glad/src/glad.c
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
GLAD_OBJ := $(OBJ_DIR)/glad.o

# Declare phony targets
.PHONY: all clean run build-run copy-res

# Default target
all: $(TARGET)

# Linking
$(TARGET): $(OBJS) $(GLAD_OBJ)
	@echo "Linking $@ ..."
	@$(CC) $^ -o $@ $(LDFLAGS)
	@echo "Build complete!"

# Compilation pattern rule for project files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $< ..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compilation rule for GLAD
$(GLAD_OBJ): $(GLAD_SRC)
	@echo "Compiling GLAD..."
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	@echo "Cleaning build files..."
	@$(RM) $(OBJ_DIR) $(TARGET)
	@echo "Clean complete!"

# Run the application
run: $(TARGET)
	@echo "Running $(TARGET)..."
	@./$(TARGET)

# Build and run
build-run: $(TARGET) run

# Copy resources
copy-res:
	@echo "Copying resources..."
	@$(RM) $(BIN_DIR)/$(RES_DIR)
	@cp -r ./$(RES_DIR) $(BIN_DIR)/$(RES_DIR)
	@echo "Resources copied!"