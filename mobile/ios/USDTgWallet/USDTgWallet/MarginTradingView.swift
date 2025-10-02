//
//  MarginTradingView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-01-20.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI

enum TradingPair: String, CaseIterable {
    case usdtgUsdt = "USDTg/USDT"
    case usdtgvUsdt = "USDTgV/USDT"
    case usdtggUsdt = "USDTgG/USDT"
    case ozbcUsdt = "OZBC/USDT"
    case usdtgEur = "USDTg/EUR"
    
    var icon: String {
        switch self {
        case .usdtgUsdt: return "🔺"
        case .usdtgvUsdt: return "🔺"
        case .usdtggUsdt: return "🔺"
        case .ozbcUsdt: return "🔺"
        case .usdtgEur: return "🔺"
        }
    }
}

enum OrderType: String, CaseIterable {
    case market = "Market"
    case limit = "Limit"
    case stopLoss = "Stop Loss"
    case takeProfit = "Take Profit"
    
    var icon: String {
        switch self {
        case .market: return "⚡"
        case .limit: return "🎯"
        case .stopLoss: return "🛑"
        case .takeProfit: return "💰"
        }
    }
}

enum MarginTradingPositionSide: String, CaseIterable {
    case long = "Long"
    case short = "Short"
    
    var icon: String {
        switch self {
        case .long: return "📈"
        case .short: return "📉"
        }
    }
    
    var color: Color {
        switch self {
        case .long: return .green
        case .short: return .red
        }
    }
}

struct MarginTradingView: View {
    @Environment(\.presentationMode) var presentationMode
    @State private var selectedPair: TradingPair = .usdtgUsdt
    @State private var selectedSide: MarginTradingPositionSide = .long
    @State private var selectedOrderType: OrderType = .market
    @State private var leverage: Double = 10
    @State private var positionSize: Double = 1000
    @State private var entryPrice: Double = 1.0000
    @State private var currentPrice: Double = 1.0000
    @State private var equity: Double = 10000.0
    @State private var marginUsed: Double = 1000.0
    @State private var unrealizedPnL: Double = 0.0
    @State private var showingOrderConfirmation = false
    @State private var openPositions: [OpenPosition] = []
    @State private var orderHistory: [MarginOrder] = []
    
    var marginCallLevel: Double {
        return equity * 0.8 // 80% of equity
    }
    
    var liquidationLevel: Double {
        return equity * 0.5 // 50% of equity
    }
    
    var body: some View {
        NavigationView {
            ScrollView {
                VStack(spacing: 20) {
                    headerView
                    tradingPanelView
                    portfolioOverviewView
                    openPositionsSection
                    orderHistorySection
                }
                .padding()
            }
            .navigationTitle("Margin Trading")
            .navigationBarTitleDisplayMode(.inline)
            .navigationBarItems(trailing: Button("Done") {
                presentationMode.wrappedValue.dismiss()
            })
        }
        .alert("Confirm Margin Trade", isPresented: $showingOrderConfirmation) {
            Button("Cancel", role: .cancel) { }
            Button("Execute Trade") {
                executeTrade()
            }
        } message: {
            Text("Place \(selectedSide.rawValue) order for \(positionSize, specifier: "%.0f") \(selectedPair.rawValue.components(separatedBy: "/").first ?? "") with \(leverage, specifier: "%.0f")x leverage?")
        }
    }
    
    private var headerView: some View {
        VStack(spacing: 16) {
            // Header Stats
            HStack(spacing: 16) {
                StatCard(title: "Equity", value: String(format: "$%.0f", equity), color: .green)
                StatCard(title: "Margin Used", value: String(format: "$%.0f", marginUsed), color: .orange)
                StatCard(title: "PnL", value: String(format: "$%.2f", unrealizedPnL), color: .blue)
                StatCard(title: "Margin Level", value: String(format: "%.1f%%", marginUsed / equity * 100), color: .purple)
            }
            
            // Price Ticker
            HStack {
                Text(selectedPair.rawValue)
                    .font(.title2)
                    .fontWeight(.bold)
                
                Spacer()
                
                VStack(alignment: .trailing) {
                    Text("$\(currentPrice, specifier: "%.4f")")
                        .font(.title2)
                        .fontWeight(.bold)
                    
                    Text(unrealizedPnL >= 0 ? "+$\(unrealizedPnL, specifier: "%.2f")" : "-$\(abs(unrealizedPnL), specifier: "%.2f")")
                        .font(.subheadline)
                        .foregroundColor(unrealizedPnL >= 0 ? .green : .red)
                }
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
        }
    }
    
    private var tradingPanelView: some View {
        VStack(spacing: 16) {
            Text("New Position")
                .font(.headline)
                .fontWeight(.semibold)
            
            // Trading Pair Selector
            VStack(spacing: 8) {
                Text("Trading Pair")
                    .font(.subheadline)
                    .fontWeight(.medium)
                
                Picker("Pair", selection: $selectedPair) {
                    ForEach(TradingPair.allCases, id: \.self) { pair in
                        Text(pair.rawValue).tag(pair)
                    }
                }
                .pickerStyle(MenuPickerStyle())
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color(.systemGray6))
                .cornerRadius(8)
            }
            
            // Position Side
            HStack(spacing: 12) {
                ForEach(MarginTradingPositionSide.allCases, id: \.self) { side in
                    Button(action: { selectedSide = side }) {
                        HStack {
                            Text(side.icon)
                            Text(side.rawValue)
                        }
                        .frame(maxWidth: .infinity)
                        .padding()
                        .background(selectedSide == side ? side.color.opacity(0.2) : Color(.systemGray6))
                        .foregroundColor(selectedSide == side ? side.color : .primary)
                        .cornerRadius(8)
                        .overlay(
                            RoundedRectangle(cornerRadius: 8)
                                .stroke(selectedSide == side ? side.color : Color.clear, lineWidth: 2)
                        )
                    }
                    .buttonStyle(PlainButtonStyle())
                }
            }
            
            // Leverage Selector
            VStack(spacing: 8) {
                HStack {
                    Text("Leverage: \(leverage, specifier: "%.0f")x")
                        .font(.subheadline)
                        .fontWeight(.medium)
                    
                    Spacer()
                    
                    Text("Max Leverage: 100x")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Slider(value: $leverage, in: 1...100, step: 1)
                    .accentColor(selectedSide.color)
            }
            
            // Position Size
            VStack(spacing: 8) {
                HStack {
                    Text("Position Size")
                        .font(.subheadline)
                        .fontWeight(.medium)
                    
                    Spacer()
                    
                    Text("Max: $\(equity * leverage, specifier: "%.0f")")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                HStack {
                    TextField("Position Size", value: $positionSize, format: .number)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                        .keyboardType(.decimalPad)
                    
                    Text("USDTg")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                }
            }
            
            // Order Type
            VStack(spacing: 8) {
                Text("Order Type")
                    .font(.subheadline)
                    .fontWeight(.medium)
                
                Picker("Order Type", selection: $selectedOrderType) {
                    ForEach(OrderType.allCases, id: \.self) { type in
                        Text(type.rawValue).tag(type)
                    }
                }
                .pickerStyle(SegmentedPickerStyle())
            }
            
            // Risk Indicators
            riskIndicatorsView
            
            // Execute Button
            Button(action: {
                showingOrderConfirmation = true
            }) {
                Text("Execute \(selectedSide.rawValue) Order")
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(selectedSide.color)
                    .foregroundColor(.white)
                    .cornerRadius(12)
                    .font(.headline)
            }
            .disabled(positionSize <= 0 || equity <= marginUsed + positionSize)
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
    }
    
    private var riskIndicatorsView: some View {
        VStack(spacing: 8) {
            Text("Risk Indicators")
                .font(.subheadline)
                .fontWeight(.medium)
            
            HStack {
                VStack(alignment: .leading, spacing: 4) {
                    Text("Margin Call Level")
                        .font(.caption)
                        .foregroundColor(.orange)
                    Text("$\(marginCallLevel, specifier: "%.0f")")
                        .font(.caption)
                        .fontWeight(.bold)
                }
                
                Spacer()
                
                VStack(alignment: .center, spacing: 4) {
                    Text("Liquidation Level")
                        .font(.caption)
                        .foregroundColor(.red)
                    Text("$\(liquidationLevel, specifier: "%.0f")")
                        .font(.caption)
                        .fontWeight(.bold)
                }
                
                Spacer()
                
            }
            .padding(.horizontal, 8)
            .frame(height: 40)
            .background(Color(.systemBackground))
            .cornerRadius(8)
        }
    }
    
    private var portfolioOverviewView: some View {
        VStack(alignment: .leading, spacing: 12) {
            Text("Portfolio Summary")
                .font(.headline)
                .fontWeight(.semibold)
            
            VStack(spacing: 8) {
                PortfolioMetricsCard(
                    title: "Total Equity",
                    value: String(format: "$%.2f", equity),
                    change: String(format: "+$%.2f", abs(unrealizedPnL)),
                    changeColor: unrealizedPnL >= 0 ? .green : .red
                )
                
                PortfolioMetricsCard(
                    title: "Available Margin",
                    value: String(format: "$%.2f", equity - marginUsed),
                    change: "Free",
                    changeColor: .blue
                )
                
                PortfolioMetricsCard(
                    title: "Margin Ratio",
                    value: String(format: "%.1f%%", marginUsed / equity * 100),
                    change: marginUsed / equity > 0.8 ? "⚠️ High Risk" : "✅ Safe",
                    changeColor: marginUsed / equity > 0.8 ? .red : .green
                )
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
    
    private var openPositionsSection: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Text("Open Positions")
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text("\(openPositions.count)")
                    .font(.caption)
                    .padding(.horizontal, 8)
                    .padding(.vertical, 4)
                    .background(Color.blue.opacity(0.2))
                    .foregroundColor(.blue)
                    .cornerRadius(4)
            }
            
            if openPositions.isEmpty {
                VStack(spacing: 12) {
                    Image(systemName: "chart.line.uptrend.xyaxis")
                        .font(.system(size: 40))
                        .foregroundColor(.gray)
                    
                    Text("No open positions")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                    
                    Text("Start trading to see your positions here")
                        .font(.caption)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color(.systemGray6))
            } else {
                VStack(spacing: 8) {
                    ForEach(openPositions) { position in
                        PositionRowView(position: position)
                    }
                }
            }
        }
    }
    
    private var orderHistorySection: some View {
        VStack(alignment: .leading, spacing: 12) {
            HStack {
                Text("Recent Orders")
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Button("View All") {
                    // Navigate to full order history
                }
                .font(.caption)
                .foregroundColor(.blue)
            }
            
            if orderHistory.isEmpty {
                VStack(spacing: 12) {
                    Image(systemName: "list.bullet")
                        .font(.system(size: 40))
                        .foregroundColor(.gray)
                    
                    Text("No orders placed")
                        .font(.subheadline)
                        .foregroundColor(.secondary)
                    
                    Text("Your trading history will appear here")
                        .font(.caption)
                        .foregroundColor(.secondary)
                        .multilineTextAlignment(.center)
                }
                .frame(maxWidth: .infinity)
                .padding()
                .background(Color(.systemGray6))
            } else {
                VStack(spacing: 8) {
                    ForEach(orderHistory.prefix(3)) { order in
                        OrderRowView(order: order)
                    }
                }
            }
        }
    }
    
    private func executeTrade() {
        // Create new position
        let newPosition = OpenPosition(
            id: UUID().uuidString,
            pair: selectedPair,
            side: selectedSide,
            size: positionSize,
            entryPrice: currentPrice,
            leverage: leverage,
            unrealizedPnL: 0,
            timestamp: Date()
        )
        
        openPositions.append(newPosition)
        
        // Reset form
        positionSize = 0
        showingOrderConfirmation = false
    }
}

struct PortfolioMetricsCard: View {
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

struct PositionRowView: View {
    let position: OpenPosition
    
    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            HStack {
                Text(position.pair.rawValue)
                    .font(.subheadline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text(position.side.rawValue)
                    .font(.caption)
                    .padding(.horizontal, 8)
                    .padding(.vertical, 2)
                    .background(position.side.color.opacity(0.2))
                    .foregroundColor(position.side.color)
                    .cornerRadius(4)
                
                Text("\(position.leverage, specifier: "%.0f")x")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            HStack {
                VStack(alignment: .leading, spacing: 2) {
                    Text("Size: \(position.size, specifier: "%.0f")")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    
                    Text("Entry: $\(position.entryPrice, specifier: "%.4f")")
                        .font(.caption)
                        .foregroundColor(.secondary)
                }
                
                Spacer()
                
                VStack(alignment: .trailing, spacing: 2) {
                    Text(position.unrealizedPnL >= 0 ? "+$\(position.unrealizedPnL, specifier: "%.2f")" : "-$\(abs(position.unrealizedPnL), specifier: "%.2f")")
                        .font(.subheadline)
                        .fontWeight(.medium)
                        .foregroundColor(position.unrealizedPnL >= 0 ? .green : .red)
                    
                    Text(position.timestamp, style: .time)
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

struct OrderRowView: View {
    let order: MarginOrder
    
    var body: some View {
        HStack {
            Image(systemName: order.side.icon)
                .foregroundColor(order.side.color)
                .frame(width: 30)
            
            VStack(alignment: .leading, spacing: 4) {
                Text(order.pair.rawValue)
                    .font(.subheadline)
                    .fontWeight(.medium)
                
                Text("\(order.side.rawValue) • \(order.type.rawValue)")
                    .font(.caption)
                    .foregroundColor(.secondary)
            }
            
            Spacer()
            
            VStack(alignment: .trailing, spacing: 4) {
                Text("\(order.size, specifier: "%.0f")")
                    .font(.subheadline)
                    .fontWeight(.medium)
                
                Text(order.status.rawValue)
                    .font(.caption)
                    .foregroundColor(order.status.color)
            }
        }
        .padding()
        .background(Color(.systemBackground))
        .cornerRadius(8)
    }
}

// MARK: - Supporting Models
struct OpenPosition: Identifiable {
    let id: String
    let pair: TradingPair
    let side: MarginTradingPositionSide
    let size: Double
    let entryPrice: Double
    let leverage: Double
    let unrealizedPnL: Double
    let timestamp: Date
}

struct MarginOrder: Identifiable {
    let id = UUID()
    let pair: TradingPair
    let side: MarginTradingPositionSide
    let type: OrderType
    let size: Double
    let price: Double
    let status: OrderStatus
    let timestamp: Date
}

enum OrderStatus: String, CaseIterable {
    case pending = "Pending"
    case filled = "Filled"
    case cancelled = "Cancelled"
    case partial = "Partial"
    
    var color: Color {
        switch self {
        case .pending: return .orange
        case .filled: return .green
        case .cancelled: return .red
        case .partial: return .blue
        }
    }
}

enum MarginTradingMarginTradingPositionSide {
    case side
    
    var icon: String {
        switch self {
        case .side: return "arrow.up"
        }
    }
    
    var color: Color {
        switch self {
        case .side: return .blue
        }
    }
}


extension MarginTradingPositionSide: RawRepresentable {
    var rawValue: String {
        return "Long"
    }
    
    init?(rawValue: String) {
        return nil
    }
}

extension MarginTradingPositionSide: Equatable {
    
}

struct MarginTradingView_Previews: PreviewProvider {
    static var previews: some View {
        MarginTradingView()
    }
}
