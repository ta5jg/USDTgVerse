//
//  TradingView.swift
//  USDTgWallet
//
//  Created by USDTgVerse Team on 2025-10-02.
//  Copyright © 2025 USDTgVerse. All rights reserved.
//

import SwiftUI
import Charts

enum TradingTab: String, CaseIterable {
    case trade = "Trade"
    case positions = "Positions"
    case orders = "Orders"
    case history = "History"
}

enum OrderType: String, CaseIterable {
    case market = "Market"
    case limit = "Limit"
    case stop = "Stop"
}

enum OrderSide: String, CaseIterable {
    case buy = "Buy"
    case sell = "Sell"
}

struct TradingView: View {
    @State private var selectedTab: TradingTab = .trade
    @State private var selectedSymbol = "USDTg/USDT"
    @State private var currentPrice: Double = 1.00
    @State private var priceChange: Double = 0.02
    @State private var balance: Double = 10000.00
    @State private var availableBalance: Double = 8500.00
    @State private var marginUsed: Double = 1500.00
    @State private var unrealizedPnL: Double = 250.00
    
    let symbols = ["USDTg/USDT", "USDTgV/USDT", "USDTgG/USDT", "USDTg/ETH", "USDTg/BTC"]
    
    var body: some View {
        NavigationView {
            VStack(spacing: 0) {
                // Header with Balance
                headerView
                
                // Tab Selector
                tabSelector
                
                // Content
                tabContent
            }
            .navigationTitle("Trading")
            .navigationBarTitleDisplayMode(.inline)
        }
    }
    
    private var headerView: some View {
        VStack(spacing: 16) {
            // Trading Logo
            VStack(spacing: 8) {
                Circle()
                    .fill(LinearGradient(gradient: Gradient(colors: [Color.blue, Color.purple]), startPoint: .topLeading, endPoint: .bottomTrailing))
                    .frame(width: 60, height: 60)
                    .overlay(
                        Image(systemName: "chart.line.uptrend.xyaxis")
                            .font(.title2)
                            .foregroundColor(.white)
                    )
                
                Text("Leveraged Trading")
                    .font(.headline)
                    .fontWeight(.bold)
            }
            
            // Balance Overview
            VStack(spacing: 12) {
                HStack {
                    VStack(alignment: .leading) {
                        Text("Trading Balance")
                            .font(.caption)
                            .foregroundColor(.secondary)
                        Text("$\(balance, specifier: "%.2f")")
                            .font(.title2)
                            .fontWeight(.bold)
                    }
                    
                    Spacer()
                    
                    VStack(alignment: .trailing) {
                        Text("Unrealized PnL")
                            .font(.caption)
                            .foregroundColor(.secondary)
                        Text("$\(unrealizedPnL, specifier: "%.2f")")
                            .font(.title2)
                            .fontWeight(.bold)
                            .foregroundColor(unrealizedPnL >= 0 ? .green : .red)
                    }
                }
                
                HStack {
                    VStack(alignment: .leading) {
                        Text("Available")
                            .font(.caption)
                            .foregroundColor(.secondary)
                        Text("$\(availableBalance, specifier: "%.2f")")
                            .font(.subheadline)
                            .fontWeight(.semibold)
                    }
                    
                    Spacer()
                    
                    VStack(alignment: .trailing) {
                        Text("Margin Used")
                            .font(.caption)
                            .foregroundColor(.secondary)
                        Text("$\(marginUsed, specifier: "%.2f")")
                            .font(.subheadline)
                            .fontWeight(.semibold)
                    }
                }
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(16)
        .padding()
    }
    
    private var tabSelector: some View {
        HStack(spacing: 0) {
            ForEach(TradingTab.allCases, id: \.self) { tab in
                Button(action: {
                    selectedTab = tab
                }) {
                    VStack(spacing: 4) {
                        Text(tab.rawValue)
                            .font(.subheadline)
                            .fontWeight(.medium)
                            .foregroundColor(selectedTab == tab ? .blue : .secondary)
                        
                        Rectangle()
                            .fill(selectedTab == tab ? Color.blue : Color.clear)
                            .frame(height: 2)
                    }
                    .frame(maxWidth: .infinity)
                }
            }
        }
        .padding(.horizontal)
        .background(Color(.systemBackground))
    }
    
    @ViewBuilder
    private var tabContent: some View {
        switch selectedTab {
        case .trade:
            TradeView(
                selectedSymbol: $selectedSymbol,
                currentPrice: $currentPrice,
                priceChange: $priceChange,
                symbols: symbols
            )
        case .positions:
            PositionsView()
        case .orders:
            OrdersView()
        case .history:
            HistoryView()
        }
    }
}

struct TradeView: View {
    @Binding var selectedSymbol: String
    @Binding var currentPrice: Double
    @Binding var priceChange: Double
    let symbols: [String]
    
    @State private var orderType: OrderType = .market
    @State private var orderSide: OrderSide = .buy
    @State private var quantity = ""
    @State private var price = ""
    @State private var leverage: Double = 1
    @State private var isPlacingOrder = false
    @State private var showingSuccess = false
    
    var body: some View {
        ScrollView {
            VStack(spacing: 24) {
                // Symbol Selector
                symbolSelector
                
                // Price Display
                priceDisplay
                
                // Order Form
                orderForm
                
                // Order Buttons
                orderButtons
            }
            .padding()
        }
        .alert("Order Placed!", isPresented: $showingSuccess) {
            Button("OK") { }
        } message: {
            Text("Your \(orderSide.rawValue) order for \(quantity) \(selectedSymbol) has been placed successfully!")
        }
    }
    
    private var symbolSelector: some View {
        VStack(alignment: .leading, spacing: 8) {
            Text("Trading Pair")
                .font(.headline)
            
            ScrollView(.horizontal, showsIndicators: false) {
                HStack(spacing: 12) {
                    ForEach(symbols, id: \.self) { symbol in
                        Button(action: {
                            selectedSymbol = symbol
                            updatePrice()
                        }) {
                            Text(symbol)
                                .font(.subheadline)
                                .fontWeight(.medium)
                                .foregroundColor(selectedSymbol == symbol ? .white : .primary)
                                .padding(.horizontal, 16)
                                .padding(.vertical, 8)
                                .background(selectedSymbol == symbol ? Color.blue : Color(.systemGray6))
                                .cornerRadius(20)
                        }
                    }
                }
                .padding(.horizontal)
            }
        }
    }
    
    private var priceDisplay: some View {
        VStack(spacing: 8) {
            Text("Current Price")
                .font(.headline)
            
            HStack {
                Text("$\(currentPrice, specifier: "%.6f")")
                    .font(.system(size: 24, weight: .bold, design: .monospaced))
                
                Spacer()
                
                HStack(spacing: 4) {
                    Image(systemName: priceChange >= 0 ? "arrow.up" : "arrow.down")
                        .foregroundColor(priceChange >= 0 ? .green : .red)
                    
                    Text("\(priceChange * 100, specifier: "%.2f")%")
                        .font(.subheadline)
                        .fontWeight(.semibold)
                        .foregroundColor(priceChange >= 0 ? .green : .red)
                }
            }
            .padding()
            .background(Color(.systemGray6))
            .cornerRadius(12)
        }
    }
    
    private var orderForm: some View {
        VStack(spacing: 20) {
            // Order Type
            VStack(alignment: .leading, spacing: 8) {
                Text("Order Type")
                    .font(.headline)
                
                Picker("Order Type", selection: $orderType) {
                    ForEach(OrderType.allCases, id: \.self) { type in
                        Text(type.rawValue).tag(type)
                    }
                }
                .pickerStyle(SegmentedPickerStyle())
            }
            
            // Quantity
            VStack(alignment: .leading, spacing: 8) {
                Text("Quantity")
                    .font(.headline)
                
                TextField("0.00", text: $quantity)
                    .keyboardType(.decimalPad)
                    .textFieldStyle(RoundedBorderTextFieldStyle())
            }
            
            // Price (for limit/stop orders)
            if orderType != .market {
                VStack(alignment: .leading, spacing: 8) {
                    Text("Price")
                        .font(.headline)
                    
                    TextField("0.00", text: $price)
                        .keyboardType(.decimalPad)
                        .textFieldStyle(RoundedBorderTextFieldStyle())
                }
            }
            
            // Leverage
            VStack(alignment: .leading, spacing: 8) {
                Text("Leverage: \(Int(leverage))x")
                    .font(.headline)
                
                Slider(value: $leverage, in: 1...100, step: 1)
                    .accentColor(.blue)
            }
        }
    }
    
    private var orderButtons: some View {
        HStack(spacing: 16) {
            Button(action: {
                orderSide = .buy
                placeOrder()
            }) {
                Text("Buy")
                    .font(.headline)
                    .fontWeight(.semibold)
                    .foregroundColor(.white)
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.green)
                    .cornerRadius(12)
            }
            
            Button(action: {
                orderSide = .sell
                placeOrder()
            }) {
                Text("Sell")
                    .font(.headline)
                    .fontWeight(.semibold)
                    .foregroundColor(.white)
                    .frame(maxWidth: .infinity)
                    .padding()
                    .background(Color.red)
                    .cornerRadius(12)
            }
        }
    }
    
    private func updatePrice() {
        // Simulate price update
        let basePrice = getBasePrice(for: selectedSymbol)
        let change = (Double.random(in: -0.02...0.02))
        currentPrice = basePrice * (1 + change)
        priceChange = change
    }
    
    private func getBasePrice(for symbol: String) -> Double {
        switch symbol {
        case "USDTg/USDT": return 1.00
        case "USDTgV/USDT": return 0.50
        case "USDTgG/USDT": return 5.00
        case "USDTg/ETH": return 0.0005
        case "USDTg/BTC": return 0.000022
        default: return 1.00
        }
    }
    
    private func placeOrder() {
        guard !quantity.isEmpty, Double(quantity) ?? 0 > 0 else { return }
        
        isPlacingOrder = true
        
        // Simulate order placement
        DispatchQueue.main.asyncAfter(deadline: .now() + 1.5) {
            isPlacingOrder = false
            showingSuccess = true
        }
    }
}

struct PositionsView: View {
    @State private var positions: [Position] = [
        Position(
            id: "pos_001",
            symbol: "USDTg/USDT",
            side: .buy,
            quantity: 1000,
            entryPrice: 0.9995,
            currentPrice: 1.0005,
            leverage: 10,
            pnl: 1.00
        ),
        Position(
            id: "pos_002",
            symbol: "USDTgV/USDT",
            side: .sell,
            quantity: 500,
            entryPrice: 0.5020,
            currentPrice: 0.4980,
            leverage: 5,
            pnl: 2.00
        )
    ]
    
    var body: some View {
        ScrollView {
            LazyVStack(spacing: 12) {
                ForEach(positions) { position in
                    PositionCard(position: position) {
                        closePosition(position.id)
                    }
                }
            }
            .padding()
        }
    }
    
    private func closePosition(_ positionId: String) {
        positions.removeAll { $0.id == positionId }
    }
}

struct PositionCard: View {
    let position: Position
    let onClose: () -> Void
    
    var body: some View {
        VStack(spacing: 12) {
            HStack {
                Text(position.symbol)
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text(position.side.rawValue)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(.white)
                    .padding(.horizontal, 8)
                    .padding(.vertical, 4)
                    .background(position.side == .buy ? Color.green : Color.red)
                    .cornerRadius(6)
            }
            
            HStack {
                VStack(alignment: .leading) {
                    Text("Quantity")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("\(position.quantity)")
                        .font(.subheadline)
                        .fontWeight(.medium)
                }
                
                Spacer()
                
                VStack(alignment: .center) {
                    Text("Entry Price")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("$\(position.entryPrice, specifier: "%.6f")")
                        .font(.subheadline)
                        .fontWeight(.medium)
                }
                
                Spacer()
                
                VStack(alignment: .trailing) {
                    Text("Current Price")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("$\(position.currentPrice, specifier: "%.6f")")
                        .font(.subheadline)
                        .fontWeight(.medium)
                }
            }
            
            HStack {
                VStack(alignment: .leading) {
                    Text("Leverage")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("\(position.leverage)x")
                        .font(.subheadline)
                        .fontWeight(.medium)
                }
                
                Spacer()
                
                VStack(alignment: .trailing) {
                    Text("PnL")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("$\(position.pnl, specifier: "%.2f")")
                        .font(.subheadline)
                        .fontWeight(.semibold)
                        .foregroundColor(position.pnl >= 0 ? .green : .red)
                }
            }
            
            Button("Close Position") {
                onClose()
            }
            .font(.subheadline)
            .foregroundColor(.white)
            .padding(.horizontal, 16)
            .padding(.vertical, 8)
            .background(Color.orange)
            .cornerRadius(8)
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
}

struct OrdersView: View {
    @State private var orders: [Order] = [
        Order(
            id: "ord_001",
            symbol: "USDTg/USDT",
            side: .buy,
            quantity: 500,
            price: 0.9990,
            type: .limit,
            status: "Pending"
        ),
        Order(
            id: "ord_002",
            symbol: "USDTgG/USDT",
            side: .sell,
            quantity: 100,
            price: 5.1000,
            type: .limit,
            status: "Pending"
        )
    ]
    
    var body: some View {
        ScrollView {
            LazyVStack(spacing: 12) {
                ForEach(orders) { order in
                    OrderCard(order: order) {
                        cancelOrder(order.id)
                    }
                }
            }
            .padding()
        }
    }
    
    private func cancelOrder(_ orderId: String) {
        orders.removeAll { $0.id == orderId }
    }
}

struct OrderCard: View {
    let order: Order
    let onCancel: () -> Void
    
    var body: some View {
        VStack(spacing: 12) {
            HStack {
                Text(order.symbol)
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text(order.side.rawValue)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(.white)
                    .padding(.horizontal, 8)
                    .padding(.vertical, 4)
                    .background(order.side == .buy ? Color.green : Color.red)
                    .cornerRadius(6)
            }
            
            HStack {
                VStack(alignment: .leading) {
                    Text("Quantity")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("\(order.quantity)")
                        .font(.subheadline)
                        .fontWeight(.medium)
                }
                
                Spacer()
                
                VStack(alignment: .center) {
                    Text("Price")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("$\(order.price, specifier: "%.6f")")
                        .font(.subheadline)
                        .fontWeight(.medium)
                }
                
                Spacer()
                
                VStack(alignment: .trailing) {
                    Text("Type")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text(order.type.rawValue)
                        .font(.subheadline)
                        .fontWeight(.medium)
                }
            }
            
            HStack {
                Text("Status: \(order.status)")
                    .font(.subheadline)
                    .foregroundColor(.secondary)
                
                Spacer()
                
                Button("Cancel") {
                    onCancel()
                }
                .font(.subheadline)
                .foregroundColor(.white)
                .padding(.horizontal, 16)
                .padding(.vertical, 8)
                .background(Color.red)
                .cornerRadius(8)
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
}

struct HistoryView: View {
    @State private var history: [TradeHistory] = [
        TradeHistory(
            id: "hist_001",
            symbol: "USDTg/USDT",
            side: .buy,
            quantity: 1000,
            price: 0.9995,
            timestamp: Date().addingTimeInterval(-3600),
            pnl: 1.00
        ),
        TradeHistory(
            id: "hist_002",
            symbol: "USDTgV/USDT",
            side: .sell,
            quantity: 500,
            price: 0.5020,
            timestamp: Date().addingTimeInterval(-7200),
            pnl: 2.00
        )
    ]
    
    var body: some View {
        ScrollView {
            LazyVStack(spacing: 12) {
                ForEach(history) { trade in
                    HistoryCard(trade: trade)
                }
            }
            .padding()
        }
    }
}

struct HistoryCard: View {
    let trade: TradeHistory
    
    var body: some View {
        VStack(spacing: 12) {
            HStack {
                Text(trade.symbol)
                    .font(.headline)
                    .fontWeight(.semibold)
                
                Spacer()
                
                Text(trade.side.rawValue)
                    .font(.subheadline)
                    .fontWeight(.medium)
                    .foregroundColor(.white)
                    .padding(.horizontal, 8)
                    .padding(.vertical, 4)
                    .background(trade.side == .buy ? Color.green : Color.red)
                    .cornerRadius(6)
            }
            
            HStack {
                VStack(alignment: .leading) {
                    Text("Quantity")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("\(trade.quantity)")
                        .font(.subheadline)
                        .fontWeight(.medium)
                }
                
                Spacer()
                
                VStack(alignment: .center) {
                    Text("Price")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("$\(trade.price, specifier: "%.6f")")
                        .font(.subheadline)
                        .fontWeight(.medium)
                }
                
                Spacer()
                
                VStack(alignment: .trailing) {
                    Text("PnL")
                        .font(.caption)
                        .foregroundColor(.secondary)
                    Text("$\(trade.pnl, specifier: "%.2f")")
                        .font(.subheadline)
                        .fontWeight(.semibold)
                        .foregroundColor(trade.pnl >= 0 ? .green : .red)
                }
            }
            
            HStack {
                Text(trade.timestamp, style: .relative)
                    .font(.caption)
                    .foregroundColor(.secondary)
                
                Spacer()
            }
        }
        .padding()
        .background(Color(.systemGray6))
        .cornerRadius(12)
    }
}

// MARK: - Data Models
struct Position: Identifiable {
    let id: String
    let symbol: String
    let side: OrderSide
    let quantity: Double
    let entryPrice: Double
    let currentPrice: Double
    let leverage: Int
    let pnl: Double
}

struct Order: Identifiable {
    let id: String
    let symbol: String
    let side: OrderSide
    let quantity: Double
    let price: Double
    let type: OrderType
    let status: String
}

struct TradeHistory: Identifiable {
    let id: String
    let symbol: String
    let side: OrderSide
    let quantity: Double
    let price: Double
    let timestamp: Date
    let pnl: Double
}

struct TradingView_Previews: PreviewProvider {
    static var previews: some View {
        TradingView()
    }
}
