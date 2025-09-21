//
//  EnhancedWalletView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

struct EnhancedWalletView: View {
    @StateObject private var walletManager = MultiChainWalletManager()
    @State private var selectedTab = 0
    @State private var showingAddWallet = false
    @State private var showingAddToken = false
    @State private var showingNetworkFilter = false
    @State private var selectedNetworkFilter: BlockchainNetwork? = nil
    
    var filteredTokens: [TokenAsset] {
        if let filter = selectedNetworkFilter {
            return walletManager.tokensForNetwork(filter)
        }
        return walletManager.tokenAssets
    }
    
    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                // Portfolio Summary
                portfolioSummaryView
                
                // Network Filter
                networkFilterView
                
                // Tab Selector
                tabSelectorView
                
                // Content
                tabContentView
                
                Spacer()
            }
            .navigationTitle("Multi-Chain Wallet")
            .navigationBarTitleDisplayMode(.inline)
            .toolbar {
                ToolbarItem(placement: .navigationBarTrailing) {
                    Menu {
                        Button("Add Wallet", action: { showingAddWallet = true })
                        Button("Add Token", action: { showingAddToken = true })
                        Button("Refresh", action: { walletManager.loadTokenAssets() })
                        Button("Settings", action: {})
                    } label: {
                        Image(systemName: "plus.circle")
                    }
                }
            }
            .sheet(isPresented: $showingAddWallet) {
                AddWalletSheet(walletManager: walletManager)
            }
            .sheet(isPresented: $showingAddToken) {
                AddTokenSheet(walletManager: walletManager)
            }
        }
    }
    
    private var portfolioSummaryView: some View {
        VStack(spacing: 16) {
            // USDTgVerse Branding
            VStack(spacing: 8) {
                Image("usdtg_logo")
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: 60, height: 60)
                    .clipShape(Circle())
                    .overlay(
                        Circle()
                            .stroke(LinearGradient(gradient: Gradient(colors: [Color.green, Color.blue]), startPoint: .topLeading, endPoint: .bottomTrailing), lineWidth: 3)
                    )
                
                Text("USDTgVerse Wallet")
                    .font(.headline)
                    .fontWeight(.bold)
                
                Text("8-Chain Multi-Verse")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            // Total Portfolio
            VStack(spacing: 4) {
                Text("Total Portfolio")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Text(walletManager.formattedTotalValue)
                    .font(.largeTitle)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                    .contentTransition(.numericText())
                
                HStack(spacing: 16) {
                    Text("\(walletManager.tokenAssets.count) Assets")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("•")
                        .foregroundColor(.secondary)
                    
                    Text("\(walletManager.walletAddresses.count) Wallets")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("•")
                        .foregroundColor(.secondary)
                    
                    Text("8 Networks")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
        .padding()
    }
    
    private var networkFilterView: some View {
        ScrollView(.horizontal, showsIndicators: false) {
            HStack(spacing: 12) {
                // All Networks
                FilterChip(
                    title: "All",
                    isSelected: selectedNetworkFilter == nil,
                    action: { selectedNetworkFilter = nil }
                )
                
                // Individual Networks
                ForEach(BlockchainNetwork.allCases, id: \.self) { network in
                    FilterChip(
                        title: network.rawValue,
                        icon: network.icon,
                        isSelected: selectedNetworkFilter == network,
                        action: { selectedNetworkFilter = network }
                    )
                }
            }
            .padding(.horizontal)
        }
    }
    
    private var tabSelectorView: some View {
        HStack(spacing: 0) {
            TabButton(title: "Assets", isSelected: selectedTab == 0) {
                selectedTab = 0
            }
            
            TabButton(title: "Wallets", isSelected: selectedTab == 1) {
                selectedTab = 1
            }
            
            TabButton(title: "Send", isSelected: selectedTab == 2) {
                selectedTab = 2
            }
            
            TabButton(title: "History", isSelected: selectedTab == 3) {
                selectedTab = 3
            }
        }
        .padding(.horizontal)
    }
    
    @ViewBuilder
    private var tabContentView: some View {
        switch selectedTab {
        case 0:
            assetsView
        case 1:
            walletsView
        case 2:
            sendView
        case 3:
            historyView
        default:
            assetsView
        }
    }
    
    private var assetsView: some View {
        ScrollView {
            LazyVStack(spacing: 12) {
                ForEach(filteredTokens) { token in
                    EnhancedAssetRow(token: token, walletManager: walletManager)
                }
                
                if filteredTokens.isEmpty {
                    VStack(spacing: 16) {
                        Image(systemName: "plus.circle.dashed")
                            .font(.system(size: 48))
                            .foregroundColor(.secondary)
                        
                        Text("No assets found")
                            .font(.headline)
                            .foregroundColor(.secondary)
                        
                        Text("Add tokens or change network filter")
                            .font(.caption)
                            .foregroundColor(.secondary)
                        
                        Button("Add Token") {
                            showingAddToken = true
                        }
                        .buttonStyle(.borderedProminent)
                    }
                    .padding()
                }
            }
            .padding()
        }
    }
    
    private var walletsView: some View {
        ScrollView {
            LazyVStack(spacing: 12) {
                ForEach(walletManager.walletAddresses) { wallet in
                    WalletAddressRow(wallet: wallet)
                }
                
                // Add Wallet Button
                Button(action: { showingAddWallet = true }) {
                    HStack {
                        Image(systemName: "plus.circle.fill")
                            .foregroundColor(.blue)
                        
                        Text("Add New Wallet")
                            .fontWeight(.medium)
                        
                        Spacer()
                        
                        Image(systemName: "chevron.right")
                            .foregroundColor(.secondary)
                    }
                    .padding()
                    .background(Color(.systemGray6))
                    .cornerRadius(12)
                }
                .buttonStyle(PlainButtonStyle())
            }
            .padding()
        }
    }
    
    private var sendView: some View {
        ScrollView {
            VStack(spacing: 20) {
                Image(systemName: "arrow.up.circle.fill")
                    .font(.system(size: 64))
                    .foregroundColor(.blue)
                
                Text("Send Assets")
                    .font(.title2)
                    .fontWeight(.semibold)
                
                VStack(spacing: 16) {
                    // Asset Selection
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Select Asset")
                            .font(.headline)
                        
                        Picker("Asset", selection: .constant("USDTg")) {
                            ForEach(walletManager.tokenAssets, id: \.symbol) { token in
                                HStack {
                                    RealTokenLogo(token: token, size: 20)
                                    Text("\(token.symbol) (\(token.network.rawValue))")
                                    Spacer()
                                    Text(token.formattedBalance)
                                        .font(.caption)
                                        .foregroundColor(.secondary)
                                }.tag(token.symbol)
                            }
                        }
                        .pickerStyle(MenuPickerStyle())
                        .padding()
                        .background(Color(.systemGray6))
                        .cornerRadius(12)
                    }
                    
                    // Recipient
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Recipient")
                            .font(.headline)
                        
                        TextField("Address or ENS name", text: .constant(""))
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                    }
                    
                    // Amount
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Amount")
                            .font(.headline)
                        
                        TextField("0.00", text: .constant(""))
                            .keyboardType(.decimalPad)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                    }
                }
                
                Button("Send Transaction") {
                    // Handle send
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color.blue)
                .foregroundColor(.white)
                .cornerRadius(12)
                .font(.headline)
            }
            .padding()
        }
    }
    
    private var historyView: some View {
        ScrollView {
            VStack(spacing: 16) {
                Text("Transaction History")
                    .font(.headline)
                
                Text("Multi-chain transaction history will appear here")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                    .multilineTextAlignment(.center)
                
                // Sample transactions for each network
                VStack(spacing: 12) {
                    SampleTransactionRow(
                        type: "Bridge",
                        description: "ETH → USDTgVerse",
                        amount: "+1,000 USDTg",
                        network: .usdtgverse
                    )
                    
                    SampleTransactionRow(
                        type: "Swap",
                        description: "ETH → USDT",
                        amount: "-0.5 ETH",
                        network: .ethereum
                    )
                    
                    SampleTransactionRow(
                        type: "Send",
                        description: "To 0x1234...5678",
                        amount: "-100 USDT",
                        network: .tron
                    )
                }
            }
            .padding()
        }
    }
}

// MARK: - Enhanced Asset Row
struct EnhancedAssetRow: View {
    let token: TokenAsset
    let walletManager: MultiChainWalletManager
    
    var body: some View {
        HStack(spacing: 16) {
            // Real Token Logo
            RealTokenLogo(token: token, size: 40)
            
            // Token Info
            VStack(alignment: .leading, spacing: 4) {
                HStack {
                    Text(token.name)
                        .font(.headline)
                        .fontWeight(.semibold)
                    
                    if token.isNative {
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
                
                Text(token.networkInfo)
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                NetworkBadge(network: token.network)
            }
            
            Spacer()
            
            // Balance & Value
            VStack(alignment: .trailing, spacing: 4) {
                Text(token.formattedBalance)
                    .font(.headline)
                    .fontWeight(.semibold)
                    .contentTransition(.numericText())
                
                Text(token.formattedValue)
                    .font(.caption)
                    .foregroundColor(.secondary)
                    .contentTransition(.numericText())
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
        .overlay(
            RoundedRectangle(cornerRadius: 12)
                .stroke(token.isNative ? Color.green.opacity(0.5) : Color.clear, lineWidth: 2)
        )
    }
}

// MARK: - Wallet Address Row
struct WalletAddressRow: View {
    let wallet: WalletAddress
    
    var body: some View {
        HStack(spacing: 16) {
            // Network Icon
            Circle()
                .fill(wallet.network.color.opacity(0.2))
                .frame(width: 40, height: 40)
                .overlay(
                    Text(wallet.network.icon)
                        .font(.title2)
                )
            
            // Wallet Info
            VStack(alignment: .leading, spacing: 4) {
                Text(wallet.name)
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Text(wallet.shortAddress)
                    .font(.system(.caption, design: .monospaced))
                    .foregroundColor(.secondary)
                
                NetworkBadge(network: wallet.network)
            }
            
            Spacer()
            
            // Status
            VStack(alignment: .trailing, spacing: 4) {
                Circle()
                    .fill(wallet.isActive ? Color.green : Color.gray)
                    .frame(width: 8, height: 8)
                
                Text(wallet.isActive ? "Active" : "Inactive")
                    .font(.caption2)
                    .foregroundColor(.secondary)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
}

// MARK: - Filter Chip
struct FilterChip: View {
    let title: String
    let icon: String?
    let isSelected: Bool
    let action: () -> Void
    
    init(title: String, icon: String? = nil, isSelected: Bool, action: @escaping () -> Void) {
        self.title = title
        self.icon = icon
        self.isSelected = isSelected
        self.action = action
    }
    
    var body: some View {
        Button(action: action) {
            HStack(spacing: 6) {
                if let icon = icon {
                    Text(icon)
                        .font(.caption)
                }
                
                Text(title)
                    .font(.caption)
                    .fontWeight(.medium)
            }
            .padding(.horizontal, 12)
            .padding(.vertical, 8)
            .background(isSelected ? Color.blue.opacity(0.2) : Color(.systemGray6))
            .foregroundColor(isSelected ? .blue : .primary)
            .cornerRadius(20)
            .overlay(
                RoundedRectangle(cornerRadius: 20)
                    .stroke(isSelected ? Color.blue : Color.clear, lineWidth: 1)
            )
        }
        .buttonStyle(PlainButtonStyle())
    }
}

// MARK: - Sample Transaction Row
struct SampleTransactionRow: View {
    let type: String
    let description: String
    let amount: String
    let network: BlockchainNetwork
    
    var body: some View {
        HStack(spacing: 16) {
            Circle()
                .fill(network.color.opacity(0.2))
                .frame(width: 40, height: 40)
                .overlay(
                    Text(network.icon)
                        .font(.title3)
                )
            
            VStack(alignment: .leading, spacing: 4) {
                Text(type)
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Text(description)
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                NetworkBadge(network: network)
            }
            
            Spacer()
            
            Text(amount)
                .font(.headline)
                .fontWeight(.semibold)
                .foregroundColor(amount.hasPrefix("-") ? .red : .green)
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
}

// MARK: - Tab Button (Enhanced)
struct TabButton: View {
    let title: String
    let isSelected: Bool
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            Text(title)
                .font(.subheadline)
                .fontWeight(isSelected ? .semibold : .regular)
                .foregroundColor(isSelected ? .blue : .secondary)
                .frame(maxWidth: .infinity)
                .padding(.vertical, 12)
                .background(
                    Rectangle()
                        .fill(isSelected ? Color.blue.opacity(0.1) : Color.clear)
                        .overlay(
                            Rectangle()
                                .fill(Color.blue)
                                .frame(height: 2),
                            alignment: .bottom
                        )
                        .opacity(isSelected ? 1 : 0)
                )
        }
    }
}

struct EnhancedWalletView_Previews: PreviewProvider {
    static var previews: some View {
        EnhancedWalletView()
    }
}
