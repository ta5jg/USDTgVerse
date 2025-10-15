# 🚀 USDTgVerse Social Media Bot - QUICK START GUIDE

## ⚡ **HEMEN BAŞLATMAK İÇİN:**

### **1. Test Modu (API Keys Olmadan)**
```bash
python3 test_social_bot.py
```
- ✅ Simülasyon modu
- ✅ Gerçek post yapmaz
- ✅ Analytics gösterir
- ✅ 24 saatlik test

### **2. Production Modu (API Keys Gerekli)**
```bash
python3 start_social_bot.py
```
- ⚠️ Gerçek post yapar
- ⚠️ API keys gerekli
- ✅ Menü sistemi
- ✅ Analytics tracking

## 🔑 **API KEYS NASIL ALINIR:**

### **Twitter API Keys:**
1. https://developer.twitter.com/ adresine git
2. "Apply for a developer account" tıkla
3. "Create an app" seçeneğini seç
4. API keys'leri kopyala
5. `.env` dosyasına ekle

### **LinkedIn:**
1. LinkedIn hesabı oluştur
2. Email/password'i `.env` dosyasına ekle
3. Bot LinkedIn'e giriş yapacak

### **Instagram:**
1. Instagram hesabı oluştur (@usdtgverse)
2. Username/password'i `.env` dosyasına ekle
3. Bot Instagram'a giriş yapacak

## 📝 **KONFIGÜRASYON:**

### **.env Dosyası Oluştur:**
```bash
cp env_example.txt .env
nano .env
```

### **config.py Düzenle:**
```bash
nano config.py
```

## 🎯 **ÖZELLİKLER:**

### **Otomatik Posting:**
- ✅ Random sıklık (2-8 saat)
- ✅ Çeşitli içerik türleri
- ✅ Trend hashtag'ler
- ✅ Multi-platform support

### **Content Types:**
- 🔧 Technical posts
- 📰 News updates
- 📚 Educational content
- 👥 Community posts
- 🎁 Promotional content

### **Analytics:**
- 📊 Post statistics
- 📈 Reach tracking
- 💬 Engagement metrics
- 📅 Daily reports

## 🚨 **ÖNEMLİ NOTLAR:**

### **Rate Limits:**
- Twitter: 300 tweet/day
- LinkedIn: 150 post/day
- Instagram: 25 post/day

### **Güvenlik:**
- API keys'leri güvenli tut
- `.env` dosyasını git'e commit etme
- Test modunda başla

### **Monitoring:**
```bash
# Bot sağlığını kontrol et
python3 monitor_bot.py

# Analytics görüntüle
cat analytics/social_media_analytics.json
```

## 🎉 **BAŞLATMA:**

### **Hızlı Test:**
```bash
python3 test_social_bot.py
```

### **Production:**
```bash
python3 start_social_bot.py
```

### **Service Olarak:**
```bash
sudo cp usdtgverse-social-bot.service /etc/systemd/system/
sudo systemctl enable usdtgverse-social-bot
sudo systemctl start usdtgverse-social-bot
```

---

## 📞 **DESTEK:**

- **Documentation:** README_SOCIAL_BOT.md
- **Issues:** GitHub issues
- **Email:** support@usdtgverse.com

**USDTgVerse - Quantum-Safe Blockchain Ecosystem** 🚀
