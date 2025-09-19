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
    
    var body: some View {
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
            SettingsView()
                .tabItem {
                    Image(systemName: "gearshape.fill")
                    Text("Settings")
                }
                .tag(3)
        }
        .accentColor(Color(red: 0.3, green: 0.7, blue: 0.3)) // USDTgVerse green
        .onAppear {
            setupTabBarAppearance()
        }
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
                            .font(.system(size: 12, family: .monospaced))
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
                            .font(.system(size: 14, family: .monospaced))
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
