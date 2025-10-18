# RGLS Stablecoin Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lssl -lcrypto -lpthread

# Source files
SOURCES = rgls_stablecoin.c
TARGET = rgls_stablecoin

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
	@echo "Please install OpenSSL development libraries:"
	@echo "  Ubuntu/Debian: sudo apt-get install libssl-dev"
	@echo "  CentOS/RHEL: sudo yum install openssl-devel"
	@echo "  macOS: brew install openssl"

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build the RGLS stablecoin system"
	@echo "  clean        - Remove build artifacts"
	@echo "  test         - Run the RGLS stablecoin test"
	@echo "  install-deps - Show dependency installation instructions"
	@echo "  help         - Show this help message"

.PHONY: all clean test install-deps help
