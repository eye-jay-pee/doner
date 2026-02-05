CC = gcc
CFLAGS = -Wall -Wextra -O2

ROOT = $(notdir $(CURDIR))
SRC_DIR = src
OUT_DIR = build
OBJ_DIR = $(OUT_DIR)/obj

TARGET = $(OUT_DIR)/$(ROOT)

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

.PHONY: help clean run link compile
help:
	@echo "Available targets:"
	@echo "  run      - Build and run $(TARGET)"
	@echo "  clean    - Remove object files and executable"
	@echo "  compile  - Compile source files to object files"
	@echo "  link     - Link object files into $(TARGET)"
	@echo "  help     - Show this message"
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
run: $(TARGET)
	./$<

link: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

compile: $(OBJ_DIR)/%.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

