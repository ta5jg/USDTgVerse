#!/bin/bash

# USDTgVerse Auto Update Script
# Author: Irfan Gedik
# Created: 13.10.2025

echo "🚀 USDTgVerse Auto Update Starting..."
echo "===================================="

# Change to dashboard directory
cd /Users/irfangedik/USDTgVerse/dashboard

# Run the Pure C data updater
./dashboard_data_updater

# Check if update was successful
if [ $? -eq 0 ]; then
    echo "✅ Auto update completed successfully!"
    echo "📊 Dashboard updated with live data"
    echo "💰 Wallet updated with live balances"
else
    echo "❌ Auto update failed!"
    echo "🔧 Check logs for errors"
fi

# Log the update time
echo "🕒 Last update: $(date)"
echo "================================"
