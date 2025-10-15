/*
==============================================
 File:        payment_core.h
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Payment System Core
   
   Comprehensive payment processing system featuring:
   - Multiple payment gateway integration
   - Bank wire transfer support
   - Credit/debit card processing
   - PayPal integration
   - USDTgVerse PAY (OdixPay rebranded)
   - Direct IP-to-IP server payments
   - Quantum-safe transaction security
   - Real-time payment tracking

 License:
   MIT License
==============================================
*/

#ifndef USDTGVERSE_PAYMENT_CORE_H
#define USDTGVERSE_PAYMENT_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Payment Method Types
typedef enum {
    PAYMENT_METHOD_BANK_WIRE = 1,
    PAYMENT_METHOD_CREDIT_CARD = 2,
    PAYMENT_METHOD_DEBIT_CARD = 3,
    PAYMENT_METHOD_PAYPAL = 4,
    PAYMENT_METHOD_USDTGVERSE_PAY = 5,
    PAYMENT_METHOD_DIRECT_IP = 6,
    // Modern Instant Payment Systems
    PAYMENT_METHOD_FEDNOW = 7,         // US Federal Reserve FedNow
    PAYMENT_METHOD_RTP = 8,            // US Real-Time Payments
    PAYMENT_METHOD_PIX = 9,            // Brazil PIX
    PAYMENT_METHOD_UPI = 10,           // India UPI
    PAYMENT_METHOD_FASTER_PAYMENTS = 11, // UK Faster Payments
    PAYMENT_METHOD_SEPA_INSTANT = 12,  // EU SEPA Instant
    PAYMENT_METHOD_PAYNOW = 13,        // Singapore PayNow
    PAYMENT_METHOD_PROMPTPAY = 14,     // Thailand PromptPay
    PAYMENT_METHOD_FAST = 15,          // Australia FAST
    PAYMENT_METHOD_BIZUM = 16,         // Spain Bizum
    // Cryptocurrency payment methods
    PAYMENT_METHOD_USDT = 17,          // Tether
    PAYMENT_METHOD_ETH = 18,           // Ethereum
    PAYMENT_METHOD_BNB = 19,           // Binance Coin
    PAYMENT_METHOD_TRX = 20,           // TRON
    PAYMENT_METHOD_BTC = 21,           // Bitcoin
    PAYMENT_METHOD_USDC = 22,          // USD Coin
    PAYMENT_METHOD_USDTG = 23,         // USDTgVerse Native
    PAYMENT_METHOD_USDTGV = 24,        // USDTgVerse Utility
    PAYMENT_METHOD_USDTGG = 25,        // USDTgVerse Governance
    // Advanced Banking Systems 2025
    PAYMENT_METHOD_FEDNOW_INSTANT = 26, // US FedNow Instant
    PAYMENT_METHOD_CROSS_BORDER = 27,   // Cross-border payments
    PAYMENT_METHOD_CENTRAL_BANK = 28,   // Central Bank Digital Currency
    PAYMENT_METHOD_MOBILE_MONEY = 29,   // Mobile Money (M-Pesa, etc.)
    PAYMENT_METHOD_BNPL = 30,           // Buy Now Pay Later
    PAYMENT_METHOD_CRYPTO_FIAT = 31,    // Crypto-Fiat Bridge
    PAYMENT_METHOD_AI_PAYMENTS = 32,    // AI-Powered Payments
    PAYMENT_METHOD_QUANTUM_SAFE = 33,   // Quantum-Safe Payments
    PAYMENT_METHOD_OPEN_BANKING = 34,   // Open Banking API
    PAYMENT_METHOD_NEURAL_PAY = 35,     // Neural Network Payments
    PAYMENT_METHOD_OPENPAYD = 36,       // OpenPayd Global Payment Platform
    PAYMENT_METHOD_ODIX_PAY = 37        // ODIX PAY Advanced Payment System
} payment_method_t;

// Payment Status
typedef enum {
    PAYMENT_STATUS_PENDING = 1,
    PAYMENT_STATUS_PROCESSING = 2,
    PAYMENT_STATUS_COMPLETED = 3,
    PAYMENT_STATUS_FAILED = 4,
    PAYMENT_STATUS_CANCELLED = 5,
    PAYMENT_STATUS_REFUNDED = 6
} payment_status_t;

// Currency Types
typedef enum {
    CURRENCY_USD = 1,
    CURRENCY_EUR = 2,
    CURRENCY_GBP = 3,
    CURRENCY_TRY = 4,
    // USDTgVerse Native Currencies
    CURRENCY_USDTG = 5,
    CURRENCY_USDTGV = 6,
    CURRENCY_USDTGG = 7,
    // Major Cryptocurrencies
    CURRENCY_BTC = 8,
    CURRENCY_ETH = 9,
    CURRENCY_BNB = 10,
    CURRENCY_TRX = 11,
    CURRENCY_USDT = 12,
    CURRENCY_USDC = 13
} currency_t;

// Payment Transaction Structure
typedef struct {
    uint64_t transaction_id;
    char merchant_id[64];
    char customer_id[64];
    payment_method_t payment_method;
    currency_t currency;
    uint64_t amount; // Amount in smallest currency unit (cents, satoshis, etc.)
    char description[256];
    char reference_number[64];
    payment_status_t status;
    time_t created_at;
    time_t updated_at;
    time_t completed_at;
    char ip_address[45]; // IPv6 support
    char user_agent[512];
    char metadata[1024]; // JSON metadata
} payment_transaction_t;

// Payment Gateway Configuration
typedef struct {
    char gateway_name[64];
    char api_key[256];
    char secret_key[256];
    char webhook_url[256];
    char endpoint_url[256];
    bool is_enabled;
    bool is_test_mode;
    uint32_t timeout_seconds;
    char supported_currencies[256];
    char supported_methods[256];
} payment_gateway_config_t;

// Bank Account Information
typedef struct {
    char bank_name[128];
    char account_holder[128];
    char account_number[64];
    char routing_number[32];
    char swift_code[16];
    char iban[64];
    char bank_address[256];
    char country_code[4];
} bank_account_t;

// Credit Card Information
typedef struct {
    char card_number[32]; // Encrypted
    char card_holder[128];
    char expiry_month[4];
    char expiry_year[8];
    char cvv[8]; // Encrypted
    char card_type[32]; // Visa, MasterCard, Amex, etc.
} credit_card_t;

// PayPal Account Information
typedef struct {
    char paypal_email[128];
    char paypal_id[64];
    char access_token[512];
    char refresh_token[512];
    time_t token_expires_at;
} paypal_account_t;

// USDTgVerse PAY Account
typedef struct {
    char usdtgverse_pay_id[64];
    char wallet_address[128];
    char private_key[256]; // Encrypted
    char public_key[256];
    uint64_t balance;
    char qr_code[512];
} usdtgverse_pay_account_t;

// Direct IP Payment Configuration
typedef struct {
    char server_ip[45];
    uint16_t server_port;
    char server_key[256];
    char server_cert[512];
    bool use_ssl;
    uint32_t timeout_ms;
    char api_version[16];
} direct_ip_config_t;

// Payment System Core Functions
typedef struct {
    // Core Payment Processing
    int (*initialize)(void);
    int (*shutdown)(void);
    int (*process_payment)(payment_transaction_t *transaction);
    int (*refund_payment)(uint64_t transaction_id, uint64_t refund_amount);
    int (*cancel_payment)(uint64_t transaction_id);
    
    // Transaction Management
    int (*get_transaction)(uint64_t transaction_id, payment_transaction_t *transaction);
    int (*update_transaction_status)(uint64_t transaction_id, payment_status_t status);
    int (*list_transactions)(char *merchant_id, payment_transaction_t *transactions, int max_count);
    
    // Gateway Management
    int (*add_gateway)(payment_gateway_config_t *config);
    int (*remove_gateway)(char *gateway_name);
    int (*update_gateway)(payment_gateway_config_t *config);
    int (*list_gateways)(payment_gateway_config_t *gateways, int max_count);
    
    // Payment Method Specific Functions
    int (*process_bank_wire)(payment_transaction_t *transaction, bank_account_t *bank_info);
    int (*process_credit_card)(payment_transaction_t *transaction, credit_card_t *card_info);
    int (*process_paypal)(payment_transaction_t *transaction, paypal_account_t *paypal_info);
    int (*process_usdtgverse_pay)(payment_transaction_t *transaction, usdtgverse_pay_account_t *pay_info);
    int (*process_direct_ip)(payment_transaction_t *transaction, direct_ip_config_t *ip_config);
    
    // Security Functions
    int (*encrypt_sensitive_data)(char *data, char *encrypted_data, size_t data_size);
    int (*decrypt_sensitive_data)(char *encrypted_data, char *data, size_t data_size);
    int (*validate_payment)(payment_transaction_t *transaction);
    int (*generate_quantum_signature)(char *data, char *signature);
    int (*verify_quantum_signature)(char *data, char *signature);
    
    // Webhook Management
    int (*register_webhook)(char *gateway_name, char *webhook_url);
    int (*process_webhook)(char *gateway_name, char *webhook_data);
    int (*validate_webhook_signature)(char *gateway_name, char *webhook_data, char *signature);
    
    // Analytics and Reporting
    int (*get_payment_analytics)(char *merchant_id, time_t start_date, time_t end_date, char *analytics_json);
    int (*generate_payment_report)(char *merchant_id, time_t start_date, time_t end_date, char *report_path);
    int (*get_revenue_summary)(char *merchant_id, time_t start_date, time_t end_date, char *summary_json);
    
    // Error Handling
    int (*get_last_error)(char *error_message, size_t max_size);
    int (*get_error_code)(void);
    void (*clear_error)(void);
} payment_system_t;

// Global Payment System Instance
extern payment_system_t *g_payment_system;

// Core API Functions
int payment_system_init(void);
int payment_system_cleanup(void);
payment_system_t *payment_system_get_instance(void);

// Utility Functions
const char *payment_method_to_string(payment_method_t method);
const char *payment_status_to_string(payment_status_t status);
const char *currency_to_string(currency_t currency);
payment_method_t string_to_payment_method(const char *method_str);
payment_status_t string_to_payment_status(const char *status_str);
currency_t string_to_currency(const char *currency_str);

// Validation Functions
bool is_valid_credit_card(const char *card_number);
bool is_valid_iban(const char *iban);
bool is_valid_swift_code(const char *swift_code);
bool is_valid_email(const char *email);
bool is_valid_ip_address(const char *ip_address);

// Conversion Functions
uint64_t currency_to_smallest_unit(double amount, currency_t currency);
double smallest_unit_to_currency(uint64_t amount, currency_t currency);
int convert_currency(uint64_t amount, currency_t from_currency, currency_t to_currency, uint64_t *converted_amount);

// Security Functions
int generate_transaction_id(uint64_t *transaction_id);
int generate_reference_number(char *reference_number, size_t max_size);
int hash_payment_data(const char *data, char *hash, size_t hash_size);
int sign_payment_data(const char *data, char *signature, size_t signature_size);

// Configuration Functions
int load_payment_config(const char *config_file);
int save_payment_config(const char *config_file);
int set_default_currency(currency_t currency);
int set_default_payment_method(payment_method_t method);

// Logging Functions
int payment_log_info(const char *message, ...);
int payment_log_warning(const char *message, ...);
int payment_log_error(const char *message, ...);
int payment_log_debug(const char *message, ...);

#endif // USDTGVERSE_PAYMENT_CORE_H
