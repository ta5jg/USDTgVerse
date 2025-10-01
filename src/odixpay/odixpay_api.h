/*
==============================================
 File:        odixpay_api.h
 Author:      Irfan Gedik
 Created:     02.10.2025
 Last Update: 02.10.2025
 Version:     1.0

 Description:
   ODIX PAY Integration API Header
   
   Quantum-safe payment processing system with:
   - Post-quantum cryptography
   - Secure payment gateway
   - Real-time transaction processing
   - Mobile wallet integration

 License:
   MIT License
==============================================
*/

#ifndef ODIXPAY_API_H
#define ODIXPAY_API_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Quantum-safe constants
#define ODIXPAY_MAX_AMOUNT 1000000.0
#define ODIXPAY_MIN_AMOUNT 0.01
#define ODIXPAY_MAX_DESCRIPTION 256
#define ODIXPAY_API_KEY_LENGTH 64
#define ODIXPAY_WEBHOOK_SECRET_LENGTH 32

// Post-quantum cryptography
#define PQ_SIGNATURE_SIZE 64
#define PQ_HASH_SIZE 32
#define PQ_KEY_SIZE 32

// Transaction status
typedef enum {
    ODIXPAY_STATUS_PENDING = 0,
    ODIXPAY_STATUS_PROCESSING = 1,
    ODIXPAY_STATUS_COMPLETED = 2,
    ODIXPAY_STATUS_FAILED = 3,
    ODIXPAY_STATUS_CANCELLED = 4,
    ODIXPAY_STATUS_REFUNDED = 5
} odixpay_status_t;

// Payment method
typedef enum {
    ODIXPAY_METHOD_USDTG = 0,
    ODIXPAY_METHOD_USDTGV = 1,
    ODIXPAY_METHOD_USDTGG = 2,
    ODIXPAY_METHOD_CREDIT_CARD = 3,
    ODIXPAY_METHOD_BANK_TRANSFER = 4
} odixpay_method_t;

// Transaction structure
typedef struct {
    char transaction_id[64];
    char merchant_id[32];
    char customer_id[32];
    double amount;
    char currency[8];
    odixpay_method_t payment_method;
    odixpay_status_t status;
    time_t created_at;
    time_t updated_at;
    char description[ODIXPAY_MAX_DESCRIPTION];
    char webhook_url[256];
    char callback_url[256];
    char metadata[512];
} odixpay_transaction_t;

// API response structure
typedef struct {
    bool success;
    int error_code;
    char error_message[256];
    char data[1024];
} odixpay_response_t;

// Configuration structure
typedef struct {
    char api_key[ODIXPAY_API_KEY_LENGTH];
    char webhook_secret[ODIXPAY_WEBHOOK_SECRET_LENGTH];
    char base_url[256];
    bool sandbox_mode;
    int timeout_seconds;
    bool quantum_safe_enabled;
} odixpay_config_t;

// Function prototypes

// Initialization
bool odixpay_init(const odixpay_config_t* config);
void odixpay_cleanup(void);

// Transaction management
odixpay_response_t* odixpay_create_transaction(const odixpay_transaction_t* transaction);
odixpay_response_t* odixpay_get_transaction(const char* transaction_id);
odixpay_response_t* odixpay_update_transaction(const char* transaction_id, odixpay_status_t status);
odixpay_response_t* odixpay_cancel_transaction(const char* transaction_id);
odixpay_response_t* odixpay_refund_transaction(const char* transaction_id, double amount);

// Payment processing
odixpay_response_t* odixpay_process_payment(const char* transaction_id, const char* payment_data);
odixpay_response_t* odixpay_verify_payment(const char* transaction_id);
odixpay_response_t* odixpay_webhook_verify(const char* payload, const char* signature);

// Utility functions
char* odixpay_generate_transaction_id(void);
bool odixpay_validate_amount(double amount);
bool odixpay_validate_currency(const char* currency);
char* odixpay_sign_data(const char* data, size_t data_len);
bool odixpay_verify_signature(const char* data, size_t data_len, const char* signature);

// Quantum-safe functions
bool odixpay_pq_generate_keypair(uint8_t* public_key, uint8_t* private_key);
bool odixpay_pq_sign(const uint8_t* private_key, const char* message, uint8_t* signature);
bool odixpay_pq_verify(const uint8_t* public_key, const char* message, const uint8_t* signature);
bool odixpay_pq_hash(const char* data, size_t data_len, uint8_t* hash);

// Mobile wallet integration
odixpay_response_t* odixpay_mobile_payment(const char* wallet_address, double amount, const char* currency);
odixpay_response_t* odixpay_mobile_balance(const char* wallet_address);
odixpay_response_t* odixpay_mobile_transfer(const char* from_wallet, const char* to_wallet, double amount);

// Error handling
const char* odixpay_get_error_message(int error_code);
void odixpay_free_response(odixpay_response_t* response);

#endif // ODIXPAY_API_H
