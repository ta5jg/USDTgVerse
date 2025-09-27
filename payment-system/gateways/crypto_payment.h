/*
==============================================
 File:        crypto_payment.h
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Cryptocurrency Payment Gateway
   
   Comprehensive cryptocurrency payment processing featuring:
   - Multiple cryptocurrency support (BTC, ETH, BNB, TRX, USDT, USDC)
   - USDTgVerse native tokens (USDTg, USDTgV, USDTgG)
   - Real-time price conversion
   - Blockchain transaction verification
   - Multi-signature wallet support
   - Quantum-safe transaction security
   - Cross-chain compatibility

 License:
   MIT License
==============================================
*/

#ifndef USDTGVERSE_CRYPTO_PAYMENT_H
#define USDTGVERSE_CRYPTO_PAYMENT_H

#include "../core/payment_core.h"
#include <stdint.h>
#include <stdbool.h>

// Cryptocurrency Types
typedef enum {
    CRYPTO_BTC = 1,           // Bitcoin
    CRYPTO_ETH = 2,           // Ethereum
    CRYPTO_BNB = 3,           // Binance Coin
    CRYPTO_TRX = 4,           // TRON
    CRYPTO_USDT = 5,          // Tether
    CRYPTO_USDC = 6,          // USD Coin
    CRYPTO_USDTG = 7,         // USDTgVerse Native
    CRYPTO_USDTGV = 8,        // USDTgVerse Utility
    CRYPTO_USDTGG = 9         // USDTgVerse Governance
} crypto_type_t;

// Blockchain Networks
typedef enum {
    NETWORK_BITCOIN = 1,
    NETWORK_ETHEREUM = 2,
    NETWORK_BSC = 3,          // Binance Smart Chain
    NETWORK_TRON = 4,
    NETWORK_POLYGON = 5,
    NETWORK_USDTGVERSE = 6,   // USDTgVerse Native Network
    NETWORK_ARBITRUM = 7,
    NETWORK_OPTIMISM = 8
} blockchain_network_t;

// Transaction Status
typedef enum {
    CRYPTO_TX_PENDING = 1,
    CRYPTO_TX_CONFIRMED = 2,
    CRYPTO_TX_FAILED = 3,
    CRYPTO_TX_CANCELLED = 4
} crypto_tx_status_t;

// Cryptocurrency Transaction Structure
typedef struct {
    uint64_t transaction_id;
    crypto_type_t crypto_type;
    blockchain_network_t network;
    char from_address[128];
    char to_address[128];
    uint64_t amount;          // Amount in smallest unit (satoshis, wei, etc.)
    uint64_t gas_fee;         // Network fee
    char transaction_hash[128];
    uint32_t confirmations;
    crypto_tx_status_t status;
    time_t created_at;
    time_t confirmed_at;
    char block_hash[128];
    uint64_t block_number;
    char metadata[1024];
} crypto_transaction_t;

// Cryptocurrency Wallet Information
typedef struct {
    crypto_type_t crypto_type;
    blockchain_network_t network;
    char wallet_address[128];
    char private_key[256];    // Encrypted
    char public_key[256];
    uint64_t balance;
    uint64_t pending_balance;
    bool is_multisig;
    uint32_t required_signatures;
    char multisig_addresses[10][128]; // Up to 10 addresses
} crypto_wallet_t;

// Price Information
typedef struct {
    crypto_type_t crypto_type;
    double price_usd;
    double price_eur;
    double price_gbp;
    double price_try;
    double price_btc;
    double price_eth;
    double market_cap;
    double volume_24h;
    double change_24h;
    time_t last_updated;
} crypto_price_t;

// Cryptocurrency Payment Configuration
typedef struct {
    char api_key[256];
    char secret_key[256];
    char webhook_url[256];
    bool is_testnet;
    bool is_enabled;
    uint32_t confirmation_blocks;
    uint32_t timeout_seconds;
    char supported_cryptos[256];
    char supported_networks[256];
    char price_api_url[256];
    char blockchain_api_url[256];
} crypto_payment_config_t;

// Cryptocurrency Payment Functions
typedef struct {
    // Core Functions
    int (*initialize)(crypto_payment_config_t *config);
    int (*shutdown)(void);
    int (*process_crypto_payment)(payment_transaction_t *payment, crypto_transaction_t *crypto_tx);
    int (*verify_transaction)(crypto_transaction_t *crypto_tx);
    int (*get_transaction_status)(char *tx_hash, crypto_tx_status_t *status);
    
    // Wallet Management
    int (*create_wallet)(crypto_type_t crypto_type, blockchain_network_t network, crypto_wallet_t *wallet);
    int (*import_wallet)(crypto_type_t crypto_type, char *private_key, crypto_wallet_t *wallet);
    int (*get_wallet_balance)(crypto_wallet_t *wallet, uint64_t *balance);
    int (*get_wallet_address)(crypto_type_t crypto_type, blockchain_network_t network, char *address);
    
    // Transaction Management
    int (*create_transaction)(crypto_wallet_t *wallet, char *to_address, uint64_t amount, crypto_transaction_t *tx);
    int (*sign_transaction)(crypto_transaction_t *tx, crypto_wallet_t *wallet);
    int (*broadcast_transaction)(crypto_transaction_t *tx);
    int (*get_transaction_details)(char *tx_hash, crypto_transaction_t *tx);
    
    // Price and Conversion
    int (*get_crypto_price)(crypto_type_t crypto_type, crypto_price_t *price);
    int (*convert_crypto_amount)(uint64_t amount, crypto_type_t from_crypto, crypto_type_t to_crypto, uint64_t *converted_amount);
    int (*convert_to_fiat)(uint64_t crypto_amount, crypto_type_t crypto_type, currency_t fiat_currency, uint64_t *fiat_amount);
    int (*convert_from_fiat)(uint64_t fiat_amount, currency_t fiat_currency, crypto_type_t crypto_type, uint64_t *crypto_amount);
    
    // Network Functions
    int (*get_network_fee)(crypto_type_t crypto_type, blockchain_network_t network, uint64_t *fee);
    int (*estimate_gas)(crypto_type_t crypto_type, char *to_address, uint64_t amount, uint64_t *gas_estimate);
    int (*get_network_status)(blockchain_network_t network, char *status_json);
    
    // Security Functions
    int (*encrypt_private_key)(char *private_key, char *encrypted_key, size_t key_size);
    int (*decrypt_private_key)(char *encrypted_key, char *private_key, size_t key_size);
    int (*generate_quantum_signature)(char *data, char *signature);
    int (*verify_quantum_signature)(char *data, char *signature);
    
    // Multi-signature Functions
    int (*create_multisig_wallet)(crypto_type_t crypto_type, char *addresses[], uint32_t num_addresses, uint32_t required_signatures, crypto_wallet_t *wallet);
    int (*sign_multisig_transaction)(crypto_transaction_t *tx, crypto_wallet_t *wallet, char *signature);
    int (*verify_multisig_signature)(crypto_transaction_t *tx, char *signature, bool *is_valid);
    
    // Cross-chain Functions
    int (*bridge_crypto)(crypto_type_t crypto_type, blockchain_network_t from_network, blockchain_network_t to_network, uint64_t amount, char *bridge_tx_hash);
    int (*get_bridge_status)(char *bridge_tx_hash, char *status_json);
    
    // Error Handling
    int (*get_last_error)(char *error_message, size_t max_size);
    int (*get_error_code)(void);
    void (*clear_error)(void);
} crypto_payment_t;

// Global Cryptocurrency Payment Instance
extern crypto_payment_t *g_crypto_payment;

// Core API Functions
int crypto_payment_init(crypto_payment_config_t *config);
int crypto_payment_cleanup(void);
crypto_payment_t *crypto_payment_get_instance(void);

// Utility Functions
const char *crypto_type_to_string(crypto_type_t type);
const char *blockchain_network_to_string(blockchain_network_t network);
const char *crypto_tx_status_to_string(crypto_tx_status_t status);
crypto_type_t string_to_crypto_type(const char *type_str);
blockchain_network_t string_to_blockchain_network(const char *network_str);
crypto_tx_status_t string_to_crypto_tx_status(const char *status_str);

// Validation Functions
bool is_valid_crypto_address(const char *address, crypto_type_t crypto_type);
bool is_valid_transaction_hash(const char *tx_hash, crypto_type_t crypto_type);
bool is_valid_private_key(const char *private_key, crypto_type_t crypto_type);
bool is_valid_amount(uint64_t amount, crypto_type_t crypto_type);

// Conversion Functions
uint64_t crypto_to_smallest_unit(double amount, crypto_type_t crypto_type);
double smallest_unit_to_crypto(uint64_t amount, crypto_type_t crypto_type);
int get_crypto_decimals(crypto_type_t crypto_type);

// Price Functions
int get_current_crypto_price(crypto_type_t crypto_type, double *price_usd);
int get_crypto_price_history(crypto_type_t crypto_type, time_t start_time, time_t end_time, char *price_data);
int calculate_crypto_fee(crypto_type_t crypto_type, blockchain_network_t network, uint64_t amount, uint64_t *fee);

// Security Functions
int generate_crypto_wallet(crypto_type_t crypto_type, blockchain_network_t network, crypto_wallet_t *wallet);
int encrypt_crypto_private_key(char *private_key, char *password, char *encrypted_key);
int decrypt_crypto_private_key(char *encrypted_key, char *password, char *private_key);
int sign_crypto_message(char *message, char *private_key, char *signature);

// Configuration Functions
int load_crypto_payment_config(const char *config_file, crypto_payment_config_t *config);
int save_crypto_payment_config(const char *config_file, crypto_payment_config_t *config);
int set_default_crypto_network(crypto_type_t crypto_type, blockchain_network_t network);

// Logging Functions
int crypto_payment_log_info(const char *message, ...);
int crypto_payment_log_warning(const char *message, ...);
int crypto_payment_log_error(const char *message, ...);
int crypto_payment_log_debug(const char *message, ...);

// Constants
#define CRYPTO_MAX_ADDRESS_LEN 128
#define CRYPTO_MAX_PRIVATE_KEY_LEN 256
#define CRYPTO_MAX_PUBLIC_KEY_LEN 256
#define CRYPTO_MAX_TX_HASH_LEN 128
#define CRYPTO_MAX_BLOCK_HASH_LEN 128
#define CRYPTO_MAX_SIGNATURE_LEN 256
#define CRYPTO_MAX_METADATA_LEN 1024

// Cryptocurrency Decimals
#define BTC_DECIMALS 8
#define ETH_DECIMALS 18
#define BNB_DECIMALS 18
#define TRX_DECIMALS 6
#define USDT_DECIMALS 6
#define USDC_DECIMALS 6
#define USDTG_DECIMALS 18
#define USDTGV_DECIMALS 18
#define USDTGG_DECIMALS 18

// Default Configuration
#define CRYPTO_DEFAULT_CONFIRMATION_BLOCKS 3
#define CRYPTO_DEFAULT_TIMEOUT_SECONDS 300
#define CRYPTO_DEFAULT_GAS_LIMIT 21000
#define CRYPTO_DEFAULT_GAS_PRICE 20000000000

// Error Codes
#define CRYPTO_ERROR_NONE 0
#define CRYPTO_ERROR_INVALID_CONFIG -1
#define CRYPTO_ERROR_INVALID_ADDRESS -2
#define CRYPTO_ERROR_INVALID_AMOUNT -3
#define CRYPTO_ERROR_INSUFFICIENT_BALANCE -4
#define CRYPTO_ERROR_NETWORK_ERROR -5
#define CRYPTO_ERROR_TRANSACTION_FAILED -6
#define CRYPTO_ERROR_INVALID_SIGNATURE -7
#define CRYPTO_ERROR_PRICE_FETCH_FAILED -8
#define CRYPTO_ERROR_WALLET_CREATION_FAILED -9
#define CRYPTO_ERROR_TRANSACTION_NOT_FOUND -10

#endif // USDTGVERSE_CRYPTO_PAYMENT_H
