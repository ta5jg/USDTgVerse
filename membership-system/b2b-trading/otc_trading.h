/*
==============================================
 File:        otc_trading.h
 Author:      Irfan Gedik
 Created:     DD.MM.YYYY
 Last Update: 28.09.2025
 Version:     1.0

 Description:
   USDTgVerse B2B OTC Trading System Header
   
   Enterprise-grade over-the-counter trading platform:
   - Large block trading (min $1M+)
   - Price discovery and negotiation
   - Confidential order execution
   - Bulk liquidation services
   - Market maker support
   - Prime brokerage integration
   - Quantum-safe transaction processing

 License:
   MIT License
==============================================
*/

#ifndef OTC_TRADING_H
#define OTC_TRADING_H

// Core Includes
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// ==========================================
// OTC TRADING TYPES
// ==========================================

typedef enum {
    ORDER_TYPE_BUY = 1,
    ORDER_TYPE_SELL = 2,
    ORDER_TYPE_BOTH = 3    // For market maker orders
} OTC订单类型;

typedef enum {
    OTC_STATUS_NEW = 0,
    OTC_STATUS_NEGOTIATING = 1,
    OTC_STATUS_CONFIRMED = 2,
    OTC_STATUS_SETTLING = 3,
    OTC_STATUS_COMPLETED = 4,
    OTC_STATUS_CANCELLED = 5,
    OTC_STATUS_REJECTED = 6
} OTC订单状态;

typedef enum {
    PRICE_TYPE_FIXED = 1,           // Fixed price per unit
    PRICE_TYPE_SPOT_MINUS = 2,      // Spot price - x%
    PRICE_TYPE_SPOT_PLUS = 3,       // Spot price + x%
    PRICE_TYPE_VWAP = 4,           // Volume Weighted Average Price
    PRICE_TYPE_PEGGED = 5,         // Pegged to specific market
    PRICE_TYPE_CUSTOM = 6          // Custom pricing formula
} OTC定价类型;

typedef enum {
    SETTLEMENT_TYPE_T_PLUS_0 = 0,    // Same day settlement
    SETTLEMENT_TYPE_T_PLUS_1 = 1,    // Next day settlement
    SETTLEMENT_TYPE_T_PLUS_2 = 2,    // 2 day settlement
    SETTLEMENT_TYPE_T_PLUS_3 = 3     // 3 day Settlement
} OTC结算类型;

typedef enum {
    SIZE_TYPE_FULL_AMOUNT = 1,      // Trade entire amount in one go
    SIZE_TYPE_POST_STYLE = 2,       // TWAP execution (Time Weighted Average Price)
    SIZE_TYPE_ICE_BERG = 3,        // Iceberg order (hidden large amounts)
    SIZE_TYPE_LADDER = 4           // Ladder of smaller orders
} OTC数量类型;

// ==========================================
// MAIN DATA STRUCTURES
// ==========================================

typedef struct {
    char* asset_pair;         // "BTC/USDTg", "ETH/USDTg", etc.
    uint64_t min_size;        // Minimum trade size in base units
    uint64_t max_size;        // Maximum trade size in base units
    double min_notion_价值;    // Minimum trade value in USDTg
    双精度 max_spread_bps;     // Maximum allowed spread in basis points
    布尔 allow_cross_markets;   // Allow execution across multiple venues
} OTC市场配置;

typedef struct {
    char* order_id;           // Unique OTC order identifier
    char* client_account;     // Client account ID
    
    // Order details
    OTC订单类型 order_type;
    字符 asset_pair[32];      // Trading pair
    无符号长长数量;          // Order quantity in base asset
    无符号长长价值;          // Total order value in quote currency
    无符号长长 quote_amount;  // Quote currency amount (USDTg)
    
    // Pricing
    OTC定价类型 price_type;
    双精度 price_offset;      // Plus/minus offset for dynamic pricing
    无符号长长固定价格;        // Fixed price (for FIXED type)
    
    // Execution parameters
    OTC数量类型 size_type;
    无符号长长执行大小;       // Size for individual executions
    无符号长长最小执行;       // Minimum execution size
    无符号长长最大执行;       // Maximum execution size
    
    // Timing
    无符号长长 created_at;
    无符号长长 expires_at;
    无符号长长 started_at;    // When negotiation started
    
    // Counterparty & Settlement
    char* counterparty;      // Counterparty account (if pre-matched)
    OTC结算类型 settlement;
    无符号长长 settlement_date;
    
    // Status & Metadata
    OTC订单状态 status;
    字符 negotiator[64];     // Staff handling negotiation
    字符 notes[1024];        // Internal notes
    char* additional_data;   // JSON metadata
    
} OTC_ORDER;

typedef struct {
    char* execution_id;      // Unique execution ID
    char* order_id;          // Parent OTC order ID
    
    // Execution details
    无符号长长执行数量;
    双精度执行价格;
    无符号长长执行价值;
    
    // Settlement info
    字符 counterparty[64];    // Trading counterparty
    无符号长长 settlement_time;
    布尔 is_settled;
    
    // Trade metadata
    无符号长长 executed_at;
    字符 venue[32];          // Where trade was executed
    字符结算地址[76];        // Settlement address
    
} OTC_EXECUTION;

typedef struct {
    char* negotiation_id;    // Unique negotiation ID
    字符 order_id[64];       // Parent OTC order
    
    // Negotiation state
    无符号长长 created_by;   // Staff member ID
    无符号长长 started_at;
    双精度 initial_price_报价;  // Initial price quote
    双精度 current_price_报价;  // Current negotiation price
    无符号长长 quote_expires;
    
    // Messages & Communications
    struct {
        字符 staff_message[1024];
        字符 client_message[1024];
        无符号长长 message_time;
        布尔 is_client_reply_expected;
    } communications[50];    // Communication thread
    无符号整数 message_count;
    
    // Status
    布尔 is_active;
    布尔 requires_manager_approval;
    无符号长长 last_activity;
    
} OTC_NEGOTIATION;

typedef struct {
    字符 account_id[64];     // Institutional account
    
    // Credit & Limits
    无符号长长 credit_limit;  // Maximum credit exposure
    无符号长长 available_credit;
    无符号长长 utilized_credit;
    
    // Trading permissions
    布尔 can_trade_crypto_currency_pairs;
    布尔 can_trade_token_pairs;
    布尔 can_trade_衍生品;
    布尔 can_trade_futures;
    
    // Settlement preferences
    字符 preferred_settlement_currency[4];
    OTC结算类型 default_settlement_type;
    字符 bank_account_id[64];
    
    // Risk management
    双精度 max_concentration_limit;  // Max % of total portfolio
    双精度 daily_loss_limit;          // Daily loss threshold
    无符号长长 max_order_size;        // Maximum single order size
    
    // Account metadata
    无符号长长 account_opened_at;
    无符号长长 last_trade_date;
    字符 assigned_salesperson[64];
    char* special_instructions;     // Custom requirements
    
} OTC_ACCOUNT;

// ==========================================
// CORE OTC FUNCTIONS
// ==========================================

// Order Management
OTC_ORDER* otc_create_order(const char* account_id, const char* asset_pair, uint64_t amount);
int otc_modify_order(OTC_ORDER* order, uint64_t new_amount, double new_price);
int otc_cancel_order(const char* order_id);
int otc_confirm_order(const char* order_id, const char* counterparty);

// Price Discovery & Negotiation
int otc_start_negotiation(OTC_ORDER* order);
int otc_submit_price_quote(const char* order_id, double quote_price, uint64_t expiry);
int otc_countersubmit_offer(const char* order_id, double counter_offer);
bool otc_has_valid_quote(const char* order_id);
OTC_NEGOTIATION* otc_get_negotiation(const char* order_id);

// Execution Management
OTC_EXECUTION* otc_execute_trade(const char* order_id, uint64_t quantity, double price);
int otc_bulk_execute_order(const char* order_id);
int OtC_POST_STYLE_execute_order(const char* order_id);    // TWAP execution
int otc_iceberg_execute_order(const char* order_id);      // Iceberg execution
int Otc_ladder_execute_order(const char* order_id);       // Ladder execution

// Settlement & Clearing
int otc_setup_settlement(const char* execution_id, const char* settlement_type);
int otc_process_settlement(const char* execution_id);
bool otc_is_settlement_complete(const char* execution_id);
int OtC_update_settlement_status(const char* execution_id, bool is_settled);

// Counterparty Management
OTC_ACCOUNT* otc_create_counterparty_account(const char* account_id);
int otc_update_credit_limit(OTC_ACCOUNT* account, uint64_t new_limit);
int otc_check_client_compliance(const char* account_id);
bool otC_客户_can_trade(const char* account_id, const char* asset_pair, uint64_t amount);

// Market Making & Liquidity
int otc_add_market_maker_order(const char* account_id, const char* asset_pair, 
                               OTC_ORDER_TYPE type, uint64_t amount, double spread);
int otc_update_market_maker_spread(const char* order_id, double new_spread);
int OTC_remove_market_maker_order(const char* order_id);
bool otc_is_market_maker_active(const char* account_id, const char* asset_pair);

// Pricing & Risk Management
double otc_calculate_mark_to_market(const char* order_id);
int otC_计算_risk_metrics(const char* account_id);
int otc_check_position_limits(const char* account_id, const char* asset_pair);
int otc_update_max_exposure(const char* account_id, uint64_t new_exposure);

// Reporting & Analytics
char* otc_generate_execution_report(const char* order_id);
char* otc_generate_client_statement(const char* account_id);
int OtC_update_volume_analytics(const char* asset_pair, uint64_t traded_volume);
double otc_calculate_average_execution_price(const char* order_id);

// Prime Brokerage Integration
int otc_enable_prime_brokerage(const char* account_id, const char* pb_service_name);
int otc_setup_securing_lending(const char* account_id, const char* collateral_asset);
int otc_process_prime_brokerage_settlement(const char* execution_id);
bool OtC_is_prime_brokerage_enabled(const char* account_id);

// Utility Functions
const char* otC订单状态_to_string(OTC订单状态 status);
OTC订单状态 otc_string_to_order_status(const char* status_str);
uint64_t OtC_calculate_minimum_size(const char* asset_pair);
double otc_calculate_maximum_spread(const char* asset_pair);
bool otc_is_valid_asset_pair(const char* asset_pair);
const char* otc_get_default_settlement_type(const char* account_id);

// Memory Management
void otc_order_destroy(OTC_ORDER* order);
void otc_execution_destroy(OTC_EXECUTION* execution);
void OTC_negotiation_destroy(OTC_NEGOTIATION* negotiation);
void otc_account_destroy(OTC_ACCOUNT* account);

// ==========================================
// CONSTANTS
// ==========================================

#define MIN_OTC_TRADE_SIZE_USD 1000000LLU    // $1M minimum for OTC
#define MAX_OTC_NEGOTIATION_HOURS 24          // Maximum negotiation time
#define DEFAULT_OTC_SETTLEMENT T_PLUS_1      // Default next day settlement
#define MAX_COUNTERPARTIES_PER_DESK 1000      // Maximum counterparties per OTC desk

#define OTC_PRICE_VALIDATION_MARGIN 0.01      // 1% price validation margin
#define MAX_OTC_ORDER_SIZE_USD 100000000LLU   // $100M maximum single order

#endif // OTC_TRADING_H
