/**
 * 🔧 USDTgVerse C SDK - Main Header
 * ================================
 * 
 * ⚛️ QUANTUM-SAFE BLOCKCHAIN SDK
 * 🌍 CROSS-PLATFORM COMPATIBILITY
 * 🔐 ENTERPRISE-GRADE SECURITY
 * 
 * Version: 1.0.0
 * License: MIT
 * 
 * Supported Platforms:
 * - Windows (MSVC, MinGW)
 * - macOS (Clang, GCC)
 * - Linux (GCC, Clang)
 * - FreeBSD, OpenBSD
 * - ARM64, x86_64, x86
 */

#ifndef USDTGVERSE_H
#define USDTGVERSE_H

#ifdef __cplusplus
extern "C" {
#endif

// 📦 STANDARD INCLUDES
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// 🔧 PLATFORM DETECTION
#ifdef _WIN32
    #ifdef USDTGVERSE_EXPORTS
        #define USDTG_API __declspec(dllexport)
    #else
        #define USDTG_API __declspec(dllimport)
    #endif
    #define USDTG_CALL __stdcall
#else
    #define USDTG_API __attribute__((visibility("default")))
    #define USDTG_CALL
#endif

// 📊 VERSION INFORMATION
#define USDTGVERSE_VERSION_MAJOR 1
#define USDTGVERSE_VERSION_MINOR 0
#define USDTGVERSE_VERSION_PATCH 0
#define USDTGVERSE_VERSION_STRING "1.0.0-quantum-safe"

// ⚛️ QUANTUM SECURITY CONSTANTS
#define USDTG_QUANTUM_SECURITY_LEVEL 256
#define USDTG_ADDRESS_LENGTH 51
#define USDTG_PRIVATE_KEY_LENGTH 64
#define USDTG_PUBLIC_KEY_LENGTH 128
#define USDTG_SIGNATURE_LENGTH 128
#define USDTG_HASH_LENGTH 32
#define USDTG_MNEMONIC_MAX_LENGTH 1024

// 🌍 NETWORK IDENTIFIERS
typedef enum {
    USDTG_NETWORK_MAINNET = 2024,
    USDTG_NETWORK_TESTNET = 2025,
    USDTG_NETWORK_ETHEREUM = 1,
    USDTG_NETWORK_BNB = 56,
    USDTG_NETWORK_POLYGON = 137,
    USDTG_NETWORK_ARBITRUM = 42161,
    USDTG_NETWORK_AVALANCHE = 43114,
    USDTG_NETWORK_TRON = 728126428,
    USDTG_NETWORK_SOLANA = 101
} usdtg_network_t;

// 🔐 ERROR CODES
typedef enum {
    USDTG_SUCCESS = 0,
    USDTG_ERROR_INVALID_PARAMETER = -1,
    USDTG_ERROR_OUT_OF_MEMORY = -2,
    USDTG_ERROR_NETWORK_ERROR = -3,
    USDTG_ERROR_INVALID_ADDRESS = -4,
    USDTG_ERROR_INVALID_PRIVATE_KEY = -5,
    USDTG_ERROR_INVALID_SIGNATURE = -6,
    USDTG_ERROR_INSUFFICIENT_BALANCE = -7,
    USDTG_ERROR_TRANSACTION_FAILED = -8,
    USDTG_ERROR_CONTRACT_ERROR = -9,
    USDTG_ERROR_QUANTUM_SECURITY_VIOLATION = -10,
    USDTG_ERROR_MNEMONIC_INVALID = -11,
    USDTG_ERROR_KEY_DERIVATION_FAILED = -12,
    USDTG_ERROR_ENCRYPTION_FAILED = -13,
    USDTG_ERROR_DECRYPTION_FAILED = -14,
    USDTG_ERROR_UNKNOWN = -999
} usdtg_error_t;

// 💰 TRANSACTION TYPES
typedef enum {
    USDTG_TX_TRANSFER = 0,
    USDTG_TX_CONTRACT_CALL = 1,
    USDTG_TX_CONTRACT_DEPLOY = 2,
    USDTG_TX_STAKE = 3,
    USDTG_TX_UNSTAKE = 4,
    USDTG_TX_BRIDGE = 5,
    USDTG_TX_GOVERNANCE = 6
} usdtg_transaction_type_t;

// 📊 TRANSACTION STATUS
typedef enum {
    USDTG_TX_STATUS_PENDING = 0,
    USDTG_TX_STATUS_CONFIRMED = 1,
    USDTG_TX_STATUS_FAILED = 2,
    USDTG_TX_STATUS_REVERTED = 3
} usdtg_transaction_status_t;

// 🏗️ CORE STRUCTURES

/**
 * 🔑 Quantum-Safe Wallet Structure
 */
typedef struct {
    char address[USDTG_ADDRESS_LENGTH + 1];
    uint8_t private_key[USDTG_PRIVATE_KEY_LENGTH];
    uint8_t public_key[USDTG_PUBLIC_KEY_LENGTH];
    char mnemonic[USDTG_MNEMONIC_MAX_LENGTH];
    usdtg_network_t network;
    bool quantum_safe;
    uint64_t nonce;
    uint64_t balance;
} usdtg_wallet_t;

/**
 * 💸 Transaction Structure
 */
typedef struct {
    char hash[65];
    char from[USDTG_ADDRESS_LENGTH + 1];
    char to[USDTG_ADDRESS_LENGTH + 1];
    uint64_t value;
    uint64_t gas_limit;
    uint64_t gas_price;
    uint64_t nonce;
    uint8_t* data;
    size_t data_length;
    usdtg_transaction_type_t type;
    usdtg_transaction_status_t status;
    uint64_t block_number;
    uint64_t timestamp;
    usdtg_network_t network;
} usdtg_transaction_t;

/**
 * 📄 Smart Contract Structure
 */
typedef struct {
    char address[USDTG_ADDRESS_LENGTH + 1];
    char name[256];
    char symbol[32];
    uint8_t decimals;
    uint64_t total_supply;
    usdtg_network_t network;
    bool quantum_safe;
    uint8_t* bytecode;
    size_t bytecode_length;
    char* abi;
} usdtg_contract_t;

/**
 * 🌐 Network Configuration
 */
typedef struct {
    usdtg_network_t id;
    char name[64];
    char rpc_url[256];
    char explorer_url[256];
    char currency_symbol[16];
    uint64_t chain_id;
    uint64_t gas_limit;
    uint64_t gas_price;
    bool quantum_safe;
} usdtg_network_config_t;

/**
 * 📊 Balance Information
 */
typedef struct {
    uint64_t native_balance;
    uint64_t usdtg_balance;
    uint32_t token_count;
    usdtg_contract_t* tokens;
    uint64_t total_value_usd;
    uint64_t last_updated;
} usdtg_balance_info_t;

// 🚀 CORE API FUNCTIONS

/**
 * 🔧 Initialize USDTgVerse SDK
 * @param network Target network
 * @param rpc_url RPC endpoint URL
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_init(
    usdtg_network_t network,
    const char* rpc_url
);

/**
 * 🧹 Cleanup SDK resources
 */
USDTG_API void USDTG_CALL usdtg_cleanup(void);

/**
 * 📋 Get SDK version information
 * @return Version string
 */
USDTG_API const char* USDTG_CALL usdtg_version(void);

/**
 * 🔐 Get error message for error code
 * @param error Error code
 * @return Error message string
 */
USDTG_API const char* USDTG_CALL usdtg_error_message(usdtg_error_t error);

// 🔑 WALLET MANAGEMENT

/**
 * 🆕 Create new quantum-safe wallet
 * @param wallet Wallet structure to initialize
 * @param mnemonic_words Number of mnemonic words (24, 36, 48)
 * @param network Target network
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_wallet_create(
    usdtg_wallet_t* wallet,
    uint32_t mnemonic_words,
    usdtg_network_t network
);

/**
 * 📥 Import wallet from mnemonic
 * @param wallet Wallet structure to initialize
 * @param mnemonic Mnemonic phrase
 * @param network Target network
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_wallet_import(
    usdtg_wallet_t* wallet,
    const char* mnemonic,
    usdtg_network_t network
);

/**
 * 📤 Export wallet private key
 * @param wallet Wallet structure
 * @param private_key Buffer for private key (hex string)
 * @param buffer_size Size of private key buffer
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_wallet_export_private_key(
    const usdtg_wallet_t* wallet,
    char* private_key,
    size_t buffer_size
);

/**
 * 💰 Get wallet balance
 * @param wallet Wallet structure
 * @param balance_info Balance information structure
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_wallet_get_balance(
    const usdtg_wallet_t* wallet,
    usdtg_balance_info_t* balance_info
);

/**
 * ✅ Validate wallet address
 * @param address Address string to validate
 * @param network Target network
 * @return true if valid, false otherwise
 */
USDTG_API bool USDTG_CALL usdtg_wallet_validate_address(
    const char* address,
    usdtg_network_t network
);

// 💸 TRANSACTION MANAGEMENT

/**
 * 📤 Send native currency transaction
 * @param wallet Sender wallet
 * @param to_address Recipient address
 * @param amount Amount to send (in wei)
 * @param gas_limit Gas limit
 * @param gas_price Gas price (in gwei)
 * @param transaction Transaction structure to fill
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_transaction_send(
    const usdtg_wallet_t* wallet,
    const char* to_address,
    uint64_t amount,
    uint64_t gas_limit,
    uint64_t gas_price,
    usdtg_transaction_t* transaction
);

/**
 * 🪙 Send token transaction
 * @param wallet Sender wallet
 * @param contract_address Token contract address
 * @param to_address Recipient address
 * @param amount Amount to send (in token units)
 * @param transaction Transaction structure to fill
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_transaction_send_token(
    const usdtg_wallet_t* wallet,
    const char* contract_address,
    const char* to_address,
    uint64_t amount,
    usdtg_transaction_t* transaction
);

/**
 * 🔍 Get transaction by hash
 * @param tx_hash Transaction hash
 * @param transaction Transaction structure to fill
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_transaction_get(
    const char* tx_hash,
    usdtg_transaction_t* transaction
);

/**
 * ⏳ Wait for transaction confirmation
 * @param tx_hash Transaction hash
 * @param confirmations Number of confirmations to wait for
 * @param timeout_seconds Timeout in seconds
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_transaction_wait(
    const char* tx_hash,
    uint32_t confirmations,
    uint32_t timeout_seconds
);

// 📄 SMART CONTRACT INTERACTION

/**
 * 🚀 Deploy smart contract
 * @param wallet Deployer wallet
 * @param bytecode Contract bytecode
 * @param bytecode_length Bytecode length
 * @param constructor_data Constructor parameters
 * @param constructor_length Constructor data length
 * @param contract Contract structure to fill
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_contract_deploy(
    const usdtg_wallet_t* wallet,
    const uint8_t* bytecode,
    size_t bytecode_length,
    const uint8_t* constructor_data,
    size_t constructor_length,
    usdtg_contract_t* contract
);

/**
 * 📞 Call smart contract function
 * @param wallet Caller wallet
 * @param contract_address Contract address
 * @param function_data Function call data
 * @param data_length Data length
 * @param result Result buffer
 * @param result_length Result buffer size
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_contract_call(
    const usdtg_wallet_t* wallet,
    const char* contract_address,
    const uint8_t* function_data,
    size_t data_length,
    uint8_t* result,
    size_t* result_length
);

/**
 * 👁️ Read contract state (view function)
 * @param contract_address Contract address
 * @param function_data Function call data
 * @param data_length Data length
 * @param result Result buffer
 * @param result_length Result buffer size
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_contract_read(
    const char* contract_address,
    const uint8_t* function_data,
    size_t data_length,
    uint8_t* result,
    size_t* result_length
);

// ⚛️ QUANTUM-SAFE CRYPTOGRAPHY

/**
 * 🔐 Generate quantum-safe key pair
 * @param private_key Private key buffer
 * @param public_key Public key buffer
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_crypto_generate_keypair(
    uint8_t private_key[USDTG_PRIVATE_KEY_LENGTH],
    uint8_t public_key[USDTG_PUBLIC_KEY_LENGTH]
);

/**
 * ✍️ Create quantum-safe signature
 * @param private_key Private key
 * @param message Message to sign
 * @param message_length Message length
 * @param signature Signature buffer
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_crypto_sign(
    const uint8_t private_key[USDTG_PRIVATE_KEY_LENGTH],
    const uint8_t* message,
    size_t message_length,
    uint8_t signature[USDTG_SIGNATURE_LENGTH]
);

/**
 * ✅ Verify quantum-safe signature
 * @param public_key Public key
 * @param message Original message
 * @param message_length Message length
 * @param signature Signature to verify
 * @return true if valid, false otherwise
 */
USDTG_API bool USDTG_CALL usdtg_crypto_verify(
    const uint8_t public_key[USDTG_PUBLIC_KEY_LENGTH],
    const uint8_t* message,
    size_t message_length,
    const uint8_t signature[USDTG_SIGNATURE_LENGTH]
);

/**
 * 🏠 Generate quantum-safe address
 * @param public_key Public key
 * @param network Target network
 * @param address Address buffer
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_crypto_address_from_pubkey(
    const uint8_t public_key[USDTG_PUBLIC_KEY_LENGTH],
    usdtg_network_t network,
    char address[USDTG_ADDRESS_LENGTH + 1]
);

// 🌐 NETWORK UTILITIES

/**
 * 📡 Get network configuration
 * @param network Network identifier
 * @param config Network configuration structure
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_network_get_config(
    usdtg_network_t network,
    usdtg_network_config_t* config
);

/**
 * 🔗 Get current block number
 * @param block_number Block number pointer
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_network_get_block_number(
    uint64_t* block_number
);

/**
 * ⛽ Estimate gas for transaction
 * @param from From address
 * @param to To address
 * @param data Transaction data
 * @param data_length Data length
 * @param gas_estimate Gas estimate pointer
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_network_estimate_gas(
    const char* from,
    const char* to,
    const uint8_t* data,
    size_t data_length,
    uint64_t* gas_estimate
);

// 🛠️ UTILITY FUNCTIONS

/**
 * 🔄 Convert wei to ether
 * @param wei_amount Amount in wei
 * @param ether_string Ether string buffer
 * @param buffer_size Buffer size
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_utils_wei_to_ether(
    uint64_t wei_amount,
    char* ether_string,
    size_t buffer_size
);

/**
 * 🔄 Convert ether to wei
 * @param ether_string Ether amount string
 * @param wei_amount Wei amount pointer
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_utils_ether_to_wei(
    const char* ether_string,
    uint64_t* wei_amount
);

/**
 * 🔤 Convert bytes to hex string
 * @param bytes Byte array
 * @param length Byte array length
 * @param hex_string Hex string buffer
 * @param buffer_size Buffer size
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_utils_bytes_to_hex(
    const uint8_t* bytes,
    size_t length,
    char* hex_string,
    size_t buffer_size
);

/**
 * 🔢 Convert hex string to bytes
 * @param hex_string Hex string
 * @param bytes Byte array buffer
 * @param buffer_size Buffer size
 * @param actual_length Actual length pointer
 * @return Error code
 */
USDTG_API usdtg_error_t USDTG_CALL usdtg_utils_hex_to_bytes(
    const char* hex_string,
    uint8_t* bytes,
    size_t buffer_size,
    size_t* actual_length
);

#ifdef __cplusplus
}
#endif

#endif // USDTGVERSE_H
