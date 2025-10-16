# USDTgVerse Pure C Audit System Makefile
# Author: Irfan Gedik
# Version: 1.0

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -pthread
LDFLAGS = -pthread
TARGETS = audit_api_c_native audit_api_real_production
SOURCES = audit_api_c_native.c audit_api_real_production.c

# Default target
all: $(TARGETS)

# Build native audit API
audit_api_c_native: audit_api_c_native.c
	@echo "🔧 Building audit_api_c_native..."
	$(CC) $(CFLAGS) -o audit_api_c_native audit_api_c_native.c $(LDFLAGS)
	@echo "✅ audit_api_c_native built successfully"

# Build production audit API
audit_api_real_production: audit_api_real_production.c
	@echo "🔧 Building audit_api_real_production..."
	$(CC) $(CFLAGS) -o audit_api_real_production audit_api_real_production.c $(LDFLAGS)
	@echo "✅ audit_api_real_production built successfully"

# Run native audit API
run-native: audit_api_c_native
	@echo "🚀 Starting native audit API server..."
	./audit_api_c_native

# Run production audit API
run-production: audit_api_real_production
	@echo "🚀 Starting production audit API server..."
	./audit_api_real_production

# Run both servers (in background)
run-both: audit_api_c_native audit_api_real_production
	@echo "🚀 Starting both audit API servers..."
	./audit_api_c_native &
	./audit_api_real_production &
	@echo "✅ Both servers started in background"
	@echo "🌐 Native API: http://localhost:8080"
	@echo "🌐 Production API: http://localhost:8081"

# Stop all servers
stop:
	@echo "🛑 Stopping all audit API servers..."
	@pkill -f audit_api_c_native || true
	@pkill -f audit_api_real_production || true
	@echo "✅ All servers stopped"

# Clean build files
clean:
	@echo "🧹 Cleaning build files..."
	rm -f $(TARGETS)
	@echo "✅ Clean completed"

# Install dependencies
install-deps:
	@echo "📦 Installing dependencies..."
	@echo "✅ No external dependencies required (Pure C)"

# Test the APIs
test:
	@echo "🧪 Testing audit APIs..."
	@echo "Testing native API..."
	@curl -s http://localhost:8080/contracts | head -c 100 || echo "❌ Native API not running"
	@echo ""
	@echo "Testing production API..."
	@curl -s http://localhost:8081/contracts | head -c 100 || echo "❌ Production API not running"

# Show help
help:
	@echo "USDTgVerse Pure C Audit System Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all              - Build all targets"
	@echo "  audit_api_c_native - Build native audit API"
	@echo "  audit_api_real_production - Build production audit API"
	@echo "  run-native       - Run native audit API server"
	@echo "  run-production   - Run production audit API server"
	@echo "  run-both         - Run both servers in background"
	@echo "  stop             - Stop all servers"
	@echo "  clean            - Clean build files"
	@echo "  install-deps     - Install dependencies"
	@echo "  test             - Test the APIs"
	@echo "  help             - Show this help"
	@echo ""
	@echo "Examples:"
	@echo "  make all         - Build everything"
	@echo "  make run-both    - Start both servers"
	@echo "  make test        - Test running servers"

.PHONY: all run-native run-production run-both stop clean install-deps test help
