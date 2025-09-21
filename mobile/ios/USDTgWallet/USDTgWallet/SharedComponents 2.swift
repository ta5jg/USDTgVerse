//
//  SharedComponents.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-09-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

// MARK: - TabButton Component
struct TabButton: View {
    let title: String
    let isSelected: Bool
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            Text(title)
                .font(.headline)
                .fontWeight(isSelected ? .bold : .medium)
                .foregroundColor(isSelected ? .white : .primary)
                .padding(.vertical, 12)
                .padding(.horizontal, 20)
                .background(
                    RoundedRectangle(cornerRadius: 25)
                        .fill(isSelected ? Color.blue : Color.clear)
                )
                .overlay(
                    RoundedRectangle(cornerRadius: 25)
                        .stroke(Color.blue, lineWidth: isSelected ? 0 : 1)
                )
        }
        .animation(.easeInOut(duration: 0.2), value: isSelected)
    }
}

// MARK: - Asset Logo Component
struct AssetLogo: View {
    let assetName: String
    let size: CGFloat
    
    var body: some View {
        Group {
            if assetName == "USDTg" {
                Image("usdtg_logo")
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: size, height: size)
                    .clipShape(Circle())
                    .overlay(
                        Circle()
                            .stroke(Color.green, lineWidth: 2)
                    )
            } else {
                Circle()
                    .fill(tokenColor(for: assetName))
                    .frame(width: size, height: size)
                    .overlay(
                        Text(tokenSymbol(for: assetName))
                            .font(.system(size: size * 0.4, weight: .bold))
                            .foregroundColor(.white)
                    )
            }
        }
    }
    
    private func tokenColor(for name: String) -> Color {
        switch name {
        case "Bitcoin", "BTC": return .orange
        case "Ethereum", "ETH": return .purple
        case "USDT": return .green
        case "USDC": return .blue
        case "TRON", "TRX": return .red
        case "BNB": return .yellow
        case "SOL", "Solana": return .purple
        case "MATIC": return .purple
        case "AVAX": return .red
        default: return .gray
        }
    }
    
    private func tokenSymbol(for name: String) -> String {
        switch name {
        case "Bitcoin", "BTC": return "₿"
        case "Ethereum", "ETH": return "Ξ"
        case "USDT": return "₮"
        case "USDC": return "©"
        case "TRON", "TRX": return "♦"
        case "BNB": return "◆"
        case "SOL", "Solana": return "◉"
        case "MATIC": return "⬟"
        case "AVAX": return "▲"
        default: return String(name.prefix(1))
        }
    }
}

// MARK: - Network Status Badge
struct NetworkStatusBadge: View {
    let network: String
    let isConnected: Bool
    
    var body: some View {
        HStack(spacing: 4) {
            Circle()
                .fill(isConnected ? Color.green : Color.red)
                .frame(width: 6, height: 6)
            
            Text(network)
                .font(.caption2)
                .fontWeight(.medium)
        }
        .padding(.horizontal, 8)
        .padding(.vertical, 4)
        .background(isConnected ? Color.green.opacity(0.1) : Color.red.opacity(0.1))
        .foregroundColor(isConnected ? .green : .red)
        .cornerRadius(8)
    }
}

// MARK: - Portfolio Summary Card
struct PortfolioSummaryCard: View {
    let totalValue: String
    let assetsCount: Int
    let networksCount: Int
    let isLoading: Bool
    
    var body: some View {
        VStack(spacing: 16) {
            // USDTgVerse Branding
            VStack(spacing: 8) {
                Image("usdtg_logo")
                    .resizable()
                    .aspectRatio(contentMode: .fit)
                    .frame(width: 60, height: 60)
                    .clipShape(Circle())
                    .overlay(
                        Circle()
                            .stroke(LinearGradient(gradient: Gradient(colors: [Color.green, Color.blue]), startPoint: .topLeading, endPoint: .bottomTrailing), lineWidth: 3)
                    )
                
                Text("USDTgVerse Wallet")
                    .font(.headline)
                    .fontWeight(.bold)
            }
            
            // Portfolio Value
            VStack(spacing: 4) {
                Text("Total Portfolio")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                HStack(spacing: 8) {
                    Text(totalValue)
                        .font(.largeTitle)
                        .fontWeight(.bold)
                        .foregroundColor(.primary)
                        .contentTransition(.numericText())
                    
                    if isLoading {
                        ProgressView()
                            .scaleEffect(0.7)
                    }
                }
                
                HStack(spacing: 16) {
                    Text("\(assetsCount) Assets")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("•")
                        .foregroundColor(.secondary)
                    
                    Text("\(networksCount)/8 Networks")
                        .font(.caption)
                        .foregroundColor(.green)
                    
                    Text("•")
                        .foregroundColor(.secondary)
                    
                    Text("LIVE DATA")
                        .font(.caption)
                        .fontWeight(.bold)
                        .foregroundColor(.green)
                }
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
        .padding()
    }
}

// MARK: - Action Button
struct ActionButton: View {
    let title: String
    let icon: String
    let color: Color
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            VStack(spacing: 8) {
                Image(systemName: icon)
                    .font(.title2)
                    .fontWeight(.semibold)
                
                Text(title)
                    .font(.caption)
                    .fontWeight(.medium)
            }
            .foregroundColor(.white)
            .frame(maxWidth: .infinity)
            .padding(.vertical, 16)
            .background(
                LinearGradient(
                    gradient: Gradient(colors: [color, color.opacity(0.8)]),
                    startPoint: .topLeading,
                    endPoint: .bottomTrailing
                )
            )
            .cornerRadius(16)
        }
    }
}
