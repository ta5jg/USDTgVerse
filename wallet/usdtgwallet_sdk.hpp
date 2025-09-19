#pragma once

#include "../src/common/types.hpp"
#include "../src/core/tx.hpp"
#include "../exchanges/dex/core/amm_engine.hpp"
#include "../exchanges/cex/core/order_book.hpp"
#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace usdtgverse {
namespace wallet {

// Wallet Types
enum class WalletType : uint8_t {
    SOFTWARE = 0,
    HARDWARE = 1,
    MULTISIG = 2,
    MPC = 3
};

enum class KeyDerivation : uint8_t {
    BIP32_ED25519 = 0,
    BIP32_SECP256K1 = 1
};

// Account Management
struct WalletAccount {
    Address address;
    PublicKey public_key;
    std::string name;
    std::string derivation_path; // e.g., "m/44'/60'/0'/0/0"
    KeyDerivation key_type;
    bool is_hardware;
    std::unordered_map<DenomId, USDTgAmount> balances;
};

// Transaction History
struct TransactionRecord {
    Hash tx_hash;
    Timestamp timestamp;
    Address from;
    Address to;
    USDTgAmount amount;
    DenomId denom;
    FeeAmount fee;
    std::string memo;
    std::string status; // "pending", "confirmed", "failed"
    uint64_t block_height;
};

// OdixPay++ Payment Features
namespace odixpay {

// Invoice System
struct Invoice {
    Hash invoice_id;
    Address merchant;
    Address payer;
    USDTgAmount amount;
    DenomId denom;
    std::string description;
    std::string reference;
    Timestamp due_date;
    Timestamp created_at;
    bool is_paid;
    Hash payment_tx_hash;
    
    // QR Code data for mobile payments
    std::string generate_qr_data() const;
};

// Escrow Service
struct EscrowContract {
    Hash escrow_id;
    Address buyer;
    Address seller;
    Address arbiter;
    USDTgAmount amount;
    DenomId denom;
    std::string description;
    Timestamp created_at;
    Timestamp release_deadline;
    bool is_released;
    bool is_disputed;
    Hash release_tx_hash;
};

// Subscription Payment
struct Subscription {
    Hash subscription_id;
    Address payer;
    Address merchant;
    std::string plan_name;
    USDTgAmount amount;
    DenomId denom;
    uint32_t interval_days; // Payment frequency
    Timestamp next_payment;
    Timestamp created_at;
    bool is_active;
    uint32_t payments_made;
};

// Spending Limits
struct SpendingLimit {
    DenomId denom;
    USDTgAmount daily_limit;
    USDTgAmount monthly_limit;
    USDTgAmount spent_today;
    USDTgAmount spent_this_month;
    Timestamp last_reset;
};

// Batch Payment
struct BatchPayment {
    Hash batch_id;
    Address payer;
    std::vector<std::pair<Address, USDTgAmount>> recipients;
    DenomId denom;
    std::string description;
    Timestamp created_at;
    std::vector<Hash> tx_hashes;
};

} // namespace odixpay

// Main USDTgWallet SDK Class
class USDTgWallet {
private:
    std::vector<WalletAccount> accounts_;
    std::string encrypted_seed_;
    WalletType wallet_type_;
    
    // Blockchain connection
    std::string rpc_endpoint_;
    
    // Exchange integrations
    std::unique_ptr<dex::AMMEngine> dex_engine_;
    std::unique_ptr<cex::TradingEngine> cex_engine_;
    
    // OdixPay++ features
    std::vector<odixpay::Invoice> invoices_;
    std::vector<odixpay::EscrowContract> escrows_;
    std::vector<odixpay::Subscription> subscriptions_;
    std::unordered_map<Address, odixpay::SpendingLimit> spending_limits_;
    
public:
    // Wallet Creation & Management
    static USDTgWallet create_new_wallet(const std::string& password, 
                                        WalletType type = WalletType::SOFTWARE);
    static USDTgWallet restore_from_seed(const std::string& seed_phrase, 
                                        const std::string& password);
    static USDTgWallet connect_hardware_wallet(const std::string& device_path);
    
    // Account Management
    WalletAccount create_account(const std::string& name, 
                                KeyDerivation key_type = KeyDerivation::BIP32_ED25519);
    std::vector<WalletAccount> get_accounts() const { return accounts_; }
    WalletAccount* get_account(const Address& address);
    bool import_private_key(const std::string& private_key_hex, const std::string& name);
    
    // Balance & Transaction Management
    bool refresh_balances();
    USDTgAmount get_balance(const Address& address, DenomId denom = 0) const;
    std::vector<TransactionRecord> get_transaction_history(const Address& address, 
                                                          size_t limit = 100) const;
    
    // Basic Transactions
    Hash send_transaction(const Address& from, const Address& to, 
                         USDTgAmount amount, DenomId denom = 0,
                         const std::string& memo = "");
    Hash send_multi_asset(const Address& from, 
                         const std::vector<std::tuple<Address, USDTgAmount, DenomId>>& transfers);
    
    // DEX Integration
    struct SwapQuote {
        DenomId token_in;
        DenomId token_out;
        USDTgAmount amount_in;
        USDTgAmount amount_out;
        USDTgAmount price_impact;
        std::vector<Hash> route;
        FeeAmount total_fees;
    };
    
    SwapQuote get_swap_quote(DenomId token_in, DenomId token_out, USDTgAmount amount_in);
    Hash execute_swap(const Address& trader, const SwapQuote& quote, 
                     USDTgAmount slippage_tolerance = 50); // 0.5%
    
    Hash add_liquidity(const Address& provider, DenomId token_a, DenomId token_b,
                      USDTgAmount amount_a, USDTgAmount amount_b);
    Hash remove_liquidity(const Address& provider, Hash pool_id, USDTgAmount liquidity_tokens);
    
    // CEX Integration  
    Hash place_limit_order(const Address& trader, const std::string& symbol,
                          cex::OrderSide side, USDTgAmount quantity, USDTgAmount price);
    Hash place_market_order(const Address& trader, const std::string& symbol,
                           cex::OrderSide side, USDTgAmount quantity);
    bool cancel_order(const std::string& symbol, Hash order_id, const Address& trader);
    
    std::vector<cex::Order> get_open_orders(const Address& trader) const;
    std::vector<cex::Trade> get_trade_history(const Address& trader, size_t limit = 100) const;
    
    // OdixPay++ Payment System
    
    // Invoice Management
    odixpay::Invoice create_invoice(const Address& merchant, const Address& payer,
                                   USDTgAmount amount, DenomId denom,
                                   const std::string& description,
                                   Timestamp due_date = 0);
    bool pay_invoice(Hash invoice_id, const Address& payer);
    std::vector<odixpay::Invoice> get_invoices(const Address& user, bool sent = true) const;
    
    // Escrow Services
    odixpay::EscrowContract create_escrow(const Address& buyer, const Address& seller,
                                         const Address& arbiter, USDTgAmount amount,
                                         DenomId denom, const std::string& description);
    bool release_escrow(Hash escrow_id, const Address& releaser);
    bool dispute_escrow(Hash escrow_id, const Address& disputer);
    std::vector<odixpay::EscrowContract> get_escrows(const Address& user) const;
    
    // Subscription Payments
    odixpay::Subscription create_subscription(const Address& payer, const Address& merchant,
                                             const std::string& plan_name, USDTgAmount amount,
                                             DenomId denom, uint32_t interval_days);
    bool cancel_subscription(Hash subscription_id, const Address& user);
    bool process_subscription_payments(); // Called periodically
    std::vector<odixpay::Subscription> get_subscriptions(const Address& user) const;
    
    // Spending Limits
    bool set_spending_limit(const Address& address, DenomId denom,
                           USDTgAmount daily_limit, USDTgAmount monthly_limit);
    odixpay::SpendingLimit get_spending_limit(const Address& address, DenomId denom) const;
    bool check_spending_limit(const Address& address, DenomId denom, USDTgAmount amount) const;
    
    // Batch Payments
    odixpay::BatchPayment create_batch_payment(const Address& payer,
                                              const std::vector<std::pair<Address, USDTgAmount>>& recipients,
                                              DenomId denom, const std::string& description);
    bool execute_batch_payment(Hash batch_id);
    
    // Security Features
    bool enable_2fa(const std::string& secret);
    bool verify_2fa(const std::string& code);
    bool set_transaction_pin(const std::string& pin);
    bool verify_transaction_pin(const std::string& pin);
    
    // Backup & Recovery
    std::string export_seed_phrase(const std::string& password) const;
    std::string export_private_key(const Address& address, const std::string& password) const;
    bool backup_wallet(const std::string& file_path, const std::string& password) const;
    static USDTgWallet restore_from_backup(const std::string& file_path, const std::string& password);
    
    // Network Configuration
    void set_rpc_endpoint(const std::string& endpoint) { rpc_endpoint_ = endpoint; }
    bool test_connection() const;
    
    // Event Callbacks
    using TransactionCallback = std::function<void(const TransactionRecord&)>;
    using BalanceCallback = std::function<void(const Address&, DenomId, USDTgAmount)>;
    using InvoiceCallback = std::function<void(const odixpay::Invoice&)>;
    
    void set_transaction_callback(TransactionCallback callback);
    void set_balance_callback(BalanceCallback callback);
    void set_invoice_callback(InvoiceCallback callback);
    
private:
    // Internal helpers
    bool unlock_wallet(const std::string& password);
    PrivateKey get_private_key(const Address& address, const std::string& password) const;
    bool validate_address(const Address& address) const;
    Hash broadcast_transaction(const Tx& tx);
    void update_spending_limits(const Address& address, DenomId denom, USDTgAmount amount);
};

// Mobile-specific extensions
namespace mobile {

// QR Code Payment
struct QRPayment {
    Address recipient;
    USDTgAmount amount;
    DenomId denom;
    std::string memo;
    Hash invoice_id; // Optional
};

class MobileWallet : public USDTgWallet {
public:
    // QR Code functionality
    std::string generate_receive_qr(const Address& address, USDTgAmount amount = 0,
                                   DenomId denom = 0, const std::string& memo = "");
    QRPayment parse_payment_qr(const std::string& qr_data);
    
    // NFC Payments
    bool enable_nfc_payments();
    bool process_nfc_payment(const std::string& nfc_data);
    
    // Biometric Authentication
    bool enable_fingerprint_auth();
    bool enable_face_id_auth();
    bool authenticate_with_biometrics();
    
    // Push Notifications
    void enable_push_notifications(const std::string& device_token);
    void notify_transaction_received(const TransactionRecord& tx);
    void notify_invoice_paid(const odixpay::Invoice& invoice);
    
    // Offline Capabilities
    std::vector<Tx> get_pending_transactions() const;
    bool sync_when_online();
};

} // namespace mobile

} // namespace wallet
} // namespace usdtgverse
