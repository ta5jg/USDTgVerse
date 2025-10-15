#!/bin/bash

echo "🚀 Starting USDTgVerse Dashboard API Server..."
echo "⚡ Pure C Backend - Maximum Performance!"

# Kill any existing dashboard API server
pkill -f dashboard-api-server

# Start the dashboard API server
cd /Users/irfangedik/USDTgVerse
./dashboard-api-server &

# Get the process ID
DASHBOARD_PID=$!

# Save PID to file
echo $DASHBOARD_PID > dashboard_api.pid

echo "📊 Dashboard API Server started with PID: $DASHBOARD_PID"
echo "🌐 API Endpoint: http://localhost:8080/api/dashboard"
echo "📈 Serving real-time dashboard metrics..."
echo "🔄 Auto-refresh every 30 seconds"

# Test the API endpoint
sleep 2
echo ""
echo "🧪 Testing API endpoint..."
curl -s http://localhost:8080/api/dashboard | head -c 200
echo ""
echo ""
echo "✅ Dashboard API Server is ready!"
echo "💡 Use 'pkill -f dashboard-api-server' to stop the server"
