#!/bin/bash
# USDTgVerse Trading Interface Deployment Script
# Deploy updated trading interface to live website
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
    print_color $CYAN "    ║        🚀 USDTgVerse Trading Interface Deployment         ║"
    print_color $CYAN "    ║                                                              ║"
    print_color $CYAN "    ║              Deploy Updated Interface to Live Site          ║"
    print_color $CYAN "    ║                                                              ║"
    print_color $CYAN "    ╚══════════════════════════════════════════════════════════════╝"
    echo
}

# Function to show deployment options
show_deployment_options() {
    print_color $GREEN "\n🌐 DEPLOYMENT OPTIONS - LIVE WEBSITE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Deployment Methods:"
    print_color $WHITE "1. FTP/SFTP Upload"
    print_color $WHITE "2. Git Push to Repository"
    print_color $WHITE "3. cPanel File Manager"
    print_color $WHITE "4. SSH/SCP Upload"
    print_color $WHITE "5. Cloud Storage (AWS S3, etc.)"
    
    print_color $WHITE "\n📋 Required Files to Deploy:"
    print_color $WHITE "- /trading/trade-interface.html (UPDATED)"
    print_color $WHITE "- /assets/logos/logo.png (if needed)"
    print_color $WHITE "- Any additional CSS/JS files"
    
    print_color $WHITE "\n📋 Deployment Checklist:"
    print_color $WHITE "✅ Updated trading interface (v2.1)"
    print_color $WHITE "✅ Chart area genişletildi (2fr 1fr)"
    print_color $WHITE "✅ Tab butonları ayrıldı (border-right)"
    print_color $WHITE "✅ Hover effects eklendi"
    print_color $WHITE "✅ Cursor changes eklendi"
    print_color $WHITE "✅ Click animations eklendi"
    
    print_color $CYAN "\n🎯 Deployment Steps:"
    print_color $CYAN "1. Backup current live version"
    print_color $CYAN "2. Upload updated trade-interface.html"
    print_color $CYAN "3. Clear CDN cache (if applicable)"
    print_color $CYAN "4. Test live website"
    print_color $CYAN "5. Verify all changes are working"
}

# Function to show FTP deployment
show_ftp_deployment() {
    print_color $GREEN "\n📤 FTP/SFTP DEPLOYMENT GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 FTP Connection Details:"
    print_color $WHITE "Host: usdtgverse.com (or your hosting provider)"
    print_color $WHITE "Port: 21 (FTP) or 22 (SFTP)"
    print_color $WHITE "Username: [Your FTP username]"
    print_color $WHITE "Password: [Your FTP password]"
    print_color $WHITE "Directory: /public_html/trading/ (or /www/trading/)"
    
    print_color $WHITE "\n📋 Files to Upload:"
    print_color $WHITE "Source: /Users/irfangedik/USDTgVerse/trading/trade-interface.html"
    print_color $WHITE "Destination: /public_html/trading/trade-interface.html"
    
    print_color $WHITE "\n📋 FTP Commands:"
    print_color $WHITE "1. Connect to FTP server"
    print_color $WHITE "2. Navigate to /trading/ directory"
    print_color $WHITE "3. Upload trade-interface.html"
    print_color $WHITE "4. Set permissions (644)"
    print_color $WHITE "5. Test website"
    
    print_color $CYAN "\n🎯 Quick FTP Upload:"
    print_color $CYAN "lftp -c 'open -u username,password ftp.usdtgverse.com; cd trading; put trade-interface.html; quit'"
}

# Function to show Git deployment
show_git_deployment() {
    print_color $GREEN "\n📤 GIT DEPLOYMENT GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Git Repository:"
    print_color $WHITE "Repository: [Your Git repository URL]"
    print_color $WHITE "Branch: main/master"
    print_color $WHITE "File: trading/trade-interface.html"
    
    print_color $WHITE "\n📋 Git Commands:"
    print_color $WHITE "1. git add trading/trade-interface.html"
    print_color $WHITE "2. git commit -m 'Update trading interface - fix chart area and tab buttons'"
    print_color $WHITE "3. git push origin main"
    print_color $WHITE "4. Wait for deployment (if auto-deploy)"
    
    print_color $WHITE "\n📋 Auto-Deployment:"
    print_color $WHITE "If using GitHub Pages, Netlify, Vercel:"
    print_color $WHITE "- Push to main branch"
    print_color $WHITE "- Automatic deployment"
    print_color $WHITE "- Clear cache if needed"
    
    print_color $CYAN "\n🎯 Quick Git Deploy:"
    print_color $CYAN "git add . && git commit -m 'Update trading interface' && git push"
}

# Function to show cPanel deployment
show_cpanel_deployment() {
    print_color $GREEN "\n📤 CPANEL DEPLOYMENT GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 cPanel Access:"
    print_color $WHITE "URL: https://usdtgverse.com:2083 (or your cPanel URL)"
    print_color $WHITE "Username: [Your cPanel username]"
    print_color $WHITE "Password: [Your cPanel password]"
    
    print_color $WHITE "\n📋 File Manager Steps:"
    print_color $WHITE "1. Login to cPanel"
    print_color $WHITE "2. Open File Manager"
    print_color $WHITE "3. Navigate to public_html/trading/"
    print_color $WHITE "4. Upload trade-interface.html"
    print_color $WHITE "5. Replace existing file"
    print_color $WHITE "6. Set permissions to 644"
    
    print_color $WHITE "\n📋 Alternative: Drag & Drop"
    print_color $WHITE "1. Open File Manager"
    print_color $WHITE "2. Drag trade-interface.html to /trading/ folder"
    print_color $WHITE "3. Confirm replacement"
    print_color $WHITE "4. Test website"
    
    print_color $CYAN "\n🎯 cPanel File Manager:"
    print_color $CYAN "1. Login to cPanel"
    print_color $CYAN "2. File Manager → public_html → trading"
    print_color $CYAN "3. Upload new trade-interface.html"
    print_color $CYAN "4. Replace old file"
}

# Function to show SSH deployment
show_ssh_deployment() {
    print_color $GREEN "\n📤 SSH/SCP DEPLOYMENT GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 SSH Connection:"
    print_color $WHITE "Host: usdtgverse.com (or your server IP)"
    print_color $WHITE "Port: 22"
    print_color $WHITE "Username: [Your SSH username]"
    print_color $WHITE "Key: [Your SSH key or password]"
    
    print_color $WHITE "\n📋 SCP Upload Command:"
    print_color $WHITE "scp /Users/irfangedik/USDTgVerse/trading/trade-interface.html username@usdtgverse.com:/public_html/trading/"
    
    print_color $WHITE "\n📋 SSH Commands:"
    print_color $WHITE "1. ssh username@usdtgverse.com"
    print_color $WHITE "2. cd /public_html/trading/"
    print_color $WHITE "3. cp trade-interface.html trade-interface.html.backup"
    print_color $WHITE "4. nano trade-interface.html (edit file)"
    print_color $WHITE "5. Or use SCP to upload"
    
    print_color $CYAN "\n🎯 Quick SCP Upload:"
    print_color $CYAN "scp trading/trade-interface.html user@usdtgverse.com:/public_html/trading/"
}

# Function to show cloud deployment
show_cloud_deployment() {
    print_color $GREEN "\n📤 CLOUD DEPLOYMENT GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 AWS S3 Deployment:"
    print_color $WHITE "1. aws s3 cp trading/trade-interface.html s3://usdtgverse-bucket/trading/"
    print_color $WHITE "2. aws cloudfront create-invalidation --distribution-id [ID] --paths '/trading/trade-interface.html'"
    
    print_color $WHITE "\n📋 Google Cloud Storage:"
    print_color $WHITE "1. gsutil cp trading/trade-interface.html gs://usdtgverse-bucket/trading/"
    print_color $WHITE "2. gsutil setmeta -h 'Cache-Control:no-cache' gs://usdtgverse-bucket/trading/trade-interface.html"
    
    print_color $WHITE "\n📋 Azure Blob Storage:"
    print_color $WHITE "1. az storage blob upload --file trading/trade-interface.html --container-name trading --name trade-interface.html"
    
    print_color $CYAN "\n🎯 Cloud Deployment Benefits:"
    print_color $CYAN "- CDN caching"
    print_color $CYAN "- Global distribution"
    print_color $CYAN "- Automatic scaling"
    print_color $CYAN "- SSL/TLS support"
}

# Function to show cache clearing
show_cache_clearing() {
    print_color $GREEN "\n🧹 CACHE CLEARING GUIDE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Browser Cache:"
    print_color $WHITE "Chrome: Cmd+Shift+R (Hard refresh)"
    print_color $WHITE "Safari: Develop → Empty Caches"
    print_color $WHITE "Firefox: Cmd+Shift+R"
    
    print_color $WHITE "\n📋 CDN Cache:"
    print_color $WHITE "Cloudflare: Purge Cache"
    print_color $WHITE "AWS CloudFront: Create Invalidation"
    print_color $WHITE "MaxCDN: Purge Cache"
    
    print_color $WHITE "\n📋 Server Cache:"
    print_color $WHITE "Apache: service httpd restart"
    print_color $WHITE "Nginx: nginx -s reload"
    print_color $WHITE "PHP: opcache_reset()"
    
    print_color $CYAN "\n🎯 Quick Cache Clear:"
    print_color $CYAN "1. Hard refresh browser (Cmd+Shift+R)"
    print_color $CYAN "2. Clear CDN cache (if applicable)"
    print_color $CYAN "3. Wait 5-10 minutes"
    print_color $CYAN "4. Test website"
}

# Function to show testing checklist
show_testing_checklist() {
    print_color $GREEN "\n🧪 TESTING CHECKLIST - LIVE WEBSITE"
    print_color $GREEN "=================================================="
    
    print_color $WHITE "📋 Visual Tests:"
    print_color $WHITE "✅ Chart area genişletildi (2fr 1fr grid)"
    print_color $WHITE "✅ Tab butonları ayrıldı (border-right)"
    print_color $WHITE "✅ Hover effects çalışıyor"
    print_color $WHITE "✅ Cursor changes çalışıyor"
    print_color $WHITE "✅ Click animations çalışıyor"
    
    print_color $WHITE "\n📋 Functional Tests:"
    print_color $WHITE "✅ Market tab çalışıyor"
    print_color $WHITE "✅ Limit tab çalışıyor"
    print_color $WHITE "✅ Stop tab çalışıyor"
    print_color $WHITE "✅ Buy button çalışıyor"
    print_color $WHITE "✅ Sell button çalışıyor"
    
    print_color $WHITE "\n📋 Browser Tests:"
    print_color $WHITE "✅ Chrome compatibility"
    print_color $WHITE "✅ Safari compatibility"
    print_color $WHITE "✅ Firefox compatibility"
    print_color $WHITE "✅ Mobile responsiveness"
    
    print_color $CYAN "\n🎯 Testing Steps:"
    print_color $CYAN "1. Open usdtgverse.com/trading/trade-interface.html"
    print_color $CYAN "2. Check chart area width"
    print_color $CYAN "3. Test tab transitions"
    print_color $CYAN "4. Test hover effects"
    print_color $CYAN "5. Test click animations"
    print_color $CYAN "6. Test on different browsers"
}

# Function to show help
show_help() {
    echo "USDTgVerse Trading Interface Deployment Script"
    echo
    echo "Usage: $0 [OPTION]"
    echo
    echo "Options:"
    echo "  -h, --help           Show this help message"
    echo "  -o, --options        Show deployment options"
    echo "  -f, --ftp            Show FTP deployment guide"
    echo "  -g, --git            Show Git deployment guide"
    echo "  -c, --cpanel         Show cPanel deployment guide"
    echo "  -s, --ssh            Show SSH deployment guide"
    echo "  -l, --cloud          Show cloud deployment guide"
    echo "  -a, --cache          Show cache clearing guide"
    echo "  -t, --test           Show testing checklist"
    echo "  -all, --all          Show all information"
    echo
    echo "Examples:"
    echo "  $0 --ftp             Show FTP deployment guide"
    echo "  $0 --test            Show testing checklist"
    echo "  $0 --all             Show all information"
}

# Function to show all information
show_all_info() {
    show_deployment_options
    show_ftp_deployment
    show_git_deployment
    show_cpanel_deployment
    show_ssh_deployment
    show_cloud_deployment
    show_cache_clearing
    show_testing_checklist
}

# Main script logic
main() {
    case "${1:-}" in
        -h|--help)
            show_help
            ;;
        -o|--options)
            print_banner
            show_deployment_options
            ;;
        -f|--ftp)
            print_banner
            show_ftp_deployment
            ;;
        -g|--git)
            print_banner
            show_git_deployment
            ;;
        -c|--cpanel)
            print_banner
            show_cpanel_deployment
            ;;
        -s|--ssh)
            print_banner
            show_ssh_deployment
            ;;
        -l|--cloud)
            print_banner
            show_cloud_deployment
            ;;
        -a|--cache)
            print_banner
            show_cache_clearing
            ;;
        -t|--test)
            print_banner
            show_testing_checklist
            ;;
        -all|--all)
            print_banner
            show_all_info
            ;;
        *)
            print_banner
            show_help
            ;;
    esac
}

# Run main function with all arguments
main "$@"
