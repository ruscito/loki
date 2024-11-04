# Detect OS
ifeq ($(OS),Windows_NT)
    # MSYS2 Windows settings
    CC = gcc
    CFLAGS = -g -Wall -Wextra -std=c11 \
             -Iinclude \
             -I./extern/cglm/include \
             -I./extern \
             -I/mingw64/include
    LDFLAGS = $(shell pkg-config --libs glfw3) -lopengl32 -lgdi32
    TARGET = $(BIN_DIR)/loki.exe
else
    # macOS settings
    CC = clang
    CFLAGS = -g -Wall -Wextra -std=c11 \
             -Iinclude \
             -I./extern/cglm/include \
             -I./extern \
             -I/usr/local/include
    LDFLAGS = -L/usr/local/lib -lglfw -framework OpenGL
    TARGET = $(BIN_DIR)/loki
endif

# Directories
SRC_DIR = src
OBJ_DIR = obj
OBJT_DIR = obj/tools
BIN_DIR = bin

# Create directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(OBJT_DIR) $(BIN_DIR))

# Automatically find all C source files, including those in subdirectories
SRCS = $(shell find $(SRC_DIR) -name '*.c') # extern/cglm/src/*.c
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean

# All target
all: clean $(BIN_DIR)/loki
# all: $(BIN_DIR)/loki

# Linking
$(BIN_DIR)/loki: $(OBJS)
	@echo "Linking $@ with objects: $^"
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compiling object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)  # Create the directory for the object file
	@echo "Compiling $< to $@"
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -rf $(OBJ_DIR)/*.o $(OBJT_DIR)/*.o $(TARGET)

run:
	./$(TARGET)

# Build and run without cleaning
build-run: $(TARGET) run