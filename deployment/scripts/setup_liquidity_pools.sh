#!/bin/bash

# USDTgVerse Liquidity Pools Setup Script
# =======================================
# 
# This script sets up liquidity pools for all three tokens
# and configures DEX trading pairs.

set -e

echo "💧 USDTgVerse Liquidity Pools Setup Script"
echo "=========================================="
echo ""

# Configuration
NETWORK="mainnet"
GAS_LIMIT="8000000"
GAS_PRICE="20000000000"  # 20 Gwei

# Contract addresses (these will be filled after deployment)
USDTG_NATIVE_ADDRESS=""
USDTGV_ADDRESS=""
USDTGG_ADDRESS=""

# Liquidity amounts (in wei)
USDTG_NATIVE_LIQUIDITY="1000000000000000000000000"  # 1M USDTg
USDTGV_LIQUIDITY="500000000000000000000000000"      # 500M USDTgV
USDTGG_LIQUIDITY="100000000000000000000000000"      # 100M USDTgG

# Initial prices (in USDTg)
USDTGV_PRICE="500000000000000000"  # 0.5 USDTg per USDTgV
USDTGG_PRICE="5000000000000000000" # 5.0 USDTg per USDTgG

echo "📋 Liquidity Pool Configuration:"
echo "  Network: $NETWORK"
echo "  Gas Limit: $GAS_LIMIT"
echo "  Gas Price: $GAS_PRICE wei"
echo ""
echo "💰 Liquidity Amounts:"
echo "  USDTg Native: $USDTG_NATIVE_LIQUIDITY wei (1M USDTg)"
echo "  USDTgV: $USDTGV_LIQUIDITY wei (500M USDTgV)"
echo "  USDTgG: $USDTGG_LIQUIDITY wei (100M USDTgG)"
echo ""
echo "💲 Initial Prices:"
echo "  USDTgV: $USDTGV_PRICE wei (0.5 USDTg)"
echo "  USDTgG: $USDTGG_PRICE wei (5.0 USDTg)"
echo ""

# Check if contract addresses are provided
if [ -z "$USDTG_NATIVE_ADDRESS" ] || [ -z "$USDTGV_ADDRESS" ] || [ -z "$USDTGG_ADDRESS" ]; then
    echo "❌ Error: Contract addresses not provided!"
    echo "   Please set USDTG_NATIVE_ADDRESS, USDTGV_ADDRESS, and USDTGG_ADDRESS"
    echo "   or run the deployment scripts first."
    exit 1
fi

echo "✅ Contract addresses found:"
echo "  USDTg Native: $USDTG_NATIVE_ADDRESS"
echo "  USDTgV: $USDTGV_ADDRESS"
echo "  USDTgG: $USDTGG_ADDRESS"
echo ""

# Create liquidity pool factory contract
echo "🏭 Creating liquidity pool factory..."
FACTORY_TX=$(usdtg-deploy \
    --network "$NETWORK" \
    --contract "LiquidityPoolFactory.usdtg" \
    --gas-limit "$GAS_LIMIT" \
    --gas-price "$GAS_PRICE" \
    --verify)

if [ $? -eq 0 ]; then
    FACTORY_ADDRESS=$(usdtg-get-contract-address "$FACTORY_TX")
    echo "✅ Liquidity pool factory deployed: $FACTORY_ADDRESS"
else
    echo "❌ Error: Factory deployment failed!"
    exit 1
fi

# Create USDTg/USDTgV trading pair
echo "🔄 Creating USDTg/USDTgV trading pair..."
PAIR1_TX=$(usdtg-call \
    --address "$FACTORY_ADDRESS" \
    --function "createPair" \
    --args "$USDTG_NATIVE_ADDRESS,$USDTGV_ADDRESS" \
    --gas-limit "$GAS_LIMIT" \
    --gas-price "$GAS_PRICE" \
    --network "$NETWORK")

if [ $? -eq 0 ]; then
    PAIR1_ADDRESS=$(usdtg-get-pair-address "$FACTORY_ADDRESS" "$USDTG_NATIVE_ADDRESS" "$USDTGV_ADDRESS")
    echo "✅ USDTg/USDTgV pair created: $PAIR1_ADDRESS"
else
    echo "❌ Error: USDTg/USDTgV pair creation failed!"
    exit 1
fi

# Create USDTg/USDTgG trading pair
echo "🔄 Creating USDTg/USDTgG trading pair..."
PAIR2_TX=$(usdtg-call \
    --address "$FACTORY_ADDRESS" \
    --function "createPair" \
    --args "$USDTG_NATIVE_ADDRESS,$USDTGG_ADDRESS" \
    --gas-limit "$GAS_LIMIT" \
    --gas-price "$GAS_PRICE" \
    --network "$NETWORK")

if [ $? -eq 0 ]; then
    PAIR2_ADDRESS=$(usdtg-get-pair-address "$FACTORY_ADDRESS" "$USDTG_NATIVE_ADDRESS" "$USDTGG_ADDRESS")
    echo "✅ USDTg/USDTgG pair created: $PAIR2_ADDRESS"
else
    echo "❌ Error: USDTg/USDTgG pair creation failed!"
    exit 1
fi

# Add liquidity to USDTg/USDTgV pair
echo "💧 Adding liquidity to USDTg/USDTgV pair..."
LIQUIDITY1_TX=$(usdtg-call \
    --address "$PAIR1_ADDRESS" \
    --function "addLiquidity" \
    --args "$USDTG_NATIVE_LIQUIDITY,$USDTGV_LIQUIDITY" \
    --gas-limit "$GAS_LIMIT" \
    --gas-price "$GAS_PRICE" \
    --network "$NETWORK")

if [ $? -eq 0 ]; then
    echo "✅ USDTg/USDTgV liquidity added successfully"
else
    echo "❌ Error: USDTg/USDTgV liquidity addition failed!"
    exit 1
fi

# Add liquidity to USDTg/USDTgG pair
echo "💧 Adding liquidity to USDTg/USDTgG pair..."
LIQUIDITY2_TX=$(usdtg-call \
    --address "$PAIR2_ADDRESS" \
    --function "addLiquidity" \
    --args "$USDTG_NATIVE_LIQUIDITY,$USDTGG_LIQUIDITY" \
    --gas-limit "$GAS_LIMIT" \
    --gas-price "$GAS_PRICE" \
    --network "$NETWORK")

if [ $? -eq 0 ]; then
    echo "✅ USDTg/USDTgG liquidity added successfully"
else
    echo "❌ Error: USDTg/USDTgG liquidity addition failed!"
    exit 1
fi

# Set initial prices
echo "💲 Setting initial prices..."
PRICE1_TX=$(usdtg-call \
    --address "$PAIR1_ADDRESS" \
    --function "setPrice" \
    --args "$USDTGV_PRICE" \
    --gas-limit "$GAS_LIMIT" \
    --gas-price "$GAS_PRICE" \
    --network "$NETWORK")

if [ $? -eq 0 ]; then
    echo "✅ USDTgV price set to 0.5 USDTg"
else
    echo "❌ Error: USDTgV price setting failed!"
    exit 1
fi

PRICE2_TX=$(usdtg-call \
    --address "$PAIR2_ADDRESS" \
    --function "setPrice" \
    --args "$USDTGG_PRICE" \
    --gas-limit "$GAS_LIMIT" \
    --gas-price "$GAS_PRICE" \
    --network "$NETWORK")

if [ $? -eq 0 ]; then
    echo "✅ USDTgG price set to 5.0 USDTg"
else
    echo "❌ Error: USDTgG price setting failed!"
    exit 1
fi

# Save liquidity pool info
cat > "liquidity_pools_info.json" << EOF
{
    "network": "$NETWORK",
    "factory_address": "$FACTORY_ADDRESS",
    "pairs": {
        "USDTg_USDTgV": {
            "address": "$PAIR1_ADDRESS",
            "token0": "$USDTG_NATIVE_ADDRESS",
            "token1": "$USDTGV_ADDRESS",
            "liquidity_tx": "$LIQUIDITY1_TX",
            "price_tx": "$PRICE1_TX",
            "initial_price": "$USDTGV_PRICE"
        },
        "USDTg_USDTgG": {
            "address": "$PAIR2_ADDRESS",
            "token0": "$USDTG_NATIVE_ADDRESS",
            "token1": "$USDTGG_ADDRESS",
            "liquidity_tx": "$LIQUIDITY2_TX",
            "price_tx": "$PRICE2_TX",
            "initial_price": "$USDTGG_PRICE"
        }
    },
    "liquidity_amounts": {
        "USDTg_native": "$USDTG_NATIVE_LIQUIDITY",
        "USDTgV": "$USDTGV_LIQUIDITY",
        "USDTgG": "$USDTGG_LIQUIDITY"
    },
    "setup_time": "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
}
EOF

echo "📊 Liquidity pool information saved to liquidity_pools_info.json"

# Test trading pairs
echo "🧪 Testing trading pairs..."

# Test USDTg/USDTgV pair
PAIR1_RESERVE0=$(usdtg-call \
    --address "$PAIR1_ADDRESS" \
    --function "getReserve0" \
    --network "$NETWORK")

PAIR1_RESERVE1=$(usdtg-call \
    --address "$PAIR1_ADDRESS" \
    --function "getReserve1" \
    --network "$NETWORK")

echo "📊 USDTg/USDTgV Pair Reserves:"
echo "  USDTg: $PAIR1_RESERVE0"
echo "  USDTgV: $PAIR1_RESERVE1"

# Test USDTg/USDTgG pair
PAIR2_RESERVE0=$(usdtg-call \
    --address "$PAIR2_ADDRESS" \
    --function "getReserve0" \
    --network "$NETWORK")

PAIR2_RESERVE1=$(usdtg-call \
    --address "$PAIR2_ADDRESS" \
    --function "getReserve1" \
    --network "$NETWORK")

echo "📊 USDTg/USDTgG Pair Reserves:"
echo "  USDTg: $PAIR2_RESERVE0"
echo "  USDTgG: $PAIR2_RESERVE1"

echo ""
echo "🎉 Liquidity pools setup completed successfully!"
echo "🏭 Factory Address: $FACTORY_ADDRESS"
echo "🔄 USDTg/USDTgV Pair: $PAIR1_ADDRESS"
echo "🔄 USDTg/USDTgG Pair: $PAIR2_ADDRESS"
echo ""
echo "💧 Liquidity Added:"
echo "  USDTg/USDTgV: 1M USDTg + 500M USDTgV"
echo "  USDTg/USDTgG: 1M USDTg + 100M USDTgG"
echo ""
echo "💲 Initial Prices:"
echo "  USDTgV: 0.5 USDTg"
echo "  USDTgG: 5.0 USDTg"
echo ""
echo "📋 Next Steps:"
echo "  1. Test trading functionality"
echo "  2. Setup price oracles"
echo "  3. Configure DEX interface"
echo "  4. Launch public trading"
echo ""
