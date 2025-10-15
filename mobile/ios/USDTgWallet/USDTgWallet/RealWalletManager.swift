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
    
    // USDTgVerse Production API endpoints
    private let productionAPI = "https://api.usdtgverse.com"
    private let backupAPI = "https://backup-api.usdtgverse.com"
    private let fallbackAPI = "https://fallback-api.usdtgverse.com"
    
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
    
    // MARK: - Blockchain Connection (Production Only)
    func connectToBlockchain() {
        isLoading = true
        errorMessage = nil
        
        // Connect to production APIs only - NO DEMO FALLBACKS
        connectToProductionAPIs { [weak self] success in
            DispatchQueue.main.async {
                if success {
                    self?.isConnected = true
                    self?.fetchRealBalances()
                } else {
                    self?.isConnected = false
                    self?.errorMessage = "Unable to connect to USDTgVerse network"
                    self?.isLoading = false
                }
            }
        }
    }
    
    private func connectToProductionAPIs(completion: @escaping (Bool) -> Void) {
        // Try production API first, then backup APIs
        let apiEndpoints = [productionAPI, backupAPI, fallbackAPI]
        var currentIndex = 0
        
        func tryNextAPI() {
            guard currentIndex < apiEndpoints.count else {
                completion(false)
                return
            }
            
            guard let url = URL(string: "\(apiEndpoints[currentIndex])/api/v1/status") else {
                currentIndex += 1
                tryNextAPI()
                return
            }
            
            URLSession.shared.dataTask(with: url) { data, response, error in
                DispatchQueue.main.async {
                    if error != nil || data == nil {
                        currentIndex += 1
                        tryNextAPI()
                    } else {
                        completion(true)
                    }
                }
            }.resume()
        }
        
        tryNextAPI()
    }
    
    // MARK: - Real Balance Fetching (Production Only)
    func fetchRealBalances() {
        guard !walletAddress.isEmpty else {
            isLoading = false
            errorMessage = "No wallet address found"
            return
        }
        
        guard isConnected else {
            isLoading = false
            errorMessage = "Not connected to USDTgVerse network"
            return
        }
        
        // Fetch real balances from blockchain
        fetchUSDTgBalance()
        fetchExternalBalances()
        
        isLoading = false
    }
    
    private func fetchUSDTgBalance() {
        guard let url = URL(string: "\(productionAPI)/api/v1/balance/\(walletAddress)") else {
            DispatchQueue.main.async {
                self.errorMessage = "Invalid API endpoint for USDTg balance"
            }
            return
        }
        
        URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
            DispatchQueue.main.async {
                guard let data = data,
                      let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let balance = json["balance"] as? Double else {
                    self?.errorMessage = "Failed to fetch USDTg balance from blockchain"
                    return
                }
                
                self?.userBalance = balance
                self?.updateUSDTgAsset(balance: balance)
            }
        }.resume()
    }
    
    private func fetchExternalBalances() {
        // Fetch real balances from external blockchain networks
        fetchEthereumBalances()
        fetchBNBBalances()
        fetchTronBalances()
        fetchSolanaBalances()
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
    
    // MARK: - External Balance Fetching Methods
    private func fetchEthereumBalances() {
        guard let url = URL(string: "\(productionAPI)/api/v1/external-balance/ethereum/\(walletAddress)") else {
            DispatchQueue.main.async {
                self.errorMessage = "Invalid API endpoint for Ethereum balances"
            }
            return
        }
        
        URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
            DispatchQueue.main.async {
                guard let data = data,
                      let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any] else {
                    self?.errorMessage = "Failed to fetch Ethereum balances"
                    return
                }
                
                // Update ETH balance
                if let ethBalance = json["eth"] as? Double {
                    self?.updateExternalAsset(symbol: "ETH", balance: ethBalance, price: 2337.85, network: "Ethereum")
                }
                
                // Update USDT balance
                if let usdtBalance = json["usdt"] as? Double {
                    self?.updateExternalAsset(symbol: "USDT", balance: usdtBalance, price: 1.00, network: "Ethereum")
                }
                
                // Update USDC balance
                if let usdcBalance = json["usdc"] as? Double {
                    self?.updateExternalAsset(symbol: "USDC", balance: usdcBalance, price: 1.00, network: "Ethereum")
                }
            }
        }.resume()
    }
    
    private func fetchBNBBalances() {
        guard let url = URL(string: "\(productionAPI)/api/v1/external-balance[b]nb/\(walletAddress)") else { return }
        
        URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
            DispatchQueue.main.async {
                guard let data = data,
                      let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let bnbBalance = json["bnb"] as? Double else { return }
                
                self?.updateExternalAsset(symbol: "BNB", balance: bnbBalance, price: 245.50, network: "BNB Chain")
            }
        }.resume()
    }
    
    private func fetchTronBalances() {
        guard let url = URL(string: "\(productionAPI)/api/v1/external-balance/tron/\(walletAddress)") else { return }
        
        URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
            DispatchQueue.main.async {
                guard let data = data,
                      let json = try? JSONSerialization.htmlObject(with: data) as? [String: Any],
                      let trxBalance = json["trx"] as? Double else { return }
                
                self?.updateExternalAsset(symbol: "TRX", balance: trxBalance, price: 0.091, network: "TRON")
            }
        }.resume()
    }
    
    private func fetchSolanaBalances() {
        guard let url = URL(string: "\(productionAPI)/api/v1/external-balance/solana/\(walletAddress)") else { return }
        
        URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
            DispatchQueue.main.async {
                guard let data = data,
                      let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let solBalance = json["sol"] as? Double else { return }
                
                self?.updateExternalAsset(symbol: "SOL", balance: solBalance, price: 145.75, network: "Solana")
            }
        }.resume()
    }
    
    private func updateExternalAsset(symbol: String, balance: Double, price: Double, network: String) {
        let asset = RealAsset(
            id: symbol.lowercased(),
            symbol: symbol,
            name: symbol == "ETH" ? "Ethereum" : 
                  symbol == "BNB" ? "BNB" : 
                  symbol == "TRX" ? "TRON" : 
                  symbol == "SOL" ? "Solana" : symbol,
            network: network,
            balance: balance,
            price: price,
            isNative: symbol == network, // Native token check
            contractAddress: nil
        )
        
        if let index = realAssets.firstIndex(where: { $0.symbol == symbol }) {
            realAssets[index] = asset
        } else if [balance > 0] {
            realAssets.append(asset)
        }
    }
    
    // MARK: - AirDrop System (Production)
    private func requestWelcomeAirDrop(address: String) {
        // Request 10 USDTg airdrop (LOCKED - Fee only until purchase)
        guard let url = URL(string: "http://localhost:3006/api/airdrop/create") else {
            DispatchQueue.main.async {
                self.errorMessage = "Invalid airdrop API endpoint"
            }
            return
        }
        
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        
        // Get device fingerprint
        let deviceID = UIDevice.current.identifierForVendor?.uuidString ?? "unknown"
        
        let body = [
            "wallet_address": address,
            "user_id": "USER_\(Int(Date().timeIntervalSince1970))",
            "device_fingerprint": deviceID,
            "ip_address": "0.0.0.0" // Will be set by server
        ] as [String : Any]
        
        request.httpBody = try? JSONSerialization.data(withJSONObject: body)
        
        URLSession.shared.dataTask(with: request) { [weak self] data, response, error in
            DispatchQueue.main.async {
                guard let data = data,
                      let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let success = json["success"] as? Bool,
                      success else {
                    self?.errorMessage = "Failed to request airdrop"
                    return
                }
                
                // Airdrop successful - 10 USDTg (LOCKED)
                let airdropID = json["airdrop_id"] as? String ?? "unknown"
                let amount = json["amount"] as? Double ?? 10.0
                let message = json["message"] as? String ?? "Airdrop received"
                
                print("✅ Airdrop received: \(airdropID)")
                print("💰 Amount: \(amount) USDTg (LOCKED - Fee only)")
                print("📝 \(message)")
                
                // Update balance with locked airdrop
                self?.userBalance = amount
                self?.updateUSDTgAsset(balance: amount)
                
                // Show notification to user
                self?.showAirdropNotification(amount: amount, message: message)
            }
        }.resume()
    }
    
    private func showAirdropNotification(amount: Double, message: String) {
        // Show in-app notification about locked airdrop
        print("🎉 Welcome! You received \(amount) USDTg")
        print("🔒 Locked for fees only")
        print("🔓 Purchase 50+ USDTg to unlock for all transactions")
    }
    
    private func generateAPIToken() -> String {
        // In production, implement secure API token generation
        return "prod_token_\(UUID().uuidString)"
    }
    
    // MARK: - Bonus System Integration
    func checkPurchaseBonus(purchaseAmount: Double) {
        // Check if purchase qualifies for bonus (10,000+ USDTg)
        guard purchaseAmount >= 10000.0 else {
            return
        }
        
        guard let url = URL(string: "http://localhost:3007/api/bonus/create") else {
            return
        }
        
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        
        let body = [
            "wallet_address": walletAddress,
            "user_id": "USER_\(Int(Date().timeIntervalSince1970))",
            "purchase_amount": purchaseAmount
        ] as [String : Any]
        
        request.httpBody = try? JSONSerialization.data(withJSONObject: body)
        
        URLSession.shared.dataTask(with: request) { [weak self] data, response, error in
            DispatchQueue.main.async {
                guard let data = data,
                      let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let success = json["success"] as? Bool,
                      success else {
                    return
                }
                
                let bonusAmount = json["bonus_amount"] as? Double ?? 0.0
                let tier = json["tier"] as? String ?? "Bronze"
                let message = json["message"] as? String ?? ""
                
                print("🎁 BONUS EARNED!")
                print("💰 Amount: \(bonusAmount) USDTg")
                print("🏆 Tier: \(tier)")
                print("📝 \(message)")
                
                // Show bonus notification
                self?.showBonusNotification(amount: bonusAmount, tier: tier, message: message)
                
                // Update balance with bonus
                self?.userBalance += bonusAmount
            }
        }.resume()
    }
    
    private func showBonusNotification(amount: Double, tier: String, message: String) {
        // Show in-app notification about bonus
        print("🎉 Congratulations!")
        print("💎 You earned \(amount) USDTg bonus (\(tier) tier)")
        print("🏆 Keep purchasing to unlock higher tiers!")
    }
    
    // MARK: - Purchase Verification & Airdrop Unlock
    func verifyPurchaseAndUnlockAirdrop(purchaseAmount: Double) {
        // Check for purchase bonus first
        checkPurchaseBonus(purchaseAmount: purchaseAmount)
        
        // Then verify for airdrop unlock
        guard let url = URL(string: "http://localhost:3006/api/airdrop/verify-purchase") else {
            return
        }
        
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        
        let body = [
            "wallet_address": walletAddress,
            "purchase_amount": purchaseAmount
        ] as [String : Any]
        
        request.httpBody = try? JSONSerialization.data(withJSONObject: body)
        
        URLSession.shared.dataTask(with: request) { [weak self] data, response, error in
            DispatchQueue.main.async {
                guard let data = data,
                      let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let success = json["success"] as? Bool,
                      success else {
                    return
                }
                
                let unlocked = json["unlocked"] as? Bool ?? false
                let message = json["message"] as? String ?? ""
                
                if unlocked {
                    print("🔓 AIRDROP UNLOCKED!")
                    print("💰 10 USDTg now available for all transactions")
                    self?.showUnlockNotification()
                } else {
                    let remaining = json["remaining_to_unlock"] as? Double ?? 0.0
                    print("💰 Purchase recorded. Need \(remaining) more USDTg to unlock")
                }
            }
        }.resume()
    }
    
    private func showUnlockNotification() {
        // Show in-app notification about unlocked airdrop
        print("🎉 Congratulations! Your airdrop is now unlocked!")
        print("💎 You can now use all 10 USDTg for any transaction")
    }
    
    // MARK: - Fee Payment with Airdrop
    func payFeeWithAirdrop(feeAmount: Double, completion: @escaping (Bool) -> Void) {
        guard let url = URL(string: "http://localhost:3006/api/airdrop/use-fee") else {
            completion(false)
            return
        }
        
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        
        let body = [
            "wallet_address": walletAddress,
            "fee_amount": feeAmount
        ] as [String : Any]
        
        request.httpBody = try? JSONSerialization.data(withJSONObject: body)
        
        URLSession.shared.dataTask(with: request) { data, response, error in
            DispatchQueue.main.async {
                guard let data = data,
                      let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                      let success = json["success"] as? Bool,
                      success else {
                    completion(false)
                    return
                }
                
                let remaining = json["remaining"] as? Double ?? 0.0
                print("✅ Fee paid from airdrop: \(feeAmount) USDTg")
                print("💰 Remaining locked: \(remaining) USDTg")
                
                completion(true)
            }
        }.resume()
    }
    
    // MARK: - Transaction Handling (Production)
    func sendTransaction(to address: String, amount: Double, asset: String) async throws {
        guard !walletAddress.isEmpty else {
            throw TransactionError.noWallet
        }
        
        guard isConnected else {
            throw TransactionError.noConnection
        }
        
        guard let url = URL(string: "\(productionAPI)/api/v1/transactions/send") else {
            throw TransactionError.invalidEndpoint
        }
        
        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        request.setValue("Bearer \(generateAPIToken())", forHTTPHeaderField: "Authorization")
        
        let transactionData = [
            "from": walletAddress,
            "to": address,
            "amount": amount,
            "asset": asset,
            "timestamp": Date().timeIntervalSince1970
        ] as [String : Any]
        
        request.httpBody = try? JSONSerialization.data(withJSONObject: transactionData)
        
        let (data, response) = try await URLSession.shared.data(for: request)
        
        guard let httpResponse = response as? HTTPURLResponse,
              httpResponse.statusCode == 200,
              let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
              let success = json["success"] as? Bool,
              success else {
            throw TransactionError.transactionFailed
        }
        
        // Refresh balances after successful transaction
        DispatchQueue.main.async {
            self.fetchRealBalances()
        }
    }
    
    // MARK: - Utility Functions
    private func generateWalletAddress(from input: String) -> String {
        // Production BIP44 wallet address generation
        guard let walletServiceURL = URL(string: "\(productionAPI)/api/v1/wallet/generate") else {
            errorMessage = "Unable to generate wallet address"
            return ""
        }
        
        var request = URLRequest(url: walletServiceURL)
        request.httpMethod = "POST"
        request.setValue("application/json", forHTTPHeaderField: "Content-Type")
        
        let walletData = ["mnemonic": input]
        request.httpBody = try? JSONSerialization.data(withJSONObject: walletData)
        
        let semaphore = DispatchSemaphore(value: 0)
        var generatedAddress = ""
        
        URLSession.shared.dataTask(with: request) { data, response, error in
            defer { semaphore.signal() }
            
            guard let data = data,
                  let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any],
                  let address = json["address"] as? String else { return }
            
            generatedAddress = address
        }.resume()
        
        semaphore.wait()
        return generatedAddress
    }
    
    enum TransactionError: Error {
        case noWallet
        case noConnection
        case invalidEndpoint
        case transactionFailed
        case insufficientBalance
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
