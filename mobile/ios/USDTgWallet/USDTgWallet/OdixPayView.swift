//
//  OdixPayView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-19.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

struct OdixPayView: View {
    @State private var selectedFeature: OdixPayFeature?
    @State private var showingQRScanner = false
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    // OdixPay Header
                    odixPayHeader
                    
                    // Quick Pay Section
                    quickPaySection
                    
                    // OdixPay Features Grid
                    featuresGrid
                    
                    // Recent OdixPay Activity
                    recentActivitySection
                }
                .padding()
            }
            .navigationTitle("OdixPay++")
            .navigationBarTitleDisplayMode(.large)
            .background(
                LinearGradient(
                    gradient: Gradient(colors: [
                        Color.purple.opacity(0.1),
                        Color.blue.opacity(0.1)
                    ]),
                    startPoint: .topLeading,
                    endPoint: .bottomTrailing
                )
            )
        }
        .sheet(item: $selectedFeature) { feature in
            OdixPayFeatureView(feature: feature)
        }
        .sheet(isPresented: $showingQRScanner) {
            QRScannerView()
        }
    }
    
    private var odixPayHeader: some View {
        VStack(spacing: 12) {
            HStack {
                Image(systemName: "creditcard.circle.fill")
                    .font(.system(size: 32))
                    .foregroundColor(.purple)
                
                VStack(alignment: .leading) {
                    Text("OdixPay++")
                        .font(.title2)
                        .fontWeight(.bold)
                        .foregroundColor(.white)
                    
                    Text("Enterprise Payment System")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                VStack(alignment: .trailing) {
                    Text("LIVE")
                        .font(.caption)
                        .fontWeight(.bold)
                        .foregroundColor(.green)
                        .padding(.horizontal, 8)
                        .padding(.vertical, 4)
                        .background(Color.green.opacity(0.2))
                        .cornerRadius(8)
                }
            }
            
            HStack {
                VStack {
                    Text("$45,678")
                        .font(.title3)
                        .fontWeight(.bold)
                        .foregroundColor(.white)
                    Text("Monthly Volume")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                VStack {
                    Text("156")
                        .font(.title3)
                        .fontWeight(.bold)
                        .foregroundColor(.white)
                    Text("Transactions")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                VStack {
                    Text("8")
                        .font(.title3)
                        .fontWeight(.bold)
                        .foregroundColor(.white)
                    Text("Networks")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(16)
    }
    
    private var quickPaySection: some View {
        VStack(spacing: 12) {
            Text("Quick Pay")
                .font(.headline)
                .foregroundColor(.white)
                .frame(maxWidth: .infinity, alignment: .leading)
            
            HStack(spacing: 15) {
                QuickPayButton(
                    icon: "qrcode.viewfinder",
                    title: "Scan QR",
                    color: .blue
                ) {
                    showingQRScanner = true
                }
                
                QuickPayButton(
                    icon: "person.circle.fill",
                    title: "Pay Contact",
                    color: .green
                ) {
                    // Handle contact payment
                }
                
                QuickPayButton(
                    icon: "doc.text.fill",
                    title: "Pay Invoice",
                    color: .orange
                ) {
                    selectedFeature = .invoice
                }
                
                QuickPayButton(
                    icon: "repeat.circle.fill",
                    title: "Recurring",
                    color: .purple
                ) {
                    selectedFeature = .subscription
                }
            }
        }
    }
    
    private var featuresGrid: some View {
        VStack(spacing: 12) {
            Text("OdixPay++ Features")
                .font(.headline)
                .foregroundColor(.white)
                .frame(maxWidth: .infinity, alignment: .leading)
            
            LazyVGrid(columns: Array(repeating: GridItem(.flexible()), count: 2), spacing: 15) {
                ForEach(OdixPayFeature.allCases, id: \.self) { feature in
                    OdixPayFeatureCard(feature: feature) {
                        selectedFeature = feature
                    }
                }
            }
        }
    }
    
    private var recentActivitySection: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("Recent OdixPay Activity")
                .font(.headline)
                .foregroundColor(.white)
                .frame(maxWidth: .infinity, alignment: .leading)
            
            ForEach(0..<3, id: \.self) { index in
                OdixPayActivityRow(
                    title: "Payment to Merchant #\(index + 1)",
                    amount: "$\(Double.random(in: 10...500), specifier: "%.2f")",
                    time: "\(index + 1)h ago",
                    status: index == 0 ? .completed : .pending
                )
            }
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
}

struct QuickPayButton: View {
    let icon: String
    let title: String
    let color: Color
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            VStack(spacing: 8) {
                Image(systemName: icon)
                    .font(.system(size: 20))
                    .foregroundColor(color)
                
                Text(title)
                    .font(.caption)
                    .foregroundColor(.white)
                    .multilineTextAlignment(.center)
            }
            .frame(maxWidth: .infinity)
            .padding(.vertical, 12)
            .background(Color.secondary.opacity(0.2))
            .cornerRadius(10)
        }
    }
}

struct OdixPayFeatureCard: View {
    let feature: OdixPayFeature
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            VStack(spacing: 12) {
                Image(systemName: feature.icon)
                    .font(.system(size: 24))
                    .foregroundColor(feature.color)
                
                Text(feature.title)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(.white)
                    .multilineTextAlignment(.center)
                
                Text(feature.description)
                    .font(.caption)
                    .foregroundColor(.secondary)
                    .multilineTextAlignment(.center)
                    .lineLimit(2)
            }
            .frame(maxWidth: .infinity)
            .padding()
            .background(Color.secondary.opacity(0.2))
            .cornerRadius(12)
            .overlay(
                RoundedRectangle(cornerRadius: 12)
                    .stroke(feature.color.opacity(0.3), lineWidth: 1)
            )
        }
    }
}

struct OdixPayActivityRow: View {
    let title: String
    let amount: String
    let time: String
    let status: ActivityStatus
    
    var body: some View {
        HStack {
            Circle()
                .fill(status.color)
                .frame(width: 8, height: 8)
            
            VStack(alignment: .leading, spacing: 2) {
                Text(title)
                    .font(.subheadline)
                    .foregroundColor(.white)
                
                Text(time)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            VStack(alignment: .trailing, spacing: 2) {
                Text(amount)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(.white)
                
                Text(status.rawValue)
                    .font(.caption)
                    .foregroundColor(status.color)
            }
        }
        .padding(.horizontal)
        .padding(.vertical, 8)
    }
}

enum OdixPayFeature: String, CaseIterable, Identifiable {
    case corporateCards = "Corporate Cards"
    case expenseManagement = "Expense Management"
    case treasury = "Treasury & Banking"
    case billPay = "Bill Pay"
    case invoice = "Smart Invoicing"
    case escrow = "Escrow Service"
    case subscription = "Subscriptions"
    case batchPayments = "Batch Payments"
    case merchantGateway = "Merchant Gateway"
    case analytics = "Payment Analytics"
    
    var id: String { rawValue }
    
    var title: String { rawValue }
    
    var icon: String {
        switch self {
        case .corporateCards: return "creditcard.fill"
        case .expenseManagement: return "chart.line.uptrend.xyaxis"
        case .treasury: return "building.columns.fill"
        case .billPay: return "doc.text.fill"
        case .invoice: return "doc.richtext.fill"
        case .escrow: return "lock.shield.fill"
        case .subscription: return "repeat.circle.fill"
        case .batchPayments: return "square.stack.3d.up.fill"
        case .merchantGateway: return "storefront.fill"
        case .analytics: return "chart.bar.fill"
        }
    }
    
    var color: Color {
        switch self {
        case .corporateCards: return .blue
        case .expenseManagement: return .green
        case .treasury: return .purple
        case .billPay: return .orange
        case .invoice: return .red
        case .escrow: return .yellow
        case .subscription: return .pink
        case .batchPayments: return .cyan
        case .merchantGateway: return .indigo
        case .analytics: return .mint
        }
    }
    
    var description: String {
        switch self {
        case .corporateCards: return "Enterprise payment cards"
        case .expenseManagement: return "Track business expenses"
        case .treasury: return "Corporate treasury management"
        case .billPay: return "Automated bill payments"
        case .invoice: return "Smart invoice generation"
        case .escrow: return "Secure escrow payments"
        case .subscription: return "Recurring subscriptions"
        case .batchPayments: return "Bulk payment processing"
        case .merchantGateway: return "Accept payments"
        case .analytics: return "Payment insights"
        }
    }
}

enum ActivityStatus: String {
    case completed = "Completed"
    case pending = "Pending"
    case failed = "Failed"
    
    var color: Color {
        switch self {
        case .completed: return .green
        case .pending: return .orange
        case .failed: return .red
        }
    }
}

struct OdixPayFeatureView: View {
    let feature: OdixPayFeature
    @Environment(\.dismiss) private var dismiss
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    // Feature header
                    VStack(spacing: 12) {
                        Image(systemName: feature.icon)
                            .font(.system(size: 48))
                            .foregroundColor(feature.color)
                        
                        Text(feature.title)
                            .font(.title2)
                            .fontWeight(.bold)
                            .foregroundColor(.white)
                        
                        Text(feature.description)
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                            .multilineTextAlignment(.center)
                    }
                    .padding()
                    
                    // Feature content based on type
                    featureContent
                    
                    Spacer()
                }
                .padding()
            }
            .navigationTitle(feature.title)
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") { dismiss() })
            .background(Color.black)
        }
    }
    
    @ViewBuilder
    private var featureContent: some View {
        switch feature {
        case .invoice:
            InvoiceCreationView()
        case .escrow:
            EscrowManagementView()
        case .subscription:
            SubscriptionManagementView()
        case .batchPayments:
            BatchPaymentView()
        default:
            ComingSoonView(feature: feature)
        }
    }
}

struct InvoiceCreationView: View {
    @State private var recipientEmail = ""
    @State private var amount = ""
    @State private var description = ""
    @State private var dueDate = Date()
    
    var body: some View {
        VStack(spacing: 16) {
            Text("Create Smart Invoice")
                .font(.headline)
                .foregroundColor(.white)
            
            VStack(spacing: 12) {
                TextField("Recipient Email", text: $recipientEmail)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                
                TextField("Amount (USDTg)", text: $amount)
                    .keyboardType(.decimalPad)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                
                TextField("Description", text: $description)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                
                DatePicker("Due Date", selection: $dueDate, displayedComponents: .date)
                    .foregroundColor(.white)
            }
            
            Button("Create Invoice") {
                createInvoice()
            }
            .buttonStyle(.borderedProminent)
            .tint(.purple)
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private func createInvoice() {
        print("📄 Creating invoice for \(recipientEmail)")
        print("💰 Amount: \(amount) USDTg")
        print("📝 Description: \(description)")
    }
}

struct EscrowManagementView: View {
    @State private var escrowAmount = ""
    @State private var buyerAddress = ""
    @State private var sellerAddress = ""
    
    var body: some View {
        VStack(spacing: 16) {
            Text("Escrow Service")
                .font(.headline)
                .foregroundColor(.white)
            
            VStack(spacing: 12) {
                TextField("Escrow Amount", text: $escrowAmount)
                    .keyboardType(.decimalPad)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                
                TextField("Buyer Address", text: $buyerAddress)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                
                TextField("Seller Address", text: $sellerAddress)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
            }
            
            HStack(spacing: 12) {
                Button("Create Escrow") {
                    createEscrow()
                }
                .buttonStyle(.borderedProminent)
                .tint(.yellow)
                
                Button("Release Funds") {
                    releaseFunds()
                }
                .buttonStyle(.bordered)
                .tint(.green)
            }
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private func createEscrow() {
        print("🔒 Creating escrow for \(escrowAmount) USDTg")
    }
    
    private func releaseFunds() {
        print("✅ Releasing escrow funds")
    }
}

struct SubscriptionManagementView: View {
    @State private var subscriptions: [Subscription] = [
        Subscription(merchant: "Netflix", amount: 15.99, frequency: "Monthly", status: .active),
        Subscription(merchant: "Spotify", amount: 9.99, frequency: "Monthly", status: .active),
        Subscription(merchant: "Adobe Creative", amount: 52.99, frequency: "Monthly", status: .paused)
    ]
    
    var body: some View {
        VStack(spacing: 16) {
            Text("Subscription Management")
                .font(.headline)
                .foregroundColor(.white)
            
            ForEach(subscriptions) { subscription in
                SubscriptionCard(subscription: subscription)
            }
            
            Button("Add New Subscription") {
                addSubscription()
            }
            .buttonStyle(.borderedProminent)
            .tint(.pink)
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private func addSubscription() {
        print("➕ Adding new subscription")
    }
}

struct BatchPaymentView: View {
    @State private var recipients: [BatchRecipient] = []
    @State private var newRecipient = ""
    @State private var newAmount = ""
    
    var body: some View {
        VStack(spacing: 16) {
            Text("Batch Payments")
                .font(.headline)
                .foregroundColor(.white)
            
            // Add recipient form
            HStack {
                TextField("Recipient", text: $newRecipient)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                
                TextField("Amount", text: $newAmount)
                    .keyboardType(.decimalPad)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
                
                Button("Add") {
                    addRecipient()
                }
                .buttonStyle(.borderedProminent)
                .tint(.cyan)
            }
            
            // Recipients list
            ForEach(recipients) { recipient in
                HStack {
                    Text(recipient.address)
                        .font(.caption)
                        .foregroundColor(.white)
                    
                    Spacer()
                    
                    Text("\(recipient.amount, specifier: "%.2f") USDTg")
                        .font(.caption)
                        .foregroundColor(.cyan)
                }
                .padding(.horizontal)
            }
            
            if !recipients.isEmpty {
                Button("Execute Batch Payment") {
                    executeBatchPayment()
                }
                .buttonStyle(.borderedProminent)
                .tint(.cyan)
            }
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(12)
    }
    
    private func addRecipient() {
        guard !newRecipient.isEmpty, let amount = Double(newAmount) else { return }
        
        recipients.append(BatchRecipient(address: newRecipient, amount: amount))
        newRecipient = ""
        newAmount = ""
    }
    
    private func executeBatchPayment() {
        print("🚀 Executing batch payment to \(recipients.count) recipients")
    }
}

struct ComingSoonView: View {
    let feature: OdixPayFeature
    
    var body: some View {
        VStack(spacing: 20) {
            Image(systemName: feature.icon)
                .font(.system(size: 60))
                .foregroundColor(feature.color)
            
            Text("Coming Soon")
                .font(.title2)
                .fontWeight(.bold)
                .foregroundColor(.white)
            
            Text("\(feature.title) will be available in the next update.")
                .font(.subheadline)
                .foregroundColor(.secondary)
                .multilineTextAlignment(.center)
        }
        .padding()
    }
}

struct SubscriptionCard: View {
    let subscription: Subscription
    
    var body: some View {
        HStack {
            VStack(alignment: .leading, spacing: 4) {
                Text(subscription.merchant)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(.white)
                
                Text("\(subscription.frequency) • $\(subscription.amount, specifier: "%.2f")")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            Text(subscription.status.rawValue)
                .font(.caption)
                .fontWeight(.medium)
                .foregroundColor(subscription.status.color)
                .padding(.horizontal, 8)
                .padding(.vertical, 4)
                .background(subscription.status.color.opacity(0.2))
                .cornerRadius(8)
        }
        .padding()
        .background(Color.secondary.opacity(0.1))
        .cornerRadius(8)
    }
}

struct QRScannerView: View {
    @Environment(\.dismiss) private var dismiss
    
    var body: some View {
        NavigationView {
            VStack {
                Text("QR Code Scanner")
                    .font(.title2)
                    .fontWeight(.bold)
                    .foregroundColor(.white)
                    .padding()
                
                // QR Scanner placeholder
                Rectangle()
                    .fill(Color.secondary.opacity(0.3))
                    .frame(width: 250, height: 250)
                    .cornerRadius(12)
                    .overlay(
                        VStack {
                            Image(systemName: "qrcode.viewfinder")
                                .font(.system(size: 60))
                                .foregroundColor(.white)
                            
                            Text("Point camera at QR code")
                                .foregroundColor(.secondary)
                        }
                    )
                
                Spacer()
                
                Button("Cancel") {
                    dismiss()
                }
                .buttonStyle(.bordered)
                .padding()
            }
            .background(Color.black)
            .navigationBarHidden(true)
        }
    }
}

// MARK: - Data Models
struct Subscription: Identifiable {
    let id = UUID()
    let merchant: String
    let amount: Double
    let frequency: String
    let status: SubscriptionStatus
}

struct BatchRecipient: Identifiable {
    let id = UUID()
    let address: String
    let amount: Double
}

enum SubscriptionStatus: String {
    case active = "Active"
    case paused = "Paused"
    case cancelled = "Cancelled"
    
    var color: Color {
        switch self {
        case .active: return .green
        case .paused: return .orange
        case .cancelled: return .red
        }
    }
}

#Preview {
    OdixPayView()
        .environmentObject(WalletManager())
        .environmentObject(NetworkManager())
}
