#!/usr/bin/env python3
"""
USDTgVerse API Keys - Browser Otomatik Açma
"""

import webbrowser
import time

def open_api_portals():
    """API portal'larını otomatik aç"""
    print("🚀 USDTgVerse API Keys - Browser Otomatik Açma")
    print("=" * 60)
    
    # Twitter Developer Portal
    print("\n🐦 Opening Twitter Developer Portal...")
    webbrowser.open("https://developer.twitter.com/en/portal/dashboard")
    time.sleep(2)
    
    # LinkedIn Company Page
    print("💼 Opening LinkedIn Company Page...")
    webbrowser.open("https://www.linkedin.com/company/setup/new/")
    time.sleep(2)
    
    # Instagram Business Account
    print("📸 Opening Instagram Business Account...")
    webbrowser.open("https://www.instagram.com/accounts/emailsignup/")
    time.sleep(2)
    
    print("\n✅ All portals opened!")
    print("\n📋 Next steps:")
    print("1. Fill forms with USDTgVerse information")
    print("2. Collect API keys")
    print("3. Update .env file")
    print("4. Start bot: python3 start_social_bot.py")

if __name__ == "__main__":
    open_api_portals()
