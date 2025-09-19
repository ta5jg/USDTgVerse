#!/bin/bash

# FRA1 Monitoring Hub Setup Script
# Role: Tertiary Validator + Explorer + Monitoring + Analytics
# IP: 104.248.251.209

set -e

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m'

echo -e "${PURPLE}📊 FRA1 MONITORING HUB SETUP${NC}"
echo -e "${PURPLE}=============================${NC}"
echo ""

# Create systemd service for blockchain node (Validator #3)
cat > /etc/systemd/system/usdtgverse-node.service << EOF
[Unit]
Description=USDTgVerse Blockchain Node (Tertiary)
After=network.target
StartLimitIntervalSec=0

[Service]
Type=simple
Restart=always
RestartSec=1
User=root
WorkingDirectory=/opt/usdtgverse
ExecStart=/opt/usdtgverse/usdtgverse_node 26658 3
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

# Install Prometheus
echo -e "${BLUE}📊 Installing Prometheus...${NC}"
wget https://github.com/prometheus/prometheus/releases/download/v2.40.0/prometheus-2.40.0.linux-amd64.tar.gz
tar xvfz prometheus-2.40.0.linux-amd64.tar.gz
mv prometheus-2.40.0.linux-amd64 /opt/prometheus
ln -sf /opt/prometheus/prometheus /usr/local/bin/
ln -sf /opt/prometheus/promtool /usr/local/bin/

# Create Prometheus user
useradd --no-create-home --shell /bin/false prometheus || true
chown -R prometheus:prometheus /opt/prometheus

# Configure Prometheus
cat > /opt/prometheus/prometheus.yml << EOF
global:
  scrape_interval: 15s
  evaluation_interval: 15s

rule_files:
  # - "first_rules.yml"

scrape_configs:
  - job_name: 'usdtgverse-nyc3'
    static_configs:
      - targets: ['159.203.83.98:26656', '159.203.83.98:3001']

  - job_name: 'usdtgverse-sfo2'
    static_configs:
      - targets: ['157.245.225.95:26657', '157.245.225.95:3001']

  - job_name: 'usdtgverse-fra1'
    static_configs:
      - targets: ['104.248.251.209:26658', '104.248.251.209:3001']

  - job_name: 'node-exporter'
    static_configs:
      - targets: ['159.203.83.98:9100', '157.245.225.95:9100', '104.248.251.209:9100']
EOF

# Create Prometheus systemd service
cat > /etc/systemd/system/prometheus.service << EOF
[Unit]
Description=Prometheus
Wants=network-online.target
After=network-online.target

[Service]
User=prometheus
Group=prometheus
Type=simple
ExecStart=/usr/local/bin/prometheus \
    --config.file /opt/prometheus/prometheus.yml \
    --storage.tsdb.path /opt/prometheus/data \
    --web.console.templates=/opt/prometheus/consoles \
    --web.console.libraries=/opt/prometheus/console_libraries \
    --web.listen-address=0.0.0.0:9090

[Install]
WantedBy=multi-user.target
EOF

# Install Grafana
echo -e "${BLUE}📈 Installing Grafana...${NC}"
wget -q -O - https://packages.grafana.com/gpg.key | apt-key add -
echo "deb https://packages.grafana.com/oss/deb stable main" > /etc/apt/sources.list.d/grafana.list
apt update
apt install -y grafana

# Configure Nginx for monitoring services
cat > /etc/nginx/sites-available/usdtgverse-monitoring << EOF
server {
    listen 80;
    server_name monitor.usdtgverse.com;
    
    # Grafana
    location / {
        proxy_pass http://127.0.0.1:3000;
        proxy_set_header Host \$host;
        proxy_set_header X-Real-IP \$remote_addr;
        proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto \$scheme;
    }
    
    # Prometheus
    location /prometheus/ {
        proxy_pass http://127.0.0.1:9090/;
        proxy_set_header Host \$host;
        proxy_set_header X-Real-IP \$remote_addr;
        proxy_set_header X-Forwarded-For \$proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto \$scheme;
    }
}

server {
    listen 80;
    server_name explorer.usdtgverse.com;
    
    # USDTgScan Explorer
    location / {
        root /opt/usdtgverse/explorer;
        try_files \$uri \$uri/ /usdtgscan.html;
    }
}
EOF

# Enable nginx site
ln -sf /etc/nginx/sites-available/usdtgverse-monitoring /etc/nginx/sites-enabled/

# Reload systemd and enable services
systemctl daemon-reload
systemctl enable usdtgverse-node
systemctl enable usdtgverse-api
systemctl enable usdtgverse-frontend
systemctl enable prometheus
systemctl enable grafana-server
systemctl enable nginx

# Start services
systemctl start usdtgverse-node
sleep 5
systemctl start usdtgverse-api
systemctl start usdtgverse-frontend
systemctl start prometheus
systemctl start grafana-server
systemctl restart nginx

echo -e "${GREEN}✅ FRA1 Monitoring Hub setup complete!${NC}"
echo -e "${BLUE}🔧 Services started:${NC}"
echo -e "${YELLOW}   - USDTgVerse Blockchain Node (Port 26658)${NC}"
echo -e "${YELLOW}   - API Gateway (Backup, Port 3001)${NC}"
echo -e "${YELLOW}   - Frontend Server (Backup, Port 8080)${NC}"
echo -e "${YELLOW}   - Prometheus Monitoring (Port 9090)${NC}"
echo -e "${YELLOW}   - Grafana Dashboard (Port 3000)${NC}"
echo -e "${YELLOW}   - USDTgScan Explorer${NC}"
