#!/usr/bin/env python3
"""
USDTgVerse - Tüm Platform Developer Sayfalarını Açma
API Keys alma için tüm gerekli sayfaları otomatik aç
"""

import webbrowser
import time
import subprocess
import os

def print_banner():
    print("""
    ╔══════════════════════════════════════════════════════════════╗
    ║                                                              ║
    ║           🌐 USDTgVerse API Keys - Tüm Sayfalar            ║
    ║                                                              ║
    ║              Developer Portal'ları Otomatik Açma           ║
    ║                                                              ║
    ╚══════════════════════════════════════════════════════════════╝
    """)

def open_all_platforms():
    """Tüm platform developer sayfalarını aç"""
    print("🚀 Opening all platform developer portals...")
    print("=" * 60)
    
    platforms = {
        "Twitter/X": {
            "url": "https://developer.twitter.com/en/portal/dashboard",
            "description": "Twitter Developer Portal - API Keys için",
            "steps": [
                "Apply for a developer account",
                "Select 'Making a bot'",
                "Fill company info: USDTgVerse",
                "Email: info@usdtgverse.com",
                "Website: https://www.usdtgverse.com"
            ]
        },
        "LinkedIn": {
            "url": "https://www.linkedin.com/company/setup/new/",
            "description": "LinkedIn Company Page - Business account için",
            "steps": [
                "Create company page",
                "Company Name: USDTgVerse",
                "Email: info@usdtgverse.com",
                "Website: https://www.usdtgverse.com",
                "Industry: Technology"
            ]
        },
        "Instagram": {
            "url": "https://www.instagram.com/accounts/emailsignup/",
            "description": "Instagram Business Account - @usdtgverse için",
            "steps": [
                "Create business account",
                "Username: @usdtgverse",
                "Email: info@usdtgverse.com",
                "Website: https://www.usdtgverse.com",
                "Category: Technology"
            ]
        },
        "YouTube": {
            "url": "https://console.developers.google.com/",
            "description": "Google Cloud Console - YouTube API için",
            "steps": [
                "Create new project",
                "Enable YouTube Data API v3",
                "Create credentials (API Key)",
                "Copy API key",
                "Channel: USDTgVerse"
            ]
        },
        "Discord": {
            "url": "https://discord.com/developers/applications",
            "description": "Discord Developer Portal - Bot için",
            "steps": [
                "Create New Application",
                "Go to Bot section",
                "Create Bot",
                "Copy Bot Token",
                "Invite bot to server"
            ]
        },
        "TikTok": {
            "url": "https://developers.tiktok.com/",
            "description": "TikTok for Developers - API için",
            "steps": [
                "Create App",
                "App Name: USDTgVerse",
                "Email: info@usdtgverse.com",
                "Get API Key",
                "Username: @usdtgverse"
            ]
        },
        "Twitch": {
            "url": "https://dev.twitch.tv/",
            "description": "Twitch Developer Console - API için",
            "steps": [
                "Register Your Application",
                "App Name: USDTgVerse",
                "Email: info@usdtgverse.com",
                "Get Client ID",
                "Copy Client Secret"
            ]
        },
        "Medium": {
            "url": "https://medium.com/me/settings",
            "description": "Medium Settings - Integration Token için",
            "steps": [
                "Go to Integration tokens",
                "Get integration token",
                "Copy token",
                "Username: @usdtgverse",
                "Email: info@usdtgverse.com"
            ]
        },
        "Reddit": {
            "url": "https://www.reddit.com/prefs/apps",
            "description": "Reddit App Preferences - API için",
            "steps": [
                "Create App",
                "App Name: USDTgVerse",
                "Select 'script' type",
                "Get Client ID",
                "Copy Client Secret"
            ]
        },
        "Telegram": {
            "url": "https://t.me/BotFather",
            "description": "Telegram BotFather - Bot için",
            "steps": [
                "Message @BotFather",
                "Use /newbot command",
                "Bot Name: USDTgVerse Bot",
                "Username: @usdtgverse_bot",
                "Copy Bot Token"
            ]
        }
    }
    
    for i, (platform, info) in enumerate(platforms.items(), 1):
        print(f"\n{i}. {platform.upper()}")
        print(f"   📱 {info['description']}")
        print(f"   🌐 Opening: {info['url']}")
        
        # Browser'da aç
        webbrowser.open(info['url'])
        time.sleep(1)
        
        print(f"   📋 Steps:")
        for step in info['steps']:
            print(f"      • {step}")
    
    print(f"\n✅ All {len(platforms)} platforms opened!")
    print("\n📋 Next steps:")
    print("1. Fill forms with USDTgVerse information")
    print("2. Collect API keys from each platform")
    print("3. Update .env file with credentials")
    print("4. Start bot: python3 start_social_bot.py")

def create_env_template():
    """Environment template oluştur"""
    print("\n📝 Creating .env template...")
    
    env_content = """# USDTgVerse Social Media Bot Environment Variables
# Bu dosyayı güvenli tutun ve git'e commit etmeyin!

# Twitter API Keys (https://developer.twitter.com/)
TWITTER_API_KEY=your_twitter_api_key_here
TWITTER_API_SECRET=your_twitter_api_secret_here
TWITTER_ACCESS_TOKEN=your_twitter_access_token_here
TWITTER_ACCESS_SECRET=your_twitter_access_secret_here
TWITTER_BEARER_TOKEN=your_twitter_bearer_token_here

# LinkedIn Credentials
LINKEDIN_EMAIL=info@usdtgverse.com
LINKEDIN_PASSWORD=your_linkedin_password_here

# Instagram Credentials
INSTAGRAM_USERNAME=usdtgverse
INSTAGRAM_PASSWORD=your_instagram_password_here

# YouTube API
YOUTUBE_API_KEY=your_youtube_api_key_here
YOUTUBE_CHANNEL_ID=your_youtube_channel_id_here

# Discord Bot
DISCORD_BOT_TOKEN=your_discord_bot_token_here
DISCORD_GUILD_ID=your_discord_guild_id_here

# TikTok API
TIKTOK_USERNAME=usdtgverse
TIKTOK_PASSWORD=your_tiktok_password_here

# Twitch API
TWITCH_CLIENT_ID=your_twitch_client_id_here
TWITCH_CLIENT_SECRET=your_twitch_client_secret_here

# Medium API
MEDIUM_INTEGRATION_TOKEN=your_medium_integration_token_here

# Reddit API
REDDIT_CLIENT_ID=your_reddit_client_id_here
REDDIT_CLIENT_SECRET=your_reddit_client_secret_here

# Telegram Bot
TELEGRAM_BOT_TOKEN=your_telegram_bot_token_here

# Bot Settings
MIN_POST_INTERVAL=7200
MAX_POST_INTERVAL=28800
ENABLE_ANALYTICS=true
ENABLE_ENGAGEMENT_BOT=true

# Platform Settings
ENABLE_TWITTER=true
ENABLE_LINKEDIN=true
ENABLE_INSTAGRAM=true
ENABLE_YOUTUBE=false
ENABLE_DISCORD=false
ENABLE_TIKTOK=false
ENABLE_TWITCH=false
ENABLE_MEDIUM=false
ENABLE_REDDIT=false
ENABLE_TELEGRAM=false
"""
    
    with open(".env", "w") as f:
        f.write(env_content)
    
    print("✅ Created .env template")
    print("📝 Next: Update .env file with your actual API keys")

def show_quick_reference():
    """Hızlı referans göster"""
    print("\n📋 QUICK REFERENCE:")
    print("=" * 60)
    
    print("🏢 Company Information:")
    print("   Company: USDTgVerse")
    print("   Email: info@usdtgverse.com")
    print("   Website: https://www.usdtgverse.com")
    print("   Username: @usdtgverse")
    
    print("\n🔑 API Keys Needed:")
    print("   Twitter: API Key, Secret, Access Token, Access Secret, Bearer Token")
    print("   LinkedIn: Email, Password")
    print("   Instagram: Username, Password")
    print("   YouTube: API Key, Channel ID")
    print("   Discord: Bot Token, Guild ID")
    print("   TikTok: Username, Password")
    print("   Twitch: Client ID, Client Secret")
    print("   Medium: Integration Token")
    print("   Reddit: Client ID, Client Secret")
    print("   Telegram: Bot Token")
    
    print("\n⏰ Estimated Time:")
    print("   Basic platforms (Twitter, LinkedIn, Instagram): 5-10 minutes")
    print("   Extended platforms (YouTube, Discord, etc.): 20-30 minutes")
    print("   Total setup time: 30-40 minutes")

def main():
    """Ana fonksiyon"""
    print_banner()
    
    print("🚀 USDTgVerse API Keys - All Platforms")
    print("Opening all developer portals for API key collection...")
    
    input("\nPress Enter to start opening all platforms...")
    
    # Tüm platformları aç
    open_all_platforms()
    
    # Environment template oluştur
    create_env_template()
    
    # Hızlı referans göster
    show_quick_reference()
    
    print("\n🎉 All platforms opened!")
    print("📱 Start collecting API keys from each platform")
    print("🔧 Update .env file with your credentials")
    print("🚀 Run: python3 start_social_bot.py")

if __name__ == "__main__":
    main()
