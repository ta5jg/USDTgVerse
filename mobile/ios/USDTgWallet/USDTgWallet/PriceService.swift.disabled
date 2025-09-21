//
//  PriceService.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import Foundation
import SwiftUI

// MARK: - Asset Model
struct CryptoAsset: Identifiable {
    let id = UUID()
    let symbol: String
    let name: String
    let balance: Double
    let price: Double
    let logoName: String // For local assets
    let logoURL: String? // For remote assets
    let isNative: Bool // USDTg is our native token
    
    var value: Double {
        return balance * price
    }
    
    var formattedBalance: String {
        if balance >= 1000000 {
            return String(format: "%.2fM", balance / 1000000)
        } else if balance >= 1000 {
            return String(format: "%.2fK", balance / 1000)
        } else {
            return String(format: "%.2f", balance)
        }
    }
    
    var formattedValue: String {
        return String(format: "$%.2f", value)
    }
    
    var formattedPrice: String {
        if price >= 1000 {
            return String(format: "$%.0f", price)
        } else {
            return String(format: "$%.2f", price)
        }
    }
}

// MARK: - Price Service
class PriceService: ObservableObject {
    @Published var assets: [CryptoAsset] = []
    @Published var isLoading = false
    @Published var lastUpdated: Date = Date()
    
    // Native USDTg configuration
    private let usdtgConfig = (
        symbol: "USDTG",
        name: "USDTg",
        balance: 10000.00,
        price: 1.00, // Fixed $1.00 peg
        logoName: "usdtg_logo",
        isNative: true
    )
    
    init() {
        loadAssets()
        startPriceUpdates()
    }
    
    func loadAssets() {
        // Native USDTg (always first, always $1.00)
        let usdtg = CryptoAsset(
            symbol: usdtgConfig.symbol,
            name: usdtgConfig.name,
            balance: usdtgConfig.balance,
            price: usdtgConfig.price,
            logoName: usdtgConfig.logoName,
            logoURL: nil,
            isNative: usdtgConfig.isNative
        )
        
        // Other cryptocurrencies with mock prices (in real app, fetch from API)
        let otherAssets = [
            CryptoAsset(
                symbol: "USDT",
                name: "Tether",
                balance: 5000.00,
                price: 1.00,
                logoName: "usdt_logo",
                logoURL: "https://cryptologos.cc/logos/tether-usdt-logo.png",
                isNative: false
            ),
            CryptoAsset(
                symbol: "USDC",
                name: "USD Coin",
                balance: 2500.00,
                price: 1.00,
                logoName: "usdc_logo",
                logoURL: "https://cryptologos.cc/logos/usd-coin-usdc-logo.png",
                isNative: false
            ),
            CryptoAsset(
                symbol: "BTC",
                name: "Bitcoin",
                balance: 0.15,
                price: 45000.00,
                logoName: "btc_logo",
                logoURL: "https://cryptologos.cc/logos/bitcoin-btc-logo.png",
                isNative: false
            ),
            CryptoAsset(
                symbol: "ETH",
                name: "Ethereum",
                balance: 1.5,
                price: 2337.85,
                logoName: "eth_logo",
                logoURL: "https://cryptologos.cc/logos/ethereum-eth-logo.png",
                isNative: false
            ),
            CryptoAsset(
                symbol: "TRX",
                name: "TRON",
                balance: 500.00,
                price: 0.091,
                logoName: "trx_logo",
                logoURL: "https://cryptologos.cc/logos/tron-trx-logo.png",
                isNative: false
            )
        ]
        
        assets = [usdtg] + otherAssets
        lastUpdated = Date()
    }
    
    private func startPriceUpdates() {
        // Simulate price updates every 30 seconds (except USDTg which stays $1.00)
        Timer.scheduledTimer(withTimeInterval: 30.0, repeats: true) { _ in
            self.updatePrices()
        }
    }
    
    private func updatePrices() {
        isLoading = true
        
        // Simulate API call delay
        DispatchQueue.main.asyncAfter(deadline: .now() + 1.0) {
            for i in 0..<self.assets.count {
                if !self.assets[i].isNative {
                    // Add small random price variation (±2%)
                    let variation = Double.random(in: -0.02...0.02)
                    let newPrice = self.assets[i].price * (1 + variation)
                    
                    self.assets[i] = CryptoAsset(
                        symbol: self.assets[i].symbol,
                        name: self.assets[i].name,
                        balance: self.assets[i].balance,
                        price: max(0.001, newPrice), // Prevent negative prices
                        logoName: self.assets[i].logoName,
                        logoURL: self.assets[i].logoURL,
                        isNative: self.assets[i].isNative
                    )
                }
                // USDTg always remains $1.00 (native token with fixed peg)
            }
            
            self.lastUpdated = Date()
            self.isLoading = false
        }
    }
    
    var totalPortfolioValue: Double {
        return assets.reduce(0) { $0 + $1.value }
    }
    
    var formattedTotalValue: String {
        return String(format: "$%.2f", totalPortfolioValue)
    }
}

// MARK: - Asset Logo View
struct AssetLogo: View {
    let asset: CryptoAsset
    let size: CGFloat
    
    var body: some View {
        Group {
            if asset.isNative {
                // Use our native USDTg logo
                Image("usdtg_logo")
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: size, height: size)
                    .clipShape(Circle())
            } else {
                // For other tokens, use text emoji for now (in production, load from URL)
                Circle()
                    .fill(tokenColor(for: asset.symbol))
                    .frame(width: size, height: size)
                    .overlay(
                        Text(tokenEmoji(for: asset.symbol))
                            .font(.system(size: size * 0.5))
                    )
            }
        }
    }
    
    private func tokenEmoji(for symbol: String) -> String {
        switch symbol {
        case "USDT": return "₮"
        case "USDC": return "©"
        case "BTC": return "₿"
        case "ETH": return "Ξ"
        case "TRX": return "♦"
        default: return "?"
        }
    }
    
    private func tokenColor(for symbol: String) -> Color {
        switch symbol {
        case "USDT": return .green
        case "USDC": return .blue
        case "BTC": return .orange
        case "ETH": return .purple
        case "TRX": return .red
        default: return .gray
        }
    }
}

// MARK: - Price Change Indicator
struct PriceChangeIndicator: View {
    let currentPrice: Double
    let previousPrice: Double
    
    private var priceChange: Double {
        return currentPrice - previousPrice
    }
    
    private var priceChangePercentage: Double {
        guard previousPrice > 0 else { return 0 }
        return (priceChange / previousPrice) * 100
    }
    
    private var isPositive: Bool {
        return priceChange >= 0
    }
    
    var body: some View {
        HStack(spacing: 4) {
            Image(systemName: isPositive ? "arrow.up" : "arrow.down")
                .font(.caption)
                .foregroundColor(isPositive ? .green : .red)
            
            Text(String(format: "%.2f%%", abs(priceChangePercentage)))
                .font(.caption)
                .foregroundColor(isPositive ? .green : .red)
        }
    }
}
