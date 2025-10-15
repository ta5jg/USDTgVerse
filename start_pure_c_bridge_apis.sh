#!/bin/bash

# Pure C Bridge API Servers Startup Script
# Author: Irfan Gedik
# Description: Starts all Pure C backend servers for the bridge system

echo "🚀 Starting Pure C Bridge API Servers..."

# Compile Binance API server
echo "🔨 Compiling Binance API server..."
clang -o binance-api-pure-c binance-api-pure-c.c -lssl -lcrypto -ljson-c

if [ $? -eq 0 ]; then
    echo "✅ Binance API server compiled successfully!"
else
    echo "❌ Binance API server compilation failed!"
    exit 1
fi

# Compile Bridge API server
echo "🔨 Compiling Bridge API server..."
clang -o bridge-api-pure-c bridge-api-pure-c.c -lm

if [ $? -eq 0 ]; then
    echo "✅ Bridge API server compiled successfully!"
else
    echo "❌ Bridge API server compilation failed!"
    exit 1
fi

# Start Binance API server in background
echo "🚀 Starting Binance API server on port 8082..."
./binance-api-pure-c &
BINANCE_PID=$!

# Start Bridge API server in background
echo "🚀 Starting Bridge API server on port 8083..."
./bridge-api-pure-c &
BRIDGE_PID=$!

# Wait a moment for servers to start
sleep 2

# Check if servers are running
if kill -0 $BINANCE_PID 2>/dev/null; then
    echo "✅ Binance API server is running (PID: $BINANCE_PID)"
else
    echo "❌ Binance API server failed to start"
fi

if kill -0 $BRIDGE_PID 2>/dev/null; then
    echo "✅ Bridge API server is running (PID: $BRIDGE_PID)"
else
    echo "❌ Bridge API server failed to start"
fi

echo ""
echo "🌉 Pure C Bridge System is ready!"
echo "📡 Binance API: http://localhost:8082"
echo "📡 Bridge API: http://localhost:8083"
echo ""
echo "🔧 Available endpoints:"
echo "   Binance API:"
echo "     GET /api/bridge/balance/{asset}"
echo "     GET /api/bridge/price/{symbol}"
echo "     GET /api/bridge/rate/{from}/{to}/{amount}"
echo ""
echo "   Bridge API:"
echo "     GET /api/bridge/networks"
echo "     GET /api/bridge/rate/{from}/{to}/{token}/{amount}"
echo "     GET /api/bridge/stats"
echo "     GET /api/bridge/status/{network}"
echo ""
echo "⏹️  To stop servers, run: kill $BINANCE_PID $BRIDGE_PID"

# Keep script running
wait
