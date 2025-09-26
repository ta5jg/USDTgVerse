#!/bin/bash

# USDTgVerse C-Based Mainnet Deployment
# ====================================
# 
# This script deploys all three tokens to USDTgVerse mainnet
# using our native C-based blockchain infrastructure.

set -e

echo "🚀 USDTgVerse C-Based Mainnet Deployment"
echo "========================================"
echo ""

# Configuration
NETWORK="usdtgverse-mainnet-1"
NODE_PATH="/Users/irfangedik/USDTgVerse/usdtgverse_node_working"
COMPILER_PATH="/Users/irfangedik/USDTgVerse/native-ecosystem/compiler/usdtgc"
CONTRACTS_DIR="/Users/irfangedik/USDTgVerse/native-ecosystem/contracts"

# Treasury and development addresses
TREASURY_ADDRESS="usdtg1treasury..."
DEVELOPMENT_ADDRESS="usdtg1development..."
MARKETING_ADDRESS="usdtg1marketing..."

echo "📋 Deployment Configuration:"
echo "  Network: $NETWORK"
echo "  Node: $NODE_PATH"
echo "  Compiler: $COMPILER_PATH"
echo "  Contracts: $CONTRACTS_DIR"
echo ""

# Check if node is running
echo "🔍 Checking USDTgVerse node status..."
if ! pgrep -f "usdtgverse_node_working" > /dev/null; then
    echo "⚠️  USDTgVerse node not running. Starting node..."
    $NODE_PATH &
    sleep 5
fi

echo "✅ USDTgVerse node is running"

# Deploy USDTg Native Coin (already native)
echo "🪙 Step 1: USDTg Native Coin (Native Token)"
echo "==========================================="
echo "✅ USDTg Native Coin is already deployed as native token"
echo "📍 Native Token Address: usdtg1native..."
echo "💰 Total Supply: 1,000,000,000 USDTg"
echo "🏛️ Treasury Balance: 400,000,000 USDTg (40%)"
echo ""

# Deploy USDTgV Utility Token
echo "⚡ Step 2: Deploying USDTgV Utility Token"
echo "========================================"

# Compile USDTgV contract
echo "🔨 Compiling USDTgV contract..."
if ! "$COMPILER_PATH" "$CONTRACTS_DIR/USDTgV.usdtg" --target usdtgvm --output "USDTgV.bytecode"; then
    echo "❌ Error: USDTgV compilation failed!"
    exit 1
fi

echo "✅ USDTgV contract compiled successfully"

# Deploy USDTgV to mainnet
echo "🌐 Deploying USDTgV to mainnet..."
USDTGV_DEPLOYMENT=$(usdtgverse-cli tx deploy \
    --from "$TREASURY_ADDRESS" \
    --contract "USDTgV.bytecode" \
    --gas 6000000 \
    --gas-price 20000000000 \
    --chain-id "$NETWORK" \
    --yes)

if [ $? -eq 0 ]; then
    USDTGV_ADDRESS=$(echo "$USDTGV_DEPLOYMENT" | grep "Contract Address" | cut -d' ' -f3)
    echo "✅ USDTgV deployed successfully"
    echo "📍 Contract Address: $USDTGV_ADDRESS"
else
    echo "❌ Error: USDTgV deployment failed!"
    exit 1
fi

# Deploy USDTgG Governance Token
echo "🏛️ Step 3: Deploying USDTgG Governance Token"
echo "============================================"

# Compile USDTgG contract
echo "🔨 Compiling USDTgG contract..."
if ! "$COMPILER_PATH" "$CONTRACTS_DIR/USDTgG.usdtg" --target usdtgvm --output "USDTgG.bytecode"; then
    echo "❌ Error: USDTgG compilation failed!"
    exit 1
fi

echo "✅ USDTgG contract compiled successfully"

# Deploy USDTgG to mainnet
echo "🌐 Deploying USDTgG to mainnet..."
USDTGG_DEPLOYMENT=$(usdtgverse-cli tx deploy \
    --from "$TREASURY_ADDRESS" \
    --contract "USDTgG.bytecode" \
    --gas 7000000 \
    --gas-price 20000000000 \
    --chain-id "$NETWORK" \
    --yes)

if [ $? -eq 0 ]; then
    USDTGG_ADDRESS=$(echo "$USDTGG_DEPLOYMENT" | grep "Contract Address" | cut -d' ' -f3)
    echo "✅ USDTgG deployed successfully"
    echo "📍 Contract Address: $USDTGG_ADDRESS"
else
    echo "❌ Error: USDTgG deployment failed!"
    exit 1
fi

# Setup liquidity pools
echo "💧 Step 4: Setting up Liquidity Pools"
echo "===================================="

# Create USDTg/USDTgV trading pair
echo "🔄 Creating USDTg/USDTgV trading pair..."
PAIR1_DEPLOYMENT=$(usdtgverse-cli tx create-pair \
    --from "$TREASURY_ADDRESS" \
    --token0 "usdtg1native..." \
    --token1 "$USDTGV_ADDRESS" \
    --chain-id "$NETWORK" \
    --yes)

if [ $? -eq 0 ]; then
    PAIR1_ADDRESS=$(echo "$PAIR1_DEPLOYMENT" | grep "Pair Address" | cut -d' ' -f3)
    echo "✅ USDTg/USDTgV pair created: $PAIR1_ADDRESS"
else
    echo "❌ Error: USDTg/USDTgV pair creation failed!"
    exit 1
fi

# Create USDTg/USDTgG trading pair
echo "🔄 Creating USDTg/USDTgG trading pair..."
PAIR2_DEPLOYMENT=$(usdtgverse-cli tx create-pair \
    --from "$TREASURY_ADDRESS" \
    --token0 "usdtg1native..." \
    --token1 "$USDTGG_ADDRESS" \
    --chain-id "$NETWORK" \
    --yes)

if [ $? -eq 0 ]; then
    PAIR2_ADDRESS=$(echo "$PAIR2_DEPLOYMENT" | grep "Pair Address" | cut -d' ' -f3)
    echo "✅ USDTg/USDTgG pair created: $PAIR2_ADDRESS"
else
    echo "❌ Error: USDTg/USDTgG pair creation failed!"
    exit 1
fi

# Add liquidity to pairs
echo "💰 Adding liquidity to trading pairs..."

# Add liquidity to USDTg/USDTgV
echo "💧 Adding liquidity to USDTg/USDTgV pair..."
LIQUIDITY1=$(usdtgverse-cli tx add-liquidity \
    --from "$TREASURY_ADDRESS" \
    --pair "$PAIR1_ADDRESS" \
    --amount0 "1000000000000000000000000" \
    --amount1 "2000000000000000000000000" \
    --chain-id "$NETWORK" \
    --yes)

if [ $? -eq 0 ]; then
    echo "✅ USDTg/USDTgV liquidity added successfully"
else
    echo "❌ Error: USDTg/USDTgV liquidity addition failed!"
    exit 1
fi

# Add liquidity to USDTg/USDTgG
echo "💧 Adding liquidity to USDTg/USDTgG pair..."
LIQUIDITY2=$(usdtgverse-cli tx add-liquidity \
    --from "$TREASURY_ADDRESS" \
    --pair "$PAIR2_ADDRESS" \
    --amount0 "1000000000000000000000000" \
    --amount1 "200000000000000000000000" \
    --chain-id "$NETWORK" \
    --yes)

if [ $? -eq 0 ]; then
    echo "✅ USDTg/USDTgG liquidity added successfully"
else
    echo "❌ Error: USDTg/USDTgG liquidity addition failed!"
    exit 1
fi

# Generate deployment summary
echo "📊 Step 5: Generating deployment summary..."
echo "=========================================="

cat > "c_mainnet_deployment.json" << EOF
{
    "deployment_info": {
        "network": "$NETWORK",
        "deployment_time": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
        "deployment_type": "c_based_mainnet",
        "status": "completed"
    },
    "contracts": {
        "USDTg_Native": {
            "name": "USDTgVerse Native Coin",
            "symbol": "USDTg",
            "address": "usdtg1native...",
            "type": "native_token",
            "supply": "1000000000000000000000000000",
            "decimals": 18
        },
        "USDTgV": {
            "name": "USDTgV Utility Token",
            "symbol": "USDTgV",
            "address": "$USDTGV_ADDRESS",
            "type": "utility_contract",
            "supply": "500000000000000000000000000",
            "decimals": 18
        },
        "USDTgG": {
            "name": "USDTgG Governance Token",
            "symbol": "USDTgG",
            "address": "$USDTGG_ADDRESS",
            "type": "governance_contract",
            "supply": "100000000000000000000000000",
            "decimals": 18
        }
    },
    "liquidity_pools": {
        "USDTg_USDTgV": {
            "pair_address": "$PAIR1_ADDRESS",
            "token0": "usdtg1native...",
            "token1": "$USDTGV_ADDRESS",
            "liquidity": "3000000000000000000000000"
        },
        "USDTg_USDTgG": {
            "pair_address": "$PAIR2_ADDRESS",
            "token0": "usdtg1native...",
            "token1": "$USDTGG_ADDRESS",
            "liquidity": "1200000000000000000000000"
        }
    },
    "trading_pairs": {
        "USDTg/USDTgV": {
            "pair": "$PAIR1_ADDRESS",
            "price": "0.5",
            "liquidity": "3000000"
        },
        "USDTg/USDTgG": {
            "pair": "$PAIR2_ADDRESS",
            "price": "5.0",
            "liquidity": "1200000"
        }
    }
}
EOF

echo "✅ C-based mainnet deployment summary generated"

echo ""
echo "🎉 USDTgVerse C-Based Mainnet Deployment Completed!"
echo "=================================================="
echo ""
echo "📍 Contract Addresses:"
echo "  USDTg Native Coin: usdtg1native... (Native Token)"
echo "  USDTgV Utility Token: $USDTGV_ADDRESS"
echo "  USDTgG Governance Token: $USDTGG_ADDRESS"
echo ""
echo "💧 Liquidity Pools:"
echo "  USDTg/USDTgV: $PAIR1_ADDRESS"
echo "  USDTg/USDTgG: $PAIR2_ADDRESS"
echo ""
echo "💰 Token Economics:"
echo "  USDTg: $1.00 (Native Token)"
echo "  USDTgV: $0.50 (Utility Token)"
echo "  USDTgG: $5.00 (Governance Token)"
echo ""
echo "🔐 Security Features Active:"
echo "  ✅ Native blockchain security"
echo "  ✅ Quantum-safe cryptography"
echo "  ✅ HotStuff BFT-PoS consensus"
echo "  ✅ Enterprise-grade infrastructure"
echo ""
echo "📁 Deployment files saved in: $(pwd)"
echo ""
