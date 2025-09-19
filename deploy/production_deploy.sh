#!/bin/bash

# USDTgVerse Production Deployment Script
# 3-Droplet Global Mainnet Deployment
# 
# Droplets:
# - NYC3 (159.203.83.98): Primary Node
# - SFO2 (157.245.225.95): Bridge Hub  
# - FRA1 (104.248.251.209): Monitoring Hub

set -e

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Server configurations
NYC3_IP="159.203.83.98"
SFO2_IP="157.245.225.95"
FRA1_IP="104.248.251.209"

SSH_KEY="~/.ssh/id_rsa"
SSH_USER="root"

echo -e "${PURPLE}🌍 USDTgVerse Global Mainnet Deployment${NC}"
echo -e "${PURPLE}===============================================${NC}"
echo ""

# Function to execute command on remote server
execute_remote() {
    local server_ip=$1
    local command=$2
    local server_name=$3
    
    echo -e "${CYAN}🖥️  Executing on $server_name ($server_ip):${NC}"
    echo -e "${YELLOW}   Command: $command${NC}"
    
    ssh -i $SSH_KEY -o StrictHostKeyChecking=no $SSH_USER@$server_ip "$command"
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}   ✅ Success on $server_name${NC}"
    else
        echo -e "${RED}   ❌ Failed on $server_name${NC}"
        exit 1
    fi
    echo ""
}

# Function to copy files to remote server
copy_to_remote() {
    local server_ip=$1
    local local_path=$2
    local remote_path=$3
    local server_name=$4
    
    echo -e "${CYAN}📦 Copying to $server_name ($server_ip):${NC}"
    echo -e "${YELLOW}   From: $local_path${NC}"
    echo -e "${YELLOW}   To: $remote_path${NC}"
    
    scp -i $SSH_KEY -r $local_path $SSH_USER@$server_ip:$remote_path
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}   ✅ Copy successful to $server_name${NC}"
    else
        echo -e "${RED}   ❌ Copy failed to $server_name${NC}"
        exit 1
    fi
    echo ""
}

echo -e "${BLUE}🚀 Phase 1: Server Environment Setup${NC}"
echo "=============================================="

# Update all servers
execute_remote $NYC3_IP "apt update && apt upgrade -y" "NYC3"
execute_remote $SFO2_IP "apt update && apt upgrade -y" "SFO2" 
execute_remote $FRA1_IP "apt update && apt upgrade -y" "FRA1"

# Install dependencies on all servers
INSTALL_DEPS="apt install -y build-essential cmake git curl wget python3 python3-pip nodejs npm nginx certbot python3-certbot-nginx htop screen"

execute_remote $NYC3_IP "$INSTALL_DEPS" "NYC3"
execute_remote $SFO2_IP "$INSTALL_DEPS" "SFO2"
execute_remote $FRA1_IP "$INSTALL_DEPS" "FRA1"

echo -e "${BLUE}📦 Phase 2: Codebase Deployment${NC}"
echo "=============================================="

# Create deployment directory on all servers
execute_remote $NYC3_IP "mkdir -p /opt/usdtgverse" "NYC3"
execute_remote $SFO2_IP "mkdir -p /opt/usdtgverse" "SFO2"
execute_remote $FRA1_IP "mkdir -p /opt/usdtgverse" "FRA1"

# Clone repository on all servers
CLONE_CMD="cd /opt && rm -rf usdtgverse && git clone https://github.com/ta5jg/USDTgVerse.git usdtgverse"

execute_remote $NYC3_IP "$CLONE_CMD" "NYC3"
execute_remote $SFO2_IP "$CLONE_CMD" "SFO2"
execute_remote $FRA1_IP "$CLONE_CMD" "FRA1"

echo -e "${BLUE}🔧 Phase 3: Blockchain Node Compilation${NC}"
echo "=============================================="

# Compile blockchain components on all servers
COMPILE_CMD="cd /opt/usdtgverse && gcc -std=c99 -O3 -pthread -o usdtgverse_node src/main.c && gcc -std=c99 -O3 -pthread -o api_gateway api/gateway.c"

execute_remote $NYC3_IP "$COMPILE_CMD" "NYC3"
execute_remote $SFO2_IP "$COMPILE_CMD" "SFO2"
execute_remote $FRA1_IP "$COMPILE_CMD" "FRA1"

echo -e "${BLUE}🌉 Phase 4: Bridge Network Compilation${NC}"
echo "=============================================="

# Compile bridge components
BRIDGE_CMD="cd /opt/usdtgverse && gcc -std=c99 -O3 -pthread -o bridges/master_orchestrator bridges/master_bridge_orchestrator.c"

execute_remote $NYC3_IP "$BRIDGE_CMD" "NYC3"
execute_remote $SFO2_IP "$BRIDGE_CMD" "SFO2" 
execute_remote $FRA1_IP "$BRIDGE_CMD" "FRA1"

echo -e "${GREEN}🎉 Deployment Phase 1 Complete!${NC}"
echo -e "${YELLOW}Next: Run node-specific configurations${NC}"
