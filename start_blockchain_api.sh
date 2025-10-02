#!/bin/bash

# USDTgVerse Blockchain API Server Launcher
# Author: Irfan Gedik
# Start Date: 2025-10-02

echo "🚀 Starting USDTgVerse Blockchain API Server..."

# Check if blockchain_api binary exists
if [ ! -f "api/blockchain_api" ]; then
    echo "⚠️  Blockchain API binary not found. Compiling from source..."
    
    # Check if source file exists
    if [ -f "api/blockchain_api.cpp" ]; then
        echo "📝 Found source code. Compiling..."
        
        # Compile the blockchain API
        g++ -std=c++17 -O2 -o api/blockchain_api api/blockchain_api.cpp \
            -I./src/core \
            ./src/core/blockchain_core_impl.c \
            -lpthread \
            -lcurl
            
        if [ $? -eq 0 ]; then
            echo "✅ Blockchain API compiled successfully!"
        else
            echo "❌ Compilation failed. Please check dependencies."
            exit 1
        fi
    else
        echo "❌ Source file not found at api/blockchain_api.cpp"
        exit 1
    fi
fi

echo "🌐 Starting API server on port 8001..."

# Set environment variables
export PORT=8001
export API_HOST=0.0.0.0
export LOG_LEVEL=INFO

# Start the blockchain API server
./api/blockchain_api

echo "🏁 Blockchain API server started!"
echo "📍 API Base URL: http://localhost:8001"
echo "🔗 Endpoints:"
echo "   - GET  /api/v1/blocks/{number}"
echo "   - GET  /api/v1/transactions/{hash}"
echo "   - GET  /api/v1/addresses/{address}"
echo "   - POST /api/v1/transactions/send"
echo "   - GET  /api/v1/stats"
