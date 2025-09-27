# USDTgVerse Payment System Makefile
# Author: Irfan Gedik
# Created: 27.09.2025
# Last Update: 27.09.2025
# Version: 1.0

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lssl -lcrypto -lcurl -ljansson -lsqlite3 -lpthread

# Directories
SRC_DIR = .
CORE_DIR = core
GATEWAYS_DIR = gateways
INTERFACES_DIR = interfaces
SECURITY_DIR = security
API_DIR = api
BUILD_DIR = build
INCLUDE_DIR = include

# Source files
CORE_SOURCES = $(CORE_DIR)/payment_core.c
GATEWAY_SOURCES = $(GATEWAYS_DIR)/usdtgverse_pay.c $(GATEWAYS_DIR)/direct_ip_payment.c
SECURITY_SOURCES = $(SECURITY_DIR)/quantum_crypto.c $(SECURITY_DIR)/encryption.c
API_SOURCES = $(API_DIR)/payment_api.c $(API_DIR)/webhook_handler.c

ALL_SOURCES = $(CORE_SOURCES) $(GATEWAY_SOURCES) $(SECURITY_SOURCES) $(API_SOURCES)

# Object files
OBJECTS = $(ALL_SOURCES:%.c=$(BUILD_DIR)/%.o)

# Target executable
TARGET = usdtgverse_payment_system

# Default target
all: $(BUILD_DIR) $(TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(CORE_DIR)
	mkdir -p $(BUILD_DIR)/$(GATEWAYS_DIR)
	mkdir -p $(BUILD_DIR)/$(SECURITY_DIR)
	mkdir -p $(BUILD_DIR)/$(API_DIR)

# Build main executable
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✅ USDTgVerse Payment System built successfully!"

# Compile source files
$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Build core payment system
core: $(BUILD_DIR)/$(CORE_DIR)/payment_core.o
	@echo "✅ Core payment system compiled!"

# Build gateways
gateways: $(BUILD_DIR)/$(GATEWAYS_DIR)/usdtgverse_pay.o $(BUILD_DIR)/$(GATEWAYS_DIR)/direct_ip_payment.o
	@echo "✅ Payment gateways compiled!"

# Build security modules
security: $(BUILD_DIR)/$(SECURITY_DIR)/quantum_crypto.o $(BUILD_DIR)/$(SECURITY_DIR)/encryption.o
	@echo "✅ Security modules compiled!"

# Build API modules
api: $(BUILD_DIR)/$(API_DIR)/payment_api.o $(BUILD_DIR)/$(API_DIR)/webhook_handler.o
	@echo "✅ API modules compiled!"

# Build shared library
lib: $(OBJECTS)
	$(CC) -shared $(OBJECTS) -o libusdtgverse_payment.so $(LDFLAGS)
	@echo "✅ Shared library built successfully!"

# Build static library
static: $(OBJECTS)
	ar rcs libusdtgverse_payment.a $(OBJECTS)
	@echo "✅ Static library built successfully!"

# Install system
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	sudo cp -r $(INTERFACES_DIR) /usr/local/share/usdtgverse_payment/
	sudo mkdir -p /etc/usdtgverse_payment
	sudo cp config/payment.conf /etc/usdtgverse_payment/
	@echo "✅ USDTgVerse Payment System installed successfully!"

# Uninstall system
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
	sudo rm -rf /usr/local/share/usdtgverse_payment/
	sudo rm -rf /etc/usdtgverse_payment/
	@echo "✅ USDTgVerse Payment System uninstalled!"

# Run system
run: $(TARGET)
	./$(TARGET)

# Run with debug
debug: $(TARGET)
	gdb ./$(TARGET)

# Run with valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Test system
test: $(TARGET)
	./$(TARGET) --test

# Performance test
perf: $(TARGET)
	./$(TARGET) --performance-test

# Security test
security-test: $(TARGET)
	./$(TARGET) --security-test

# Generate documentation
docs:
	doxygen Doxyfile
	@echo "✅ Documentation generated!"

# Format code
format:
	find . -name "*.c" -o -name "*.h" | xargs clang-format -i
	@echo "✅ Code formatted!"

# Lint code
lint:
	find . -name "*.c" -o -name "*.h" | xargs cppcheck --enable=all
	@echo "✅ Code linted!"

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(TARGET)
	rm -f libusdtgverse_payment.so
	rm -f libusdtgverse_payment.a
	@echo "✅ Build files cleaned!"

# Clean all files
distclean: clean
	rm -rf docs/
	rm -f *.log
	rm -f *.pid
	@echo "✅ All files cleaned!"

# Show help
help:
	@echo "USDTgVerse Payment System Build System"
	@echo "======================================"
	@echo ""
	@echo "Available targets:"
	@echo "  all          - Build the complete payment system"
	@echo "  core         - Build core payment system only"
	@echo "  gateways     - Build payment gateways only"
	@echo "  security     - Build security modules only"
	@echo "  api          - Build API modules only"
	@echo "  lib          - Build shared library"
	@echo "  static       - Build static library"
	@echo "  install      - Install system to /usr/local"
	@echo "  uninstall    - Remove system from /usr/local"
	@echo "  run          - Run the payment system"
	@echo "  debug        - Run with GDB debugger"
	@echo "  valgrind     - Run with Valgrind memory checker"
	@echo "  test         - Run system tests"
	@echo "  perf         - Run performance tests"
	@echo "  security-test - Run security tests"
	@echo "  docs         - Generate documentation"
	@echo "  format       - Format source code"
	@echo "  lint         - Lint source code"
	@echo "  clean        - Clean build files"
	@echo "  distclean    - Clean all files"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make all     - Build everything"
	@echo "  make run     - Build and run"
	@echo "  make test    - Build and test"
	@echo "  make clean   - Clean build files"

# Dependencies
$(BUILD_DIR)/$(CORE_DIR)/payment_core.o: $(CORE_DIR)/payment_core.c $(CORE_DIR)/payment_core.h
$(BUILD_DIR)/$(GATEWAYS_DIR)/usdtgverse_pay.o: $(GATEWAYS_DIR)/usdtgverse_pay.c $(GATEWAYS_DIR)/usdtgverse_pay.h
$(BUILD_DIR)/$(GATEWAYS_DIR)/direct_ip_payment.o: $(GATEWAYS_DIR)/direct_ip_payment.c $(GATEWAYS_DIR)/direct_ip_payment.h
$(BUILD_DIR)/$(SECURITY_DIR)/quantum_crypto.o: $(SECURITY_DIR)/quantum_crypto.c $(SECURITY_DIR)/quantum_crypto.h
$(BUILD_DIR)/$(SECURITY_DIR)/encryption.o: $(SECURITY_DIR)/encryption.c $(SECURITY_DIR)/encryption.h
$(BUILD_DIR)/$(API_DIR)/payment_api.o: $(API_DIR)/payment_api.c $(API_DIR)/payment_api.h
$(BUILD_DIR)/$(API_DIR)/webhook_handler.o: $(API_DIR)/webhook_handler.c $(API_DIR)/webhook_handler.h

# Phony targets
.PHONY: all core gateways security api lib static install uninstall run debug valgrind test perf security-test docs format lint clean distclean help
