# Cross-platform Makefile for Windows, Linux and Mac

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic
DEBUG_FLAGS = -g
RELEASE_FLAGS = -O2

# Detect operating system
ifeq ($(OS),Windows_NT)
    # Windows
    TARGET = movdino.exe
    RM = del /Q
    RM_CMD = $(RM) $(OBJ) $(TARGET) 2>nul
    FIX_PATH = 
else
    # Unix-like (Linux, Mac)
    TARGET = movdino
    RM = rm -f
    RM_CMD = $(RM) $(OBJ) $(TARGET)
    FIX_PATH = ./
endif

# Source files
SRC = movdino.c field.c movement.c utils.c objects.c command_proc.c options.c engine.c
OBJ = $(SRC:.c=.o)

# Default build
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Debug build
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(TARGET)

# Release build  
release: CFLAGS += $(RELEASE_FLAGS)
release: $(TARGET)

# Clean build files
clean:
	$(RM_CMD)

# Test commands
test: $(TARGET)
	$(FIX_PATH)$(TARGET) input.txt output.txt

test-display: $(TARGET)
	$(FIX_PATH)$(TARGET) input.txt output.txt interval 0.5

test-no-display: $(TARGET)
	$(FIX_PATH)$(TARGET) input.txt output.txt no-display

test-no-save: $(TARGET)
	$(FIX_PATH)$(TARGET) input.txt output.txt no-save

test-all: $(TARGET)
	$(FIX_PATH)$(TARGET) input.txt output.txt no-display no-save

# Individual file compilation (for debugging)
movdino.o: movdino.c game.h command_proc.h errors.h options.h utils.h
	$(CC) $(CFLAGS) -c movdino.c

field.o: field.c game.h
	$(CC) $(CFLAGS) -c field.c

movement.o: movement.c game.h errors.h
	$(CC) $(CFLAGS) -c movement.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

objects.o: objects.c game.h objects.h errors.h
	$(CC) $(CFLAGS) -c objects.c

command_proc.o: command_proc.c game.h command_proc.h errors.h objects.h
	$(CC) $(CFLAGS) -c command_proc.c

options.o: options.c options.h
	$(CC) $(CFLAGS) -c options.c

engine.o: engine.c engine.h game.h command_proc.h options.h utils.h
	$(CC) $(CFLAGS) -c engine.c

# Phony targets
.PHONY: all clean test debug release test-display test-no-display test-no-save test-all