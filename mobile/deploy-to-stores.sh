#!/bin/bash

# USDTgVerse Wallet - Store Deployment Script
# This script prepares and builds the apps for both App Store and Google Play Store

set -e

echo "🚀 USDTgVerse Wallet - Store Deployment Script"
echo "=============================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if we're in the right directory
if [ ! -d "android/USDTgWallet" ] || [ ! -d "ios/USDTgWallet" ]; then
    print_error "Please run this script from the mobile/ directory"
    exit 1
fi

print_status "Starting deployment preparation..."

# Android Deployment
print_status "🤖 Preparing Android App for Google Play Store..."

cd android/USDTgWallet

# Clean previous builds
print_status "Cleaning previous builds..."
./gradlew clean

# Update version code and name
print_status "Updating version information..."
CURRENT_VERSION_CODE=$(grep "versionCode" app/build.gradle | sed 's/.*versionCode //' | sed 's/ //')
NEW_VERSION_CODE=$((CURRENT_VERSION_CODE + 1))
CURRENT_VERSION_NAME=$(grep "versionName" app/build.gradle | sed 's/.*versionName "//' | sed 's/"//')

print_status "Current version: $CURRENT_VERSION_NAME ($CURRENT_VERSION_CODE)"
print_status "New version code: $NEW_VERSION_CODE"

# Update build.gradle
sed -i.bak "s/versionCode $CURRENT_VERSION_CODE/versionCode $NEW_VERSION_CODE/" app/build.gradle

# Generate App Bundle for Google Play Store
print_status "Generating App Bundle (.aab)..."
./gradlew bundleRelease

if [ -f "app/build/outputs/bundle/release/app-release.aab" ]; then
    print_success "App Bundle generated: app/build/outputs/bundle/release/app-release.aab"
    
    # Copy to deployment directory
    mkdir -p ../../deployment/android
    cp app/build/outputs/bundle/release/app-release.aab ../../deployment/android/usdtgverse-wallet-v$CURRENT_VERSION_NAME.aab
    print_success "App Bundle copied to deployment/android/"
else
    print_error "Failed to generate App Bundle"
    exit 1
fi

# Generate APK for testing
print_status "Generating APK for testing..."
./gradlew assembleRelease

if [ -f "app/build/outputs/apk/release/app-release.apk" ]; then
    print_success "APK generated: app/build/outputs/apk/release/app-release.apk"
    
    # Copy to deployment directory
    cp app/build/outputs/apk/release/app-release.apk ../../deployment/android/usdtgverse-wallet-v$CURRENT_VERSION_NAME.apk
    print_success "APK copied to deployment/android/"
else
    print_error "Failed to generate APK"
    exit 1
fi

cd ../..

# iOS Deployment
print_status "🍎 Preparing iOS App for App Store..."

cd ios/USDTgWallet

# Check if Xcode is available
if ! command -v xcodebuild &> /dev/null; then
    print_error "Xcode is not installed or not in PATH"
    print_warning "Please install Xcode from the Mac App Store"
    exit 1
fi

# Clean previous builds
print_status "Cleaning previous builds..."
xcodebuild clean -project USDTgWallet.xcodeproj -scheme USDTgWallet

# Archive for App Store
print_status "Creating archive for App Store..."
xcodebuild archive \
    -project USDTgWallet.xcodeproj \
    -scheme USDTgWallet \
    -configuration Release \
    -archivePath ../../deployment/ios/USDTgWallet.xcarchive \
    -allowProvisioningUpdates

if [ -d "../../deployment/ios/USDTgWallet.xcarchive" ]; then
    print_success "Archive created: deployment/ios/USDTgWallet.xcarchive"
    
    # Export IPA for App Store
    print_status "Exporting IPA for App Store..."
    
    # Create export options plist
    cat > ../../deployment/ios/ExportOptions.plist << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>method</key>
    <string>app-store</string>
    <key>teamID</key>
    <string>YOUR_TEAM_ID</string>
    <key>uploadBitcode</key>
    <false/>
    <key>uploadSymbols</key>
    <true/>
    <key>compileBitcode</key>
    <false/>
</dict>
</plist>
EOF
    
    xcodebuild -exportArchive \
        -archivePath ../../deployment/ios/USDTgWallet.xcarchive \
        -exportPath ../../deployment/ios/ \
        -exportOptionsPlist ../../deployment/ios/ExportOptions.plist \
        -allowProvisioningUpdates
    
    if [ -f "../../deployment/ios/USDTgWallet.ipa" ]; then
        print_success "IPA exported: deployment/ios/USDTgWallet.ipa"
    else
        print_error "Failed to export IPA"
        exit 1
    fi
else
    print_error "Failed to create archive"
    exit 1
fi

cd ../..

# Create deployment summary
print_status "📋 Creating deployment summary..."

cat > deployment/DEPLOYMENT_SUMMARY.md << EOF
# USDTgVerse Wallet - Deployment Summary

## Deployment Date
$(date)

## Android (Google Play Store)
- **App Bundle**: \`deployment/android/usdtgverse-wallet-v$CURRENT_VERSION_NAME.aab\`
- **APK**: \`deployment/android/usdtgverse-wallet-v$CURRENT_VERSION_NAME.apk\`
- **Version**: $CURRENT_VERSION_NAME ($NEW_VERSION_CODE)
- **Package**: com.usdtgverse.wallet

### Google Play Store Steps:
1. Go to [Google Play Console](https://play.google.com/console)
2. Create new app or upload new version
3. Upload the .aab file
4. Fill in store listing information
5. Submit for review

## iOS (App Store)
- **IPA**: \`deployment/ios/USDTgWallet.ipa\`
- **Archive**: \`deployment/ios/USDTgWallet.xcarchive\`
- **Version**: $CURRENT_VERSION_NAME

### App Store Steps:
1. Open Xcode
2. Go to Window > Organizer
3. Select the archive
4. Click "Distribute App"
5. Choose "App Store Connect"
6. Upload to App Store Connect

## Next Steps:
1. Update Team ID in ExportOptions.plist
2. Configure signing certificates
3. Test apps thoroughly
4. Submit for review
5. Monitor review status

## Store Listing Information:
- **Android**: See \`android/USDTgWallet/store-listing.md\`
- **iOS**: See \`ios/USDTgWallet/AppStore-listing.md\`

## Support:
- Email: support@usdtgverse.com
- Website: https://www.usdtgverse.com
EOF

print_success "Deployment summary created: deployment/DEPLOYMENT_SUMMARY.md"

# Final status
print_status "🎉 Deployment preparation completed!"
print_success "Android App Bundle: deployment/android/usdtgverse-wallet-v$CURRENT_VERSION_NAME.aab"
print_success "Android APK: deployment/android/usdtgverse-wallet-v$CURRENT_VERSION_NAME.apk"
print_success "iOS IPA: deployment/ios/USDTgWallet.ipa"
print_success "Deployment Summary: deployment/DEPLOYMENT_SUMMARY.md"

print_warning "⚠️  Remember to:"
print_warning "   1. Update Team ID in ExportOptions.plist"
print_warning "   2. Configure signing certificates"
print_warning "   3. Test apps before submission"
print_warning "   4. Fill in store listing information"

echo ""
print_status "Ready for store submission! 🚀"
