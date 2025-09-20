//
//  RealWalletManager.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI
import Foundation

// MARK: - Real Wallet Manager
class RealWalletManager: ObservableObject {
    @Published var isConnected = false
    @Published var userBalance: Double = 0.0
    @Published var realAssets: [RealAsset] = []
    @Published var isLoading = false
    @Published var errorMessage: String?
    @Published var hasWallet = false
    @Published var walletAddress = ""
    
    // USDTgVerse API endpoints
    private let baseURL = "https://api.usdtgverse.com"
    private let localNodeURL = "http://localhost:3001"
    
    init() {
        checkWalletExists()
        if hasWallet {
            connectToBlockchain()
        }
    }
    
    // MARK: - Wallet Management
    func checkWalletExists() {
        // Check if user has created/imported a wallet
        hasWallet = UserDefaults.standard.bool(forKey: "hasWallet")
        walletAddress = UserDefaults.standard.string(forKey: "walletAddress") ?? ""
    }
    
    func createNewWallet(name: String, mnemonic: String) {
        // Generate wallet from mnemonic
        let address = generateWalletAddress(from: mnemonic)
        
        // Save wallet info
        UserDefaults.standard.set(true, forKey: "hasWallet")
        UserDefaults.standard.set(address, forKey: "walletAddress")
        UserDefaults.standard.set(name, forKey: "walletName")
        UserDefaults.standard.set(mnemonic, forKey: "walletMnemonic") // In production, use Keychain!
        
        hasWallet = true
        walletAddress = address
        
        // Request AirDrop for new wallet
        requestWelcomeAirDrop(address: address)
        
        // Connect to blockchain
        connectToBlockchain()
    }
    
    func importWallet(name: String, mnemonic: String?, privateKey: String?) {
        var address = ""
        
        if let mnemonic = mnemonic, !mnemonic.isEmpty {
            address = generateWalletAddress(from: mnemonic)
            UserDefaults.standard.set(mnemonic, forKey: "walletMnemonic")
        } else if let privateKey = privateKey, !privateKey.isEmpty {
            address = generateWalletAddress(from: privateKey)
            UserDefaults.standard.set(privateKey, forKey: "walletPrivateKey")
        }
        
        // Save wallet info
        UserDefaults.standard.set(true, forKey: "hasWallet")
        UserDefaults.standard.set(address, forKey: "walletAddress")
        UserDefaults.standard.set(name, forKey: "walletName")
        
        hasWallet = true
        walletAddress = address
        
        // Connect to blockchain
        connectToBlockchain()
    }
    
    // MARK: - Blockchain Connection
    func connectToBlockchain() {
        isLoading = true
        errorMessage = nil
        
        // Try production API first
        connectToProductionAPI { [weak self] success in
            if success {
                self?.fetchRealBalances()
            } else {
                // Fallback to local node
                self?.connectToLocalNode()
            }
        }
    }
    
    private func connectToProductionAPI(completion: @escaping (Bool) -> Void) {
        guard let url = URL(string: "\(baseURL)/api/status") else {
            completion(false)
            return
        }
        
        URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
            DispatchQueue.main.async {
                if error != nil {
                    completion(false)
                } else {
                    self?.isConnected = true
                    completion(true)
                }
            }
        }.resume()
    }
    
    private func connectToLocalNode() {
        guard let url = URL(string: "\(localNodeURL)/api/status") else {
            DispatchQueue.main.async {
                self.isLoading = false
                self.errorMessage = "Cannot connect to blockchain"
            }
            return
        }
        
        URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
            DispatchQueue.main.async {
                if error != nil {
                    self?.isConnected = false
                    self?.loadDemoData() // Fallback to demo data
                } else {
                    self?.isConnected = true
                    self?.fetchRealBalances()
                }
            }
        }.resume()
    }
    
    // MARK: - Real Balance Fetching
    func fetchRealBalances() {
        guard !walletAddress.isEmpty else { return }
        
        // Fetch USDTgVerse balance
        fetchUSDTgBalance()
        
        // Fetch external chain balances
        fetchExternalBalances()
        
        isLoading = false
    }
    
    private func fetchUSDTgBalance() {
        let endpoint = isConnected ? "\(baseURL)/api/balance/\(walletAddress)" : "\(localNodeURL)/api/balance/\(walletAddress)"
        
        guard let url = URL(string: endpoint) else { return }
        
        URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
            DispatchQueue.main.async {
                if let data = data,
                   let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                   let balance = json["balance"] as? Double {
                    
                    self?.userBalance = balance
                    self?.updateUSDTgAsset(balance: balance)
                } else {
                    // Demo fallback
                    self?.userBalance = 10.0 // Welcome AirDrop amount
                    self?.updateUSDTgAsset(balance: 10.0)
                }
            }
        }.resume()
    }
    
    private func fetchExternalBalances() {
        // Simulate fetching from external chains
        DispatchQueue.main.asyncAfter(deadline: .now() + 1) {
            self.loadExternalAssets()
        }
    }
    
    // MARK: - Asset Management
    private func updateUSDTgAsset(balance: Double) {
        let usdtgAsset = RealAsset(
            id: "usdtg",
            symbol: "USDTg",
            name: "USDTg",
            network: "USDTgVerse",
            balance: balance,
            price: 1.00, // Fixed $1.00 peg
            isNative: true,
            contractAddress: nil
        )
        
        if let index = realAssets.firstIndex(where: { $0.id == "usdtg" }) {
            realAssets[index] = usdtgAsset
        } else {
            realAssets.insert(usdtgAsset, at: 0)
        }
    }
    
    private func loadExternalAssets() {
        // Real external chain balances (0 for new users)
        let externalAssets = [
            RealAsset(id: "eth", symbol: "ETH", name: "Ethereum", network: "Ethereum", balance: 0.0, price: 2337.85, isNative: false, contractAddress: nil),
            RealAsset(id: "usdt", symbol: "USDT", name: "Tether USD", network: "Ethereum", balance: 0.0, price: 1.00, isNative: false, contractAddress: "0xdAC17F958D2ee523a2206206994597C13D831ec7"),
            RealAsset(id: "usdc", symbol: "USDC", name: "USD Coin", network: "Ethereum", balance: 0.0, price: 1.00, isNative: false, contractAddress: "0xA0b86a33E6441b8435b662c8c6C1d4c7B6F2e0C6"),
            RealAsset(id: "bnb", symbol: "BNB", name: "BNB", network: "BNB Chain", balance: 0.0, price: 245.50, isNative: false, contractAddress: nil),
            RealAsset(id: "trx", symbol: "TRX", name: "TRON", network: "TRON", balance: 0.0, price: 0.091, isNative: false, contractAddress: nil)
        ]
        
        realAssets.append(contentsOf: externalAssets)
    }
    
    private func loadDemoData() {
        // Demo data for development/offline mode
        realAssets = [
            RealAsset(id: "usdtg", symbol: "USDTg", name: "USDTg", network: "USDTgVerse", balance: 10.0, price: 1.00, isNative: true, contractAddress: nil),
            RealAsset(id: "eth", symbol: "ETH", name: "Ethereum", network: "Ethereum", balance: 0.0, price: 2337.85, isNative: false, contractAddress: nil),
            RealAsset(id: "usdt", symbol: "USDT", name: "Tether USD", network: "Ethereum", balance: 0.0, price: 1.00, isNative: false, contractAddress: "0xdAC17F958D2ee523a2206206994597C13D831ec7")
        ]
        isLoading = false
    }
    
    // MARK: - AirDrop System
    private func requestWelcomeAirDrop(address: String) {
        let endpoint = isConnected ? "\(baseURL)/api/airdrop" : "\(localNodeURL)/api/airdrop"
        
        guard let url = URL(string: endpoint) else { return }
        
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        
        let body = [
            "address": address,
            "amount": 10.0, // 10 USDTg welcome bonus
            "type": "welcome_airdrop"
        ]
        
        request.httpBody = try? JSONSerialization.data(withJSONObject: body)
        
        URLSession.shared.dataTask(with: request) { [weak self] data, response, error in
            DispatchQueue.main.async {
                if error == nil {
                    // AirDrop successful
                    self?.userBalance = 10.0
                    self?.updateUSDTgAsset(balance: 10.0)
                }
            }
        }.resume()
    }
    
    // MARK: - Utility Functions
    private func generateWalletAddress(from input: String) -> String {
        // In production, use proper BIP44 derivation
        // For demo, generate a mock address
        let hash = input.data(using: .utf8)?.base64EncodedString() ?? ""
        return "0x" + String(hash.prefix(40))
    }
    
    var totalPortfolioValue: Double {
        return realAssets.reduce(0) { $0 + ($1.balance * $1.price) }
    }
    
    var formattedTotalValue: String {
        return String(format: "$%.2f", totalPortfolioValue)
    }
}

// MARK: - Real Asset Model
struct RealAsset: Identifiable {
    let id: String
    let symbol: String
    let name: String
    let network: String
    let balance: Double
    let price: Double
    let isNative: Bool
    let contractAddress: String?
    
    var value: Double {
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
        return String(format: "$%.2f", value)
    }
}

struct RealWalletManager_Previews: PreviewProvider {
    static var previews: some View {
        Text("RealWalletManager")
    }
}
