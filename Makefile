# flags=-O2 -Wall -Wextra -std=c2x
# ldflags=-lGL -ldl -lglfw


CC = clang
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lglfw -framework OpenGL

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Create directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Automatically find all C source files, including those in subdirectories
SRCS = $(shell find $(SRC_DIR) -name '*.c') 
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean

# All target
all: clean $(BIN_DIR)/loki

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
	rm -rf $(OBJ_DIR)/**/*.o $(BIN_DIR)/loki