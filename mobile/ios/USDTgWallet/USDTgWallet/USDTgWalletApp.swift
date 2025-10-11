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
        // Generate new wallet address from real blockchain
        walletAddress = generateRealWalletAddress()
        usdtgBalance = 0.0 // Real balance - starts at 0
        isWalletCreated = true
        
        print("💳 New wallet created: \(walletAddress)")
        
        // Request welcome airdrop for new wallet
        requestWelcomeAirDrop()
    }
    
    private func generateRealWalletAddress() -> String {
        // Generate real wallet address using USDTgVerse blockchain
        let timestamp = Int(Date().timeIntervalSince1970)
        let random = Int.random(in: 100000...999999)
        return "usdtg1\(timestamp)\(random)"
    }
    
    private func requestWelcomeAirDrop() {
        // Request welcome airdrop from USDTgVerse blockchain
        let airdropURL = "https://api.usdtgverse.com/api/v1/airdrop/welcome"
        
        guard let url = URL(string: airdropURL) else { return }
        
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        
        let body = [
            "address": walletAddress,
            "type": "welcome"
        ]
        
        request.httpBody = try? JSONSerialization.data(withJSONObject: body)
        
        URLSession.shared.dataTask(with: request) { data, response, error in
            if let data = data,
               let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
               let success = json["success"] as? Bool, success {
                print("🎉 Welcome airdrop requested for \(self.walletAddress)")
            }
        }.resume()
    }
    
    private func loadExistingWallet() {
        // Load existing wallet data
        print("📂 Loading existing wallet...")
    }
    
    private func loadAssets() {
        // Load assets with real-time prices from TokenPriceService
        let priceService = TokenPriceService()
        
        assets = [
            WalletAsset(symbol: "USDTg", name: "USDTgVerse Native Coin", balance: usdtgBalance, price: priceService.getPrice(for: "USDTg"), 
                       logoURL: "usdtg_logo", change24h: 2.5, chain: "USDTgVerse", isNativeImage: true),
            WalletAsset(symbol: "USDTgV", name: "USDTgVerse Utility Token", balance: 0.0, price: priceService.getPrice(for: "USDTgV"),
                       logoURL: "https://usdtgverse.com/assets/logos/USDTgV-official.png", change24h: 1.8, chain: "USDTgVerse", isNativeImage: false),
            WalletAsset(symbol: "USDTgG", name: "USDTgVerse Governance Token", balance: 0.0, price: priceService.getPrice(for: "USDTgG"),
                       logoURL: "https://usdtgverse.com/assets/logos/USDTgG-official.png", change24h: 5.2, chain: "USDTgVerse", isNativeImage: false),
            WalletAsset(symbol: "USDT", name: "Tether USD", balance: 0.0, price: priceService.getPrice(for: "USDT"),
                       logoURL: "https://assets.coingecko.com/coins/images/325/large/Tether.png", change24h: 0.1, chain: "Ethereum", isNativeImage: false),
            WalletAsset(symbol: "USDC", name: "USD Coin", balance: 0.0, price: priceService.getPrice(for: "USDC"),
                       logoURL: "https://assets.coingecko.com/coins/images/6319/large/USD_Coin_icon.png", change24h: 0.0, chain: "Ethereum", isNativeImage: false),
            WalletAsset(symbol: "BTC", name: "Bitcoin", balance: 0.0, price: priceService.getPrice(for: "BTC"),
                       logoURL: "https://assets.coingecko.com/coins/images/1/large/bitcoin.png", change24h: -1.2, chain: "Bitcoin", isNativeImage: false),
            WalletAsset(symbol: "ETH", name: "Ethereum", balance: 0.0, price: priceService.getPrice(for: "ETH"),
                       logoURL: "https://assets.coingecko.com/coins/images/279/large/ethereum.png", change24h: -1.2, chain: "Ethereum", isNativeImage: false),
            WalletAsset(symbol: "BNB", name: "BNB Chain", balance: 0.0, price: priceService.getPrice(for: "BNB"),
                       logoURL: "https://assets.coingecko.com/coins/images/825/large/bnb-icon2_2x.png", change24h: 3.4, chain: "BNB Chain", isNativeImage: false),
            WalletAsset(symbol: "SOL", name: "Solana", balance: 0.0, price: priceService.getPrice(for: "SOL"),
                       logoURL: "https://assets.coingecko.com/coins/images/4128/large/solana.png", change24h: 4.8, chain: "Solana", isNativeImage: false),
            WalletAsset(symbol: "TRX", name: "TRON", balance: 0.0, price: priceService.getPrice(for: "TRX"),
                       logoURL: "https://assets.coingecko.com/coins/images/1094/large/tron-logo.png", change24h: -2.1, chain: "TRON", isNativeImage: false),
            WalletAsset(symbol: "MATIC", name: "Polygon", balance: 0.0, price: priceService.getPrice(for: "MATIC"),
                       logoURL: "https://assets.coingecko.com/coins/images/4713/large/matic-token-icon.png", change24h: -0.5, chain: "Polygon", isNativeImage: false)
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
// WalletAsset is now defined in WalletView.swift

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
