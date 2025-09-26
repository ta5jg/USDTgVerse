/*
==============================================
 File:        order_book.hpp
 Author:      Irfan Gedik
 Created:     19.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse CEX Order Book
   
   Centralized exchange order book featuring:
   - Order matching engine
   - Bid/ask price management
   - Trade execution algorithms
   - Market depth analysis
   - Order book visualization

 License:
   MIT License
==============================================
*/

#pragma once

#include "../../src/common/types.hpp"
#include <map>
#include <queue>
#include <unordered_map>
#include <chrono>

namespace usdtgverse {
namespace cex {

// Order Types
enum class OrderType : uint8_t {
    MARKET = 0,
    LIMIT = 1,
    STOP_LOSS = 2,
    TAKE_PROFIT = 3,
    STOP_LIMIT = 4,
    ICEBERG = 5
};

enum class OrderSide : uint8_t {
    BUY = 0,
    SELL = 1
};

enum class OrderStatus : uint8_t {
    PENDING = 0,
    PARTIALLY_FILLED = 1,
    FILLED = 2,
    CANCELLED = 3,
    REJECTED = 4,
    EXPIRED = 5
};

// Trading Order
struct Order {
    Hash order_id;
    Address user_id;
    std::string symbol; // e.g., "USDTG/USD"
    OrderType type;
    OrderSide side;
    USDTgAmount quantity;
    USDTgAmount price;        // 0 for market orders
    USDTgAmount filled_qty;
    USDTgAmount remaining_qty;
    OrderStatus status;
    Timestamp created_at;
    Timestamp updated_at;
    Timestamp expires_at;     // 0 for GTC (Good Till Cancelled)
    
    // Advanced order parameters
    USDTgAmount stop_price;   // For stop orders
    USDTgAmount iceberg_qty;  // For iceberg orders
    bool post_only;           // Maker-only flag
    bool reduce_only;         // Position reduction only
    
    bool is_buy() const { return side == OrderSide::BUY; }
    bool is_sell() const { return side == OrderSide::SELL; }
    bool is_active() const { 
        return status == OrderStatus::PENDING || status == OrderStatus::PARTIALLY_FILLED; 
    }
};

// Trade Execution Result
struct Trade {
    Hash trade_id;
    Hash maker_order_id;
    Hash taker_order_id;
    Address maker_user_id;
    Address taker_user_id;
    std::string symbol;
    USDTgAmount quantity;
    USDTgAmount price;
    USDTgAmount maker_fee;
    USDTgAmount taker_fee;
    Timestamp timestamp;
    OrderSide taker_side;
};

// Price Level in Order Book
struct PriceLevel {
    USDTgAmount price;
    USDTgAmount total_quantity;
    uint32_t order_count;
    std::queue<Hash> order_ids; // FIFO for price-time priority
};

// High-Performance Order Book
class OrderBook {
private:
    std::string symbol_;
    
    // Price levels - using map for automatic sorting
    std::map<USDTgAmount, PriceLevel, std::greater<USDTgAmount>> bids_; // Descending
    std::map<USDTgAmount, PriceLevel> asks_;                            // Ascending
    
    // Order storage
    std::unordered_map<Hash, Order> orders_;
    
    // User order tracking
    std::unordered_map<Address, std::vector<Hash>> user_orders_;
    
    // Market data
    USDTgAmount last_trade_price_;
    USDTgAmount best_bid_;
    USDTgAmount best_ask_;
    USDTgAmount daily_volume_;
    USDTgAmount daily_high_;
    USDTgAmount daily_low_;
    
public:
    explicit OrderBook(const std::string& symbol);
    
    // Order Management
    Hash place_order(const Order& order);
    bool cancel_order(Hash order_id, Address user_id);
    bool modify_order(Hash order_id, USDTgAmount new_qty, USDTgAmount new_price);
    
    // Order Matching
    std::vector<Trade> match_order(const Order& incoming_order);
    
    // Market Data
    struct BookSnapshot {
        std::vector<std::pair<USDTgAmount, USDTgAmount>> bids; // price, quantity
        std::vector<std::pair<USDTgAmount, USDTgAmount>> asks;
        Timestamp timestamp;
    };
    
    BookSnapshot get_snapshot(size_t depth = 20) const;
    USDTgAmount get_best_bid() const { return best_bid_; }
    USDTgAmount get_best_ask() const { return best_ask_; }
    USDTgAmount get_spread() const { return best_ask_ - best_bid_; }
    USDTgAmount get_mid_price() const { return (best_bid_ + best_ask_) / 2; }
    
    // Statistics
    USDTgAmount get_daily_volume() const { return daily_volume_; }
    USDTgAmount get_daily_high() const { return daily_high_; }
    USDTgAmount get_daily_low() const { return daily_low_; }
    
    // Order Information
    const Order* get_order(Hash order_id) const;
    std::vector<Order> get_user_orders(Address user_id) const;
    
private:
    void add_order_to_book(const Order& order);
    void remove_order_from_book(Hash order_id);
    std::vector<Trade> match_against_book(const Order& order);
    void update_market_data(const Trade& trade);
    void cleanup_empty_levels();
};

// Multi-Symbol Trading Engine
class TradingEngine {
private:
    std::unordered_map<std::string, std::unique_ptr<OrderBook>> order_books_;
    std::vector<Trade> recent_trades_;
    
    // Risk Management
    struct RiskLimits {
        USDTgAmount max_position_size;
        USDTgAmount max_daily_loss;
        USDTgAmount max_order_value;
        uint32_t max_orders_per_second;
    };
    
    std::unordered_map<Address, RiskLimits> user_risk_limits_;
    
public:
    // Order Book Management
    bool create_market(const std::string& symbol);
    OrderBook* get_order_book(const std::string& symbol);
    
    // Order Processing
    Hash place_order(const Order& order);
    bool cancel_order(const std::string& symbol, Hash order_id, Address user_id);
    std::vector<Order> get_user_orders(Address user_id, const std::string& symbol = "");
    
    // Market Data
    std::vector<std::string> get_active_symbols() const;
    OrderBook::BookSnapshot get_market_snapshot(const std::string& symbol, size_t depth = 20) const;
    std::vector<Trade> get_recent_trades(const std::string& symbol, size_t limit = 100) const;
    
    // Risk Management
    bool set_risk_limits(Address user_id, const RiskLimits& limits);
    bool validate_order_risk(const Order& order) const;
    
    // Statistics
    struct MarketStats {
        std::string symbol;
        USDTgAmount last_price;
        USDTgAmount daily_volume;
        USDTgAmount daily_change;
        USDTgAmount daily_high;
        USDTgAmount daily_low;
        USDTgAmount best_bid;
        USDTgAmount best_ask;
    };
    
    std::vector<MarketStats> get_market_stats() const;
    
private:
    bool validate_order(const Order& order) const;
    void broadcast_trade(const Trade& trade);
    void update_user_positions(const Trade& trade);
};

// Market Making Bot Interface
class MarketMaker {
private:
    TradingEngine& engine_;
    std::string symbol_;
    USDTgAmount spread_bps_;
    USDTgAmount max_position_;
    
public:
    MarketMaker(TradingEngine& engine, const std::string& symbol, 
                USDTgAmount spread_bps, USDTgAmount max_position)
        : engine_(engine), symbol_(symbol), spread_bps_(spread_bps), max_position_(max_position) {}
    
    void start_making();
    void stop_making();
    void update_quotes(USDTgAmount fair_price);
    
private:
    void place_bid_ask_orders(USDTgAmount fair_price);
    void cancel_existing_orders();
};

} // namespace cex
} // namespace usdtgverse
