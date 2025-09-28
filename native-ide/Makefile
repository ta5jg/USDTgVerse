# USDTgVerse Native IDE - Makefile
# 
# Created: 28.09.2025
# Last Update: 28.09.2025
# 
# Description: Build system for USDTgVerse Native C IDE with Web App frontend
# Author: USDTgVerse Development Team
# License: MIT

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lwebsockets -lpthread -ljson-c -lm

# Source files
SOURCES = src/usdtgverse-ide-core.c \
          src/file-manager.c \
          src/compiler-integration.c \
          src/websocket-handler.c \
          src/quantum-safe.c \
          src/ide-utils.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Target
TARGET = usdtgverse-ide

# Default target
all: $(TARGET)

# Build the IDE
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(LDFLAGS)
	@echo "✅ USDTgVerse IDE built successfully!"

# Compile source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Install dependencies
install-deps:
	@echo "📦 Installing dependencies..."
	sudo apt-get update
	sudo apt-get install -y libwebsockets-dev libjson-c-dev build-essential
	@echo "✅ Dependencies installed!"

# Run the IDE
run: $(TARGET)
	@echo "🚀 Starting USDTgVerse IDE..."
	./$(TARGET) &
	@echo "🌐 Starting web server..."
	cd web && python3 -m http.server 8000 &
	@echo "✅ IDE started! Access at: http://localhost:8000"

# Stop the IDE
stop:
	@echo "⏹️ Stopping USDTgVerse IDE..."
	pkill -f usdtgverse-ide
	pkill -f "python3 -m http.server"
	@echo "✅ IDE stopped!"

# Clean
clean:
	@echo "🧹 Cleaning build files..."
	rm -f $(OBJECTS) $(TARGET)
	@echo "✅ Cleaned!"

# Debug build
debug: CFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Install IDE system-wide
install: $(TARGET)
	@echo "📥 Installing USDTgVerse IDE..."
	sudo cp $(TARGET) /usr/local/bin/
	sudo mkdir -p /usr/local/share/usdtgverse-ide
	sudo cp -r web/* /usr/local/share/usdtgverse-ide/
	@echo "✅ IDE installed system-wide!"

# Uninstall
uninstall:
	@echo "🗑️ Uninstalling USDTgVerse IDE..."
	sudo rm -f /usr/local/bin/$(TARGET)
	sudo rm -rf /usr/local/share/usdtgverse-ide
	@echo "✅ IDE uninstalled!"

# Help
help:
	@echo "USDTgVerse Native IDE - Build System"
	@echo "====================================="
	@echo "make all          - Build the IDE"
	@echo "make install-deps - Install dependencies"
	@echo "make run          - Run the IDE"
	@echo "make stop         - Stop the IDE"
	@echo "make clean        - Clean build files"
	@echo "make debug        - Build debug version"
	@echo "make install      - Install system-wide"
	@echo "make uninstall    - Uninstall"
	@echo "make help         - Show this help"

.PHONY: all install-deps run stop clean debug install uninstall help
