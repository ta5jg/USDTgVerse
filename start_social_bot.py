#!/usr/bin/env python3
"""
USDTgVerse Social Media Bot - QUICK START GUIDE
Hızlı başlatma rehberi
"""

import os
import sys

def print_banner():
    print("""
    ╔══════════════════════════════════════════════════════════════╗
    ║                                                              ║
    ║           🤖 USDTgVerse Social Media Bot                    ║
    ║                                                              ║
    ║              Otomatik Sosyal Medya Yönetim Sistemi          ║
    ║                                                              ║
    ╚══════════════════════════════════════════════════════════════╝
    """)

def check_setup():
    """Kurulum kontrolü"""
    print("🔍 Checking setup...")
    
    required_files = [
        "config.py",
        ".env", 
        "automated_social_media_system.py",
        "test_social_bot.py"
    ]
    
    missing_files = []
    for file in required_files:
        if not os.path.exists(file):
            missing_files.append(file)
    
    if missing_files:
        print(f"❌ Missing files: {', '.join(missing_files)}")
        return False
    
    print("✅ All required files found")
    return True

def show_menu():
    """Menü göster"""
    print("\n📋 USDTgVerse Social Media Bot Menu:")
    print("=" * 50)
    print("1. 🧪 Test Mode (Simulation - No real posts)")
    print("2. 🚀 Production Mode (Real posts - API keys required)")
    print("3. 📊 View Analytics")
    print("4. ⚙️  Configure Settings")
    print("5. 📖 View Documentation")
    print("6. 🛑 Exit")
    print("=" * 50)

def run_test_mode():
    """Test modunu çalıştır"""
    print("\n🧪 Starting Test Mode...")
    print("This will simulate posts without making real ones")
    print("Press Ctrl+C to stop\n")
    
    try:
        os.system("python3 test_social_bot.py")
    except KeyboardInterrupt:
        print("\n🛑 Test mode stopped")

def run_production_mode():
    """Production modunu çalıştır"""
    print("\n🚀 Starting Production Mode...")
    print("⚠️  WARNING: This will make REAL posts!")
    print("Make sure you have configured API keys in .env file")
    
    confirm = input("\nAre you sure? (yes/no): ").lower()
    if confirm != "yes":
        print("❌ Cancelled")
        return
    
    try:
        os.system("python3 automated_social_media_system.py")
    except KeyboardInterrupt:
        print("\n🛑 Production mode stopped")

def view_analytics():
    """Analytics görüntüle"""
    print("\n📊 Viewing Analytics...")
    
    if os.path.exists("analytics/social_media_analytics.json"):
        try:
            os.system("python3 monitor_bot.py")
        except Exception as e:
            print(f"❌ Error viewing analytics: {e}")
    else:
        print("❌ No analytics data found")
        print("Run the bot first to generate analytics")

def configure_settings():
    """Ayarları yapılandır"""
    print("\n⚙️ Configuration Options:")
    print("=" * 40)
    print("1. Edit .env file (API keys)")
    print("2. Edit config.py (Bot settings)")
    print("3. View current settings")
    print("4. Back to main menu")
    
    choice = input("\nSelect option (1-4): ")
    
    if choice == "1":
        print("\n📝 Editing .env file...")
        os.system("nano .env")
    elif choice == "2":
        print("\n📝 Editing config.py...")
        os.system("nano config.py")
    elif choice == "3":
        print("\n📋 Current Settings:")
        if os.path.exists(".env"):
            with open(".env", "r") as f:
                print(f.read())
        else:
            print("❌ .env file not found")
    elif choice == "4":
        return
    else:
        print("❌ Invalid option")

def view_documentation():
    """Dokümantasyonu görüntüle"""
    print("\n📖 USDTgVerse Social Media Bot Documentation")
    print("=" * 60)
    
    if os.path.exists("README_SOCIAL_BOT.md"):
        with open("README_SOCIAL_BOT.md", "r") as f:
            content = f.read()
            # İlk 50 satırı göster
            lines = content.split('\n')[:50]
            for line in lines:
                print(line)
            print("\n... (truncated, see README_SOCIAL_BOT.md for full documentation)")
    else:
        print("❌ Documentation not found")

def main():
    """Ana fonksiyon"""
    print_banner()
    
    if not check_setup():
        print("\n❌ Setup incomplete. Please run setup_social_bot.py first")
        return
    
    while True:
        show_menu()
        choice = input("\nSelect option (1-6): ")
        
        if choice == "1":
            run_test_mode()
        elif choice == "2":
            run_production_mode()
        elif choice == "3":
            view_analytics()
        elif choice == "4":
            configure_settings()
        elif choice == "5":
            view_documentation()
        elif choice == "6":
            print("\n👋 Goodbye!")
            break
        else:
            print("❌ Invalid option. Please select 1-6.")
        
        input("\nPress Enter to continue...")

if __name__ == "__main__":
    main()
