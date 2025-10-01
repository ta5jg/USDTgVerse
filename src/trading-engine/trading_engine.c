/*
==============================================
 File:        trading_engine.c
 Author:      Irfan Gedik
 Created:     02.10.2025
 Last Update: 02.10.2025
 Version:     1.0

 Description:
   USDTgVerse Trading Engine Implementation
   
   Quantum-safe leveraged trading system

 License:
   MIT License
==============================================
*/

#include "trading_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

// Global configuration
static trading_config_t g_config;
static bool g_initialized = false;

// Quantum-safe hash function
bool trading_pq_hash(const char* data, size_t data_len, uint8_t* hash) {
    if (!data || !hash) return false;
    
    SHA256_CTX sha256;
    if (!SHA256_Init(&sha256)) return false;
    if (!SHA256_Update(&sha256, data, data_len)) return false;
    if (!SHA256_Final(hash, &sha256)) return false;
    
    return true;
}

// Generate quantum-safe keypair
bool trading_pq_generate_keypair(uint8_t* public_key, uint8_t* private_key) {
    if (!public_key || !private_key) return false;
    
    // Generate random private key
    if (!RAND_bytes(private_key, PQ_KEY_SIZE)) return false;
    
    // For now, use the private key as public key (simplified)
    // In production, implement proper post-quantum key generation
    memcpy(public_key, private_key, PQ_KEY_SIZE);
    
    return true;
}

// Sign data with quantum-safe signature
bool trading_pq_sign(const uint8_t* private_key, const char* message, uint8_t* signature) {
    if (!private_key || !message || !signature) return false;
    
    // Generate hash of message
    uint8_t hash[PQ_HASH_SIZE];
    if (!trading_pq_hash(message, strlen(message), hash)) return false;
    
    // Simple signature (XOR with private key)
    // In production, implement proper post-quantum signature
    for (int i = 0; i < PQ_SIGNATURE_SIZE; i++) {
        signature[i] = hash[i % PQ_HASH_SIZE] ^ private_key[i % PQ_KEY_SIZE];
    }
    
    return true;
}

// Verify quantum-safe signature
bool trading_pq_verify(const uint8_t* public_key, const char* message, const uint8_t* signature) {
    if (!public_key || !message || !signature) return false;
    
    // Generate expected signature
    uint8_t expected_signature[PQ_SIGNATURE_SIZE];
    if (!trading_pq_sign(public_key, message, expected_signature)) return false;
    
    // Compare signatures
    return memcmp(signature, expected_signature, PQ_SIGNATURE_SIZE) == 0;
}

// Initialize trading engine
bool trading_engine_init(const trading_config_t* config) {
    if (!config) return false;
    
    memcpy(&g_config, config, sizeof(trading_config_t));
    g_initialized = true;
    
    return true;
}

// Cleanup trading engine
void trading_engine_cleanup(void) {
    g_initialized = false;
}

// Generate unique order ID
char* trading_generate_order_id(void) {
    static char order_id[TRADING_MAX_ORDER_ID_LENGTH];
    time_t now = time(NULL);
    uint8_t random_bytes[16];
    
    if (!RAND_bytes(random_bytes, 16)) {
        return NULL;
    }
    
    snprintf(order_id, sizeof(order_id), 
             "trade_%ld_%02x%02x%02x%02x%02x%02x%02x%02x",
             now,
             random_bytes[0], random_bytes[1], random_bytes[2], random_bytes[3],
             random_bytes[4], random_bytes[5], random_bytes[6], random_bytes[7]);
    
    return order_id;
}

// Validate symbol
bool trading_validate_symbol(const char* symbol) {
    if (!symbol) return false;
    
    const char* valid_symbols[] = {
        "USDTg/USDT", "USDTg/USDC", "USDTg/ETH", "USDTg/BTC",
        "USDTgV/USDT", "USDTgG/USDT", "USDTgV/USDTgG"
    };
    
    int num_symbols = sizeof(valid_symbols) / sizeof(valid_symbols[0]);
    
    for (int i = 0; i < num_symbols; i++) {
        if (strcmp(symbol, valid_symbols[i]) == 0) {
            return true;
        }
    }
    
    return false;
}

// Validate quantity
bool trading_validate_quantity(double quantity) {
    return quantity >= TRADING_MIN_POSITION_SIZE && quantity <= TRADING_MAX_POSITION_SIZE;
}

// Validate leverage
bool trading_validate_leverage(double leverage) {
    return leverage >= TRADING_MIN_LEVERAGE && leverage <= TRADING_MAX_LEVERAGE;
}

// Validate price
bool trading_validate_price(double price) {
    return price > 0.0 && price < 1000000.0;
}

// Create order
trading_response_t* trading_create_order(const trading_order_t* order) {
    if (!order || !g_initialized) return NULL;
    
    trading_response_t* response = malloc(sizeof(trading_response_t));
    if (!response) return NULL;
    
    // Validate order
    if (!trading_validate_symbol(order->symbol)) {
        response->success = false;
        response->error_code = 400;
        strcpy(response->error_message, "Invalid symbol");
        return response;
    }
    
    if (!trading_validate_quantity(order->quantity)) {
        response->success = false;
        response->error_code = 400;
        strcpy(response->error_message, "Invalid quantity");
        return response;
    }
    
    if (!trading_validate_leverage(order->leverage)) {
        response->success = false;
        response->error_code = 400;
        strcpy(response->error_message, "Invalid leverage");
        return response;
    }
    
    if (order->order_type == TRADING_ORDER_LIMIT && !trading_validate_price(order->price)) {
        response->success = false;
        response->error_code = 400;
        strcpy(response->error_message, "Invalid price");
        return response;
    }
    
    // Generate order ID if not provided
    if (strlen(order->order_id) == 0) {
        char* order_id = trading_generate_order_id();
        if (order_id) {
            strncpy((char*)order->order_id, order_id, sizeof(order->order_id) - 1);
        }
    }
    
    // Simulate successful order creation
    response->success = true;
    response->error_code = 0;
    strcpy(response->error_message, "");
    
    // Create JSON response
    snprintf(response->data, sizeof(response->data),
             "{\"order_id\":\"%s\",\"symbol\":\"%s\",\"side\":\"%s\",\"quantity\":%.6f,\"leverage\":%.1f,\"status\":\"pending\"}",
             order->order_id, order->symbol, 
             order->side == TRADING_SIDE_BUY ? "buy" : "sell",
             order->quantity, order->leverage);
    
    return response;
}

// Get market data
trading_response_t* trading_get_market_data(const char* symbol) {
    if (!symbol || !g_initialized) return NULL;
    
    trading_response_t* response = malloc(sizeof(trading_response_t));
    if (!response) return NULL;
    
    // Simulate market data
    response->success = true;
    response->error_code = 0;
    strcpy(response->error_message, "");
    
    // Generate realistic market data based on symbol
    double base_price = 1.0;
    if (strstr(symbol, "USDTgV")) base_price = 0.50;
    else if (strstr(symbol, "USDTgG")) base_price = 5.00;
    else if (strstr(symbol, "ETH")) base_price = 2000.0;
    else if (strstr(symbol, "BTC")) base_price = 45000.0;
    
    double bid_price = base_price * 0.999;
    double ask_price = base_price * 1.001;
    double volume_24h = 1000000.0;
    double change_24h = (rand() % 200 - 100) / 100.0; // -1% to +1%
    
    snprintf(response->data, sizeof(response->data),
             "{\"symbol\":\"%s\",\"bid_price\":%.6f,\"ask_price\":%.6f,\"last_price\":%.6f,\"volume_24h\":%.2f,\"change_24h\":%.2f}",
             symbol, bid_price, ask_price, base_price, volume_24h, change_24h);
    
    return response;
}

// Mobile create order
trading_response_t* trading_mobile_create_order(const char* user_id, const char* symbol, 
                                               trading_side_t side, double quantity, 
                                               double leverage, trading_order_type_t type) {
    if (!user_id || !symbol || !g_initialized) return NULL;
    
    trading_order_t order = {0};
    strncpy(order.user_id, user_id, sizeof(order.user_id) - 1);
    strncpy(order.symbol, symbol, sizeof(order.symbol) - 1);
    order.side = side;
    order.quantity = quantity;
    order.leverage = leverage;
    order.order_type = type;
    order.status = TRADING_STATUS_PENDING;
    order.created_at = time(NULL);
    
    return trading_create_order(&order);
}

// Get account balance
trading_response_t* trading_get_account_balance(const char* user_id) {
    if (!user_id || !g_initialized) return NULL;
    
    trading_response_t* response = malloc(sizeof(trading_response_t));
    if (!response) return NULL;
    
    // Simulate account balance
    response->success = true;
    response->error_code = 0;
    strcpy(response->error_message, "");
    
    snprintf(response->data, sizeof(response->data),
             "{\"user_id\":\"%s\",\"total_balance\":10000.00,\"available_balance\":8500.00,\"margin_used\":1500.00,\"unrealized_pnl\":250.00}",
             user_id);
    
    return response;
}

// Get error message
const char* trading_get_error_message(int error_code) {
    switch (error_code) {
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 500: return "Internal Server Error";
        default: return "Unknown Error";
    }
}

// Free response
void trading_free_response(trading_response_t* response) {
    if (response) {
        free(response);
    }
}
