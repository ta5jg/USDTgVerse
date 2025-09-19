//
//  MultiChainWalletManager.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import Foundation
import SwiftUI

// MARK: - Blockchain Networks
enum BlockchainNetwork: String, CaseIterable {
    case usdtgverse = "USDTgVerse"
    case ethereum = "Ethereum"
    case bnb = "BNB Chain"
    case polygon = "Polygon"
    case arbitrum = "Arbitrum"
    case tron = "TRON"
    case solana = "Solana"
    case avalanche = "Avalanche"
    case optimism = "Optimism"
    
    var chainId: String {
        switch self {
        case .usdtgverse: return "usdtgverse-mainnet-1"
        case .ethereum: return "1"
        case .bnb: return "56"
        case .polygon: return "137"
        case .arbitrum: return "42161"
        case .tron: return "728126428"
        case .solana: return "101"
        case .avalanche: return "43114"
        case .optimism: return "10"
        }
    }
    
    var nativeCurrency: String {
        switch self {
        case .usdtgverse: return "USDTg"
        case .ethereum: return "ETH"
        case .bnb: return "BNB"
        case .polygon: return "MATIC"
        case .arbitrum: return "ETH"
        case .tron: return "TRX"
        case .solana: return "SOL"
        case .avalanche: return "AVAX"
        case .optimism: return "ETH"
        }
    }
    
    var icon: String {
        switch self {
        case .usdtgverse: return "💰"
        case .ethereum: return "🔵"
        case .bnb: return "🟡"
        case .polygon: return "🔶"
        case .arbitrum: return "🔵"
        case .tron: return "🟢"
        case .solana: return "🟣"
        case .avalanche: return "🔴"
        case .optimism: return "🟠"
        }
    }
    
    var color: Color {
        switch self {
        case .usdtgverse: return .green
        case .ethereum: return .blue
        case .bnb: return .yellow
        case .polygon: return .purple
        case .arbitrum: return .blue
        case .tron: return .red
        case .solana: return .purple
        case .avalanche: return .red
        case .optimism: return .orange
        }
    }
}

// MARK: - Wallet Address
struct WalletAddress: Identifiable {
    let id = UUID()
    let network: BlockchainNetwork
    let address: String
    let name: String
    let isActive: Bool
    
    var shortAddress: String {
        if address.count > 10 {
            return "\(address.prefix(6))...\(address.suffix(4))"
        }
        return address
    }
}

// MARK: - Token Asset
struct TokenAsset: Identifiable {
    let id = UUID()
    let symbol: String
    let name: String
    let network: BlockchainNetwork
    let contractAddress: String?
    let balance: Double
    let price: Double
    let logoURL: String?
    let isNative: Bool
    let decimals: Int
    
    var value: Double {
        return balance * price
    }
    
    var formattedBalance: String {
        return String(format: "%.\(decimals)f", balance)
    }
    
    var formattedValue: String {
        return String(format: "$%.2f", value)
    }
    
    var formattedPrice: String {
        return String(format: "$%.2f", price)
    }
    
    var networkInfo: String {
        if isNative && network == .usdtgverse {
            return "Native • $1.00 FIXED"
        } else {
            return "\(network.rawValue) • \(formattedPrice)"
        }
    }
}

// MARK: - Multi-Chain Wallet Manager
class MultiChainWalletManager: ObservableObject {
    @Published var walletAddresses: [WalletAddress] = []
    @Published var tokenAssets: [TokenAsset] = []
    @Published var isLoading = false
    @Published var selectedNetwork: BlockchainNetwork = .usdtgverse
    
    init() {
        generateWalletAddresses()
        loadTokenAssets()
    }
    
    // MARK: - Wallet Address Generation
    private func generateWalletAddresses() {
        walletAddresses = [
            // USDTgVerse Native
            WalletAddress(
                network: .usdtgverse,
                address: "usdtg1234567890abcdef1234567890abcdef12345678",
                name: "USDTgVerse Main",
                isActive: true
            ),
            
            // Ethereum
            WalletAddress(
                network: .ethereum,
                address: "0x1234567890abcdef1234567890abcdef12345678",
                name: "Ethereum Main",
                isActive: true
            ),
            
            // BNB Chain
            WalletAddress(
                network: .bnb,
                address: "0xabcdef1234567890abcdef1234567890abcdef12",
                name: "BNB Chain Main",
                isActive: true
            ),
            
            // TRON
            WalletAddress(
                network: .tron,
                address: "TAbcDef123456789aBcDeF123456789aBcDeF123",
                name: "TRON Main",
                isActive: true
            ),
            
            // Solana
            WalletAddress(
                network: .solana,
                address: "AbCdEf123456789aBcDeF123456789aBcDeF123456789aBc",
                name: "Solana Main",
                isActive: true
            ),
            
            // Polygon
            WalletAddress(
                network: .polygon,
                address: "0x9876543210fedcba9876543210fedcba98765432",
                name: "Polygon Main",
                isActive: true
            ),
            
            // Arbitrum
            WalletAddress(
                network: .arbitrum,
                address: "0x5678901234abcdef5678901234abcdef56789012",
                name: "Arbitrum Main",
                isActive: true
            ),
            
            // Avalanche
            WalletAddress(
                network: .avalanche,
                address: "0xfedcba0987654321fedcba0987654321fedcba09",
                name: "Avalanche Main",
                isActive: true
            ),
            
            // Optimism
            WalletAddress(
                network: .optimism,
                address: "0x1357924680abcdef1357924680abcdef13579246",
                name: "Optimism Main",
                isActive: true
            )
        ]
    }
    
    // MARK: - Token Asset Loading
    private func loadTokenAssets() {
        tokenAssets = [
            // Native USDTgVerse
            TokenAsset(
                symbol: "USDTg",
                name: "USDTg",
                network: .usdtgverse,
                contractAddress: nil, // Native token
                balance: 10000.00,
                price: 1.00, // Fixed $1.00 peg
                logoURL: nil, // Use local asset
                isNative: true,
                decimals: 2
            ),
            
            // Ethereum Network Tokens
            TokenAsset(
                symbol: "ETH",
                name: "Ethereum",
                network: .ethereum,
                contractAddress: nil, // Native ETH
                balance: 1.5,
                price: 2337.85,
                logoURL: "https://cryptologos.cc/logos/ethereum-eth-logo.png",
                isNative: false,
                decimals: 4
            ),
            
            TokenAsset(
                symbol: "USDT",
                name: "Tether (Ethereum)",
                network: .ethereum,
                contractAddress: "0xdAC17F958D2ee523a2206206994597C13D831ec7",
                balance: 3000.00,
                price: 1.00,
                logoURL: "https://cryptologos.cc/logos/tether-usdt-logo.png",
                isNative: false,
                decimals: 2
            ),
            
            TokenAsset(
                symbol: "USDC",
                name: "USD Coin (Ethereum)",
                network: .ethereum,
                contractAddress: "0xA0b86a33E6441c8C7c4c4D5B5B5B5B5B5B5B5B5B",
                balance: 1500.00,
                price: 1.00,
                logoURL: "https://cryptologos.cc/logos/usd-coin-usdc-logo.png",
                isNative: false,
                decimals: 2
            ),
            
            // TRON Network Tokens
            TokenAsset(
                symbol: "TRX",
                name: "TRON",
                network: .tron,
                contractAddress: nil, // Native TRX
                balance: 500.00,
                price: 0.091,
                logoURL: "https://cryptologos.cc/logos/tron-trx-logo.png",
                isNative: false,
                decimals: 2
            ),
            
            TokenAsset(
                symbol: "USDT",
                name: "Tether (TRON)",
                network: .tron,
                contractAddress: "TR7NHqjeKQxGTCi8q8ZY4pL8otSzgjLj6t",
                balance: 2000.00,
                price: 1.00,
                logoURL: "https://cryptologos.cc/logos/tether-usdt-logo.png",
                isNative: false,
                decimals: 2
            ),
            
            // BNB Chain Tokens
            TokenAsset(
                symbol: "BNB",
                name: "BNB",
                network: .bnb,
                contractAddress: nil, // Native BNB
                balance: 10.0,
                price: 245.50,
                logoURL: "https://cryptologos.cc/logos/bnb-bnb-logo.png",
                isNative: false,
                decimals: 4
            ),
            
            // Solana Network Tokens
            TokenAsset(
                symbol: "SOL",
                name: "Solana",
                network: .solana,
                contractAddress: nil, // Native SOL
                balance: 50.0,
                price: 145.75,
                logoURL: "https://cryptologos.cc/logos/solana-sol-logo.png",
                isNative: false,
                decimals: 4
            ),
            
            // Bitcoin (via bridge)
            TokenAsset(
                symbol: "BTC",
                name: "Bitcoin (Wrapped)",
                network: .ethereum,
                contractAddress: "0x2260FAC5E5542a773Aa44fBCfeDf7C193bc2C599",
                balance: 0.15,
                price: 45000.00,
                logoURL: "https://cryptologos.cc/logos/bitcoin-btc-logo.png",
                isNative: false,
                decimals: 8
            )
        ]
    }
    
    // MARK: - Add New Wallet
    func addNewWallet(network: BlockchainNetwork, name: String) {
        let newAddress = generateAddressForNetwork(network)
        let newWallet = WalletAddress(
            network: network,
            address: newAddress,
            name: name,
            isActive: true
        )
        walletAddresses.append(newWallet)
    }
    
    private func generateAddressForNetwork(_ network: BlockchainNetwork) -> String {
        // Generate network-specific address format
        switch network {
        case .usdtgverse:
            return "usdtg" + String((0..<36).map { _ in "0123456789abcdef".randomElement()! })
        case .ethereum, .bnb, .polygon, .arbitrum, .optimism:
            return "0x" + String((0..<40).map { _ in "0123456789abcdef".randomElement()! })
        case .tron:
            return "T" + String((0..<33).map { _ in "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz".randomElement()! })
        case .solana:
            return String((0..<44).map { _ in "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz".randomElement()! })
        case .avalanche:
            return "0x" + String((0..<40).map { _ in "0123456789abcdef".randomElement()! })
        }
    }
    
    // MARK: - Token Management
    func addCustomToken(symbol: String, name: String, network: BlockchainNetwork, contractAddress: String?) {
        let newToken = TokenAsset(
            symbol: symbol,
            name: name,
            network: network,
            contractAddress: contractAddress,
            balance: 0.0,
            price: 0.0,
            logoURL: nil,
            isNative: false,
            decimals: 2
        )
        tokenAssets.append(newToken)
    }
    
    func removeToken(_ token: TokenAsset) {
        tokenAssets.removeAll { $0.id == token.id }
    }
    
    // MARK: - Network Filtering
    func tokensForNetwork(_ network: BlockchainNetwork) -> [TokenAsset] {
        return tokenAssets.filter { $0.network == network }
    }
    
    func walletsForNetwork(_ network: BlockchainNetwork) -> [WalletAddress] {
        return walletAddresses.filter { $0.network == network }
    }
    
    // MARK: - Portfolio Calculations
    var totalPortfolioValue: Double {
        return tokenAssets.reduce(0) { $0 + $1.value }
    }
    
    var formattedTotalValue: String {
        return String(format: "$%.2f", totalPortfolioValue)
    }
    
    // MARK: - Bridge Integration
    func canBridgeToken(_ token: TokenAsset, to targetNetwork: BlockchainNetwork) -> Bool {
        // USDTg can be bridged to any network
        if token.symbol == "USDTg" {
            return targetNetwork != .usdtgverse
        }
        
        // USDT/USDC can be bridged between most networks
        if ["USDT", "USDC"].contains(token.symbol) {
            return true
        }
        
        // Native tokens can be wrapped/bridged
        if token.contractAddress == nil {
            return true
        }
        
        return false
    }
    
    func getBridgeNetworks(for token: TokenAsset) -> [BlockchainNetwork] {
        return BlockchainNetwork.allCases.filter { network in
            canBridgeToken(token, to: network)
        }
    }
}

// MARK: - Real Token Logos
struct RealTokenLogo: View {
    let token: TokenAsset
    let size: CGFloat
    
    var body: some View {
        Group {
            if token.isNative && token.network == .usdtgverse {
                // Use our native USDTg logo
                Image("usdtg_logo")
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: size, height: size)
                    .clipShape(Circle())
                    .overlay(
                        Circle()
                            .stroke(Color.green, lineWidth: 2)
                    )
            } else {
                // For other tokens, use network-specific styling
                AsyncImage(url: URL(string: token.logoURL ?? "")) { image in
                    image
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                } placeholder: {
                    Circle()
                        .fill(token.network.color.opacity(0.3))
                        .overlay(
                            Text(String(token.symbol.prefix(1)))
                                .font(.system(size: size * 0.4, weight: .bold))
                                .foregroundColor(token.network.color)
                        )
                }
                .frame(width: size, height: size)
                .clipShape(Circle())
            }
        }
    }
}

// MARK: - Network Badge
struct NetworkBadge: View {
    let network: BlockchainNetwork
    
    var body: some View {
        HStack(spacing: 4) {
            Text(network.icon)
                .font(.caption2)
            
            Text(network.rawValue)
                .font(.caption2)
                .fontWeight(.medium)
        }
        .padding(.horizontal, 8)
        .padding(.vertical, 4)
        .background(network.color.opacity(0.2))
        .foregroundColor(network.color)
        .cornerRadius(8)
    }
}

// MARK: - Add Wallet Sheet
struct AddWalletSheet: View {
    @Environment(\.presentationMode) var presentationMode
    @ObservedObject var walletManager: MultiChainWalletManager
    
    @State private var selectedNetwork: BlockchainNetwork = .ethereum
    @State private var walletName = ""
    @State private var customAddress = ""
    @State private var useGeneratedAddress = true
    
    var body: some View {
        NavigationView {
            Form {
                Section("Network Selection") {
                    Picker("Blockchain Network", selection: $selectedNetwork) {
                        ForEach(BlockchainNetwork.allCases, id: \.self) { network in
                            HStack {
                                Text(network.icon)
                                Text(network.rawValue)
                                Spacer()
                                Text(network.nativeCurrency)
                                    .font(.caption)
                                    .foregroundColor(.secondary)
                            }
                            .tag(network)
                        }
                    }
                    .pickerStyle(MenuPickerStyle())
                }
                
                Section("Wallet Details") {
                    TextField("Wallet Name", text: $walletName)
                        .textInputAutocapitalization(.words)
                    
                    Toggle("Generate New Address", isOn: $useGeneratedAddress)
                    
                    if !useGeneratedAddress {
                        TextField("Custom Address", text: $customAddress)
                            .textInputAutocapitalization(.never)
                            .autocorrectionDisabled()
                    }
                }
                
                Section("Network Info") {
                    HStack {
                        Text("Chain ID")
                        Spacer()
                        Text(selectedNetwork.chainId)
                            .foregroundColor(.secondary)
                    }
                    
                    HStack {
                        Text("Native Currency")
                        Spacer()
                        Text(selectedNetwork.nativeCurrency)
                            .foregroundColor(.secondary)
                    }
                }
            }
            .navigationTitle("Add Wallet")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(
                leading: Button("Cancel") {
                    presentationMode.wrappedValue.dismiss()
                },
                trailing: Button("Add") {
                    addWallet()
                }
                .disabled(walletName.isEmpty)
            )
        }
    }
    
    private func addWallet() {
        walletManager.addNewWallet(network: selectedNetwork, name: walletName)
        presentationMode.wrappedValue.dismiss()
    }
}

// MARK: - Add Token Sheet
struct AddTokenSheet: View {
    @Environment(\.presentationMode) var presentationMode
    @ObservedObject var walletManager: MultiChainWalletManager
    
    @State private var tokenSymbol = ""
    @State private var tokenName = ""
    @State private var selectedNetwork: BlockchainNetwork = .ethereum
    @State private var contractAddress = ""
    
    var body: some View {
        NavigationView {
            Form {
                Section("Token Details") {
                    TextField("Symbol (e.g. LINK)", text: $tokenSymbol)
                        .textInputAutocapitalization(.characters)
                    
                    TextField("Name (e.g. Chainlink)", text: $tokenName)
                        .textInputAutocapitalization(.words)
                }
                
                Section("Network") {
                    Picker("Blockchain Network", selection: $selectedNetwork) {
                        ForEach(BlockchainNetwork.allCases, id: \.self) { network in
                            HStack {
                                Text(network.icon)
                                Text(network.rawValue)
                            }
                            .tag(network)
                        }
                    }
                    .pickerStyle(MenuPickerStyle())
                }
                
                Section("Contract Address") {
                    TextField("Contract Address", text: $contractAddress)
                        .textInputAutocapitalization(.never)
                        .autocorrectionDisabled()
                    
                    Text("Leave empty for native tokens")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
            .navigationTitle("Add Token")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(
                leading: Button("Cancel") {
                    presentationMode.wrappedValue.dismiss()
                },
                trailing: Button("Add") {
                    addToken()
                }
                .disabled(tokenSymbol.isEmpty || tokenName.isEmpty)
            )
        }
    }
    
    private func addToken() {
        walletManager.addCustomToken(
            symbol: tokenSymbol,
            name: tokenName,
            network: selectedNetwork,
            contractAddress: contractAddress.isEmpty ? nil : contractAddress
        )
        presentationMode.wrappedValue.dismiss()
    }
}
