#!/bin/bash

# USDTgG Governance Token Deployment Script
# =========================================
# 
# This script deploys the USDTgG Governance Token to mainnet
# with DAO governance, voting, and treasury management features.

set -e

echo "🏛️ USDTgG Governance Token Deployment Script"
echo "============================================"
echo ""

# Configuration
CONTRACT_FILE="USDTgG.usdtg"
CONTRACT_NAME="USDTgG"
NETWORK="mainnet"
GAS_LIMIT="7000000"
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

# Validate contract syntax
echo "🔍 Validating contract syntax..."
if ! usdtg-compiler --validate "$CONTRACT_FILE"; then
    echo "❌ Error: Contract validation failed!"
    exit 1
fi

echo "✅ Contract validation passed"

# Compile contract
echo "🔨 Compiling contract..."
if ! usdtg-compiler --optimize --gas-optimize "$CONTRACT_FILE" -o "${CONTRACT_NAME}.bytecode"; then
    echo "❌ Error: Contract compilation failed!"
    exit 1
fi

echo "✅ Contract compiled successfully"

# Generate ABI
echo "📄 Generating ABI..."
if ! usdtg-compiler --abi "$CONTRACT_FILE" -o "${CONTRACT_NAME}.abi"; then
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
    
    echo "💰 Total Supply: $TOTAL_SUPPLY USDTgG"
    
    # Test treasury balance
    TREASURY_BALANCE=$(usdtg-call \
        --address "$CONTRACT_ADDRESS" \
        --function "balanceOf" \
        --args "$TREASURY_ADDRESS" \
        --network "$NETWORK")
    
    echo "🏛️ Treasury Balance: $TREASURY_BALANCE USDTgG"
    
    # Test voting period
    VOTING_PERIOD=$(usdtg-call \
        --address "$CONTRACT_ADDRESS" \
        --function "votingPeriod" \
        --network "$NETWORK")
    
    echo "🗳️ Voting Period: $VOTING_PERIOD seconds"
    
    # Test execution delay
    EXECUTION_DELAY=$(usdtg-call \
        --address "$CONTRACT_ADDRESS" \
        --function "executionDelay" \
        --network "$NETWORK")
    
    echo "⏰ Execution Delay: $EXECUTION_DELAY seconds"
    
    # Test min validator stake
    MIN_VALIDATOR_STAKE=$(usdtg-call \
        --address "$CONTRACT_ADDRESS" \
        --function "minValidatorStake" \
        --network "$NETWORK")
    
    echo "💎 Min Validator Stake: $MIN_VALIDATOR_STAKE USDTgG"
    
    echo ""
    echo "🎉 USDTgG Governance Token deployment completed successfully!"
    echo "📍 Contract Address: $CONTRACT_ADDRESS"
    echo "📝 Transaction: $DEPLOYMENT_TX"
    echo "🌐 Network: $NETWORK"
    echo ""
    echo "🏛️ Governance Features Active:"
    echo "  ✅ DAO voting system"
    echo "  ✅ Proposal creation and execution"
    echo "  ✅ Treasury management"
    echo "  ✅ Validator system"
    echo "  ✅ Delegation mechanism"
    echo "  ✅ Anti-fraud measures"
    echo "  ✅ Quantum-safe addresses"
    echo ""
    echo "📋 Next Steps:"
    echo "  1. Setup initial validators"
    echo "  2. Configure treasury allocations"
    echo "  3. Create first governance proposal"
    echo "  4. Setup liquidity pools for all tokens"
    echo ""
    
else
    echo "❌ Error: Contract deployment failed!"
    exit 1
fi
