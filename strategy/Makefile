# Major Exchanges Listing Strategy Makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g

# Source files
SOURCES = binance_listing_strategy.c coinbase_listing_strategy.c major_exchanges_airdrop_strategy.c
TARGETS = binance_listing_strategy coinbase_listing_strategy major_exchanges_airdrop_strategy

# Default target
all: $(TARGETS)

# Build targets
binance_listing_strategy: binance_listing_strategy.c
	$(CC) $(CFLAGS) -o binance_listing_strategy binance_listing_strategy.c

coinbase_listing_strategy: coinbase_listing_strategy.c
	$(CC) $(CFLAGS) -o coinbase_listing_strategy coinbase_listing_strategy.c

major_exchanges_airdrop_strategy: major_exchanges_airdrop_strategy.c
	$(CC) $(CFLAGS) -o major_exchanges_airdrop_strategy major_exchanges_airdrop_strategy.c

# Clean
clean:
	rm -f $(TARGETS)

# Test all strategies
test: $(TARGETS)
	@echo "Testing Binance Strategy..."
	./binance_listing_strategy
	@echo ""
	@echo "Testing Coinbase Strategy..."
	./coinbase_listing_strategy
	@echo ""
	@echo "Testing Airdrop Strategy..."
	./major_exchanges_airdrop_strategy

# Help
help:
	@echo "Available targets:"
	@echo "  all                              - Build all strategies"
	@echo "  binance_listing_strategy         - Build Binance strategy"
	@echo "  coinbase_listing_strategy        - Build Coinbase strategy"
	@echo "  major_exchanges_airdrop_strategy - Build airdrop strategy"
	@echo "  clean                            - Remove build artifacts"
	@echo "  test                             - Run all strategy tests"
	@echo "  help                             - Show this help message"

.PHONY: all clean test help
