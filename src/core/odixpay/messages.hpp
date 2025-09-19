#pragma once

#include "common/types.hpp"
#include "common/bytes.hpp"
#include "core/tx.hpp"
#include <optional>
#include <chrono>

namespace usdtgverse::odixpay {

/**
 * OdixPay++ Native Payment Messages
 * 
 * Enterprise-grade payment system built into the blockchain core.
 * All operations are deterministic and state-machine verified.
 */

// ============================================================================
// INVOICE SYSTEM
// ============================================================================

struct Invoice {
    std::string invoice_id;        // Unique invoice identifier
    Address merchant;              // Merchant receiving payment
    Address payer;                 // Expected payer (optional, can be anyone)
    Coin amount;                   // Invoice amount
    Timestamp due_date;            // Payment deadline
    std::string description;       // Invoice description
    std::string metadata;          // Additional merchant data
    bool allow_partial;            // Allow partial payments
    bool allow_overpay;            // Allow overpayments
    
    // Status (managed by state machine)
    enum Status { PENDING, PAID, EXPIRED, CANCELLED } status;
    Amount paid_amount;            // Total paid so far
    std::vector<TxHash> payments;  // Payment transaction hashes
    
    Invoice();
    bool is_valid() const;
    bool is_expired(Timestamp current_time) const;
    bool is_fully_paid() const;
    Amount remaining_amount() const;
    std::string to_string() const;
};

struct MsgCreateInvoice {
    static constexpr MsgType TYPE = MSG_INVOICE;
    
    std::string invoice_id;
    Address payer;                 // Can be zero address for "anyone can pay"
    Coin amount;
    uint64_t due_seconds;          // Seconds from now until due
    std::string description;
    std::string metadata;
    bool allow_partial;
    bool allow_overpay;
    
    Bytes encode() const;
    static std::optional<MsgCreateInvoice> decode(const Bytes& data);
    bool is_valid() const;
};

struct MsgPayInvoice {
    static constexpr MsgType TYPE = MSG_PAY_INVOICE;
    
    std::string invoice_id;
    Coin payment_amount;
    std::string memo;
    
    Bytes encode() const;
    static std::optional<MsgPayInvoice> decode(const Bytes& data);
    bool is_valid() const;
};

// ============================================================================
// ESCROW SYSTEM
// ============================================================================

struct Escrow {
    std::string escrow_id;
    Address buyer;
    Address seller;
    Address arbiter;               // Optional third-party arbiter
    Coin amount;
    Timestamp timeout;             // Auto-release timeout
    std::string terms;             // Escrow terms/conditions
    
    enum Status { ACTIVE, RELEASED_TO_SELLER, RELEASED_TO_BUYER, DISPUTED } status;
    
    Escrow();
    bool is_valid() const;
    bool is_expired(Timestamp current_time) const;
    bool can_auto_release(Timestamp current_time) const;
    std::string to_string() const;
};

struct MsgCreateEscrow {
    static constexpr MsgType TYPE = MSG_ESCROW_CREATE;
    
    std::string escrow_id;
    Address seller;
    Address arbiter;               // Zero address = no arbiter
    Coin amount;
    uint64_t timeout_seconds;      // Auto-release timeout
    std::string terms;
    
    Bytes encode() const;
    static std::optional<MsgCreateEscrow> decode(const Bytes& data);
    bool is_valid() const;
};

struct MsgReleaseEscrow {
    static constexpr MsgType TYPE = MSG_ESCROW_RELEASE;
    
    std::string escrow_id;
    Address recipient;             // buyer or seller
    std::string reason;
    
    Bytes encode() const;
    static std::optional<MsgReleaseEscrow> decode(const Bytes& data);
    bool is_valid() const;
};

// ============================================================================
// SUBSCRIPTION SYSTEM
// ============================================================================

struct Subscription {
    std::string subscription_id;
    Address payer;
    Address merchant;
    Coin amount_per_period;
    uint64_t period_seconds;       // Billing period (e.g., 30 days)
    uint64_t max_periods;          // Maximum periods (0 = unlimited)
    Timestamp next_payment;        // Next payment due
    uint64_t periods_paid;         // Periods already paid
    
    enum Status { ACTIVE, PAUSED, CANCELLED, EXPIRED } status;
    
    Subscription();
    bool is_valid() const;
    bool is_payment_due(Timestamp current_time) const;
    bool is_expired() const;
    Timestamp calculate_next_payment() const;
    std::string to_string() const;
};

struct MsgCreateSubscription {
    static constexpr MsgType TYPE = MSG_SUBSCRIBE;
    
    std::string subscription_id;
    Address merchant;
    Coin amount_per_period;
    uint64_t period_seconds;
    uint64_t max_periods;          // 0 = unlimited
    
    Bytes encode() const;
    static std::optional<MsgCreateSubscription> decode(const Bytes& data);
    bool is_valid() const;
};

struct MsgProcessSubscription {
    static constexpr MsgType TYPE = MSG_PAY_INVOICE; // Reuse existing type
    
    std::string subscription_id;
    uint64_t periods_to_pay;       // Usually 1, but can pay multiple periods
    
    Bytes encode() const;
    static std::optional<MsgProcessSubscription> decode(const Bytes& data);
    bool is_valid() const;
};

// ============================================================================
// SPENDING LIMITS
// ============================================================================

struct SpendingLimit {
    Address account;
    Coin daily_limit;
    Coin monthly_limit;
    Coin spent_today;
    Coin spent_this_month;
    Timestamp last_reset_day;
    Timestamp last_reset_month;
    
    SpendingLimit();
    bool is_valid() const;
    bool check_daily_limit(const Coin& amount, Timestamp current_time);
    bool check_monthly_limit(const Coin& amount, Timestamp current_time);
    void update_spent(const Coin& amount, Timestamp current_time);
    void reset_if_needed(Timestamp current_time);
    std::string to_string() const;
};

struct MsgSetSpendingLimit {
    static constexpr MsgType TYPE = MSG_SET_SPENDING_LIMIT;
    
    Coin daily_limit;              // 0 = no limit
    Coin monthly_limit;            // 0 = no limit
    
    Bytes encode() const;
    static std::optional<MsgSetSpendingLimit> decode(const Bytes& data);
    bool is_valid() const;
};

// ============================================================================
// BATCH PAYMENTS
// ============================================================================

struct BatchPayment {
    std::string batch_id;
    Address payer;
    std::vector<std::pair<Address, Coin>> recipients;
    Coin total_amount;
    Fee fee_per_recipient;
    std::string memo;
    
    BatchPayment();
    bool is_valid() const;
    Coin calculate_total_with_fees() const;
    uint32_t calculate_weight() const;
    std::string to_string() const;
};

struct MsgBatchPay {
    static constexpr MsgType TYPE = MSG_BATCH_PAY;
    
    std::string batch_id;
    std::vector<std::pair<Address, Coin>> recipients;
    std::string memo;
    
    Bytes encode() const;
    static std::optional<MsgBatchPay> decode(const Bytes& data);
    bool is_valid() const;
    uint32_t calculate_weight() const;
};

// ============================================================================
// FEE SPONSORSHIP
// ============================================================================

struct FeeSponsorship {
    Address sponsor;               // Who pays the fees
    Address beneficiary;           // Who benefits from sponsorship
    Coin sponsored_amount;         // Maximum sponsored per tx
    uint64_t max_transactions;     // Maximum transactions to sponsor
    uint64_t transactions_used;    // Transactions already sponsored
    Timestamp expiry;              // Sponsorship expiry
    
    FeeSponsorship();
    bool is_valid() const;
    bool is_expired(Timestamp current_time) const;
    bool can_sponsor(const Fee& fee) const;
    void use_sponsorship();
    std::string to_string() const;
};

// ============================================================================
// ODIXPAY++ STATE MANAGER
// ============================================================================

class OdixPayState {
private:
    std::unordered_map<std::string, Invoice> invoices_;
    std::unordered_map<std::string, Escrow> escrows_;
    std::unordered_map<std::string, Subscription> subscriptions_;
    std::unordered_map<Address, SpendingLimit> spending_limits_;
    std::unordered_map<std::pair<Address, Address>, FeeSponsorship> sponsorships_;

public:
    // Invoice operations
    bool create_invoice(const MsgCreateInvoice& msg, Address merchant, Timestamp current_time);
    bool pay_invoice(const MsgPayInvoice& msg, Address payer, Timestamp current_time);
    std::optional<Invoice> get_invoice(const std::string& invoice_id) const;
    
    // Escrow operations
    bool create_escrow(const MsgCreateEscrow& msg, Address buyer, Timestamp current_time);
    bool release_escrow(const MsgReleaseEscrow& msg, Address releaser, Timestamp current_time);
    std::optional<Escrow> get_escrow(const std::string& escrow_id) const;
    
    // Subscription operations
    bool create_subscription(const MsgCreateSubscription& msg, Address payer, Timestamp current_time);
    bool process_subscription(const MsgProcessSubscription& msg, Timestamp current_time);
    std::optional<Subscription> get_subscription(const std::string& subscription_id) const;
    
    // Spending limit operations
    bool set_spending_limit(const MsgSetSpendingLimit& msg, Address account);
    bool check_spending_limit(Address account, const Coin& amount, Timestamp current_time);
    void update_spending(Address account, const Coin& amount, Timestamp current_time);
    
    // Batch payment operations
    bool process_batch_payment(const MsgBatchPay& msg, Address payer, Timestamp current_time);
    
    // Fee sponsorship
    bool can_sponsor_fee(Address sponsor, Address beneficiary, const Fee& fee, Timestamp current_time) const;
    void use_fee_sponsorship(Address sponsor, Address beneficiary, const Fee& fee);
    
    // State management
    void cleanup_expired(Timestamp current_time);
    std::string get_state_summary() const;
};

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Generate unique IDs
std::string generate_invoice_id(Address merchant, Timestamp timestamp);
std::string generate_escrow_id(Address buyer, Address seller, Timestamp timestamp);
std::string generate_subscription_id(Address payer, Address merchant, Timestamp timestamp);
std::string generate_batch_id(Address payer, Timestamp timestamp);

// Validation helpers
bool is_valid_invoice_id(const std::string& id);
bool is_valid_escrow_id(const std::string& id);
bool is_valid_subscription_id(const std::string& id);
bool is_valid_batch_id(const std::string& id);

// Time utilities
Timestamp get_current_timestamp();
uint64_t days_to_seconds(uint32_t days);
uint64_t months_to_seconds(uint32_t months);

// Amount utilities
Coin parse_usdtg_amount(const std::string& amount_str);
std::string format_usdtg_amount(const Coin& coin);

} // namespace usdtgverse::odixpay

