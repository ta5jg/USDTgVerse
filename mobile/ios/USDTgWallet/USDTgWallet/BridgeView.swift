//
//  BridgeView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-19.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

struct BridgeView: View {
    @State private var fromNetwork: BridgeNetwork = .usdtgverse
    @State private var toNetwork: BridgeNetwork = .ethereum
    @State private var amount = ""
    @State private var isTransferring = false
    @State private var showingNetworkSelector = false
    @State private var selectingFromNetwork = true
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 24) {
                    // Bridge Header
                    bridgeHeader
                    
                    // Network Selection
                    networkSelectionView
                    
                    // Amount Input
                    amountInputView
                    
                    // Transfer Details
                    transferDetailsView
                    
                    // Transfer Button
                    transferButton
                    
                    // Recent Bridge Transfers
                    recentTransfersView
                }
                .padding()
            }
            .navigationTitle("Cross-Chain Bridge")
            .navigationBarTitleDisplayMode(.large)
            .background(
                LinearGradient(
                    gradient: Gradient(colors: [
                        Color.blue.opacity(0.1),
                        Color.purple.opacity(0.1)
                    ]),
                    startPoint: .topLeading,
                    endPoint: .bottomTrailing
                )
            )
        }
        .sheet(isPresented: $showingNetworkSelector) {
            NetworkSelectorView(
                selectedNetwork: selectingFromNetwork ? $fromNetwork : $toNetwork,
                isPresented: $showingNetworkSelector
            )
        }
    }
    
    private var bridgeHeader: some View {
        VStack(spacing: 12) {
            Image(systemName: "arrow.left.arrow.right.circle.fill")
                .font(.system(size: 48))
                .foregroundColor(.blue)
            
            Text("Multi-Chain Bridge")
                .font(.title2)
                .fontWeight(.bold)
                .foregroundColor(.white)
            
            Text("Transfer assets across 8 major blockchain networks")
                .font(.subheadline)
                .foregroundColor(.secondary)
                .multilineTextAlignment(.center)
            
            HStack {
                VStack {
                    Text("$116.2M")
                        .font(.headline)
                        .fontWeight(.bold)
                        .foregroundColor(.white)
                    Text("Total Volume")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                VStack {
                    Text("23,616")
                        .font(.headline)
                        .fontWeight(.bold)
                        .foregroundColor(.white)
                    Text("Transfers")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                VStack {
                    Text("99.8%")
                        .font(.headline)
                        .fontWeight(.bold)
                        .foregroundColor(.green)
                    Text("Success Rate")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(16)
    }
    
    private var networkSelectionView: some View {
        VStack(spacing: 16) {
            // From Network
            VStack(alignment: .leading, spacing: 8) {
                Text("From Network")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Button(action: {
                    selectingFromNetwork = true
                    showingNetworkSelector = true
                }) {
                    HStack {
                        Text(fromNetwork.emoji)
                            .font(.title2)
                        
                        VStack(alignment: .leading) {
                            Text(fromNetwork.name)
                                .font(.headline)
                                .foregroundColor(.white)
                            
                            Text(fromNetwork.timeEstimate)
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Spacer()
                        
                        Image(systemName: "chevron.down")
                            .foregroundColor(.secondary)
                    }
                    .padding()
                    .background(Color.secondary.opacity(0.2))
                    .cornerRadius(12)
                }
            }
            
            // Swap button
            Button(action: swapNetworks) {
                Image(systemName: "arrow.up.arrow.down.circle.fill")
                    .font(.system(size: 24))
                    .foregroundColor(.blue)
            }
            
            // To Network
            VStack(alignment: .leading, spacing: 8) {
                Text("To Network")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Button(action: {
                    selectingFromNetwork = false
                    showingNetworkSelector = true
                }) {
                    HStack {
                        Text(toNetwork.emoji)
                            .font(.title2)
                        
                        VStack(alignment: .leading) {
                            Text(toNetwork.name)
                                .font(.headline)
                                .foregroundColor(.white)
                            
                            Text(toNetwork.timeEstimate)
                                .font(.caption)
                                .foregroundColor(.secondary)
                        }
                        
                        Spacer()
                        
                        Image(systemName: "chevron.down")
                            .foregroundColor(.secondary)
                    }
                    .padding()
                    .background(Color.secondary.opacity(0.2))
                    .cornerRadius(12)
                }
            }
        }
    }
    
    private var amountInputView: some View {
        VStack(alignment: .leading, spacing: 8) {
            Text("Amount")
                .font(.subheadline)
                .foregroundColor(.secondary)
            
            HStack {
                TextField("0.00", text: $amount)
                    .keyboardType(.decimalPad)
                    .font(.title2)
                    .fontWeight(.medium)
                    .foregroundColor(.white)
                
                Text(fromNetwork.nativeToken)
                    .font(.headline)
                    .foregroundColor(.secondary)
            }
            .padding()
            .background(Color.secondary.opacity(0.2))
            .cornerRadius(12)
            
            HStack {
                Text("Balance: \(fromNetwork.balance, specifier: "%.6f") \(fromNetwork.nativeToken)")
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                Spacer()
                
                Button("Max") {
                    amount = String(fromNetwork.balance)
                }
                .font(.caption)
                .foregroundColor(.blue)
            }
        }
    }
    
    private var transferDetailsView: some View {
        VStack(spacing: 12) {
            HStack {
                Text("Estimated Time")
                Spacer()
                Text(toNetwork.timeEstimate)
                    .foregroundColor(.secondary)
            }
            
            HStack {
                Text("Bridge Fee")
                Spacer()
                Text("0.1% + \(toNetwork.gasFee)")
                    .foregroundColor(.secondary)
            }
            
            HStack {
                Text("You'll Receive")
                Spacer()
                Text("~\(calculateReceiveAmount()) \(toNetwork.nativeToken)")
                    .foregroundColor(.green)
                    .fontWeight(.medium)
            }
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private var transferButton: some View {
        Button(action: initiateBridgeTransfer) {
            HStack {
                if isTransferring {
                    ProgressView()
                        .progressViewStyle(CircularProgressViewStyle(tint: .white))
                        .scaleEffect(0.8)
                }
                
                Text(isTransferring ? "Processing..." : "Bridge Transfer")
                    .fontWeight(.medium)
            }
            .frame(maxWidth: .infinity)
            .padding()
            .background(
                LinearGradient(
                    gradient: Gradient(colors: [.blue, .purple]),
                    startPoint: .leading,
                    endPoint: .trailing
                )
            )
            .foregroundColor(.white)
            .cornerRadius(12)
        }
        .disabled(amount.isEmpty || isTransferring || fromNetwork == toNetwork)
    }
    
    private var recentTransfersView: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("Recent Bridge Transfers")
                .font(.headline)
                .foregroundColor(.white)
            
            ForEach(0..<3, id: \.self) { index in
                BridgeTransferRow(
                    from: BridgeNetwork.allCases.randomElement()!,
                    to: BridgeNetwork.allCases.randomElement()!,
                    amount: Double.random(in: 0.1...10.0),
                    status: index == 0 ? .completed : .pending
                )
            }
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private func swapNetworks() {
        let temp = fromNetwork
        fromNetwork = toNetwork
        toNetwork = temp
    }
    
    private func calculateReceiveAmount() -> String {
        guard let amountValue = Double(amount) else { return "0.00" }
        let fee = amountValue * 0.001 // 0.1% fee
        let received = amountValue - fee
        return String(format: "%.6f", received)
    }
    
    private func initiateBridgeTransfer() {
        isTransferring = true
        
        print("🌉 Initiating bridge transfer:")
        print("   From: \(fromNetwork.name)")
        print("   To: \(toNetwork.name)")
        print("   Amount: \(amount)")
        
        // Simulate transfer
        DispatchQueue.main.asyncAfter(deadline: .now() + 3) {
            isTransferring = false
            amount = ""
        }
    }
}

struct NetworkSelectorView: View {
    @Binding var selectedNetwork: BridgeNetwork
    @Binding var isPresented: Bool
    
    var body: some View {
        NavigationView {
            List {
                ForEach(BridgeNetwork.allCases, id: \.self) { network in
                    Button(action: {
                        selectedNetwork = network
                        isPresented = false
                    }) {
                        HStack {
                            Text(network.emoji)
                                .font(.title2)
                            
                            VStack(alignment: .leading) {
                                Text(network.name)
                                    .font(.headline)
                                    .foregroundColor(.white)
                                
                                Text(network.timeEstimate)
                                    .font(.caption)
                                    .foregroundColor(.secondary)
                            }
                            
                            Spacer()
                            
                            if network == selectedNetwork {
                                Image(systemName: "checkmark.circle.fill")
                                    .foregroundColor(.green)
                            }
                        }
                        .padding(.vertical, 4)
                    }
                }
            }
            .navigationTitle("Select Network")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Cancel") { isPresented = false })
            .background(Color.black)
        }
    }
}

struct BridgeTransferRow: View {
    let from: BridgeNetwork
    let to: BridgeNetwork
    let amount: Double
    let status: TransferStatus
    
    var body: some View {
        HStack {
            HStack(spacing: 8) {
                Text(from.emoji)
                    .font(.caption)
                
                Image(systemName: "arrow.right")
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                Text(to.emoji)
                    .font(.caption)
            }
            
            VStack(alignment: .leading, spacing: 2) {
                Text("\(from.name) → \(to.name)")
                    .font(.caption)
                    .foregroundColor(.white)
                
                Text("\(amount, specifier: "%.4f") \(from.nativeToken)")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            Text(status.rawValue)
                .font(.caption)
                .fontWeight(.medium)
                .foregroundColor(status.color)
                .padding(.horizontal, 6)
                .padding(.vertical, 2)
                .background(status.color.opacity(0.2))
                .cornerRadius(6)
        }
        .padding(.horizontal)
        .padding(.vertical, 8)
    }
}

// MARK: - Bridge Data Models
enum BridgeNetwork: String, CaseIterable {
    case usdtgverse = "USDTgVerse"
    case ethereum = "Ethereum"
    case bnbChain = "BNB Chain"
    case polygon = "Polygon"
    case arbitrum = "Arbitrum"
    case tron = "TRON"
    case solana = "Solana"
    case avalanche = "Avalanche"
    case optimism = "Optimism"
    
    var name: String { rawValue }
    
    var emoji: String {
        switch self {
        case .usdtgverse: return "🌌"
        case .ethereum: return "🔵"
        case .bnbChain: return "🟡"
        case .polygon: return "🔶"
        case .arbitrum: return "🔷"
        case .tron: return "🟢"
        case .solana: return "🟣"
        case .avalanche: return "🔴"
        case .optimism: return "🟠"
        }
    }
    
    var nativeToken: String {
        switch self {
        case .usdtgverse: return "USDTg"
        case .ethereum: return "ETH"
        case .bnbChain: return "BNB"
        case .polygon: return "MATIC"
        case .arbitrum: return "ARB"
        case .tron: return "TRX"
        case .solana: return "SOL"
        case .avalanche: return "AVAX"
        case .optimism: return "OP"
        }
    }
    
    var timeEstimate: String {
        switch self {
        case .usdtgverse: return "Instant"
        case .bnbChain: return "3 seconds"
        case .polygon: return "2 seconds"
        case .solana: return "30 seconds"
        case .tron: return "3 minutes"
        case .arbitrum: return "10 minutes"
        case .ethereum: return "15 minutes"
        case .avalanche: return "90 seconds"
        case .optimism: return "7 minutes"
        }
    }
    
    var gasFee: String {
        switch self {
        case .usdtgverse: return "0.01 USDTg"
        case .bnbChain: return "~$0.10"
        case .polygon: return "~$0.01"
        case .solana: return "~$0.001"
        case .tron: return "~$0.05"
        case .arbitrum: return "~$1.50"
        case .ethereum: return "~$15.00"
        case .avalanche: return "~$0.25"
        case .optimism: return "~$2.00"
        }
    }
    
    var balance: Double {
        switch self {
        case .usdtgverse: return 10000.0
        case .ethereum: return 2.5
        case .bnbChain: return 15.8
        case .polygon: return 8750.0
        case .arbitrum: return 125.5
        case .tron: return 12500.0
        case .solana: return 45.2
        case .avalanche: return 95.7
        case .optimism: return 234.8
        }
    }
}

enum TransferStatus: String {
    case pending = "Pending"
    case completed = "Completed"
    case failed = "Failed"
    
    var color: Color {
        switch self {
        case .pending: return .orange
        case .completed: return .green
        case .failed: return .red
        }
    }
}

// MARK: - Additional Views
struct SendView: View {
    @Environment(\.dismiss) private var dismiss
    @State private var recipientAddress = ""
    @State private var amount = ""
    @State private var selectedAsset = "USDTg"
    @State private var memo = ""
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    VStack(alignment: .leading, spacing: 12) {
                        Text("Recipient Address")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                        
                        HStack {
                            TextField("usdtg1...", text: $recipientAddress)
                                .textFieldStyle(RoundedBorderTextFieldStyle())
                            
                            Button(action: {}) {
                                Image(systemName: "qrcode.viewfinder")
                            }
                            .buttonStyle(.bordered)
                        }
                    }
                    
                    VStack(alignment: .leading, spacing: 12) {
                        Text("Amount")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                        
                        HStack {
                            TextField("0.00", text: $amount)
                                .keyboardType(.decimalPad)
                                .textFieldStyle(RoundedBorderTextFieldStyle())
                            
                            Picker("Asset", selection: $selectedAsset) {
                                Text("USDTg").tag("USDTg")
                                Text("USDT").tag("USDT")
                                Text("USDC").tag("USDC")
                            }
                            .pickerStyle(MenuPickerStyle())
                        }
                    }
                    
                    VStack(alignment: .leading, spacing: 12) {
                        Text("Memo (Optional)")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                        
                        TextField("Payment description...", text: $memo)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                    }
                    
                    Button("Send Payment") {
                        sendPayment()
                    }
                    .buttonStyle(.borderedProminent)
                    .tint(Color(red: 0.3, green: 0.7, blue: 0.3))
                    .frame(maxWidth: .infinity)
                    
                    Spacer()
                }
                .padding()
            }
            .navigationTitle("Send Payment")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(
                leading: Button("Cancel") { dismiss() }
            )
            .background(Color.black)
        }
    }
    
    private func sendPayment() {
        print("💸 Sending \(amount) \(selectedAsset) to \(recipientAddress)")
        dismiss()
    }
}

struct ReceiveView: View {
    @Environment(\.dismiss) private var dismiss
    @EnvironmentObject var walletManager: WalletManager
    @State private var selectedAsset = "USDTg"
    @State private var amount = ""
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 24) {
                    Text("Receive Payment")
                        .font(.title2)
                        .fontWeight(.bold)
                        .foregroundColor(.white)
                    
                    // QR Code placeholder
                    Rectangle()
                        .fill(Color.white)
                        .frame(width: 200, height: 200)
                        .cornerRadius(12)
                        .overlay(
                            VStack {
                                Image(systemName: "qrcode")
                                    .font(.system(size: 60))
                                    .foregroundColor(.black)
                                
                                Text("QR Code")
                                    .font(.caption)
                                    .foregroundColor(.black)
                            }
                        )
                    
                    VStack(spacing: 12) {
                        Text("Your Wallet Address")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                        
                        Text(walletManager.walletAddress)
                            .font(.system(size: 14, family: .monospaced))
                            .foregroundColor(.white)
                            .padding()
                            .background(Color.secondary.opacity(0.2))
                            .cornerRadius(8)
                    }
                    
                    HStack(spacing: 12) {
                        Button("Copy Address") {
                            copyAddress()
                        }
                        .buttonStyle(.bordered)
                        .tint(.blue)
                        
                        Button("Share QR") {
                            shareQR()
                        }
                        .buttonStyle(.bordered)
                        .tint(.green)
                    }
                    
                    Spacer()
                }
                .padding()
            }
            .navigationTitle("Receive")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(
                trailing: Button("Done") { dismiss() }
            )
            .background(Color.black)
        }
    }
    
    private func copyAddress() {
        UIPasteboard.general.string = walletManager.walletAddress
        print("📋 Address copied to clipboard")
    }
    
    private func shareQR() {
        print("📤 Sharing QR code")
    }
}

struct SwapView: View {
    @Environment(\.dismiss) private var dismiss
    @State private var fromAsset = "USDTg"
    @State private var toAsset = "USDT"
    @State private var amount = ""
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    Text("Token Swap")
                        .font(.title2)
                        .fontWeight(.bold)
                        .foregroundColor(.white)
                    
                    VStack(spacing: 16) {
                        // From token
                        VStack(alignment: .leading, spacing: 8) {
                            Text("From")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                            
                            HStack {
                                Picker("From Asset", selection: $fromAsset) {
                                    Text("USDTg").tag("USDTg")
                                    Text("USDT").tag("USDT")
                                    Text("USDC").tag("USDC")
                                    Text("ETH").tag("ETH")
                                }
                                .pickerStyle(MenuPickerStyle())
                                
                                TextField("0.00", text: $amount)
                                    .keyboardType(.decimalPad)
                                    .textFieldStyle(RoundedBorderTextFieldStyle())
                            }
                        }
                        
                        // Swap button
                        Button(action: swapAssets) {
                            Image(systemName: "arrow.up.arrow.down.circle.fill")
                                .font(.system(size: 24))
                                .foregroundColor(.blue)
                        }
                        
                        // To token
                        VStack(alignment: .leading, spacing: 8) {
                            Text("To")
                                .font(.subheadline)
                                .foregroundColor(.secondary)
                            
                            HStack {
                                Picker("To Asset", selection: $toAsset) {
                                    Text("USDT").tag("USDT")
                                    Text("USDTg").tag("USDTg")
                                    Text("USDC").tag("USDC")
                                    Text("ETH").tag("ETH")
                                }
                                .pickerStyle(MenuPickerStyle())
                                
                                Text(calculateSwapAmount())
                                    .padding()
                                    .background(Color.secondary.opacity(0.2))
                                    .cornerRadius(8)
                                    .foregroundColor(.secondary)
                            }
                        }
                    }
                    
                    // Swap details
                    VStack(spacing: 8) {
                        HStack {
                            Text("Exchange Rate")
                            Spacer()
                            Text("1 \(fromAsset) = 1.0001 \(toAsset)")
                                .foregroundColor(.secondary)
                        }
                        
                        HStack {
                            Text("Price Impact")
                            Spacer()
                            Text("0.01%")
                                .foregroundColor(.green)
                        }
                        
                        HStack {
                            Text("Network Fee")
                            Spacer()
                            Text("0.1 USDTg")
                                .foregroundColor(.secondary)
                        }
                    }
                    .padding()
                    .background(Color.secondary.opacity(0.1))
                    .cornerRadius(12)
                    
                    Button("Swap Tokens") {
                        executeSwap()
                    }
                    .buttonStyle(.borderedProminent)
                    .tint(.blue)
                    .frame(maxWidth: .infinity)
                    
                    Spacer()
                }
                .padding()
            }
            .navigationTitle("Swap")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(
                trailing: Button("Cancel") { dismiss() }
            )
            .background(Color.black)
        }
    }
    
    private func swapAssets() {
        let temp = fromAsset
        fromAsset = toAsset
        toAsset = temp
    }
    
    private func calculateSwapAmount() -> String {
        guard let amountValue = Double(amount) else { return "0.00" }
        return String(format: "%.6f", amountValue * 1.0001)
    }
    
    private func executeSwap() {
        print("🔄 Swapping \(amount) \(fromAsset) for \(toAsset)")
        dismiss()
    }
}

#Preview {
    BridgeView()
        .environmentObject(WalletManager())
        .environmentObject(NetworkManager())
}
