# 🌟 USDTgVerse Native Transfer System LIVE DEPLOYMENT GUIDE

## 🚀 DEPLOYMENT STATUS
**Local Development:** ✅ COMPLETED  
**Live Deployment:** 🔄 IN PROGRESS  

## 📦 DEPLOYMENT PACKAGE READY
File: `native-transfer-live.tar.gz` (9.3 KB)  
Contains: 
- `native_coin_transfer_system.c` - Core transfer logic
- `transfer_api_server.c` - REST API server  
- `start_native_transfer_system.sh` - Startup script

## 🎯 DEPLOYMENT TARGET
**Server:** SFO2 Backend (164.92.146.221)  
**Service:** Native Transfer API  
**Port:** 3002  
**Path:** `/opt/usdtgverse/native-transfer/`

## 📋 MANUAL DEPLOYMENT COMMANDS

### Step 1: Upload Package
```bash
scp native-transfer-live.tar.gz root@164.92.146.221:/opt/usdtgverse/
```

### Step 2: SSH Setup
```bash
ssh root@164.92.146.221
cd /opt/usdtgverse
mkdir -p native-transfer
tar -xzf native-transfer-live.tar.gz -C native-transfer/
cd native-transfer
```

### Step 3: Compile
```bash
gcc -O3 -o native_transfer_api transfer_api_server.c native_coin_transfer_system.c -lssl -lcrypto -lm -pthread
chmod +x native_transfer_api
```

### Step 4: Create Systemd Service
```bash
cat > /etc/systemd/system/native-transfer-api.service << 'EOF'
[Unit]
Description=USDTgVerse Native Transfer API Server
After=network.target
[Service]
Type=simple
User=root
WorkingDirectory=/opt/usdtgverse/native-transfer
ExecStart=/opt/usdtgverse/native-transfer/native_transfer_api
Restart=always
RestartSec=5
Environment=TZ=UTC
[Install]
WantedBy=multi-user.target
EOF

systemctl daemon-reload
systemctl enable native-transfer-api
systemctl start native-transfer-api
```

### Step 5: Test
```bash
curl http://localhost:3002/api/v1/native/stats
```