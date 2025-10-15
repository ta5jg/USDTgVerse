# USDTgVerse Airdrop System

## 🎁 Overview

Secure airdrop system that distributes **10 USDTg** to every new wallet with intelligent locking and anti-abuse protection.

## 🔒 Security Features

### **Locked Airdrop System**
- **10 USDTg** automatically sent to new wallets
- **LOCKED** - Can only be used for transaction fees
- **Unlocked** after user purchases 50-100 USDTg
- Prevents system abuse and ensures real user engagement

### **Anti-Fraud Protection**
- Device fingerprint tracking
- IP address monitoring
- One airdrop per verified identity
- Daily limit: 10,000 airdrops
- Automatic fraud detection

## 💰 Token Economics

### **Airdrop Distribution**
```
Total Supply: 1,000,000,000 USDTg
Public Distribution: 300,000,000 USDTg (30%)
Airdrop Allocation: 10 USDTg per wallet
Maximum Users: 30,000,000 (30 Million)
```

### **Unlock Requirements**
- **Minimum Purchase**: 50 USDTg
- **Recommended Purchase**: 100 USDTg
- **Locked Usage**: Transaction fees only
- **Unlocked Usage**: All transactions

## 🚀 System Architecture

### **Backend API Server** (Pure C)
```
Port: 3006
Language: Pure C
Database: File-based (CSV)
Performance: Sub-millisecond response
```

### **API Endpoints**
```
POST /api/airdrop/create          - Create new airdrop
POST /api/airdrop/use-fee         - Use airdrop for fee
POST /api/airdrop/verify-purchase - Verify purchase
GET  /api/airdrop/status          - Get airdrop status
GET  /api/airdrop/stats           - Get system statistics
```

## 📱 Platform Integration

### **iOS Wallet** (Swift)
- File: `mobile/ios/USDTgWallet/USDTgWallet/RealWalletManager.swift`
- Functions:
  - `requestWelcomeAirDrop()` - Request airdrop
  - `verifyPurchaseAndUnlockAirdrop()` - Verify purchase
  - `payFeeWithAirdrop()` - Pay fee with airdrop

### **Android Wallet** (Kotlin)
- File: `mobile/android/USDTgWallet/app/src/main/java/com/usdtgverse/wallet/data/AirdropManager.kt`
- Functions:
  - `requestAirdrop()` - Request airdrop
  - `verifyPurchaseAndUnlock()` - Verify purchase
  - `payFeeWithAirdrop()` - Pay fee with airdrop

### **Web Wallet** (JavaScript)
- File: `wallet/airdrop-manager.js`
- Class: `AirdropManager`
- Functions:
  - `requestAirdrop()` - Request airdrop
  - `verifyPurchaseAndUnlock()` - Verify purchase
  - `payFeeWithAirdrop()` - Pay fee with airdrop

### **Browser Extensions** (JavaScript)
- File: `browser-extensions/shared/airdrop-manager.js`
- Class: `BrowserAirdropManager`
- Cross-browser compatible (Chrome, Safari, Firefox)

## 🔄 User Flow

### **1. New Wallet Creation**
```
User creates wallet
    ↓
System generates wallet address
    ↓
Request 10 USDTg airdrop
    ↓
Airdrop sent (LOCKED - Fee only)
    ↓
User notified
```

### **2. Using Locked Airdrop**
```
User initiates transaction
    ↓
Transaction fee calculated
    ↓
Fee paid from locked airdrop
    ↓
Remaining balance updated
```

### **3. Unlocking Airdrop**
```
User purchases 50+ USDTg
    ↓
System verifies purchase
    ↓
Purchase threshold met
    ↓
Airdrop unlocked
    ↓
Full access granted
```

## 📊 Database Structure

### **Airdrop Record**
```csv
airdrop_id,wallet_address,user_id,airdrop_amount,locked_amount,used_amount,purchase_amount,status,created_at,unlocked_at,expires_at,last_used_at,device_fingerprint,ip_address,fee_usage_count,kyc_verified
```

### **Status Types**
- `LOCKED` - Airdrop locked (fee only)
- `UNLOCKED` - Airdrop unlocked (full access)
- `USED` - Airdrop fully used
- `EXPIRED` - Airdrop expired (1 year)
- `REVOKED` - Airdrop revoked (fraud)

## 🛡️ Security Measures

### **Fraud Detection**
- Maximum 3 airdrops per device
- Maximum 5 airdrops per IP address
- Device fingerprint tracking
- IP address monitoring
- Automatic revocation on fraud

### **Daily Limits**
- Maximum 10,000 airdrops per day
- Automatic reset at midnight UTC
- Protection against system abuse

### **Expiration**
- Airdrop expires after 365 days
- Unused airdrops returned to treasury
- Automatic cleanup system

## 🔧 Installation & Setup

### **1. Compile Backend**
```bash
cd /Users/irfangedik/USDTgVerse/airdrop-system
make clean && make
```

### **2. Start API Server**
```bash
./airdrop_api_server &
```

### **3. Test Airdrop**
```bash
curl -X POST http://localhost:3006/api/airdrop/create \
  -H "Content-Type: application/json" \
  -d '{
    "wallet_address": "USDTg_TEST_001",
    "user_id": "USER_001",
    "device_fingerprint": "TEST_DEVICE",
    "ip_address": "127.0.0.1"
  }'
```

## 📈 Statistics

### **System Metrics**
- Total airdrops sent
- Locked airdrops
- Unlocked airdrops
- Expired airdrops
- Revoked airdrops (fraud)
- Total value distributed
- Total used for fees
- Airdrops today

### **Get Statistics**
```bash
curl http://localhost:3006/api/airdrop/stats
```

## 🎯 Benefits

### **For Users**
- ✅ Free 10 USDTg to start
- ✅ No upfront investment required
- ✅ Pay transaction fees immediately
- ✅ Unlock with first purchase
- ✅ Anti-fraud protection

### **For System**
- ✅ Prevents abuse
- ✅ Ensures real users
- ✅ Encourages purchases
- ✅ Sustainable distribution
- ✅ Fraud detection

## 🔐 Best Practices

### **For Users**
1. Create wallet and receive 10 USDTg
2. Use locked airdrop for transaction fees
3. Purchase 50+ USDTg to unlock
4. Enjoy full access to airdrop

### **For Developers**
1. Always verify airdrop status before transactions
2. Use locked airdrop for fees only
3. Verify purchases to unlock airdrop
4. Implement fraud detection
5. Monitor daily limits

## 📝 License

MIT License - See LICENSE file for details

---

**Created by**: USDTG GROUP TECHNOLOGY LLC  
**Version**: 1.0  
**Date**: 2025-10-13

