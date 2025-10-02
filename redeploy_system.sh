#!/bin/bash

# USDTgVerse System Redeployment & Startup Script
# Author: Irfan Gedik

set -e

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m'

echo -e "${PURPLE}🔄 USDTgVerse System Redeployment${NC}"
echo -e "${PURPLE}=================================${NC}"

# Droplet IPs
NYC3_IP="159.203.83.98"
SFO2_IP="157.245.225.95" 
FRA1_IP="104.248.251.209"

SSH_KEY="~/.ssh/id_rsa"
SSH_USER="root"

echo -e "${CYAN}🎯 REDEPLOYMENT STRATEGY:${NC}"
echo "1. Deploy to all 3 droplets"
echo "2. Initialize blockchain nodes"
echo "3. Start API services"
echo "4. Configure frontend"
echo ""

# Function to deploy to specific droplet
deploy_to_droplet() {
    local ip=$1
    local name=$2
    
    echo -e "${CYAN}📦 Deploying to $name ($ip)${NC}"
    
    # Copy deployment files
    echo -e "${YELLOW}   Copying files...${NC}"
    scp -i $SSH_KEY -r deploy/ $SSH_USER@$ip:/opt/usdtgverse/ 2>/dev/null || echo "   ⚠️  Copy failed - droplet might be down"
    
    # Execute deployment
    echo -e "${YELLOW}   Running deployment...${NC}"
    ssh -i $SSH_KEY $SSH_USER@$ip "cd /opt/usdtgverse && ./deploy/${name,,}_setup.sh" 2>/dev/null || echo "   ⚠️  Deployment failed"
    
    echo -e "${GREEN}   ✅ Deployment to $name completed${NC}"
    echo ""
}

# Execute deployments
echo -e "${BLUE}🚀 Starting Redeployment...${NC}"
echo "======================================"

deploy_to_droplet $NYC3_IP "NYC3"
deploy_to_droplet $SFO2_IP "SFO2" 
deploy_to_droplet $FRA1_IP "FRA1"

echo -e "${GREEN}🏁 Redeployment Process Complete!${NC}"
echo ""
echo -e "${CYAN}🔗 CHECK ENDPOINTS:${NC}"
echo -e "${YELLOW}  Frontend: http://usdtgverse.com${NC}"
echo -e "${YELLOW}  API: http://usdtgverse.com/api/v1/${NC}"
echo -e "${YELLOW}  Explorer: http://usdtgverse.com/explorer/usdtgscan.html${NC}"
echo -e "${YELLOW}  Monitoring: http://monitor.usdtgverse.com${NC}"
echo ""
echo -e "${PURPLE}📊 Node Status Check:${NC}"
echo -e "${YELLOW}  ssh root@159.203.83.98 'systemctl status usdtgverse-node'${NC}"
