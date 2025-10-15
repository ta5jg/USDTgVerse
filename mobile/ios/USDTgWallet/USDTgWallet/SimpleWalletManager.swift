//
//  SimpleWalletManager.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI
import Foundation

// MARK: - Simple Wallet Data
struct SimpleWallet: Codable, Identifiable {
    let id: String
    let name: String
    let address: String
    let createdAt: Date
    let isDemo: Bool
    
    init(name: String, isDemo: Bool = false) {
        self.id = UUID().uuidString
        self.name = name
        self.address = "0x" + String(UUID().uuidString.replacingOccurrences(of: "-", with: "").prefix(40))
        self.createdAt = Date()
        self.isDemo = isDemo
    }
    
    var balance: Double {
        // Real balance from blockchain - no demo data
        return fetchRealBalanceFromBlockchain()
    }
    
    private func fetchRealBalanceFromBlockchain() -> Double {
        // Fetch real balance from USDTgVerse blockchain
        let semaphore = DispatchSemaphore(value: 0)
        var balance: Double = 0.0
        
        let apiURL = "https://api.usdtgverse.com/api/v1/balance/\(address)/usdtg"
        
        guard let url = URL(string: apiURL) else { return 0.0 }
        
        URLSession.shared.dataTask(with: url) { data, response, error in
            defer { semaphore.signal() }
            
            guard let data = data,
                  let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                  let fetchedBalance = json["balance"] as? Double else {
                return
            }
            
            balance = fetchedBalance
        }.resume()
        
        semaphore.wait()
        return balance
    }
    
    var assets: [WalletAsset] {
        if isDemo {
            return [
                WalletAsset(symbol: "USDTg", name: "USDTg Native", balance: 10000.0, price: 1.0,
                           logoURL: "usdtg_logo", change24h: 2.5, chain: "USDTgVerse", isNativeImage: true),
                WalletAsset(symbol: "RGLS", name: "Regilis", balance: 5000.0, price: 1.0,
                           logoURL: "regilis", change24h: 0.0, chain: "USDTgVerse", isNativeImage: true),
                WalletAsset(symbol: "USDT", name: "Tether USD", balance: 5432.1, price: 1.0,
                           logoURL: "https://assets.coingecko.com/coins/images/325/large/Tether.png", change24h: 0.1, chain: "Ethereum", isNativeImage: false),
                WalletAsset(symbol: "USDC", name: "USD Coin", balance: 2156.78, price: 1.0,
                           logoURL: "https://assets.coingecko.com/coins/images/6319/large/USD_Coin_icon.png", change24h: 0.0, chain: "Ethereum", isNativeImage: false),
                WalletAsset(symbol: "ETH", name: "Ethereum", balance: 2.5, price: 2337.85,
                           logoURL: "https://assets.coingecko.com/coins/images/279/large/ethereum.png", change24h: -1.2, chain: "Ethereum", isNativeImage: false),
                WalletAsset(symbol: "BNB", name: "BNB Chain", balance: 15.8, price: 245.5,
                           logoURL: "https://assets.coingecko.com/coins/images/825/large/bnb-icon2_2x.png", change24h: 3.4, chain: "BNB Chain", isNativeImage: false),
                WalletAsset(symbol: "SOL", name: "Solana", balance: 45.2, price: 145.75,
                           logoURL: "https://assets.coingecko.com/coins/images/4128/large/solana.png", change24h: 4.8, chain: "Solana", isNativeImage: false),
                WalletAsset(symbol: "TRX", name: "TRON", balance: 12500.0, price: 0.091,
                           logoURL: "https://assets.coingecko.com/coins/images/1094/large/tron-logo.png", change24h: -2.1, chain: "TRON", isNativeImage: false),
                WalletAsset(symbol: "MATIC", name: "Polygon", balance: 8750.0, price: 0.89,
                           logoURL: "https://assets.coingecko.com/coins/images/4713/large/matic-token-icon.png", change24h: -0.5, chain: "Polygon", isNativeImage: false)
            ]
        } else {
            return [
                WalletAsset(symbol: "USDTg", name: "USDTg Native", balance: 10.0, price: 1.0,
                           logoURL: "https://usdtgverse.com/assets/logos/USDTg-official.png", change24h: 2.5, chain: "USDTgVerse"),
                WalletAsset(symbol: "RGLS", name: "Regilis", balance: 0.0, price: 1.0,
                           logoURL: "https://usdtgverse.com/assets/logos/regilis.png", change24h: 0.0, chain: "USDTgVerse"),
                WalletAsset(symbol: "USDT", name: "Tether USD", balance: 0.0, price: 1.0,
                           logoURL: "https://assets.coingecko.com/coins/images/325/large/Tether.png", change24h: 0.1, chain: "Ethereum", isNativeImage: false),
                WalletAsset(symbol: "USDC", name: "USD Coin", balance: 0.0, price: 1.0,
                           logoURL: "https://assets.coingecko.com/coins/images/6319/large/USD_Coin_icon.png", change24h: 0.0, chain: "Ethereum", isNativeImage: false),
                WalletAsset(symbol: "ETH", name: "Ethereum", balance: 0.0, price: 2337.85,
                           logoURL: "https://assets.coingecko.com/coins/images/279/large/ethereum.png", change24h: -1.2, chain: "Ethereum", isNativeImage: false),
                WalletAsset(symbol: "BNB", name: "BNB Chain", balance: 0.0, price: 245.5,
                           logoURL: "https://assets.coingecko.com/coins/images/825/large/bnb-icon2_2x.png", change24h: 3.4, chain: "BNB Chain", isNativeImage: false)
            ]
        }
    }
    
    var totalValue: Double {
        return assets.reduce(0) { $0 + ($1.balance * $1.price) }
    }
}

// MARK: - Simple Wallet Asset
struct WalletAsset: Identifiable, Codable {
    let id = UUID()
    let symbol: String
    let name: String
    let balance: Double
    let price: Double
    
    var totalValue: Double {
        return balance * price
    }
    
    var formattedBalance: String {
        if symbol == "BTC" {
            return String(format: "%.8f", balance)
        } else if ["USDTg", "USDT", "USDC"].contains(symbol) {
            return String(format: "%.2f", balance)
        } else {
            return String(format: "%.4f", balance)
        }
    }
    
    var formattedValue: String {
        return String(format: "$%.2f", totalValue)
    }
}

// MARK: - Simple Wallet Manager
class SimpleWalletManager: ObservableObject {
    @Published var wallets: [SimpleWallet] = []
    @Published var currentWallet: SimpleWallet?
    
    private let walletsKey = "MultipleWallets"
    private let currentWalletKey = "CurrentWalletID"
    
    init() {
        loadWallets()
        
        // If no wallets exist, create demo wallet
        if wallets.isEmpty {
            createDemoWallet()
        }
        
        // Set current wallet
        if currentWallet == nil {
            currentWallet = wallets.first
        }
    }
    
    // MARK: - Wallet Management
    func createWallet(name: String) {
        let newWallet = SimpleWallet(name: name, isDemo: false)
        wallets.append(newWallet)
        currentWallet = newWallet
        saveWallets()
    }
    
    func importWallet(name: String, seedPhrase: String) {
        let importedWallet = SimpleWallet(name: name, isDemo: false)
        wallets.append(importedWallet)
        currentWallet = importedWallet
        saveWallets()
    }
    
    func switchToWallet(_ wallet: SimpleWallet) {
        currentWallet = wallet
        UserDefaults.standard.set(wallet.id, forKey: currentWalletKey)
    }
    
    func deleteWallet(_ wallet: SimpleWallet) {
        // Don't delete demo wallet
        guard !wallet.isDemo else { return }
        
        wallets.removeAll { $0.id == wallet.id }
        
        if currentWallet?.id == wallet.id {
            currentWallet = wallets.first { $0.isDemo } ?? wallets.first
        }
        
        saveWallets()
    }
    
    // MARK: - Data Persistence
    private func saveWallets() {
        do {
            let data = try JSONEncoder().encode(wallets)
            UserDefaults.standard.set(data, forKey: walletsKey)
            
            if let currentWallet = currentWallet {
                UserDefaults.standard.set(currentWallet.id, forKey: currentWalletKey)
            }
        } catch {
            print("Failed to save wallets: \(error)")
        }
    }
    
    private func loadWallets() {
        guard let data = UserDefaults.standard.data(forKey: walletsKey) else {
            return
        }
        
        do {
            wallets = try JSONDecoder().decode([SimpleWallet].self, from: data)
            
            // Load current wallet
            let currentWalletID = UserDefaults.standard.string(forKey: currentWalletKey)
            currentWallet = wallets.first { $0.id == currentWalletID } ?? wallets.first
        } catch {
            print("Failed to load wallets: \(error)")
        }
    }
    
    private func createDemoWallet() {
        let demoWallet = SimpleWallet(name: "USDTgVerse Wallet", isDemo: true)
        wallets.append(demoWallet)
        currentWallet = demoWallet
        saveWallets()
    }
    
    // MARK: - Computed Properties
    var hasWallets: Bool {
        return !wallets.isEmpty
    }
    
    var currentWalletName: String {
        return currentWallet?.name ?? "No Wallet"
    }
    
    var currentWalletBalance: Double {
        return currentWallet?.totalValue ?? 0.0
    }
    
    var currentWalletAssets: [WalletAsset] {
        return currentWallet?.assets ?? []
    }
}
