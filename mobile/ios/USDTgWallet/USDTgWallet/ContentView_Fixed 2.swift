//
//  ContentView_Fixed.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

struct ContentView_Fixed: View {
    @EnvironmentObject var walletManager: WalletManager
    @EnvironmentObject var networkManager: NetworkManager
    @StateObject private var multiWalletManager = MultiWalletManager()
    @State private var selectedTab = 0
    @State private var showingWalletSetup = false
    @State private var showingNewWallet = false
    @State private var showingImportWallet = false
    
    var body: some View {
        Group {
            if multiWalletManager.hasWallets {
                mainTabView
            } else {
                walletSetupView
            }
        }
        .onAppear {
            setupTabBarAppearance()
        }
        .sheet(isPresented: $showingWalletSetup) {
            SimpleWalletSetupSheet_Fixed(walletManager: multiWalletManager)
        }
        .sheet(isPresented: $showingNewWallet) {
            NewWalletSheet_Fixed(walletManager: multiWalletManager)
        }
        .sheet(isPresented: $showingImportWallet) {
            ImportWalletSheet_Fixed(walletManager: multiWalletManager)
        }
    }
    
    private var walletSetupView: some View {
        VStack(spacing: 30) {
            // USDTgVerse Logo
            VStack(spacing: 16) {
                Image("usdtg_logo")
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: 100, height: 100)
                    .clipShape(Circle())
                    .overlay(
                        Circle()
                            .stroke(LinearGradient(gradient: Gradient(colors: [Color.green, Color.blue]), startPoint: .topLeading, endPoint: .bottomTrailing), lineWidth: 4)
                    )
                
                Text("Welcome to USDTgVerse")
                    .font(.title)
                    .fontWeight(.bold)
                
                Text("Create or import your multi-chain wallet")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                    .multilineTextAlignment(.center)
            }
            
            // Action Buttons
            VStack(spacing: 16) {
                Button("Create New Wallet") {
                    showingNewWallet = true
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color.blue)
                .foregroundColor(.white)
                .cornerRadius(12)
                .font(.headline)
                
                Button("Import Existing Wallet") {
                    showingImportWallet = true
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color.green)
                .foregroundColor(.white)
                .cornerRadius(12)
                .font(.headline)
                
                Button("Show Setup Options") {
                    showingWalletSetup = true
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color.orange)
                .foregroundColor(.white)
                .cornerRadius(12)
                .font(.headline)
            }
            
            Spacer()
        }
        .padding()
    }
    
    private var mainTabView: some View {
        TabView(selection: $selectedTab) {
            // Wallet Tab
            RealWalletView(multiWalletManager: multiWalletManager)
                .tabItem {
                    Image(systemName: "wallet.pass.fill")
                    Text("Wallet")
                }
                .tag(0)
            
            // USDTgVerse PAY Tab
            USDTgVersePayView()
                .tabItem {
                    Image(systemName: "creditcard.fill")
                    Text("USDTgVerse PAY")
                }
                .tag(1)
            
            // Bridge Tab
            BridgeView()
                .tabItem {
                    Image(systemName: "arrow.left.arrow.right.circle.fill")
                    Text("Bridge")
                }
                .tag(2)
            
            // Settings Tab
            NavigationView {
                VStack(spacing: 20) {
                    Text("Settings")
                        .font(.largeTitle)
                        .fontWeight(.bold)
                        .padding()
                    
                    VStack(spacing: 16) {
                        // Add New Wallet
                        Button(action: {
                            showingNewWallet = true
                        }) {
                            HStack(spacing: 16) {
                                Image(systemName: "plus.circle.fill")
                                    .font(.title2)
                                    .foregroundColor(.blue)
                                
                                VStack(alignment: .leading, spacing: 4) {
                                    Text("Add New Wallet")
                                        .font(.headline)
                                        .fontWeight(.semibold)
                                    
                                    Text("Create or import another wallet")
                                        .font(.caption)
                                        .foregroundColor(.secondary)
                                }
                                
                                Spacer()
                                
                                Image(systemName: "chevron.right")
                                    .foregroundColor(.secondary)
                            }
                            .foregroundColor(.primary)
                            .padding()
                            .background(Color(.systemGray6))
                            .cornerRadius(12)
                        }
                        
                        // Manage Wallets
                        Button(action: {
                            multiWalletManager.showingWalletList = true
                        }) {
                            HStack(spacing: 16) {
                                Image(systemName: "list.bullet.circle.fill")
                                    .font(.title2)
                                    .foregroundColor(.purple)
                                
                                VStack(alignment: .leading, spacing: 4) {
                                    Text("Manage Wallets")
                                        .font(.headline)
                                        .fontWeight(.semibold)
                                    
                                    Text("Switch between your wallets")
                                        .font(.caption)
                                        .foregroundColor(.secondary)
                                }
                                
                                Spacer()
                                
                                Image(systemName: "chevron.right")
                                    .foregroundColor(.secondary)
                            }
                            .foregroundColor(.primary)
                            .padding()
                            .background(Color(.systemGray6))
                            .cornerRadius(12)
                        }
                    }
                    .padding()
                    
                    Spacer()
                }
            }
            .tabItem {
                Image(systemName: "gearshape.fill")
                Text("Settings")
            }
            .tag(3)
        }
        .accentColor(Color(red: 0.3, green: 0.7, blue: 0.3))
        .sheet(isPresented: $multiWalletManager.showingWalletList) {
            WalletListView(walletManager: multiWalletManager)
        }
    }
    
    private func setupTabBarAppearance() {
        let appearance = UITabBarAppearance()
        appearance.configureWithOpaqueBackground()
        appearance.backgroundColor = UIColor.systemBackground
        
        UITabBar.appearance().standardAppearance = appearance
        UITabBar.appearance().scrollEdgeAppearance = appearance
    }
}

// MARK: - Real Wallet View (using MultiWalletManager)
struct RealWalletView: View {
    @ObservedObject var multiWalletManager: MultiWalletManager
    @State private var selectedTab = 0
    
    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                // Portfolio Summary with clickable wallet selector
                portfolioSummaryView
                
                // Tab Selector
                tabSelectorView
                
                // Content
                tabContentView
                
                Spacer()
            }
            .navigationTitle("My Wallet")
            .navigationBarTitleDisplayMode(.inline)
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
                            .stroke(LinearGradient(gradient: Gradient(colors: [Color.green, Color.blue]), startPoint: .topLeading, endPoint: .bottomTrailing), lineWidth: 2)
                    )
                
                Button(action: {
                    multiWalletManager.showingWalletList = true
                }) {
                    HStack(spacing: 8) {
                        Text(multiWalletManager.currentWalletName)
                            .font(.headline)
                            .fontWeight(.semibold)
                            .foregroundColor(.primary)
                        
                        Image(systemName: "chevron.down")
                            .font(.caption)
                            .foregroundColor(.secondary)
                    }
                }
            }
            
            // Portfolio Value
            VStack(spacing: 4) {
                Text("Total Portfolio")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Text(multiWalletManager.currentWalletBalance)
                    .font(.largeTitle)
                    .fontWeight(.bold)
                    .foregroundColor(.primary)
                    .contentTransition(.numericText())
                
                if let currentWallet = multiWalletManager.currentWallet {
                    Text("\(currentWallet.tokens.count) Assets • \(multiWalletManager.wallets.count) Wallets")
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
    
    private var tabSelectorView: some View {
        HStack(spacing: 0) {
            TabButton(title: "Assets", isSelected: selectedTab == 0) {
                selectedTab = 0
            }
            
            TabButton(title: "Send", isSelected: selectedTab == 1) {
                selectedTab = 1
            }
            
            TabButton(title: "Receive", isSelected: selectedTab == 2) {
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
            realAssetsView
        case 1:
            sendView
        case 2:
            receiveView
        default:
            historyView
        }
    }
    
    private var realAssetsView: some View {
        ScrollView {
            LazyVStack(spacing: 12) {
                if let currentWallet = multiWalletManager.currentWallet {
                    ForEach(currentWallet.tokens) { token in
                        RealTokenRow(token: token)
                    }
                }
            }
            .padding()
        }
    }
    
    private var sendView: some View {
        VStack(spacing: 20) {
            Image(systemName: "arrow.up.right.circle.fill")
                .font(.system(size: 64))
                .foregroundColor(.blue)
            
            Text("Send Assets")
                .font(.title2)
                .fontWeight(.semibold)
            
            Text("Coming soon...")
                .foregroundColor(.secondary)
        }
    }
    
    private var receiveView: some View {
        VStack(spacing: 20) {
            Image(systemName: "arrow.down.left.circle.fill")
                .font(.system(size: 64))
                .foregroundColor(.green)
            
            Text("Receive Assets")
                .font(.title2)
                .fontWeight(.semibold)
            
            if let currentWallet = multiWalletManager.currentWallet {
                VStack(spacing: 12) {
                    Text("Your Wallet Address:")
                        .font(.headline)
                    
                    Text(currentWallet.address)
                        .font(.caption)
                        .foregroundColor(.secondary)
                        .padding()
                        .background(Color(.systemGray6))
                        .cornerRadius(8)
                }
            }
        }
        .padding()
    }
    
    private var historyView: some View {
        VStack(spacing: 20) {
            Image(systemName: "clock.circle.fill")
                .font(.system(size: 64))
                .foregroundColor(.purple)
            
            Text("Transaction History")
                .font(.title2)
                .fontWeight(.semibold)
            
            Text("No transactions yet")
                .foregroundColor(.secondary)
        }
    }
}

// MARK: - Real Token Row
struct RealTokenRow: View {
    let token: WalletToken
    
    var body: some View {
        HStack(spacing: 16) {
            // Token Logo
            Group {
                if token.symbol == "USDTg" {
                    Image("usdtg_logo")
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                        .frame(width: 32, height: 32)
                        .clipShape(Circle())
                        .overlay(
                            Circle()
                                .stroke(Color.green, lineWidth: 1.5)
                        )
                } else {
                    Circle()
                        .fill(tokenColor(for: token.symbol))
                        .frame(width: 32, height: 32)
                        .overlay(
                            Text(tokenSymbol(for: token.symbol))
                                .font(.headline)
                                .fontWeight(.bold)
                                .foregroundColor(.white)
                        )
                }
            }
            
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
                
                Text(token.isNative ? "Native • $1.00 FIXED" : token.symbol)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            VStack(alignment: .trailing, spacing: 4) {
                Text(token.formattedBalance)
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Text(token.formattedValue)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
    
    private func tokenColor(for symbol: String) -> Color {
        switch symbol {
        case "BTC": return .orange
        case "ETH": return .purple
        case "USDT": return .green
        case "USDC": return .blue
        case "TRX": return .red
        default: return .gray
        }
    }
    
    private func tokenSymbol(for symbol: String) -> String {
        switch symbol {
        case "BTC": return "₿"
        case "ETH": return "Ξ"
        case "USDT": return "₮"
        case "USDC": return "©"
        case "TRX": return "♦"
        default: return String(symbol.prefix(1))
        }
    }
}

// MARK: - Fixed Wallet Setup Sheets
struct SimpleWalletSetupSheet_Fixed: View {
    @Environment(\.presentationMode) var presentationMode
    @ObservedObject var walletManager: MultiWalletManager
    @State private var showingNewWallet = false
    @State private var showingImportWallet = false
    
    var body: some View {
        NavigationView {
            VStack(spacing: 30) {
                VStack(spacing: 16) {
                    Image("usdtg_logo")
                        .resizable()
                        .aspectRatio(contentMode: .fit)
                        .frame(width: 80, height: 80)
                        .clipShape(Circle())
                        .overlay(
                            Circle()
                                .stroke(LinearGradient(gradient: Gradient(colors: [Color.green, Color.blue]), startPoint: .topLeading, endPoint: .bottomTrailing), lineWidth: 3)
                        )
                    
                    Text("USDTgVerse Wallet Setup")
                        .font(.title2)
                        .fontWeight(.bold)
                    
                    Text("Choose how to set up your wallet")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                }
                
                VStack(spacing: 20) {
                    Button("Create New Wallet") {
                        showingNewWallet = true
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.blue)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                    .font(.headline)
                    
                    Button("Import Existing Wallet") {
                        showingImportWallet = true
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.green)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                    .font(.headline)
                }
                
                Spacer()
            }
            .padding()
            .navigationTitle("Wallet Setup")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Close") {
                presentationMode.wrappedValue.dismiss()
            })
        }
        .sheet(isPresented: $showingNewWallet) {
            NewWalletSheet_Fixed(walletManager: walletManager)
        }
        .sheet(isPresented: $showingImportWallet) {
            ImportWalletSheet_Fixed(walletManager: walletManager)
        }
    }
}

struct NewWalletSheet_Fixed: View {
    @Environment(\.presentationMode) var presentationMode
    @ObservedObject var walletManager: MultiWalletManager
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

struct ImportWalletSheet_Fixed: View {
    @Environment(\.presentationMode) var presentationMode
    @ObservedObject var walletManager: MultiWalletManager
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

private func setupTabBarAppearance() {
    let appearance = UITabBarAppearance()
    appearance.configureWithOpaqueBackground()
    appearance.backgroundColor = UIColor.systemBackground
    
    UITabBar.appearance().standardAppearance = appearance
    UITabBar.appearance().scrollEdgeAppearance = appearance
}

struct ContentView_Fixed_Previews: PreviewProvider {
    static var previews: some View {
        ContentView_Fixed()
            .environmentObject(WalletManager())
            .environmentObject(NetworkManager())
    }
}
