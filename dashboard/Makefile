# USDTgVerse Dashboard Makefile
# Author: Irfan Gedik
# Created: 13.10.2025

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = -lcurl

# Source files
DASHBOARD_UPDATER_SRC = dashboard_data_updater.c
DASHBOARD_UPDATER_EXEC = dashboard_data_updater

# Default target
all: $(DASHBOARD_UPDATER_EXEC)

# Dashboard data updater
$(DASHBOARD_UPDATER_EXEC): $(DASHBOARD_UPDATER_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

# Update dashboard with live data
update: $(DASHBOARD_UPDATER_EXEC)
	./$(DASHBOARD_UPDATER_EXEC)

# Auto update with script
auto-update: auto_update.sh
	./auto_update.sh

# Set up cron job for automatic updates (every 5 minutes)
setup-cron:
	@echo "Setting up cron job for automatic updates..."
	@echo "*/5 * * * * cd /Users/irfangedik/USDTgVerse/dashboard && ./auto_update.sh >> /tmp/usdtgverse_update.log 2>&1" | crontab -
	@echo "✅ Cron job set up successfully!"
	@echo "Dashboard will update every 5 minutes"

# Remove cron job
remove-cron:
	@echo "Removing cron job..."
	@crontab -r
	@echo "✅ Cron job removed successfully!"

# Check cron jobs
check-cron:
	@echo "Current cron jobs:"
	@crontab -l || echo "No cron jobs found"

# Clean compiled files
clean:
	rm -f $(DASHBOARD_UPDATER_EXEC) index_temp.html

# Install dependencies
install-deps:
	@echo "Installing dependencies..."
	@if command -v brew >/dev/null 2>&1; then \
		brew install curl; \
	elif command -v apt-get >/dev/null 2>&1; then \
		sudo apt-get update && sudo apt-get install -y libcurl4-openssl-dev; \
	elif command -v yum >/dev/null 2>&1; then \
		sudo yum install -y libcurl-devel; \
	else \
		echo "Please install curl development libraries manually"; \
	fi

# Test dashboard update
test: $(DASHBOARD_UPDATER_EXEC)
	@echo "Testing dashboard update..."
	./$(DASHBOARD_UPDATER_EXEC)
	@echo "✅ Test completed!"

# Help target
help:
	@echo "USDTgVerse Dashboard Makefile"
	@echo "============================="
	@echo "Available targets:"
	@echo "  all          - Build dashboard updater"
	@echo "  update       - Update dashboard with live data"
	@echo "  auto-update  - Run auto update script"
	@echo "  setup-cron   - Set up automatic updates (every 5 minutes)"
	@echo "  remove-cron  - Remove automatic updates"
	@echo "  check-cron   - Check current cron jobs"
	@echo "  clean        - Remove compiled files"
	@echo "  install-deps - Install required dependencies"
	@echo "  test         - Test dashboard update"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make update      - Update dashboard once"
	@echo "  make setup-cron  - Set up automatic updates"
	@echo "  make test        - Test the update system"

.PHONY: all update auto-update setup-cron remove-cron check-cron clean install-deps test help
