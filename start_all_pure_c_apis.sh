#!/bin/bash

# All Pure C API Servers Startup Script
# Author: Irfan Gedik
# Description: Starts all Pure C backend servers for USDTgVerse

echo "🚀 Starting All Pure C API Servers..."

# Kill any existing servers
echo "🧹 Cleaning up existing servers..."
pkill -f "dashboard-api-server" 2>/dev/null
pkill -f "dashboard-charts-api" 2>/dev/null
pkill -f "binance-api-pure-c" 2>/dev/null
pkill -f "bridge-api-pure-c" 2>/dev/null
pkill -f "price-service-pure-c" 2>/dev/null

sleep 2

# Compile and start Dashboard API server (port 8080)
echo "🔨 Compiling Dashboard API server..."
if [ -f "dashboard-api-server.c" ]; then
    clang -o dashboard-api-server dashboard-api-server.c
    if [ $? -eq 0 ]; then
        echo "✅ Dashboard API server compiled successfully!"
        echo "🚀 Starting Dashboard API server on port 8080..."
        ./dashboard-api-server &
        DASHBOARD_PID=$!
        sleep 1
    else
        echo "❌ Dashboard API server compilation failed!"
    fi
else
    echo "⚠️  Dashboard API server source not found"
fi

# Compile and start Charts API server (port 8081)
echo "🔨 Compiling Charts API server..."
if [ -f "dashboard-charts-api.c" ]; then
    clang -o dashboard-charts-api dashboard-charts-api.c -lm
    if [ $? -eq 0 ]; then
        echo "✅ Charts API server compiled successfully!"
        echo "🚀 Starting Charts API server on port 8081..."
        ./dashboard-charts-api &
        CHARTS_PID=$!
        sleep 1
    else
        echo "❌ Charts API server compilation failed!"
    fi
else
    echo "⚠️  Charts API server source not found"
fi

# Compile and start Binance API server (port 8082)
echo "🔨 Compiling Binance API server..."
if [ -f "binance-api-pure-c.c" ]; then
    clang -o binance-api-pure-c binance-api-pure-c.c -lssl -lcrypto
    if [ $? -eq 0 ]; then
        echo "✅ Binance API server compiled successfully!"
        echo "🚀 Starting Binance API server on port 8082..."
        ./binance-api-pure-c &
        BINANCE_PID=$!
        sleep 1
    else
        echo "❌ Binance API server compilation failed!"
    fi
else
    echo "⚠️  Binance API server source not found"
fi

# Compile and start Bridge API server (port 8083)
echo "🔨 Compiling Bridge API server..."
if [ -f "bridge-api-pure-c.c" ]; then
    clang -o bridge-api-pure-c bridge-api-pure-c.c -lm
    if [ $? -eq 0 ]; then
        echo "✅ Bridge API server compiled successfully!"
        echo "🚀 Starting Bridge API server on port 8083..."
        ./bridge-api-pure-c &
        BRIDGE_PID=$!
        sleep 1
    else
        echo "❌ Bridge API server compilation failed!"
    fi
else
    echo "⚠️  Bridge API server source not found"
fi

# Compile and start Price Service server (port 8084)
echo "🔨 Compiling Price Service server..."
if [ -f "price-service-pure-c.c" ]; then
    clang -o price-service-pure-c price-service-pure-c.c -lpthread -lm
    if [ $? -eq 0 ]; then
        echo "✅ Price Service server compiled successfully!"
        echo "🚀 Starting Price Service server on port 8084..."
        ./price-service-pure-c &
        PRICE_PID=$!
        sleep 1
    else
        echo "❌ Price Service server compilation failed!"
    fi
else
    echo "⚠️  Price Service server source not found"
fi

# Check server status
echo ""
echo "🔍 Checking server status..."
sleep 3

if kill -0 $DASHBOARD_PID 2>/dev/null; then
    echo "✅ Dashboard API server is running (PID: $DASHBOARD_PID)"
else
    echo "❌ Dashboard API server failed to start"
fi

if kill -0 $CHARTS_PID 2>/dev/null; then
    echo "✅ Charts API server is running (PID: $CHARTS_PID)"
else
    echo "❌ Charts API server failed to start"
fi

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

if kill -0 $PRICE_PID 2>/dev/null; then
    echo "✅ Price Service server is running (PID: $PRICE_PID)"
else
    echo "❌ Price Service server failed to start"
fi

echo ""
echo "🌉 All Pure C API Servers Status:"
echo "📡 Dashboard API: http://localhost:8080"
echo "📊 Charts API: http://localhost:8081"
echo "🔗 Binance API: http://localhost:8082"
echo "🌉 Bridge API: http://localhost:8083"
echo "💰 Price Service: http://localhost:8084"
echo ""
echo "🎯 Available endpoints:"
echo "   Dashboard: GET /api/dashboard"
echo "   Charts: GET /api/charts"
echo "   Binance: GET /api/bridge/balance/{asset}"
echo "   Bridge: GET /api/bridge/networks"
echo "   Price: GET /api/price/native"
echo ""
echo "⏹️  To stop all servers, run: pkill -f 'pure-c'"

# Keep script running
wait
