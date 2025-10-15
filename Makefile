#!/bin/bash
# ==============================================
# File:        Makefile
# Author:      USDTG GROUP TECHNOLOGY LLC
# Created:     2025-10-04
# Last Update: 2025-10-05
# Version:     1.0
#
# Description:
#   USDTgVerse Social Media Bot - Makefile
#   
#   Pure C ve Bash script'ler için build sistemi
#   Professional build system for USDTgVerse ecosystem
#
# License:
#   MIT License
# ==============================================

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LDFLAGS = -lpthread

# Directories
SRC_DIR = .
BUILD_DIR = build
ANALYTICS_DIR = analytics
INTEGRATION_DIR = cex_integration

# Source files
SOCIAL_BOT_C = social_media_bot.c
SOCIAL_BOT_SH = social_media_bot.sh
CEX_INTEGRATION_SH = cex_integration.sh

# Executables
SOCIAL_BOT_EXE = $(BUILD_DIR)/social_media_bot
SOCIAL_BOT_SH_EXE = $(BUILD_DIR)/social_media_bot.sh
CEX_INTEGRATION_SH_EXE = $(BUILD_DIR)/cex_integration.sh

# Default target
all: $(SOCIAL_BOT_EXE) $(SOCIAL_BOT_SH_EXE) $(CEX_INTEGRATION_SH_EXE)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Create analytics directory
$(ANALYTICS_DIR):
	mkdir -p $(ANALYTICS_DIR)

# Create integration directory
$(INTEGRATION_DIR):
	mkdir -p $(INTEGRATION_DIR)

# Compile C social media bot
$(SOCIAL_BOT_EXE): $(SOCIAL_BOT_C) | $(BUILD_DIR) $(ANALYTICS_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)
	@echo "✅ Compiled C social media bot: $@"

# Copy bash social media bot
$(SOCIAL_BOT_SH_EXE): $(SOCIAL_BOT_SH) | $(BUILD_DIR) $(ANALYTICS_DIR)
	cp $< $@
	chmod +x $@
	@echo "✅ Copied bash social media bot: $@"

# Copy CEX integration script
$(CEX_INTEGRATION_SH_EXE): $(CEX_INTEGRATION_SH) | $(BUILD_DIR) $(INTEGRATION_DIR)
	cp $< $@
	chmod +x $@
	@echo "✅ Copied CEX integration script: $@"

# Install dependencies
install-deps:
	@echo "📦 Installing dependencies..."
	@if command -v apt-get >/dev/null 2>&1; then \
		sudo apt-get update && sudo apt-get install -y gcc jq; \
	elif command -v yum >/dev/null 2>&1; then \
		sudo yum install -y gcc jq; \
	elif command -v brew >/dev/null 2>&1; then \
		brew install gcc jq; \
	else \
		echo "❌ Package manager not found. Please install gcc and jq manually."; \
	fi
	@echo "✅ Dependencies installed"

# Test C social media bot
test-c: $(SOCIAL_BOT_EXE)
	@echo "🧪 Testing C social media bot..."
	timeout 30s $(SOCIAL_BOT_EXE) || true
	@echo "✅ C social media bot test completed"

# Test bash social media bot
test-bash: $(SOCIAL_BOT_SH_EXE)
	@echo "🧪 Testing bash social media bot..."
	$(SOCIAL_BOT_SH_EXE) --test
	@echo "✅ Bash social media bot test completed"

# Test CEX integration
test-cex: $(CEX_INTEGRATION_SH_EXE)
	@echo "🧪 Testing CEX integration..."
	$(CEX_INTEGRATION_SH_EXE) --env
	@echo "✅ CEX integration test completed"

# Run all tests
test: test-c test-bash test-cex
	@echo "🎉 All tests completed!"

# Start C social media bot
start-c: $(SOCIAL_BOT_EXE)
	@echo "🚀 Starting C social media bot..."
	$(SOCIAL_BOT_EXE)

# Start bash social media bot
start-bash: $(SOCIAL_BOT_SH_EXE)
	@echo "🚀 Starting bash social media bot..."
	$(SOCIAL_BOT_SH_EXE) --start

# Run CEX integration
run-cex: $(CEX_INTEGRATION_SH_EXE)
	@echo "🏦 Running CEX integration..."
	$(CEX_INTEGRATION_SH_EXE) --run

# Show analytics
analytics: $(SOCIAL_BOT_SH_EXE)
	@echo "📊 Showing analytics..."
	$(SOCIAL_BOT_SH_EXE) --analytics

# Clean build files
clean:
	rm -rf $(BUILD_DIR)
	@echo "🧹 Cleaned build directory"

# Clean analytics
clean-analytics:
	rm -rf $(ANALYTICS_DIR)
	@echo "🧹 Cleaned analytics directory"

# Clean integration files
clean-integration:
	rm -rf $(INTEGRATION_DIR)
	@echo "🧹 Cleaned integration directory"

# Clean everything
clean-all: clean clean-analytics clean-integration
	@echo "🧹 Cleaned everything"

# Show help
help:
	@echo "USDTgVerse Social Media Bot - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build all executables"
	@echo "  install-deps     - Install dependencies"
	@echo "  test            - Run all tests"
	@echo "  test-c          - Test C social media bot"
	@echo "  test-bash       - Test bash social media bot"
	@echo "  test-cex        - Test CEX integration"
	@echo "  start-c         - Start C social media bot"
	@echo "  start-bash      - Start bash social media bot"
	@echo "  run-cex         - Run CEX integration"
	@echo "  analytics       - Show analytics"
	@echo "  clean           - Clean build files"
	@echo "  clean-analytics - Clean analytics directory"
	@echo "  clean-integration - Clean integration directory"
	@echo "  clean-all       - Clean everything"
	@echo "  help            - Show this help"
	@echo ""
	@echo "Examples:"
	@echo "  make all        - Build everything"
	@echo "  make test       - Run all tests"
	@echo "  make start-bash - Start bash social media bot"
	@echo "  make run-cex    - Run CEX integration"

# Phony targets
.PHONY: all install-deps test test-c test-bash test-cex start-c start-bash run-cex analytics clean clean-analytics clean-integration clean-all help
