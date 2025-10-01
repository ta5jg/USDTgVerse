/*
==============================================
 File:        trading_engine.h
 Author:      Irfan Gedik
 Created:     02.10.2025
 Last Update: 02.10.2025
 Version:     1.0

 Description:
   USDTgVerse Trading Engine Header
   
   Quantum-safe leveraged trading system with:
   - Post-quantum cryptography
   - Real-time price feeds
   - Risk management
   - Leverage trading (up to 100x)
   - Mobile trading interface

 License:
   MIT License
==============================================
*/

#ifndef TRADING_ENGINE_H
#define TRADING_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Trading constants
#define TRADING_MAX_LEVERAGE 100
#define TRADING_MIN_LEVERAGE 1
#define TRADING_MAX_POSITION_SIZE 1000000.0
#define TRADING_MIN_POSITION_SIZE 0.01
#define TRADING_MAX_SYMBOL_LENGTH 16
#define TRADING_MAX_ORDER_ID_LENGTH 64

// Post-quantum cryptography
#define PQ_SIGNATURE_SIZE 64
#define PQ_HASH_SIZE 32
#define PQ_KEY_SIZE 32

// Order types
typedef enum {
    TRADING_ORDER_MARKET = 0,
    TRADING_ORDER_LIMIT = 1,
    TRADING_ORDER_STOP = 2,
    TRADING_ORDER_STOP_LIMIT = 3,
    TRADING_ORDER_TRAILING_STOP = 4
} trading_order_type_t;

// Order side
typedef enum {
    TRADING_SIDE_BUY = 0,
    TRADING_SIDE_SELL = 1
} trading_side_t;

// Order status
typedef enum {
    TRADING_STATUS_PENDING = 0,
    TRADING_STATUS_PARTIALLY_FILLED = 1,
    TRADING_STATUS_FILLED = 2,
    TRADING_STATUS_CANCELLED = 3,
    TRADING_STATUS_REJECTED = 4,
    TRADING_STATUS_EXPIRED = 5
} trading_status_t;

// Position status
typedef enum {
    TRADING_POSITION_OPEN = 0,
    TRADING_POSITION_CLOSED = 1,
    TRADING_POSITION_LIQUIDATED = 2
} trading_position_status_t;

// Order structure
typedef struct {
    char order_id[TRADING_MAX_ORDER_ID_LENGTH];
    char user_id[32];
    char symbol[TRADING_MAX_SYMBOL_LENGTH];
    trading_order_type_t order_type;
    trading_side_t side;
    double quantity;
    double price;
    double stop_price;
    double leverage;
    trading_status_t status;
    time_t created_at;
    time_t updated_at;
    char metadata[512];
} trading_order_t;

// Position structure
typedef struct {
    char position_id[64];
    char user_id[32];
    char symbol[TRADING_MAX_SYMBOL_LENGTH];
    trading_side_t side;
    double quantity;
    double entry_price;
    double current_price;
    double leverage;
    double pnl;
    double margin;
    trading_position_status_t status;
    time_t opened_at;
    time_t closed_at;
    char metadata[512];
} trading_position_t;

// Market data structure
typedef struct {
    char symbol[TRADING_MAX_SYMBOL_LENGTH];
    double bid_price;
    double ask_price;
    double last_price;
    double volume_24h;
    double change_24h;
    double high_24h;
    double low_24h;
    time_t timestamp;
} trading_market_data_t;

// API response structure
typedef struct {
    bool success;
    int error_code;
    char error_message[256];
    char data[1024];
} trading_response_t;

// Configuration structure
typedef struct {
    char api_key[64];
    char base_url[256];
    bool sandbox_mode;
    int timeout_seconds;
    bool quantum_safe_enabled;
    double max_leverage;
    double margin_requirement;
    double liquidation_threshold;
} trading_config_t;

// Function prototypes

// Initialization
bool trading_engine_init(const trading_config_t* config);
void trading_engine_cleanup(void);

// Order management
trading_response_t* trading_create_order(const trading_order_t* order);
trading_response_t* trading_get_order(const char* order_id);
trading_response_t* trading_cancel_order(const char* order_id);
trading_response_t* trading_get_user_orders(const char* user_id, int limit, int offset);

// Position management
trading_response_t* trading_get_position(const char* position_id);
trading_response_t* trading_get_user_positions(const char* user_id);
trading_response_t* trading_close_position(const char* position_id);
trading_response_t* trading_update_position_margin(const char* position_id, double margin);

// Market data
trading_response_t* trading_get_market_data(const char* symbol);
trading_response_t* trading_get_all_market_data(void);
trading_response_t* trading_subscribe_to_updates(const char* symbol);

// Risk management
trading_response_t* trading_check_margin_requirement(const char* user_id, double amount, double leverage);
trading_response_t* trading_calculate_liquidation_price(const char* position_id);
trading_response_t* trading_force_liquidate_position(const char* position_id);

// Account management
trading_response_t* trading_get_account_balance(const char* user_id);
trading_response_t* trading_get_account_positions(const char* user_id);
trading_response_t* trading_get_account_history(const char* user_id, int limit, int offset);

// Mobile trading
trading_response_t* trading_mobile_create_order(const char* user_id, const char* symbol, 
                                               trading_side_t side, double quantity, 
                                               double leverage, trading_order_type_t type);
trading_response_t* trading_mobile_get_positions(const char* user_id);
trading_response_t* trading_mobile_close_position(const char* position_id);

// Utility functions
char* trading_generate_order_id(void);
bool trading_validate_symbol(const char* symbol);
bool trading_validate_quantity(double quantity);
bool trading_validate_leverage(double leverage);
bool trading_validate_price(double price);

// Quantum-safe functions
bool trading_pq_generate_keypair(uint8_t* public_key, uint8_t* private_key);
bool trading_pq_sign(const uint8_t* private_key, const char* message, uint8_t* signature);
bool trading_pq_verify(const uint8_t* public_key, const char* message, const uint8_t* signature);
bool trading_pq_hash(const char* data, size_t data_len, uint8_t* hash);

// Error handling
const char* trading_get_error_message(int error_code);
void trading_free_response(trading_response_t* response);

#endif // TRADING_ENGINE_H
