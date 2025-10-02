//
//  CopyTradingView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-01-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

struct CopyTradingView: View {
    @Environment(\.presentationMode) var presentationMode
    @State private var copyAmount: Double = 100.0
    @State private var showingCopyConfirmation = false
    @State private var selectedStrategy: TradingCopyStrategy? = nil
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    headerView
                    statsView
                    strategiesView
                }
                .padding()
            }
            .navigationTitle("Copy Trading")
            .navigationBarTitleDisplayMode(.large)
        }
        .alert("Confirm Copy Trade", isPresented: $showingCopyConfirmation) {
            Button("Cancel", role: .cancel) { }
            Button("Start Copying") {
                startCopying()
            }
        } message: {
            Text("Start copying trading strategy with \(copyAmount, specifier: "%.0f") USDTg?")
        }
    }
    
    private var headerView: some View {
        VStack(spacing: 12) {
            Text("Copy Professional Traders")
                .font(.title2)
                .fontWeight(.bold)
            
            Text("Automatically copy successful trading strategies from verified traders")
                .font(.subheadline)
                .foregroundColor(.secondary)
                .multilineTextAlignment(.center)
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
    
    private var statsView: some View {
        VStack(spacing: 12) {
            Text("Your Copy Trading Performance")
                .font(.headline)
                .fontWeight(.semibold)
            
            LazyVGrid(columns: [
                GridItem(.flexible()),
                GridItem(.flexible())
            ], spacing: 12) {
                StatCard(title: "Total ROI", value: "+12.3%", color: .green)
                StatCard(title: "Active Copies", value: "5", color: .blue)
                StatCard(title: "Win Rate", value: "78%", color: .purple)
                StatCard(title: "Total Volume", value: "$2,550", color: .orange)
            }
        }
    }
    
    private var strategiesView: some View {
        VStack(spacing: 12) {
            Text("Available Strategies")
                .font(.headline)
                .fontWeight(.semibold)
            
            ForEach(tradingStrategies, id: \.name) { strategy in
                TradingStrategyCard(
                    strategy: strategy,
                    copyAmount: $copyAmount,
                    onCopyTapped: {
                        selectedStrategy = strategy
                        showingCopyConfirmation = true
                    }
                )
            }
        }
    }
    
    private func startCopying() {
        // Implementation for starting copy trading
        print("Starting copy trading with \(copyAmount) USDTg")
    }
}

struct TradingCopyStrategy {
    let id: String
    let name: String
    let trader: String
    let monthlyReturn: Double
    let winRate: Double
    let followers: Int
    let riskLevel: String
    let icon: String
}

struct StatCard: View {
    let title: String
    let value: String
    let color: Color
    
    var body: some View {
        VStack(spacing: 4) {
            Text(value)
                .font(.title3)
                .fontWeight(.bold)
                .foregroundColor(color)
            
            Text(title)
                .font(.caption)
                .foregroundColor(.secondary)
        }
        .frame(maxWidth: .infinity)
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(8)
    }
}

struct TradingStrategyCard: View {
    let strategy: TradingCopyStrategy
    @Binding var copyAmount: Double
    let onCopyTapped: () -> Void
    
    var body: some View {
        VStack(spacing: 12) {
            HStack {
                Text(strategy.icon)
                    .font(.title2)
                
                VStack(alignment: .leading, spacing: 4) {
                    Text(strategy.name)
                        .font(.headline)
                        .fontWeight(.semibold)
                    
                    Text(strategy.trader)
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                VStack(alignment: .trailing, spacing: 4) {
                    Text("+\(strategy.monthlyReturn, specifier: "%.1f")%")
                        .font(.headline)
                        .fontWeight(.bold)
                        .foregroundColor(.green)
                    
                    Text("\(strategy.winRate, specifier: "%.0f")% win rate")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
            
            HStack {
                Label("\(strategy.followers) followers", systemImage: "person.3.fill")
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                Spacer()
                
                Label(strategy.riskLevel, systemImage: "exclamationmark.triangle.fill")
                    .font(.caption)
                    .foregroundColor(strategy.riskLevel == "High" ? .red : strategy.riskLevel == "Medium" ? .yellow : .green)
            }
            
            HStack {
                Stepper(
                    value: $copyAmount,
                    in: 10...10000,
                    step: 10,
                    label: { Text("Copy Amount") }
                )
                
                Spacer()
                
                Button("Copy Trade") {
                    onCopyTapped()
                }
                .buttonStyle(.borderedProminent)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
}

// Data
private let tradingStrategies = [
    TradingCopyStrategy(
        id: "conservative",
        name: "Conservative Portfolio",
        trader: "CryptoMaster",
        monthlyReturn: 8.2,
        winRate: 85,
        followers: 1247,
        riskLevel: "Low",
        icon: "🛡️"
    ),
    TradingCopyStrategy(
        id: "scalping",
        name: "Scalping Bot",
        trader: "ScalpKing",
        monthlyReturn: 12.5,
        winRate: 72,
        followers: 892,
        riskLevel: "Medium",
        icon: "⚡"
    ),
    TradingCopyStrategy(
        id: "scalping",
        name: "Trend Following",
        trader: "TrendTracker",
        monthlyReturn: 15.8,
        winRate: 68,
        followers: 2341,
        riskLevel: "Medium",
        icon: "📈"
    ),
    TradingCopyStrategy(
        id: "trend_following",
        name: "High Frequency",
        trader: "SpeedTrader",
        monthlyReturn: 22.1,
        winRate: 61,
        followers: 567,
        riskLevel: "高",
        icon: "🚀"
    ),
    TradingCopyStrategy(
        id: "high_frequency",
        name: "Borrowing Arbitrage",
        trader: "ArbMaster",
        monthlyReturn: 6.7,
        winRate: 92,
        followers: 1834,
        riskLevel: "Low",
        icon: "⚖️"
    ),
    TradingCopyStrategy(
        id: "momentum",
        name: "Momentum Trading",
        trader: "MomentumGuru",
        monthlyReturn: 18.9,
        winRate: 59,
        followers: 1089,
        riskLevel: "High",
        icon: "💨"
    )
]

#Preview {
    CopyTradingView()
}