//
//  BridgeView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-19.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

enum BridgeNetwork: String, CaseIterable {
    case usdtgverse = "USDTgVerse"
    case ethereum = "Ethereum"
    case bnb = "BNB Chain"
    case polygon = "Polygon"
    case arbitrum = "Arbitrum"
    case tron = "TRON"
    case solana = "Solana"
    case avalanche = "Avalanche"
    case optimism = "Optimism"
    
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
}

struct BridgeView: View {
    @State private var fromNetwork: BridgeNetwork = .ethereum
    @State private var toNetwork: BridgeNetwork = .usdtgverse
    @State private var amount = ""
    @State private var isTransferring = false
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 24) {
                    // Bridge Header
                    VStack(spacing: 8) {
                        Image(systemName: "arrow.left.arrow.right.square.fill")
                            .font(.system(size: 48))
                            .foregroundColor(.blue)
                        
                        Text("Cross-Chain Bridge")
                            .font(.title)
                            .fontWeight(.bold)
                        
                        Text("Transfer assets between 8 blockchain networks")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                            .multilineTextAlignment(.center)
                    }
                    .padding(.top)
                    
                    // Network Selection
                    VStack(spacing: 16) {
                        // From Network
                        VStack(alignment: .leading, spacing: 8) {
                            Text("From Network")
                                .font(.headline)
                            
                            Picker("From Network", selection: $fromNetwork) {
                                ForEach(BridgeNetwork.allCases, id: \.self) { network in
                                    HStack {
                                        Text(network.icon)
                                        Text(network.rawValue)
                                    }
                                    .tag(network)
                                }
                            }
                            .pickerStyle(MenuPickerStyle())
                            .padding()
                            .background(Color(.systemGray6))
                            .cornerRadius(12)
                        }
                        
                        // Swap Button
                        Button(action: swapNetworks) {
                            Image(systemName: "arrow.up.arrow.down")
                                .font(.title2)
                                .foregroundColor(.blue)
                                .padding(12)
                                .background(Color(.systemGray6))
                                .cornerRadius(8)
                        }
                        
                        // To Network
                        VStack(alignment: .leading, spacing: 8) {
                            Text("To Network")
                                .font(.headline)
                            
                            Picker("To Network", selection: $toNetwork) {
                                ForEach(BridgeNetwork.allCases, id: \.self) { network in
                                    HStack {
                                        Text(network.icon)
                                        Text(network.rawValue)
                                    }
                                    .tag(network)
                                }
                            }
                            .pickerStyle(MenuPickerStyle())
                            .padding()
                            .background(Color(.systemGray6))
                            .cornerRadius(12)
                        }
                    }
                    
                    // Amount Input
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Amount")
                            .font(.headline)
                        
                        TextField("0.00", text: $amount)
                            .keyboardType(.decimalPad)
                            .padding()
                            .background(Color(.systemGray6))
                            .cornerRadius(12)
                            .font(.title2)
                    }
                    
                    // Transfer Info
                    VStack(spacing: 12) {
                        HStack {
                            Text("Estimated Time:")
                            Spacer()
                            Text(estimatedTime)
                                .fontWeight(.semibold)
                        }
                        
                        HStack {
                            Text("Network Fee:")
                            Spacer()
                            Text("~0.1 USDTg")
                                .fontWeight(.semibold)
                        }
                        
                        HStack {
                            Text("Exchange Rate:")
                            Spacer()
                            Text("1:1")
                                .fontWeight(.semibold)
                        }
                    }
                    .padding()
                    .background(Color(.systemGray6))
                    .cornerRadius(12)
                    
                    // Transfer Button
                    Button(action: initiateTransfer) {
                        HStack {
                            if isTransferring {
                                ProgressView()
                                    .progressViewStyle(CircularProgressViewStyle(tint: .white))
                                    .scaleEffect(0.8)
                            } else {
                                Image(systemName: "arrow.left.arrow.right.square.fill")
                            }
                            
                            Text(isTransferring ? "Processing..." : "Initiate Transfer")
                        }
                        .frame(maxWidth: .infinity)
                        .padding()
                        .background(amount.isEmpty ? Color.gray : Color.blue)
                        .foregroundColor(.white)
                        .cornerRadius(12)
                        .font(.headline)
                    }
                    .disabled(amount.isEmpty || isTransferring)
                    
                    Spacer()
                }
                .padding()
            }
            .navigationTitle("Bridge")
            .navigationBarTitleDisplayMode(.inline)
        }
    }
    
    private var estimatedTime: String {
        switch toNetwork {
        case .ethereum: return "15 min"
        case .bnb: return "3 sec"
        case .polygon: return "2 sec"
        case .arbitrum: return "10 min"
        case .tron: return "3 min"
        case .solana: return "30 sec"
        case .avalanche: return "90 sec"
        case .optimism: return "7 min"
        case .usdtgverse: return "1 sec"
        }
    }
    
    private func swapNetworks() {
        let temp = fromNetwork
        fromNetwork = toNetwork
        toNetwork = temp
    }
    
    private func initiateTransfer() {
        guard !amount.isEmpty else { return }
        
        isTransferring = true
        
        // Simulate transfer process
        DispatchQueue.main.asyncAfter(deadline: .now() + 2) {
            isTransferring = false
            // Show success message or handle result
        }
    }
}

struct BridgeView_Previews: PreviewProvider {
    static var previews: some View {
        BridgeView()
    }
}