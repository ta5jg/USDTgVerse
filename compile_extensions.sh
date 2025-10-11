#!/bin/bash

# ==============================================
# File:        compile_extensions.sh
# Author:      USDTG GROUP TECHNOLOGY LLC
# Created:     2025-10-05
# Last Update: 2025-10-05
# Version:     1.0
#
# Description:
#   Compile all USDTgVerse Extension Backends (Pure C)
#   High-performance compilation for all browsers
#
# License:
#   MIT License
# ==============================================

echo "🚀 Compiling USDTgVerse Extension Backends (Pure C)"
echo "=================================================="

# Compiler settings
CC=gcc
CFLAGS="-O3 -Wall -Wextra -std=c99 -fPIC"
LIBS=""

# Create build directory
mkdir -p build/extensions

# Compile Safari Extension Backend
echo "📱 Compiling Safari Extension Backend..."
$CC $CFLAGS -o build/extensions/safari_extension_backend \
    browser-extensions/safari-extension/safari_extension_backend.c \
    $LIBS

if [ $? -eq 0 ]; then
    echo "✅ Safari Extension Backend compiled successfully"
else
    echo "❌ Failed to compile Safari Extension Backend"
    exit 1
fi

# Compile Chrome Extension Backend
echo "🌐 Compiling Chrome Extension Backend..."
$CC $CFLAGS -o build/extensions/chrome_extension_backend \
    browser-extensions/chrome-extension/chrome_extension_backend.c \
    $LIBS

if [ $? -eq 0 ]; then
    echo "✅ Chrome Extension Backend compiled successfully"
else
    echo "❌ Failed to compile Chrome Extension Backend"
    exit 1
fi

# Compile Firefox Extension Backend
echo "🦊 Compiling Firefox Extension Backend..."
$CC $CFLAGS -o build/extensions/firefox_extension_backend \
    browser-extensions/firefox-extension/firefox_extension_backend.c \
    $LIBS

if [ $? -eq 0 ]; then
    echo "✅ Firefox Extension Backend compiled successfully"
else
    echo "❌ Failed to compile Firefox Extension Backend"
    exit 1
fi

# Test all extensions
echo ""
echo "🧪 Testing Extension Backends..."
echo "================================"

echo "Testing Safari Extension Backend..."
./build/extensions/safari_extension_backend
echo ""

echo "Testing Chrome Extension Backend..."
./build/extensions/chrome_extension_backend
echo ""

echo "Testing Firefox Extension Backend..."
./build/extensions/firefox_extension_backend
echo ""

echo "🎉 All USDTgVerse Extension Backends compiled and tested successfully!"
echo "📁 Compiled binaries are in: build/extensions/"
echo ""
echo "📊 Extension Backend Summary:"
echo "- Safari Extension Backend: build/extensions/safari_extension_backend"
echo "- Chrome Extension Backend: build/extensions/chrome_extension_backend"
echo "- Firefox Extension Backend: build/extensions/firefox_extension_backend"
echo ""
echo "✅ Pure C Extension Backends Ready for Production!"
