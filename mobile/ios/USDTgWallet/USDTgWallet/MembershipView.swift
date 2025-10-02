//
//  MembershipView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-01-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

enum MembershipTier: String, CaseIterable {
    case bronze = "Bronze"
    case silver = "Silver"
    case gold = "Gold"
    case platinum = "Platinum"
    case diamond = "Diamond"
    
    var icon: String {
        switch self {
        case .bronze: return "🥉"
        case .silver: return "📍"
        case .gold: return "🥇"
        case .platinum: return "💎"
        case .diamond: return "💠"
        }
    }
    
    var color: Color {
        switch self {
        case .bronze: return .orange
        case .silver: return .gray
        case .gold: return .yellow
        case .platinum: return .blue
        case .diamond: return .purple
        }
    }
    
    var benefits: [String] {
        switch self {
        case .bronze:
            return ["Basic trading features", "Community access", "10% discount on fees"]
        case .silver:
            return ["Priority support", "Advanced analytics", "20% discount on fees", "Early feature access"]
        case .gold:
            return ["Personal portfolio manager", "30% discount on fees", "VIP trading signals", "Institutional tools"]
        case .platinum:
            return ["White-label options", "50% discount on fees", "Dedicated account manager", "Custom integrations"]
        case .diamond:
            return ["Full platform customization", "Unlimited transactions", "98% discount on fees", "Executive support"]
        }
    }
    
    var monthlyFee: Double {
        switch self {
        case .bronze: return 29.99
        case .silver: return 99.99
        case .gold: return 299.99
        case .platinum: return 999.99
        case .diamond: return 4999.99
        }
    }
    
    var minUSDTgHolding: Double {
        switch self {
        case .bronze: return 100.0
        case .silver: return 1000.0
        case .gold: return 10000.0
        case .platinum: return 100000.0
        case .diamond: return 1000000.0
        }
    }
}

extension MembershipTier: FeatureProtocol {
    var title: String { rawValue }
    
    var description: String {
        return "Hold \(Int(minUSDTgHolding)) USDTg • $\(String(format: "%.0f", monthlyFee))"
    }
}

extension MembershipTier: Identifiable {
    var id: String { rawValue }
}

struct MembershipView: View {
    @EnvironmentObject var walletManager: WalletManager
    @Environment(\.presentationMode) var presentationMode
    @State private var selectedTier: MembershipTier?
    @State private var currentUSDTgBalance: Double = 10.0
    @State private var currentMembership: MembershipTier = .bronze
    @State private var membershipMetrics = MembershipMetrics()
    @State private var recentActivities: [MembershipActivity] = []
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    headerView
                    membershipOverviewCard
                    tiersSection
                    benefitsSection
                    activitySection
                }
                .padding()
            }
            .navigationTitle("Membership Portal")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
        }
        .sheet(item: Binding(
            get: { selectedTier },
            set: { selectedTier = $0 }
        )) { tier in
            MembershipDetailView(tier: tier, currentBalance: currentUSDTgBalance)
        }
    }
    
    private var headerView: some View {
        VStack(spacing: 16) {
            // USDTgVerse Membership Logo
            VStack(spacing: 8) {
                Circle()
                    .fill(LinearGradient(gradient: Gradient(colors: [Color.purple, Color.blue]), startPoint: .topLeading, endPoint: .bottomTrailing))
                    .frame(width: 80, height: 80)
                    .overlay(
                        Text("👑")
                            .font(.system(size: 40))
                    )
                
                Text("USDTgVerse Membership")
                    .font(.title)
                    .fontWeight(.bold)
                
                Text("Quantum-Safe Financial Services")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            }
            
            // Current Membership Status
            HStack {
                VStack(spacing: 4) {
                    Text("Current Tier")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text(currentMembership.rawValue)
                        .font(.headline)
                        .fontWeight(.bold)
                        .foregroundColor(currentMembership.color)
                }
                
                Spacer()
                
                VStack(spacing: 4) {
                    Text("USDTg Balance")
                        .font(.caption)
                        .foregroundColor(.secondary)
                        Text("\(currentUSDTgBalance, specifier: "%.2f")")
                        .font(.headline)
                        .fontWeight(.bold)
                        .foregroundColor(.primary)
                }
                
                Spacer()
                
                VStack(spacing: 4) {
                    Text("Next Payment")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("Jan 25")
                        .font(.headline)
                        .fontWeight(.bold)
                        .foregroundColor(.green)
                }
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
    }
    
    private var membershipOverviewCard: some View {
        VStack(spacing: 16) {
            Text("Membership Performance")
                .font(.headline)
                .fontWeight(.semibold)
            
            HStack(spacing: 16) {
                StatCard(title: "Active Days", value: "\(membershipMetrics.activeDays)", color: .blue)
                StatCard(title: "Trading Volume", value: String(format: "$%.1fK", membershipMetrics.tradingVolume), color: .green)
                StatCard(title: "Savings Earned", value: String(format: "$%.0f", membershipMetrics.savingsEarned), color: .orange)
                StatCard(title: "Cashback",
                        value: String(format: "$%.0f", membershipMetrics.cashbackEarned),
                        color: .purple)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
    
    private var tiersSection: some View {
        VStack(alignment: .leading, spacing: 16) {
            HStack {
                Text("Upgrade Membership")
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text("Hold USDTg to unlock higher tiers")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            LazyVGrid(columns: [
                GridItem(.flexible()),
                GridItem(.flexible())
            ], spacing: 12) {
                ForEach(MembershipTier.allCases, id: \.self) { tier in
                    MembershipTierCard(
                        tier: tier,
                        isCurrentTier: tier == currentMembership,
                        isUpgradeable: currentUSDTgBalance >= tier.minUSDTgHolding,
                        onTierSelected: { selectedTier = tier }
                    )
                }
            }
        }
    }
    
    private var benefitsSection: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("Current Benefits")
                .font(.headline)
                .fontWeight(.semibold)
            
            VStack(spacing: 8) {
                ForEach(currentMembership.benefits, id: \.self) { benefit in
                    HStack {
                        Image(systemName: "checkmark.circle.fill")
                            .foregroundColor(.green)
                        Text(benefit)
                            .font(.subheadline)
                        Spacer()
                    }
                    .padding(.horizontal, 8)
                }
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
        }
    }
    
    private var activitySection: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Text("Recent Activity")
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Button("View All") {
                    // Navigate to full activity log
                }
                .font(.caption)
                .foregroundColor(.blue)
            }
            
            VStack(spacing: 8) {
                ForEach(recentActivities.prefix(3)) { activity in
                    ActivityRow(activity: activity)
                }
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
        }
    }
}

struct MembershipTierCard: View {
    let tier: MembershipTier
    let isCurrentTier: Bool
    let isUpgradeable: Bool
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
                
                if isCurrentTier {
                    Text("CURRENT")
                    .font(.caption2)
                    .fontWeight(.bold)
                    .padding(.horizontal, 8)
                    .padding(.vertical, 2)
                    .background(Color.green.opacity(0.2))
                    .foregroundColor(.green)
                    .cornerRadius(4)
                } else if isUpgradeable {
                    Text("UPGRADE")
                        .font(.caption2)
                        .fontWeight(.bold)
                        .padding(.horizontal, 8)
                        .padding(.vertical, 2)
                        .background(Color.blue.opacity(0.2))
                        .foregroundColor(.blue)
                        .cornerRadius(4)
                }
            }
            .frame(maxWidth: .infinity, minHeight: 120)
            .padding()
            .background(Color(.systemBackground))
            .cornerRadius(12)
            .overlay(
                RoundedRectangle(cornerRadius: 12)
                    .stroke(isCurrentTier ? Color.green : Color.clear, lineWidth: 2)
            )
        }
        .buttonStyle(PlainButtonStyle())
        .disabled(!isUpgradeable && !isCurrentTier)
    }
}

struct MembershipDetailView: View {
    let tier: MembershipTier
    let currentBalance: Double
    @Environment(\.presentationMode) var presentationMode
    @State private var showingUpgradeConfirmation = false
    
    var canUpgrade: Bool {
        return currentBalance >= tier.minUSDTgHolding
    }
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 24) {
                    // Tier Header
                    VStack(spacing: 16) {
                        Text(tier.icon)
                            .font(.system(size: 80))
                        
                        Text(tier.rawValue + " Membership")
                            .font(.title)
                            .fontWeight(.bold)
                        
                        Text("$\(tier.monthlyFee, specifier: "%.0f") per month")
                            .font(.title2)
                            .fontWeight(.semibold)
                            .foregroundColor(tier.color)
                        
                        Text("Requires \(Int(tier.minUSDTgHolding), specifier: "%.0f") USDTg minimum balance")
                            .font(.subheadline)
                            .foregroundColor(.secondary)
                            .multilineTextAlignment(.center)
                    }
                    .padding()
                    
                    // Benefits
                    VStack(alignment: .leading, spacing: 12) {
                        Text("Membership Benefits")
                            .font(.headline)
                            .fontWeight(.semibold)
                        
                        ForEach(tier.benefits, id: \.self) { benefit in
                            HStack {
                                Image(systemName: "checkmark.circle.fill")
                                    .foregroundColor(.green)
                                Text(benefit)
                                    .font(.subheadline)
                                Spacer()
                            }
                        }
                    }
                    .padding()
                    .background(Color(.systemGray6))
                    .cornerRadius(12)
                    
                    // Requirements Section
                    VStack(alignment: .leading, spacing: 12) {
                        Text("Membership Requirements")
                            .font(.headline)
                            .fontWeight(.semibold)
                        
                        VStack(spacing: 8) {
                            HStack {
                                Text("Minimum USDTg Balance:")
                                    .font(.subheadline)
                                Spacer()
                                Text("\(Int(tier.minUSDTgHolding)) USDTg")
                                    .font(.subheadline)
                                    .fontWeight(.medium)
                                    .foregroundColor(canUpgrade ? .green : .red)
                            }
                            
                            HStack {
                                Text("Your Current Balance:")
                                    .font(.subheadline)
                                Spacer()
                                Text("\(currentBalance, specifier: "%.2f") USDTg")
                                    .font(.subheadline)
                                    .fontWeight(.medium)
                            }
                            
                            HStack {
                                Text("Monthly Fee:")
                                    .font(.subheadline)
                                Spacer()
                                Text("$\(tier.monthlyFee, specifier: "%.2f")")
                                    .font(.subheadline)
                                    .fontWeight(.medium)
                            }
                        }
                    }
                    .padding()
                    .background(Color(.systemGray6))
                    .cornerRadius(12)
                    
                    Spacer()
                    
                    // Upgrade Button
                    Button(action: {
                        if canUpgrade {
                            showingUpgradeConfirmation = true
                        }
                    }) {
                        Text(canUpgrade ? "Upgrade to \(tier.rawValue)" : "Requires \(Int(tier.minUSDTgHolding - currentBalance)) more USDTg")
                    }
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(canUpgrade ? tier.color : Color.gray)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                    .font(.headline)
                    .disabled(!canUpgrade)
                }
                .padding()
            }
            .navigationTitle(tier.rawValue)
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Cancel") {
                presentationMode.wrappedValue.dismiss()
            })
        }
        .alert("Upgrade Confirmation", isPresented: $showingUpgradeConfirmation) {
            Button("Cancel", role: .cancel) { }
            Button("Upgrade") {
                // Handle upgrade
                presentationMode.wrappedValue.dismiss()
            }
        } message: {
            Text("Upgrade to \(tier.rawValue) membership for $\(tier.monthlyFee, specifier: "%.2f") per month?")
        }
    }
}

struct ActivityRow: View {
    let activity: MembershipActivity
    
    var body: some View {
        HStack {
            Image(systemName: activity.icon)
                .foregroundColor(activity.iconColor)
                .frame(width: 30)
            
            VStack(alignment: .leading, spacing: 4) {
                Text(activity.title)
                    .font(.subheadline)
                    .fontWeight(.medium)
                
                Text(activity.description)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            VStack(alignment: .trailing, spacing: 4) {
                Text(activity.amount)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(activity.amountColor)
                
                Text(activity.time)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding(.vertical, 4)
    }
}

// MARK: - Supporting Models
struct MembershipMetrics {
    let activeDays: Int = 45
    let tradingVolume: Double = 125.5
    let savingsEarned: Double = 245.80
    let cashbackEarned: Double = 89.32
}

struct MembershipActivity: Identifiable {
    let id = UUID()
    let icon: String
    let iconColor: Color
    let title: String
    let description: String
    let amount: String
    let amountColor: Color
    let time: String
    
    static let sampleActivities = [
        MembershipActivity(
            icon: "creditcard.fill",
            iconColor: .blue,
            title: "Monthly Subscription",
            description: "\(MembershipTier.bronze.rawValue) membership fee",
            amount: "-$29.99",
            amountColor: .red,
            time: "2 days ago"
        ),
        MembershipActivity(
            icon: "percent",
            iconColor: .green,
            title: "Trading Fee Discount",
            description: "10% discount applied to swap",
            amount: "-$1.50",
            amountColor: .green,
            time: "5 days ago"
        ),
        MembershipActivity(
            icon: "gift.fill",
            iconColor: .purple,
            title: "Welcome Bonus",
            description: "New member cashback reward",
            amount: "+$10.00",
            amountColor: .green,
            time: "1 week ago"
        )
    ]
}

struct MembershipView_Previews: PreviewProvider {
    static var previews: some View {
        MembershipView()
            .environmentObject(WalletManager())
    }
}
