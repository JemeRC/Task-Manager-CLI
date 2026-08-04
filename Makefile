# ==============================================================================
# BUILD CONFIGURATION
# ==============================================================================

COMPILER	= gcc
FLAGS		= -Wall -Wextra

# ==============================================================================
# DIRECTORIES
# ==============================================================================

SOURCE_DIR	= src
BUILD_DIR	= build

# ==============================================================================
# FILES & OBJECTS
# ==============================================================================

TARGET 	= $(BUILD_DIR)/taskManagerCLI

SRCS	= $(wildcard $(SOURCE_DIR)/*.c)
OBJS	= $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# ==============================================================================
# COMPILATION
# ==============================================================================

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(COMPILER) $(FLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(COMPILER) $(FLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)