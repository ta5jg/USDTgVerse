#!/bin/bash
# USDTgVerse API Key Acquisition Guide
# GeckoTerminal ve CEX'ler için API anahtarı alma rehberi
# 
# Author: Irfan Gedik
# Created: 2025-10-04
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
    print_color $CYAN "    ║           🔑 USDTgVerse API Key Acquisition Guide          ║"
    print_color $CYAN "    ║                                                              ║"
    print_color $CYAN "    ║              GeckoTerminal ve CEX API Rehberi              ║"
    print_color $CYAN "    ║                                                              ║"
    print_color $CYAN "    ╚══════════════════════════════════════════════════════════════╝"
    echo
}

# Function to open URL in browser
open_url() {
    local url=$1
    local platform=$2
    
    print_color $YELLOW "🌐 Opening $platform..."
    
    if command -v open >/dev/null 2>&1; then
        # macOS
        open "$url"
    elif command -v xdg-open >/dev/null 2>&1; then
        # Linux
        xdg-open "$url"
    elif command -v start >/dev/null 2>&1; then
        # Windows
        start "$url"
    else
        print_color $RED "❌ Cannot open browser automatically"
        print_color $YELLOW "💡 Please manually open: $url"
    fi
}

# Function to show GeckoTerminal guide
show_gecko_terminal_guide() {
    print_color $GREEN "\n🦎 GECKOTERMINAL API KEY GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Steps to get GeckoTerminal API access:"
    print_color $WHITE "1. Visit: https://www.geckoterminal.com/tr/update-token-info"
    print_color $WHITE "2. Fill out the token information form"
    print_color $WHITE "3. Provide USDTgVerse project details:"
    print_color $WHITE "   - Company: USDTgVerse"
    print_color $WHITE "   - Email: info@usdtgverse.com"
    print_color $WHITE "   - Website: https://www.usdtgverse.com"
    print_color $WHITE "   - Token: USDTg, USDTgV, USDTgG"
    print_color $WHITE "4. Complete email verification"
    print_color $WHITE "5. Pay verification fee (if required)"
    print_color $WHITE "6. Wait for approval (24-48 hours)"
    
    print_color $YELLOW "\n⚠️  IMPORTANT NOTES:"
    print_color $YELLOW "- GeckoTerminal requires MANUAL submission"
    print_color $YELLOW "- No automatic API key generation available"
    print_color $YELLOW "- Verification process is mandatory"
    print_color $YELLOW "- Payment may be required for premium features"
    
    print_color $CYAN "\n🔗 Quick Links:"
    print_color $CYAN "- Token Update Form: https://www.geckoterminal.com/tr/update-token-info"
    print_color $CYAN "- GeckoTerminal API Docs: https://docs.geckoterminal.com"
    
    read -p "Press Enter to open GeckoTerminal token update form..."
    open_url "https://www.geckoterminal.com/tr/update-token-info" "GeckoTerminal"
}

# Function to show Binance guide
show_binance_guide() {
    print_color $GREEN "\n🏦 BINANCE API KEY GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Steps to get Binance API access:"
    print_color $WHITE "1. Visit: https://www.binance.com/en/my/settings/api-management"
    print_color $WHITE "2. Log in to your Binance account"
    print_color $WHITE "3. Enable 2FA (Two-Factor Authentication)"
    print_color $WHITE "4. Click 'Create API'"
    print_color $WHITE "5. Set API restrictions:"
    print_color $WHITE "   - Enable 'Enable Reading'"
    print_color $WHITE "   - Enable 'Enable Spot & Margin Trading'"
    print_color $WHITE "   - Enable 'Enable Futures' (if needed)"
    print_color $WHITE "6. Set IP restrictions (recommended)"
    print_color $WHITE "7. Confirm with 2FA"
    print_color $WHITE "8. Copy API Key and Secret Key"
    
    print_color $YELLOW "\n⚠️  IMPORTANT NOTES:"
    print_color $YELLOW "- Binance requires MANUAL API key creation"
    print_color $YELLOW "- 2FA is mandatory for API access"
    print_color $YELLOW "- IP restrictions recommended for security"
    print_color $YELLOW "- API keys have expiration dates"
    
    print_color $CYAN "\n🔗 Quick Links:"
    print_color $CYAN "- API Management: https://www.binance.com/en/my/settings/api-management"
    print_color $CYAN "- Binance API Docs: https://binance-docs.github.io/apidocs"
    
    read -p "Press Enter to open Binance API management..."
    open_url "https://www.binance.com/en/my/settings/api-management" "Binance"
}

# Function to show Coinbase guide
show_coinbase_guide() {
    print_color $GREEN "\n🏦 COINBASE API KEY GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Steps to get Coinbase API access:"
    print_color $WHITE "1. Visit: https://pro.coinbase.com/profile/api"
    print_color $WHITE "2. Log in to your Coinbase Pro account"
    print_color $WHITE "3. Enable 2FA (Two-Factor Authentication)"
    print_color $WHITE "4. Click 'Create API Key'"
    print_color $WHITE "5. Set API permissions:"
    print_color $WHITE "   - Enable 'View' (read access)"
    print_color $WHITE "   - Enable 'Trade' (trading access)"
    print_color $WHITE "   - Enable 'Transfer' (withdrawal access)"
    print_color $WHITE "6. Set IP restrictions (recommended)"
    print_color $WHITE "7. Confirm with 2FA"
    print_color $WHITE "8. Copy API Key and Secret Key"
    
    print_color $YELLOW "\n⚠️  IMPORTANT NOTES:"
    print_color $YELLOW "- Coinbase requires MANUAL API key creation"
    print_color $YELLOW "- 2FA is mandatory for API access"
    print_color $YELLOW "- IP restrictions recommended for security"
    print_color $YELLOW "- API keys have expiration dates"
    
    print_color $CYAN "\n🔗 Quick Links:"
    print_color $CYAN "- API Management: https://pro.coinbase.com/profile/api"
    print_color $CYAN "- Coinbase API Docs: https://docs.pro.coinbase.com"
    
    read -p "Press Enter to open Coinbase API management..."
    open_url "https://pro.coinbase.com/profile/api" "Coinbase"
}

# Function to show Kraken guide
show_kraken_guide() {
    print_color $GREEN "\n🏦 KRAKEN API KEY GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Steps to get Kraken API access:"
    print_color $WHITE "1. Visit: https://www.kraken.com/features/api"
    print_color $WHITE "2. Log in to your Kraken account"
    print_color $WHITE "3. Enable 2FA (Two-Factor Authentication)"
    print_color $WHITE "4. Click 'Generate New Key'"
    print_color $WHITE "5. Set API permissions:"
    print_color $WHITE "   - Enable 'Query Private Data'"
    print_color $WHITE "   - Enable 'Query Private Data'"
    print_color $WHITE "   - Enable 'Query Private Data'"
    print_color $WHITE "6. Set IP restrictions (recommended)"
    print_color $WHITE "7. Confirm with 2FA"
    print_color $WHITE "8. Copy API Key and Secret Key"
    
    print_color $YELLOW "\n⚠️  IMPORTANT NOTES:"
    print_color $YELLOW "- Kraken requires MANUAL API key creation"
    print_color $YELLOW "- 2FA is mandatory for API access"
    print_color $YELLOW "- IP restrictions recommended for security"
    print_color $YELLOW "- API keys have expiration dates"
    
    print_color $CYAN "\n🔗 Quick Links:"
    print_color $CYAN "- API Management: https://www.kraken.com/features/api"
    print_color $CYAN "- Kraken API Docs: https://www.kraken.com/features/api"
    
    read -p "Press Enter to open Kraken API management..."
    open_url "https://www.kraken.com/features/api" "Kraken"
}

# Function to show automatic vs manual comparison
show_automatic_vs_manual() {
    print_color $GREEN "\n🤖 AUTOMATIC vs MANUAL API KEY ACQUISITION"
    print_color $GREEN "=================================================="
    
    print_color $RED "\n❌ AUTOMATIC API KEY ACQUISITION:"
    print_color $RED "- NOT POSSIBLE for security reasons"
    print_color $RED "- All platforms require manual verification"
    print_color $RED "- 2FA and identity verification mandatory"
    print_color $RED "- No programmatic way to create API keys"
    
    print_color $GREEN "\n✅ MANUAL API KEY ACQUISITION:"
    print_color $GREEN "- REQUIRED for all platforms"
    print_color $GREEN "- User must log in and create keys manually"
    print_color $GREEN "- 2FA verification required"
    print_color $GREEN "- IP restrictions recommended"
    
    print_color $YELLOW "\n🔒 SECURITY REASONS:"
    print_color $YELLOW "- Prevents unauthorized access"
    print_color $YELLOW "- Ensures user identity verification"
    print_color $YELLOW "- Protects against automated attacks"
    print_color $YELLOW "- Maintains platform security standards"
    
    print_color $CYAN "\n💡 RECOMMENDATIONS:"
    print_color $CYAN "- Create API keys manually for each platform"
    print_color $CYAN "- Use strong passwords and 2FA"
    print_color $CYAN "- Set IP restrictions when possible"
    print_color $CYAN "- Store API keys securely (not in code)"
    print_color $CYAN "- Use environment variables for API keys"
}

# Function to show USDTgVerse specific information
show_usdtgverse_info() {
    print_color $GREEN "\n🚀 USDTGVERSE PROJECT INFORMATION"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Project Details for API Applications:"
    print_color $WHITE "Company Name: USDTgVerse"
    print_color $WHITE "Email: info@usdtgverse.com"
    print_color $WHITE "Website: https://www.usdtgverse.com"
    print_color $WHITE "Username: @usdtgverse"
    
    print_color $WHITE "\n🪙 Token Information:"
    print_color $WHITE "USDTg (Stablecoin): 0x742d35Cc6634C0532925a3b8D4C9db96C4b4d8b6"
    print_color $WHITE "USDTgV (Utility): 0x8ba1f109551bD432803012645Hac136c4c8b3d5"
    print_color $WHITE "USDTgG (Governance): 0x1234567890123456789012345678901234567890"
    
    print_color $WHITE "\n📊 Market Data:"
    print_color $WHITE "Total Market Cap: $1.75B"
    print_color $WHITE "Total Volume 24h: $1.5M"
    print_color $WHITE "Active Users: 1,000+"
    print_color $WHITE "Transactions 24h: 5,000+"
    
    print_color $YELLOW "\n⚠️  IMPORTANT NOTES:"
    print_color $YELLOW "- Use exact project information for consistency"
    print_color $YELLOW "- Provide accurate token contract addresses"
    print_color $YELLOW "- Include current market data"
    print_color $YELLOW "- Maintain professional communication"
}

# Function to show help
show_help() {
    echo "USDTgVerse API Key Acquisition Guide"
    echo
    echo "Usage: $0 [OPTION]"
    echo
    echo "Options:"
    echo "  -h, --help           Show this help message"
    echo "  -g, --gecko          Show GeckoTerminal guide"
    echo "  -b, --binance        Show Binance guide"
    echo "  -c, --coinbase        Show Coinbase guide"
    echo "  -k, --kraken         Show Kraken guide"
    echo "  -a, --automatic      Show automatic vs manual comparison"
    echo "  -u, --usdtgverse     Show USDTgVerse project information"
    echo "  -all, --all          Show all guides"
    echo
    echo "Examples:"
    echo "  $0 --gecko           Show GeckoTerminal guide"
    echo "  $0 --binance         Show Binance guide"
    echo "  $0 --all             Show all guides"
}

# Function to show all guides
show_all_guides() {
    show_gecko_terminal_guide
    show_binance_guide
    show_coinbase_guide
    show_kraken_guide
    show_automatic_vs_manual
    show_usdtgverse_info
}

# Main script logic
main() {
    case "${1:-}" in
        -h|--help)
            show_help
            ;;
        -g|--gecko)
            print_banner
            show_gecko_terminal_guide
            ;;
        -b|--binance)
            print_banner
            show_binance_guide
            ;;
        -c|--coinbase)
            print_banner
            show_coinbase_guide
            ;;
        -k|--kraken)
            print_banner
            show_kraken_guide
            ;;
        -a|--automatic)
            print_banner
            show_automatic_vs_manual
            ;;
        -u|--usdtgverse)
            print_banner
            show_usdtgverse_info
            ;;
        -all|--all)
            print_banner
            show_all_guides
            ;;
        *)
            print_banner
            show_help
            ;;
    esac
}

# Run main function with all arguments
main "$@"
