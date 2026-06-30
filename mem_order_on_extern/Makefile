
# Variables
CC = gcc
CFLAGS = -Wall -Wextra -O3 -Iinclude -pthread
TARGET = bin/app

# Automatically find all .c files inside the src/ directory
SRCS = $(wildcard src/*.c)

# Default rule: build the executable
all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@

# Clean up build artifacts
clean:
	rm -rf bin
