//
//  USDTgVersePayView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-19.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

enum USDTgVersePayFeature: String, CaseIterable {
    case corporateCards = "Corporate Cards"
    case expenseManagement = "Expense Management"
    case treasury = "Treasury & Banking"
    case billPay = "Bill Pay"
    case smartInvoicing = "Smart Invoicing"
    case escrow = "Escrow Service"
    case subscriptions = "Subscription Management"
    case batchPayments = "Batch Payments"
    case merchantGateway = "Merchant Gateway"
    
    var icon: String {
        switch self {
        case .corporateCards: return "💳"
        case .expenseManagement: return "📊"
        case .treasury: return "🏦"
        case .billPay: return "🧾"
        case .smartInvoicing: return "✉️"
        case .escrow: return "🤝"
        case .subscriptions: return "📅"
        case .batchPayments: return "📦"
        case .merchantGateway: return "🛍️"
        }
    }
    
    var description: String {
        switch self {
        case .corporateCards: return "Manage corporate payment cards"
        case .expenseManagement: return "Track and approve expenses"
        case .treasury: return "Banking and treasury operations"
        case .billPay: return "Automated bill payments"
        case .smartInvoicing: return "Create and send smart invoices"
        case .escrow: return "Secure escrow transactions"
        case .subscriptions: return "Manage recurring subscriptions"
        case .batchPayments: return "Process multiple payments"
        case .merchantGateway: return "Accept payments from customers"
        }
    }
}

extension USDTgVersePayFeature: FeatureProtocol {
    var title: String { rawValue }
}

struct USDTgVersePayView: View {
    @State private var selectedFeature: USDTgVersePayFeature?
    
    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                // Header
                headerView
                
                // Features Grid
                featuresGridView
                
                Spacer()
            }
            .navigationTitle("USDTgVerse PAY")
            .navigationBarTitleDisplayMode(.inline)
        }
    }
    
    private var headerView: some View {
        VStack(spacing: 16) {
            // USDTgVerse PAY Logo
            VStack(spacing: 8) {
                Circle()
                    .fill(LinearGradient(gradient: Gradient(colors: [Color.green, Color.blue]), startPoint: .topLeading, endPoint: .bottomTrailing))
                    .frame(width: 80, height: 80)
                    .overlay(
                        Text("USDTg")
                            .font(.title2)
                            .fontWeight(.bold)
                            .foregroundColor(.white)
                    )
                
                Text("USDTgVerse PAY")
                    .font(.title)
                    .fontWeight(.bold)
                
                Text("Quantum-Safe Payment Solutions")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            }
            
            // Quick Stats
            HStack(spacing: 20) {
                StatCard(title: "Active Cards", value: "12", color: .blue)
                StatCard(title: "Monthly Volume", value: "$45K", color: .green)
                StatCard(title: "Transactions", value: "234", color: .purple)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
        .padding()
    }
    
    private var featuresGridView: some View {
        ScrollView {
            LazyVGrid(columns: [
                GridItem(.flexible()),
                GridItem(.flexible())
            ], spacing: 16) {
                ForEach(USDTgVersePayFeature.allCases, id: \.self) { feature in
                    FeatureCard(feature: feature) {
                        selectedFeature = feature
                    }
                }
            }
            .padding()
        }
        .sheet(item: Binding(
            get: { selectedFeature },
            set: { selectedFeature = $0 }
        )) { feature in
            USDTgVersePayFeatureDetailView(feature: feature)
        }
    }
}

struct USDTgVersePayFeatureDetailView: View {
    let feature: USDTgVersePayFeature
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
        case .corporateCards:
            VStack(spacing: 16) {
                Text("💳 Virtual Cards: 8 Active")
                Text("💰 Monthly Limit: $10,000")
                Text("🔒 Security: Multi-factor auth")
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
            .padding(.horizontal)
            
        case .expenseManagement:
            VStack(spacing: 16) {
                Text("📊 Pending Approvals: 3")
                Text("💸 Monthly Expenses: $2,450")
                Text("📈 Budget Utilization: 68%")
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
            .padding(.horizontal)
            
        case .treasury:
            VStack(spacing: 16) {
                Text("🏦 Treasury Balance: $125,000")
                Text("💱 Multi-currency: 5 currencies")
                Text("📊 Yield: 4.2% APY")
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

extension USDTgVersePayFeature: Identifiable {
    var id: String { rawValue }
}

struct USDTgVersePayView_Previews: PreviewProvider {
    static var previews: some View {
        USDTgVersePayView()
    }
}