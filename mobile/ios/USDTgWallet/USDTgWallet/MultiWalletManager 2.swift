//
//  MultiWalletManager.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI
import Foundation

// MARK: - Wallet Data Model
struct WalletData: Codable, Identifiable {
    let id: String
    let name: String
    let address: String
    let mnemonic: String?
    let createdAt: Date
    var balance: Double
    var tokens: [WalletToken]
    
    init(name: String, mnemonic: String? = nil) {
        self.id = UUID().uuidString
        self.name = name
        self.address = "0x" + String(UUID().uuidString.replacingOccurrences(of: "-", with: "").prefix(40))
        self.mnemonic = mnemonic
        self.createdAt = Date()
        self.balance = 10.0 // 10 USDTg welcome bonus for new wallets
        self.tokens = [
            WalletToken(symbol: "USDTg", name: "USDTg", balance: 10.0, price: 1.0, isNative: true),
            WalletToken(symbol: "USDT", name: "Tether USD", balance: 0.0, price: 1.0, isNative: false),
            WalletToken(symbol: "USDC", name: "USD Coin", balance: 0.0, price: 1.0, isNative: false),
            WalletToken(symbol: "ETH", name: "Ethereum", balance: 0.0, price: 2337.85, isNative: false),
            WalletToken(symbol: "BTC", name: "Bitcoin", balance: 0.0, price: 45000.0, isNative: false)
        ]
    }
    
    var totalValue: Double {
        return tokens.reduce(0) { $0 + ($1.balance * $1.price) }
    }
    
    var formattedTotalValue: String {
        return String(format: "$%.2f", totalValue)
    }
}

// MARK: - Wallet Token Model
struct WalletToken: Codable, Identifiable {
    let id = UUID()
    let symbol: String
    let name: String
    var balance: Double
    let price: Double
    let isNative: Bool
    
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

// MARK: - Multi Wallet Manager
class MultiWalletManager: ObservableObject {
    @Published var wallets: [WalletData] = []
    @Published var currentWallet: WalletData?
    @Published var showingWalletList = false
    
    private let walletsKey = "SavedWallets"
    private let currentWalletKey = "CurrentWalletID"
    
    init() {
        loadWallets()
    }
    
    // MARK: - Wallet Management
    func createWallet(name: String, mnemonic: String? = nil) {
        let newWallet = WalletData(name: name, mnemonic: mnemonic)
        wallets.append(newWallet)
        currentWallet = newWallet
        saveWallets()
        
        // Update legacy UserDefaults for compatibility
        UserDefaults.standard.set(true, forKey: "hasWallet")
        UserDefaults.standard.set(name, forKey: "walletName")
    }
    
    func importWallet(name: String, mnemonic: String) {
        var importedWallet = WalletData(name: name, mnemonic: mnemonic)
        
        // Simulate imported wallet with some existing balance
        importedWallet.balance = 25.0 // Imported wallets might have existing balance
        importedWallet.tokens = [
            WalletToken(symbol: "USDTg", name: "USDTg", balance: 25.0, price: 1.0, isNative: true),
            WalletToken(symbol: "USDT", name: "Tether USD", balance: 100.0, price: 1.0, isNative: false),
            WalletToken(symbol: "ETH", name: "Ethereum", balance: 0.5, price: 2337.85, isNative: false),
            WalletToken(symbol: "BTC", name: "Bitcoin", balance: 0.001, price: 45000.0, isNative: false)
        ]
        
        wallets.append(importedWallet)
        currentWallet = importedWallet
        saveWallets()
        
        // Update legacy UserDefaults for compatibility
        UserDefaults.standard.set(true, forKey: "hasWallet")
        UserDefaults.standard.set(name, forKey: "walletName")
    }
    
    func switchToWallet(_ wallet: WalletData) {
        currentWallet = wallet
        UserDefaults.standard.set(wallet.id, forKey: currentWalletKey)
        UserDefaults.standard.set(wallet.name, forKey: "walletName")
    }
    
    func deleteWallet(_ wallet: WalletData) {
        wallets.removeAll { $0.id == wallet.id }
        
        if currentWallet?.id == wallet.id {
            currentWallet = wallets.first
        }
        
        if wallets.isEmpty {
            UserDefaults.standard.removeObject(forKey: "hasWallet")
            currentWallet = nil
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
            // Check if we have legacy wallet
            if UserDefaults.standard.bool(forKey: "hasWallet") {
                let legacyName = UserDefaults.standard.string(forKey: "walletName") ?? "My Wallet"
                createDemoWallet(name: legacyName)
            }
            return
        }
        
        do {
            wallets = try JSONDecoder().decode([WalletData].self, from: data)
            
            // Load current wallet
            let currentWalletID = UserDefaults.standard.string(forKey: currentWalletKey)
            currentWallet = wallets.first { $0.id == currentWalletID } ?? wallets.first
        } catch {
            print("Failed to load wallets: \(error)")
            // Create demo wallet as fallback
            createDemoWallet(name: "My Wallet")
        }
    }
    
    private func createDemoWallet(name: String) {
        var demoWallet = WalletData(name: name)
        
        // Demo wallet with higher balance for demonstration
        demoWallet.balance = 27802.28
        demoWallet.tokens = [
            WalletToken(symbol: "USDTg", name: "USDTg", balance: 10000.0, price: 1.0, isNative: true),
            WalletToken(symbol: "USDT", name: "Tether USD", balance: 5000.0, price: 1.0, isNative: false),
            WalletToken(symbol: "USDC", name: "USD Coin", balance: 2500.0, price: 1.0, isNative: false),
            WalletToken(symbol: "BTC", name: "Bitcoin", balance: 0.15, price: 45000.0, isNative: false),
            WalletToken(symbol: "ETH", name: "Ethereum", balance: 1.5, price: 2337.85, isNative: false),
            WalletToken(symbol: "TRX", name: "TRON", balance: 500.0, price: 0.091, isNative: false)
        ]
        
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
    
    var currentWalletBalance: String {
        return currentWallet?.formattedTotalValue ?? "$0.00"
    }
}

// MARK: - Wallet List View
struct WalletListView: View {
    @ObservedObject var walletManager: MultiWalletManager
    @Environment(\.presentationMode) var presentationMode
    @State private var showingCreateWallet = false
    @State private var showingImportWallet = false
    
    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                // Current Wallet Header
                if let currentWallet = walletManager.currentWallet {
                    VStack(spacing: 12) {
                        HStack {
                            Image("usdtg_logo")
                                .resizable()
                                .aspectRatio(contentMode: .fit)
                                .frame(width: 40, height: 40)
                                .clipShape(Circle())
                            
                            VStack(alignment: .leading, spacing: 4) {
                                Text("Current Wallet")
                                    .font(.caption)
                                    .foregroundColor(.secondary)
                                
                                Text(currentWallet.name)
                                    .font(.headline)
                                    .fontWeight(.semibold)
                            }
                            
                            Spacer()
                            
                            Text(currentWallet.formattedTotalValue)
                                .font(.title2)
                                .fontWeight(.bold)
                                .foregroundColor(.green)
                        }
                        .padding()
                        .background(Color(.systemGray6))
                        .cornerRadius(12)
                    }
                    .padding()
                }
                
                // Wallet List
                List {
                    Section("All Wallets") {
                        ForEach(walletManager.wallets) { wallet in
                            WalletRowView(
                                wallet: wallet,
                                isSelected: wallet.id == walletManager.currentWallet?.id,
                                onSelect: {
                                    walletManager.switchToWallet(wallet)
                                    presentationMode.wrappedValue.dismiss()
                                }
                            )
                        }
                        .onDelete(perform: deleteWallets)
                    }
                }
                
                // Action Buttons
                VStack(spacing: 12) {
                    Button("Create New Wallet") {
                        showingCreateWallet = true
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.blue)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                    .font(.headline)
                    
                    Button("Import Wallet") {
                        showingImportWallet = true
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.green)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                    .font(.headline)
                }
                .padding()
            }
            .navigationTitle("My Wallets")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
        }
        .sheet(isPresented: $showingCreateWallet) {
            NewWalletSheet(walletManager: walletManager)
        }
        .sheet(isPresented: $showingImportWallet) {
            ImportWalletSheet(walletManager: walletManager)
        }
    }
    
    private func deleteWallets(offsets: IndexSet) {
        for index in offsets {
            walletManager.deleteWallet(walletManager.wallets[index])
        }
    }
}

// MARK: - Wallet Row View
struct WalletRowView: View {
    let wallet: WalletData
    let isSelected: Bool
    let onSelect: () -> Void
    
    var body: some View {
        Button(action: onSelect) {
            HStack(spacing: 16) {
                // Wallet Icon
                Circle()
                    .fill(isSelected ? Color.green : Color.blue)
                    .frame(width: 40, height: 40)
                    .overlay(
                        Image(systemName: isSelected ? "checkmark" : "wallet.pass")
                            .foregroundColor(.white)
                            .font(.headline)
                    )
                
                // Wallet Info
                VStack(alignment: .leading, spacing: 4) {
                    Text(wallet.name)
                        .font(.headline)
                        .fontWeight(.semibold)
                        .foregroundColor(.primary)
                    
                    Text(String(wallet.address.prefix(20)) + "...")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("Created: \(wallet.createdAt, formatter: dateFormatter)")
                        .font(.caption2)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                // Balance
                VStack(alignment: .trailing, spacing: 4) {
                    Text(wallet.formattedTotalValue)
                        .font(.headline)
                        .fontWeight(.semibold)
                        .foregroundColor(.primary)
                    
                    Text("\(wallet.tokens.count) tokens")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
            .padding(.vertical, 8)
        }
        .buttonStyle(PlainButtonStyle())
    }
    
    private var dateFormatter: DateFormatter {
        let formatter = DateFormatter()
        formatter.dateStyle = .short
        formatter.timeStyle = .none
        return formatter
    }
}

// MARK: - Updated Wallet Creation Sheets
struct NewWalletSheet: View {
    @ObservedObject var walletManager: MultiWalletManager
    @Environment(\.presentationMode) var presentationMode
    @State private var walletName = ""
    @State private var isCreating = false
    
    var body: some View {
        NavigationView {
            VStack(spacing: 30) {
                VStack(spacing: 16) {
                    Image(systemName: "plus.circle.fill")
                        .font(.system(size: 60))
                        .foregroundColor(.blue)
                    
                    Text("Create New Wallet")
                        .font(.title2)
                        .fontWeight(.bold)
                    
                    Text("Generate a new wallet and get 10 USDTg bonus!")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                
                VStack(alignment: .leading, spacing: 8) {
                    Text("Wallet Name")
                        .font(.headline)
                        .fontWeight(.medium)
                    
                    TextField("My New Wallet", text: $walletName)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                }
                
                Button(action: {
                    isCreating = true
                    DispatchQueue.main.asyncAfter(deadline: .now() + 2) {
                        walletManager.createWallet(name: walletName)
                        isCreating = false
                        presentationMode.wrappedValue.dismiss()
                    }
                }) {
                    HStack {
                        if isCreating {
                            ProgressView()
                                .progressViewStyle(CircularProgressViewStyle(tint: .white))
                                .scaleEffect(0.8)
                        }
                        Text("Create Wallet & Get 10 USDTg")
                            .font(.headline)
                            .fontWeight(.semibold)
                    }
                    .foregroundColor(.white)
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(walletName.isEmpty ? Color.gray : Color.blue)
                    .cornerRadius(12)
                }
                .disabled(walletName.isEmpty || isCreating)
                
                Spacer()
            }
            .padding()
            .navigationTitle("New Wallet")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Cancel") {
                presentationMode.wrappedValue.dismiss()
            })
        }
    }
}

struct ImportWalletSheet: View {
    @ObservedObject var walletManager: MultiWalletManager
    @Environment(\.presentationMode) var presentationMode
    @State private var walletName = ""
    @State private var seedPhrase = ""
    @State private var isImporting = false
    
    var body: some View {
        NavigationView {
            VStack(spacing: 30) {
                VStack(spacing: 16) {
                    Image(systemName: "square.and.arrow.down.fill")
                        .font(.system(size: 60))
                        .foregroundColor(.green)
                    
                    Text("Import Existing Wallet")
                        .font(.title2)
                        .fontWeight(.bold)
                    
                    Text("Import your wallet using seed phrase")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                
                VStack(alignment: .leading, spacing: 16) {
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Wallet Name")
                            .font(.headline)
                            .fontWeight(.medium)
                        
                        TextField("Imported Wallet", text: $walletName)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                    }
                    
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Seed Phrase (12 or 24 words)")
                            .font(.headline)
                            .fontWeight(.medium)
                        
                        TextEditor(text: $seedPhrase)
                            .frame(height: 120)
                            .padding(8)
                            .background(Color(.systemGray6))
                            .cornerRadius(8)
                            .overlay(
                                RoundedRectangle(cornerRadius: 8)
                                    .stroke(Color(.systemGray4), lineWidth: 1)
                            )
                    }
                }
                
                Button(action: {
                    isImporting = true
                    DispatchQueue.main.asyncAfter(deadline: .now() + 2) {
                        walletManager.importWallet(name: walletName, mnemonic: seedPhrase)
                        isImporting = false
                        presentationMode.wrappedValue.dismiss()
                    }
                }) {
                    HStack {
                        if isImporting {
                            ProgressView()
                                .progressViewStyle(CircularProgressViewStyle(tint: .white))
                                .scaleEffect(0.8)
                        }
                        Text("Import Wallet")
                            .font(.headline)
                            .fontWeight(.semibold)
                    }
                    .foregroundColor(.white)
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background((walletName.isEmpty || seedPhrase.isEmpty) ? Color.gray : Color.green)
                    .cornerRadius(12)
                }
                .disabled(walletName.isEmpty || seedPhrase.isEmpty || isImporting)
                
                Spacer()
            }
            .padding()
            .navigationTitle("Import Wallet")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Cancel") {
                presentationMode.wrappedValue.dismiss()
            })
        }
    }
}
