#!/bin/bash

# USDTgVerse Quick API Setup Script
# Author: Irfan Gedik
# Created: 13.10.2025

echo "🚀 USDTgVerse Quick API Setup"
echo "============================="

# Colors
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

print_status() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ️  $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

# Function to open browser tabs
open_browser_tabs() {
    print_info "Opening browser tabs for API key setup..."
    
    # Open Stripe dashboard
    if command -v open &> /dev/null; then
        open "https://dashboard.stripe.com/apikeys"
        print_status "Opened Stripe API keys page"
    elif command -v xdg-open &> /dev/null; then
        xdg-open "https://dashboard.stripe.com/apikeys"
        print_status "Opened Stripe API keys page"
    fi
    
    # Open Plaid dashboard
    if command -v open &> /dev/null; then
        open "https://dashboard.plaid.com/team/keys"
        print_status "Opened Plaid API keys page"
    elif command -v xdg-open &> /dev/null; then
        xdg-open "https://dashboard.plaid.com/team/keys"
        print_status "Opened Plaid API keys page"
    fi
    
    print_info "Browser tabs opened! Copy your API keys from the opened pages."
}

# Function to create API keys template
create_template() {
    print_info "Creating API keys template..."
    
    cat > api_keys_template.txt << EOF
# USDTgVerse API Keys Template
# Copy your API keys here and run: ./update_api_keys.sh

# Stripe API Keys (from https://dashboard.stripe.com/apikeys)
STRIPE_PUBLISHABLE_KEY="pk_live_51..."
STRIPE_SECRET_KEY="sk_live_51..."

# Plaid API Keys (from https://dashboard.plaid.com/team/keys)
PLAID_CLIENT_ID="your_client_id"
PLAID_SECRET_KEY="your_secret_key"

# Instructions:
# 1. Replace the placeholder values above with your real API keys
# 2. Save this file
# 3. Run: ./update_api_keys.sh
EOF
    
    print_status "API keys template created: api_keys_template.txt"
}

# Function to update API keys from template
update_api_keys() {
    if [ ! -f "api_keys_template.txt" ]; then
        print_warning "api_keys_template.txt not found. Creating template..."
        create_template
        return
    fi
    
    print_info "Reading API keys from template..."
    
    # Extract API keys from template
    STRIPE_PUBLISHABLE_KEY=$(grep "STRIPE_PUBLISHABLE_KEY=" api_keys_template.txt | cut -d'"' -f2)
    STRIPE_SECRET_KEY=$(grep "STRIPE_SECRET_KEY=" api_keys_template.txt | cut -d'"' -f2)
    PLAID_CLIENT_ID=$(grep "PLAID_CLIENT_ID=" api_keys_template.txt | cut -d'"' -f2)
    PLAID_SECRET_KEY=$(grep "PLAID_SECRET_KEY=" api_keys_template.txt | cut -d'"' -f2)
    
    # Check if keys are still placeholders
    if [[ $STRIPE_PUBLISHABLE_KEY == "pk_live_51..." ]] || [[ $STRIPE_SECRET_KEY == "sk_live_51..." ]]; then
        print_warning "Stripe API keys are still placeholders. Please update api_keys_template.txt"
        return
    fi
    
    if [[ $PLAID_CLIENT_ID == "your_client_id" ]] || [[ $PLAID_SECRET_KEY == "your_secret_key" ]]; then
        print_warning "Plaid API keys are still placeholders. Please update api_keys_template.txt"
        return
    fi
    
    # Update Stripe C file
    if [ -f "stripe_real_live.c" ]; then
        sed -i.bak "s/#define STRIPE_PUBLISHABLE_KEY \"pk_live_51...\"/#define STRIPE_PUBLISHABLE_KEY \"$STRIPE_PUBLISHABLE_KEY\"/" stripe_real_live.c
        sed -i.bak "s/#define STRIPE_SECRET_KEY \"sk_live_51...\"/#define STRIPE_SECRET_KEY \"$STRIPE_SECRET_KEY\"/" stripe_real_live.c
        print_status "Stripe API keys updated in stripe_real_live.c"
    fi
    
    # Update Plaid C file
    if [ -f "plaid_real_live.c" ]; then
        sed -i.bak "s/#define PLAID_CLIENT_ID \"your_client_id\"/#define PLAID_CLIENT_ID \"$PLAID_CLIENT_ID\"/" plaid_real_live.c
        sed -i.bak "s/#define PLAID_SECRET_KEY \"your_secret_key\"/#define PLAID_SECRET_KEY \"$PLAID_SECRET_KEY\"/" plaid_real_live.c
        print_status "Plaid API keys updated in plaid_real_live.c"
    fi
    
    # Compile updated integrations
    print_info "Compiling updated integrations..."
    
    if [ -f "stripe_real_live.c" ]; then
        gcc -Wall -Wextra -std=c99 -O2 -o stripe_real_live stripe_real_live.c -lcurl
        if [ $? -eq 0 ]; then
            print_status "Stripe integration compiled successfully!"
        else
            print_warning "Stripe integration compilation failed!"
        fi
    fi
    
    if [ -f "plaid_real_live.c" ]; then
        gcc -Wall -Wextra -std=c99 -O2 -o plaid_real_live plaid_real_live.c -lcurl
        if [ $? -eq 0 ]; then
            print_status "Plaid integration compiled successfully!"
        else
            print_warning "Plaid integration compilation failed!"
        fi
    fi
    
    print_status "API keys updated and integrations compiled!"
    print_info "Test your integrations:"
    echo "  ./stripe_real_live"
    echo "  ./plaid_real_live"
}

# Function to test all integrations
test_all() {
    print_info "Testing all integrations..."
    
    # Test CoinGecko
    if [ -f "coingecko_live_integration" ]; then
        print_info "Testing CoinGecko..."
        ./coingecko_live_integration > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            print_status "CoinGecko: ✅ Working"
        else
            print_warning "CoinGecko: ❌ Failed"
        fi
    fi
    
    # Test Binance
    if [ -f "binance_live_integration" ]; then
        print_info "Testing Binance..."
        ./binance_live_integration > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            print_status "Binance: ✅ Working"
        else
            print_warning "Binance: ❌ Failed"
        fi
    fi
    
    # Test Stripe
    if [ -f "stripe_real_live" ]; then
        print_info "Testing Stripe..."
        ./stripe_real_live > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            print_status "Stripe: ✅ Working"
        else
            print_warning "Stripe: ❌ Failed (Check API keys)"
        fi
    fi
    
    # Test Plaid
    if [ -f "plaid_real_live" ]; then
        print_info "Testing Plaid..."
        ./plaid_real_live > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            print_status "Plaid: ✅ Working"
        else
            print_warning "Plaid: ❌ Failed (Check API keys)"
        fi
    fi
}

# Main menu
main_menu() {
    while true; do
        echo ""
        echo "🎯 USDTgVerse Quick API Setup"
        echo "============================="
        echo "1. Open browser tabs for API keys"
        echo "2. Create API keys template"
        echo "3. Update API keys from template"
        echo "4. Test all integrations"
        echo "5. Exit"
        echo ""
        read -p "Choose an option (1-5): " choice
        
        case $choice in
            1)
                open_browser_tabs
                break
                ;;
            2)
                create_template
                break
                ;;
            3)
                update_api_keys
                break
                ;;
            4)
                test_all
                break
                ;;
            5)
                print_info "Goodbye!"
                exit 0
                ;;
            *)
                print_warning "Invalid option. Please choose 1-5."
                ;;
        esac
    done
}

# Check if we're in the right directory
if [ ! -f "stripe_real_live.c" ] || [ ! -f "plaid_real_live.c" ]; then
    echo "❌ Please run this script from the integrations directory"
    exit 1
fi

# Start the main menu
main_menu
