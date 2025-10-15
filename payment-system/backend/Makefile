# USDTgVerse Payment API Server Makefile
# Author: Irfan Gedik
# Created: 27.09.2025

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -D_GNU_SOURCE
LDFLAGS = -lpthread
TARGET = payment_api_server
SOURCE = payment_api_server.c

# Default target
all: $(TARGET)

# Build the payment API server
$(TARGET): $(SOURCE)
	@echo "🔨 Building Payment API Server..."
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)
	@echo "✅ Payment API Server built successfully!"

# Clean build artifacts
clean:
	@echo "🧹 Cleaning build artifacts..."
	rm -f $(TARGET)
	rm -rf transactions/
	@echo "✅ Clean completed!"

# Run the server
run: $(TARGET)
	@echo "🚀 Starting Payment API Server..."
	./$(TARGET)

# Install server (copy to system)
install: $(TARGET)
	@echo "📦 Installing Payment API Server..."
	sudo cp $(TARGET) /usr/local/bin/
	sudo chmod +x /usr/local/bin/$(TARGET)
	@echo "✅ Payment API Server installed!"

# Uninstall server
uninstall:
	@echo "🗑️  Uninstalling Payment API Server..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "✅ Payment API Server uninstalled!"

# Create transactions directory
setup:
	@echo "📁 Setting up directories..."
	mkdir -p transactions
	chmod 755 transactions
	@echo "✅ Setup completed!"

# Help
help:
	@echo "USDTgVerse Payment API Server Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all       - Build the payment API server (default)"
	@echo "  clean     - Remove build artifacts"
	@echo "  run       - Build and run the server"
	@echo "  install   - Install server to system"
	@echo "  uninstall - Remove server from system"
	@echo "  setup     - Create necessary directories"
	@echo "  help      - Show this help message"
	@echo ""
	@echo "Usage:"
	@echo "  make          # Build the server"
	@echo "  make run      # Build and run"
	@echo "  make install  # Install to system"
	@echo "  make clean    # Clean up"

.PHONY: all clean run install uninstall setup help
