# USDTgVerse Payment System Integrations Makefile
# Author: Irfan Gedik
# Created: 13.10.2025

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lcurl -lssl -lcrypto

# Source files
STRIPE_SRC = stripe_live_integration.c
COINGECKO_SRC = coingecko_live_integration.c
BINANCE_SRC = binance_live_integration.c
OPEN_BANKING_SRC = open_banking_live_integration.c
PLAID_SRC = plaid_live_integration.c

# Executables
STRIPE_EXEC = stripe_live_integration
COINGECKO_EXEC = coingecko_live_integration
BINANCE_EXEC = binance_live_integration
OPEN_BANKING_EXEC = open_banking_live_integration
PLAID_EXEC = plaid_live_integration

# All executables
ALL_EXEC = $(STRIPE_EXEC) $(COINGECKO_EXEC) $(BINANCE_EXEC) $(OPEN_BANKING_EXEC) $(PLAID_EXEC)

# Default target
all: $(ALL_EXEC)

# Stripe integration
$(STRIPE_EXEC): $(STRIPE_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# CoinGecko integration
$(COINGECKO_EXEC): $(COINGECKO_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Binance integration
$(BINANCE_EXEC): $(BINANCE_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Open Banking integration
$(OPEN_BANKING_EXEC): $(OPEN_BANKING_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Plaid integration
$(PLAID_EXEC): $(PLAID_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Clean target
clean:
	rm -f $(ALL_EXEC)

# Install dependencies
install-deps:
	@echo "Installing dependencies..."
	@if command -v brew >/dev/null 2>&1; then \
		brew install curl json-c openssl; \
	elif command -v apt-get >/dev/null 2>&1; then \
		sudo apt-get update && sudo apt-get install -y libcurl4-openssl-dev libjson-c-dev libssl-dev; \
	elif command -v yum >/dev/null 2>&1; then \
		sudo yum install -y libcurl-devel json-c-devel openssl-devel; \
	else \
		echo "Please install curl, json-c, and openssl development libraries manually"; \
	fi

# Test all integrations
test: all
	@echo "Testing Stripe integration..."
	./$(STRIPE_EXEC)
	@echo "\nTesting CoinGecko integration..."
	./$(COINGECKO_EXEC)
	@echo "\nTesting Binance integration..."
	./$(BINANCE_EXEC)
	@echo "\nTesting Open Banking integration..."
	./$(OPEN_BANKING_EXEC)
	@echo "\nTesting Plaid integration..."
	./$(PLAID_EXEC)

# Help target
help:
	@echo "USDTgVerse Payment System Integrations"
	@echo "======================================"
	@echo "Available targets:"
	@echo "  all          - Build all integrations"
	@echo "  clean        - Remove all executables"
	@echo "  install-deps - Install required dependencies"
	@echo "  test         - Build and test all integrations"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Individual targets:"
	@echo "  $(STRIPE_EXEC)      - Build Stripe integration"
	@echo "  $(COINGECKO_EXEC)   - Build CoinGecko integration"
	@echo "  $(BINANCE_EXEC)      - Build Binance integration"
	@echo "  $(OPEN_BANKING_EXEC) - Build Open Banking integration"
	@echo "  $(PLAID_EXEC)       - Build Plaid integration"

.PHONY: all clean install-deps test help
