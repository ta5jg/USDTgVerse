/*
==============================================
 File:        account_abstraction.h
 Author:      Irfan Gedik
 Created:     26.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Account Abstraction System (EIP-4337)
   
   Account abstraction system featuring:
   - Smart contract wallets
   - Gasless transactions
   - Batch transaction processing
   - Social recovery mechanisms
   - Multi-signature support

 License:
   MIT License
==============================================
*/

#ifndef ACCOUNT_ABSTRACTION_H
#define ACCOUNT_ABSTRACTION_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Forward declarations
typedef struct AccountAbstractionSystem AccountAbstractionSystem;
typedef struct SmartContractWallet SmartContractWallet;
typedef struct MetaTransaction MetaTransaction;
typedef struct SocialRecovery SocialRecovery;
typedef struct GaslessTransaction GaslessTransaction;
typedef struct BatchTransaction BatchTransaction;

// Account types
typedef enum {
    ACCOUNT_EOA = 0,           // Externally Owned Account
    ACCOUNT_SMART_CONTRACT,    // Smart Contract Account
    ACCOUNT_MULTI_SIG,         // Multi-signature Account
    ACCOUNT_SOCIAL_RECOVERY,   // Social Recovery Account
    ACCOUNT_QUANTUM_SAFE       // Quantum-safe Account
} AccountType;

// Transaction types
typedef enum {
    TX_TRANSFER = 0,
    TX_CONTRACT_CALL,
    TX_DELEGATE_CALL,
    TX_CREATE,
    TX_SELF_DESTRUCT,
    TX_BATCH,
    TX_META_TRANSACTION
} TransactionType;

// Gas payment modes
typedef enum {
    GAS_PAYMENT_NORMAL = 0,    // User pays gas
    GAS_PAYMENT_SPONSORED,     // Sponsor pays gas
    GAS_PAYMENT_RELAYER,       // Relayer pays gas
    GAS_PAYMENT_BATCH          // Batch gas payment
} GasPaymentMode;

// Social recovery types
typedef enum {
    SOCIAL_RECOVERY_EMAIL = 0,
    SOCIAL_RECOVERY_PHONE,
    SOCIAL_RECOVERY_GOOGLE,
    SOCIAL_RECOVERY_APPLE,
    SOCIAL_RECOVERY_TWITTER,
    SOCIAL_RECOVERY_DISCORD,
    SOCIAL_RECOVERY_TELEGRAM
} SocialRecoveryType;

// Smart Contract Wallet structure
typedef struct {
    char wallet_address[42];
    char owner_address[42];
    AccountType account_type;
    bool is_initialized;
    uint64_t nonce;
    uint64_t balance;
    char implementation_address[42];
    char factory_address[42];
    time_t created_at;
    time_t last_used;
    bool is_active;
    char recovery_addresses[5][42];  // Up to 5 recovery addresses
    uint8_t recovery_threshold;
    uint8_t current_recovery_count;
} SmartContractWallet;

// Meta Transaction structure
typedef struct {
    char transaction_id[64];
    char from_address[42];
    char to_address[42];
    uint64_t value;
    char data[1024];
    uint64_t gas_limit;
    uint64_t gas_price;
    uint64_t nonce;
    char signature[128];
    GasPaymentMode gas_payment_mode;
    char sponsor_address[42];
    char relayer_address[42];
    time_t deadline;
    bool is_executed;
    char execution_hash[64];
} MetaTransaction;

// Social Recovery structure
typedef struct {
    char wallet_address[42];
    SocialRecoveryType recovery_type;
    char recovery_identifier[256];  // Email, phone, etc.
    char recovery_code[32];
    time_t code_expires_at;
    bool is_verified;
    uint8_t attempts_remaining;
    time_t last_attempt;
    char backup_codes[5][32];       // Backup recovery codes
    bool backup_codes_used[5];
} SocialRecovery;

// Gasless Transaction structure
typedef struct {
    char transaction_id[64];
    char user_address[42];
    char sponsor_address[42];
    uint64_t gas_cost;
    uint64_t gas_limit;
    uint64_t gas_price;
    char payment_token[42];         // Token used for payment
    uint64_t payment_amount;
    bool is_paid;
    time_t payment_deadline;
    char payment_signature[128];
} GaslessTransaction;

// Batch Transaction structure
typedef struct {
    char batch_id[64];
    char wallet_address[42];
    MetaTransaction* transactions;
    size_t transaction_count;
    uint64_t total_gas_limit;
    uint64_t total_gas_price;
    char batch_signature[128];
    bool is_executed;
    time_t execution_time;
    char execution_hash[64];
} BatchTransaction;

// Smart Contract Wallet functions
SmartContractWallet* smart_contract_wallet_create(const char* owner_address, AccountType account_type);
void smart_contract_wallet_destroy(SmartContractWallet* wallet);
bool smart_contract_wallet_initialize(SmartContractWallet* wallet, const char* implementation_address);
bool smart_contract_wallet_set_recovery_addresses(SmartContractWallet* wallet, const char** recovery_addresses, size_t count);
bool smart_contract_wallet_set_recovery_threshold(SmartContractWallet* wallet, uint8_t threshold);
bool smart_contract_wallet_validate_signature(SmartContractWallet* wallet, const char* message, const char* signature);
bool smart_contract_wallet_execute_transaction(SmartContractWallet* wallet, const MetaTransaction* transaction);
uint64_t smart_contract_wallet_get_nonce(SmartContractWallet* wallet);
bool smart_contract_wallet_increment_nonce(SmartContractWallet* wallet);
char* smart_contract_wallet_get_address(SmartContractWallet* wallet);
bool smart_contract_wallet_is_initialized(SmartContractWallet* wallet);
bool smart_contract_wallet_activate(SmartContractWallet* wallet);
bool smart_contract_wallet_deactivate(SmartContractWallet* wallet);

// Meta Transaction functions
MetaTransaction* meta_transaction_create(const char* from_address, const char* to_address, uint64_t value, const char* data);
void meta_transaction_destroy(MetaTransaction* transaction);
bool meta_transaction_set_gas_payment_mode(MetaTransaction* transaction, GasPaymentMode mode, const char* sponsor_address);
bool meta_transaction_set_deadline(MetaTransaction* transaction, time_t deadline);
bool meta_transaction_sign(MetaTransaction* transaction, const char* private_key);
bool meta_transaction_validate(MetaTransaction* transaction);
bool meta_transaction_execute(MetaTransaction* transaction);
char* meta_transaction_get_id(MetaTransaction* transaction);
bool meta_transaction_is_expired(MetaTransaction* transaction);
uint64_t meta_transaction_calculate_gas_cost(MetaTransaction* transaction);

// Social Recovery functions
SocialRecovery* social_recovery_create(const char* wallet_address, SocialRecoveryType recovery_type, const char* identifier);
void social_recovery_destroy(SocialRecovery* recovery);
bool social_recovery_generate_code(SocialRecovery* recovery);
bool social_recovery_verify_code(SocialRecovery* recovery, const char* code);
bool social_recovery_generate_backup_codes(SocialRecovery* recovery);
bool social_recovery_use_backup_code(SocialRecovery* recovery, const char* backup_code);
bool social_recovery_recover_wallet(SocialRecovery* recovery, const char* new_owner_address);
bool social_recovery_is_code_valid(SocialRecovery* recovery);
bool social_recovery_is_verified(SocialRecovery* recovery);
uint8_t social_recovery_get_attempts_remaining(SocialRecovery* recovery);
time_t social_recovery_get_code_expiry(SocialRecovery* recovery);

// Gasless Transaction functions
GaslessTransaction* gasless_transaction_create(const char* user_address, const char* sponsor_address, uint64_t gas_cost);
void gasless_transaction_destroy(GaslessTransaction* transaction);
bool gasless_transaction_set_payment_token(GaslessTransaction* transaction, const char* token_address, uint64_t amount);
bool gasless_transaction_set_payment_deadline(GaslessTransaction* transaction, time_t deadline);
bool gasless_transaction_pay_gas(GaslessTransaction* transaction, const char* payment_signature);
bool gasless_transaction_execute(GaslessTransaction* transaction);
bool gasless_transaction_is_paid(GaslessTransaction* transaction);
bool gasless_transaction_is_expired(GaslessTransaction* transaction);
uint64_t gasless_transaction_get_payment_amount(GaslessTransaction* transaction);
char* gasless_transaction_get_payment_token(GaslessTransaction* transaction);

// Batch Transaction functions
BatchTransaction* batch_transaction_create(const char* wallet_address);
void batch_transaction_destroy(BatchTransaction* batch);
bool batch_transaction_add_transaction(BatchTransaction* batch, const MetaTransaction* transaction);
bool batch_transaction_remove_transaction(BatchTransaction* batch, const char* transaction_id);
bool batch_transaction_sign_batch(BatchTransaction* batch, const char* private_key);
bool batch_transaction_validate_batch(BatchTransaction* batch);
bool batch_transaction_execute_batch(BatchTransaction* batch);
size_t batch_transaction_get_transaction_count(BatchTransaction* batch);
uint64_t batch_transaction_calculate_total_gas(BatchTransaction* batch);
bool batch_transaction_is_executed(BatchTransaction* batch);
char* batch_transaction_get_id(BatchTransaction* batch);

// Account Abstraction System functions
AccountAbstractionSystem* account_abstraction_system_create(void);
void account_abstraction_system_destroy(AccountAbstractionSystem* system);
bool account_abstraction_system_initialize(AccountAbstractionSystem* system);
bool account_abstraction_system_create_wallet(AccountAbstractionSystem* system, const char* owner_address, AccountType account_type);
SmartContractWallet* account_abstraction_system_get_wallet(AccountAbstractionSystem* system, const char* wallet_address);
bool account_abstraction_system_execute_meta_transaction(AccountAbstractionSystem* system, const MetaTransaction* transaction);
bool account_abstraction_system_execute_batch_transaction(AccountAbstractionSystem* system, const BatchTransaction* batch);
bool account_abstraction_system_enable_social_recovery(AccountAbstractionSystem* system, const char* wallet_address, SocialRecoveryType recovery_type, const char* identifier);
bool account_abstraction_system_recover_wallet(AccountAbstractionSystem* system, const char* wallet_address, const char* recovery_code);
bool account_abstraction_system_sponsor_gas(AccountAbstractionSystem* system, const char* sponsor_address, const GaslessTransaction* transaction);
bool account_abstraction_system_relay_transaction(AccountAbstractionSystem* system, const char* relayer_address, const MetaTransaction* transaction);
bool account_abstraction_system_validate_account(AccountAbstractionSystem* system, const char* account_address);
bool account_abstraction_system_get_account_type(AccountAbstractionSystem* system, const char* account_address, AccountType* account_type);
size_t account_abstraction_system_get_wallet_count(AccountAbstractionSystem* system);
char** account_abstraction_system_get_all_wallets(AccountAbstractionSystem* system, size_t* count);
bool account_abstraction_system_is_wallet_initialized(AccountAbstractionSystem* system, const char* wallet_address);
bool account_abstraction_system_activate_wallet(AccountAbstractionSystem* system, const char* wallet_address);
bool account_abstraction_system_deactivate_wallet(AccountAbstractionSystem* system, const char* wallet_address);

// Utility functions
bool account_abstraction_validate_address(const char* address);
bool account_abstraction_validate_signature(const char* message, const char* signature, const char* address);
char* account_abstraction_generate_wallet_address(const char* owner_address, uint64_t nonce);
char* account_abstraction_calculate_transaction_hash(const MetaTransaction* transaction);
char* account_abstraction_calculate_batch_hash(const BatchTransaction* batch);
uint64_t account_abstraction_estimate_gas_cost(const MetaTransaction* transaction);
bool account_abstraction_is_contract_address(const char* address);
char* account_abstraction_get_implementation_address(const char* wallet_address);
bool account_abstraction_upgrade_wallet(const char* wallet_address, const char* new_implementation_address);

// Configuration functions
void account_abstraction_system_set_gas_price_threshold(AccountAbstractionSystem* system, uint64_t threshold);
void account_abstraction_system_set_max_batch_size(AccountAbstractionSystem* system, size_t max_size);
void account_abstraction_system_set_social_recovery_timeout(AccountAbstractionSystem* system, time_t timeout);
void account_abstraction_system_enable_quantum_safe_accounts(AccountAbstractionSystem* system, bool enable);
void account_abstraction_system_set_recovery_threshold(AccountAbstractionSystem* system, uint8_t threshold);

// Statistics functions
size_t account_abstraction_system_get_total_wallets(AccountAbstractionSystem* system);
size_t account_abstraction_system_get_active_wallets(AccountAbstractionSystem* system);
size_t account_abstraction_system_get_total_transactions(AccountAbstractionSystem* system);
size_t account_abstraction_system_get_total_batches(AccountAbstractionSystem* system);
uint64_t account_abstraction_system_get_total_gas_saved(AccountAbstractionSystem* system);
double account_abstraction_system_get_average_gas_cost(AccountAbstractionSystem* system);
char* account_abstraction_system_generate_report(AccountAbstractionSystem* system);

#endif // ACCOUNT_ABSTRACTION_H
