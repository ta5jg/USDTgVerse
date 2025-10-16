#!/usr/bin/env python3
"""
USDTgVerse API Keys Otomatik Alma Sistemi
En hızlı yöntemlerle API keys alma
"""

import webbrowser
import time
import subprocess
import os

class AutoAPIKeyGetter:
    def __init__(self):
        self.email = "info@usdtgverse.com"
        self.company = "USDTgVerse"
        self.website = "https://www.usdtgverse.com"
        self.username = "usdtgverse"
        
    def print_banner(self):
        print("""
        ╔══════════════════════════════════════════════════════════════╗
        ║                                                              ║
        ║           🔑 USDTgVerse API Keys Auto Setup                 ║
        ║                                                              ║
        ║              En Hızlı API Keys Alma Sistemi                 ║
        ║                                                              ║
        ╚══════════════════════════════════════════════════════════════╝
        """)
    
    def open_twitter_developer(self):
        """Twitter Developer Portal'ı aç"""
        print("\n🐦 TWITTER API KEYS ALMA:")
        print("=" * 50)
        print("1. Opening Twitter Developer Portal...")
        
        # Browser'da aç
        webbrowser.open("https://developer.twitter.com/en/portal/dashboard")
        
        print("2. Manual steps:")
        print("   ✅ Click 'Apply for a developer account'")
        print("   ✅ Select 'Making a bot'")
        print("   ✅ Fill form with:")
        print(f"      - Company: {self.company}")
        print(f"      - Email: {self.email}")
        print(f"      - Website: {self.website}")
        print("   ✅ Use case: 'Social media automation for blockchain project'")
        print("   ✅ Description: 'Automated posting for USDTgVerse quantum-safe blockchain ecosystem'")
        print("   ✅ Submit application")
        
        print("\n⏰ Expected time: 2-5 minutes")
        print("📋 Status: Usually auto-approved for business accounts")
        
        input("\nPress Enter when Twitter setup is complete...")
        
    def open_linkedin_company(self):
        """LinkedIn Company Page oluştur"""
        print("\n💼 LINKEDIN COMPANY PAGE:")
        print("=" * 50)
        print("1. Opening LinkedIn Company Page...")
        
        # Browser'da aç
        webbrowser.open("https://www.linkedin.com/company/setup/new/")
        
        print("2. Manual steps:")
        print("   ✅ Fill company information:")
        print(f"      - Company Name: {self.company}")
        print(f"      - Email: {self.email}")
        print(f"      - Website: {self.website}")
        print("   ✅ Industry: Technology/Blockchain")
        print("   ✅ Company Size: 1-10 employees")
        print("   ✅ Create company page")
        
        print("\n⏰ Expected time: 1-2 minutes")
        print("📋 Status: Instant approval")
        
        input("\nPress Enter when LinkedIn setup is complete...")
        
    def open_instagram_business(self):
        """Instagram Business Account oluştur"""
        print("\n📸 INSTAGRAM BUSINESS ACCOUNT:")
        print("=" * 50)
        print("1. Opening Instagram Business Account...")
        
        # Browser'da aç
        webbrowser.open("https://www.instagram.com/accounts/emailsignup/")
        
        print("2. Manual steps:")
        print("   ✅ Create business account:")
        print(f"      - Username: @{self.username}")
        print(f"      - Email: {self.email}")
        print(f"      - Website: {self.website}")
        print("   ✅ Switch to business account")
        print("   ✅ Add business information")
        print("   ✅ Connect to Facebook page (optional)")
        
        print("\n⏰ Expected time: 2-3 minutes")
        print("📋 Status: Instant approval")
        
        input("\nPress Enter when Instagram setup is complete...")
        
    def create_env_file(self):
        """Environment dosyası oluştur"""
        print("\n📝 CREATING .env FILE:")
        print("=" * 50)
        
        env_content = f"""# USDTgVerse Social Media Bot Environment Variables
# Bu dosyayı güvenli tutun ve git'e commit etmeyin!

# Twitter API Keys (https://developer.twitter.com/)
TWITTER_API_KEY=your_twitter_api_key_here
TWITTER_API_SECRET=your_twitter_api_secret_here
TWITTER_ACCESS_TOKEN=your_twitter_access_token_here
TWITTER_ACCESS_SECRET=your_twitter_access_secret_here
TWITTER_BEARER_TOKEN=your_twitter_bearer_token_here

# LinkedIn Credentials
LINKEDIN_EMAIL={self.email}
LINKEDIN_PASSWORD=your_linkedin_password_here

# Instagram Credentials
INSTAGRAM_USERNAME={self.username}
INSTAGRAM_PASSWORD=your_instagram_password_here

# Bot Settings
MIN_POST_INTERVAL=7200
MAX_POST_INTERVAL=28800
ENABLE_ANALYTICS=true
ENABLE_ENGAGEMENT_BOT=true

# Platform Settings
ENABLE_TWITTER=true
ENABLE_LINKEDIN=true
ENABLE_INSTAGRAM=true
"""
        
        with open(".env", "w") as f:
            f.write(env_content)
        
        print("✅ Created .env file")
        print("📝 Next: Update .env file with your actual API keys")
        
    def show_next_steps(self):
        """Sonraki adımları göster"""
        print("\n🎯 NEXT STEPS:")
        print("=" * 50)
        print("1. 📝 Update .env file with your API keys")
        print("2. 🧪 Test the bot: python3 test_social_bot.py")
        print("3. 🚀 Start production: python3 start_social_bot.py")
        print("4. 📊 Monitor: python3 monitor_bot.py")
        
        print("\n📋 API Keys to collect:")
        print("   - Twitter: API Key, Secret, Access Token, Access Secret, Bearer Token")
        print("   - LinkedIn: Email, Password")
        print("   - Instagram: Username, Password")
        
        print("\n⏰ Total time needed: 5-10 minutes")
        print("🎉 After setup: Fully automated social media!")
        
    def run_auto_setup(self):
        """Otomatik kurulum başlat"""
        self.print_banner()
        
        print("🚀 Starting USDTgVerse API Keys Auto Setup...")
        print(f"📧 Email: {self.email}")
        print(f"🏢 Company: {self.company}")
        print(f"🌐 Website: {self.website}")
        print(f"👤 Username: @{self.username}")
        
        input("\nPress Enter to start...")
        
        # Twitter
        self.open_twitter_developer()
        
        # LinkedIn
        self.open_linkedin_company()
        
        # Instagram
        self.open_instagram_business()
        
        # Environment file
        self.create_env_file()
        
        # Next steps
        self.show_next_steps()
        
        print("\n🎉 API Keys setup complete!")
        print("📱 Your social media bot is ready to launch!")

def main():
    """Ana fonksiyon"""
    getter = AutoAPIKeyGetter()
    getter.run_auto_setup()

if __name__ == "__main__":
    main()
