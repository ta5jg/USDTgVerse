#!/bin/bash

# Pure C Dashboard Charts API Server Startup Script

echo "🔨 Compiling Pure C Charts API server..."
clang -o dashboard-charts-api dashboard-charts-api.c -lm

if [ $? -eq 0 ]; then
    echo "✅ Compile successful!"
    echo "🚀 Starting Pure C Charts API server on port 8081..."
    echo "📊 Serving real-time chart data..."
    ./dashboard-charts-api
else
    echo "❌ Compile failed!"
    exit 1
fi
