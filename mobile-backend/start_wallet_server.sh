#!/bin/bash
#
# ==============================================
#  File:        start_wallet_server.sh
#  Author:      Irfan Gedik
#  Created:     October 2, 2025
#  Last Update: October 2, 2025
#  Version:     3.0.0 Pure C Native
#
#  Description:
#    This script starts the USDTgVerse Pure C Native Wallet API Server
#    for mobile wallet integration and database connectivity testing.
#
#  Usage:
#    ./start_wallet_server.sh
#
#  License:
#    MIT License
# ==============================================
#

echo "🚀 Starting USDTgVerse Wallet API Server..."
echo "============================================="
echo ""

# Check if binary exists
if [ ! -f "./wallet_api_server" ]; then
    echo "❌ Error: wallet_api_server binary not found!"
    echo "🔄 Compiling server..."
    gcc -O3 -o wallet_api_server wallet_api_server_pure_c.c
    if [ $? -ne 0 ]; then
        echo "❌ Compilation failed!"
        exit 1
    fi
    echo "✅ Compilation successful!"
fi

# Create data directory
echo "📂 Creating data directory..."
mkdir -p /opt/usdtgverse/data

echo "🎯 Starting Wallet API Server on port 3001..."
echo "📱 Mobile Integration: Ready"
echo "🗄️ Database Integration: Active"
echo "🎁 10 USDTg Airdrop System: Enabled"
echo ""
echo "Press Ctrl+C to stop the server"
echo ""

# Start the server
./wallet_api_server
