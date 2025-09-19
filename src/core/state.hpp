#pragma once

#include "common/types.hpp"
#include "common/bytes.hpp"
#include "core/tx.hpp"
#include "core/account.hpp"
#include "kv/ikv.hpp"
#include <unordered_map>
#include <memory>

namespace usdtgverse::core {

/**
 * WorldState - The Heart of USDTgVerse Blockchain
 * 
 * Manages the global state of the blockchain including:
 * - Account balances (USDTg native + multi-asset)
 * - Nonces for replay protection
 * - OdixPay++ state (invoices, escrows, subscriptions)
 * - Spending limits and controls
 * - Fee sponsorships
 * 
 * Features:
 * - Deterministic state transitions
 * - Atomic transaction execution
 * - Gas/Weight metering
 * - State root calculation (Merkle)
 * - Rollback capability
 */

// ============================================================================
// EXECUTION RESULT
// ============================================================================

enum class ExecutionResult {
    SUCCESS = 0,
    INSUFFICIENT_FUNDS,
    INVALID_NONCE,
    INVALID_SIGNATURE,
    ACCOUNT_NOT_FOUND,
    INVALID_MESSAGE,
    GAS_LIMIT_EXCEEDED,
    SPENDING_LIMIT_EXCEEDED,
    INVALID_INVOICE,
    INVALID_ESCROW,
    INVALID_SUBSCRIPTION,
    PERMISSION_DENIED,
    STATE_ERROR,
    UNKNOWN_ERROR
};

struct ExecutionContext {
    Timestamp block_time;
    BlockHeight block_height;
    Address block_proposer;
    uint64_t gas_used;
    uint64_t gas_limit;
    
    ExecutionContext() : block_time(0), block_height(0), gas_used(0), gas_limit(0) {}
    ExecutionContext(Timestamp time, BlockHeight height, Address proposer, uint64_t limit)
        : block_time(time), block_height(height), block_proposer(proposer), gas_used(0), gas_limit(limit) {}
};

struct TxExecutionResult {
    ExecutionResult result;
    uint64_t gas_used;
    std::string error_message;
    std::vector<std::string> events; // Event logs
    
    TxExecutionResult() : result(ExecutionResult::SUCCESS), gas_used(0) {}
    TxExecutionResult(ExecutionResult res, uint64_t gas = 0, const std::string& error = "")
        : result(res), gas_used(gas), error_message(error) {}
    
    bool is_success() const { return result == ExecutionResult::SUCCESS; }
    std::string to_string() const;
};

// ============================================================================
// WORLD STATE
// ============================================================================

class WorldState {
private:
    std::shared_ptr<kv::IKV> kv_store_;
    
    // State caches (for performance)
    mutable std::unordered_map<Address, Account> account_cache_;
    mutable std::unordered_map<std::string, std::string> state_cache_;
    
    // State tracking
    StateRoot current_state_root_;
    bool dirty_;
    
    // OdixPay++ state managers
    std::unique_ptr<odixpay::OdixPayState> odixpay_state_;
    
public:
    explicit WorldState(std::shared_ptr<kv::IKV> kv_store);
    ~WorldState();
    
    // ========================================================================
    // ACCOUNT MANAGEMENT
    // ========================================================================
    
    // Account operations
    bool account_exists(const Address& addr) const;
    Account get_account(const Address& addr) const;
    void set_account(const Address& addr, const Account& account);
    void create_account(const Address& addr);
    void delete_account(const Address& addr);
    
    // Balance operations
    Amount get_balance(const Address& addr, AssetID denom_id = USDTG_DENOM_ID) const;
    void set_balance(const Address& addr, AssetID denom_id, Amount amount);
    void add_balance(const Address& addr, AssetID denom_id, Amount amount);
    bool subtract_balance(const Address& addr, AssetID denom_id, Amount amount);
    
    // USDTg specific operations
    Amount get_usdtg_balance(const Address& addr) const;
    void mint_usdtg(const Address& addr, Amount amount);
    bool burn_usdtg(const Address& addr, Amount amount);
    
    // Nonce management
    Nonce get_nonce(const Address& addr) const;
    void set_nonce(const Address& addr, Nonce nonce);
    void increment_nonce(const Address& addr);
    
    // ========================================================================
    // TRANSACTION EXECUTION
    // ========================================================================
    
    // Main transaction execution
    TxExecutionResult execute_transaction(const Tx& tx, const ExecutionContext& ctx);
    
    // Message execution
    TxExecutionResult execute_transfer(const Msg& msg, const Address& sender, const ExecutionContext& ctx);
    TxExecutionResult execute_multisend(const Msg& msg, const Address& sender, const ExecutionContext& ctx);
    
    // OdixPay++ message execution
    TxExecutionResult execute_create_invoice(const Msg& msg, const Address& sender, const ExecutionContext& ctx);
    TxExecutionResult execute_pay_invoice(const Msg& msg, const Address& sender, const ExecutionContext& ctx);
    TxExecutionResult execute_create_escrow(const Msg& msg, const Address& sender, const ExecutionContext& ctx);
    TxExecutionResult execute_release_escrow(const Msg& msg, const Address& sender, const ExecutionContext& ctx);
    TxExecutionResult execute_create_subscription(const Msg& msg, const Address& sender, const ExecutionContext& ctx);
    TxExecutionResult execute_set_spending_limit(const Msg& msg, const Address& sender, const ExecutionContext& ctx);
    TxExecutionResult execute_batch_pay(const Msg& msg, const Address& sender, const ExecutionContext& ctx);
    
    // ========================================================================
    // VALIDATION
    // ========================================================================
    
    // Transaction validation
    bool validate_transaction(const Tx& tx, const ExecutionContext& ctx) const;
    bool validate_signatures(const Tx& tx) const;
    bool validate_nonces(const Tx& tx) const;
    bool validate_balances(const Tx& tx) const;
    bool validate_spending_limits(const Tx& tx, const ExecutionContext& ctx) const;
    
    // Message validation
    bool validate_message(const Msg& msg, const Address& sender) const;
    
    // ========================================================================
    // STATE MANAGEMENT
    // ========================================================================
    
    // State root calculation
    StateRoot calculate_state_root() const;
    StateRoot get_current_state_root() const { return current_state_root_; }
    void update_state_root();
    
    // State persistence
    bool commit_state();
    bool rollback_state();
    void clear_cache();
    
    // State queries
    bool is_dirty() const { return dirty_; }
    size_t get_account_count() const;
    std::vector<Address> get_all_accounts() const;
    
    // ========================================================================
    // STATISTICS & MONITORING
    // ========================================================================
    
    // Supply statistics
    Amount get_total_usdtg_supply() const;
    Amount get_circulating_usdtg_supply() const;
    std::unordered_map<AssetID, Amount> get_all_asset_supplies() const;
    
    // Account statistics
    size_t get_active_account_count() const;
    std::vector<std::pair<Address, Amount>> get_top_holders(size_t limit = 100) const;
    
    // OdixPay++ statistics
    size_t get_active_invoice_count() const;
    size_t get_active_escrow_count() const;
    size_t get_active_subscription_count() const;
    Amount get_total_escrowed_amount() const;
    
    // State size and performance
    size_t get_state_size_bytes() const;
    double get_cache_hit_ratio() const;
    void optimize_state();
    
    // ========================================================================
    // DEBUGGING & TESTING
    // ========================================================================
    
    std::string get_state_summary() const;
    std::string get_account_summary(const Address& addr) const;
    void dump_state_to_json(const std::string& filename) const;
    bool load_state_from_json(const std::string& filename);
    
    // Genesis state
    void initialize_genesis_state(const std::vector<std::pair<Address, Amount>>& initial_balances);
    
private:
    // ========================================================================
    // INTERNAL HELPERS
    // ========================================================================
    
    // Key generation for KV store
    std::string account_key(const Address& addr) const;
    std::string balance_key(const Address& addr, AssetID denom_id) const;
    std::string nonce_key(const Address& addr) const;
    std::string state_key(const std::string& key) const;
    
    // Cache management
    void cache_account(const Address& addr, const Account& account) const;
    void invalidate_account_cache(const Address& addr);
    void flush_cache();
    
    // Gas/Weight calculation
    uint64_t calculate_base_gas(const Tx& tx) const;
    uint64_t calculate_message_gas(const Msg& msg) const;
    uint64_t calculate_signature_gas(size_t signature_count) const;
    
    // Fee handling
    bool deduct_fees(const Address& payer, const Fee& fee);
    void distribute_fees(const Fee& fee, const Address& proposer);
    
    // Event logging
    void emit_event(const std::string& event_type, const std::string& data);
    
    // State validation
    bool validate_state_consistency() const;
    void repair_state_if_needed();
};

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Result helpers
std::string execution_result_to_string(ExecutionResult result);
bool is_recoverable_error(ExecutionResult result);

// State helpers
Address derive_contract_address(const Address& creator, Nonce nonce);
std::string format_state_key(const std::string& prefix, const std::string& suffix);

// Genesis helpers
std::vector<std::pair<Address, Amount>> create_genesis_distribution(Amount total_supply, size_t validator_count = 21);
Address create_genesis_validator_address(size_t validator_index);

// Testing helpers
WorldState create_test_state();
void fund_test_account(WorldState& state, const Address& addr, Amount usdtg_amount);
Tx create_test_transfer(const Address& from, const Address& to, Amount amount, Nonce nonce);

} // namespace usdtgverse::core

