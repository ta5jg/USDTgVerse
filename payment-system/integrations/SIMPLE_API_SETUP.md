# 🚀 USDTgVerse Simple API Setup

## Bu akşam API anahtarlarını alalım!

### 1. Stripe API Anahtarları (Ödeme İşleme)

**Adımlar:**
1. https://stripe.com adresine git
2. "Start now" butonuna tıkla
3. İş hesabı oluştur
4. İş doğrulamasını tamamla
5. https://dashboard.stripe.com/apikeys adresine git
6. API anahtarlarını kopyala

**Anahtarlar:**
- Publishable key: `pk_live_51...`
- Secret key: `sk_live_51...`

### 2. Plaid API Anahtarları (Banka Verisi)

**Adımlar:**
1. https://plaid.com adresine git
2. "Get started" butonuna tıkla
3. Developer hesabı oluştur
4. İş doğrulamasını tamamla
5. https://dashboard.plaid.com/team/keys adresine git
6. API anahtarlarını kopyala

**Anahtarlar:**
- Client ID: `your_client_id`
- Secret Key: `your_secret_key`

### 3. API Anahtarlarını Koda Ekleme

**Stripe için:**
```bash
cd /Users/irfangedik/USDTgVerse/payment-system/integrations
nano stripe_real_live.c
```

Bu satırları bul ve değiştir:
```c
#define STRIPE_PUBLISHABLE_KEY "pk_live_51..." // Gerçek anahtarın
#define STRIPE_SECRET_KEY "sk_live_51..." // Gerçek anahtarın
```

**Plaid için:**
```bash
nano plaid_real_live.c
```

Bu satırları bul ve değiştir:
```c
#define PLAID_CLIENT_ID "your_client_id" // Gerçek client ID
#define PLAID_SECRET_KEY "your_secret_key" // Gerçek secret key
```

### 4. Test Etme

**Stripe testi:**
```bash
gcc -Wall -Wextra -std=c99 -O2 -o stripe_real_live stripe_real_live.c -lcurl
./stripe_real_live
```

**Plaid testi:**
```bash
gcc -Wall -Wextra -std=c99 -O2 -o plaid_real_live plaid_real_live.c -lcurl
./plaid_real_live
```

### 5. Çalışan Entegrasyonlar

**CoinGecko (Zaten çalışıyor):**
```bash
./coingecko_live_integration
```

**Binance (Zaten çalışıyor):**
```bash
./binance_live_integration
```

## 🎯 Bu Akşam Yapılacaklar

1. ✅ Stripe hesabı oluştur
2. ✅ Plaid hesabı oluştur
3. ✅ API anahtarlarını al
4. ✅ Koda ekle
5. ✅ Test et
6. ✅ USDTgVerse sistemine entegre et

## 💡 İpuçları

- **Stripe:** İş doğrulaması gerekebilir
- **Plaid:** Ücretsiz tier mevcut
- **CoinGecko + Binance:** Zaten çalışıyor
- **Test:** Her API'yi ayrı ayrı test et
- **Güvenlik:** API anahtarlarını asla paylaşma

## 🚨 Önemli Notlar

- API anahtarları hassas bilgilerdir
- Sadece güvenilir ortamlarda kullan
- Rate limitlere dikkat et
- Maliyetleri takip et

## 📞 Destek

- **Stripe:** https://support.stripe.com
- **Plaid:** https://support.plaid.com
- **CoinGecko:** https://www.coingecko.com/en/api
- **Binance:** https://www.binance.com/en/support

---

**Bu akşam API anahtarlarını alıp sistemi tamamen çalışır hale getirelim! 🚀**
