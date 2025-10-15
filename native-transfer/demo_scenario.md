# 🌟 USDTgVerse Native Coin Transfer Demo Senaryosu

## 📋 Senaryo: "Yeni Kullanıcı Hoşgeldin Paketi"

### 🎯 Hedef
Yeni kayıt olan kullanıcılara **10 USDTg** airdrop göndererek sistemi tanıtmak.

### 📱 Adımlar

#### 1️⃣ **Yeni Cüzdan Oluştur**
```bash
curl -X POST http://localhost:3002/api/v1/native/wallet/create \
  -H "Content-Type: application/json" \
  -d '{
    "address": "usdtg1yeni_kullanici123"
  }'
```

**Beklenen Response:**
```json
{
  "address": "usdtg1yeni_kullanici123",
  "status": "success",
  "message": "Wallet created successfully",
  "initial_balance": "0.00000000",
  "created_at": 1704216789
}
```

#### 2️⃣ **Balance Kontrol Et**
```bash
curl http://localhost:3002/api/v1/native/balance/usdtg1yeni_kullanici123/USDTg
```

**Beklenen Response:**
```json
{
  "address": "usdtg1yeni_kullanici123",
  "coin": "USDTg",
  "balance": "0.00000000",
  "status": "success",
  "timestamp": 1704216789
}
```

#### 3️⃣ **Hoşgeldin Airdrop Gönder**
```bash
curl -X POST http://localhost:3002/api/v1/native/airdrop \
  -H "Content-Type: application/json" \
  -d '{
    "to_address": "usdtg1yeni_kullanici123",
    "coin": "USDTg",
    "amount": 10.0,
    "reason": "Hoşgeldin Bonus"
  }'
```

**Beklenen Response:**
```json
{
  "to_address": "usdtg1yeni_kullanici123",
  "coin": "USDTg",
  "amount": "10.00000000",
  "reason": "Hoşgeldin Bonus",
  "status": "success",
  "message": "Airdrop sent successfully",
  "timestamp": 1704216789
}
```

#### 4️⃣ **Balance Tekrar Kontrol Et**
```bash
curl http://localhost:3002/api/v1/native/balance/usdtg1yeni_kullanici123/USDTg
```

**Beklenen Response:**
```json
{
  "address": "usdtg1yeni_kullanici123",
  "coin": "USDTg",
  "balance": "10.00000000",
  "status": "success",
  "timestamp": 1704216789
}
```

#### 5️⃣ **Başka Bir Cüzdana Transfer Et**
```bash
curl -X POST http://localhost:3002/api/v1/native/transfer \
  -H "Content-Type: application/json" \
  -d '{
    "from_address": "usdtg1yeni_kullanici123",
    "to_address": "usdtg1arkadas456",
    "coin": "USDTg",
    "amount": 3.5,
    "memo": "Arkadaşıma ödünç"
  }'
```

#### 6️⃣ **Transaction History Kontrol Et**
```bash
curl http://localhost:3002/api/v1/native/transactions/usdtg1yeni_kullanici123
```

---

## 🚀 Merchant Payment Senaryosu

### 🛒 Senaryo: "E-Ticaret Ödemesi"

#### 1️⃣ **Merchant Cüzdan Oluştur**
```bash
curl -X POST http://localhost:3002/api/v1/native/wallet/create \
  -H "Content-Type: application/json" \
  -d '{"address": "usdtg1merchant_magaza789"}'
```

#### 2️⃣ **Customer Alışveriş Yapar**
```bash
curl -X POST http://localhost:3002/api/v1/native/transfer \
  -H "Content-Type: application/json" \
  -d '{
    "from_address": "usdtg1yeni_kullanici123",
    "to_address": "usdtg1merchant_magaza789",
    "coin": "USDTg",
    "amount": 25.99,
    "memo": "Online alışveriş ödemesi #12345"
  }'
```

---

## 💰 Bulk Airdrop Senaryosu

### 🎁 Senaryo: "Aktivasyon Kampanyası"

```bash
# 100 kullanıcıya tek seferde airdrop (bash script)
for i in {1..100}; do
  curl -X POST http://localhost:3002/api/v1/native/airdrop \
    -H "Content-Type: application/json" \
    -d "{
      \"to_address\": \"usdtg1kampanya_kullanici${i}\",
      \"coin\": \"USDTg\",
      \"amount\": 5.0,
      \"reason\": \"Kampanya Aktivasyon Ödülü\"
    }" &
done
wait
echo "🎁 100 airdrop tamamlandı!"
```

---

## 📊 Trading Senaryosu

### 📈 Senaryo: "Coin Takası"

#### 1️⃣ **USDTgV Sahibi Ol**
```bash
curl -X POST http://localhost:3002/api/v1/native/wallet/create \
  -H "Content-Type: application/json" \
  -d '{"address": "usdtg1trader_pro999"}'
  
curl -X POST http://localhost:3002/api/v1/native/airdrop \
  -H "Content-Type: application/json" \
  -d '{
    "to_address": "usdtg1trader_pro999",
    "coin": "USDTgV",
    "amount": 1000.0,
    "reason": "Trading demo"
  }'
```

#### 2️⃣ **Governance Token Al**
```bash
curl -X POST http://localhost:3002/api/v1/native/wallet/create \
  -H "Content-Type: application/json" \
  -d '{"address": "usdtg1governance_lover777"}'
  
curl -X POST http://localhost:3002/api/v1/native/airdrop \
  -H "Content-Type: application/json" \
  -d '{
    "to_address": "usdtg1governance_lover777",
    "coin": "USDTgG",
    "amount": 100.0,
    "reason": "Governance demo"
  }'
```

---

## 🔧 Sistem Monitorü

### 📈 API Stats Kontrolü
```bash
curl http://localhost:3002/api/v1/native/stats
```

**Response:**
```json
{
  "service": "USDTgVerse Native Transfer API",
  "version": "3.0.0",
  "status": "running",
  "native_coins": ["USDTg", "USDTgV", "USDTgG"],
  "features": ["zero_fee", "fast_transfer", "atomic_operations"],
  "timestamp": 1704216789
}
```

---

## 🎯 Özet

**✅ Bu sistemle yapabilecekleriniz:**
- ✅ Cüzdan oluşturma (sınırsız)
- ✅ Native coin transfer etme (USDTg, USDTgV, USDTgG)
- ✅ Airdrop gönderme (kampanyalar için)
- ✅ Merchant ödemeleri 
- ✅ Zero fee transactions
- ✅ Real-time balance tracking
- ✅ Transaction history
- ✅ Atomic operations (güvenli)

**🚀 Kardeşim bu sistem tamamen hazır!** Artık mobil cüzdanlarından ve web platformundan bu API'leri kullanarak native coinlerimizi transfer edebilirsin!
