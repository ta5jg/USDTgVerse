#pragma once

#include "common/types.hpp"
#include "common/bytes.hpp"
#include <vector>
#include <memory>

namespace usdtgverse {

// ============================================================================
// MESSAGE TYPES (OdixPay++ Native Messages)
// ============================================================================

enum class MsgType : uint8_t {
    // Basic transfers
    TRANSFER = 0,
    MULTI_TRANSFER = 1,
    
    // OdixPay++ Messages
    INVOICE = 10,
    PAY_INVOICE = 11,
    ESCROW_OPEN = 12,
    ESCROW_RELEASE = 13,
    SUBSCRIBE = 14,
    BATCH_PAY = 15,
    SET_SPENDING_LIMIT = 16,
    
    // Staking & Governance
    DELEGATE = 20,
    UNDELEGATE = 21,
    VOTE = 22,
    
    // System messages
    VALIDATOR_UPDATE = 30,
    PARAM_CHANGE = 31,
};

// Base message interface
struct IMsg {
    virtual ~IMsg() = default;
    virtual MsgType get_type() const = 0;
    virtual Bytes encode() const = 0;
    virtual bool validate() const = 0;
    virtual uint32_t get_weight() const = 0; // Gas/weight calculation
};

// ============================================================================
// BASIC TRANSFER MESSAGES
// ============================================================================

struct MsgTransfer : public IMsg {
    Address from;
    Address to;
    Coin amount;
    std::string memo;
    
    MsgTransfer() = default;
    MsgTransfer(const Address& f, const Address& t, const Coin& amt, const std::string& m = "")
        : from(f), to(t), amount(amt), memo(m) {}
    
    MsgType get_type() const override { return MsgType::TRANSFER; }
    Bytes encode() const override;
    bool validate() const override;
    uint32_t get_weight() const override { return 1000; } // Base transfer weight
};

struct MsgMultiTransfer : public IMsg {
    Address from;
    std::vector<std::pair<Address, Coin>> outputs;
    std::string memo;
    
    MsgType get_type() const override { return MsgType::MULTI_TRANSFER; }
    Bytes encode() const override;
    bool validate() const override;
    uint32_t get_weight() const override { return 1000 + outputs.size() * 500; }
};

// ============================================================================
// ODIXPAY++ MESSAGES
// ============================================================================

struct MsgInvoice : public IMsg {
    Address merchant;
    Address payer;
    Coin amount;
    Timestamp due_date;
    std::string metadata; // JSON metadata for invoice details
    
    MsgType get_type() const override { return MsgType::INVOICE; }
    Bytes encode() const override;
    bool validate() const override;
    uint32_t get_weight() const override { return 2000; }
};

struct MsgPayInvoice : public IMsg {
    Hash invoice_id;
    Address payer;
    Coin amount; // May be partial payment
    
    MsgType get_type() const override { return MsgType::PAY_INVOICE; }
    Bytes encode() const override;
    bool validate() const override;
    uint32_t get_weight() const override { return 1500; }
};

struct MsgEscrowOpen : public IMsg {
    Address buyer;
    Address seller;
    Coin amount;
    std::optional<Address> arbiter;
    Timestamp timeout;
    std::string terms; // Escrow terms
    
    MsgType get_type() const override { return MsgType::ESCROW_OPEN; }
    Bytes encode() const override;
    bool validate() const override;
    uint32_t get_weight() const override { return 3000; }
};

struct MsgEscrowRelease : public IMsg {
    Hash escrow_id;
    Address releaser; // Must be buyer, seller, or arbiter
    Address recipient;
    
    MsgType get_type() const override { return MsgType::ESCROW_RELEASE; }
    Bytes encode() const override;
    bool validate() const override;
    uint32_t get_weight() const override { return 2000; }
};

struct MsgSubscribe : public IMsg {
    Address payer;
    Address merchant;
    std::string plan_id;
    uint32_t period_days;
    Coin max_amount_per_period;
    
    MsgType get_type() const override { return MsgType::SUBSCRIBE; }
    Bytes encode() const override;
    bool validate() const override;
    uint32_t get_weight() const override { return 2500; }
};

struct MsgBatchPay : public IMsg {
    Address payer;
    std::vector<std::pair<Address, Coin>> payments;
    std::optional<Address> fee_sponsor; // Merchant can sponsor fees
    
    MsgType get_type() const override { return MsgType::BATCH_PAY; }
    Bytes encode() const override;
    bool validate() const override;
    uint32_t get_weight() const override { return 1000 + payments.size() * 300; }
};

struct MsgSetSpendingLimit : public IMsg {
    Address account;
    Coin daily_limit;
    Coin monthly_limit;
    std::vector<Address> authorized_merchants; // Whitelist
    
    MsgType get_type() const override { return MsgType::SET_SPENDING_LIMIT; }
    Bytes encode() const override;
    bool validate() const override;
    uint32_t get_weight() const override { return 1500; }
};

// ============================================================================
// TRANSACTION STRUCTURE
// ============================================================================

struct TxSignature {
    PublicKey public_key;
    Signature signature;
    
    TxSignature() = default;
    TxSignature(const PublicKey& pk, const Signature& sig) : public_key(pk), signature(sig) {}
    
    bool verify(const Hash& tx_hash) const;
    Bytes encode() const;
    static std::optional<TxSignature> decode(const Bytes& data, size_t& offset);
};

struct Transaction {
    // Transaction metadata
    ChainID chain_id;
    Nonce nonce;
    Timestamp timeout; // Transaction timeout
    
    // Messages to execute
    std::vector<std::unique_ptr<IMsg>> messages;
    
    // Fee information
    Fee fee;
    std::optional<Address> fee_payer; // For fee sponsorship
    
    // Additional data
    std::string memo;
    
    // Signatures (multisig support)
    std::vector<TxSignature> signatures;
    
    // Cached values
    mutable std::optional<Hash> cached_hash;
    mutable std::optional<Bytes> cached_encoded;
    
    Transaction() = default;
    Transaction(const ChainID& chain, Nonce n) : chain_id(chain), nonce(n), timeout(0) {}
    
    // Hash calculation (deterministic)
    Hash calculate_hash() const;
    const Hash& get_hash() const;
    void invalidate_cache();
    
    // Encoding/Decoding (USDTg-Bin format)
    Bytes encode() const;
    static std::optional<Transaction> decode(const Bytes& data);
    
    // Validation
    bool validate() const;
    bool verify_signatures() const;
    bool check_fee() const;
    bool check_timeout(Timestamp current_time) const;
    
    // Utility
    size_t get_size() const;
    uint32_t calculate_weight() const;
    uint64_t calculate_min_fee() const;
    
    // Message helpers
    void add_message(std::unique_ptr<IMsg> msg);
    template<typename T, typename... Args>
    void add_message(Args&&... args) {
        add_message(std::make_unique<T>(std::forward<Args>(args)...));
    }
    
    // Signature helpers
    void add_signature(const TxSignature& sig);
    void clear_signatures();
    bool is_signed() const { return !signatures.empty(); }
    
    // Fee helpers
    void set_fee(uint64_t max_fee, uint64_t tip = 0);
    void set_fee_sponsor(const Address& sponsor);
    bool has_fee_sponsor() const { return fee_payer.has_value(); }
    
    // Debug
    std::string to_string() const;
    void print() const;
};

// ============================================================================
// TRANSACTION FACTORY
// ============================================================================

class TxBuilder {
private:
    Transaction tx_;

public:
    explicit TxBuilder(const ChainID& chain_id);
    
    // Basic setup
    TxBuilder& set_nonce(Nonce nonce);
    TxBuilder& set_timeout(Timestamp timeout);
    TxBuilder& set_memo(const std::string& memo);
    TxBuilder& set_fee(uint64_t max_fee, uint64_t tip = 0);
    TxBuilder& set_fee_sponsor(const Address& sponsor);
    
    // Add messages
    TxBuilder& transfer(const Address& from, const Address& to, const Coin& amount, const std::string& memo = "");
    TxBuilder& invoice(const Address& merchant, const Address& payer, const Coin& amount, Timestamp due_date);
    TxBuilder& pay_invoice(const Hash& invoice_id, const Address& payer, const Coin& amount);
    TxBuilder& escrow_open(const Address& buyer, const Address& seller, const Coin& amount, Timestamp timeout);
    TxBuilder& escrow_release(const Hash& escrow_id, const Address& releaser, const Address& recipient);
    TxBuilder& subscribe(const Address& payer, const Address& merchant, const std::string& plan_id, uint32_t period_days, const Coin& max_amount);
    TxBuilder& batch_pay(const Address& payer, const std::vector<std::pair<Address, Coin>>& payments);
    TxBuilder& set_spending_limit(const Address& account, const Coin& daily_limit, const Coin& monthly_limit);
    
    // Finalize
    Transaction build();
    Transaction& get_transaction() { return tx_; }
    const Transaction& get_transaction() const { return tx_; }
};

// ============================================================================
// TRANSACTION UTILITIES
// ============================================================================

// Transaction validation
Result<void> validate_transaction(const Transaction& tx, Timestamp current_time);

// Fee calculation
uint64_t calculate_base_fee(uint32_t weight);
uint64_t calculate_priority_fee(uint32_t weight, double priority_multiplier = 1.0);

// Signature helpers
Hash get_signing_hash(const Transaction& tx);
bool sign_transaction(Transaction& tx, const PrivateKey& private_key);
bool verify_transaction_signatures(const Transaction& tx);

} // namespace usdtgverse

