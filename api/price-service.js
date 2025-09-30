/**
 * USDTgVerse Real-Time Price Service
 * 
 * This service provides real-time price data for:
 * - USDTgVerse native coins (USDTg, USDTgV, USDTgG) from our DEX/CEX
 * - External coins from CoinGecko API
 * 
 * Author: Irfan Gedik
 * Created: 2025-09-30
 * Version: 1.0
 */

class USDTgVersePriceService {
    constructor() {
        this.usdtgverseAPI = 'https://api.usdtgverse.com';
        this.coingeckoAPI = 'https://api.coingecko.com/api/v3';
        this.cache = new Map();
        this.cacheTimeout = 30000; // 30 seconds
        this.updateInterval = 10000; // 10 seconds
        
        // USDTgVerse native tokens
        this.nativeTokens = {
            'USDTg': {
                name: 'USDTgVerse Native Coin',
                symbol: 'USDTg',
                decimals: 6,
                stable: true,
                targetPrice: 1.00
            },
            'USDTgV': {
                name: 'USDTgVerse Utility Token',
                symbol: 'USDTgV',
                decimals: 18,
                stable: false
            },
            'USDTgG': {
                name: 'USDTgVerse Governance Token',
                symbol: 'USDTgG',
                decimals: 18,
                stable: false
            }
        };
        
        // External tokens for CoinGecko
        this.externalTokens = {
            'BTC': 'bitcoin',
            'ETH': 'ethereum',
            'BNB': 'binancecoin',
            'SOL': 'solana',
            'ADA': 'cardano',
            'DOT': 'polkadot',
            'MATIC': 'matic-network',
            'AVAX': 'avalanche-2',
            'LINK': 'chainlink',
            'UNI': 'uniswap',
            'USDT': 'tether',
            'USDC': 'usd-coin',
            'DAI': 'dai',
            'TRX': 'tron'
        };
        
        this.startPriceUpdates();
    }
    
    /**
     * Get price for a specific token
     */
    async getPrice(symbol) {
        const cacheKey = `price_${symbol}`;
        const cached = this.cache.get(cacheKey);
        
        if (cached && Date.now() - cached.timestamp < this.cacheTimeout) {
            return cached.data;
        }
        
        try {
            let priceData;
            
            if (this.nativeTokens[symbol]) {
                priceData = await this.getNativeTokenPrice(symbol);
            } else if (this.externalTokens[symbol]) {
                priceData = await this.getExternalTokenPrice(symbol);
            } else {
                throw new Error(`Unknown token: ${symbol}`);
            }
            
            this.cache.set(cacheKey, {
                data: priceData,
                timestamp: Date.now()
            });
            
            return priceData;
        } catch (error) {
            console.error(`Error fetching price for ${symbol}:`, error);
            return this.getFallbackPrice(symbol);
        }
    }
    
    /**
     * Get prices for multiple tokens
     */
    async getPrices(symbols) {
        const promises = symbols.map(symbol => this.getPrice(symbol));
        const results = await Promise.allSettled(promises);
        
        const prices = {};
        symbols.forEach((symbol, index) => {
            if (results[index].status === 'fulfilled') {
                prices[symbol] = results[index].value;
            } else {
                prices[symbol] = this.getFallbackPrice(symbol);
            }
        });
        
        return prices;
    }
    
    /**
     * Get USDTgVerse native token price from our DEX/CEX
     */
    async getNativeTokenPrice(symbol) {
        try {
            // Simulate API call to our DEX/CEX
            // In production, this would be a real API call
            const response = await fetch(`${this.usdtgverseAPI}/v1/prices/${symbol}`, {
                method: 'GET',
                headers: {
                    'Content-Type': 'application/json',
                    'X-API-Key': 'usdtgverse-api-key'
                }
            });
            
            if (!response.ok) {
                throw new Error(`API error: ${response.status}`);
            }
            
            const data = await response.json();
            
            return {
                symbol: symbol,
                price: parseFloat(data.price),
                change24h: parseFloat(data.change_24h || 0),
                volume24h: parseFloat(data.volume_24h || 0),
                marketCap: parseFloat(data.market_cap || 0),
                lastUpdated: new Date().toISOString(),
                source: 'USDTgVerse DEX/CEX'
            };
        } catch (error) {
            console.warn(`Failed to fetch ${symbol} from USDTgVerse API, using fallback`);
            return this.getFallbackNativePrice(symbol);
        }
    }
    
    /**
     * Get external token price from CoinGecko
     */
    async getExternalTokenPrice(symbol) {
        try {
            const coinId = this.externalTokens[symbol];
            const response = await fetch(
                `${this.coingeckoAPI}/simple/price?ids=${coinId}&vs_currencies=usd&include_24hr_change=true&include_24hr_vol=true&include_market_cap=true`
            );
            
            if (!response.ok) {
                throw new Error(`CoinGecko API error: ${response.status}`);
            }
            
            const data = await response.json();
            const coinData = data[coinId];
            
            if (!coinData) {
                throw new Error(`Coin not found: ${coinId}`);
            }
            
            return {
                symbol: symbol,
                price: parseFloat(coinData.usd),
                change24h: parseFloat(coinData.usd_24h_change || 0),
                volume24h: parseFloat(coinData.usd_24h_vol || 0),
                marketCap: parseFloat(coinData.usd_market_cap || 0),
                lastUpdated: new Date().toISOString(),
                source: 'CoinGecko'
            };
        } catch (error) {
            console.warn(`Failed to fetch ${symbol} from CoinGecko, using fallback`);
            return this.getFallbackExternalPrice(symbol);
        }
    }
    
    /**
     * Fallback price for native tokens
     */
    getFallbackNativePrice(symbol) {
        const fallbackPrices = {
            'USDTg': { price: 1.00, change24h: 0.00 },
            'USDTgV': { price: 0.50, change24h: 2.50 },
            'USDTgG': { price: 5.00, change24h: -1.20 }
        };
        
        const fallback = fallbackPrices[symbol] || { price: 0.00, change24h: 0.00 };
        
        return {
            symbol: symbol,
            price: fallback.price,
            change24h: fallback.change24h,
            volume24h: 0,
            marketCap: 0,
            lastUpdated: new Date().toISOString(),
            source: 'Fallback'
        };
    }
    
    /**
     * Fallback price for external tokens
     */
    getFallbackExternalPrice(symbol) {
        const fallbackPrices = {
            'BTC': { price: 45000, change24h: 2.5 },
            'ETH': { price: 3000, change24h: 1.8 },
            'BNB': { price: 300, change24h: -0.5 },
            'SOL': { price: 100, change24h: 3.2 },
            'USDT': { price: 1.00, change24h: 0.00 },
            'USDC': { price: 1.00, change24h: 0.00 }
        };
        
        const fallback = fallbackPrices[symbol] || { price: 0.00, change24h: 0.00 };
        
        return {
            symbol: symbol,
            price: fallback.price,
            change24h: fallback.change24h,
            volume24h: 0,
            marketCap: 0,
            lastUpdated: new Date().toISOString(),
            source: 'Fallback'
        };
    }
    
    /**
     * Get fallback price for any token
     */
    getFallbackPrice(symbol) {
        if (this.nativeTokens[symbol]) {
            return this.getFallbackNativePrice(symbol);
        } else {
            return this.getFallbackExternalPrice(symbol);
        }
    }
    
    /**
     * Start automatic price updates
     */
    startPriceUpdates() {
        setInterval(async () => {
            try {
                // Update native tokens
                for (const symbol of Object.keys(this.nativeTokens)) {
                    await this.getPrice(symbol);
                }
                
                // Update external tokens
                for (const symbol of Object.keys(this.externalTokens)) {
                    await this.getPrice(symbol);
                }
                
                // Trigger price update event
                this.dispatchPriceUpdateEvent();
            } catch (error) {
                console.error('Error in price update cycle:', error);
            }
        }, this.updateInterval);
    }
    
    /**
     * Dispatch price update event
     */
    dispatchPriceUpdateEvent() {
        const event = new CustomEvent('usdtgverse-price-update', {
            detail: {
                timestamp: new Date().toISOString(),
                cache: Object.fromEntries(this.cache)
            }
        });
        window.dispatchEvent(event);
    }
    
    /**
     * Get all cached prices
     */
    getAllPrices() {
        const prices = {};
        for (const [key, value] of this.cache.entries()) {
            if (key.startsWith('price_')) {
                const symbol = key.replace('price_', '');
                prices[symbol] = value.data;
            }
        }
        return prices;
    }
    
    /**
     * Clear cache
     */
    clearCache() {
        this.cache.clear();
    }
}

// Global instance
window.USDTgVersePriceService = new USDTgVersePriceService();

// Export for module systems
if (typeof module !== 'undefined' && module.exports) {
    module.exports = USDTgVersePriceService;
}
