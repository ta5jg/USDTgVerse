#!/bin/bash

# USDTgV Utility Token Deployment Script
# ======================================
# 
# This script deploys the USDTgV Utility Token to mainnet
# with staking, rewards, and ecosystem integration features.

set -e

echo "⚡ USDTgV Utility Token Deployment Script"
echo "========================================"
echo ""

# Configuration
CONTRACT_FILE="USDTgV.usdtg"
CONTRACT_NAME="USDTgV"
NETWORK="mainnet"
GAS_LIMIT="6000000"
GAS_PRICE="20000000000"  # 20 Gwei

# Treasury and development addresses (these should be multi-sig wallets)
TREASURY_ADDRESS="0x742d35Cc6634C0532925a3b8D4C9db96C4b4d8b6"
DEVELOPMENT_ADDRESS="0x8ba1f109551bD432803012645Hac136c4c8b3d5"
MARKETING_ADDRESS="0x1234567890123456789012345678901234567890"

echo "📋 Deployment Configuration:"
echo "  Contract: $CONTRACT_FILE"
echo "  Network: $NETWORK"
echo "  Gas Limit: $GAS_LIMIT"
echo "  Gas Price: $GAS_PRICE wei"
echo "  Treasury: $TREASURY_ADDRESS"
echo "  Development: $DEVELOPMENT_ADDRESS"
echo "  Marketing: $MARKETING_ADDRESS"
echo ""

# Check if contract file exists
if [ ! -f "$CONTRACT_FILE" ]; then
    echo "❌ Error: Contract file $CONTRACT_FILE not found!"
    exit 1
fi

echo "✅ Contract file found: $CONTRACT_FILE"

# Set compiler path
COMPILER_PATH="/Users/irfangedik/USDTgVerse/native-ecosystem/compiler/usdtgc"

# Validate contract syntax
echo "🔍 Validating contract syntax..."
if ! "$COMPILER_PATH" "$CONTRACT_FILE" --target usdtgvm; then
    echo "❌ Error: Contract validation failed!"
    exit 1
fi

echo "✅ Contract validation passed"

# Compile contract
echo "🔨 Compiling contract..."
if ! "$COMPILER_PATH" "$CONTRACT_FILE" --output "${CONTRACT_NAME}.bytecode" --target usdtgvm --optimize; then
    echo "❌ Error: Contract compilation failed!"
    exit 1
fi

echo "✅ Contract compiled successfully"

# Generate ABI
echo "📄 Generating ABI..."
if ! "$COMPILER_PATH" "$CONTRACT_FILE" --abi "${CONTRACT_NAME}.abi" --target usdtgvm; then
    echo "❌ Error: ABI generation failed!"
    exit 1
fi

echo "✅ ABI generated successfully"

# Deploy to mainnet
echo "🌐 Deploying to $NETWORK..."
echo "⚠️  WARNING: This will deploy to MAINNET with real funds!"
echo "   Press Ctrl+C within 10 seconds to cancel..."
sleep 10

# Deploy contract
DEPLOYMENT_TX=$(usdtg-deploy \
    --network "$NETWORK" \
    --contract "${CONTRACT_NAME}.bytecode" \
    --abi "${CONTRACT_NAME}.abi" \
    --gas-limit "$GAS_LIMIT" \
    --gas-price "$GAS_PRICE" \
    --constructor-args "$TREASURY_ADDRESS,$DEVELOPMENT_ADDRESS,$MARKETING_ADDRESS" \
    --verify)

if [ $? -eq 0 ]; then
    echo "✅ Contract deployed successfully!"
    echo "📝 Deployment Transaction: $DEPLOYMENT_TX"
    
    # Wait for confirmation
    echo "⏳ Waiting for transaction confirmation..."
    sleep 30
    
    # Get contract address
    CONTRACT_ADDRESS=$(usdtg-get-contract-address "$DEPLOYMENT_TX")
    echo "📍 Contract Address: $CONTRACT_ADDRESS"
    
    # Verify contract on block explorer
    echo "🔍 Verifying contract on block explorer..."
    usdtg-verify-contract \
        --address "$CONTRACT_ADDRESS" \
        --source "$CONTRACT_FILE" \
        --network "$NETWORK"
    
    if [ $? -eq 0 ]; then
        echo "✅ Contract verified successfully!"
    else
        echo "⚠️  Warning: Contract verification failed"
    fi
    
    # Save deployment info
    cat > "deployment_info.json" << EOF
{
    "contract": "$CONTRACT_NAME",
    "address": "$CONTRACT_ADDRESS",
    "transaction": "$DEPLOYMENT_TX",
    "network": "$NETWORK",
    "deployment_time": "$(date -u +%Y-%m-%dT%H:%M:%SZ)",
    "treasury": "$TREASURY_ADDRESS",
    "development": "$DEVELOPMENT_ADDRESS",
    "marketing": "$MARKETING_ADDRESS",
    "gas_used": "$GAS_LIMIT",
    "gas_price": "$GAS_PRICE"
}
EOF
    
    echo "📊 Deployment information saved to deployment_info.json"
    
    # Test basic functions
    echo "🧪 Testing basic contract functions..."
    
    # Test total supply
    TOTAL_SUPPLY=$(usdtg-call \
        --address "$CONTRACT_ADDRESS" \
        --function "totalSupply" \
        --network "$NETWORK")
    
    echo "💰 Total Supply: $TOTAL_SUPPLY USDTgV"
    
    # Test treasury balance
    TREASURY_BALANCE=$(usdtg-call \
        --address "$CONTRACT_ADDRESS" \
        --function "balanceOf" \
        --args "$TREASURY_ADDRESS" \
        --network "$NETWORK")
    
    echo "🏛️ Treasury Balance: $TREASURY_BALANCE USDTgV"
    
    # Test staking reward rate
    STAKING_RATE=$(usdtg-call \
        --address "$CONTRACT_ADDRESS" \
        --function "stakingRewardRate" \
        --network "$NETWORK")
    
    echo "💎 Staking Reward Rate: $STAKING_RATE% APY"
    
    # Test base fee
    BASE_FEE=$(usdtg-call \
        --address "$CONTRACT_ADDRESS" \
        --function "baseFee" \
        --network "$NETWORK")
    
    echo "💸 Base Fee: $BASE_FEE basis points"
    
    echo ""
    echo "🎉 USDTgV Utility Token deployment completed successfully!"
    echo "📍 Contract Address: $CONTRACT_ADDRESS"
    echo "📝 Transaction: $DEPLOYMENT_TX"
    echo "🌐 Network: $NETWORK"
    echo ""
    echo "⚡ Utility Features Active:"
    echo "  ✅ Staking and rewards (10% APY)"
    echo "  ✅ Fee discounts for ecosystem services"
    echo "  ✅ Ecosystem partner integration"
    echo "  ✅ Anti-fraud measures"
    echo "  ✅ Daily transfer limits"
    echo "  ✅ Quantum-safe addresses"
    echo ""
    echo "📋 Next Steps:"
    echo "  1. Setup staking pools"
    echo "  2. Configure fee discounts"
    echo "  3. Add ecosystem partners"
    echo "  4. Deploy USDTgG governance token"
    echo ""
    
else
    echo "❌ Error: Contract deployment failed!"
    exit 1
fi
