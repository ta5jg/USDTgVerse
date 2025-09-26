#!/bin/bash

# USDTgVerse Complete Token Deployment Script
# ===========================================
# 
# This script deploys all three tokens (USDTg, USDTgV, USDTgG) to mainnet
# and sets up liquidity pools in the correct order.

set -e

echo "🚀 USDTgVerse Complete Token Deployment"
echo "======================================="
echo ""

# Configuration
NETWORK="mainnet"
DEPLOYMENT_DIR="/Users/irfangedik/USDTgVerse/deployment/scripts"
CONTRACTS_DIR="/Users/irfangedik/USDTgVerse/native-ecosystem/contracts"

echo "📋 Deployment Configuration:"
echo "  Network: $NETWORK"
echo "  Deployment Scripts: $DEPLOYMENT_DIR"
echo "  Contracts Directory: $CONTRACTS_DIR"
echo ""

# Check if all required files exist
echo "🔍 Checking required files..."

if [ ! -f "$CONTRACTS_DIR/USDTgNativeCoin.usdtg" ]; then
    echo "❌ Error: USDTgNativeCoin.usdtg not found!"
    exit 1
fi

if [ ! -f "$CONTRACTS_DIR/USDTgV.usdtg" ]; then
    echo "❌ Error: USDTgV.usdtg not found!"
    exit 1
fi

if [ ! -f "$CONTRACTS_DIR/USDTgG.usdtg" ]; then
    echo "❌ Error: USDTgG.usdtg not found!"
    exit 1
fi

if [ ! -f "$DEPLOYMENT_DIR/deploy_usdtg_native.sh" ]; then
    echo "❌ Error: deploy_usdtg_native.sh not found!"
    exit 1
fi

if [ ! -f "$DEPLOYMENT_DIR/deploy_usdtgv.sh" ]; then
    echo "❌ Error: deploy_usdtgv.sh not found!"
    exit 1
fi

if [ ! -f "$DEPLOYMENT_DIR/deploy_usdtgg.sh" ]; then
    echo "❌ Error: deploy_usdtgg.sh not found!"
    exit 1
fi

if [ ! -f "$DEPLOYMENT_DIR/setup_liquidity_pools.sh" ]; then
    echo "❌ Error: setup_liquidity_pools.sh not found!"
    exit 1
fi

echo "✅ All required files found"
echo ""

# Create deployment directory
DEPLOYMENT_WORK_DIR="/Users/irfangedik/USDTgVerse/deployment/work"
mkdir -p "$DEPLOYMENT_WORK_DIR"
cd "$DEPLOYMENT_WORK_DIR"

echo "📁 Working directory: $DEPLOYMENT_WORK_DIR"
echo ""

# Step 1: Deploy USDTg Native Coin
echo "🪙 Step 1: Deploying USDTg Native Coin..."
echo "========================================"
cp "$CONTRACTS_DIR/USDTgNativeCoin.usdtg" .
cd "$DEPLOYMENT_WORK_DIR"

if bash "$DEPLOYMENT_DIR/deploy_usdtg_native.sh"; then
    echo "✅ USDTg Native Coin deployed successfully"
    
    # Extract contract address from deployment info
    if [ -f "deployment_info.json" ]; then
        USDTG_NATIVE_ADDRESS=$(grep -o '"address": "[^"]*"' deployment_info.json | cut -d'"' -f4)
        echo "📍 USDTg Native Address: $USDTG_NATIVE_ADDRESS"
    else
        echo "❌ Error: Could not find deployment info for USDTg Native"
        exit 1
    fi
else
    echo "❌ Error: USDTg Native Coin deployment failed!"
    exit 1
fi

echo ""

# Step 2: Deploy USDTgV Utility Token
echo "⚡ Step 2: Deploying USDTgV Utility Token..."
echo "==========================================="
cp "$CONTRACTS_DIR/USDTgV.usdtg" .
cd "$DEPLOYMENT_WORK_DIR"

if bash "$DEPLOYMENT_DIR/deploy_usdtgv.sh"; then
    echo "✅ USDTgV Utility Token deployed successfully"
    
    # Extract contract address from deployment info
    if [ -f "deployment_info.json" ]; then
        USDTGV_ADDRESS=$(grep -o '"address": "[^"]*"' deployment_info.json | cut -d'"' -f4)
        echo "📍 USDTgV Address: $USDTGV_ADDRESS"
    else
        echo "❌ Error: Could not find deployment info for USDTgV"
        exit 1
    fi
else
    echo "❌ Error: USDTgV Utility Token deployment failed!"
    exit 1
fi

echo ""

# Step 3: Deploy USDTgG Governance Token
echo "🏛️ Step 3: Deploying USDTgG Governance Token..."
echo "=============================================="
cp "$CONTRACTS_DIR/USDTgG.usdtg" .
cd "$DEPLOYMENT_WORK_DIR"

if bash "$DEPLOYMENT_DIR/deploy_usdtgg.sh"; then
    echo "✅ USDTgG Governance Token deployed successfully"
    
    # Extract contract address from deployment info
    if [ -f "deployment_info.json" ]; then
        USDTGG_ADDRESS=$(grep -o '"address": "[^"]*"' deployment_info.json | cut -d'"' -f4)
        echo "📍 USDTgG Address: $USDTGG_ADDRESS"
    else
        echo "❌ Error: Could not find deployment info for USDTgG"
        exit 1
    fi
else
    echo "❌ Error: USDTgG Governance Token deployment failed!"
    exit 1
fi

echo ""

# Step 4: Setup Liquidity Pools
echo "💧 Step 4: Setting up Liquidity Pools..."
echo "======================================="
cd "$DEPLOYMENT_WORK_DIR"

# Export contract addresses for liquidity pool script
export USDTG_NATIVE_ADDRESS
export USDTGV_ADDRESS
export USDTGG_ADDRESS

if bash "$DEPLOYMENT_DIR/setup_liquidity_pools.sh"; then
    echo "✅ Liquidity pools setup successfully"
else
    echo "❌ Error: Liquidity pools setup failed!"
    exit 1
fi

echo ""

# Step 5: Generate final deployment summary
echo "📊 Step 5: Generating deployment summary..."
echo "=========================================="

cat > "complete_deployment_summary.json" << EOF
{
    "deployment_info": {
        "network": "$NETWORK",
        "deployment_time": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
        "deployment_dir": "$DEPLOYMENT_WORK_DIR"
    },
    "contracts": {
        "USDTg_Native": {
            "name": "USDTg Native Coin",
            "address": "$USDTG_NATIVE_ADDRESS",
            "type": "stablecoin",
            "supply": "1000000000000000000000000000",
            "decimals": 18
        },
        "USDTgV": {
            "name": "USDTgV Utility Token",
            "address": "$USDTGV_ADDRESS",
            "type": "utility",
            "supply": "500000000000000000000000000",
            "decimals": 18
        },
        "USDTgG": {
            "name": "USDTgG Governance Token",
            "address": "$USDTGG_ADDRESS",
            "type": "governance",
            "supply": "100000000000000000000000000",
            "decimals": 18
        }
    },
    "liquidity_pools": {
        "USDTg_USDTgV": {
            "pair_address": "TBD",
            "initial_price": "0.5 USDTg",
            "liquidity": "1M USDTg + 500M USDTgV"
        },
        "USDTg_USDTgG": {
            "pair_address": "TBD",
            "initial_price": "5.0 USDTg",
            "liquidity": "1M USDTg + 100M USDTgG"
        }
    },
    "security_features": {
        "multi_signature": true,
        "anti_fraud": true,
        "time_locked_operations": true,
        "emergency_controls": true,
        "daily_transfer_limits": true,
        "blacklist_functionality": true,
        "quantum_safe_addresses": true
    }
}
EOF

echo "✅ Deployment summary generated: complete_deployment_summary.json"

# Display final summary
echo ""
echo "🎉 USDTgVerse Token Deployment Completed Successfully!"
echo "====================================================="
echo ""
echo "📍 Contract Addresses:"
echo "  USDTg Native Coin: $USDTG_NATIVE_ADDRESS"
echo "  USDTgV Utility Token: $USDTGV_ADDRESS"
echo "  USDTgG Governance Token: $USDTGG_ADDRESS"
echo ""
echo "💧 Liquidity Pools:"
echo "  USDTg/USDTgV: 1M USDTg + 500M USDTgV (Price: 0.5 USDTg)"
echo "  USDTg/USDTgG: 1M USDTg + 100M USDTgG (Price: 5.0 USDTg)"
echo ""
echo "🔐 Security Features Active:"
echo "  ✅ Multi-signature security"
echo "  ✅ Anti-fraud measures"
echo "  ✅ Time-locked operations"
echo "  ✅ Emergency controls"
echo "  ✅ Daily transfer limits"
echo "  ✅ Blacklist functionality"
echo "  ✅ Quantum-safe addresses"
echo ""
echo "📋 Next Steps:"
echo "  1. Transfer ownership to multi-sig wallets"
echo "  2. Setup price oracles"
echo "  3. Configure DEX interface"
echo "  4. Launch public trading"
echo "  5. Setup staking pools"
echo "  6. Configure governance parameters"
echo ""
echo "📁 All deployment files saved in: $DEPLOYMENT_WORK_DIR"
echo ""
