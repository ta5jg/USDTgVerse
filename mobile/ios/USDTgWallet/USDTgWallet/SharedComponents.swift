//
//  SharedComponents.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-10-02.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

// MARK: - Shared Components for USDTgWallet

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
    let feature: FeatureProtocol
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            VStack(spacing: 12) {
                Text(feature.icon)
                    .font(.system(size: 40))
                
                Text(feature.title)
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

struct FeatureDetailView: View {
    let feature: FeatureProtocol
    @Environment(\.presentationMode) var presentationMode
    
    var body: some View {
        NavigationView {
            VStack(spacing: 24) {
                // Feature Header
                VStack(spacing: 16) {
                    Text(feature.icon)
                        .font(.system(size: 80))
                    
                    Text(feature.title)
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
            .navigationTitle(feature.title)
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
        }
    }
    
    @ViewBuilder
    var demoContent: some View {
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

// MARK: - Protocol for Features
protocol FeatureProtocol {
    var title: String { get }
    var description: String { get }
    var icon: String { get }
}

/* Disabled duplicate StatCard - now defined in CopyTradingView.swift
struct StatCard: View {
    let title: String
    let value: String
    
    var body: some View {
        VStack(spacing: 4) {
            Text(value)
                .font(.title2)
                .fontWeight(.bold)
                .foregroundColor(.primary)
            
            Text(title)
                .font(.caption)
                .foregroundColor(.secondary)
        }
        .frame(maxWidth: .infinity)
        .padding()
        .background(Color(.systemBackground))
        .cornerRadius(12)
    }
}
*/

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

struct TransactionCard: View {
    let transaction: TransactionProtocol
    
    var body: some View {
        VStack(spacing: 8) {
            HStack {
                Text(transaction.type)
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text(transaction.amount)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(transaction.isPositive ? .green : .red)
            }
            
            HStack {
                Text(transaction.id.prefix(12) + "...")
                    .font(.caption)
                    .foregroundColor(.secondary)
                    .font(.system(.caption, design: .monospaced))
                
                Spacer()
                
                Text(transaction.timestamp)
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(8)
    }
}

protocol TransactionProtocol {
    var type: String { get }
    var amount: String { get }
    var id: String { get }
    var timestamp: String { get }
    var isPositive: Bool { get }
}