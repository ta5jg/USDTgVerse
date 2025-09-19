#pragma once

#include "common/types.hpp"
#include "core/tx.hpp"
#include <unordered_map>
#include <vector>

namespace usdtgverse {

// ============================================================================
// ACCOUNT MODEL
// ============================================================================

struct SpendingLimit {
    Coin daily_limit;
    Coin monthly_limit;
    Amount daily_spent;
    Amount monthly_spent;
    Timestamp last_daily_reset;
    Timestamp last_monthly_reset;
    std::vector<Address> authorized_merchants; // Whitelist for unlimited spending
    
    SpendingLimit() : daily_spent(0), monthly_spent(0), last_daily_reset(0), last_monthly_reset(0) {}
    
    bool check_and_update(const Coin& amount, const Address& recipient, Timestamp current_time);
    void reset_daily_if_needed(Timestamp current_time);
    void reset_monthly_if_needed(Timestamp current_time);
    bool is_merchant_authorized(const Address& merchant) const;
};

struct Account {
    // Basic account data
    Nonce nonce;
    std::unordered_map<AssetID, Amount> balances; // Multi-asset support
    Timestamp last_activity;
    
    // OdixPay++ features
    std::optional<SpendingLimit> spending_limits;
    std::vector<Hash> active_invoices;
    std::vector<Hash> active_escrows;
    std::vector<Hash> active_subscriptions;
    
    // Staking (for validators)
    Amount staked_amount;
    std::vector<Address> delegated_to;
    
    // Metadata
    std::string account_type; // "user", "merchant", "validator", "treasury"
    std::unordered_map<std::string, std::string> metadata;
    
    Account() : nonce(0), last_activity(0), staked_amount(0) {}
    
    // Balance operations
    Amount get_balance(AssetID denom_id = USDTG_DENOM_ID) const;
    void set_balance(AssetID denom_id, Amount amount);
    bool has_sufficient_balance(const Coin& required) const;
    bool deduct_balance(const Coin& amount);
    void add_balance(const Coin& amount);
    
    // Nonce management
    void increment_nonce() { nonce++; last_activity = current_timestamp_ms(); }
    bool is_valid_nonce(Nonce tx_nonce) const { return tx_nonce == nonce + 1; }
    
    // Spending limits
    void set_spending_limits(const Coin& daily, const Coin& monthly);
    bool check_spending_limit(const Coin& amount, const Address& recipient, Timestamp current_time);
    void add_authorized_merchant(const Address& merchant);
    void remove_authorized_merchant(const Address& merchant);
    
    // OdixPay++ state
    void add_invoice(const Hash& invoice_id) { active_invoices.push_back(invoice_id); }
    void remove_invoice(const Hash& invoice_id);
    void add_escrow(const Hash& escrow_id) { active_escrows.push_back(escrow_id); }
    void remove_escrow(const Hash& escrow_id);
    void add_subscription(const Hash& subscription_id) { active_subscriptions.push_back(subscription_id); }
    void remove_subscription(const Hash& subscription_id);
    
    // Validation
    bool validate() const;
    
    // Serialization
    Bytes encode() const;
    static std::optional<Account> decode(const Bytes& data);
    
    // Utility
    size_t get_size() const;
    std::string to_string() const;
    void print() const;
};

// ============================================================================
// ACCOUNT MANAGER
// ============================================================================

class AccountManager {
private:
    class IKV* storage_; // Key-value storage backend
    mutable std::shared_mutex accounts_mutex_;
    
    // Cache for frequently accessed accounts
    mutable std::unordered_map<Address, std::shared_ptr<Account>> account_cache_;
    static constexpr size_t MAX_CACHE_SIZE = 10000;
    
    // Statistics
    mutable std::atomic<size_t> cache_hits_{0};
    mutable std::atomic<size_t> cache_misses_{0};

public:
    explicit AccountManager(IKV* storage);
    ~AccountManager() = default;
    
    // Account operations
    Result<std::shared_ptr<Account>> get_account(const Address& address) const;
    Result<void> set_account(const Address& address, const Account& account);
    Result<void> create_account(const Address& address, const Account& initial_state = Account{});
    bool account_exists(const Address& address) const;
    
    // Balance operations
    Amount get_balance(const Address& address, AssetID denom_id = USDTG_DENOM_ID) const;
    Result<void> transfer(const Address& from, const Address& to, const Coin& amount);
    Result<void> mint(const Address& to, const Coin& amount);
    Result<void> burn(const Address& from, const Coin& amount);
    
    // Nonce management
    Nonce get_nonce(const Address& address) const;
    Result<void> increment_nonce(const Address& address);
    bool validate_nonce(const Address& address, Nonce tx_nonce) const;
    
    // OdixPay++ operations
    Result<void> create_invoice(const Address& merchant, const Address& payer, const Coin& amount, Timestamp due_date, const std::string& metadata);
    Result<void> pay_invoice(const Hash& invoice_id, const Address& payer, const Coin& amount);
    Result<void> create_escrow(const Address& buyer, const Address& seller, const Coin& amount, const std::optional<Address>& arbiter, Timestamp timeout);
    Result<void> release_escrow(const Hash& escrow_id, const Address& releaser, const Address& recipient);
    
    // Spending limits
    Result<void> set_spending_limits(const Address& address, const Coin& daily, const Coin& monthly);
    bool check_spending_limit(const Address& address, const Coin& amount, const Address& recipient) const;
    
    // Batch operations
    Result<void> apply_transaction(const Transaction& tx);
    Result<void> revert_transaction(const Transaction& tx);
    
    // Statistics
    size_t get_account_count() const;
    Amount get_total_supply(AssetID denom_id = USDTG_DENOM_ID) const;
    Amount get_circulating_supply(AssetID denom_id = USDTG_DENOM_ID) const;
    
    // Cache management
    void clear_cache();
    void warm_cache(const std::vector<Address>& addresses);
    std::pair<size_t, size_t> get_cache_stats() const; // hits, misses
    
    // Utility
    std::vector<Address> get_all_addresses() const;
    std::vector<std::pair<Address, Account>> get_accounts_with_balance(AssetID denom_id = USDTG_DENOM_ID) const;
    
    // Debug
    void print_stats() const;
    void validate_all_accounts() const;
};

// ============================================================================
// GENESIS ACCOUNT SETUP
// ============================================================================

struct GenesisAccount {
    Address address;
    Account account;
    std::string name; // Human-readable name
    
    GenesisAccount(const Address& addr, const Account& acc, const std::string& n)
        : address(addr), account(acc), name(n) {}
};

class GenesisAccountBuilder {
public:
    static std::vector<GenesisAccount> create_genesis_accounts();
    static Address create_address(const std::string& seed);
    static Account create_treasury_account(Amount usdtg_amount);
    static Account create_user_account(Amount usdtg_amount);
    static Account create_validator_account(Amount usdtg_amount, Amount stake_amount);
    static Account create_merchant_account(Amount usdtg_amount);
};

} // namespace usdtgverse

