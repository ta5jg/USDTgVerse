//
//  WalletView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-19.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI
import CoreImage.CIFilterBuiltins

// MARK: - Wallet Asset Model (Inline)
struct WalletAsset: Identifiable, Codable {
    var id = UUID()
    let symbol: String
    let name: String
    let balance: Double
    let price: Double
    let logoURL: String
    let change24h: Double
    let chain: String
    let isNativeImage: Bool // For assets.xcassets images vs URLs
    
    var totalValue: Double {
        return balance * price
    }
    
    var formattedBalance: String {
        let formatter = NumberFormatter()
        formatter.numberStyle = .decimal
        formatter.locale = Locale.current
        
        if symbol == "BTC" {
            formatter.minimumFractionDigits = 8
            formatter.maximumFractionDigits = 8
        } else if ["USDTg", "USDT", "USDC", "BUSD"].contains(symbol) {
            formatter.minimumFractionDigits = 2
            formatter.maximumFractionDigits = 2
        } else {
            formatter.minimumFractionDigits = 2
            formatter.maximumFractionDigits = 8
        }
        
        // Remove trailing zeros for amounts
        if balance == floor(balance) && balance < 1000 {
            formatter.maximumFractionDigits = 0
        }
        
        return formatter.string(from: NSNumber(value: balance)) ?? "0"
    }
    
    var formattedValue: String {
        let formatter = NumberFormatter()
        formatter.numberStyle = .currency
        formatter.locale = Locale.current
        formatter.currencyCode = "USD"
        return formatter.string(from: NSNumber(value: totalValue)) ?? "$0.00"
    }
}

struct WalletView: View {
    @StateObject private var nativeWalletManager = NativeWalletManager()
    @EnvironmentObject var networkManager: NetworkManager
    @State private var showingSendView = false
    @State private var showingReceiveView = false
    @State private var showingSwapView = false
    @State private var showingWalletList = false
    @State private var showingNetworkSelector = false
    @State private var showingUSDTgVersePay = false
    @State private var showingMembership = false
    
    // Real-time portfolio value from NativeWalletManager
    private var totalPortfolioValue: Double {
        return nativeWalletManager.balance.totalPortfolioValue
    }
    @State private var showingMarginTrading = false
    @State private var showingCopyTrading = false
    @State private var showingCustody = false
    @State private var currentWalletName = "My Quantum Wallet"
    @State private var currentNetwork = "USDTgVerse"
    @State private var currentWalletAssets: [WalletAsset] = []
    
    
    // MARK: - Number Formatting
    private func formatCurrency(_ amount: Double) -> String {
        let formatter = NumberFormatter()
        formatter.numberStyle = .currency
        formatter.locale = Locale.current
        formatter.currencyCode = "USD"
        return formatter.string(from: NSNumber(value: amount)) ?? "$0.00"
    }
    
    private func formatBalance(_ amount: Double) -> String {
        let formatter = NumberFormatter()
        formatter.numberStyle = .decimal
        formatter.locale = Locale.current
        formatter.minimumFractionDigits = 2
        formatter.maximumFractionDigits = 8
        
        // Remove trailing zeros for crypto amounts
        if amount == floor(amount) && amount < 1000 {
            formatter.maximumFractionDigits = 0
        }
        
        return formatter.string(from: NSNumber(value: amount)) ?? "0"
    }
    
    var body: some View {
         NavigationView {
             VStack(spacing: 0) {
                 // Fixed Header (doesn't scroll)
                 VStack(spacing: 20) {
                     // Header with USDTgVerse branding
                     headerView
                     
                     // Portfolio Overview
                     portfolioCard
                     
                     // Quick Actions
                     quickActionsView
                 }
                 .padding()
                 .background(Color(.systemBackground))
                 
                 // Scrollable Content
                 ScrollView {
                     VStack(spacing: 20) {
                         // Assets List
                         assetsListView
                         
                         // Recent Transactions
                         recentTransactionsView
                     }
                     .padding()
                 }
             }
             .navigationBarHidden(true)
            .onAppear {
                // Load existing wallet or create new one with airdrop
                Task {
                    if !nativeWalletManager.hasWallets {
                        await nativeWalletManager.createWallet(name: "My Quantum Wallet")
                    }
                }
                loadCurrentWallet()
            }
            .background(
                LinearGradient(
                    gradient: Gradient(colors: [
                        Color(red: 0.1, green: 0.1, blue: 0.2),
                        Color(red: 0.2, green: 0.1, blue: 0.3)
                    ]),
                    startPoint: .topLeading,
                    endPoint: .bottomTrailing
                )
            )
        }
         .sheet(isPresented: $showingSendView) {
             BasicSendView()
         }
         .sheet(isPresented: $showingReceiveView) {
             BasicReceiveView()
         }
         .sheet(isPresented: $showingSwapView) {
             BasicSwapView()
         }
         .sheet(isPresented: $showingWalletList) {
             BasicWalletListSheet(
                 currentWalletName: $currentWalletName,
                 onWalletSelected: { walletName in
                     currentWalletName = walletName
                     UserDefaults.standard.set(walletName, forKey: "CurrentWallet")
                     loadCurrentWallet()
                 }
             )
         }
         .sheet(isPresented: $showingNetworkSelector) {
             NetworkSelectorSheet(
                 currentNetwork: $currentNetwork,
                 onNetworkSelected: { network in
                     currentNetwork = network
                     loadAssetsForNetwork(network)
                 }
             )
         }
         .sheet(isPresented: $showingUSDTgVersePay) {
             USDTgVersePayView()
         }
         .sheet(isPresented: $showingMembership) {
             MembershipView()
         }
         .sheet(isPresented: $showingMarginTrading) {
             MarginTradingView()
         }
         .sheet(isPresented: $showingCopyTrading) {
             CopyTradingView()
         }
         .sheet(isPresented: $showingCustody) {
             CustodyView()
         }
    }
    
    // MARK: - Wallet Data Loading
    private func loadCurrentWallet() {
        // Load current wallet from UserDefaults
        currentWalletName = UserDefaults.standard.string(forKey: "CurrentWallet") ?? "My Quantum Wallet"
        currentNetwork = UserDefaults.standard.string(forKey: "Wallet_\(currentWalletName)_Network") ?? "USDTgVerse"
        
        // Load assets for current network
        loadAssetsForNetwork(currentNetwork)
    }
    
    // MARK: - Real Balance Loading Helper
    private func loadRealWalletBalance(for walletName: String, asset: String) -> Double {
        // Production blockchain balance loading
        guard networkManager.isConnected else {
            return 0.0 // No connection = 0 balance
        }
        
        return fetchBalanceFromBlockchain(for: walletName, asset: asset)
    }
    
    private func fetchBalanceFromBlockchain(for walletName: String, asset: String) -> Double {
        // Synchronous blockchain balance fetching
        let walletAddress = UserDefaults.standard.string(forKey: "walletAddress") ?? ""
        
        guard !walletAddress.isEmpty else {
            return 0.0
        }
        
        let semaphore = DispatchSemaphore(value: 0)
        var balance: Double = 0.0
        let apiURL = "https://api.usdtgverse.com/api/v1/balance/\(walletAddress)/\(asset.lowercased())"
        
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
    
    private func loadAssetsForNetwork(_ network: String) {
        // Load real balance from NativeWalletManager - NO DEMO DATA!
        switch network {
        case "USDTgVerse":
            // Fetch REAL balance from backend using NativeWalletManager
            Task {
                await nativeWalletManager.fetchBalance(for: currentWalletName)
                print("✅ Real balance fetched: \(nativeWalletManager.balance.usdtg)")
            }
            
            currentWalletAssets = [
                WalletAsset(symbol: "USDTg", name: "USDTgVerse Token", balance: nativeWalletManager.balance.usdtg, 
                           price: nativeWalletManager.getOraclePrice(for: "USDTg"),  // Real Oracle price
                           logoURL: "usdtg_logo", change24h: nativeWalletManager.getOracleChange24h(for: "USDTg"), chain: "USDTgVerse", isNativeImage: true),
                WalletAsset(symbol: "RGLS", name: "Regilis", balance: nativeWalletManager.balance.rgls ?? 0.0, 
                           price: nativeWalletManager.getOraclePrice(for: "RGLS"),  // Real Oracle price
                           logoURL: "regilis", change24h: nativeWalletManager.getOracleChange24h(for: "RGLS"), chain: "USDTgVerse", isNativeImage: true),
                WalletAsset(symbol: "USDTgV", name: "USDTgVerse Voting", balance: nativeWalletManager.balance.usdtgv, 
                           price: nativeWalletManager.getOraclePrice(for: "USDTgV"),  // Real Oracle price
                           logoURL: "USDTgV-logo", change24h: nativeWalletManager.getOracleChange24h(for: "USDTgV"), chain: "USDTgVerse", isNativeImage: true),
                WalletAsset(symbol: "USDTgG", name: "USDTgVerse Governance", balance: nativeWalletManager.balance.usdtgg, 
                           price: nativeWalletManager.getOraclePrice(for: "USDTgG"),  // Real Oracle price
                           logoURL: "USDTgG-logo", change24h: nativeWalletManager.getOracleChange24h(for: "USDTgG"), chain: "USDTgVerse", isNativeImage: true),
                WalletAsset(symbol: "USDT", name: "Tether USD", balance: nativeWalletManager.balance.usdt, price: 1.0, 
                           logoURL: "https://assets.coingecko.com/coins/images/325/large/Tether.png", change24h: 0.1, chain: "Ethereum", isNativeImage: false),
                WalletAsset(symbol: "USDC", name: "USD Coin", balance: nativeWalletManager.balance.usdc, price: 1.0,
                           logoURL: "https://assets.coingecko.com/coins/images/6319/large/USD_Coin_icon.png", change24h: 0.0, chain: "Ethereum", isNativeImage: false)
            ]
            
        case "Ethereum":
            currentWalletAssets = [
                WalletAsset(symbol: "ETH", name: "Ethereum", balance: 0.0, price: 2337.85,
                           logoURL: "https://cryptologos.cc/logos/ethereum-eth-logo.svg", change24h: -1.2, chain: "Ethereum", isNativeImage: false),
                WalletAsset(symbol: "USDT", name: "Tether USD", balance: 0.0, price: 1.0,
                           logoURL: "https://cryptologos.cc/logos/tether-usdt-logo.svg", change24h: 0.1, chain: "Ethereum", isNativeImage: false),
                WalletAsset(symbol: "USDC", name: "USDC Coin", balance: 0.0, price: 1.0,
                           logoURL: "https://cryptologos.cc/logos/usd-coin-usdc-logo.svg", change24h: 0.0, chain: "Ethereum", isNativeImage: false)
            ]
            
        case "BNB Chain":
            currentWalletAssets = [
                WalletAsset(symbol: "BNB", name: "BNB Chain", balance: 0.0, price: 245.5,
                           logoURL: "https://cryptologos.cc/logos/bnb-bnb-logo.svg", change24h: 3.4, chain: "BNB Chain", isNativeImage: false),
                WalletAsset(symbol: "USDT", name: "Tether USD", balance: 0.0, price: 1.0,
                           logoURL: "https://cryptologos.cc/logos/tether-usdt-logo.svg", change24h: 0.1, chain: "BNB Chain", isNativeImage: false),
                WalletAsset(symbol: "BUSD", name: "Binance USD", balance: 0.0, price: 1.0,
                           logoURL: "https://cryptologos.cc/logos/bnb-bnb-logo.svg", change24h: 0.0, chain: "BNB Chain", isNativeImage: false)
            ]
            
        case "TRON":
            currentWalletAssets = [
                WalletAsset(symbol: "TRX", name: "TRON", balance: 0.0, price: 0.091,
                           logoURL: "https://cryptologos.cc/logos/tron-trx-logo.svg", change24h: -2.1, chain: "TRON", isNativeImage: false),
                WalletAsset(symbol: "USDT", name: "Tether USD (TRC20)", balance: 0.0, price: 1.0,
                           logoURL: "https://cryptologos.cc/logos/tether-usdt-logo.svg", change24h: 0.1, chain: "TRON", isNativeImage: false)
            ]
            
        case "Solana":
            currentWalletAssets = [
                WalletAsset(symbol: "SOL", name: "Solana", balance: 0.0, price: 145.75,
                           logoURL: "https://cryptologos.cc/logos/solana-sol-logo.svg", change24h: 4.8, chain: "Solana", isNativeImage: false),
                WalletAsset(symbol: "USDT", name: "Tether USD (SPL)", balance: 0.0, price: 1.0,
                           logoURL: "https://cryptologos.cc/logos/tether-usdt-logo.svg", change24h: 0.1, chain: "Solana", isNativeImage: false)
            ]
            
        default:
            // Default to USDTgVerse
            loadCurrentWallet()
        }
    }
    
    private var headerView: some View {
        VStack(spacing: 12) {
            // Network Selector
            Button(action: {
                showingNetworkSelector = true
            }) {
                HStack(spacing: 8) {
                    Circle()
                        .fill(networkColor(for: currentNetwork))
                        .frame(width: 20, height: 20)
                        .overlay(
                            Text(networkIcon(for: currentNetwork))
                                .font(.caption2)
                        )
                    
                    Text(currentNetwork)
                        .font(.headline)
                        .fontWeight(.semibold)
                        .foregroundColor(.primary)
                    
                    Image(systemName: "chevron.down")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                .padding(.horizontal, 12)
                .padding(.vertical, 8)
                .background(Color(.systemGray6))
                .cornerRadius(20)
            }
            
            // Dynamic Network Status
            HStack {
                VStack(alignment: .leading) {
                    Text(networkTitle(for: currentNetwork))
                        .font(.title2)
                        .fontWeight(.bold)
                        .foregroundColor(networkColor(for: currentNetwork))
                    
                    .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                VStack(alignment: .trailing) {
                    Circle()
                        .fill(networkManager.isConnected ? .green : .red)
                        .frame(width: 12, height: 12)
                    
                    .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private func networkColor(for network: String) -> Color {
        switch network {
        case "USDTgVerse": return Color(red: 0.3, green: 0.7, blue: 0.3)
        case "Ethereum": return .purple
        case "BNB Chain": return .yellow
        case "TRON": return .red
        case "Solana": return .purple
        case "Polygon": return .purple
        case "Arbitrum": return .blue
        case "Avalanche": return .red
        default: return .gray
        }
    }
    
    private func networkImageURL(for network: String) -> String {
        switch network {
        case "USDTgVerse": return "usdtg_logo"  // Native asset
        case "Ethereum": return "https://assets.coingecko.com/coins/images/279/large/ethereum.png"
        case "BNB Chain": return "https://assets.coingecko.com/coins/images/825/large/bnb-icon2_2x.png"
        case "TRON": return "https://assets.coingecko.com/coins/images/1094/large/tron-logo.png"
        case "Solana": return "https://assets.coingecko.com/coins/images/4128/large/solana.png"
        case "Polygon": return "https://assets.coingecko.com/coins/images/4713/large/matic-token-icon.png"
        case "Arbitrum": return "https://assets.coingecko.com/coins/images/16547/large/LPApeTCM.png"
        case "Avalanche": return "https://assets.coingecko.com/coins/images/12559/large/Avalanche_Circle_RedWhite_Trans.png"
        default: return "https://assets.coingecko.com/coins/images/1/large/bitcoin.png"
        }
    }
    
    private func networkIcon(for network: String) -> String {
        switch network {
        case "USDTgVerse": return "💰"
        case "Ethereum": return "Ξ"
        case "BNB Chain": return "◆"
        case "TRON": return "♦"
        case "Solana": return "◉"
        case "Polygon": return "⬟"
        case "Arbitrum": return "🔵"
        case "Avalanche": return "▲"
        default: return "🌐"
        }
    }
    
    private func networkTitle(for network: String) -> String {
        switch network {
        case "USDTgVerse": return "USDTgVerse"
        case "Ethereum": return "Ethereum"
        case "BNB Chain": return "BNB Chain"
        case "TRON": return "TRON"
        case "Solana": return "Solana"
        case "Polygon": return "Polygon"
        case "Arbitrum": return "Arbitrum"
        case "Avalanche": return "Avalanche"
        default: return "Blockchain"
        }
    }
    
    private func networkSubtitle(for network: String) -> String {
        switch network {
        case "USDTgVerse": return "Enterprise Blockchain"
        case "Ethereum": return "Layer 1 • Decentralized Finance"
        case "BNB Chain": return "Fast & Low Cost"
        case "TRON": return "High Throughput Network"
        case "Solana": return "Ultra-Fast Blockchain"
        case "Polygon": return "Ethereum Layer 2"
        case " Arbitrum": return "Ethereum Scaling Solution"
        case "Avalanche": return "Fast Finality Network"
        default: return "Blockchain Network"
        }
    }
    
    private var portfolioCard: some View {
        VStack(spacing: 16) {
            Text("Total Portfolio Value")
                .font(.headline)
                .foregroundColor(.secondary)
            
             Button(action: {
                 showingWalletList = true
             }) {
                 HStack(spacing: 8) {
                     Text(currentWalletName)
                         .font(.headline)
                         .fontWeight(.semibold)
                         .foregroundColor(.white)
                     
                     .font(.caption)
                         .foregroundColor(.secondary)
                 }
             }
             
             Text(formatCurrency(totalPortfolioValue))
                 .font(.system(size: 36, weight: .bold, design: .rounded))
                 .foregroundColor(.white)
             
             HStack {
                 Text("USDTg: \(formatBalance(nativeWalletManager.currentWalletBalance))")
                     .font(.subheadline)
                     .foregroundColor(Color(red: 0.3, green: 0.7, blue: 0.3))
                
                Spacer()
                
                Text("+2.5% (24h)")
                    .font(.subheadline)
                    .foregroundColor(.green)
            }
        }
        .padding()
        .background(
            LinearGradient(
                gradient: Gradient(colors: [
                    Color(red: 0.3, green: 0.7, blue: 0.3).opacity(0.3),
                    Color(red: 0.2, green: 0.5, blue: 0.2).opacity(0.3)
                ]),
                startPoint: .topLeading,
                endPoint: .bottomTrailing
            )
        )
        .cornerRadius(16)
        .overlay(
            RoundedRectangle(cornerRadius: 16)
                .stroke(Color(red: 0.3, green: 0.7, blue: 0.3).opacity(0.5), lineWidth: 1)
        )
    }
    
    private var quickActionsView: some View {
        VStack(spacing: 15) {
            // Primary Actions Row
            HStack(spacing: 20) {
                QuickActionButton(
                    icon: "arrow.up.circle.fill",
                    title: "Send",
                    color: .red
                ) {
                    showingSendView = true
                }
                
                QuickActionButton(
                    icon: "arrow.down.circle.fill",
                    title: "Receive",
                    color: .green
                ) {
                    showingReceiveView = true
                }
                
                QuickActionButton(
                    icon: "arrow.left.arrow.right.circle.fill",
                    title: "Swap",
                    color: .blue
                ) {
                    showingSwapView = true
                }
                
                QuickActionButton(
                    icon: "creditcard.circle.fill",
                    title: "USDTgVerse PAY",
                    color: .green
                ) {
                    showingUSDTgVersePay = true
                }
            }
            
            // Enterprise Features Row
            HStack(spacing: 20) {
                QuickActionButton(
                    icon: "person.3.fill",
                        title: "Membership",
                        color: .purple
                    ) {
                        showingMembership = true
                    }
                    
                    QuickActionButton(
                        icon: "chart.line.uptrend.xyaxis",
                        title: "Margin Trading",
                        color: .orange
                    ) {
                        showingMarginTrading = true
                    }
                    
                    QuickActionButton(
                        icon: "doc.on.doc.fill",
                        title: "Copy Trading",
                        color: .cyan
                    ) {
                        showingCopyTrading = true
                    }
                    
                    QuickActionButton(
                        icon: "building.columns.fill",
                        title: "Custody",
                        color: .yellow
                    ) {
                        showingCustody = true
                    }
                }
        }
    }
    
    private var assetsListView: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("Assets")
                .font(.headline)
                .padding(.horizontal)
            
             ForEach(currentWalletAssets) { asset in
                 AssetRowView(asset: asset)
             }
        }
        .padding(.vertical)
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private var recentTransactionsView: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Text("Recent Transactions")
                    .font(.headline)
                
                Spacer()
                
                Button("View All") {
                    // Navigate to all transactions
                }
                .font(.caption)
                .foregroundColor(Color(red: 0.3, green: 0.7, blue: 0.3))
            }
            .padding(.horizontal)
            
            ForEach(getWalletTransactions(), id: \.id) { transaction in
                TransactionRowView(transaction: transaction)
            }
        }
        .padding(.vertical)
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private func getWalletTransactions() -> [WalletTransaction] {
        // Fetch real transactions from blockchain
        guard networkManager.isConnected else {
            return []
        }
        
        // In production, this will fetch from blockchain API
        return fetchRealTransactionsFromBlockchain()
    }
    
    private func fetchRealTransactionsFromBlockchain() -> [WalletTransaction] {
        // This will be implemented to fetch from production blockchain API
        // No demo transactions will be returned
        return []
    }
    
    struct WalletTransaction: Identifiable {
        let id: String
        let type: String
        let title: String
        let subtitle: String
        let amount: String
        let time: String
        let icon: String
    }
}

struct TransactionRowView: View {
    let transaction: WalletView.WalletTransaction
    
    var body: some View {
        HStack {
            Circle()
                .fill(transaction.type == "received" ? .green : transaction.type == "sent" ? .red : .blue)
                .frame(width: 40, height: 40)
                .overlay(
                    Image(systemName: transaction.icon)
                        .foregroundColor(.white)
                        .font(.system(size: 16, weight: .bold))
                )
            
            VStack(alignment: .leading, spacing: 4) {
                Text(transaction.title)
                    .font(.subheadline)
                    .fontWeight(.medium)
                
                Text(transaction.subtitle)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            VStack(alignment: .trailing, spacing: 4) {
                Text(transaction.amount)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(transaction.type == "received" ? .green : transaction.type == "sent" ? .red : .blue)
                
                Text(transaction.time)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(8)
    }
}

struct AssetRowView: View {
    let asset: WalletAsset
    
    var body: some View {
        HStack {
            // Asset icon - Native image or fallback
            Group {
                if asset.isNativeImage {
                    Image(asset.logoURL) // Assets.xcassets image
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                        .frame(width: 40, height: 40)
                        .clipShape(Circle())
                } else {
                    AsyncImage(url: URL(string: asset.logoURL)) { image in
                        image
                            .resizable()
                            .aspectRatio(contentMode: .fit)
                            .frame(width: 40, height: 40)
                            .clipShape(Circle())
                    } placeholder: {
                        Circle()
                            .fill(Color(red: 0.3, green: 0.7, blue: 0.3))
                            .frame(width: 40, height: 40)
                            .overlay(
                                Text(String(asset.symbol.prefix(2)))
                                    .font(.caption)
                                    .fontWeight(.bold)
                                    .foregroundColor(.white)
                            )
                    }
                }
            }
            
            VStack(alignment: .leading, spacing: 4) {
                Text(asset.symbol)
                    .font(.headline)
                    .foregroundColor(.white)
                
                Text(asset.name)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            VStack(alignment: .trailing, spacing: 4) {
                Text("\(asset.balance, specifier: "%.6f")")
                    .font(.headline)
                    .foregroundColor(.white)
                
                Text("$\(asset.totalValue, specifier: "%.2f")")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding(.horizontal)
        .padding(.vertical, 8)
    }
}

// MARK: - Basic Views (Placeholder)
struct BasicSendView: View {
    @Environment(\.presentationMode) var presentationMode
    
    var body: some View {
        NavigationView {
            VStack(spacing: 30) {
                Image(systemName: "arrow.up.circle.fill")
                    .font(.system(size: 60))
                    .foregroundColor(.blue)
                
                Text("Send Assets")
                    .font(.title2)
                    .fontWeight(.bold)
                
                Text("Send functionality coming soon!")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Button("Close") {
                    presentationMode.wrappedValue.dismiss()
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color.blue)
                .foregroundColor(.white)
                .cornerRadius(12)
                
                Spacer()
            }
            .padding()
            .navigationTitle("Send")
            .navigationBarTitleDisplayMode(.inline)
        }
    }
}

struct BasicReceiveView: View {
    @Environment(\.presentationMode) var presentationMode
    @State private var currentNetwork = "USDTgVerse"
    @State private var walletAddress = ""
    @State private var qrCodeImage: UIImage?
    @State private var showingShareSheet = false
    
    var body: some View {
        NavigationView {
            VStack(spacing: 30) {
                // Header
                VStack(spacing: 16) {
                    Image(systemName: "arrow.down.circle.fill")
                        .font(.system(size: 60))
                        .foregroundColor(.green)
                    
                    Text("Receive Assets")
                        .font(.title2)
                        .fontWeight(.bold)
                    
                    Text("Send assets to this address")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                }
                
                // Network Selector
                VStack(alignment: .leading, spacing: 8) {
                    Text("Network")
                        .font(.headline)
                        .fontWeight(.medium)
                    
                    Menu {
                        Button("USDTgVerse") { selectNetwork("USDTgVerse") }
                        Button("Ethereum") { selectNetwork("Ethereum") }
                        Button("BNB Chain") { selectNetwork("BNB Chain") }
                        Button("TRON") { selectNetwork("TRON") }
                        Button("Solana") { selectNetwork("Solana") }
                    } label: {
                        HStack {
                            Text(currentNetwork)
                                .foregroundColor(.primary)
                            Spacer()
                            Image(systemName: "chevron.down")
                                .foregroundColor(.secondary)
                        }
                        .padding()
                        .background(Color(.systemGray6))
                        .cornerRadius(8)
                    }
                }
                
                // QR Code Display
                VStack(spacing: 16) {
                    Text("Scan QR Code to Send")
                        .font(.headline)
                        .fontWeight(.medium)
                    
                    if let qrImage = qrCodeImage {
                        Image(uiImage: qrImage)
                            .interpolation(.none)
                            .resizable()
                            .scaledToFit()
                            .frame(width: 200, height: 200)
                            .background(Color.white)
                            .cornerRadius(12)
                            .overlay(
                                RoundedRectangle(cornerRadius: 12)
                                    .stroke(Color(.systemGray4), lineWidth: 1)
                            )
                    } else {
                        RoundedRectangle(cornerRadius: 12)
                            .fill(Color(.systemGray5))
                            .frame(width: 200, height: 200)
                            .overlay(
                                VStack {
                                    ProgressView()
                                    Text("Generating QR...")
                                        .font(.caption)
                                        .foregroundColor(.secondary)
                                }
                            )
                    }
                    
                    Button(action: {
                        showingShareSheet = true
                    }) {
                        HStack {
                            Image(systemName: "square.and.arrow.up")
                            Text("Share QR Code")
                        }
                        .frame(maxWidth: .infinity)
                        .padding()
                        .background(Color.green)
                        .foregroundColor(.white)
                        .cornerRadius(8)
                    }
                    .disabled(qrCodeImage == nil)
                }
                
                // Address Display
                VStack(alignment: .leading, spacing: 8) {
                    Text("Your \(currentNetwork) Address")
                        .font(.headline)
                        .fontWeight(.medium)
                    
                    VStack(spacing: 12) {
                        Text(walletAddress)
                            .font(.system(.body, design: .monospaced))
                            .padding()
                            .background(Color(.systemGray6))
                            .cornerRadius(8)
                            .textSelection(.enabled)
                        
                        Button(action: {
                            UIPasteboard.general.string = walletAddress
                        }) {
                            HStack {
                                Image(systemName: "doc.on.doc")
                                Text("Copy Address")
                            }
                            .frame(maxWidth: .infinity)
                            .padding()
                            .background(Color.blue)
                            .foregroundColor(.white)
                            .cornerRadius(8)
                        }
                    }
                }
                
                Spacer()
                
                Button("Close") {
                    presentationMode.wrappedValue.dismiss()
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color.green)
                .foregroundColor(.white)
                .cornerRadius(12)
            }
            .padding()
            .navigationTitle("Receive")
            .navigationBarTitleDisplayMode(.inline)
            .onAppear {
                loadWalletAddress()
            }
        }
        .sheet(isPresented: $showingShareSheet) {
            // ShareSheet implementation would go here
            Text("Share")
        }
    }
    
    private func selectNetwork(_ network: String) {
        currentNetwork = network
        loadWalletAddress()
    }
    
    private func loadWalletAddress() {
        let currentWallet = UserDefaults.standard.string(forKey: "CurrentWallet") ?? "USDTgVerse Wallet"
        
        if let addresses = UserDefaults.standard.dictionary(forKey: "Wallet_\(currentWallet)_Addresses") as? [String: String] {
            walletAddress = addresses[currentNetwork] ?? "Address not available"
        } else {
            // Fallback for demo wallet
            walletAddress = generateDemoAddress(for: currentNetwork)
        }
        
        // Generate QR code for the address
        generateAddressQRCode()
    }
    
    private func generateAddressQRCode() {
        // Generate QR Code using Core Image
        let context = CIContext()
        let filter = CIFilter.qrCodeGenerator()
        
        // Create QR data with address and network info
        let qrData = """
        {
            "network": "\(currentNetwork)",
            "address": "\(walletAddress)",
            "type": "USDTgVerse_Address",
            "quantum_safe": true
        }
        """
        
        filter.message = Data(qrData.utf8)
        filter.correctionLevel = "H" // High error correction for mobile scanning
        
        if let outputImage = filter.outputImage {
            // Scale.up the QR code for better visibility
            let transform = CGAffineTransform(scaleX: 10, y: 10)
            let scaledImage = outputImage.transformed(by: transform)
            
            if let cgImage = context.createCGImage(scaledImage, from: scaledImage.extent) {
                qrCodeImage = UIImage(cgImage: cgImage)
            }
        }
    }
    
    private func generateDemoAddress(for network: String) -> String {
        switch network {
        case "USDTgVerse": return "USDTg1Demo123456"
        case "Ethereum": return "0x742d35Cc6634C0532925a3b8D2Cc7a1c4c1aA6d1"
        case "BNB Chain": return "bnb1demo123456789"
        case "TRON": return "TRDemo123456789"
        case "Solana": return "SolDemo123456789"
        default: return "Address not available"
        }
    }
}

struct BasicSwapView: View {
    @Environment(\.presentationMode) var presentationMode
    
    var body: some View {
        NavigationView {
            VStack(spacing: 30) {
                Image(systemName: "arrow.left.arrow.right.circle.fill")
                    .font(.system(size: 60))
                    .foregroundColor(.purple)
                
                Text("Swap Assets")
                    .font(.title2)
                    .fontWeight(.bold)
                
                Text("Swap functionality coming soon!")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Button("Close") {
                    presentationMode.wrappedValue.dismiss()
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color.purple)
                .foregroundColor(.white)
                .cornerRadius(12)
                
                Spacer()
            }
            .padding()
            .navigationTitle("Swap")
            .navigationBarTitleDisplayMode(.inline)
        }
    }
}

// MARK: - Basic Wallet List Sheet
struct BasicWalletListSheet: View {
    @Environment(\.presentationMode) var presentationMode
    @Binding var currentWalletName: String
    let onWalletSelected: (String) -> Void
    @State private var availableWallets: [String] = []
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                // Header
                VStack(spacing: 12) {
                    Image("usdtg_logo")
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                        .frame(width: 60, height: 60)
                        .clipShape(Circle())
                    
                    Text("Select Wallet")
                        .font(.title2)
                        .fontWeight(.bold)
                    
                    Text("Switch between your wallets")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                }
                
                // Wallet List
                VStack(spacing: 12) {
                    ForEach(availableWallets, id: \.self) { walletName in
                        Button(action: {
                            onWalletSelected(walletName)
                            presentationMode.wrappedValue.dismiss()
                        }) {
                            HStack(spacing: 16) {
                                Circle()
                                    .fill(walletName == currentWalletName ? Color.green : Color.blue)
                                    .frame(width: 40, height: 40)
                                    .overlay(
                                        Image(systemName: walletName == currentWalletName ? "checkmark" : "wallet.pass")
                                            .foregroundColor(.white)
                                            .font(.headline)
                                    )
                                
                                VStack(alignment: .leading, spacing: 4) {
                                    HStack {
                                        Text(walletName)
                                            .font(.headline)
                                            .fontWeight(.semibold)
                                            .foregroundColor(.primary)
                                    }
                                    
                                    Text("Balance: $\(walletName == "USDTgVerse Wallet" ? 0.0 : 10.0, specifier: "%.2f")")
                                        .font(.caption)
                                        .foregroundColor(.secondary)
                                }
                                
                                Spacer()
                                
                                if walletName == currentWalletName {
                                    Text("ACTIVE")
                                        .font(.caption2)
                                        .fontWeight(.bold)
                                        .padding(.horizontal, 8)
                                        .padding(.vertical, 4)
                                        .background(Color.green.opacity(0.2))
                                        .foregroundColor(.green)
                                        .cornerRadius(6)
                                }
                            }
                            .padding()
                            .background(Color(.systemGray6))
                            .cornerRadius(12)
                            .overlay(
                                RoundedRectangle(cornerRadius: 12)
                                    .stroke(walletName == currentWalletName ? Color.green : Color.clear, lineWidth: 2)
                            )
                        }
                        .buttonStyle(PlainButtonStyle())
                    }
                }
                
                Spacer()
            }
            .padding()
            .navigationTitle("My Wallets")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
            .onAppear {
                loadAvailableWallets()
            }
        }
    }
    
    private func loadAvailableWallets() {
        // Load wallet list from UserDefaults (persistent across operations)
        availableWallets = UserDefaults.standard.stringArray(forKey: "WalletList") ?? ["USDTgVerse Wallet"]
    }
}

// MARK: - Network Selector Sheet
struct NetworkSelectorSheet: View {
    @Environment(\.presentationMode) var presentationMode
    @Binding var currentNetwork: String
    let onNetworkSelected: (String) -> Void
    
    private let networks = [
        "USDTgVerse", "Ethereum", "BNB Chain", "TRON", 
        "Solana", "Polygon", "Arbitrum", "Avalanche"
    ]
    
    var body: some View {
        NavigationView {
            VStack(spacing: 20) {
                // Header
                VStack(spacing: 12) {
                    Image("usdtg_logo")
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                        .frame(width: 60, height: 60)
                        .clipShape(Circle())
                    
                    Text("Select Network")
                        .font(.title2)
                        .fontWeight(.bold)
                    
                    Text("Switch between blockchain networks")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                }
                
                // Network List
                VStack(spacing: 12) {
                    ForEach(networks, id: \.self) { network in
                        Button(action: {
                            onNetworkSelected(network)
                            presentationMode.wrappedValue.dismiss()
                        }) {
                            HStack(spacing: 16) {
                                // Network Logo: Native for USDTgVerse, Remote for others
                                if network == "USDTgVerse" {
                                    Image("usdtg_logo")
                                        .resizable()
                                        .aspectRatio(contentMode: .fit)
                                        .frame(width: 40, height: 40)
                                        .clipShape(Circle())
                                } else {
                                    AsyncImage(url: URL(string: networkImageURL(for: network))) { image in
                                        image
                                            .resizable()
                                            .aspectRatio(contentMode: .fit)
                                    } placeholder: {
                                        Circle()
                                            .fill(networkColor(for: network))
                                            .overlay(
                                                Text(networkIcon(for: network))
                                                    .font(.headline)
                                            )
                                    }
                                    .frame(width: 40, height: 40)
                                    .clipShape(Circle())
                                }
                                
                                VStack(alignment: .leading, spacing: 4) {
                                    HStack {
                                        Text(network)
                                            .font(.headline)
                                            .fontWeight(.semibold)
                                            .foregroundColor(.primary)
                                        
                                        if network == "USDTgVerse" {
                                            Text("NATIVE")
                                                .font(.caption2)
                                                .fontWeight(.bold)
                                                .padding(.horizontal, 6)
                                                .padding(.vertical, 2)
                                                .background(Color.green.opacity(0.2))
                                                .foregroundColor(.green)
                                                .cornerRadius(4)
                                        }
                                    }
                                    
                                    Text(networkDescription(for: network))
                                        .font(.caption)
                                        .foregroundColor(.secondary)
                                }
                                
                                Spacer()
                                
                                if network == currentNetwork {
                                    Image(systemName: "checkmark.circle.fill")
                                        .foregroundColor(.green)
                                        .font(.title2)
                                }
                            }
                            .padding()
                            .background(Color(.systemGray6))
                            .cornerRadius(12)
                            .overlay(
                                RoundedRectangle(cornerRadius: 12)
                                    .stroke(network == currentNetwork ? Color.green : Color.clear, lineWidth: 2)
                            )
                        }
                        .buttonStyle(PlainButtonStyle())
                    }
                }
                
                Spacer()
            }
            .padding()
            .navigationTitle("Networks")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
        }
    }
    
    private func networkColor(for network: String) -> Color {
        switch network {
        case "USDTgVerse": return Color(red: 0.3, green: 0.7, blue: 0.3)
        case "Ethereum": return .purple
        case "BNB Chain": return .yellow
        case "TRON": return .red
        case "Solana": return .purple
        case "Polygon": return .purple
        case "Arbitrum": return .blue
        case "Avalanche": return .red
        default: return .gray
        }
    }
    
    private func networkImageURL(for network: String) -> String {
        switch network {
        case "USDTgVerse": return "usdtg_logo"  // Native asset
        case "Ethereum": return "https://assets.coingecko.com/coins/images/279/large/ethereum.png"
        case "BNB Chain": return "https://assets.coingecko.com/coins/images/825/large/bnb-icon2_2x.png"
        case "TRON": return "https://assets.coingecko.com/coins/images/1094/large/tron-logo.png"
        case "Solana": return "https://assets.coingecko.com/coins/images/4128/large/solana.png"
        case "Polygon": return "https://assets.coingecko.com/coins/images/4713/large/matic-token-icon.png"
        case "Arbitrum": return "https://assets.coingecko.com/coins/images/16547/large/LPApeTCM.png"
        case "Avalanche": return "https://assets.coingecko.com/coins/images/12559/large/Avalanche_Circle_RedWhite_Trans.png"
        default: return "https://assets.coingecko.com/coins/images/1/large/bitcoin.png"
        }
    }
    
    private func networkIcon(for network: String) -> String {
        switch network {
        case "USDTgVerse": return "💰"
        case "Ethereum": return "Ξ"
        case "BNB Chain": return "◆"
        case "TRON": return "♦"
        case "Solana": return "◉"
        case "Polygon": return "⬟"
        case "Arbitrum": return "🔵"
        case "Avalanche": return "▲"
        default: return "🌐"
        }
    }
    
    private func networkDescription(for network: String) -> String {
        switch network {
        case "USDTgVerse": return "Native blockchain • USDTg stablecoin"
        case "Ethereum": return "Layer 1 • ETH, USDT, USDC, DeFi"
        case "BNB Chain": return "Fast & cheap • BNB, USDT, BUSD"
        case "TRON": return "High throughput • TRX, USDT-TRC20"
        case "Solana": return "Ultra-fast • SOL, USDT-SPL"
        case "Polygon": return "Ethereum L2 • MATIC, USDT"
        case "Arbitrum": return "Ethereum L2 • ETH, USDT, ARB"
        case "Avalanche": return "Fast finality • AVAX, USDT"
        default: return "Blockchain network"
        }
    }
}

#Preview {
    WalletView()
        .environmentObject(NetworkManager())
}
