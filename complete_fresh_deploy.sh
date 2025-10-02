#!/bin/bash

# USDTgVerse Complete Fresh Deployment Script
# Author: Irfan Gedik
# Complete system redeployment from local files

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${PURPLE}🚀 USDTgVerse Complete Fresh Deployment${NC}"
echo -e "${PURPLE}======================================${NC}"

# Server configuration
SERVER_IP="159.203.83.98"
SSH_USER="root"

echo -e "${CYAN}🎯 Deploying fresh system to: $SERVER_IP${NC}"
echo ""

# Stop services and clean directory
echo -e "${BLUE}🛑 Stopping services and cleaning server...${NC}"
ssh $SSH_USER@$SERVER_IP '
echo "Stopping all USDTg services..."
systemctl stop usdtgverse-* 2>/dev/null || true

echo "Cleaning deployment directory..."
rm -rf /opt/usdtgverse-new
mkdir -p /opt/usdtgverse-new

echo "✅ Server cleaned and ready for fresh deployment"
'

# Deploy main website files
echo -e "${BLUE}📱 Deploying main website files...${NC}"
scp /Users/irfangedik/USDTgVerse/index.html $SSH_USER@$SERVER_IP:/opt/usdtgverse-new/
scp /Users/irfangedik/USDTgVerse/website/index.html $SSH_USER@$SERVER_IP:/opt/usdtgverse-new/website-homepage.html
scp -r /Users/irfangedik/USDTgVerse/explorer/ $SSH_USER@$SERVER_IP:/opt/usdtgverse-new/
scp -r /Users/irfangedik/USDTgVerse/styles/ $SSH_USER@$SERVER_IP:/opt/usdtgverse-new/
scp -r /Users/irfangedik/USDTgVerse/assets/ $SSH_USER@$SERVER_IP:/opt/usdtgverse-new/ 2>/dev/null || echo "Assets directory not found, skipping..."

# Deploy production API
echo -e "${BLUE}🔧 Deploying production API...${NC}"
scp /Users/irfangedik/USDTgVerse/production_api.py $SSH_USER@$SERVER_IP:/opt/usdtgverse-new/

# Switch to new deployment
ssh $SSH_USER@$SERVER_IP '
echo "🔄 Switching to fresh deployment..."

# Backup old deployment 
mv /opt/usdtgverse /opt/usdtgverse-old-$(date +%Y%m%d-%H%M)
mv /opt/usdtgverse-new /opt/usdtgverse

# Set permissions
chmod +x /opt/usdtgverse/production_api.py
chown -R root:root /opt/usdtgverse

echo "✅ Fresh deployment active"
'

# Configure services
ssh $SSH_USER@$SERVER_IP '
echo "🔧 Configuring services..."

# Create Python API service
cat > /etc/systemd/system/usdtgverse-python-api.service << EOF
[Unit]
Description=USDTgVerse Production Python API
After=network.target
StartLimitIntervalSec=0

[Service]
Type=simple
Restart=always  
RestartSec=3
User=root
WorkingDirectory=/opt/usdtgverse
ExecStart=/usr/bin/python3 /opt/usdtgverse/production_api.py
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
EOF

echo "✅ Service configured"
'

# Update nginx configuration
ssh $SSH_USER@$SERVER_IP '
echo "🌐 Updating Nginx configuration..."

cat > /etc/nginx/sites-available/usdtgverse << EOF
# HTTP to HTTPS redirect
server {
    listen 80;
    server_name usdtgverse.com www.usdtgverse.com;
    return 301 https://\$server_name\$request_uri;
}

# HTTPS server
server {
    listen 443 ssl http2;
    server_name usdtgverse.com www.usdtgverse.com;
    root /opt/usdtgverse;
    index index.html;
    
    # SSL Configuration
    ssl_certificate /etc/letsencrypt/live/usdtgverse.com/fullchain.pem;
    ssl_certificate_key /etc/letsencrypt/live/usdtgverse.com/privkey.pem;
    
    # Security settings
    ssl_protocols TLSv1.2 TLSv1.3;
    ssl_prefer_server_ciphers off;
    
    # Main Dashboard
    location / {
        try_files \$uri \$uri/ =404;
    }
    
    # Enterprise Website
    location = /website {
        alias /opt/usdtgverse/website-homepage.html;
    }
    
    # Explorer
    location /explorer/ {
        alias /opt/usdtgverse/explorer/;
        index Usdtgscan.html;
    }
    
    # API
    location /api/ {
        proxy_pass http://127.0.0.1:3001/;
        proxy_set_header Host \$host;
        proxy_set_header X-Real-IP \$remote_addr;
        proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto \$scheme;
    }
}
EOF

echo "✅ Nginx configured"
'

# Enable and start services
ssh $SSH_USER@$SERVER_IP '
echo "🚀 Starting fresh services..."

systemctl daemon-reload
systemctl enable usdtgverse-python-api
systemctl start usdtgverse-python-api

nginx -t && systemctl restart nginx

echo "✅ Fresh services started"
'

# Final verification
echo -e "${GREEN}🏁 DEPLOYMENT COMPLETED${NC}"
echo -e "${CYAN}🔗 Testing fresh deployment:${NC}"
ssh $SSH_USER@$SERVER_IP '
echo "Frontend Test: $(curl -s http://localhost | grep -o "<title>[^<]*" | head -1)"
echo "API Test: $(curl -s http://localhost:3001/api/status 2>/dev/null | grep -o "\"status\": \"[^\"]*\"" || echo "API not responding")"
'

echo ""
echo -e "${GREEN}🎉 Fresh deployment completed!${NC}"
echo -e "${CYAN}Endpoints ready:${NC}"
echo -e "${YELLOW}  HTTPS Frontend: https://usdtgverse.com${NC}"
echo -e "${YELLOW}  Enterprise Site: https://usdtgverse.com/website${NC}"  
echo -e "${YELLOW}  Explorer: https://usdtgverse.com/explorer/${NC}"
echo -e "${YELLOW}  API: http://$SERVER_IP/api/status${NC}"
