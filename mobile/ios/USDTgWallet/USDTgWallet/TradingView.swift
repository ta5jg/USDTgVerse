//
//  TradingView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-10-02.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

enum TradingAction: String, CaseIterable {
    case buy = "Buy"
    case sell = "Sell"
    case marketOrder = "Market Order"
    case benchmarkOrder = "Benchmark Order"
    case smartOrder = "Smart Order"
    case basketOrder = "Basket Order"
    case portfolioRebalance = "Portfolio Rebalance"
    case volumeOrder = "Volume Order"
    case liquidityOrder = "Liquidity Order"
    case advancedOrder = "Advanced Order"

    var icon: String {
        switch self {
        case .buy: return "📈"
        case .sell: return "📉"
        case .marketOrder: return "🎯"
        case .benchmarkOrder: return "🎯"
        case .smartOrder: return "🧠"
        case .basketOrder: return "🛍️"
        case .portfolioRebalance: return "⚖️"
        case .volumeOrder: return "📊"
        case .liquidityOrder: return "💧"
        case .advancedOrder: return "⚙️"
        }
    }
}

extension TradingAction: FeatureProtocol {
    var title: String { rawValue }
    
    var description: String {
        switch self {
        case .buy: return "Execute purchase orders"
        case .sell: return "Execute sell orders"
        case .marketOrder: return "Place market orders"
        case .benchmarkOrder: return "Compare against benchmarks"
        case .smartOrder: return "AI-powered order execution"
        case .basketOrder: return "Execute basket of securities"
        case .portfolioRebalance: return "Rebalance entire portfolio"
        case .volumeOrder: return "Volume-based order types"
        case .liquidityOrder: return "Liquidity-focused orders"
        case .advancedOrder: return "Complex order strategies"
        }
    }
}

extension TradingAction: Identifiable {
    var id: String { rawValue }
}

struct TradingView: View {
    @State private var selectedPair: String = "USDTg/USDT"
    @State private var currentPrice: Double = 1.00
    @State private var priceChange: Double = 0.02
    @State private var leverage: Int = 10
    @State private var selectedAction: TradingAction?
    @State private var orderCount: Int = 0
    @State private var profits: Double = 2147.83
    @State private var currentPortfolioValue: Double = 50000.00

    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    headerView
                    tradingActionsGrid
                    livePricesSection
                    orderBookSection
                    positionsSection
                    orderHistorySection
                }
                .padding()
            }
            .navigationTitle("Trading Engine")
            .navigationBarTitleDisplayMode(.inline)
        }
    }

    private var headerView: some View {
        VStack(spacing: 12) {
            Picker("Trading Pair", selection: $selectedPair) {
                Text("USDTg/USDT").tag("USDTg/USDT")
                Text("USDTgV/USDT").tag("USDTgV/USDT")
                Text("USDTgG/USDT").tag("USDTgG/USDT")
            }
            .pickerStyle(SegmentedPickerStyle())

            HStack {
                Text(selectedPair)
                    .font(.title2)
                    .fontWeight(.bold)
                Spacer()
                VStack(alignment: .trailing) {
                    Text(String(format: "$%.4f", currentPrice))
                        .font(.title2)
                        .fontWeight(.bold)
                    Text(String(format: "%+.2f%%", priceChange * 100))
                        .foregroundColor(priceChange >= 0 ? .green : .red)
                        .font(.subheadline)
                }
            }
            .padding(.horizontal)

            HStack {
                StatCard(title: "Leverage", value: "\(leverage)x", color: .orange)
                StatCard(title: "24h Volume", value: "$15.2M", color: .green)
                StatCard(title: "Open Interest", value: "$8.9M", color: .blue)
                StatCard(title: "Orders", value: "42", color: .purple)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
    }

    private var tradingActionsGrid: some View {
        LazyVGrid(columns: [GridItem(.flexible()), GridItem(.flexible())], spacing: 16) {
            ForEach(Array(TradingAction.allCases.prefix(8)), id: \.self) { action in
                TradingActionCard(action: action, onAction: {
                    selectedAction = action
                })
            }
        }
    }

    private var livePricesSection: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("📈 Live Prices")
                .font(.headline)
                .fontWeight(.semibold)
            
            VStack(spacing: 8) {
                PriceRow(symbol: "USDTg/USDT", price: 1.0000, change: 2.14, volume: "15.2M")
                PriceRow(symbol: "USDTgV/USDT", price: 0.9980, change: -0.85, volume: "12.8M")
                PriceRow(symbol: "USDTgG/USDT", price: 1.0015, change: 1.23, volume: "8.5M")
                PriceRow(symbol: "OZBC/USDT", price: 256.20, change: 3.45, volume: "25.1M")
                PriceRow(symbol: "USDTg/EUR", price: 0.9150, change: -1.12, volume: "5.2M")
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }

    private var orderBookSection: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Text("📊 Order Book")
                    .font(.headline)
                    .fontWeight(.semibold)
                Spacer()
                Button("Refresh") {
                    // Refresh order book
                }
                .foregroundColor(.blue)
                .font(.caption)
            }
            
            VStack(spacing: 4) {
                HStack {
                    Text("Price")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Spacer()
                    Text("Amount")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Spacer()
                    Text("Total")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                .padding(.horizontal)
                
                // Sample order book data
                ForEach(0..<5) { index in
                    HStack {
                        Text("\(1.0000 + Double(index) * 0.001, specifier: "%.4f")")
                            .foregroundColor(.red)
                            .font(.system(.caption, design: .monospaced))
                        Spacer()
                        Text("\(Double.random(in: 100...1000), specifier: "%.2f")")
                            .font(.caption)
                            .font(.system(.caption, design: .monospaced))
                        Spacer()
                        Text("\(Double.random(in: 500...5000), specifier: "%.2f")")
                            .font(.caption)
                            .font(.system(.caption, design: .monospaced))
                    }
                    .padding(.horizontal)
                }
                
                Divider()
                    .background(Color.gray)
                
                ForEach(0..<5) { index in
                    HStack {
                        Text("\(1.0000 - Double(index) * 0.001, specifier: "%.4f")")
                            .foregroundColor(.green)
                            .font(.caption)
                            .font(.system(.caption, design: .monospaced))
                        Spacer()
                        Text("\(Double.random(in: 100...1000), specifier: "%.2f")")
                            .font(.caption)
                            .font(.system(.caption, design: .monospaced))
                        Spacer()
                        Text("\(Double.random(in: 500...5000), specifier: "%.2f")")
                            .font(.caption)
                            .font(.system(.caption, design: .monospaced))
                    }
                    .padding(.horizontal)
                }
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }

    private var positionsSection: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Text("💼 Positions")
                    .font(.headline)
                    .fontWeight(.semibold)
                Spacer()
                Button("View All") {
                    // Navigate to positions
                }
                .foregroundColor(.blue)
                .font(.caption)
            }
            
            VStack(spacing: 8) {
                PositionCard(
                    symbol: "USDTg/USDT",
                    type: "Long",
                    size: "1,000",
                    entryPrice: "$0.9980",
                    currentPrice: "$1.0000",
                    pnl: "$2.00",
                    pnlPercent: "+0.20%"
                )
                
                PositionCard(
                    symbol: "OZBC/USDT",
                    type: "Short",
                    size: "100",
                    entryPrice: "$258.50",
                    currentPrice: "$256.20",
                    pnl: "$230.00",
                    pnlPercent: "+0.89%"
                )
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }

    private var orderHistorySection: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Text("📋 Recent Orders")
                    .font(.headline)
                    .fontWeight(.semibold)
                Spacer()
                Button("View All") {
                    // Navigate to order history
                }
                .foregroundColor(.blue)
                .font(.caption)
            }
            
            VStack(spacing: 8) {
                OrderHistoryCard(
                    symbol: "USDTg/USDT",
                    side: "BUY",
                    amount: "500.00",
                    price: "$1.0000",
                    status: "Filled",
                    time: "10:25:30"
                )
                
                OrderHistoryCard(
                    symbol: "OZBC/USDT",
                    side: "SELL",
                    amount: "50.00",
                    price: "$258.50",
                    status: "Partially Filled",
                    time: "09:45:15"
                )
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
}

struct TradingActionCard: View {
    let action: TradingAction
    let onAction: () -> Void
    
    var body: some View {
        Button(action: onAction) {
            VStack(spacing: 12) {
                Text(action.icon)
                    .font(.system(size: 32))
                
                Text(action.title)
                    .font(.headline)
                    .fontWeight(.semibold)
                    .multilineTextAlignment(.center)
                    .foregroundColor(.primary)
                
                Text(action.description)
                    .font(.caption)
                    .foregroundColor(.secondary)
                    .multilineTextAlignment(.center)
            }
            .frame(maxWidth: .infinity, minHeight: 140)
            .padding()
            .background(Color(.systemBackground))
            .cornerRadius(16)
            .shadow(color: Color.black.opacity(0.1), radius: 2, x: 0, y: 1)
        }
        .buttonStyle(PlainButtonStyle())
    }
}

struct PriceRow: View {
    let symbol: String
    let price: Double
    let change: Double
    let volume: String
    
    var body: some View {
        HStack {
            Text(symbol)
                .font(.subheadline)
                .fontWeight(.medium)
                .frame(width: 100, alignment: .leading)
            
            Spacer()
            
            Text("\(price, specifier: "%.4f")")
                .font(.subheadline)
                .fontWeight(.medium)
                .font(.system(.caption, design: .monospaced))
            
            Spacer()
            
            Text("\(change, specifier: "%+.2f")%")
                .font(.subheadline)
                .fontWeight(.medium)
                .foregroundColor(change >= 0 ? .green : .red)
            
            Spacer()
            
            Text(volume)
                .font(.caption)
                .foregroundColor(.secondary)
                .frame(width: 60, alignment: .trailing)
        }
        .padding(.vertical, 4)
    }
}

struct PositionCard: View {
    let symbol: String
    let type: String
    let size: String
    let entryPrice: String
    let currentPrice: String
    let pnl: String
    let pnlPercent: String
    
    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            HStack {
                Text(symbol)
                    .font(.subheadline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text(type)
                    .font(.caption)
                    .padding(.horizontal, 8)
                    .padding(.vertical, 2)
                    .background(type == "Long" ? Color.green.opacity(0.2) : Color.red.opacity(0.2))
                    .foregroundColor(type == "Long" ? .green : .red)
                    .cornerRadius(4)
            }
            
            HStack {
                Text("Size: \(size)")
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                Spacer()
                
                Text("Entry: \(entryPrice)")
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                Spacer()
                
                VStack(alignment: .trailing) {
                    Text("PnL: \(pnl)")
                        .font(.subheadline)
                        .fontWeight(.medium)
                        .foregroundColor(pnl.hasPrefix("+") ? .green : .red)
                    
                    Text(pnlPercent)
                        .font(.caption)
                        .foregroundColor(pnlPercent.hasPrefix("+") ? .green : .red)
                }
            }
        }
        .padding()
        .background(Color(.systemBackground))
        .cornerRadius(8)
    }
}

struct OrderHistoryCard: View {
    let symbol: String
    let side: String
    let amount: String
    let price: String
    let status: String
    let time: String
    
    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            HStack {
                Text(symbol)
                    .font(.subheadline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text(side)
                    .font(.caption)
                    .padding(.horizontal, 8)
                    .padding(.vertical, 2)
                    .background(side == "BUY" ? Color.green.opacity(0.2) : Color.red.opacity(0.2))
                    .foregroundColor(side == "BUY" ? .green : .red)
                    .cornerRadius(4)
            }
            
            HStack {
                VStack(alignment: .leading, spacing: 2) {
                    Text("Amount: \(amount)")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("Price: \(price)")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                VStack(alignment: .trailing, spacing: 2) {
                    Text(status)
                        .font(.caption)
                        .fontWeight(.medium)
                        .foregroundColor(status == "Filled" ? .green : .orange)
                    
                    Text(time)
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
            }
        }
        .padding()
        .background(Color(.systemBackground))
        .cornerRadius(8)
    }
}

struct TradingView_Previews: PreviewProvider {
    static var previews: some View {
        TradingView()
    }
}