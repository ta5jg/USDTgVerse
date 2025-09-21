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
        if isDemo {
            return 48904.26 // Demo wallet balance
        } else {
            return 10.0 // New wallets get 10 USDTg
        }
    }
    
    var assets: [WalletAsset] {
        if isDemo {
            return [
                WalletAsset(symbol: "USDTg", name: "USDTg Native", balance: 10000.0, price: 1.0),
                WalletAsset(symbol: "USDT", name: "Tether USD", balance: 5432.1, price: 1.0),
                WalletAsset(symbol: "USDC", name: "USD Coin", balance: 2156.78, price: 1.0),
                WalletAsset(symbol: "ETH", name: "Ethereum", balance: 2.5, price: 2337.85),
                WalletAsset(symbol: "BNB", name: "BNB Chain", balance: 15.8, price: 245.5),
                WalletAsset(symbol: "SOL", name: "Solana", balance: 45.2, price: 145.75),
                WalletAsset(symbol: "TRX", name: "TRON", balance: 12500.0, price: 0.091),
                WalletAsset(symbol: "MATIC", name: "Polygon", balance: 8750.0, price: 0.89)
            ]
        } else {
            return [
                WalletAsset(symbol: "USDTg", name: "USDTg Native", balance: 10.0, price: 1.0),
                WalletAsset(symbol: "USDT", name: "Tether USD", balance: 0.0, price: 1.0),
                WalletAsset(symbol: "USDC", name: "USD Coin", balance: 0.0, price: 1.0),
                WalletAsset(symbol: "ETH", name: "Ethereum", balance: 0.0, price: 2337.85),
                WalletAsset(symbol: "BNB", name: "BNB Chain", balance: 0.0, price: 245.5)
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
