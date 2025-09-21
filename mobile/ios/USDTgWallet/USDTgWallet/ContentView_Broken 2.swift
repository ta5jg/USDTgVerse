//
//  ContentView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-19.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

struct ContentView: View {
    @EnvironmentObject var walletManager: WalletManager
    @EnvironmentObject var networkManager: NetworkManager
    @State private var selectedTab = 0
    @State private var showingWalletSetup = false
    @State private var showingNewWallet = false
    @State private var showingImportWallet = false
    
    var body: some View {
        TabView(selection: $selectedTab) {
        .onAppear {
            setupTabBarAppearance()
        }
        .sheet(isPresented: $showingWalletSetup) {
            SimpleWalletSetupSheet(walletManager: multiWalletManager)
        }
        .sheet(isPresented: $showingNewWallet) {
            NewWalletSheet(walletManager: multiWalletManager)
        }
        .sheet(isPresented: $showingImportWallet) {
            ImportWalletSheet(walletManager: multiWalletManager)
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
            WalletView()
                .tabItem {
                    Image(systemName: "wallet.pass.fill")
                    Text("Wallet")
                }
                .tag(0)
            
            // OdixPay Tab
            OdixPayView()
                .tabItem {
                    Image(systemName: "creditcard.fill")
                    Text("OdixPay++")
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
                        
                        // Import Existing Wallet
                        Button(action: {
                            showingImportWallet = true
                        }) {
                            HStack(spacing: 16) {
                                Image(systemName: "square.and.arrow.down.fill")
                                    .font(.title2)
                                    .foregroundColor(.green)
                                
                                VStack(alignment: .leading, spacing: 4) {
                                    Text("Import Wallet")
                                        .font(.headline)
                                        .fontWeight(.semibold)
                                    
                                    Text("Import with seed phrase or private key")
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
                        
                        // Show Wallet Setup (for testing)
                        Button(action: {
                            showingWalletSetup = true
                        }) {
                            HStack(spacing: 16) {
                                Image(systemName: "arrow.clockwise.circle.fill")
                                    .font(.title2)
                                    .foregroundColor(.orange)
                                
                                VStack(alignment: .leading, spacing: 4) {
                                    Text("Show Wallet Setup")
                                        .font(.headline)
                                        .fontWeight(.semibold)
                                    
                                    Text("Open wallet creation/import screen")
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
        .accentColor(Color(red: 0.3, green: 0.7, blue: 0.3)) // USDTgVerse green
    }
    
    private func setupTabBarAppearance() {
        let appearance = UITabBarAppearance()
        appearance.configureWithOpaqueBackground()
        appearance.backgroundColor = UIColor.black
        
        UITabBar.appearance().standardAppearance = appearance
        UITabBar.appearance().scrollEdgeAppearance = appearance
    }
}

struct SettingsView: View {
    @EnvironmentObject var walletManager: WalletManager
    @EnvironmentObject var networkManager: NetworkManager
    
    var body: some View {
        NavigationView {
            List {
                Section("Network") {
                    HStack {
                        Circle()
                            .fill(networkManager.isConnected ? .green : .red)
                            .frame(width: 12, height: 12)
                        Text(networkManager.currentNetwork)
                        Spacer()
                        Text(networkManager.networkStatus)
                            .foregroundColor(.secondary)
                    }
                    
                    HStack {
                        Text("Block Height")
                        Spacer()
                        Text("#\(networkManager.blockHeight)")
                            .foregroundColor(.secondary)
                    }
                }
                
                Section("Wallet") {
                    HStack {
                        Text("Address")
                        Spacer()
                        Text(walletManager.walletAddress)
                            .foregroundColor(.secondary)
                            .font(.system(size: 12, design: .monospaced))
                    }
                    
                    HStack {
                        Text("Total Assets")
                        Spacer()
                        Text("\(walletManager.assets.count)")
                            .foregroundColor(.secondary)
                    }
                }
                
                Section("Security") {
                    NavigationLink("Backup Wallet") {
                        BackupView()
                    }
                    
                    NavigationLink("Security Settings") {
                        SecurityView()
                    }
                }
                
                Section("About") {
                    HStack {
                        Text("Version")
                        Spacer()
                        Text("1.0.0")
                            .foregroundColor(.secondary)
                    }
                    
                    HStack {
                        Text("Build")
                        Spacer()
                        Text("2025.09.19")
                            .foregroundColor(.secondary)
                    }
                    
                    Link("USDTgVerse.com", destination: URL(string: "https://usdtgverse.com")!)
                        .foregroundColor(Color(red: 0.3, green: 0.7, blue: 0.3))
                }
            }
            .navigationTitle("Settings")
            .navigationBarTitleDisplayMode(.large)
        }
    }
}

struct BackupView: View {
    var body: some View {
        VStack(spacing: 20) {
            Image(systemName: "shield.checkered")
                .font(.system(size: 60))
                .foregroundColor(Color(red: 0.3, green: 0.7, blue: 0.3))
            
            Text("Backup Your Wallet")
                .font(.title2)
                .fontWeight(.bold)
            
            Text("Secure your wallet with a backup phrase. This is the only way to recover your wallet if you lose your device.")
                .multilineTextAlignment(.center)
                .foregroundColor(.secondary)
                .padding(.horizontal)
            
            VStack(alignment: .leading, spacing: 10) {
                Text("12-Word Recovery Phrase:")
                    .font(.headline)
                
                LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 3), spacing: 10) {
                    ForEach(0..<12, id: \.self) { index in
                        Text("word\(index + 1)")
                            .padding(8)
                            .background(Color.secondary.opacity(0.2))
                            .cornerRadius(8)
                            .font(.system(size: 14, design: .monospaced))
                    }
                }
            }
            .padding()
            .background(Color.secondary.opacity(0.1))
            .cornerRadius(12)
            
            Button("I've Saved My Backup Phrase") {
                // Handle backup confirmation
            }
            .buttonStyle(.borderedProminent)
            .tint(Color(red: 0.3, green: 0.7, blue: 0.3))
            
            Spacer()
        }
        .padding()
        .navigationTitle("Backup")
        .navigationBarTitleDisplayMode(.inline)
    }
}

struct SecurityView: View {
    @State private var biometricEnabled = true
    @State private var pinEnabled = false
    
    var body: some View {
        List {
            Section("Authentication") {
                Toggle("Face ID / Touch ID", isOn: $biometricEnabled)
                Toggle("PIN Code", isOn: $pinEnabled)
            }
            
            Section("Advanced") {
                Button("Change PIN") {
                    // Handle PIN change
                }
                
                Button("Reset Wallet") {
                    // Handle wallet reset
                }
                .foregroundColor(.red)
            }
        }
        .navigationTitle("Security")
        .navigationBarTitleDisplayMode(.inline)
    }
}

#Preview {
    ContentView()
        .environmentObject(WalletManager())
        .environmentObject(NetworkManager())
}

// MARK: - Simple Wallet Setup Sheets
struct SimpleWalletSetupSheet: View {
    @Environment(\.presentationMode) var presentationMode
    @ObservedObject var walletManager: MultiWalletManager
    @State private var showingNewWallet = false
    @State private var showingImportWallet = false
    
    var body: some View {
        NavigationView {
            VStack(spacing: 30) {
                // Header
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
                
                // Options
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
            NewWalletSheet(walletManager: walletManager)
        }
        .sheet(isPresented: $showingImportWallet) {
            ImportWalletSheet(walletManager: walletManager)
        }
    }
}

struct NewWalletSheet: View {
    @Environment(\.presentationMode) var presentationMode
    @Binding var hasWallet: Bool
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
                    
                    TextField("My USDTgVerse Wallet", text: $walletName)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                }
                
                Button(action: {
                    isCreating = true
                    // Simulate wallet creation
                    DispatchQueue.main.asyncAfter(deadline: .now() + 2) {
                        // Save wallet to UserDefaults
                        UserDefaults.standard.set(true, forKey: "hasWallet")
                        UserDefaults.standard.set(walletName, forKey: "walletName")
                        UserDefaults.standard.set("demo_seed_phrase_12_words", forKey: "walletMnemonic")
                        
                        // Update state to show main wallet
                        hasWallet = true
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
    @Environment(\.presentationMode) var presentationMode
    @Binding var hasWallet: Bool
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
                
                Button(action: {
                    isImporting = true
                    // Simulate wallet import
                    DispatchQueue.main.asyncAfter(deadline: .now() + 2) {
                        // Save imported wallet to UserDefaults
                        UserDefaults.standard.set(true, forKey: "hasWallet")
                        UserDefaults.standard.set("Imported Wallet", forKey: "walletName")
                        UserDefaults.standard.set(seedPhrase, forKey: "walletMnemonic")
                        
                        // Update state to show main wallet
                        hasWallet = true
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
                    .background(seedPhrase.isEmpty ? Color.gray : Color.green)
                    .cornerRadius(12)
                }
                .disabled(seedPhrase.isEmpty || isImporting)
                
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
