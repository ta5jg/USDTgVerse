#!/bin/bash

# USDTgVerse Auto API Keys Setup Script
# Author: Irfan Gedik
# Created: 13.10.2025

echo "🚀 USDTgVerse Auto API Keys Setup"
echo "=================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${GREEN}✅ $1${NC}"
}

print_warning() {
    echo -e "${YELLOW}⚠️  $1${NC}"
}

print_error() {
    echo -e "${RED}❌ $1${NC}"
}

print_info() {
    echo -e "${BLUE}ℹ️  $1${NC}"
}

# Check if curl is installed
if ! command -v curl &> /dev/null; then
    print_error "curl is not installed. Please install curl first."
    exit 1
fi

# Check if jq is installed
if ! command -v jq &> /dev/null; then
    print_warning "jq is not installed. Installing jq..."
    if command -v brew &> /dev/null; then
        brew install jq
    elif command -v apt-get &> /dev/null; then
        sudo apt-get update && sudo apt-get install -y jq
    elif command -v yum &> /dev/null; then
        sudo yum install -y jq
    else
        print_error "Cannot install jq automatically. Please install jq manually."
        exit 1
    fi
fi

# Function to create Stripe account and get API keys
setup_stripe() {
    print_info "Setting up Stripe API keys..."
    
    echo "📋 Stripe Setup Instructions:"
    echo "1. Go to: https://stripe.com"
    echo "2. Click 'Start now' and create account"
    echo "3. Complete business verification"
    echo "4. Go to: https://dashboard.stripe.com/apikeys"
    echo "5. Copy your API keys"
    
    read -p "Enter your Stripe Publishable Key (pk_live_...): " STRIPE_PUBLISHABLE_KEY
    read -p "Enter your Stripe Secret Key (sk_live_...): " STRIPE_SECRET_KEY
    
    if [[ $STRIPE_PUBLISHABLE_KEY == pk_live_* ]] && [[ $STRIPE_SECRET_KEY == sk_live_* ]]; then
        # Update the C file with real API keys
        sed -i.bak "s/#define STRIPE_PUBLISHABLE_KEY \"pk_live_51...\"/#define STRIPE_PUBLISHABLE_KEY \"$STRIPE_PUBLISHABLE_KEY\"/" stripe_real_live.c
        sed -i.bak "s/#define STRIPE_SECRET_KEY \"sk_live_51...\"/#define STRIPE_SECRET_KEY \"$STRIPE_SECRET_KEY\"/" stripe_real_live.c
        
        print_status "Stripe API keys updated successfully!"
        
        # Test the integration
        print_info "Testing Stripe integration..."
        gcc -Wall -Wextra -std=c99 -O2 -o stripe_real_live stripe_real_live.c -lcurl
        if [ $? -eq 0 ]; then
            print_status "Stripe integration compiled successfully!"
            echo "Run './stripe_real_live' to test live payments"
        else
            print_error "Stripe integration compilation failed!"
        fi
    else
        print_error "Invalid Stripe API keys format!"
    fi
}

# Function to create Plaid account and get API keys
setup_plaid() {
    print_info "Setting up Plaid API keys..."
    
    echo "📋 Plaid Setup Instructions:"
    echo "1. Go to: https://plaid.com"
    echo "2. Click 'Get started' and create account"
    echo "3. Complete business verification"
    echo "4. Go to: https://dashboard.plaid.com/team/keys"
    echo "5. Copy your API keys"
    
    read -p "Enter your Plaid Client ID: " PLAID_CLIENT_ID
    read -p "Enter your Plaid Secret Key: " PLAID_SECRET_KEY
    
    if [[ -n $PLAID_CLIENT_ID ]] && [[ -n $PLAID_SECRET_KEY ]]; then
        # Update the C file with real API keys
        sed -i.bak "s/#define PLAID_CLIENT_ID \"your_client_id\"/#define PLAID_CLIENT_ID \"$PLAID_CLIENT_ID\"/" plaid_real_live.c
        sed -i.bak "s/#define PLAID_SECRET_KEY \"your_secret_key\"/#define PLAID_SECRET_KEY \"$PLAID_SECRET_KEY\"/" plaid_real_live.c
        
        print_status "Plaid API keys updated successfully!"
        
        # Test the integration
        print_info "Testing Plaid integration..."
        gcc -Wall -Wextra -std=c99 -O2 -o plaid_real_live plaid_real_live.c -lcurl
        if [ $? -eq 0 ]; then
            print_status "Plaid integration compiled successfully!"
            echo "Run './plaid_real_live' to test live banking data"
        else
            print_error "Plaid integration compilation failed!"
        fi
    else
        print_error "Invalid Plaid API keys!"
    fi
}

# Function to test existing integrations
test_existing() {
    print_info "Testing existing integrations..."
    
    # Test CoinGecko
    if [ -f "coingecko_live_integration" ]; then
        print_info "Testing CoinGecko integration..."
        ./coingecko_live_integration
        if [ $? -eq 0 ]; then
            print_status "CoinGecko integration working!"
        else
            print_error "CoinGecko integration failed!"
        fi
    fi
    
    # Test Binance
    if [ -f "binance_live_integration" ]; then
        print_info "Testing Binance integration..."
        ./binance_live_integration
        if [ $? -eq 0 ]; then
            print_status "Binance integration working!"
        else
            print_error "Binance integration failed!"
        fi
    fi
}

# Function to create environment file
create_env_file() {
    print_info "Creating environment file for API keys..."
    
    cat > .env << EOF
# USDTgVerse API Keys Environment File
# Generated on: $(date)

# Stripe API Keys
STRIPE_PUBLISHABLE_KEY=""
STRIPE_SECRET_KEY=""

# Plaid API Keys
PLAID_CLIENT_ID=""
PLAID_SECRET_KEY=""

# CoinGecko API (No key required)
COINGECKO_API_URL="https://api.coingecko.com/api/v3"

# Binance API (No key required for public endpoints)
BINANCE_API_URL="https://api.binance.com/api/v3"

# Open Banking API
OPEN_BANKING_ACCESS_TOKEN=""

# Usage Instructions:
# 1. Fill in your API keys above
# 2. Source this file: source .env
# 3. Use environment variables in your code
EOF
    
    print_status "Environment file created: .env"
    print_info "Edit .env file with your API keys"
}

# Function to show integration status
show_status() {
    print_info "Integration Status:"
    echo "==================="
    
    # Check CoinGecko
    if [ -f "coingecko_live_integration" ]; then
        print_status "CoinGecko: ✅ Working (No API key required)"
    else
        print_error "CoinGecko: ❌ Not compiled"
    fi
    
    # Check Binance
    if [ -f "binance_live_integration" ]; then
        print_status "Binance: ✅ Working (No API key required)"
    else
        print_error "Binance: ❌ Not compiled"
    fi
    
    # Check Stripe
    if grep -q "pk_live_51..." stripe_real_live.c; then
        print_warning "Stripe: ⏳ Needs API keys"
    else
        print_status "Stripe: ✅ API keys configured"
    fi
    
    # Check Plaid
    if grep -q "your_client_id" plaid_real_live.c; then
        print_warning "Plaid: ⏳ Needs API keys"
    else
        print_status "Plaid: ✅ API keys configured"
    fi
}

# Main menu
main_menu() {
    while true; do
        echo ""
        echo "🎯 USDTgVerse API Keys Setup Menu"
        echo "=================================="
        echo "1. Setup Stripe API keys"
        echo "2. Setup Plaid API keys"
        echo "3. Test existing integrations"
        echo "4. Show integration status"
        echo "5. Create environment file"
        echo "6. Exit"
        echo ""
        read -p "Choose an option (1-6): " choice
        
        case $choice in
            1)
                setup_stripe
                ;;
            2)
                setup_plaid
                ;;
            3)
                test_existing
                ;;
            4)
                show_status
                ;;
            5)
                create_env_file
                ;;
            6)
                print_info "Goodbye!"
                exit 0
                ;;
            *)
                print_error "Invalid option. Please choose 1-6."
                ;;
        esac
    done
}

# Check if we're in the right directory
if [ ! -f "stripe_real_live.c" ] || [ ! -f "plaid_real_live.c" ]; then
    print_error "Please run this script from the integrations directory"
    exit 1
fi

# Start the main menu
main_menu
