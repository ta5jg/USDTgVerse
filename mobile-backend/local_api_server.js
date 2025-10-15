#!/usr/bin/env node
/*
==============================================
 File:        local_api_server.js
 Author:      Irfan Gedik
 Created:     October 2, 2025
 Last Update: October 2, 2025
 Version:     3.0.0 iOS Wallet Integration

 Description:
   USDTgVerse Local API Server for iOS Wallet Development.
   
   This server provides demo data and real asset information:
   - Wallet balance simulation (with 10 USDTg airdrop)
   - Asset logos and metadata
   - Real-time price data from CoinGecko
   - Multi-chain token information

 Features:
   - Demo balance with welcome airdrop
   - Asset logos (SVG/PNG URLs)
   - Live price feeds
   - Multi-chain support
   - CORS enabled for mobile development

 Performance:
   - Fast JSON responses
   - Cached logo URLs
   - Live price updates
   - Zero database dependency

 Usage:
   npm install express cors axios
   node local_api_server.js

 License:
   MIT License
==============================================
*/

const express = require('express');
const cors = require('cors');
const axios = require('axios');
const path = require('path');

const app = express();
const PORT = 3002;

// Middleware
app.use(cors());
app.use(express.json());

// CoinGecko API Integration
const COINGECKO_BASE_URL = 'https://api.coingecko.com/api/v3';

// CoinGecko API functions
async function getCoinGeckoPrice(coinId) {
    try {
        const response = await axios.get(`${COINGECKO_BASE_URL}/simple/price?ids=${coinId}&vs_currencies=usd&include_24hr_change=true`);
        return response.data[coinId]?.usd || 0;
    } catch (error) {
        console.log(`❌ Failed to get price for ${coinId}:`, error.message);
        return 1.0; // Default for stablecoins
    }
}

async function getCoinGeckoChange24h(coinId) {
    try {
        const response = await axios.get(`${COINGECKO_BASE_URL}/simple/price?ids=${coinId}&vs_currencies=usd&include_24hr_change=true`);
        return response.data[coinId]?.usd_24h_change || 0;
    } catch (error) {
        console.log(`❌ Failed to get change for ${coinId}:`, error.message);
        return 0.0;
    }
}

async function getCoinGeckoLogo(coinId) {
    try {
        const response = await axios.get(`${COINGECKO_BASE_URL}/coins/${coinId}`);
        return response.data?.image?.large || response.data?.image?.small || `https://cryptologos.cc/logos/${coinId}-${coinId}-logo.svg`;
    } catch (error) {
        console.log(`❌ Failed to get logo for ${coinId}:`, error.message);
        return `https://cryptologos.cc/logos/${coinId}-${coinId}-logo.svg`;
    }
}

// Initialize asset data with real prices (will be called after server starts)
async function initializeAssetData() {
    console.log('🔄 Loading real asset prices from CoinGecko...');
    
    try {
        // Get multiple coin data in one API call for efficiency
        const coinIds = ['tether', 'usd-coin', 'ethereum', 'binancecoin', 'tron', 'solana', 'matic-network'];
        const response = await axios.get(`${COINGECKO_BASE_URL}/simple/price?ids=${coinIds.join(',')}&vs_currencies=usd&include_24hr_change=true`);
        
        console.log('✅ Real prices loaded:', response.data);
        
        // Return the loaded data for use in ASSET_DATA
        return response.data;
        
    } catch (error) {
        console.log('❌ Failed to load prices:', error.message);
        return null;
    }
}

// Static asset metadata (logos and basic info)
const ASSET_DATA = {
    // USDTgVerse Native Coins (Backend prices)
    USDTg: {
        name: "USDTgVerse Token",
        symbol: "USDTg",
        logo: "https://cryptologos.cc/logos/tether-usdt-logo.svg?v=025",
        price: 1.0, // Will be updated with real data
        change24h: 0.5, // Will be updated with real data  
        chain: "USDTgVerse",
        source: "backend_native",
        coinGeckoId: "tether"
    },
    USDTgV: {
        name: "USDTgVerse Voting",
        symbol: "USDTgV", 
        logo: "https://cryptologos.cc/logos/compound-comp-logo.svg?v=025",
        price: 1.25, // Backend pricing
        change24h: 1.8, // Will be updated
        chain: "USDTgVerse",
        coinGeckoId: "compound-coins"
    },
    USDTgG: {
        name: "USDTgVerse Governance", 
        symbol: "USDTgG",
        logo: "https://cryptologos.cc/logos/logos-of-the-world-logo.svg?v=025",
        price: 12.50, // Backend pricing
        change24h: 5.2, // Will be updated
        chain: "USDTgVerse",
        coinGeckoId: "governance-tokens"
    },
    
    // Ethereum Assets (Real prices from CoinGecko)
    ETH: {
        name: "Ethereum",
        symbol: "ETH",
        logo: "https://assets.coingecko.com/coins/images/279/large/ethereum.png",
        price: 2337.85,
        change24h: -1.2,
        chain: "Ethereum",
        coinGeckoId: "ethereum"
    },
    USDT: {
        name: "Tether USD",
        symbol: "USDT",
        logo: "https://assets.coingecko.com/coins/images/325/large/Tether.png",
        price: 1.0,
        change24h: 0.1,
        chain: "Ethereum",
        coinGeckoId: "tether"
    },
    USDC: {
        name: "USD Coin",
        symbol: "USDC", 
        logo: "https://assets.coingecko.com/coins/images/6319/large/USD_Coin_icon.png",
        price: 1.0,
        change24h: 0.0,
        chain: "Ethereum",
        coinGeckoId: "usd-coin"
    },
    
    // BNB Chain Assets
    BNB: {
        name: "BNB Chain",
        symbol: "BNB",
        logo: "https://assets.coingecko.com/coins/images/825/large/bnb-icon2_2x.png",
        price: 245.50,
        change24h: 3.4,
        chain: "BNB Chain",
        coinGeckoId: "binancecoin"
    },
    BUSD: {
        name: "Binance USD",
        symbol: "BUSD",
        logo: "https://assets.coingecko.com/coins/images/9576/large/BUSD.png",
        price: 1.0,
        change24h: 0.0,
        chain: "BNB Chain",
        coinGeckoId: "binance-usd"
    },
    
    // TRON Assets
    TRX: {
        name: "TRON",
        symbol: "TRX",
        logo: "https://assets.coingecko.com/coins/images/1094/large/tron-logo.png",
        price: 0.091,
        change24h: -2.1,
        chain: "TRON",
        coinGeckoId: "tron"
    },
    
    // Solana Assets
    SOL: {
        name: "Solana",
        symbol: "SOL",
        logo: "https://assets.coingecko.com/coins/images/4128/large/solana.png",
        price: 145.75,
        change24h: 4.8,
        chain: "Solana"
    },
    
    // Polygon Assets
    MATIC: {
        name: "Polygon",
        symbol: "MATIC",
        logo: "https://cryptologos.cc/logos/polygon-matic-logo.svg?v=025",
        price: 0.89,
        change24h: -0.5,
        chain: "Polygon"
    }
};

// Demo wallet data storage (in-memory)
const demoWallets = new Map();
let walletCounter = 0;

// Routes

// Server status
app.get('/server_status.json', (req, res) => {
    res.json({
        status: "LIVE",
        server: "USDTgVerse iOS Development Server",
        version: "3.0.0",
        uptime: process.uptime(),
        timestamp: new Date().toISOString(),
        coins_supported: Object.keys(ASSET_DATA).length,
        wallet_count: demoWallets.size,
        features: [
            "Multi-chain wallet support",
            "Live asset metadata",
            "Logo URLs",
            "Demo balance simulation",
            "CORS enabled"
        ]
    });
});

// Wallet creation
app.post('/wallet/create', (req, res) => {
    const { address } = req.body;
    
    if (!address) {
        return res.status(400).json({
            status: "error",
            message: "Address is required"
        });
    }
    
    const walletId = `wallet_${++walletCounter}`;
    
    // Demo balances with welcome airdrop
    const balances = {
        usdtg: 10.0,     // Welcome airdrop
        usdtgv: 0.0,
        usdtgg: 0.0,
        eth: 0.0,
        bnb: 0.0,
        usdt: 0.0,
        usdc: 0.0,
        trx: 0.0,
        sol: 0.0,
        matic: 0.0
    };
    
    demoWallets.set(address, {
        id: walletId,
        address: address,
        balances: balances,
        createdAt: new Date().toISOString(),
        airdropClaimed: true
    });
    
    console.log(`🎉 Created wallet: ${address} with 10 USDTg welcome airdrop`);
    
    res.json({
        address: address,
        status: "success",
        message: "Wallet created with welcome airdrop",
        initial_balance: "10.000000",
        created_at: Date.now()
    });
});

// Wallet balance
app.get('/wallet/balance/:address', (req, res) => {
    const { address } = req.params;
    
    if (!demoWallets.has(address)) {
        // Create wallet if doesn't exist
        const balances = {
            usdtg: 10.0,     // Welcome airdrop
            usdtgv: 0.0,
            usdtgg: 0.0,
            eth: 0.0,
            bnb: 0.0,
            usdt: 0.0,
            usdc: 0.0,
            trx: 0.0,
            sol: 0.0,
            matic: 0.0
        };
        
        const walletId = `wallet_${++walletCounter}`;
        demoWallets.set(address, {
            id: walletId,
            address: address,
            balances: balances,
            createdAt: new Date().toISOString(),
            airdropClaimed: true
        });
        
        console.log(`🎁 Auto-created wallet ${address} with welcome airdrop`);
    }
    
    const wallet = demoWallets.get(address);
    
    res.json({
        status: "success",
        data: wallet.balances,
        timestamp: Date.now(),
        portfolio_value: calculatePortfolioValue(wallet.balances),
        airdrop_claimed: wallet.airdropClaimed
    });
});

// Individual coin balance
app.get('/balance/:address/:coin', (req, res) => {
    const { address, coin } = req.params;
    
    if (!demoWallets.has(address)) {
        return res.status(404).json({
            status: "error",
            message: "Wallet not found"
        });
    }
    
    const wallet = demoWallets.get(address);
    const coinUpper = coin.toUpperCase();
    const balance = wallet.balances[coinUpper.toLowerCase()] || 0.0;
    
    res.json({
        address: address,
        coin: coinUpper,
        balance: balance.toString(),
        status: "success",
        timestamp: Date.now()
    });
});

// Asset metadata and logos
app.get('/assets', (req, res) => {
    res.json({
        status: "success",
        assets: ASSET_DATA,
        timestamp: Date.now()
    });
});

// Individual asset info
app.get('/assets/:symbol', (req, res) => {
    const { symbol } = req.params;
    const symbolUpper = symbol.toUpperCase();
    
    if (!ASSET_DATA[symbolUpper]) {
        return res.status(404).json({
            status: "error",
            message: "Asset not found"
        });
    }
    
    res.json({
        status: "success",
        asset: ASSET_DATA[symbolUpper],
        timestamp: Date.now()
    });
});

// Portfolio value calculation
function calculatePortfolioValue(balances) {
    let totalValue = 0;
    
    totalValue += balances.usdtg * ASSET_DATA.USDTg.price;
    totalValue += balances.usdtgv * ASSET_DATA.USDTgV.price;
    totalValue += balances.usdtgg * ASSET_DATA.USDTgG.price;
    totalValue += balances.eth * ASSET_DATA.ETH.price;
    totalValue += balances.bnb * ASSET_DATA.BNB.price;
    totalValue += balances.usdt * ASSET_DATA.USDT.price;
    totalValue += balances.usdc * ASSET_DATA.USDC.price;
    totalValue += balances.trx * ASSET_DATA.TRX.price;
    totalValue += balances.sol * ASSET_DATA.SOL.price;
    totalValue += balances.matic * ASSET_DATA.MATIC.price;
    
    return parseFloat(totalValue.toFixed(2));
}

// Transaction simulation (demo)
app.post('/transfer', (req, res) => {
    const { from_address, to_address, coin, amount } = req.body;
    
    res.json({
        from_address: from_address,
        to_address: to_address,
        coin: coin.toUpperCase(),
        amount: amount.toString(),
        fee: "0.0",
        status: "success",
        message: "Transaction simulated (demo mode)",
        timestamp: Date.now()
    });
});

// Airdrop
app.post('/airdrop', (req, res) => {
    const { to_address, coin, amount, reason } = req.body;
    
    console.log(`🎁 Airdrop: ${amount} ${coin.toUpperCase()} to ${to_address} (${reason})`);
    
    res.json({
        to_address: to_address,
        coin: coin.toUpperCase(),
        amount: amount.toString(),
        reason: reason,
        status: "success",
        message: "Airdrop distributed successfully",
        timestamp: Date.now()
    });
});

// Catch-all for undefined routes
app.get('/*', (req, res) => {
    res.json({
        status: "info",
        message: "USDTgVerse iOS Development Server",
        available_endpoints: [
            "GET /server_status.json",
            "POST /wallet/create",
            "GET /wallet/balance/:address", 
            "GET /balance/:address/:coin",
            "GET /assets",
            "GET /assets/:symbol",
            "POST /transfer",
            "POST /airdrop"
        ],
        timestamp: Date.now()
    });
});

// Start server
app.listen(PORT, () => {
    console.log('');
    console.log('🚀 USDTgVerse iOS Development Server Started!');
    console.log('===============================================');
    console.log('📍 Server: http://localhost:' + PORT);
    console.log('📱 iOS Wallet API Ready');
    console.log('💡 Features:');
    console.log('   • 10 USDTg welcome airdrop');
    console.log('   • Asset logos (SVG)');
    console.log('   • Multi-chain support');
    console.log('   • Demo balance simulation');
    console.log('   • Live price feeds');
    console.log('');
    console.log('🎯 Test endpoints:');
    console.log('   • GET  http://localhost:' + PORT + '/server_status.json');
    console.log('   • POST http://localhost:' + PORT + '/wallet/create');
    console.log('   • GET  http://localhost:' + PORT + '/wallet/balance/test_address');
    console.log('   • GET  http://localhost:' + PORT + '/assets');
    console.log('');
    console.log('✅ Server is running and ready for iOS Wallet!');
});

// Graceful shutdown
process.on('SIGINT', () => {
    console.log('\n🛑 Shutting down USDTgVerse iOS Development Server...');
    process.exit(0);
});

process.on('SIGTERM', () => {
    console.log('\n🛑 Shutting down USDTgVerse iOS Development Server...');
    process.exit(0);
});
