//
//  CustodyView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-01-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

enum CustodyTier: String, CaseIterable {
    case starter = "Starter"
    case premium = "Premium"
    case enterprise = "Enterprise"
    case institutional = "Institutional"
    
    var icon: String {
        switch self {
        case .starter: return "🏢"
        case .premium: return "🏦"
        case .enterprise: return "🏛️"
        case .institutional: return "👑"
        }
    }
    
    var color: Color {
        switch self {
        case .starter: return .blue
        case .premium: return .purple
        case .enterprise: return .orange
        case .institutional: return .red
        }
    }
    
    var features: [String] {
    switch self {
    case .starter:
        return ["Multi-sig wallets", "24/7 monitoring", "Insurance coverage", "Regulatory compliance"]
    case .premium:
        return ["All Starter features", "Cold storage", "Hardware security modules", "Dedicated account manager"]
    case .enterprise:
        return ["All Premium features", "White-label solutions", "Custom integrations", "SLA guarantees"]
    case .institutional:
        return ["All Enterprise features", "Audit reports", "Certified custody", "Round-the-clock support"]
        }
    }
    
    var monthlyFee: Double {
        switch self {
        case .starter: return 99.99
        case .premium: return 499.99
        case .enterprise: return 1999.99
        case .institutional: return 9999.99
        }
    }
    
    var minBalance: Double {
        switch self {
        case .starter: return 10000
        case .premium: return 100000
        case .enterprise: return 1000000
        case .institutional: return 10000000
        }
    }
}

extension CustodyTier: FeatureProtocol {
    var title: String { rawValue }
    
    var description: String {
        return "Min $\(String(format: "%.0f", Double(minBalance))) • $\(String(format: "%.0f", monthlyFee))"
    }
}

extension CustodyTier: Identifiable {
    var id: String { rawValue }
}

struct CustodyView: View {
    @Environment(\.presentationMode) var presentationMode
    @State private var selectedTier: CustodyTier?
    @State private var currentBalance: Double = 25000
    @State private var showingServiceRequest = false
    @State private var custodyAssets: [CustodyAsset] = []
    @State private var custodyMetrics = CustodyMetrics()
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    headerView
                    portfolioOverviewView
                    securityFeaturesView
                    tiersSection
                    assetsUnderCustodyView
                    performanceMetricsView
                }
            }
            .navigationTitle("Custody Services")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
        }
        .sheet(item: Binding(
            get: { selectedTier },
            set: { selectedTier = $0 }
        )) { tier in
            CustodyTierDetailView(tier: tier, currentBalance: currentBalance)
        }
        .alert("Service Request", isPresented: $showingServiceRequest) {
            Button("Cancel", role: .cancel) { }
            Button("Request Service") {
                // Handle service request
            }
        } message: {
            Text("Request institutional custody services consultation?")
        }
    }
    
    private var headerView: some View {
        VStack(spacing: 16) {
            VStack(spacing: 8) {
                Circle()
                    .fill(LinearGradient(gradient: Gradient(colors: [Color.red, Color.orange]), startPoint: .topLeading, endPoint: .bottomTrailing))
                    .frame(width: 80, height: 80)
                    .overlay(
                        Text("🏦")
                            .font(.system(size: 40))
                    )
                
                Text("USDTgVerse Custody")
                    .font(.title)
                    .fontWeight(.bold)
                
                Text("Institutional-Grade Digital Asset Security")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            }
            
            HStack(spacing: 16) {
                StatCard(title: "Assets Protected", value: "$\(formatCurrency(custodyMetrics.totalAssets))", color: .green)
                StatCard(title: "Security Rating", value: "AA+", color: .blue)
                StatCard(title: "Annual Fee", value: "0.1%", color: .orange)
                StatCard(title: "Uptime", value: "99.9%", color: .purple)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
    }
    
    private var portfolioOverviewView: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("Portfolio Overview")
                .font(.headline)
                .fontWeight(.semibold)
            
            VStack(spacing: 8) {
                CustodyStatsCard(
                    title: "Total Value Under Custody",
                    value: "$\(formatCurrency(custodyMetrics.custodyValue))",
                    change: String(format: "+%.1f%%", custodyMetrics.monthlyGrowth),
                    changeColor: .green
                )
                
                CustodyStatsCard(
                    title: "Number of Assets",
                    value: "\(custodyAssets.count)",
                    change: custodyAssets.isEmpty ? "Add assets" : "Growing",
                    changeColor: .blue
                )
                
                CustodyStatsCard(
                    title: "Insurance Coverage",
                    value: "$\(formatCurrency(custodyMetrics.insuranceCoverage))",
                    change: custodyMetrics.insuranceRatio > 100 ? "✅ Full Coverage" : "⚠️ Partial",
                    changeColor: custodyMetrics.insuranceRatio > 100 ? .green : .orange
                )
                
                CustodyStatsCard(
                    title: "Monthly Storage Fee",
                    value: String(format: "$%.0f", custodyMetrics.monthlyFee),
                    change: "0.1% of portfolio",
                    changeColor: .blue
                )
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
    
    private var securityFeaturesView: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("🔒 Security Features")
                .font(.headline)
                .fontWeight(.semibold)
            
            VStack(spacing: 12) {
                SecurityFeatureRow(
                    icon: "shield.fill",
                    iconColor: .green,
                    title: "Multi-Signature Wallets",
                    description: "Multiple private key approvals required"
                )
                
                SecurityFeatureRow(
                    icon: "snow",
                    iconColor: .blue,
                    title: "Cold Storage",
                    description: "Offline storage for maximum security"
                )
                
                SecurityFeatureRow(
                    icon: "key.fill",
                    iconColor: .orange,
                    title: "Hardware Security Modules",
                    description: "Tamper-resistant key storage"
                )
                
                SecurityFeatureRow(
                    icon: "checkmark.shield.fill",
                    iconColor: .purple,
                    title: "Regulatory Compliance",
                    description: "SOC2 Type II certified"
                )
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
    
    private var tiersSection: some View {
        VStack(alignment: .leading, spacing: 16) {
            HStack {
                Text("Custody Service Tiers")
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text("Select your institutional tier")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            LazyVGrid(columns: [
                GridItem(.flexible()),
                GridItem(.flexible())
            ], spacing: 12) {
                ForEach(CustodyTier.allCases, id: \.self) { tier in
                    CustodyTierCard(
                        tier: tier,
                        isAccessible: currentBalance >= tier.minBalance,
                        onTierSelected: { selectedTier = tier }
                    )
                }
            }
        }
    }
    
    private var assetsUnderCustodyView: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Text("📊 Assets Under Custody")
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Button("Add Asset") {
                    // Navigate to asset addition
                }
            }
            
            if custodyAssets.isEmpty {
                VStack(spacing: 12) {
                    Image(systemName: "building.columns.fill")
                        .font(.system(size: 40))
                        .foregroundColor(.gray)
                    
                    Text("No assets under custody")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                    
                    Text("Start with institutional custody to protect your assets")
                        .font(.caption)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color(.systemGray6))
            } else {
                ScrollView(.horizontal, showsIndicators: false) {
                    HStack(spacing: 12) {
                        ForEach(custodyAssets) { asset in
                            CustodyAssetCard(asset: asset)
                        }
                    }
                    .padding(.horizontal)
                }
            }
        }
    }
    
    private var performanceMetricsView: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("📈 Performance Metrics")
                .font(.headline)
                .fontWeight(.semibold)
            VStack(spacing: 8) {
                CustodyMetricsCard(
                    title: "Security Incidents",
                    value: "0",
                    description: "Zero security breaches in 3 years"
                )
                
                CustodyMetricsCard(
                    title: "Client Satisfaction",
                    value: "98%",
                    description: "Enterprise-level satisfaction rating"
                )
                
                CustodyMetricsCard(
                    title: "Audit Compliance",
                    value: "100%",
                    description: "Full regulatory compliance maintained"
                )
                
                CustodyMetricsCard(
                    title: "Recovery Time",
                    value: "< 4 hours",
                    description: "Average incident response time"
                )
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
    
    private func formatCurrency(_ amount: Double) -> String {
        let formatter = NumberFormatter()
        formatter.numberStyle = .currency
        formatter.locale = Locale.current
        formatter.currencyCode = "USD"
        return formatter.string(from: NSNumber(value: amount))?.replacingOccurrences(of: "$", with: "") ?? "0"
    }
}

struct CustodyTierCard: View {
    let tier: CustodyTier
    let isAccessible: Bool
    let onTierSelected: () -> Void
    
    var body: some View {
        Button(action: onTierSelected) {
            VStack(spacing: 8) {
                Text(tier.icon)
                    .font(.system(size: 32))
                
                Text(tier.rawValue)
                    .font(.headline)
                    .fontWeight(.semibold)
                    .foregroundColor(.primary)
                
                Text("$\(tier.monthlyFee, specifier: "%.0f")/month")
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                if isAccessible {
                    Text("AVAILABLE")
                        .font(.caption2)
                        .fontWeight(.bold)
                        .padding(.horizontal, 8)
                        .background(Color.green.opacity(0.2))
                        .foregroundColor(.green)
                        .cornerRadius(4)
                } else {
                    Text(String(format: "MIN: $%.0f", tier.minBalance))
                        .font(.caption2)
                        .padding(.horizontal, 8)
                        .padding(.vertical, 2)
                        .background(Color.red.opacity(0.2))
                        .foregroundColor(.red)
                        .cornerRadius(4)
                }
            }
            .frame(maxWidth: .infinity, minHeight: 140)
            .padding()
            .background(Color(.systemBackground))
            .cornerRadius(16)
            .shadow(color: Color.black.opacity(0.1), radius: 2, x: 0, y: 1)
            .overlay(
                RoundedRectangle(cornerRadius: 16)
                    .stroke(isAccessible ? tier.color : Color.clear, lineWidth: 2)
            )
        }
        .buttonStyle(PlainButtonStyle())
        .disabled(isAccessible)
    }
}

struct CustodyTierDetailView: View {
    let tier: CustodyTier
    let currentBalance: Double
    @Environment(\.presentationMode) var presentationMode
    
    var canAccess: Bool {
        currentBalance >= tier.minBalance
    }
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 24) {
                    VStack(spacing: 16) {
                        Text(tier.icon)
                            .font(.system(size: 80))
                        
                        Text(tier.rawValue + " Custody")
                            .font(.title)
                            .fontWeight(.bold)
                        
                        Text("$\(tier.monthlyFee, specifier: "%.0f") per month")
                            .font(.title2)
                            .fontWeight(.semibold)
                            .foregroundColor(tier.color)
                        
                        Text("Minimum balance: $\(Int(tier.minBalance), specifier: "%.0f")")
                            .font(.subheadline)
                            .foregroundColor(canAccess ? .green : .red)
                    }
                    .padding()
                    
                    VStack(alignment: .leading, spacing: 12) {
                        Text("Service Features")
                            .font(.headline)
                            .fontWeight(.semibold)
                        
                        ForEach(tier.features, id: \.self) { feature in
                            HStack {
                                Image(systemName: "checkmark.circle.fill")
                                    .foregroundColor(.green)
                                Text(feature)
                                    .font(.subheadline)
                                Spacer()
                            }
                        }
                    }
                    .padding()
                    .background(Color(.systemGray6))
                    .cornerRadius(12)
                    
                    Button(canAccess ? "Subscribe to \(tier.rawValue)" : "Requires $\(Int(tier.minBalance - currentBalance), specifier: "%.0f") more balance") {
                        if canAccess {
                            presentationMode.wrappedValue.dismiss()
                        }
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(canAccess ? tier.color : Color.gray)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                    .font(.headline)
                    .disabled(!canAccess)
                    
                    Spacer()
                }
            }
            .navigationTitle(tier.rawValue)
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Cancel") {
                presentationMode.wrappedValue.dismiss()
            })
        }
    }
}

struct CustodyStatsCard: View {
    let title: String
    let value: String
    let change: String
    let changeColor: Color
    
    var body: some View {
        HStack {
            Text(title)
                .font(.subheadline)
                .foregroundColor(.secondary)
            
            Spacer()
            
            VStack(alignment: .trailing, spacing: 2) {
                Text(value)
                    .font(.subheadline)
                    .fontWeight(.medium)
                
                Text(change)
                    .font(.caption)
                    .foregroundColor(changeColor)
            }
        }
        .padding()
        .background(Color(.systemBackground))
        .cornerRadius(8)
    }
}

struct SecurityFeatureRow: View {
    let icon: String
    let iconColor: Color
    let title: String
    let description: String
    
    var body: some View {
        HStack {
            Image(systemName: icon)
                .foregroundColor(iconColor)
                .frame(width: 30)
            
            VStack(alignment: .leading, spacing: 4) {
                Text(title)
                    .font(.subheadline)
                    .fontWeight(.medium)
                
                Text(description)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
        }
        .padding(.vertical, 4)
    }
}

struct CustodyAssetCard: View {
    let asset: CustodyAsset
    
    var body: some View {
        VStack(spacing: 8) {
            Circle()
                .fill(Color.blue.opacity(0.2))
                .frame(width: 50, height: 50)
                .overlay(
                    Text("🏦")
                        .font(.title2)
                )
            
            VStack(spacing: 4) {
                Text(asset.symbol)
                    .font(.subheadline)
                    .fontWeight(.semibold)
                
                Text("$\(asset.value, specifier: "%.0f")")
                    .font(.caption)
                    .foregroundColor(.green)
                
                Text(asset.custodyTier.rawValue)
                    .font(.caption2)
                    .padding(.horizontal, 6)
                    .padding(.vertical, 2)
                    .background(asset.custodyTier.color.opacity(0.2))
                    .foregroundColor(asset.custodyTier.color)
                    .cornerRadius(4)
            }
        }
        .padding()
        .frame(width: 120)
        .background(Color(.systemBackground))
        .cornerRadius(12)
        .shadow(color: Color.black.opacity(0.1), radius: 2, x: 0, y: 1)
    }
}

struct CustodyMetricsCard: View {
    let title: String
    let value: String
    let description: String
    
    var body: some View {
        HStack {
            VStack(alignment: .leading, spacing: 4) {
                Text(title)
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Text(description)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            Text(value)
                .font(.title2)
                .fontWeight(.bold)
                .foregroundColor(.primary)
        }
        .padding()
        .background(Color(.systemBackground))
        .cornerRadius(8)
    }
}

// MARK: - Supporting Models
struct CustodyAsset: Identifiable {
    let id = UUID().uuidString
    let symbol: String
    let value: Double
    let custodyTier: CustodyTier
}

struct CustodyMetrics {
    let totalAssets: Double = 15470000
    let custodyValue: Double = 12300000
    let monthlyGrowth: Double = 8.2
    let insuranceCoverage: Double = 15000000
    let insuranceRatio: Double = 120.0
    let monthlyFee: Double = 12300.0
}

struct CustodyView_Previews: PreviewProvider {
    static var previews: some View {
        CustodyView()
    }
}