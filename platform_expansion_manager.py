#!/usr/bin/env python3
"""
USDTgVerse Platform Ekleme Script'i
Yeni sosyal medya platformları ekleme sistemi
"""

import webbrowser
import time
import json
import os

class PlatformExpansionManager:
    def __init__(self):
        self.platforms = {
            "twitter": {"enabled": True, "priority": 1, "status": "active"},
            "linkedin": {"enabled": True, "priority": 2, "status": "active"},
            "instagram": {"enabled": True, "priority": 3, "status": "active"},
            "youtube": {"enabled": False, "priority": 1, "status": "future"},
            "discord": {"enabled": False, "priority": 1, "status": "future"},
            "tiktok": {"enabled": False, "priority": 3, "status": "future"},
            "twitch": {"enabled": False, "priority": 3, "status": "future"},
            "medium": {"enabled": False, "priority": 2, "status": "future"},
            "reddit": {"enabled": False, "priority": 2, "status": "future"},
            "telegram": {"enabled": False, "priority": 1, "status": "future"}
        }
        
        self.platform_urls = {
            "youtube": "https://console.developers.google.com/",
            "discord": "https://discord.com/developers/applications",
            "tiktok": "https://developers.tiktok.com/",
            "twitch": "https://dev.twitch.tv/",
            "medium": "https://medium.com/me/settings",
            "reddit": "https://www.reddit.com/prefs/apps",
            "telegram": "https://t.me/BotFather"
        }
    
    def print_banner(self):
        print("""
        ╔══════════════════════════════════════════════════════════════╗
        ║                                                              ║
        ║           🌐 USDTgVerse Platform Expansion                  ║
        ║                                                              ║
        ║              Sosyal Medya Platform Genişletme                ║
        ║                                                              ║
        ╚══════════════════════════════════════════════════════════════╝
        """)
    
    def show_current_status(self):
        """Mevcut platform durumunu göster"""
        print("\n📱 CURRENT PLATFORM STATUS:")
        print("=" * 60)
        
        for platform, config in self.platforms.items():
            status_icon = "✅" if config["enabled"] else "⏳"
            status_text = "ACTIVE" if config["enabled"] else "FUTURE"
            priority = config["priority"]
            
            print(f"{status_icon} {platform.upper()}: {status_text} (Priority: {priority})")
        
        print("\n📊 Summary:")
        active_count = sum(1 for config in self.platforms.values() if config["enabled"])
        future_count = sum(1 for config in self.platforms.values() if not config["enabled"])
        
        print(f"  Active Platforms: {active_count}")
        print(f"  Future Platforms: {future_count}")
        print(f"  Total Platforms: {len(self.platforms)}")
    
    def show_expansion_options(self):
        """Genişletme seçeneklerini göster"""
        print("\n🚀 PLATFORM EXPANSION OPTIONS:")
        print("=" * 60)
        
        future_platforms = [p for p, config in self.platforms.items() if not config["enabled"]]
        
        for i, platform in enumerate(future_platforms, 1):
            priority = self.platforms[platform]["priority"]
            print(f"{i}. {platform.upper()} (Priority: {priority})")
        
        print(f"\n{len(future_platforms) + 1}. Add Custom Platform")
        print(f"{len(future_platforms) + 2}. View All Platforms")
        print(f"{len(future_platforms) + 3}. Exit")
    
    def add_platform(self, platform_name):
        """Platform ekle"""
        print(f"\n🔧 ADDING {platform_name.upper()} PLATFORM:")
        print("=" * 50)
        
        if platform_name in self.platform_urls:
            print(f"1. Opening {platform_name} developer portal...")
            webbrowser.open(self.platform_urls[platform_name])
            time.sleep(2)
            
            print(f"2. Follow these steps:")
            self.show_platform_setup_guide(platform_name)
            
            print(f"3. API credentials needed:")
            self.show_api_requirements(platform_name)
            
            # Platform'u aktif et
            self.platforms[platform_name]["enabled"] = True
            self.platforms[platform_name]["status"] = "active"
            
            print(f"✅ {platform_name.upper()} platform added successfully!")
            
        else:
            print(f"❌ {platform_name} platform not supported yet")
    
    def show_platform_setup_guide(self, platform):
        """Platform kurulum rehberi göster"""
        guides = {
            "youtube": [
                "Go to Google Cloud Console",
                "Create a new project",
                "Enable YouTube Data API v3",
                "Create credentials (API Key)",
                "Copy the API key"
            ],
            "discord": [
                "Go to Discord Developer Portal",
                "Create New Application",
                "Go to Bot section",
                "Create Bot",
                "Copy Bot Token"
            ],
            "tiktok": [
                "Go to TikTok for Developers",
                "Create App",
                "Fill app information",
                "Get API Key",
                "Copy credentials"
            ],
            "twitch": [
                "Go to Twitch Developer Console",
                "Register Your Application",
                "Fill app details",
                "Get Client ID",
                "Copy Client Secret"
            ],
            "medium": [
                "Go to Medium Settings",
                "Integration tokens section",
                "Get integration token",
                "Copy token"
            ],
            "reddit": [
                "Go to Reddit App Preferences",
                "Create App",
                "Select 'script' type",
                "Get Client ID",
                "Copy Client Secret"
            ],
            "telegram": [
                "Message @BotFather on Telegram",
                "Use /newbot command",
                "Enter bot name",
                "Get Bot Token",
                "Copy token"
            ]
        }
        
        if platform in guides:
            for i, step in enumerate(guides[platform], 1):
                print(f"   {i}. {step}")
    
    def show_api_requirements(self, platform):
        """API gereksinimlerini göster"""
        requirements = {
            "youtube": ["API Key", "Channel ID"],
            "discord": ["Bot Token", "Guild ID"],
            "tiktok": ["Username", "Password"],
            "twitch": ["Client ID", "Client Secret"],
            "medium": ["Integration Token"],
            "reddit": ["Client ID", "Client Secret"],
            "telegram": ["Bot Token"]
        }
        
        if platform in requirements:
            print(f"   Required: {', '.join(requirements[platform])}")
    
    def save_platforms(self):
        """Platform konfigürasyonunu kaydet"""
        os.makedirs("config", exist_ok=True)
        with open("config/platforms.json", "w") as f:
            json.dump(self.platforms, f, indent=2)
    
    def load_platforms(self):
        """Platform konfigürasyonunu yükle"""
        if os.path.exists("config/platforms.json"):
            with open("config/platforms.json", "r") as f:
                self.platforms = json.load(f)
    
    def run_expansion_manager(self):
        """Genişletme yöneticisini çalıştır"""
        self.print_banner()
        self.load_platforms()
        
        while True:
            self.show_current_status()
            self.show_expansion_options()
            
            try:
                choice = input("\nSelect option: ")
                
                if choice.isdigit():
                    choice = int(choice)
                    future_platforms = [p for p, config in self.platforms.items() if not config["enabled"]]
                    
                    if 1 <= choice <= len(future_platforms):
                        platform = future_platforms[choice - 1]
                        self.add_platform(platform)
                        self.save_platforms()
                    
                    elif choice == len(future_platforms) + 1:
                        custom_platform = input("Enter custom platform name: ").lower()
                        self.add_platform(custom_platform)
                        self.save_platforms()
                    
                    elif choice == len(future_platforms) + 2:
                        self.show_current_status()
                    
                    elif choice == len(future_platforms) + 3:
                        print("\n👋 Goodbye!")
                        break
                    
                    else:
                        print("❌ Invalid option")
                
                else:
                    print("❌ Please enter a number")
                
                input("\nPress Enter to continue...")
                
            except KeyboardInterrupt:
                print("\n👋 Goodbye!")
                break
            except Exception as e:
                print(f"❌ Error: {e}")

def main():
    """Ana fonksiyon"""
    manager = PlatformExpansionManager()
    manager.run_expansion_manager()

if __name__ == "__main__":
    main()
