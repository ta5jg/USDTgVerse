#!/bin/bash

# Multi-Chain Bridge API Server Startup Script

echo "🌉 Compiling Multi-Chain Bridge API server..."
clang -o bridge-api-server bridge-api-server.c -lm

if [ $? -eq 0 ]; then
    echo "✅ Compile successful!"
    echo "🚀 Starting Multi-Chain Bridge API server on port 8082..."
    echo "🌐 Supporting BSC, Tron, Ethereum, Polygon, Avalanche..."
    echo "💰 Token bridges: USDT, USDC, BNB, ETH, TRX → USDTgVerse tokens"
    ./bridge-api-server
else
    echo "❌ Compile failed!"
    exit 1
fi
