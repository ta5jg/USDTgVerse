#!/bin/bash

# USDTgVerse Native IDE - Build Script
# 
# Created: 28.09.2025
# Last Update: 28.09.2025
# 
# Description: Build script for USDTgVerse Native IDE
# Author: USDTgVerse Development Team
# License: MIT

set -e

echo "🚀 Building USDTgVerse Native IDE..."

# Check if we're in the right directory
if [ ! -f "Makefile" ]; then
    echo "❌ Error: Makefile not found. Please run this script from the native-ide directory."
    exit 1
fi

# Install dependencies if needed
echo "📦 Checking dependencies..."
if ! pkg-config --exists libwebsockets; then
    echo "⚠️  libwebsockets not found. Installing dependencies..."
    make install-deps
fi

# Clean previous build
echo "🧹 Cleaning previous build..."
make clean

# Build the IDE
echo "🔨 Building IDE..."
make all

# Check if build was successful
if [ -f "usdtgverse-ide" ]; then
    echo "✅ Build successful!"
    echo "📁 Binary created: usdtgverse-ide"
    echo "🌐 Web interface: web/index.html"
    echo ""
    echo "🚀 To run the IDE:"
    echo "   make run"
    echo ""
    echo "🌐 To access the web interface:"
    echo "   http://localhost:8000"
else
    echo "❌ Build failed!"
    exit 1
fi
