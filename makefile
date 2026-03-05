CC = gcc
CFLAGS = -Wall -Wextra -O2 -Isrc -Iinclude

ROOT = $(notdir $(CURDIR))
SRC_DIR = src
OUT_DIR = build
OBJ_DIR = $(OUT_DIR)/obj
BIN     = build/app

TARGET = $(OUT_DIR)/$(ROOT)

SRC := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))


$(TARGET): $(OBJ)
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: help list clean run link compile
help:
	@echo "Available targets:"
	@echo "  run      - Build and run $(TARGET)"
	@echo "  list     - list all source files $(TARGET)"
	@echo "  clean    - Remove object files and executable"
	@echo "  compile  - Compile source files to object files"
	@echo "  link     - Link object files into $(TARGET)"
	@echo "  help     - Show this message"
list:
	@echo "source files:"
	@echo $(SRC)
	@echo "obj files:"
	@echo $(OBJ)
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
run: $(TARGET)
	./$<
link: $(TARGET)

