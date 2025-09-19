//
//  WalletView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-19.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

struct WalletView: View {
    @EnvironmentObject var walletManager: WalletManager
    @EnvironmentObject var networkManager: NetworkManager
    @State private var showingSendView = false
    @State private var showingReceiveView = false
    @State private var showingSwapView = false
    
    var totalPortfolioValue: Double {
        walletManager.assets.reduce(0) { $0 + $1.totalValue }
    }
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    // Header with USDTgVerse branding
                    headerView
                    
                    // Portfolio Overview
                    portfolioCard
                    
                    // Quick Actions
                    quickActionsView
                    
                    // Assets List
                    assetsListView
                    
                    // Recent Transactions
                    recentTransactionsView
                }
                .padding()
            }
            .navigationTitle("USDTgWallet")
            .navigationBarTitleDisplayMode(.large)
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
            SendView()
        }
        .sheet(isPresented: $showingReceiveView) {
            ReceiveView()
        }
        .sheet(isPresented: $showingSwapView) {
            SwapView()
        }
    }
    
    private var headerView: some View {
        HStack {
            VStack(alignment: .leading) {
                Text("USDTgVerse")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(Color(red: 0.3, green: 0.7, blue: 0.3))
                
                Text("Enterprise Blockchain")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            VStack(alignment: .trailing) {
                Circle()
                    .fill(networkManager.isConnected ? .green : .red)
                    .frame(width: 12, height: 12)
                
                Text("Block #\(networkManager.blockHeight)")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private var portfolioCard: some View {
        VStack(spacing: 16) {
            Text("Total Portfolio Value")
                .font(.headline)
                .foregroundColor(.secondary)
            
            Text("$\(totalPortfolioValue, specifier: "%.2f")")
                .font(.system(size: 36, weight: .bold, design: .rounded))
                .foregroundColor(.white)
            
            HStack {
                Text("USDTg: \(walletManager.usdtgBalance, specifier: "%.2f")")
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
                title: "OdixPay",
                color: .purple
            ) {
                // Navigate to OdixPay
            }
        }
    }
    
    private var assetsListView: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("Assets")
                .font(.headline)
                .padding(.horizontal)
            
            ForEach(walletManager.assets) { asset in
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
            
            ForEach(walletManager.transactions.prefix(5)) { transaction in
                TransactionRowView(transaction: transaction)
            }
        }
        .padding(.vertical)
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
}

struct QuickActionButton: View {
    let icon: String
    let title: String
    let color: Color
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            VStack(spacing: 8) {
                Image(systemName: icon)
                    .font(.system(size: 24))
                    .foregroundColor(color)
                
                Text(title)
                    .font(.caption)
                    .foregroundColor(.white)
            }
            .frame(maxWidth: .infinity)
            .padding()
            .background(Color.secondary.opacity(0.2))
            .cornerRadius(12)
        }
    }
}

struct AssetRowView: View {
    let asset: WalletAsset
    
    var body: some View {
        HStack {
            // Asset icon
            Circle()
                .fill(Color(red: 0.3, green: 0.7, blue: 0.3))
                .frame(width: 40, height: 40)
                .overlay(
                    Text(String(asset.symbol.prefix(2)))
                        .font(.caption)
                        .fontWeight(.bold)
                        .foregroundColor(.white)
                )
            
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

struct TransactionRowView: View {
    let transaction: Transaction
    
    var body: some View {
        HStack {
            Image(systemName: transaction.type.icon)
                .font(.system(size: 20))
                .foregroundColor(transaction.type.color)
                .frame(width: 30)
            
            VStack(alignment: .leading, spacing: 4) {
                Text(transactionTitle)
                    .font(.subheadline)
                    .foregroundColor(.white)
                
                Text(transactionSubtitle)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            VStack(alignment: .trailing, spacing: 4) {
                Text("\(transaction.type == .sent ? "-" : "+")\(transaction.amount, specifier: "%.6f") \(transaction.asset)")
                    .font(.subheadline)
                    .foregroundColor(transaction.type == .sent ? .red : .green)
                
                Text(transaction.timestamp, style: .time)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding(.horizontal)
        .padding(.vertical, 8)
    }
    
    private var transactionTitle: String {
        switch transaction.type {
        case .sent: return "Sent \(transaction.asset)"
        case .received: return "Received \(transaction.asset)"
        case .bridge: return "Bridge Transfer"
        case .odixpay: return "OdixPay Payment"
        }
    }
    
    private var transactionSubtitle: String {
        if let from = transaction.from {
            return "From: \(String(from.prefix(12)))..."
        } else if let to = transaction.to {
            return "To: \(String(to.prefix(12)))..."
        } else {
            return "Transaction ID: \(String(transaction.id.prefix(8)))..."
        }
    }
}

#Preview {
    ContentView()
        .environmentObject(WalletManager())
        .environmentObject(NetworkManager())
}
