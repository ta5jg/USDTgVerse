# USDTgVerse Oracle Server Makefile (Simplified)
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lpthread

# Source files
SOURCES = usdtgverse_oracle_server_simple.c
TARGET = usdtgverse_oracle_server

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

# Clean
clean:
	rm -f $(TARGET)

# Test
test: $(TARGET)
	./$(TARGET)

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build the USDTgVerse oracle server"
	@echo "  clean        - Remove build artifacts"
	@echo "  test         - Run the USDTgVerse oracle server test"
	@echo "  help         - Show this help message"

.PHONY: all clean test help
