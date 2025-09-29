/*
==============================================
 File:        MembershipView.swift
 Author:      Irfan Gedik
 Created:     DD.MM.YYYY
 Last Update: 28.09.2025
 Version:     1.0

 Description:
   USDTgVerse iOS Mobile Membership Integration
   
   Complete membership management in mobile wallets:
   - Membership tier management
   - KYC document submission
   - Trading permissions control
   - Real-time benefits tracking
   - Upgrade/downgrade flows

 License:
   MIT License
==============================================
*/

import SwiftUI

struct MembershipView: View {
    @StateObject private var membershipService = MembershipService()
    @State private var currentTier: MembershipTier = .individual
    @State private var showUpgrade = false
    @State private var showKYC = false
    @State private var isKYCComplete = false
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    // Membership Header
                    membershipHeaderView
                    
                    // Current Status Cards
                    statusCardsView
                    
                    // Benefits Grid
                    benefitsView
                    
                    // Trading Permissions
                    tradingPermissionsView
                    
                    // Upgrade Options
                    upgradeOptionsView
                    
                    // Quick Actions
                    quickActionsView
                }
                .padding()
            }
            .navigationTitle("Membership")
            .navigationBarTitleDisplayMode(.large)
        }
    }
    
    // MARK: - Membership Header
    private var membershipHeaderView: some View {
        VStack(spacing: 16) {
            HStack {
                VStack(alignment: .leading) {
                    Text(membershipService.currentTier.displayName)
                        .font(.title)
                        .fontWeight(.bold)
                    
                    Text(membershipService.currentTier.subtitle)
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                Button(action: { showKYC = true }) {
                    Text(isKYCComplete ? "✅ Verified" : "📝 Complete KYC")
                        .font(.caption)
                        .padding(.horizontal, 12)
                        .padding(.vertical, 6)
                        .background(isKYCComplete ? Color.green : Color.orange)
                        .foregroundColor(.white)
                        .cornerRadius(12)
                }
            }
            
            // Progress Bar
            ProgressView(value: membershipService.verificationProgress)
                .progressViewStyle(LinearProgressViewStyle())
            
            Text("Verification: \(Int(membershipService.verificationProgress * 100))%")
                .font(.caption)
                .foregroundColor(.secondary)
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
    }
    
    // MARK: - Status Cards
    private var statusCardsView: some View {
        HStack(spacing: 12) {
            StatusCard(
                title: "Daily Limit",
                value: membershipService.currentTier.dailyLimit,
                icon: "dollarsign.circle.fill",
                color: .green
            )
            
            StatusCard(
                title: "Trading Fees",
                value: membershipService.currentTier.feePercentage,
                icon: "percent",
                color: .blue
            )
            
            StatusCard(
                title: "Support Level",
                value: membershipService.currentTier.supportLevel,
                icon: "headphones",
                color: .purple
            )
        }
    }
    
    // MARK: - Benefits Grid
    private var benefitsView: some View {
        VStack(alignment: .leading, spacing: 16) {
            Text("Your Benefits")
                .font(.headline)
                .fontWeight(.semibold)
            
            LazyVGrid(columns: [
                GridItem(.flexible()),
                GridItem(.flexible())
            ], spacing: 12) {
                ForEach(membershipService.currentTier.benefits, id: \.title) { benefit in
                    BenefitCard(benefit: benefit)
                }
            }
            
            if membershipService.currentTier != .institutional {
                Button("Upgrade Membership") {
                    showUpgrade = true
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(LinearGradient(colors: [Color.blue, Color.purple], startPoint: .leading, endPoint: .trailing))
                .foregroundColor(.white)
                .cornerRadius(12)
                .font(.headline)
            }
        }
        .padding()
        .background(Color.white)
        .cornerRadius(16)
        .shadow(color: .black.opacity(0.05), radius: 8, y: 4)
    }
    
    // MARK: - Trading Permissions
    private var tradingPermissionsView: some View {
        VStack(alignment: .leading, spacing: 16) {
            Text("Trading Permissions")
                .font(.headline)
                .fontWeight(.semibold)
            
            VStack(spacing: 8) {
                PermissionRow(
                    permission: "Spot Trading",
                    isEnabled: membershipService.currentTier.permissions.spot,
                    icon: "chart.line.uptrend.xyaxis"
                )
                
                PermissionRow(
                    permission: "Margin Trading",
                    isEnabled: membershipService.currentTier.permissions.margin,
                    icon: "arrow.up.arrow.down.circle"
                )
                
                PermissionRow(
                    permission: "Futures Trading",
                    isEnabled: membershipService.currentTier.permissions.futures,
                    icon: "calendar.badge.clock"
                )
                
                PermissionRow(
                    permission: "OTC Trading",
                    isEnabled: membershipService.currentTier.permissions.otc,
                    icon: "building.2.crop.circle"
                )
                
                PermissionRow(
                    permission: "API Access",
                    isEnabled: membershipService.currentTier.permissions.api,
                    icon: "terminal"
                )
            }
        }
        .padding()
        .background(Color.white)
        .cornerRadius(16)
        .shadow(color: .black.opacity(0.05), radius: 8, y: 4)
    }
    
    // MARK: - Upgrade Options
    private var upgradeOptionsView: some View {
        VStack(alignment: .leading, spacing: 16) {
            Text("Upgrade Options")
                .font(.headline)
                .fontWeight(.semibold)
            
            LazyVStack(spacing: 12) {
                ForEach(MembershipTier.allCases.dropFirst(), id: \.self) { tier in
                    TierUpgradeCard(
                        tier: tier,
                        isCurrent: tier == membershipService.currentTier,
                        action: { upgradeToTier(tier) }
                    )
                }
            }
        }
        .padding()
        .background(Color.white)
        .cornerRadius(16)
        .shadow(color: .black.opacity(0.05), radius: 8, y: 4)
    }
    
    // MARK: - Quick Actions
    private var quickActionsView: some View {
        VStack(alignment: .leading, spacing: 16) {
            Text("Quick Actions")
                .font(.headline)
                .fontWeight(.semibold)
            
            HStack(spacing: 12) {
                QuickActionButton(
                    icon: "doc.text",
                    title: "Submit Docs",
                    action: { showKYC = true }
                )
                
                QuickActionButton(
                    icon: "questionmark.circle",
                    title: "Support",
                    action: { openSupport() }
                )
                
                QuickActionButton(
                    icon: "doc.plaintext",
                    title: "Reports",
                    action: { openReports() }
                )
            }
        }
        .padding()
        .background(Color.white)
        .cornerRadius(16)
        .shadow(color: .black.opacity(0.05), radius: 8, y: 4)
    }
    
    // MARK: - Functions
    private func upgradeToTier(_ tier: MembershipTier) {
        membershipService.upgradeToTier(tier)
    }
    
    private func openSupport() {
        // Open support chat or phone
    }
    
    private func openReports() {
        // Open trading reports
    }
}

// MARK: - Supporting Views

struct StatusCard: View {
    let title: String
    let value: String
    let icon: String
    let color: Color
    
    var body: some View {
        VStack(spacing: 8) {
            Image(systemName: icon)
                .font(.title2)
                .foregroundColor(color)
            
            Text(value)
                .font(.headline)
                .fontWeight(.semibold)
            
            Text(title)
                .font(.caption)
                .foregroundColor(.secondary)
        }
        .frame(maxWidth: .infinity)
        .padding()
        .background(color.opacity(0.1))
        .cornerRadius(12)
    }
}

struct BenefitCard: View {
    let benefit: MembershipBenefit
    
    var body: some View {
        HStack {
            Image(systemName: "checkmark.circle.fill")
                .foregroundColor(.green)
            
            Text(benefit.title)
                .font(.subheadline)
            
            Spacer()
        }
        .padding()
        .background(Color.green.opacity(0.05))
        .cornerRadius(8)
    }
}

struct PermissionRow: View {
    let permission: String
    let isEnabled: Bool
    let icon: String
    
    var body: some View {
        HStack {
            Image(systemName: icon)
                .foregroundColor(isEnabled ? .green : .gray)
            
            Text(permission)
                .font(.subheadline)
            
            Spacer()
            
            Image(systemName: isEnabled ? "checkmark.circle.fill" : "xmark.circle.fill")
                .foregroundColor(isEnabled ? .green : .red)
        }
        .padding(.vertical, 4)
    }
}

struct TierUpgradeCard: View {
    let tier: MembershipTier
    let isCurrent: Bool
    let action: () -> Void
    
    var body: some View {
        HStack {
            VStack(alignment: .leading) {
                Text(tier.displayName)
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Text(tier.price)
                    .font(.subheadline)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            Button(isCurrent ? "Current" : "Upgrade") {
                action()
            }
            .disabled(isCurrent)
            .padding(.horizontal, 16)
            .padding(.vertical, 8)
            .background(isCurrent ? Color.gray : Color.blue)
            .foregroundColor(.white)
            .cornerRadius(8)
            .font(.subheadline)
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
}

struct QuickActionButton: View {
    let<｜tool▁call▁begin｜>
let title: String
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            VStack {
                Image(systemName: icon)
                    .font(.title2)
                    .foregroundColor(.blue)
                
                Text(title)
                    .font(.caption)
                    .foregroundColor(.primary)
            }
            .frame(maxWidth: .infinity)
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
        }
    }
}

// MARK: - Data Models

enum MembershipTier: CaseIterable {
    case individual, professional, corporate, institutional, vip
    
    var displayName: String {
        switch self {
        case .individual: return "Individual"
        case .professional: return "Professional"
        case .corporate: return "Corporate"
        case .institutional: return "Institutional"
        case .vip: return "VIP"
        }
    }
    
    var subtitle: String {
        switch self {
        case .individual: return "Basic trading access"
        case .professional: return "Advanced trading features"
        case .corporate: return "Enterprise solutions"
        case .institutional: return "Institutional grade"
        case .vip: return "Premium exclusive"
        }
    }
    
    var price: String {
        switch self {
        case .individual: return "Free"
        case .professional: return "$99/month"
        case .corporate: return "$499/month"
        case .institutional: return "Custom"
        case .vip: return "Invite Only"
        }
    }
    
    var dailyLimit: String {
        switch self {
        case .individual: return "$50K"
        case .professional: return "$500K"
        case .corporate: return "$5M"
        case .institutional: return "Unlimited"
        case .vip: return "Custom"
        }
    }
    
    var feePercentage: String {
        switch self {
        case .individual: return "0.1%"
        case .professional: return "0.08%"
        case .corporate: return "0.06%"
        case .institutional: return "0.04%"
        case .vip: return "Custom"
        }
    }
    
    var supportLevel: String {
        switch self {
        case .individual: return "Standard"
        case .professional: return "Priority"
        case .corporate: return "Dedicated"
        case .institutional: return "24/7"
        case .vip: return "Personal"
        }
    }
    
    var permissions: TradingPermissions {
        TradingPermissions(tier: self)
    }
    
    var benefits: [MembershipBenefit] {
        switch self {
        case .individual:
            return [
                MembershipBenefit(title: "Spot Trading"),
                MembershipBenefit(title: "Basic Wallet"),
                MembershipBenefit(title: "Standard Support")
            ]
        case .professional:
            return [
                MembershipBenefit(title: "Margin Trading"),
                MembershipBenefit(title: "API Access"),
                MembershipBenefit(title: "Priority Support"),
                MembershipBenefit(title: "Reduced Fees")
            ]
        case .corporate:
            return [
                MembershipBenefit(title: "Futures Trading"),
                MembershipBenefit(title: "OTC Access"),
                MembershipBenefit(title: "Dedicated Manager"),
                MembershipBenefit(title: "Custom Integration")
            ]
        case .institutional:
            return [
                MembershipBenefit(title: "Dark Pools"),
                MembershipBenefit(title: "Prime Services"),
                MembershipBenefit(title: "Algorithmic Trading"),
                MembershipBenefit(title: "Custom Solutions")
            ]
        case .vip:
            return [
                MembershipBenefit(title: "All Features"),
                MembershipBenefit(title: "Personal Advisor"),
                MembershipBenefit(title: "Custom Pricing"),
                MembershipBenefit(title: "Exclusive Access")
            ]
        }
    }
}

struct TradingPermissions {
    let spot: Bool
    let margin: Bool
    let futures: Bool
    let otc: Bool
    let api: Bool
    
    init(tier: MembershipTier) {
        switch tier {
        case .individual:
            spot = true
            margin = false
            futures = false
            otc = false
            api = false
        case .professional:
            spot = true
            margin = true
            futures = false
            otc = false
            api = true
        case .corporate:
            spot = true
            margin = true
            futures = true
            otc = true
            api = true
        case .institutional, .vip:
            spot = true
            margin = true
            futures = true
            otc = true
            api = true
        }
    }
}

struct MembershipBenefit {
    let title: String
    let description: String?
    
    init(title: String, description: String? = nil) {
        self.title = title
        self.description = description
    }
}

// MARK: - Services

class MembershipService: ObservableObject {
    @Published var currentTier: MembershipTier = .individual
    @Published var verificationProgress: Double = 0.6
    @Published var kycStatus: KYCStatus = .pending
    
    func upgradeToTier(_ tier: MembershipTier) {
        // Implement upgrade logic
        currentTier = tier
    }
    
    func submitKYC() {
        // Implement KYC submission
        verificationProgress = 1.0
        kycStatus = .verified
    }
}

enum KYCStatus {
    case pending, submitted, verified, rejected
}

struct MembershipView_Previews: PreviewProvider {
    static var previews: some View {
        MembershipView()
    }
}
