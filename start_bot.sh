#!/bin/bash
# ==============================================
# File:        start_bot.sh
# Author:      USDTG GROUP TECHNOLOGY LLC
# Created:     2025-10-05
# Last Update: 2025-10-05
# Version:     1.0
#
# Description:
#   USDTgVerse Social Media Bot Startup Script
#   
#   Automated social media management system
#   Professional bot startup and management
#
# License:
#   MIT License
# ==============================================

echo "🤖 Starting USDTgVerse Social Media Bot..."

# Python environment'ı aktif et
source venv/bin/activate 2>/dev/null || echo "Virtual environment not found"

# Bot'u başlat
python3 automated_social_media_system.py

echo "🛑 Bot stopped"
