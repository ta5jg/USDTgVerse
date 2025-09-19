#!/bin/bash

# SFO2 Bridge Hub Setup Script
# Role: Secondary Validator + Multi-Chain Bridges + DEX/CEX
# IP: 157.245.225.95

set -e

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m'

echo -e "${PURPLE}🌉 SFO2 BRIDGE HUB SETUP${NC}"
echo -e "${PURPLE}=========================${NC}"
echo ""

# Create systemd service for blockchain node (Validator #2)
cat > /etc/systemd/system/usdtgverse-node.service << EOF
[Unit]
Description=USDTgVerse Blockchain Node (Secondary)
After=network.target
StartLimitIntervalSec=0

[Service]
Type=simple
Restart=always
RestartSec=1
User=root
WorkingDirectory=/opt/usdtgverse
ExecStart=/opt/usdtgverse/usdtgverse_node 26657 2
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
EOF

# Create systemd service for Bridge Orchestrator
cat > /etc/systemd/system/usdtgverse-bridges.service << EOF
[Unit]
Description=USDTgVerse Multi-Chain Bridge Orchestrator
After=network.target usdtgverse-node.service
StartLimitIntervalSec=0

[Service]
Type=simple
Restart=always
RestartSec=1
User=root
WorkingDirectory=/opt/usdtgverse
ExecStart=/opt/usdtgverse/bridges/master_orchestrator
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
EOF

# Create systemd service for API Gateway (backup)
cat > /etc/systemd/system/usdtgverse-api.service << EOF
[Unit]
Description=USDTgVerse API Gateway (Backup)
After=network.target usdtgverse-node.service
StartLimitIntervalSec=0

[Service]
Type=simple
Restart=always
RestartSec=1
User=root
WorkingDirectory=/opt/usdtgverse
ExecStart=/opt/usdtgverse/api_gateway 3001
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
EOF

# Create systemd service for Frontend (backup)
cat > /etc/systemd/system/usdtgverse-frontend.service << EOF
[Unit]
Description=USDTgVerse Frontend Server (Backup)
After=network.target
StartLimitIntervalSec=0

[Service]
Type=simple
Restart=always
RestartSec=1
User=root
WorkingDirectory=/opt/usdtgverse
ExecStart=/usr/bin/python3 -m http.server 8080
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
EOF

# Compile all bridge components
echo -e "${BLUE}🌉 Compiling bridge components...${NC}"
cd /opt/usdtgverse

gcc -std=c99 -O3 -pthread -o bridges/eth_bridge bridges/ethereum/eth_bridge.c
gcc -std=c99 -O3 -pthread -o bridges/tron_bridge bridges/tron/tron_bridge.c
gcc -std=c99 -O3 -pthread -o bridges/solana_bridge bridges/solana/solana_bridge.c
gcc -std=c99 -O3 -pthread -o bridges/bnb_bridge bridges/bnb/bnb_bridge.c
gcc -std=c99 -O3 -pthread -o bridges/polygon_bridge bridges/polygon/polygon_bridge.c
gcc -std=c99 -O3 -pthread -o bridges/arbitrum_bridge bridges/arbitrum/arbitrum_bridge.c
gcc -std=c99 -O3 -pthread -o bridges/avalanche_bridge bridges/avalanche/avalanche_bridge.c
gcc -std=c99 -O3 -pthread -o bridges/optimism_bridge bridges/optimism/optimism_bridge.c

# Configure Nginx for bridge services
cat > /etc/nginx/sites-available/usdtgverse-bridges << EOF
server {
    listen 80;
    server_name bridges.usdtgverse.com;
    
    # Bridge API routes
    location /bridges/ {
        proxy_pass http://127.0.0.1:3002;
        proxy_set_header Host \$host;
        proxy_set_header X-Real-IP \$remote_addr;
        proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto \$scheme;
    }
    
    # DEX/CEX routes
    location /dex/ {
        root /opt/usdtgverse/exchanges/web;
        try_files \$uri \$uri/ =404;
    }
    
    location /cex/ {
        root /opt/usdtgverse/exchanges/web;
        try_files \$uri \$uri/ =404;
    }
}
EOF

# Enable nginx site
ln -sf /etc/nginx/sites-available/usdtgverse-bridges /etc/nginx/sites-enabled/

# Reload systemd and enable services
systemctl daemon-reload
systemctl enable usdtgverse-node
systemctl enable usdtgverse-bridges
systemctl enable usdtgverse-api
systemctl enable usdtgverse-frontend
systemctl enable nginx

# Start services
systemctl start usdtgverse-node
sleep 5
systemctl start usdtgverse-bridges
systemctl start usdtgverse-api
systemctl start usdtgverse-frontend
systemctl restart nginx

echo -e "${GREEN}✅ SFO2 Bridge Hub setup complete!${NC}"
echo -e "${BLUE}🔧 Services started:${NC}"
echo -e "${YELLOW}   - USDTgVerse Blockchain Node (Port 26657)${NC}"
echo -e "${YELLOW}   - Multi-Chain Bridge Orchestrator${NC}"
echo -e "${YELLOW}   - API Gateway (Backup, Port 3001)${NC}"
echo -e "${YELLOW}   - Frontend Server (Backup, Port 8080)${NC}"
echo -e "${YELLOW}   - Bridge Services (Port 3002)${NC}"
echo -e "${YELLOW}   - DEX/CEX Interfaces${NC}"
