//
//  TokenPriceService.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-28.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import Foundation
import SwiftUI

// MARK: - Token Price Service
class TokenPriceService: ObservableObject {
    @Published var tokenPrices: [String: Double] = [:]
    @Published var isLoading = false
    @Published var lastUpdate: Date?
    
    private let session = URLSession.shared
    private let usdtgverseAPI = "https://api.usdtgverse.com"
    private let coingeckoAPI = "https://api.coingecko.com/api/v3"
    
    // MARK: - USDTgVerse Native Tokens
    private let usdtgverseTokens = [
        "USDTg": "USDTgVerse Native Coin",
        "USDTgV": "USDTgVerse Utility Token", 
        "USDTgG": "USDTgVerse Governance Token"
    ]
    
    // MARK: - Token Logo URLs
    private let tokenLogoURLs: [String: String] = [
        "USDTg": "https://usdtgverse.com/assets/logos/logo.png",
        "USDTgV": "https://usdtgverse.com/assets/logos/USDTgV-logo.png",
        "USDTgG": "https://usdtgverse.com/assets/logos/USDTgG-logo.png",
        "USDT": "https://assets.coingecko.com/coins/images/325/large/Tether.png",
        "USDC": "https://assets.coingecko.com/coins/images/6319/large/USD_Coin_icon.png",
        "BTC": "https://assets.coingecko.com/coins/images/1/large/bitcoin.png",
        "ETH": "https://assets.coingecko.com/coins/images/279/large/ethereum.png",
        "BNB": "https://assets.coingecko.com/coins/images/825/large/bnb-icon2_2x.png",
        "SOL": "https://assets.coingecko.com/coins/images/4128/large/solana.png",
        "TRX": "https://assets.coingecko.com/coins/images/1094/large/tron-logo.png",
        "MATIC": "https://assets.coingecko.com/coins/images/4713/large/matic-token-icon.png"
    ]
    
    // MARK: - External Tokens
    private let externalTokens = [
        "USDT": "tether",
        "USDC": "usd-coin", 
        "BTC": "bitcoin",
        "ETH": "ethereum",
        "BNB": "binancecoin",
        "SOL": "solana",
        "TRX": "tron",
        "MATIC": "matic-network",
        "AVAX": "avalanche-2",
        "ADA": "cardano",
        "DOT": "polkadot",
        "LINK": "chainlink",
        "UNI": "uniswap",
        "LTC": "litecoin",
        "BCH": "bitcoin-cash",
        "XRP": "ripple",
        "DOGE": "dogecoin",
        "SHIB": "shiba-inu"
    ]
    
    init() {
        loadCachedPrices()
        fetchAllPrices()
    }
    
    // MARK: - Price Fetching
    func fetchAllPrices() {
        isLoading = true
        
        Task {
            await withTaskGroup(of: Void.self) { group in
                // Fetch USDTgVerse native token prices
                group.addTask {
                    await self.fetchUSDTgVersePrices()
                }
                
                // Fetch external token prices
                group.addTask {
                    await self.fetchExternalPrices()
                }
            }
            
            await MainActor.run {
                self.isLoading = false
                self.lastUpdate = Date()
                self.saveCachedPrices()
            }
        }
    }
    
    // MARK: - USDTgVerse Native Token Prices
    private func fetchUSDTgVersePrices() async {
        do {
            let url = URL(string: "\(usdtgverseAPI)/tokens/prices")!
            let (data, _) = try await session.data(from: url)
            
            if let response = try? JSONSerialization.jsonObject(with: data) as? [String: Any] {
                await MainActor.run {
                    if let usdtg = response["USDTg"] as? Double {
                        self.tokenPrices["USDTg"] = usdtg
                    }
                    if let usdtgv = response["USDTgV"] as? Double {
                        self.tokenPrices["USDTgV"] = usdtgv
                    }
                    if let usdtgg = response["USDTgG"] as? Double {
                        self.tokenPrices["USDTgG"] = usdtgg
                    }
                }
            }
        } catch {
            print("❌ Failed to fetch USDTgVerse prices: \(error)")
            // Fallback to default prices
            await MainActor.run {
                self.tokenPrices["USDTg"] = 1.00
                self.tokenPrices["USDTgV"] = 0.50
                self.tokenPrices["USDTgG"] = 5.00
            }
        }
    }
    
    // MARK: - External Token Prices
    private func fetchExternalPrices() async {
        let tokenIds = Array(externalTokens.values).joined(separator: ",")
        let urlString = "\(coingeckoAPI)/simple/price?ids=\(tokenIds)&vs_currencies=usd"
        
        do {
            let url = URL(string: urlString)!
            let (data, _) = try await session.data(from: url)
            
            if let response = try? JSONSerialization.jsonObject(with: data) as? [String: [String: Double]] {
                await MainActor.run {
                    for (symbol, coingeckoId) in self.externalTokens {
                        if let priceData = response[coingeckoId],
                           let price = priceData["usd"] {
                            self.tokenPrices[symbol] = price
                        }
                    }
                }
            }
        } catch {
            print("❌ Failed to fetch external prices: \(error)")
            // Fallback to cached or default prices
            await MainActor.run {
                self.loadDefaultPrices()
            }
        }
    }
    
    // MARK: - Default Prices (Fallback)
    private func loadDefaultPrices() {
        let defaultPrices: [String: Double] = [
            "USDTg": 1.00,
            "USDTgV": 0.50,
            "USDTgG": 5.00,
            "USDT": 1.00,
            "USDC": 1.00,
            "BTC": 45000.0,
            "ETH": 2500.0,
            "BNB": 300.0,
            "SOL": 100.0,
            "TRX": 0.08,
            "MATIC": 0.80,
            "AVAX": 25.0,
            "ADA": 0.45,
            "DOT": 6.50,
            "LINK": 12.0,
            "UNI": 8.50,
            "LTC": 70.0,
            "BCH": 250.0,
            "XRP": 0.60,
            "DOGE": 0.08,
            "SHIB": 0.00001
        ]
        
        for (symbol, price) in defaultPrices {
            if tokenPrices[symbol] == nil {
                tokenPrices[symbol] = price
            }
        }
    }
    
    // MARK: - Price Access
    func getPrice(for symbol: String) -> Double {
        return tokenPrices[symbol] ?? 0.0
    }
    
    func getFormattedPrice(for symbol: String) -> String {
        let price = getPrice(for: symbol)
        
        if price >= 1.0 {
            return String(format: "$%.2f", price)
        } else if price >= 0.01 {
            return String(format: "$%.4f", price)
        } else {
            return String(format: "$%.8f", price)
        }
    }
    
    func getLogoURL(for symbol: String) -> String {
        return tokenLogoURLs[symbol] ?? ""
    }
    
    // MARK: - Cache Management
    private func saveCachedPrices() {
        if let data = try? JSONSerialization.data(withJSONObject: tokenPrices) {
            UserDefaults.standard.set(data, forKey: "CachedTokenPrices")
            UserDefaults.standard.set(Date(), forKey: "LastPriceUpdate")
        }
    }
    
    private func loadCachedPrices() {
        if let data = UserDefaults.standard.data(forKey: "CachedTokenPrices"),
           let cached = try? JSONSerialization.jsonObject(with: data) as? [String: Double] {
            tokenPrices = cached
        }
        
        if let lastUpdate = UserDefaults.standard.object(forKey: "LastPriceUpdate") as? Date {
            self.lastUpdate = lastUpdate
        }
    }
    
    // MARK: - Refresh
    func refreshPrices() {
        fetchAllPrices()
    }
}

// MARK: - Token Logo Service
class TokenLogoService: ObservableObject {
    @Published var tokenLogos: [String: String] = [:]
    
    init() {
        loadTokenLogos()
    }
    
    private func loadTokenLogos() {
        // USDTgVerse Native Token Logos
        tokenLogos["USDTg"] = "usdtg_logo"
        tokenLogos["USDTgV"] = "usdtgv_logo" 
        tokenLogos["USDTgG"] = "usdtgg_logo"
        
        // External Token Logos (using SF Symbols or custom icons)
        tokenLogos["USDT"] = "dollarsign.circle.fill"
        tokenLogos["USDC"] = "dollarsign.circle"
        tokenLogos["BTC"] = "bitcoinsign.circle.fill"
        tokenLogos["ETH"] = "ethereum.circle.fill"
        tokenLogos["BNB"] = "b.circle.fill"
        tokenLogos["SOL"] = "s.circle.fill"
        tokenLogos["TRX"] = "t.circle.fill"
        tokenLogos["MATIC"] = "m.circle.fill"
        tokenLogos["AVAX"] = "a.circle.fill"
        tokenLogos["ADA"] = "a.circle"
        tokenLogos["DOT"] = "d.circle.fill"
        tokenLogos["LINK"] = "link.circle.fill"
        tokenLogos["UNI"] = "u.circle.fill"
        tokenLogos["LTC"] = "l.circle.fill"
        tokenLogos["BCH"] = "b.circle"
        tokenLogos["XRP"] = "x.circle.fill"
        tokenLogos["DOGE"] = "d.circle"
        tokenLogos["SHIB"] = "s.circle"
    }
    
    func getLogo(for symbol: String) -> String {
        return tokenLogos[symbol] ?? "questionmark.circle"
    }
    
    func getLogoColor(for symbol: String) -> Color {
        switch symbol {
        case "USDTg", "USDTgV", "USDTgG":
            return .green
        case "USDT", "USDC":
            return .blue
        case "BTC":
            return .orange
        case "ETH":
            return .purple
        case "BNB":
            return .yellow
        case "SOL":
            return .purple
        case "TRX":
            return .red
        case "MATIC":
            return .purple
        case "AVAX":
            return .red
        case "ADA":
            return .blue
        case "DOT":
            return .pink
        case "LINK":
            return .blue
        case "UNI":
            return .pink
        case "LTC":
            return .gray
        case "BCH":
            return .orange
        case "XRP":
            return .blue
        case "DOGE":
            return .yellow
        case "SHIB":
            return .orange
        default:
            return .gray
        }
    }
}
