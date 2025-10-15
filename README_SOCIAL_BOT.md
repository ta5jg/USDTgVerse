# 🤖 USDTgVerse Social Media Bot

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
