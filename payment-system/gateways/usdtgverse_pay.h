/*
==============================================
 File:        usdtgverse_pay.h
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse PAY Gateway (OdixPay Rebranded)
   
   Advanced payment gateway featuring:
   - OdixPay integration rebranded as USDTgVerse PAY
   - Quantum-safe transaction processing
   - Multi-currency support (USDTg, USDTgV, USDTgG)
   - Real-time payment tracking
   - Advanced security features
   - Mobile and web integration
   - API and SDK support

 License:
   MIT License
==============================================
*/

#ifndef USDTGVERSE_PAY_H
#define USDTGVERSE_PAY_H

#include "../core/payment_core.h"
#include <stdint.h>
#include <stdbool.h>

// USDTgVerse PAY Configuration
typedef struct {
    char merchant_id[64];
    char api_key[256];
    char secret_key[256];
    char webhook_url[256];
    char endpoint_url[256];
    bool is_test_mode;
    bool is_enabled;
    uint32_t timeout_seconds;
    char supported_currencies[256];
    char supported_methods[256];
    char quantum_key[512]; // Quantum-safe encryption key
    char server_ip[45];
    uint16_t server_port;
    bool use_ssl;
    char ssl_cert[512];
    char ssl_key[512];
} usdtgverse_pay_config_t;

// USDTgVerse PAY Transaction
typedef struct {
    uint64_t transaction_id;
    char merchant_id[64];
    char customer_id[64];
    char payment_method[32];
    char currency[16];
    uint64_t amount;
    char description[256];
    char reference_number[64];
    char status[32];
    time_t created_at;
    time_t updated_at;
    time_t completed_at;
    char ip_address[45];
    char user_agent[512];
    char metadata[1024];
    char quantum_signature[512];
    char odixpay_transaction_id[64];
    char odixpay_reference[64];
} usdtgverse_pay_transaction_t;

// USDTgVerse PAY Account
typedef struct {
    char account_id[64];
    char wallet_address[128];
    char public_key[256];
    char private_key[512]; // Encrypted
    uint64_t balance_usdtg;
    uint64_t balance_usdtgv;
    uint64_t balance_usdtgg;
    char qr_code[512];
    bool is_verified;
    time_t created_at;
    time_t last_activity;
    char kyc_status[32];
    char compliance_level[32];
} usdtgverse_pay_account_t;

// USDTgVerse PAY API Response
typedef struct {
    bool success;
    int error_code;
    char error_message[256];
    char transaction_id[64];
    char reference_number[64];
    char status[32];
    char payment_url[512];
    char qr_code[512];
    char quantum_signature[512];
    char metadata[1024];
} usdtgverse_pay_response_t;

// USDTgVerse PAY Webhook Data
typedef struct {
    char event_type[64];
    char transaction_id[64];
    char merchant_id[64];
    char customer_id[64];
    char status[32];
    char amount[32];
    char currency[16];
    char reference_number[64];
    char quantum_signature[512];
    char timestamp[32];
    char metadata[1024];
} usdtgverse_pay_webhook_t;

// USDTgVerse PAY Functions
typedef struct {
    // Core Functions
    int (*initialize)(usdtgverse_pay_config_t *config);
    int (*shutdown)(void);
    int (*process_payment)(usdtgverse_pay_transaction_t *transaction, usdtgverse_pay_response_t *response);
    int (*refund_payment)(char *transaction_id, uint64_t refund_amount, usdtgverse_pay_response_t *response);
    int (*cancel_payment)(char *transaction_id, usdtgverse_pay_response_t *response);
    
    // Transaction Management
    int (*get_transaction)(char *transaction_id, usdtgverse_pay_transaction_t *transaction);
    int (*update_transaction_status)(char *transaction_id, char *status);
    int (*list_transactions)(char *merchant_id, usdtgverse_pay_transaction_t *transactions, int max_count);
    
    // Account Management
    int (*create_account)(usdtgverse_pay_account_t *account);
    int (*get_account)(char *account_id, usdtgverse_pay_account_t *account);
    int (*update_account)(usdtgverse_pay_account_t *account);
    int (*delete_account)(char *account_id);
    int (*verify_account)(char *account_id, char *verification_data);
    
    // Balance Management
    int (*get_balance)(char *account_id, char *currency, uint64_t *balance);
    int (*transfer_balance)(char *from_account, char *to_account, char *currency, uint64_t amount);
    int (*add_balance)(char *account_id, char *currency, uint64_t amount);
    int (*subtract_balance)(char *account_id, char *currency, uint64_t amount);
    
    // Security Functions
    int (*generate_quantum_signature)(char *data, char *signature);
    int (*verify_quantum_signature)(char *data, char *signature);
    int (*encrypt_sensitive_data)(char *data, char *encrypted_data, size_t data_size);
    int (*decrypt_sensitive_data)(char *encrypted_data, char *data, size_t data_size);
    int (*validate_transaction)(usdtgverse_pay_transaction_t *transaction);
    
    // Webhook Management
    int (*register_webhook)(char *webhook_url);
    int (*process_webhook)(char *webhook_data, usdtgverse_pay_webhook_t *webhook);
    int (*validate_webhook_signature)(char *webhook_data, char *signature);
    int (*send_webhook_notification)(char *webhook_url, usdtgverse_pay_webhook_t *webhook);
    
    // QR Code Generation
    int (*generate_qr_code)(char *payment_data, char *qr_code_data);
    int (*generate_payment_qr)(usdtgverse_pay_transaction_t *transaction, char *qr_code);
    int (*generate_account_qr)(char *account_id, char *qr_code);
    
    // Mobile Integration
    int (*generate_mobile_payment_url)(usdtgverse_pay_transaction_t *transaction, char *mobile_url);
    int (*process_mobile_payment)(char *mobile_payment_data, usdtgverse_pay_response_t *response);
    int (*validate_mobile_payment)(char *mobile_payment_data);
    
    // API Integration
    int (*make_api_request)(char *endpoint, char *method, char *data, char *response);
    int (*parse_api_response)(char *response, usdtgverse_pay_response_t *parsed_response);
    int (*handle_api_error)(int error_code, char *error_message);
    
    // Analytics and Reporting
    int (*get_payment_analytics)(char *merchant_id, time_t start_date, time_t end_date, char *analytics_json);
    int (*generate_payment_report)(char *merchant_id, time_t start_date, time_t end_date, char *report_path);
    int (*get_revenue_summary)(char *merchant_id, time_t start_date, time_t end_date, char *summary_json);
    
    // Compliance and KYC
    int (*submit_kyc_document)(char *account_id, char *document_type, char *document_data);
    int (*verify_kyc_status)(char *account_id, char *kyc_status);
    int (*update_compliance_level)(char *account_id, char *compliance_level);
    int (*check_aml_compliance)(char *account_id, char *transaction_data);
    
    // Error Handling
    int (*get_last_error)(char *error_message, size_t max_size);
    int (*get_error_code)(void);
    void (*clear_error)(void);
} usdtgverse_pay_t;

// Global USDTgVerse PAY Instance
extern usdtgverse_pay_t *g_usdtgverse_pay;

// Core API Functions
int usdtgverse_pay_init(usdtgverse_pay_config_t *config);
int usdtgverse_pay_cleanup(void);
usdtgverse_pay_t *usdtgverse_pay_get_instance(void);

// Utility Functions
const char *usdtgverse_pay_status_to_string(const char *status);
const char *usdtgverse_pay_currency_to_string(const char *currency);
bool usdtgverse_pay_is_valid_currency(const char *currency);
bool usdtgverse_pay_is_valid_status(const char *status);
bool usdtgverse_pay_is_valid_account_id(const char *account_id);
bool usdtgverse_pay_is_valid_transaction_id(const char *transaction_id);

// Conversion Functions
uint64_t usdtgverse_pay_currency_to_smallest_unit(double amount, const char *currency);
double usdtgverse_pay_smallest_unit_to_currency(uint64_t amount, const char *currency);
int usdtgverse_pay_convert_currency(uint64_t amount, const char *from_currency, const char *to_currency, uint64_t *converted_amount);

// Security Functions
int usdtgverse_pay_generate_transaction_id(char *transaction_id, size_t max_size);
int usdtgverse_pay_generate_reference_number(char *reference_number, size_t max_size);
int usdtgverse_pay_hash_payment_data(const char *data, char *hash, size_t hash_size);
int usdtgverse_pay_sign_payment_data(const char *data, char *signature, size_t signature_size);

// Configuration Functions
int usdtgverse_pay_load_config(const char *config_file, usdtgverse_pay_config_t *config);
int usdtgverse_pay_save_config(const char *config_file, usdtgverse_pay_config_t *config);
int usdtgverse_pay_set_default_currency(const char *currency);
int usdtgverse_pay_set_default_payment_method(const char *method);

// Logging Functions
int usdtgverse_pay_log_info(const char *message, ...);
int usdtgverse_pay_log_warning(const char *message, ...);
int usdtgverse_pay_log_error(const char *message, ...);
int usdtgverse_pay_log_debug(const char *message, ...);

// Constants
#define USDTGVERSE_PAY_MAX_TRANSACTION_ID_LEN 64
#define USDTGVERSE_PAY_MAX_ACCOUNT_ID_LEN 64
#define USDTGVERSE_PAY_MAX_CURRENCY_LEN 16
#define USDTGVERSE_PAY_MAX_STATUS_LEN 32
#define USDTGVERSE_PAY_MAX_DESCRIPTION_LEN 256
#define USDTGVERSE_PAY_MAX_METADATA_LEN 1024
#define USDTGVERSE_PAY_MAX_QR_CODE_LEN 512
#define USDTGVERSE_PAY_MAX_SIGNATURE_LEN 512
#define USDTGVERSE_PAY_MAX_WEBHOOK_URL_LEN 256
#define USDTGVERSE_PAY_MAX_API_KEY_LEN 256
#define USDTGVERSE_PAY_MAX_SECRET_KEY_LEN 256

// Supported Currencies
#define USDTGVERSE_PAY_CURRENCY_USDTG "USDTg"
#define USDTGVERSE_PAY_CURRENCY_USDTGV "USDTgV"
#define USDTGVERSE_PAY_CURRENCY_USDTGG "USDTgG"
#define USDTGVERSE_PAY_CURRENCY_USD "USD"
#define USDTGVERSE_PAY_CURRENCY_EUR "EUR"
#define USDTGVERSE_PAY_CURRENCY_GBP "GBP"
#define USDTGVERSE_PAY_CURRENCY_BTC "BTC"
#define USDTGVERSE_PAY_CURRENCY_ETH "ETH"

// Transaction Statuses
#define USDTGVERSE_PAY_STATUS_PENDING "pending"
#define USDTGVERSE_PAY_STATUS_PROCESSING "processing"
#define USDTGVERSE_PAY_STATUS_COMPLETED "completed"
#define USDTGVERSE_PAY_STATUS_FAILED "failed"
#define USDTGVERSE_PAY_STATUS_CANCELLED "cancelled"
#define USDTGVERSE_PAY_STATUS_REFUNDED "refunded"

// Event Types
#define USDTGVERSE_PAY_EVENT_PAYMENT_CREATED "payment.created"
#define USDTGVERSE_PAY_EVENT_PAYMENT_PROCESSING "payment.processing"
#define USDTGVERSE_PAY_EVENT_PAYMENT_COMPLETED "payment.completed"
#define USDTGVERSE_PAY_EVENT_PAYMENT_FAILED "payment.failed"
#define USDTGVERSE_PAY_EVENT_PAYMENT_CANCELLED "payment.cancelled"
#define USDTGVERSE_PAY_EVENT_PAYMENT_REFUNDED "payment.refunded"

// Error Codes
#define USDTGVERSE_PAY_ERROR_NONE 0
#define USDTGVERSE_PAY_ERROR_INVALID_CONFIG -1
#define USDTGVERSE_PAY_ERROR_INVALID_TRANSACTION -2
#define USDTGVERSE_PAY_ERROR_INVALID_ACCOUNT -3
#define USDTGVERSE_PAY_ERROR_INSUFFICIENT_BALANCE -4
#define USDTGVERSE_PAY_ERROR_INVALID_SIGNATURE -5
#define USDTGVERSE_PAY_ERROR_NETWORK_ERROR -6
#define USDTGVERSE_PAY_ERROR_API_ERROR -7
#define USDTGVERSE_PAY_ERROR_DATABASE_ERROR -8
#define USDTGVERSE_PAY_ERROR_SECURITY_ERROR -9
#define USDTGVERSE_PAY_ERROR_COMPLIANCE_ERROR -10

#endif // USDTGVERSE_PAY_H
