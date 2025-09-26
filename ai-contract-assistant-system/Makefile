# USDTgVerse AI-Powered Smart Contract Assistant Makefile
# Created by USDTgVerse Development Team
# Copyright © 2024 USDTgVerse. All rights reserved.

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
INCLUDES = -I./include
LIBS = -lpthread -lcurl -ljson-c

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TEST_DIR = tests

# Source files
SOURCES = $(SRC_DIR)/ai_contract_assistant.c
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target
TARGET = $(BUILD_DIR)/libai_contract_assistant.a
SHARED_TARGET = $(BUILD_DIR)/libai_contract_assistant.so

# Test files
TEST_SOURCES = $(TEST_DIR)/test_ai_contract_assistant.c
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.c=$(BUILD_DIR)/%.o)
TEST_TARGET = $(BUILD_DIR)/test_ai_contract_assistant

# Default target
all: $(TARGET) $(SHARED_TARGET)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Static library
$(TARGET): $(BUILD_DIR) $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	@echo "✅ Static library built: $@"

# Shared library
$(SHARED_TARGET): $(BUILD_DIR) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS) $(LIBS)
	@echo "✅ Shared library built: $@"

# Object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Test target
test: $(TEST_TARGET)
	@echo "🧪 Running AI-Powered Smart Contract Assistant tests..."
	./$(TEST_TARGET)

# Test executable
$(TEST_TARGET): $(BUILD_DIR) $(TEST_OBJECTS) $(TARGET)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(TEST_OBJECTS) -L$(BUILD_DIR) -lai_contract_assistant $(LIBS)
	@echo "✅ Test executable built: $@"

# Test object files
$(BUILD_DIR)/%.o: $(TEST_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)
	@echo "🧹 Cleaned build directory"

# Install
install: $(TARGET) $(SHARED_TARGET)
	@echo "📦 Installing AI-Powered Smart Contract Assistant..."
	sudo cp $(TARGET) /usr/local/lib/
	sudo cp $(SHARED_TARGET) /usr/local/lib/
	sudo cp $(INCLUDE_DIR)/ai_contract_assistant.h /usr/local/include/
	sudo ldconfig
	@echo "✅ AI-Powered Smart Contract Assistant installed"

# Uninstall
uninstall:
	@echo "🗑️ Uninstalling AI-Powered Smart Contract Assistant..."
	sudo rm -f /usr/local/lib/libai_contract_assistant.a
	sudo rm -f /usr/local/lib/libai_contract_assistant.so
	sudo rm -f /usr/local/include/ai_contract_assistant.h
	sudo ldconfig
	@echo "✅ AI-Powered Smart Contract Assistant uninstalled"

# Debug build
debug: CFLAGS += -DDEBUG -g3
debug: $(TARGET) $(SHARED_TARGET)

# Release build
release: CFLAGS += -DNDEBUG -O3
release: $(TARGET) $(SHARED_TARGET)

# Documentation
docs:
	@echo "📚 Generating documentation..."
	doxygen Doxyfile
	@echo "✅ Documentation generated in docs/"

# Format code
format:
	@echo "🎨 Formatting code..."
	clang-format -i $(SRC_DIR)/*.c $(INCLUDE_DIR)/*.h
	@echo "✅ Code formatted"

# Lint code
lint:
	@echo "🔍 Linting code..."
	cppcheck --enable=all --std=c99 $(SRC_DIR)/*.c $(INCLUDE_DIR)/*.h
	@echo "✅ Code linted"

# Memory check
memcheck: $(TEST_TARGET)
	@echo "🔍 Running memory check..."
	valgrind --leak-check=full --show-leak-kinds=all ./$(TEST_TARGET)
	@echo "✅ Memory check completed"

# Performance test
perf: $(TEST_TARGET)
	@echo "⚡ Running performance test..."
	perf stat ./$(TEST_TARGET)
	@echo "✅ Performance test completed"

# Coverage report
coverage: CFLAGS += --coverage
coverage: $(TEST_TARGET)
	@echo "📊 Running coverage test..."
	./$(TEST_TARGET)
	gcov $(SOURCES)
	@echo "✅ Coverage report generated"

# Help
help:
	@echo "USDTgVerse AI-Powered Smart Contract Assistant Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  all        - Build static and shared libraries (default)"
	@echo "  test       - Build and run tests"
	@echo "  clean      - Clean build directory"
	@echo "  install    - Install libraries and headers"
	@echo "  uninstall  - Uninstall libraries and headers"
	@echo "  debug      - Build with debug symbols"
	@echo "  release    - Build optimized release version"
	@echo "  docs       - Generate documentation"
	@echo "  format     - Format source code"
	@echo "  lint       - Lint source code"
	@echo "  memcheck   - Run memory leak check"
	@echo "  perf       - Run performance test"
	@echo "  coverage   - Generate coverage report"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Build configuration:"
	@echo "  CC         = $(CC)"
	@echo "  CFLAGS     = $(CFLAGS)"
	@echo "  INCLUDES   = $(INCLUDES)"
	@echo "  LIBS       = $(LIBS)"

.PHONY: all test clean install uninstall debug release docs format lint memcheck perf coverage help
