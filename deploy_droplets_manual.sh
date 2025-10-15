#!/bin/bash
# USDTgVerse DigitalOcean Droplets Deployment Script - Alternative Method
# Deploy updated trading interface to all 3 droplets via SSH with password
# 
# Author: USDTG GROUP TECHNOLOGY LLC
# Created: 2025-10-05
# Version: 1.1

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
    print_color $CYAN "    ║              Alternative Deployment Method                 ║"
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
REMOTE_PATH="/var/www/html/trading/"

# Local file to deploy
LOCAL_FILE="/Users/irfangedik/USDTgVerse/trading/trade-interface.html"

# Function to show SSH key setup instructions
show_ssh_key_setup() {
    print_color $GREEN "\n🔑 SSH KEY SETUP INSTRUCTIONS"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Your SSH Public Key:"
    print_color $CYAN "$(cat ~/.ssh/id_ed25519.pub)"
    
    print_color $WHITE "\n📋 Steps to Add SSH Key to Droplets:"
    print_color $WHITE "1. Login to DigitalOcean Console"
    print_color $WHITE "2. Go to Droplets → Select Droplet"
    print_color $WHITE "3. Click 'Console' button"
    print_color $WHITE "4. Run these commands:"
    
    print_color $CYAN "\n# Create .ssh directory if it doesn't exist"
    print_color $CYAN "mkdir -p ~/.ssh"
    
    print_color $CYAN "\n# Add your public key to authorized_keys"
    print_color $CYAN "echo '$(cat ~/.ssh/id_ed25519.pub)' >> ~/.ssh/authorized_keys"
    
    print_color $CYAN "\n# Set correct permissions"
    print_color $CYAN "chmod 700 ~/.ssh"
    print_color $CYAN "chmod 600 ~/.ssh/authorized_keys"
    
    print_color $WHITE "\n📋 Repeat for all 3 droplets:"
    print_color $WHITE "- $DROPLET_1 ($DROPLET_1_IP)"
    print_color $WHITE "- $DROPLET_2 ($DROPLET_2_IP)"
    print_color $WHITE "- $DROPLET_3 ($DROPLET_3_IP)"
}

# Function to show manual deployment instructions
show_manual_deployment() {
    print_color $GREEN "\n📋 MANUAL DEPLOYMENT INSTRUCTIONS"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Method 1: DigitalOcean Console"
    print_color $WHITE "1. Login to DigitalOcean Console"
    print_color $WHITE "2. Go to Droplets → Select Droplet"
    print_color $WHITE "3. Click 'Console' button"
    print_color $WHITE "4. Run these commands:"
    
    print_color $CYAN "\n# Navigate to web directory"
    print_color $CYAN "cd /var/www/html/trading/"
    
    print_color $CYAN "\n# Create backup"
    print_color $CYAN "cp trade-interface.html trade-interface.html.backup.\$(date +%Y%m%d_%H%M%S)"
    
    print_color $CYAN "\n# Create new file"
    print_color $CYAN "nano trade-interface.html"
    
    print_color $WHITE "\n📋 Method 2: Copy-Paste Content"
    print_color $WHITE "1. Copy content from local file:"
    print_color $CYAN "cat $LOCAL_FILE"
    
    print_color $WHITE "\n2. Paste into nano editor on droplet"
    print_color $WHITE "3. Save with Ctrl+X, Y, Enter"
    
    print_color $WHITE "\n📋 Method 3: wget/curl (if you have a temporary URL)"
    print_color $WHITE "1. Upload file to temporary location"
    print_color $WHITE "2. Use wget or curl to download"
    print_color $CYAN "wget https://temp-url.com/trade-interface.html -O trade-interface.html"
}

# Function to show deployment commands for each droplet
show_droplet_commands() {
    print_color $GREEN "\n📋 DEPLOYMENT COMMANDS FOR EACH DROPLET"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Droplet 1 - NYC3 ($DROPLET_1_IP):"
    print_color $CYAN "ssh root@$DROPLET_1_IP"
    print_color $CYAN "cd /var/www/html/trading/"
    print_color $CYAN "cp trade-interface.html trade-interface.html.backup.\$(date +%Y%m%d_%H%M%S)"
    print_color $CYAN "nano trade-interface.html"
    
    print_color $WHITE "\n📋 Droplet 2 - SFO2 ($DROPLET_2_IP):"
    print_color $CYAN "ssh root@$DROPLET_2_IP"
    print_color $CYAN "cd /var/www/html/trading/"
    print_color $CYAN "cp trade-interface.html trade-interface.html.backup.\$(date +%Y%m%d_%H%M%S)"
    print_color $CYAN "nano trade-interface.html"
    
    print_color $WHITE "\n📋 Droplet 3 - FRA1 ($DROPLET_3_IP):"
    print_color $CYAN "ssh root@$DROPLET_3_IP"
    print_color $CYAN "cd /var/www/html/trading/"
    print_color $CYAN "cp trade-interface.html trade-interface.html.backup.\$(date +%Y%m%d_%H%M%S)"
    print_color $CYAN "nano trade-interface.html"
}

# Function to show file content
show_file_content() {
    print_color $GREEN "\n📄 TRADING INTERFACE FILE CONTENT"
    print_color $GREEN "=================================================="
    
    if [ -f "$LOCAL_FILE" ]; then
        print_color $WHITE "📋 File: $LOCAL_FILE"
        print_color $WHITE "📋 Size: $(wc -c < "$LOCAL_FILE") bytes"
        print_color $WHITE "📋 Lines: $(wc -l < "$LOCAL_FILE")"
        
        print_color $CYAN "\n📋 First 20 lines:"
        head -20 "$LOCAL_FILE"
        
        print_color $CYAN "\n📋 Last 10 lines:"
        tail -10 "$LOCAL_FILE"
        
        print_color $WHITE "\n📋 To copy full content:"
        print_color $CYAN "cat $LOCAL_FILE"
    else
        print_color $RED "❌ File not found: $LOCAL_FILE"
    fi
}

# Function to show testing URLs
show_testing_urls() {
    print_color $GREEN "\n🧪 TESTING URLS"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 After deployment, test these URLs:"
    print_color $CYAN "Droplet 1 (NYC3): http://$DROPLET_1_IP/trading/trade-interface.html"
    print_color $CYAN "Droplet 2 (SFO2): http://$DROPLET_2_IP/trading/trade-interface.html"
    print_color $CYAN "Droplet 3 (FRA1): http://$DROPLET_3_IP/trading/trade-interface.html"
    
    print_color $WHITE "\n📋 Expected Changes:"
    print_color $WHITE "✅ Chart area genişletildi (2fr 1fr grid)"
    print_color $WHITE "✅ Tab butonları ayrıldı (border-right)"
    print_color $WHITE "✅ Hover effects çalışıyor"
    print_color $WHITE "✅ Cursor changes çalışıyor"
    print_color $WHITE "✅ Click animations çalışıyor"
    print_color $WHITE "✅ Title: 'USDTgVerse Trading Interface - UPDATED v2.1'"
}

# Function to show help
show_help() {
    echo "USDTgVerse DigitalOcean Droplets Deployment Script - Alternative Method"
    echo
    echo "Usage: $0 [OPTION]"
    echo
    echo "Options:"
    echo "  -h, --help           Show this help message"
    echo "  -s, --ssh-setup      Show SSH key setup instructions"
    echo "  -m, --manual         Show manual deployment instructions"
    echo "  -c, --commands       Show deployment commands for each droplet"
    echo "  -f, --file-content   Show file content to copy"
    echo "  -t, --test-urls      Show testing URLs"
    echo
    echo "Examples:"
    echo "  $0 --ssh-setup       Show SSH key setup instructions"
    echo "  $0 --manual          Show manual deployment instructions"
    echo "  $0 --file-content    Show file content to copy"
}

# Main script logic
main() {
    case "${1:-}" in
        -h|--help)
            show_help
            ;;
        -s|--ssh-setup)
            print_banner
            show_ssh_key_setup
            ;;
        -m|--manual)
            print_banner
            show_manual_deployment
            ;;
        -c|--commands)
            print_banner
            show_droplet_commands
            ;;
        -f|--file-content)
            print_banner
            show_file_content
            ;;
        -t|--test-urls)
            print_banner
            show_testing_urls
            ;;
        *)
            print_banner
            show_help
            ;;
    esac
}

# Run main function with all arguments
main "$@"
