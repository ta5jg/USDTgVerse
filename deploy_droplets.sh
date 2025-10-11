#!/bin/bash
# USDTgVerse DigitalOcean Droplets Deployment Script
# Deploy updated trading interface to all 3 droplets via SSH
# 
# Author: USDTG GROUP TECHNOLOGY LLC
# Created: 2025-10-05
# Version: 1.0

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Function to print colored output
print_color() {
    local color=$1
    local message=$2
    echo -e "${color}${message}${NC}"
}

# Function to print banner
print_banner() {
    clear
    print_color $CYAN "    ╔══════════════════════════════════════════════════════════════╗"
    print_color $CYAN "    ║                                                              ║"
    print_color $CYAN "    ║        🚀 USDTgVerse DigitalOcean Droplets Deployment     ║"
    print_color $CYAN "    ║                                                              ║"
    print_color $CYAN "    ║              Deploy to All 3 Droplets via SSH             ║"
    print_color $CYAN "    ║                                                              ║"
    print_color $CYAN "    ╚══════════════════════════════════════════════════════════════╝"
    echo
}

# DigitalOcean Droplets Configuration
DROPLET_1="debian-s-4vcpu-8gb-240gb-intel-nyc3-01"
DROPLET_1_IP="159.203.83.98"
DROPLET_1_REGION="NYC3"

DROPLET_2="debian-s-4vcpu-8gb-240gb-intel-sfo2-01"
DROPLET_2_IP="157.245.225.95"
DROPLET_2_REGION="SFO2"

DROPLET_3="debian-s-4vcpu-8gb-240gb-intel-fra1-01"
DROPLET_3_IP="104.248.251.209"
DROPLET_3_REGION="FRA1"

# SSH Configuration
SSH_USER="root"
SSH_KEY="$HOME/.ssh/id_ed25519"

# Different paths for different droplets
DROPLET_1_PATH="/var/www/html/trading/"  # NYC3 - uses /var/www/html
DROPLET_2_PATH="/opt/usdtgverse/trading/"  # SFO2 - uses /opt/usdtgverse
DROPLET_3_PATH="/opt/usdtgverse/trading/"  # FRA1 - uses /opt/usdtgverse

# Local file to deploy
LOCAL_FILE="/Users/irfangedik/USDTgVerse/trading/trade-interface.html"

# Function to show droplet information
show_droplet_info() {
    print_color $GREEN "\n🌐 DIGITALOCEAN DROPLETS INFORMATION"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Droplet 1 - NYC3:"
    print_color $WHITE "Name: $DROPLET_1"
    print_color $WHITE "IP: $DROPLET_1_IP"
    print_color $WHITE "Region: $DROPLET_1_REGION"
    print_color $WHITE "Specs: 8 GB / 4 Intel vCPUs / 240 GB Disk"
    
    print_color $WHITE "\n📋 Droplet 2 - SFO2:"
    print_color $WHITE "Name: $DROPLET_2"
    print_color $WHITE "IP: $DROPLET_2_IP"
    print_color $WHITE "Region: $DROPLET_2_REGION"
    print_color $WHITE "Specs: 8 GB / 4 Intel vCPUs / 240 GB Disk"
    
    print_color $WHITE "\n📋 Droplet 3 - FRA1:"
    print_color $WHITE "Name: $DROPLET_3"
    print_color $WHITE "IP: $DROPLET_3_IP"
    print_color $WHITE "Region: $DROPLET_3_REGION"
    print_color $WHITE "Specs: 8 GB / 4 Intel vCPUs / 240 GB Disk"
    
    print_color $CYAN "\n🎯 Deployment Configuration:"
    print_color $CYAN "SSH User: $SSH_USER"
    print_color $CYAN "SSH Key: $SSH_KEY"
    print_color $CYAN "Remote Path: $REMOTE_PATH"
    print_color $CYAN "Local File: $LOCAL_FILE"
}

# Function to deploy to single droplet
deploy_to_droplet() {
    local droplet_name=$1
    local droplet_ip=$2
    local droplet_region=$3
    local remote_path=$4
    
    print_color $YELLOW "\n🚀 Deploying to $droplet_name ($droplet_region)..."
    print_color $WHITE "IP: $droplet_ip"
    print_color $WHITE "Path: $remote_path"
    
    # Check if local file exists
    if [ ! -f "$LOCAL_FILE" ]; then
        print_color $RED "❌ Local file not found: $LOCAL_FILE"
        return 1
    fi
    
    # Create directory if it doesn't exist
    print_color $BLUE "📁 Creating directory if needed..."
    ssh -i $SSH_KEY $SSH_USER@$droplet_ip "mkdir -p $remote_path"
    
    # Create backup on remote server
    print_color $BLUE "📦 Creating backup on remote server..."
    ssh -i $SSH_KEY $SSH_USER@$droplet_ip "cp $remote_path/trade-interface.html $remote_path/trade-interface.html.backup.$(date +%Y%m%d_%H%M%S)" 2>/dev/null
    
    # Deploy file via SCP
    print_color $BLUE "📤 Uploading file via SCP..."
    scp -i $SSH_KEY "$LOCAL_FILE" $SSH_USER@$droplet_ip:$remote_path/trade-interface.html
    
    if [ $? -eq 0 ]; then
        print_color $GREEN "✅ Successfully deployed to $droplet_name"
        
        # Set permissions
        ssh -i $SSH_KEY $SSH_USER@$droplet_ip "chmod 644 $remote_path/trade-interface.html"
        
        # Test deployment
        print_color $BLUE "🧪 Testing deployment..."
        ssh -i $SSH_KEY $SSH_USER@$droplet_ip "ls -la $remote_path/trade-interface.html"
        
        return 0
    else
        print_color $RED "❌ Failed to deploy to $droplet_name"
        return 1
    fi
}

# Function to deploy to all droplets
deploy_to_all_droplets() {
    print_color $GREEN "\n🚀 DEPLOYING TO ALL 3 DROPLETS"
    print_color $GREEN "=================================================="
    
    local success_count=0
    local total_count=3
    
    # Deploy to Droplet 1 (NYC3)
    if deploy_to_droplet "$DROPLET_1" "$DROPLET_1_IP" "$DROPLET_1_REGION" "$DROPLET_1_PATH"; then
        ((success_count++))
    fi
    
    # Deploy to Droplet 2 (SFO2)
    if deploy_to_droplet "$DROPLET_2" "$DROPLET_2_IP" "$DROPLET_2_REGION" "$DROPLET_2_PATH"; then
        ((success_count++))
    fi
    
    # Deploy to Droplet 3 (FRA1)
    if deploy_to_droplet "$DROPLET_3" "$DROPLET_3_IP" "$DROPLET_3_REGION" "$DROPLET_3_PATH"; then
        ((success_count++))
    fi
    
    print_color $GREEN "\n📊 DEPLOYMENT SUMMARY"
    print_color $GREEN "=================================================="
    print_color $WHITE "Total Droplets: $total_count"
    print_color $GREEN "Successful Deployments: $success_count"
    print_color $RED "Failed Deployments: $((total_count - success_count))"
    
    if [ $success_count -eq $total_count ]; then
        print_color $GREEN "🎉 All deployments successful!"
        return 0
    else
        print_color $RED "⚠️  Some deployments failed!"
        return 1
    fi
}

# Function to test all droplets
test_all_droplets() {
    print_color $GREEN "\n🧪 TESTING ALL DROPLETS"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Testing Droplet 1 (NYC3):"
    curl -s -o /dev/null -w "HTTP Status: %{http_code}\n" http://$DROPLET_1_IP/trading/trade-interface.html
    
    print_color $WHITE "\n📋 Testing Droplet 2 (SFO2):"
    curl -s -o /dev/null -w "HTTP Status: %{http_code}\n" http://$DROPLET_2_IP/trading/trade-interface.html
    
    print_color $WHITE "\n📋 Testing Droplet 3 (FRA1):"
    curl -s -o /dev/null -w "HTTP Status: %{http_code}\n" http://$DROPLET_3_IP/trading/trade-interface.html
    
    print_color $CYAN "\n🎯 Test URLs:"
    print_color $CYAN "Droplet 1: http://$DROPLET_1_IP/trading/trade-interface.html"
    print_color $CYAN "Droplet 2: http://$DROPLET_2_IP/trading/trade-interface.html"
    print_color $CYAN "Droplet 3: http://$DROPLET_3_IP/trading/trade-interface.html"
}

# Function to show deployment commands
show_deployment_commands() {
    print_color $GREEN "\n📋 DEPLOYMENT COMMANDS"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Deploy to Droplet 1 (NYC3):"
    print_color $CYAN "scp -i $SSH_KEY $LOCAL_FILE $SSH_USER@$DROPLET_1_IP:$REMOTE_PATH/trade-interface.html"
    
    print_color $WHITE "\n📋 Deploy to Droplet 2 (SFO2):"
    print_color $CYAN "scp -i $SSH_KEY $LOCAL_FILE $SSH_USER@$DROPLET_2_IP:$REMOTE_PATH/trade-interface.html"
    
    print_color $WHITE "\n📋 Deploy to Droplet 3 (FRA1):"
    print_color $CYAN "scp -i $SSH_KEY $LOCAL_FILE $SSH_USER@$DROPLET_3_IP:$REMOTE_PATH/trade-interface.html"
    
    print_color $WHITE "\n📋 SSH Commands:"
    print_color $CYAN "ssh -i $SSH_KEY $SSH_USER@$DROPLET_1_IP"
    print_color $CYAN "ssh -i $SSH_KEY $SSH_USER@$DROPLET_2_IP"
    print_color $CYAN "ssh -i $SSH_KEY $SSH_USER@$DROPLET_3_IP"
}

# Function to show help
show_help() {
    echo "USDTgVerse DigitalOcean Droplets Deployment Script"
    echo
    echo "Usage: $0 [OPTION]"
    echo
    echo "Options:"
    echo "  -h, --help           Show this help message"
    echo "  -i, --info           Show droplet information"
    echo "  -d, --deploy         Deploy to all droplets"
    echo "  -t, --test           Test all droplets"
    echo "  -c, --commands       Show deployment commands"
    echo "  -1, --droplet1       Deploy to Droplet 1 (NYC3)"
    echo "  -2, --droplet2       Deploy to Droplet 2 (SFO2)"
    echo "  -3, --droplet3       Deploy to Droplet 3 (FRA1)"
    echo
    echo "Examples:"
    echo "  $0 --deploy          Deploy to all 3 droplets"
    echo "  $0 --test            Test all droplets"
    echo "  $0 --droplet1        Deploy to Droplet 1 only"
}

# Main script logic
main() {
    case "${1:-}" in
        -h|--help)
            show_help
            ;;
        -i|--info)
            print_banner
            show_droplet_info
            ;;
        -d|--deploy)
            print_banner
            deploy_to_all_droplets
            ;;
        -t|--test)
            print_banner
            test_all_droplets
            ;;
        -c|--commands)
            print_banner
            show_deployment_commands
            ;;
        -1|--droplet1)
            print_banner
            deploy_to_droplet "$DROPLET_1" "$DROPLET_1_IP" "$DROPLET_1_REGION" "$DROPLET_1_PATH"
            ;;
        -2|--droplet2)
            print_banner
            deploy_to_droplet "$DROPLET_2" "$DROPLET_2_IP" "$DROPLET_2_REGION" "$DROPLET_2_PATH"
            ;;
        -3|--droplet3)
            print_banner
            deploy_to_droplet "$DROPLET_3" "$DROPLET_3_IP" "$DROPLET_3_REGION" "$DROPLET_3_PATH"
            ;;
        *)
            print_banner
            show_help
            ;;
    esac
}

# Run main function with all arguments
main "$@"
