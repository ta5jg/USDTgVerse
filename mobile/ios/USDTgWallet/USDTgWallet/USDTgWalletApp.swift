//
//  USDTgWalletApp.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-19.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

@main
struct USDTgWalletApp: App {
    @StateObject private var walletManager = WalletManager()
    @StateObject private var networkManager = NetworkManager()
    
    var body: some Scene {
        WindowGroup {
            ContentView()
                .environmentObject(walletManager)
                .environmentObject(networkManager)
                .preferredColorScheme(.dark)
                .onAppear {
                    setupWallet()
                }
        }
    }
    
    private func setupWallet() {
        // Initialize USDTgVerse connection
        networkManager.connectToMainnet()
        
        // Load wallet data
        walletManager.initializeWallet()
        
        print("🌌 USDTgWallet iOS App Started")
        print("🔗 Connected to: https://api.usdtgverse.com")
        print("🌉 Bridge networks: 8 chains available")
        print("💳 OdixPay++ features: Enabled")
    }
}

// MARK: - Wallet Manager
class WalletManager: ObservableObject {
    @Published var isWalletCreated = false
    @Published var walletAddress = ""
    @Published var usdtgBalance: Double = 0.0
    @Published var assets: [WalletAsset] = []
    @Published var transactions: [Transaction] = []
    
    func initializeWallet() {
        // Load or create wallet
        if !isWalletCreated {
            createNewWallet()
        } else {
            loadExistingWallet()
        }
        
        loadAssets()
        loadTransactions()
    }
    
    private func createNewWallet() {
        // Generate new wallet address
        walletAddress = "usdtg1" + String(Int.random(in: 100000...999999))
        usdtgBalance = 10000.0 // Demo balance
        isWalletCreated = true
        
        print("💳 New wallet created: \(walletAddress)")
    }
    
    private func loadExistingWallet() {
        // Load existing wallet data
        print("📂 Loading existing wallet...")
    }
    
    private func loadAssets() {
        assets = [
            WalletAsset(symbol: "USDTg", name: "USDTg Native", balance: usdtgBalance, price: 1.00),
            WalletAsset(symbol: "USDT", name: "Tether USD", balance: 5432.10, price: 1.00),
            WalletAsset(symbol: "USDC", name: "USD Coin", balance: 2156.78, price: 1.00),
            WalletAsset(symbol: "ETH", name: "Ethereum", balance: 2.5, price: 2456.78),
            WalletAsset(symbol: "BNB", name: "BNB Chain", balance: 15.8, price: 635.45),
            WalletAsset(symbol: "SOL", name: "Solana", balance: 45.2, price: 142.33),
            WalletAsset(symbol: "TRX", name: "TRON", balance: 12500.0, price: 0.087),
            WalletAsset(symbol: "MATIC", name: "Polygon", balance: 8750.0, price: 0.87)
        ]
    }
    
    private func loadTransactions() {
        transactions = [
            Transaction(id: "tx001", type: .received, amount: 1000.0, asset: "USDTg", from: "usdtg1genesis...", timestamp: Date()),
            Transaction(id: "tx002", type: .sent, amount: 250.0, asset: "USDTg", to: "usdtg1bridge...", timestamp: Date().addingTimeInterval(-3600)),
            Transaction(id: "tx003", type: .bridge, amount: 1.5, asset: "ETH", from: "Ethereum Bridge", timestamp: Date().addingTimeInterval(-7200))
        ]
    }
}

// MARK: - Network Manager
class NetworkManager: ObservableObject {
    @Published var isConnected = false
    @Published var currentNetwork = "USDTgVerse Mainnet"
    @Published var blockHeight: UInt64 = 0
    @Published var networkStatus = "Live"
    
    private let apiBaseURL = "https://api.usdtgverse.com"
    
    func connectToMainnet() {
        // Connect to USDTgVerse mainnet
        isConnected = true
        blockHeight = UInt64.random(in: 100000...200000)
        
        // Start periodic updates
        Timer.scheduledTimer(withTimeInterval: 3.0, repeats: true) { _ in
            self.updateNetworkStatus()
        }
        
        print("🌐 Connected to USDTgVerse Mainnet")
        print("📊 API: \(apiBaseURL)")
    }
    
    private func updateNetworkStatus() {
        blockHeight += 1
        // Simulate network updates
    }
}

// MARK: - Data Models
struct WalletAsset: Identifiable {
    let id = UUID()
    let symbol: String
    let name: String
    let balance: Double
    let price: Double
    
    var totalValue: Double {
        return balance * price
    }
}

struct Transaction: Identifiable {
    let id: String
    let type: TransactionType
    let amount: Double
    let asset: String
    let from: String?
    let to: String?
    let timestamp: Date
    
    init(id: String, type: TransactionType, amount: Double, asset: String, from: String? = nil, to: String? = nil, timestamp: Date) {
        self.id = id
        self.type = type
        self.amount = amount
        self.asset = asset
        self.from = from
        self.to = to
        self.timestamp = timestamp
    }
}

enum TransactionType {
    case sent, received, bridge, odixpay
    
    var icon: String {
        switch self {
        case .sent: return "arrow.up.circle.fill"
        case .received: return "arrow.down.circle.fill"
        case .bridge: return "arrow.left.arrow.right.circle.fill"
        case .odixpay: return "creditcard.circle.fill"
        }
    }
    
    var color: Color {
        switch self {
        case .sent: return .red
        case .received: return .green
        case .bridge: return .blue
        case .odixpay: return .purple
        }
    }
}
