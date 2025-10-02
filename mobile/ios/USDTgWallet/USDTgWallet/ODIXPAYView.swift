//
//  ODIXPAYView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-10-02.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI
import CryptoKit

enum ODIXPAYFeature: String, CaseIterable {
    case sendPayment = "Send Payment"
    case receivePayment = "Receive Payment"
    case paymentHistory = "Payment History"
    case merchantPayments = "Merchant Payments"
    case billPay = "Bill Pay"
    case subscriptions = "Subscriptions"
    case internationalTransfer = "International Transfer"
    case cryptoPayments = "Crypto Payments"
    case quantumSafePay = "Quantum-Safe Pay"
    
    var icon: String {
        switch self {
        case .sendPayment: return "📤"
        case .receivePayment: return "📥"
        case .paymentHistory: return "📋"
        case .merchantPayments: return "🏪"
        case .billPay: return "🧾"
        case .subscriptions: return "📅"
        case .internationalTransfer: return "🌍"
        case .cryptoPayments: return "₿"
        case .quantumSafePay: return "🔐"
        }
    }
    
    var description: String {
        switch self {
        case .sendPayment: return "Send payments instantly"
        case .receivePayment: return "Receive payments securely"
        case .paymentHistory: return "View transaction history"
        case .merchantPayments: return "Pay merchants worldwide"
        case .billPay: return "Pay bills automatically"
        case .subscriptions: return "Manage subscriptions"
        case .internationalTransfer: return "Global money transfers"
        case .cryptoPayments: return "Crypto payment gateway"
        case .quantumSafePay: return "Quantum-safe transactions"
        }
    }
}

struct ODIXPAYView: View {
    @State private var selectedFeature: ODIXPAYFeature?
    @State private var showingSendPayment = false
    @State private var showingReceivePayment = false
    @State private var balance: Double = 1250.00
    @State private var usdtgBalance: Double = 1000.00
    @State private var usdtgvBalance: Double = 500.00
    @State private var usdtggBalance: Double = 100.00
    
    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                // Header with Balance
                headerView
                
                // Quick Actions
                quickActionsView
                
                // Features Grid
                featuresGridView
                
                Spacer()
            }
            .navigationTitle("ODIX PAY")
            .navigationBarTitleDisplayMode(.inline)
        }
        .sheet(isPresented: $showingSendPayment) {
            SendPaymentView()
        }
        .sheet(isPresented: $showingReceivePayment) {
            ReceivePaymentView()
        }
        .sheet(item: Binding(
            get: { selectedFeature },
            set: { selectedFeature = $0 }
        )) { feature in
            FeatureDetailView(feature: feature)
        }
    }
    
    private var headerView: some View {
        VStack(spacing: 16) {
            // ODIX PAY Logo
            VStack(spacing: 8) {
                Circle()
                    .fill(LinearGradient(gradient: Gradient(colors: [Color.purple, Color.blue]), startPoint: .topLeading, endPoint: .bottomTrailing))
                    .frame(width: 80, height: 80)
                    .overlay(
                        Text("ODIX")
                            .font(.title2)
                            .fontWeight(.bold)
                            .foregroundColor(.white)
                    )
                
                Text("ODIX PAY")
                    .font(.title)
                    .fontWeight(.bold)
                
                Text("Quantum-Safe Payment Platform")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            }
            
            // Balance Card
            VStack(spacing: 12) {
                Text("Total Balance")
                    .font(.headline)
                    .foregroundColor(.secondary)
                
                Text("$\(balance, specifier: "%.2f")")
                    .font(.system(size: 32, weight: .bold, design: .rounded))
                    .foregroundColor(.primary)
                
                // Token Balances
                HStack(spacing: 20) {
                    TokenBalanceView(symbol: "USDTg", balance: usdtgBalance)
                    TokenBalanceView(symbol: "USDTgV", balance: usdtgvBalance)
                    TokenBalanceView(symbol: "USDTgG", balance: usdtggBalance)
                }
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
        .padding()
    }
    
    private var quickActionsView: some View {
        HStack(spacing: 16) {
            QuickActionButton(
                icon: "arrow.up.circle.fill",
                title: "Send",
                color: .green
            ) {
                showingSendPayment = true
            }
            
            QuickActionButton(
                icon: "arrow.down.circle.fill",
                title: "Receive",
                color: .blue
            ) {
                showingReceivePayment = true
            }
            
            QuickActionButton(
                icon: "qrcode.viewfinder",
                title: "Scan",
                color: .orange
            ) {
                // Handle QR scan
            }
            
            QuickActionButton(
                icon: "chart.line.uptrend.xyaxis",
                title: "History",
                color: .purple
            ) {
                selectedFeature = .paymentHistory
            }
        }
        .padding(.horizontal)
    }
    
    private var featuresGridView: some View {
        ScrollView {
            LazyVGrid(columns: [
                GridItem(.flexible()),
                GridItem(.flexible())
            ], spacing: 16) {
                ForEach(ODIXPAYFeature.allCases, id: \.self) { feature in
                    FeatureCard(feature: feature) {
                        selectedFeature = feature
                    }
                }
            }
            .padding()
        }
    }
}

struct TokenBalanceView: View {
    let symbol: String
    let balance: Double
    
    var body: some View {
        VStack(spacing: 4) {
            Text(symbol)
                .font(.caption)
                .foregroundColor(.secondary)
            
            Text("\(balance, specifier: "%.2f")")
                .font(.headline)
                .fontWeight(.semibold)
        }
        .frame(maxWidth: .infinity)
        .padding(.vertical, 8)
        .background(Color(.systemBackground))
        .cornerRadius(8)
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
                    .font(.title2)
                    .foregroundColor(color)
                
                Text(title)
                    .font(.caption)
                    .fontWeight(.medium)
                    .foregroundColor(.primary)
            }
            .frame(maxWidth: .infinity)
            .padding(.vertical, 12)
            .background(Color(.systemGray6))
            .cornerRadius(12)
        }
        .buttonStyle(PlainButtonStyle())
    }
}

struct FeatureCard: View {
    let feature: ODIXPAYFeature
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            VStack(spacing: 12) {
                Text(feature.icon)
                    .font(.system(size: 40))
                
                Text(feature.rawValue)
                    .font(.headline)
                    .fontWeight(.semibold)
                    .multilineTextAlignment(.center)
                    .foregroundColor(.primary)
                
                Text(feature.description)
                    .font(.caption)
                    .foregroundColor(.secondary)
                    .multilineTextAlignment(.center)
            }
            .frame(maxWidth: .infinity, minHeight: 140)
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(16)
        }
        .buttonStyle(PlainButtonStyle())
    }
}

struct SendPaymentView: View {
    @Environment(\.presentationMode) var presentationMode
    @State private var recipientAddress = ""
    @State private var amount = ""
    @State private var selectedCurrency = "USDTg"
    @State private var paymentMethod = "wallet"
    @State private var isProcessing = false
    @State private var showingSuccess = false
    
    let currencies = ["USDTg", "USDTgV", "USDTgG"]
    let paymentMethods = ["wallet", "card", "bank"]
    
    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
                // Header
                VStack(spacing: 16) {
                    Image(systemName: "arrow.up.circle.fill")
                        .font(.system(size: 60))
                        .foregroundColor(.green)
                    
                    Text("Send Payment")
                        .font(.title2)
                        .fontWeight(.bold)
                }
                
                // Form
                VStack(spacing: 20) {
                    // Recipient Address
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Recipient Address")
                            .font(.headline)
                        
                        TextField("Enter wallet address", text: $recipientAddress)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                    }
                    
                    // Amount
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Amount")
                            .font(.headline)
                        
                        HStack {
                            TextField("0.00", text: $amount)
                                .keyboardType(.decimalPad)
                                .textFieldStyle(RoundedBorderTextFieldStyle())
                            
                            Picker("Currency", selection: $selectedCurrency) {
                                ForEach(currencies, id: \.self) { currency in
                                    Text(currency).tag(currency)
                                }
                            }
                            .pickerStyle(MenuPickerStyle())
                        }
                    }
                    
                    // Payment Method
                    VStack(alignment: .leading, spacing: 8) {
                        Text("Payment Method")
                            .font(.headline)
                        
                        Picker("Payment Method", selection: $paymentMethod) {
                            Text("USDTgVerse Wallet").tag("wallet")
                            Text("Credit Card").tag("card")
                            Text("Bank Transfer").tag("bank")
                        }
                        .pickerStyle(SegmentedPickerStyle())
                    }
                }
                
                Spacer()
                
                // Send Button
                Button(action: sendPayment) {
                    HStack {
                        if isProcessing {
                            ProgressView()
                                .progressViewStyle(CircularProgressViewStyle(tint: .white))
                                .scaleEffect(0.8)
                        }
                        Text("Send Payment")
                            .font(.headline)
                            .fontWeight(.semibold)
                    }
                    .foregroundColor(.white)
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(canSend ? Color.green : Color.gray)
                    .cornerRadius(12)
                }
                .disabled(!canSend || isProcessing)
            }
            .padding()
            .navigationTitle("Send Payment")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Cancel") {
                presentationMode.wrappedValue.dismiss()
            })
        }
        .alert("Payment Sent!", isPresented: $showingSuccess) {
            Button("OK") {
                presentationMode.wrappedValue.dismiss()
            }
        } message: {
            Text("Your payment of \(amount) \(selectedCurrency) has been sent successfully!")
        }
    }
    
    private var canSend: Bool {
        !recipientAddress.isEmpty && !amount.isEmpty && Double(amount) ?? 0 > 0
    }
    
    private func sendPayment() {
        isProcessing = true
        
        // Simulate payment processing
        DispatchQueue.main.asyncAfter(deadline: .now() + 2) {
            isProcessing = false
            showingSuccess = true
        }
    }
}

struct ReceivePaymentView: View {
    @Environment(\.presentationMode) var presentationMode
    @State private var walletAddress = "USDTg1q1234567890abcdefghijklmnopqrstuvwxyz"
    @State private var amount = ""
    @State private var selectedCurrency = "USDTg"
    @State private var showingQRCode = false
    
    let currencies = ["USDTg", "USDTgV", "USDTgG"]
    
    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
                // Header
                VStack(spacing: 16) {
                    Image(systemName: "arrow.down.circle.fill")
                        .font(.system(size: 60))
                        .foregroundColor(.blue)
                    
                    Text("Receive Payment")
                        .font(.title2)
                        .fontWeight(.bold)
                }
                
                // Wallet Address
                VStack(spacing: 16) {
                    Text("Your Wallet Address")
                        .font(.headline)
                    
                    Text(walletAddress)
                        .font(.system(.body, design: .monospaced))
                        .padding()
                        .background(Color(.systemGray6))
                        .cornerRadius(8)
                        .onTapGesture {
                            UIPasteboard.general.string = walletAddress
                        }
                    
                    Button("Copy Address") {
                        UIPasteboard.general.string = walletAddress
                    }
                    .foregroundColor(.blue)
                }
                
                // Amount Request
                VStack(spacing: 16) {
                    Text("Request Amount (Optional)")
                        .font(.headline)
                    
                    HStack {
                        TextField("0.00", text: $amount)
                            .keyboardType(.decimalPad)
                            .textFieldStyle(RoundedBorderTextFieldStyle())
                        
                        Picker("Currency", selection: $selectedCurrency) {
                            ForEach(currencies, id: \.self) { currency in
                                Text(currency).tag(currency)
                            }
                        }
                        .pickerStyle(MenuPickerStyle())
                    }
                }
                
                // QR Code Button
                Button(action: { showingQRCode = true }) {
                    HStack {
                        Image(systemName: "qrcode")
                        Text("Show QR Code")
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.blue)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                }
                
                Spacer()
            }
            .padding()
            .navigationTitle("Receive Payment")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
        }
        .sheet(isPresented: $showingQRCode) {
            QRCodeView(
                address: walletAddress,
                amount: amount.isEmpty ? nil : amount,
                currency: selectedCurrency
            )
        }
    }
}

struct QRCodeView: View {
    @Environment(\.presentationMode) var presentationMode
    let address: String
    let amount: String?
    let currency: String
    
    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
                Text("Payment QR Code")
                    .font(.title2)
                    .fontWeight(.bold)
                
                // QR Code Placeholder
                RoundedRectangle(cornerRadius: 16)
                    .fill(Color(.systemGray5))
                    .frame(width: 250, height: 250)
                    .overlay(
                        VStack {
                            Image(systemName: "qrcode")
                                .font(.system(size: 100))
                                .foregroundColor(.gray)
                            Text("QR Code")
                                .font(.caption)
                                .foregroundColor(.gray)
                        }
                    )
                
                if let amount = amount {
                    Text("Amount: \(amount) \(currency)")
                        .font(.headline)
                }
                
                Text("Scan to send payment")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Spacer()
            }
            .padding()
            .navigationTitle("QR Code")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
        }
    }
}

struct FeatureDetailView: View {
    let feature: ODIXPAYFeature
    @Environment(\.presentationMode) var presentationMode
    
    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
                // Feature Header
                VStack(spacing: 16) {
                    Text(feature.icon)
                        .font(.system(size: 80))
                    
                    Text(feature.rawValue)
                        .font(.title)
                        .fontWeight(.bold)
                    
                    Text(feature.description)
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                .padding()
                
                // Demo Content
                demoContent
                
                Spacer()
                
                // Action Button
                Button("Get Started") {
                    // Handle feature action
                    presentationMode.wrappedValue.dismiss()
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color.blue)
                .foregroundColor(.white)
                .cornerRadius(12)
                .font(.headline)
                .padding()
            }
            .navigationTitle(feature.rawValue)
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
        }
    }
    
    @ViewBuilder
    private var demoContent: some View {
        switch feature {
        case .quantumSafePay:
            VStack(spacing: 16) {
                Text("🔐 Quantum-Safe Encryption")
                Text("🛡️ Post-Quantum Cryptography")
                Text("⚡ Instant Settlement")
                Text("🌍 Global Network")
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
            .padding(.horizontal)
            
        case .cryptoPayments:
            VStack(spacing: 16) {
                Text("₿ Multi-Crypto Support")
                Text("💱 Real-time Conversion")
                Text("🔒 Secure Transactions")
                Text("📊 Price Tracking")
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
            .padding(.horizontal)
            
        case .internationalTransfer:
            VStack(spacing: 16) {
                Text("🌍 200+ Countries")
                Text("💱 50+ Currencies")
                Text("⚡ Instant Transfer")
                Text("💰 Low Fees")
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
            .padding(.horizontal)
            
        default:
            VStack(spacing: 16) {
                Text("🚀 Coming Soon")
                    .font(.title2)
                    .fontWeight(.semibold)
                
                Text("This feature is under development and will be available in the next update.")
                    .multilineTextAlignment(.center)
                    .foregroundColor(.secondary)
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
            .padding(.horizontal)
        }
    }
}

extension ODIXPAYFeature: Identifiable {
    var id: String { rawValue }
}

struct ODIXPAYView_Previews: PreviewProvider {
    static var previews: some View {
        ODIXPAYView()
    }
}
