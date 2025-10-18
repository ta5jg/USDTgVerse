# RGLS Oracle Server Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lssl -lcrypto -lpthread -lcurl

# Source files
SOURCES = rgls_oracle_server.c
TARGET = rgls_oracle_server

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

# Install dependencies
install-deps:
	@echo "Installing dependencies..."
	@echo "Please install required libraries:"
	@echo "  Ubuntu/Debian: sudo apt-get install libssl-dev libcurl4-openssl-dev"
	@echo "  CentOS/RHEL: sudo yum install openssl-devel libcurl-devel"
	@echo "  macOS: brew install openssl curl"

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build the RGLS oracle server"
	@echo "  clean        - Remove build artifacts"
	@echo "  test         - Run the RGLS oracle server test"
	@echo "  install-deps - Show dependency installation instructions"
	@echo "  help         - Show this help message"

.PHONY: all clean test install-deps help
