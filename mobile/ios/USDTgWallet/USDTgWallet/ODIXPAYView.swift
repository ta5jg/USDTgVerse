//
//  ODIXPAYView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-10-02.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

enum ODIXPAYFeature: String, CaseIterable {
    case sendPayment = "Send Payment"
    case receivePayment = "Receive Payment"
    case scanQR = "Scan QR"
    case transactionHistory = "History"
    case merchantPayments = "Merchant Payments"
    case internationalTransfer = "International Transfer"
    case multiTokenSupport = "Multi-Token Support"
    case instantTransfer = "Instant Transfer"

    var icon: String {
        switch self {
        case .sendPayment: return "🚀"
        case .receivePayment: return "📥"
        case .scanQR: return "📷"
        case .transactionHistory: return "📋"
        case .merchantPayments: return "🏪"
        case .internationalTransfer: return "🌍"
        case .multiTokenSupport: return "🔀"
        case .instantTransfer: return "⚡"
        }
    }

    var description: String {
        switch self {
        case .sendPayment: return "Send funds securely"
        case .receivePayment: return "Receive funds via address"
        case .scanQR: return "Scan QR for quick payments"
        case .transactionHistory: return "View all your transactions"
        case .merchantPayments: return "Pay at supported merchants"
        case .internationalTransfer: return "Send money globally"
        case .multiTokenSupport: return "Manage various tokens"
        case .instantTransfer: return "Lightning-fast transactions"
        }
    }
}

extension ODIXPAYFeature: FeatureProtocol {
    var title: String { rawValue }
}

extension ODIXPAYFeature: Identifiable {
    var id: String { rawValue }
}

struct ODIXPAYView: View {
    @State private var selectedFeature: ODIXPAYFeature?
    @State private var showingQRScanner = false
    @State private var scannedAddress: String = ""

    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                headerView
                featuresGridView
                Spacer()
            }
            .navigationTitle("ODIX PAY")
            .navigationBarTitleDisplayMode(.inline)
        }
        .sheet(item: Binding(
            get: { selectedFeature },
            set: { selectedFeature = $0 }
        )) { feature in
            ODIXPAYFeatureDetailView(feature: feature)
        }
        .sheet(isPresented: $showingQRScanner) {
            QRScannerSheet { scannedData in
                scannedAddress = scannedData
                selectedFeature = .sendPayment // Or handle based on QR content
            }
        }
    }

    private var headerView: some View {
        VStack(spacing: 16) {
            VStack(spacing: 8) {
                Image(systemName: "creditcard.fill")
                    .font(.system(size: 60))
                    .foregroundColor(.blue)
                Text("ODIX PAY")
                    .font(.title)
                    .fontWeight(.bold)
                Text("Quantum-Safe Payment Solutions")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            }
            HStack(spacing: 20) {
                StatCard(title: "Total Volume", value: "$1.2M", color: .green)
                StatCard(title: "Transactions", value: "5,432", color: .blue)
                StatCard(title: "Merchants", value: "120", color: .purple)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
        .padding()
    }

    private var featuresGridView: some View {
        ScrollView {
            LazyVGrid(columns: [GridItem(.flexible()), GridItem(.flexible())], spacing: 16) {
                ForEach(ODIXPAYFeature.allCases, id: \.self) { feature in
                    FeatureCard(feature: feature) {
                        if feature == .scanQR {
                            showingQRScanner = true
                        } else {
                            selectedFeature = feature
                        }
                    }
                }
            }
            .padding()
        }
    }
}

struct ODIXPAYFeatureDetailView: View {
    let feature: ODIXPAYFeature
    @Environment(\.presentationMode) var presentationMode
    
    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
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
                
                demoContent
                Spacer()
                
                Button("Get Started") {
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

struct ODIXPAYView_Previews: PreviewProvider {
    static var previews: some View {
        ODIXPAYView()
    }
}