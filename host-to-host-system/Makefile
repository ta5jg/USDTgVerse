# USDTgVerse Host-to-Host (H2H) System Makefile
# Author: Irfan Gedik
# Created: 27.09.2025
# Last Update: 27.09.2025
# Version: 1.0

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lssl -lcrypto -lpthread -ljson-c

# Directories
SRC_DIR = .
CORE_DIR = core
PROTOCOLS_DIR = protocols
SECURITY_DIR = security
ANALYTICS_DIR = analytics
INTEGRATION_DIR = integration
API_DIR = api
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
CORE_SOURCES = $(CORE_DIR)/h2h_core.c
PROTOCOL_SOURCES = $(PROTOCOLS_DIR)/swift_protocol.c $(PROTOCOLS_DIR)/iso20022_protocol.c $(PROTOCOLS_DIR)/fix_protocol.c
SECURITY_SOURCES = $(SECURITY_DIR)/h2h_security.c $(SECURITY_DIR)/quantum_encryption.c
ANALYTICS_SOURCES = $(ANALYTICS_DIR)/h2h_analytics.c
INTEGRATION_SOURCES = $(INTEGRATION_DIR)/h2h_integration.c
API_SOURCES = $(API_DIR)/h2h_api.c

# All source files
SOURCES = $(CORE_SOURCES) $(PROTOCOL_SOURCES) $(SECURITY_SOURCES) $(ANALYTICS_SOURCES) $(INTEGRATION_SOURCES) $(API_SOURCES)

# Object files
OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/%.o)

# Header files
HEADERS = $(CORE_DIR)/h2h_core.h $(PROTOCOLS_DIR)/swift_protocol.h

# Target executable
TARGET = usdtgverse_h2h_system

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(CORE_DIR)
	mkdir -p $(BUILD_DIR)/$(PROTOCOLS_DIR)
	mkdir -p $(BUILD_DIR)/$(SECURITY_DIR)
	mkdir -p $(BUILD_DIR)/$(ANALYTICS_DIR)
	mkdir -p $(BUILD_DIR)/$(INTEGRATION_DIR)
	mkdir -p $(BUILD_DIR)/$(API_DIR)

# Build target
$(TARGET): $(OBJECTS)
	@echo "🔗 Linking USDTgVerse H2H System..."
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✅ USDTgVerse H2H System built successfully!"

# Compile source files
$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@echo "🔨 Compiling $<..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -I$(PROTOCOLS_DIR) -I$(SECURITY_DIR) -I$(ANALYTICS_DIR) -I$(INTEGRATION_DIR) -I$(API_DIR) -c $< -o $@

# Core H2H system
$(BUILD_DIR)/$(CORE_DIR)/h2h_core.o: $(CORE_DIR)/h2h_core.c $(CORE_DIR)/h2h_core.h
	@echo "🔨 Compiling H2H Core..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -c $< -o $@

# SWIFT Protocol
$(BUILD_DIR)/$(PROTOCOLS_DIR)/swift_protocol.o: $(PROTOCOLS_DIR)/swift_protocol.c $(PROTOCOLS_DIR)/swift_protocol.h
	@echo "🔨 Compiling SWIFT Protocol..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -I$(PROTOCOLS_DIR) -c $< -o $@

# ISO 20022 Protocol
$(BUILD_DIR)/$(PROTOCOLS_DIR)/iso20022_protocol.o: $(PROTOCOLS_DIR)/iso20022_protocol.c $(PROTOCOLS_DIR)/iso20022_protocol.h
	@echo "🔨 Compiling ISO 20022 Protocol..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -I$(PROTOCOLS_DIR) -c $< -o $@

# FIX Protocol
$(BUILD_DIR)/$(PROTOCOLS_DIR)/fix_protocol.o: $(PROTOCOLS_DIR)/fix_protocol.c $(PROTOCOLS_DIR)/fix_protocol.h
	@echo "🔨 Compiling FIX Protocol..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -I$(PROTOCOLS_DIR) -c $< -o $@

# Security Module
$(BUILD_DIR)/$(SECURITY_DIR)/h2h_security.o: $(SECURITY_DIR)/h2h_security.c $(SECURITY_DIR)/h2h_security.h
	@echo "🔨 Compiling H2H Security..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -I$(SECURITY_DIR) -c $< -o $@

# Quantum Encryption
$(BUILD_DIR)/$(SECURITY_DIR)/quantum_encryption.o: $(SECURITY_DIR)/quantum_encryption.c $(SECURITY_DIR)/quantum_encryption.h
	@echo "🔨 Compiling Quantum Encryption..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -I$(SECURITY_DIR) -c $< -o $@

# Analytics Module
$(BUILD_DIR)/$(ANALYTICS_DIR)/h2h_analytics.o: $(ANALYTICS_DIR)/h2h_analytics.c $(ANALYTICS_DIR)/h2h_analytics.h
	@echo "🔨 Compiling H2H Analytics..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -I$(ANALYTICS_DIR) -c $< -o $@

# Integration Module
$(BUILD_DIR)/$(INTEGRATION_DIR)/h2h_integration.o: $(INTEGRATION_DIR)/h2h_integration.c $(INTEGRATION_DIR)/h2h_integration.h
	@echo "🔨 Compiling H2H Integration..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -I$(INTEGRATION_DIR) -c $< -o $@

# API Module
$(BUILD_DIR)/$(API_DIR)/h2h_api.o: $(API_DIR)/h2h_api.c $(API_DIR)/h2h_api.h
	@echo "🔨 Compiling H2H API..."
	$(CC) $(CFLAGS) -I$(CORE_DIR) -I$(API_DIR) -c $< -o $@

# Clean build files
clean:
	@echo "🧹 Cleaning build files..."
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	@echo "✅ Clean completed!"

# Install system
install: $(TARGET)
	@echo "📦 Installing USDTgVerse H2H System..."
	sudo cp $(TARGET) /usr/local/bin/
	sudo mkdir -p /etc/usdtgverse/h2h
	sudo cp config/h2h.conf /etc/usdtgverse/h2h/
	sudo cp -r interfaces/ /var/www/usdtgverse/h2h/
	@echo "✅ Installation completed!"

# Uninstall system
uninstall:
	@echo "🗑️ Uninstalling USDTgVerse H2H System..."
	sudo rm -f /usr/local/bin/$(TARGET)
	sudo rm -rf /etc/usdtgverse/h2h
	sudo rm -rf /var/www/usdtgverse/h2h
	@echo "✅ Uninstallation completed!"

# Run system
run: $(TARGET)
	@echo "🚀 Starting USDTgVerse H2H System..."
	./$(TARGET)

# Debug build
debug: CFLAGS += -DDEBUG -g3
debug: $(BUILD_DIR) $(TARGET)

# Release build
release: CFLAGS += -DNDEBUG -O3
release: $(BUILD_DIR) $(TARGET)

# Test system
test: $(TARGET)
	@echo "🧪 Running H2H System tests..."
	./$(TARGET) --test

# Generate documentation
docs:
	@echo "📚 Generating H2H System documentation..."
	doxygen Doxyfile

# Format code
format:
	@echo "🎨 Formatting H2H System code..."
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i

# Static analysis
analyze:
	@echo "🔍 Running static analysis..."
	cppcheck --enable=all --inconclusive --std=c99 $(SOURCES)

# Memory check
memcheck: $(TARGET)
	@echo "🔍 Running memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Performance profiling
profile: $(TARGET)
	@echo "📊 Running performance profiling..."
	gprof ./$(TARGET) gmon.out > profile.txt

# Package for distribution
package: release
	@echo "📦 Creating distribution package..."
	mkdir -p dist/usdtgverse-h2h-system
	cp $(TARGET) dist/usdtgverse-h2h-system/
	cp -r config/ dist/usdtgverse-h2h-system/
	cp -r interfaces/ dist/usdtgverse-h2h-system/
	cp README.md dist/usdtgverse-h2h-system/
	cp LICENSE dist/usdtgverse-h2h-system/
	tar -czf usdtgverse-h2h-system.tar.gz -C dist usdtgverse-h2h-system
	@echo "✅ Package created: usdtgverse-h2h-system.tar.gz"

# Help
help:
	@echo "USDTgVerse Host-to-Host (H2H) System Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build the H2H system (default)"
	@echo "  clean      - Remove build files"
	@echo "  install    - Install the H2H system"
	@echo "  uninstall  - Uninstall the H2H system"
	@echo "  run        - Run the H2H system"
	@echo "  debug      - Build with debug symbols"
	@echo "  release    - Build optimized release version"
	@echo "  test       - Run system tests"
	@echo "  docs       - Generate documentation"
	@echo "  format     - Format source code"
	@echo "  analyze    - Run static analysis"
	@echo "  memcheck   - Run memory check"
	@echo "  profile    - Run performance profiling"
	@echo "  package    - Create distribution package"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Example usage:"
	@echo "  make all"
	@echo "  make run"
	@echo "  make clean"

# Phony targets
.PHONY: all clean install uninstall run debug release test docs format analyze memcheck profile package help

# Dependencies
$(BUILD_DIR)/$(CORE_DIR)/h2h_core.o: $(CORE_DIR)/h2h_core.h
$(BUILD_DIR)/$(PROTOCOLS_DIR)/swift_protocol.o: $(PROTOCOLS_DIR)/swift_protocol.h $(CORE_DIR)/h2h_core.h
$(BUILD_DIR)/$(PROTOCOLS_DIR)/iso20022_protocol.o: $(PROTOCOLS_DIR)/iso20022_protocol.h $(CORE_DIR)/h2h_core.h
$(BUILD_DIR)/$(PROTOCOLS_DIR)/fix_protocol.o: $(PROTOCOLS_DIR)/fix_protocol.h $(CORE_DIR)/h2h_core.h
$(BUILD_DIR)/$(SECURITY_DIR)/h2h_security.o: $(SECURITY_DIR)/h2h_security.h $(CORE_DIR)/h2h_core.h
$(BUILD_DIR)/$(SECURITY_DIR)/quantum_encryption.o: $(SECURITY_DIR)/quantum_encryption.h $(CORE_DIR)/h2h_core.h
$(BUILD_DIR)/$(ANALYTICS_DIR)/h2h_analytics.o: $(ANALYTICS_DIR)/h2h_analytics.h $(CORE_DIR)/h2h_core.h
$(BUILD_DIR)/$(INTEGRATION_DIR)/h2h_integration.o: $(INTEGRATION_DIR)/h2h_integration.h $(CORE_DIR)/h2h_core.h
$(BUILD_DIR)/$(API_DIR)/h2h_api.o: $(API_DIR)/h2h_api.h $(CORE_DIR)/h2h_core.h
