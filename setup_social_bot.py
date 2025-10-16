#!/usr/bin/env python3
"""
USDTgVerse Social Media Bot Setup Script
Otomatik sosyal medya bot kurulum script'i
"""

import os
import sys
import subprocess
import json
from datetime import datetime

def print_banner():
    """Banner yazdır"""
    print("""
    ╔══════════════════════════════════════════════════════════════╗
    ║                                                              ║
    ║           🤖 USDTgVerse Social Media Bot Setup              ║
    ║                                                              ║
    ║              Otomatik Sosyal Medya Yönetim Sistemi          ║
    ║                                                              ║
    ╚══════════════════════════════════════════════════════════════╝
    """)

def check_python_version():
    """Python versiyonunu kontrol et"""
    if sys.version_info < (3, 7):
        print("❌ Python 3.7+ gerekli!")
        sys.exit(1)
    print(f"✅ Python {sys.version_info.major}.{sys.version_info.minor} detected")

def install_requirements():
    """Gerekli kütüphaneleri yükle"""
    print("\n📦 Installing required packages...")
    
    requirements = [
        "tweepy>=4.14.0",
        "linkedin-api>=2.2.0", 
        "instagrapi>=2.0.0",
        "schedule>=1.2.0",
        "requests>=2.31.0",
        "python-dotenv>=1.0.0",
        "pytz>=2023.3",
        "pillow>=10.0.0"
    ]
    
    for package in requirements:
        try:
            print(f"Installing {package}...")
            subprocess.check_call([sys.executable, "-m", "pip", "install", package])
            print(f"✅ {package} installed successfully")
        except subprocess.CalledProcessError:
            print(f"❌ Failed to install {package}")
            return False
    
    return True

def create_directories():
    """Gerekli dizinleri oluştur"""
    print("\n📁 Creating directories...")
    
    directories = [
        "logs",
        "backups", 
        "analytics",
        "content",
        "images"
    ]
    
    for directory in directories:
        if not os.path.exists(directory):
            os.makedirs(directory)
            print(f"✅ Created directory: {directory}")
        else:
            print(f"📁 Directory exists: {directory}")

def create_config_file():
    """Config dosyasını oluştur"""
    print("\n⚙️ Creating configuration file...")
    
    if not os.path.exists("config.py"):
        print("❌ config.py not found! Please create it first.")
        return False
    
    print("✅ Configuration file ready")
    return True

def create_env_file():
    """Environment dosyasını oluştur"""
    print("\n🔐 Creating environment file...")
    
    env_content = """# USDTgVerse Social Media Bot Environment Variables
# Bu dosyayı güvenli tutun ve git'e commit etmeyin!

# Twitter API Keys
TWITTER_API_KEY=your_twitter_api_key_here
TWITTER_API_SECRET=your_twitter_api_secret_here
TWITTER_ACCESS_TOKEN=your_twitter_access_token_here
TWITTER_ACCESS_SECRET=your_twitter_access_secret_here
TWITTER_BEARER_TOKEN=your_twitter_bearer_token_here

# LinkedIn Credentials
LINKEDIN_EMAIL=your_email@usdtgverse.com
LINKEDIN_PASSWORD=your_linkedin_password_here

# Instagram Credentials
INSTAGRAM_USERNAME=usdtgverse
INSTAGRAM_PASSWORD=your_instagram_password_here

# Bot Settings
MIN_POST_INTERVAL=7200
MAX_POST_INTERVAL=28800
ENABLE_ANALYTICS=true
ENABLE_ENGAGEMENT_BOT=true
"""
    
    if not os.path.exists(".env"):
        with open(".env", "w") as f:
            f.write(env_content)
        print("✅ Created .env file")
        print("⚠️  Please update .env file with your actual credentials!")
    else:
        print("📁 .env file already exists")

def create_startup_script():
    """Başlatma script'ini oluştur"""
    print("\n🚀 Creating startup script...")
    
    startup_content = """#!/bin/bash
# USDTgVerse Social Media Bot Startup Script

echo "🤖 Starting USDTgVerse Social Media Bot..."

# Python environment'ı aktif et
source venv/bin/activate 2>/dev/null || echo "Virtual environment not found"

# Bot'u başlat
python3 automated_social_media_system.py

echo "🛑 Bot stopped"
"""
    
    with open("start_bot.sh", "w") as f:
        f.write(startup_content)
    
    # Execute permission ver
    os.chmod("start_bot.sh", 0o755)
    print("✅ Created start_bot.sh")

def create_systemd_service():
    """Systemd service dosyası oluştur"""
    print("\n🔧 Creating systemd service...")
    
    service_content = """[Unit]
Description=USDTgVerse Social Media Bot
After=network.target

[Service]
Type=simple
User=usdtgverse
WorkingDirectory=/opt/usdtgverse/social-bot
ExecStart=/usr/bin/python3 automated_social_media_system.py
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
"""
    
    with open("usdtgverse-social-bot.service", "w") as f:
        f.write(service_content)
    
    print("✅ Created systemd service file")
    print("📋 To install service:")
    print("   sudo cp usdtgverse-social-bot.service /etc/systemd/system/")
    print("   sudo systemctl enable usdtgverse-social-bot")
    print("   sudo systemctl start usdtgverse-social-bot")

def create_monitoring_script():
    """Monitoring script'ini oluştur"""
    print("\n📊 Creating monitoring script...")
    
    monitoring_content = '''#!/usr/bin/env python3
"""
USDTgVerse Social Media Bot Monitoring Script
"""

import json
import os
from datetime import datetime, timedelta

def check_bot_health():
    """Bot sağlığını kontrol et"""
    analytics_file = "analytics/social_media_analytics.json"
    
    if not os.path.exists(analytics_file):
        print("❌ Analytics file not found")
        return False
    
    with open(analytics_file, 'r') as f:
        data = json.load(f)
    
    # Son 24 saatteki post sayısını kontrol et
    today = datetime.now().strftime("%Y-%m-%d")
    yesterday = (datetime.now() - timedelta(days=1)).strftime("%Y-%m-%d")
    
    if today in data.get("daily_stats", {}):
        today_posts = sum(platform.get("posts", 0) for platform in data["daily_stats"][today].values())
        print(f"📊 Today's posts: {today_posts}")
        
        if today_posts == 0:
            print("⚠️  No posts today - bot might be down")
            return False
    
    print("✅ Bot is healthy")
    return True

def generate_report():
    """Günlük rapor oluştur"""
    analytics_file = "analytics/social_media_analytics.json"
    
    if not os.path.exists(analytics_file):
        print("❌ Analytics file not found")
        return
    
    with open(analytics_file, 'r') as f:
        data = json.load(f)
    
    print("\\n📊 USDTgVerse Social Media Report")
    print("=" * 50)
    print(f"Total Posts: {data.get('total_posts', 0)}")
    print(f"Total Reach: {data.get('total_reach', 0)}")
    print(f"Total Engagement: {data.get('total_engagement', 0)}")
    
    # Platform breakdown
    if "daily_stats" in data:
        today = datetime.now().strftime("%Y-%m-%d")
        if today in data["daily_stats"]:
            print(f"\\nToday's Stats ({today}):")
            for platform, stats in data["daily_stats"][today].items():
                print(f"  {platform.upper()}: {stats.get('posts', 0)} posts")

if __name__ == "__main__":
    print("🔍 Checking bot health...")
    check_bot_health()
    generate_report()
'''
    
    with open("monitor_bot.py", "w") as f:
        f.write(monitoring_content)
    
    os.chmod("monitor_bot.py", 0o755)
    print("✅ Created monitor_bot.py")

def create_readme():
    """README dosyası oluştur"""
    print("\n📖 Creating README...")
    
    readme_content = """# 🤖 USDTgVerse Social Media Bot

Otomatik sosyal medya yönetim sistemi - USDTgVerse için geliştirilmiş.

## 🚀 Özellikler

- ✅ **Otomatik hesap oluşturma** (Twitter, LinkedIn, Instagram)
- ✅ **Random sıklıkta post yayınlama** (2-8 saat arası)
- ✅ **Çeşitli içerik türleri** (teknik, haber, eğlenceli)
- ✅ **Trend hashtag'ler** otomatik ekleme
- ✅ **Engagement botları** (like, retweet, comment)
- ✅ **Analytics tracking** (reach, engagement, growth)

## 📋 Kurulum

### 1. Gerekli Kütüphaneler
```bash
pip install -r requirements.txt
```

### 2. API Keys Setup
```bash
# .env dosyasını düzenle
cp .env.example .env
nano .env
```

### 3. Bot'u Başlat
```bash
python3 automated_social_media_system.py
```

### 4. Service Olarak Çalıştır
```bash
sudo cp usdtgverse-social-bot.service /etc/systemd/system/
sudo systemctl enable usdtgverse-social-bot
sudo systemctl start usdtgverse-social-bot
```

## 📊 Monitoring

```bash
# Bot sağlığını kontrol et
python3 monitor_bot.py

# Logları görüntüle
tail -f logs/social_bot.log
```

## ⚙️ Konfigürasyon

`config.py` dosyasını düzenleyerek:
- Post sıklığını ayarlayabilirsiniz
- İçerik türlerini değiştirebilirsiniz
- Hashtag'leri güncelleyebilirsiniz
- Platform ayarlarını yapabilirsiniz

## 📈 Analytics

Bot otomatik olarak analytics verilerini toplar:
- Toplam post sayısı
- Reach ve engagement
- Platform bazlı istatistikler
- Günlük raporlar

## 🛠️ Troubleshooting

### Bot çalışmıyor
```bash
# Logları kontrol et
tail -f logs/social_bot.log

# Service durumunu kontrol et
sudo systemctl status usdtgverse-social-bot
```

### API Limitleri
- Twitter: Günde 300 tweet
- LinkedIn: Günde 150 post
- Instagram: Günde 25 post

## 📞 Destek

Sorunlar için: support@usdtgverse.com

---

**USDTgVerse - Quantum-Safe Blockchain Ecosystem** 🚀
"""
    
    with open("README_SOCIAL_BOT.md", "w") as f:
        f.write(readme_content)
    
    print("✅ Created README_SOCIAL_BOT.md")

def main():
    """Ana kurulum fonksiyonu"""
    print_banner()
    
    print("🚀 USDTgVerse Social Media Bot Setup Starting...")
    
    # Kurulum adımları
    steps = [
        ("Python Version Check", check_python_version),
        ("Install Requirements", install_requirements),
        ("Create Directories", create_directories),
        ("Create Config File", create_config_file),
        ("Create Environment File", create_env_file),
        ("Create Startup Script", create_startup_script),
        ("Create Systemd Service", create_systemd_service),
        ("Create Monitoring Script", create_monitoring_script),
        ("Create README", create_readme)
    ]
    
    for step_name, step_func in steps:
        print(f"\n🔄 {step_name}...")
        try:
            if step_func():
                print(f"✅ {step_name} completed")
            else:
                print(f"❌ {step_name} failed")
        except Exception as e:
            print(f"❌ {step_name} failed: {e}")
    
    print("\n🎉 Setup completed!")
    print("\n📋 Next Steps:")
    print("1. Update .env file with your API credentials")
    print("2. Update config.py with your settings")
    print("3. Run: python3 automated_social_media_system.py")
    print("4. Or run: ./start_bot.sh")
    
    print("\n⚠️  Important Notes:")
    print("- API keys'leri güvenli tutun")
    print("- Bot'u test etmeden önce küçük hesaplarla deneyin")
    print("- Rate limit'lere dikkat edin")
    print("- Analytics verilerini düzenli kontrol edin")

if __name__ == "__main__":
    main()
