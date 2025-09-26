/*
==============================================
 File:        mev_protection.h
 Author:      Irfan Gedik
 Created:     26.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse MEV Protection System Header
   
   Header file for the MEV protection system including:
   - Data structures and types
   - Function declarations
   - Constants and macros
   - Interface definitions

 License:
   MIT License
==============================================
*/

#ifndef MEV_PROTECTION_H
#define MEV_PROTECTION_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Forward declarations
typedef struct MEVProtectionSystem MEVProtectionSystem;
typedef struct PrivateMempool PrivateMempool;
typedef struct MEVDetector MEVDetector;
typedef struct FairOrdering FairOrdering;
typedef struct MEVRedistribution MEVRedistribution;
typedef struct SandwichProtection SandwichProtection;
typedef struct FrontRunningProtection FrontRunningProtection;

// Transaction types for MEV protection
typedef enum {
    TX_SWAP = 0,
    TX_LIQUIDITY_ADD,
    TX_LIQUIDITY_REMOVE,
    TX_STAKE,
    TX_UNSTAKE,
    TX_TRANSFER,
    TX_CONTRACT_CALL,
    TX_BATCH_TRANSACTION
} TransactionType;

// MEV attack types
typedef enum {
    MEV_SANDWICH_ATTACK = 0,
    MEV_FRONT_RUNNING,
    MEV_BACK_RUNNING,
    MEV_LIQUIDATION_FRONT_RUNNING,
    MEV_ARBITRAGE_EXTRACTION,
    MEV_DEX_MEV,
    MEV_LIQUIDITY_SNIPING,
    MEV_JIT_LIQUIDITY
} MEVAttackType;

// Transaction priority levels
typedef enum {
    PRIORITY_CRITICAL = 0,    // Emergency transactions
    PRIORITY_HIGH,            // User transactions
    PRIORITY_NORMAL,          // Standard transactions
    PRIORITY_LOW,             // Batch transactions
    PRIORITY_MEV_PROTECTED    // MEV-protected transactions
} PriorityLevel;

// Transaction structure for MEV protection
typedef struct {
    char transaction_id[64];
    char user_address[42];
    TransactionType type;
    PriorityLevel priority;
    char data[1024];
    char signature[128];
    uint64_t gas_price;
    uint64_t max_fee_per_gas;
    uint64_t max_priority_fee_per_gas;
    time_t timestamp;
    time_t deadline;
    bool is_mev_protected;
    char dependencies[256];
    char bundle_id[64];
    uint64_t estimated_mev;
    uint64_t protection_fee;
} ProtectedTransaction;

// MEV detection result
typedef struct {
    bool is_mev;
    MEVAttackType attack_type;
    double confidence;
    char description[256];
    char affected_transactions[512];
    uint64_t estimated_extraction;
    char recommendation[256];
} MEVDetectionResult;

// Private Mempool functions
PrivateMempool* private_mempool_create(void);
void private_mempool_destroy(PrivateMempool* mempool);
bool private_mempool_add_transaction(PrivateMempool* mempool, const ProtectedTransaction* transaction);
bool private_mempool_remove_transaction(PrivateMempool* mempool, const char* transaction_id);
ProtectedTransaction* private_mempool_get_transactions_by_user(PrivateMempool* mempool, const char* user_address, size_t* count);
ProtectedTransaction* private_mempool_get_transactions_by_priority(PrivateMempool* mempool, PriorityLevel priority, size_t* count);
char* private_mempool_create_bundle(PrivateMempool* mempool, const char** transaction_ids, size_t count);
bool private_mempool_add_to_bundle(PrivateMempool* mempool, const char* bundle_id, const char* transaction_id);
bool private_mempool_remove_from_bundle(PrivateMempool* mempool, const char* bundle_id, const char* transaction_id);
ProtectedTransaction* private_mempool_get_bundle_transactions(PrivateMempool* mempool, const char* bundle_id, size_t* count);
bool private_mempool_enable_private_mode(PrivateMempool* mempool, const char* user_address);
bool private_mempool_disable_private_mode(PrivateMempool* mempool, const char* user_address);
bool private_mempool_is_private_mode_enabled(PrivateMempool* mempool, const char* user_address);
size_t private_mempool_get_total_transactions(PrivateMempool* mempool);
size_t private_mempool_get_protected_transactions(PrivateMempool* mempool);
double private_mempool_get_average_protection_fee(PrivateMempool* mempool);

// MEV Detection Engine functions
MEVDetector* mev_detector_create(void);
void mev_detector_destroy(MEVDetector* detector);
MEVDetectionResult* mev_detector_detect_mev(MEVDetector* detector, const ProtectedTransaction* transaction);
MEVDetectionResult* mev_detector_detect_mev_in_bundle(MEVDetector* detector, const ProtectedTransaction* transactions, size_t count);
MEVDetectionResult* mev_detector_scan_mempool(MEVDetector* detector, size_t* result_count);
bool mev_detector_detect_sandwich_attack(MEVDetector* detector, const ProtectedTransaction* transaction);
bool mev_detector_detect_front_running(MEVDetector* detector, const ProtectedTransaction* transaction);
bool mev_detector_detect_back_running(MEVDetector* detector, const ProtectedTransaction* transaction);
bool mev_detector_detect_liquidation_front_running(MEVDetector* detector, const ProtectedTransaction* transaction);
bool mev_detector_detect_arbitrage_extraction(MEVDetector* detector, const ProtectedTransaction* transaction);
bool mev_detector_detect_dex_mev(MEVDetector* detector, const ProtectedTransaction* transaction);
bool mev_detector_detect_liquidity_sniping(MEVDetector* detector, const ProtectedTransaction* transaction);
bool mev_detector_detect_jit_liquidity(MEVDetector* detector, const ProtectedTransaction* transaction);
char** mev_detector_identify_mev_patterns(MEVDetector* detector, const ProtectedTransaction* transactions, size_t count, size_t* pattern_count);
double mev_detector_calculate_mev_probability(MEVDetector* detector, const ProtectedTransaction* transaction);
char* mev_detector_generate_mev_report(MEVDetector* detector);
void mev_detector_set_detection_threshold(MEVDetector* detector, double threshold);
void mev_detector_enable_attack_type(MEVDetector* detector, MEVAttackType attack_type);
void mev_detector_disable_attack_type(MEVDetector* detector, MEVAttackType attack_type);

// Fair Transaction Ordering functions
FairOrdering* fair_ordering_create(void);
void fair_ordering_destroy(FairOrdering* ordering);
ProtectedTransaction* fair_ordering_order_by_timestamp(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
ProtectedTransaction* fair_ordering_order_by_priority(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
ProtectedTransaction* fair_ordering_order_by_fairness(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
ProtectedTransaction* fair_ordering_order_by_mev_protection(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
ProtectedTransaction* fair_ordering_time_weighted_fair_ordering(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
ProtectedTransaction* fair_ordering_priority_based_ordering(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
ProtectedTransaction* fair_ordering_random_ordering(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
ProtectedTransaction* fair_ordering_commitment_based_ordering(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
bool fair_ordering_validate_ordering(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
double fair_ordering_calculate_ordering_fairness(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
char* fair_ordering_generate_ordering_report(FairOrdering* ordering, const ProtectedTransaction* transactions, size_t count);
void fair_ordering_set_ordering_strategy(FairOrdering* ordering, const char* strategy);
void fair_ordering_set_fairness_weight(FairOrdering* ordering, double weight);
void fair_ordering_set_priority_weight(FairOrdering* ordering, double weight);

// MEV Redistribution System functions
MEVRedistribution* mev_redistribution_create(void);
void mev_redistribution_destroy(MEVRedistribution* redistribution);
bool mev_redistribution_redistribute_mev(MEVRedistribution* redistribution, const char* block_hash, uint64_t total_mev);
bool mev_redistribution_distribute_to_users(MEVRedistribution* redistribution, const char** user_addresses, const uint64_t* contributions, size_t count);
bool mev_redistribution_distribute_to_validators(MEVRedistribution* redistribution, const char** validator_addresses, const uint64_t* contributions, size_t count);
bool mev_redistribution_distribute_to_treasury(MEVRedistribution* redistribution, uint64_t amount);
bool mev_redistribution_proportional_redistribution(MEVRedistribution* redistribution, const char** addresses, const uint64_t* contributions, size_t count);
bool mev_redistribution_equal_redistribution(MEVRedistribution* redistribution, const char** recipients, size_t count);
bool mev_redistribution_weighted_redistribution(MEVRedistribution* redistribution, const char** addresses, const double* weights, size_t count);
bool mev_redistribution_lottery_based_redistribution(MEVRedistribution* redistribution, const char** participants, size_t count);
uint64_t mev_redistribution_calculate_user_reward(MEVRedistribution* redistribution, const char* user_address, uint64_t total_mev);
bool mev_redistribution_claim_mev_reward(MEVRedistribution* redistribution, const char* user_address);
uint64_t* mev_redistribution_get_user_mev_rewards(MEVRedistribution* redistribution, const char** user_addresses, size_t count);
uint64_t mev_redistribution_get_total_mev_redistributed(MEVRedistribution* redistribution);
double mev_redistribution_get_average_user_reward(MEVRedistribution* redistribution);
size_t mev_redistribution_get_total_redistributions(MEVRedistribution* redistribution);

// Sandwich Attack Protection functions
SandwichProtection* sandwich_protection_create(void);
void sandwich_protection_destroy(SandwichProtection* protection);
bool sandwich_protection_detect_sandwich_attack(SandwichProtection* protection, const ProtectedTransaction* transaction);
bool sandwich_protection_detect_sandwich_pattern(SandwichProtection* protection, const ProtectedTransaction* transactions, size_t count);
char** sandwich_protection_identify_sandwich_targets(SandwichProtection* protection, const ProtectedTransaction* transaction, size_t* target_count);
bool sandwich_protection_protect_from_sandwich(SandwichProtection* protection, const ProtectedTransaction* transaction);
bool sandwich_protection_implement_slippage_protection(SandwichProtection* protection, const ProtectedTransaction* transaction);
bool sandwich_protection_implement_deadline_protection(SandwichProtection* protection, const ProtectedTransaction* transaction);
bool sandwich_protection_implement_price_impact_protection(SandwichProtection* protection, const ProtectedTransaction* transaction);
bool sandwich_protection_implement_twap_protection(SandwichProtection* protection, const ProtectedTransaction* transaction);
bool sandwich_protection_implement_private_mempool_protection(SandwichProtection* protection, const ProtectedTransaction* transaction);
bool sandwich_protection_implement_commitment_scheme_protection(SandwichProtection* protection, const ProtectedTransaction* transaction);
void sandwich_protection_set_max_slippage(SandwichProtection* protection, double max_slippage);
void sandwich_protection_set_max_price_impact(SandwichProtection* protection, double max_price_impact);
void sandwich_protection_set_protection_deadline(SandwichProtection* protection, time_t deadline);

// Front Running Protection functions
FrontRunningProtection* front_running_protection_create(void);
void front_running_protection_destroy(FrontRunningProtection* protection);
bool front_running_protection_detect_front_running(FrontRunningProtection* protection, const ProtectedTransaction* transaction);
bool front_running_protection_detect_front_run_attempt(FrontRunningProtection* protection, const ProtectedTransaction* transactions, size_t count);
char** front_running_protection_identify_front_run_targets(FrontRunningProtection* protection, const ProtectedTransaction* transaction, size_t* target_count);
bool front_running_protection_protect_from_front_running(FrontRunningProtection* protection, const ProtectedTransaction* transaction);
bool front_running_protection_implement_commitment_scheme(FrontRunningProtection* protection, const ProtectedTransaction* transaction);
bool front_running_protection_implement_private_execution(FrontRunningProtection* protection, const ProtectedTransaction* transaction);
bool front_running_protection_implement_delayed_revelation(FrontRunningProtection* protection, const ProtectedTransaction* transaction);
bool front_running_protection_implement_zero_knowledge_proofs(FrontRunningProtection* protection, const ProtectedTransaction* transaction);
bool front_running_protection_implement_homomorphic_encryption(FrontRunningProtection* protection, const ProtectedTransaction* transaction);
bool front_running_protection_implement_secure_multi_party_computation(FrontRunningProtection* protection, const ProtectedTransaction* transaction);
void front_running_protection_set_commitment_delay(FrontRunningProtection* protection, time_t delay);
void front_running_protection_set_revelation_delay(FrontRunningProtection* protection, time_t delay);
void front_running_protection_enable_zero_knowledge_protection(FrontRunningProtection* protection, bool enable);

// Main MEV Protection System functions
MEVProtectionSystem* mev_protection_system_create(void);
void mev_protection_system_destroy(MEVProtectionSystem* system);
bool mev_protection_system_protect_transaction(MEVProtectionSystem* system, const ProtectedTransaction* transaction);
bool mev_protection_system_process_transaction_bundle(MEVProtectionSystem* system, const ProtectedTransaction* transactions, size_t count);
bool mev_protection_system_execute_mev_protection(MEVProtectionSystem* system, const char* block_hash);
bool mev_protection_system_start_mev_protection(MEVProtectionSystem* system);
bool mev_protection_system_stop_mev_protection(MEVProtectionSystem* system);
bool mev_protection_system_is_mev_protection_active(MEVProtectionSystem* system);
void mev_protection_system_set_protection_level(MEVProtectionSystem* system, const char* level);
void mev_protection_system_enable_mev_redistribution(MEVProtectionSystem* system, bool enable);
void mev_protection_system_enable_sandwich_protection(MEVProtectionSystem* system, bool enable);
void mev_protection_system_enable_front_running_protection(MEVProtectionSystem* system, bool enable);
char* mev_protection_system_generate_mev_protection_report(MEVProtectionSystem* system);
char* mev_protection_system_generate_attack_prevention_report(MEVProtectionSystem* system);
uint64_t mev_protection_system_get_total_mev_prevented(MEVProtectionSystem* system);
uint64_t mev_protection_system_get_total_mev_redistributed(MEVProtectionSystem* system);
size_t mev_protection_system_get_total_attacks_prevented(MEVProtectionSystem* system);

// Component access functions
PrivateMempool* mev_protection_system_get_private_mempool(MEVProtectionSystem* system);
MEVDetector* mev_protection_system_get_mev_detector(MEVProtectionSystem* system);
FairOrdering* mev_protection_system_get_fair_ordering(MEVProtectionSystem* system);
MEVRedistribution* mev_protection_system_get_mev_redistribution(MEVProtectionSystem* system);
SandwichProtection* mev_protection_system_get_sandwich_protection(MEVProtectionSystem* system);
FrontRunningProtection* mev_protection_system_get_front_running_protection(MEVProtectionSystem* system);

#endif // MEV_PROTECTION_H
