#!/bin/bash

# ==============================================
#  File:        deploy_native_transfer_system.sh
#  Author:      Irfan Gedik
#  Created:     October 2, 2025
#  Last Update: October 2, 2025
#  Version:     3.0.0 Pure C Native
#
#  Description:
#    Deploy Native Coin Transfer System to droplets
#
#  Usage:
#    ./deploy_native_transfer_system.sh
#
#  License:
#    MIT License
# ==============================================

echo "🚀 USDTgVerse Native Transfer System Deployment"
echo "================================================"
echo ""

# Configuration
BACKEND_IP="164.92.146.221"
FRONTEND_IP="157.230.46.199"
NODE_IP="192.241.159.75"

echo "📡 Connection Details:"
echo "Backend Droplet (SFO2): $BACKEND_IP"
echo "Frontend Droplet (NYC3): $FRONTEND_IP"
echo "Blockchain Node (FRA1): $NODE_IP"
echo ""

# Create deployment package
echo "📦 Creating deployment package..."
cd native-transfer 2>/dev/null || {
    echo "❌ native-transfer directory not found!"
    echo "Please run from the main USDTgVerse directory"
    exit 1
}

tar -czf ../native-transfer-deployment.tar.gz *.c *.md *.sh
cd ..

if [ -f native-transfer-deployment.tar.gz ]; then
    echo "✅ Deployment package created: native-transfer-deployment.tar.gz"
else
    echo "❌ Failed to create deployment package"
    exit 1
fi

echo ""
echo "📋 DEPLOYMENT CHECKLIST:"
echo "=================================="
echo ""
echo "1️⃣ Backend Server (Port 3002):"
echo "   • Deploy transfer API server"
echo "   • Setup database integration"
echo "   • Configure nginx proxy"
echo ""
echo "2️⃣ Nginx Configuration:"
echo "   location /api/native/ {"
echo "       proxy_pass http://localhost:3002/api/v1/native/;"
echo "   }"
echo ""
echo "3️⃣ Data Directory Setup:"
echo "   mkdir -p /opt/usdtgverse/data"
echo "   • wallets.db"
echo "   • balances.db"
echo "   • transactions.db"
echo "   • transfer_logs.db"
echo ""
echo "4️⃣ Service Management:"
echo "   systemctl enable native-transfer-api"
echo "   systemctl start native-transfer-api"
echo ""

echo "🎯 MANUAL DEPLOYMENT STEPS:"
echo "==========================="
echo ""
echo "Step 1: Upload files to SFO2"
echo "-----------------------------"
echo "scp native-transfer-deployment.tar.gz root@$BACKEND_IP:/opt/usdtgverse/"
echo ""

echo "Step 2: Extract and compile on SFO2"
echo "-----------------------------------"
echo "ssh root@$BACKEND_IP"
echo "cd /opt/usdtgverse"
echo "tar -xzf native-transfer-deployment.tar.gz -C native-transfer/"
echo "cd native-transfer"
echo "gcc -O3 -o native_transfer_api transfer_api_server.c native_coin_transfer_system.c \\"
echo "    -lssl -lcrypto -lm -pthread"
echo ""

echo "Step 3: Configure nginx proxy"
echo "-----------------------------"
echo "Add to /etc/nginx/sites-available/usdtgverse:"
echo "location /api/native/ {"
echo "    proxy_pass http://localhost:3002/api/v1/native/;"
echo "    proxy_set_header Host \$host;"
echo "    proxy_set_header X-Real-IP \$remote_addr;"
echo "}"
echo ""

echo "Step 4: Create systemd service"
echo "-------------------------------"
echo "Write /etc/systemd/system/native-transfer-api.service:"
echo "[Unit]"
echo "Description=USDTgVerse Native Transfer API"
echo "After=network.target"
echo ""
echo "[Service]"
echo "Type=simple"
echo "User=root"
echo "WorkingDirectory=/opt/usdtgverse/native-transfer"
echo "ExecStart=/opt/usdtgverse/native-transfer/native_transfer_api"
echo "Restart=always"
echo "RestartSec=5"
echo ""
echo "[Install]"
echo "WantedBy=multi-user.target"
echo ""

echo "Step 5: Enable and start service"
echo "--------------------------------"
echo "systemctl daemon-reload"
echo "systemctl enable native-transfer-api"
echo "systemctl start native-transfer-api"
echo "systemctl status native-transfer-api"
echo ""

echo "🎉 DEPLOYMENT COMPLETE!"
echo "======================="
echo ""
echo "🌐 Test URLs:"
echo "• API Stats: http://$BACKEND_IP/api/native/stats"
echo "• Create Wallet: POST http://$BACKEND_IP/api/native/wallet/create"
echo "• Check Balance: GET http://$BACKEND_IP/api/native/balance/wallet_address/USDTg"
echo "• Send Transfer: POST http://$BACKEND_IP/api/native/transfer"
echo "• Send Airdrop: POST http://$BACKEND_IP/api/native/airdrop"
echo ""
echo "📱 Mobile Integration:"
echo "Update iOS/Android apps to use:"
echo "https://api.usdtgverse.com/api/native/"
echo ""
echo "🎯 READY FOR LIVE TESTING!"
