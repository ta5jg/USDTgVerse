#!/bin/bash

# NYC3 Primary Node Setup Script
# Role: Primary Validator + API Gateway + Frontend
# IP: 159.203.83.98

set -e

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m'

echo -e "${PURPLE}🏙️ NYC3 PRIMARY NODE SETUP${NC}"
echo -e "${PURPLE}============================${NC}"
echo ""

# Create systemd service for blockchain node
cat > /etc/systemd/system/usdtgverse-node.service << EOF
[Unit]
Description=USDTgVerse Blockchain Node (Primary)
After=network.target
StartLimitIntervalSec=0

[Service]
Type=simple
Restart=always
RestartSec=1
User=root
WorkingDirectory=/opt/usdtgverse
ExecStart=/opt/usdtgverse/usdtgverse_node 26656 1
StandardOutput=journal
StandardError=journal

[Install]
WantedBy=multi-user.target
EOF

# Create systemd service for API Gateway
cat > /etc/systemd/system/usdtgverse-api.service << EOF
[Unit]
Description=USDTgVerse API Gateway
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

# Create systemd service for Frontend
cat > /etc/systemd/system/usdtgverse-frontend.service << EOF
[Unit]
Description=USDTgVerse Frontend Server
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

# Configure Nginx for load balancing
cat > /etc/nginx/sites-available/usdtgverse << EOF
upstream usdtgverse_api {
    server 127.0.0.1:3001;
    server 157.245.225.95:3001 backup;
    server 104.248.251.209:3001 backup;
}

upstream usdtgverse_frontend {
    server 127.0.0.1:8080;
    server 157.245.225.95:8080 backup;
    server 104.248.251.209:8080 backup;
}

server {
    listen 80;
    server_name usdtgverse.com www.usdtgverse.com;
    
    # API routes
    location /api/ {
        proxy_pass http://usdtgverse_api;
        proxy_set_header Host \$host;
        proxy_set_header X-Real-IP \$remote_addr;
        proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto \$scheme;
    }
    
    # Frontend routes
    location / {
        proxy_pass http://usdtgverse_frontend;
        proxy_set_header Host \$host;
        proxy_set_header X-Real-IP \$remote_addr;
        proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto \$scheme;
    }
}
EOF

# Enable nginx site
ln -sf /etc/nginx/sites-available/usdtgverse /etc/nginx/sites-enabled/
rm -f /etc/nginx/sites-enabled/default

# Reload systemd and enable services
systemctl daemon-reload
systemctl enable usdtgverse-node
systemctl enable usdtgverse-api
systemctl enable usdtgverse-frontend
systemctl enable nginx

# Start services
systemctl start usdtgverse-node
sleep 5
systemctl start usdtgverse-api
systemctl start usdtgverse-frontend
systemctl restart nginx

echo -e "${GREEN}✅ NYC3 Primary Node setup complete!${NC}"
echo -e "${BLUE}🔧 Services started:${NC}"
echo -e "${YELLOW}   - USDTgVerse Blockchain Node (Port 26656)${NC}"
echo -e "${YELLOW}   - API Gateway (Port 3001)${NC}"
echo -e "${YELLOW}   - Frontend Server (Port 8080)${NC}"
echo -e "${YELLOW}   - Nginx Load Balancer (Port 80)${NC}"
