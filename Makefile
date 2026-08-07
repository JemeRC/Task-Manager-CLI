# ==============================================================================
# BUILD CONFIGURATION
# ==============================================================================

COMPILER	= gcc
FLAGS		= -Wall -Wextra -Iinclude -fsanitize=address

# ==============================================================================
# DIRECTORIES
# ==============================================================================

SOURCE_DIR	= src
BUILD_DIR	= build

# ==============================================================================
# FILES & OBJECTS
# ==============================================================================

TARGET 	= $(BUILD_DIR)/taskManagerCLI

SRCS	= $(wildcard $(SOURCE_DIR)/*.c src/**/*.c)
OBJS	= $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# ==============================================================================
# COMPILATION
# ==============================================================================

.PHONY: all clean run

all: makeDir $(TARGET)

$(TARGET): $(OBJS)
	$(COMPILER) $(FLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(COMPILER) $(FLAGS) -c $< -o $@

makeDir:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BUILD_DIR)/process

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)