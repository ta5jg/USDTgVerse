/*
 * OdixPay++ API - High-Level C++ Interface
 * User-friendly payment system built on ultra-fast C core
 */

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <sys/stat.h>

extern "C" {
    #include "../core/blockchain_core.c"  // Include C core
}

namespace usdtgverse::odixpay {

// ============================================================================
// DATABASE LOGGING INTEGRATION FOR ODIXPAY
// ============================================================================

#define DATA_DIR "/opt/usdtgverse/data"
#define ODIXPAY_LOG_DB DATA_DIR "/odixpay_payments.db"
#define ODIXPAY_MERCHANT_DB DATA_DIR "/merchant_transactions.db"

void log_payment_to_database(const std::string& payment_id, const std::string& merchant_id, 
                             double amount, const std::string& status) {
    std::ofstream payment_file(ODIXPAY_LOG_DB, std::ios::app);
    if (payment_file.is_open()) {
        payment_file << payment_id << "|" << merchant_id << "|" << amount 
                   << "|" << status << "|" << time(nullptr) << std::endl;
        payment_file.close();
        std::cout << "💳 Payment Logged: " << payment_id << " ($" << amount << ") " << status << std::endl;
    }
}

void log_merchant_transaction(const std::string& merchant_id, const std::string& customer_id,
                             double amount, const std::string& tx_hash) {
    std::ofstream merchant_file(ODIXPAY_MERCHANT_DB, std::ios::app);
    if (merchant_file.is_open()) {
        merchant_file << merchant_id << "|" << customer_id << "|" << amount 
                    << "|" << tx_hash << "|" << time(nullptr) << std::endl;
        merchant_file.close();
        std::cout << "🏪 Merchant Tx Logged: " << merchant_id << " -> Customer " << customer_id << std::endl;
    }
}

void ensure_odixpay_data_directory() {
    system("mkdir -p /opt/usdtgverse/data");
    std::cout << "📂 OdixPay++ data directory ensured: " << DATA_DIR << std::endl;
}

// ============================================================================
// HIGH-LEVEL C++ WRAPPER CLASSES
// ============================================================================

class USDTgAmount {
private:
    usdtg_amount_t raw_amount_;

public:
    USDTgAmount(double usdtg) : raw_amount_(static_cast<usdtg_amount_t>(usdtg * USDTG_DECIMALS)) {}
    USDTgAmount(usdtg_amount_t raw) : raw_amount_(raw) {}
    
    double toDouble() const { return static_cast<double>(raw_amount_) / USDTG_DECIMALS; }
    usdtg_amount_t raw() const { return raw_amount_; }
    
    std::string toString() const {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << toDouble() << " USDTg";
        return oss.str();
    }
    
    USDTgAmount operator+(const USDTgAmount& other) const {
        return USDTgAmount(raw_amount_ + other.raw_amount_);
    }
    
    USDTgAmount operator-(const USDTgAmount& other) const {
        return USDTgAmount(raw_amount_ - other.raw_amount_);
    }
    
    bool operator>=(const USDTgAmount& other) const {
        return raw_amount_ >= other.raw_amount_;
    }
};

class Address {
private:
    usdtg_address_t addr_;

public:
    Address() { memset(addr_, 0, 20); }
    
    Address(const std::string& hex) {
        // Simple hex parsing for demo
        memset(addr_, 0, 20);
        for (size_t i = 0; i < std::min(hex.length(), size_t(40)); i += 2) {
            if (i/2 < 20) {
                addr_[i/2] = static_cast<uint8_t>(std::stoul(hex.substr(i, 2), nullptr, 16));
            }
        }
    }
    
    static Address fromSeed(uint8_t seed) {
        Address addr;
        memset(addr.addr_, seed, 20);
        return addr;
    }
    
    std::string toString() const {
        std::ostringstream oss;
        oss << "usdtg1" << std::hex << std::setfill('0');
        for (int i = 0; i < 4; i++) {
            oss << std::setw(2) << static_cast<unsigned>(addr_[i]);
        }
        oss << "...";
        return oss.str();
    }
    
    const usdtg_address_t& raw() const { return addr_; }
    usdtg_address_t& raw() { return addr_; }
    
    bool operator==(const Address& other) const {
        return memcmp(addr_, other.addr_, 20) == 0;
    }
};

class Account {
private:
    usdtg_account_t core_account_;

public:
    Account() { memset(&core_account_, 0, sizeof(core_account_)); }
    
    USDTgAmount getUSDTgBalance() const {
        usdtg_amount_t balance;
        usdtg_account_get_balance(&core_account_, USDTG_DENOM_ID, &balance);
        return USDTgAmount(balance);
    }
    
    void setUSDTgBalance(const USDTgAmount& amount) {
        usdtg_account_set_balance(&core_account_, USDTG_DENOM_ID, amount.raw());
    }
    
    bool transfer(Account& to, const USDTgAmount& amount) {
        usdtg_coin_t coin = {USDTG_DENOM_ID, amount.raw()};
        return usdtg_account_transfer(&core_account_, &to.core_account_, &coin) == 0;
    }
    
    void setNonce(uint64_t nonce) { core_account_.nonce = nonce; }
    uint64_t getNonce() const { return core_account_.nonce; }
    
    std::string toString() const {
        return "Account{balance: " + getUSDTgBalance().toString() + 
               ", nonce: " + std::to_string(getNonce()) + "}";
    }
};

// ============================================================================
// ODIXPAY++ HIGH-LEVEL API
// ============================================================================

class Invoice {
public:
    std::string invoice_id;
    Address merchant;
    Address payer;
    USDTgAmount amount;
    std::string description;
    std::chrono::system_clock::time_point due_date;
    bool is_paid;
    
    Invoice(const std::string& id, const Address& merch, const USDTgAmount& amt, 
            const std::string& desc, std::chrono::minutes valid_for = std::chrono::minutes(60))
        : invoice_id(id), merchant(merch), amount(amt), description(desc), is_paid(false) {
        due_date = std::chrono::system_clock::now() + valid_for;
    }
    
    bool isExpired() const {
        return std::chrono::system_clock::now() > due_date;
    }
    
    std::string toString() const {
        std::ostringstream oss;
        oss << "Invoice{id: " << invoice_id 
            << ", merchant: " << merchant.toString()
            << ", amount: " << amount.toString()
            << ", desc: \"" << description << "\""
            << ", status: " << (is_paid ? "PAID" : "PENDING")
            << ", expired: " << (isExpired() ? "YES" : "NO") << "}";
        return oss.str();
    }
};

class OdixPayAPI {
private:
    std::unordered_map<Address, Account> accounts_;
    std::unordered_map<std::string, Invoice> invoices_;
    
    // Hash function for Address
    struct AddressHash {
        size_t operator()(const Address& addr) const {
            size_t hash = 0;
            const uint8_t* data = reinterpret_cast<const uint8_t*>(&addr.raw());
            for (size_t i = 0; i < 20; i++) {
                hash = hash * 31 + data[i];
            }
            return hash;
        }
    };
    
    std::unordered_map<Address, Account, AddressHash> accounts_hashed_;

public:
    OdixPayAPI() = default;
    
    // Account management
    Account& getAccount(const Address& addr) {
        return accounts_hashed_[addr];
    }
    
    void createAccount(const Address& addr, const USDTgAmount& initial_balance = USDTgAmount(0.0)) {
        accounts_hashed_[addr].setUSDTgBalance(initial_balance);
    }
    
    // Basic transfers
    bool transfer(const Address& from, const Address& to, const USDTgAmount& amount) {
        auto& from_account = accounts_hashed_[from];
        auto& to_account = accounts_hashed_[to];
        
        if (from_account.getUSDTgBalance().raw() < amount.raw()) {
            return false; // Insufficient funds
        }
        
        return from_account.transfer(to_account, amount);
    }
    
    // Invoice system
    std::string createInvoice(const Address& merchant, const USDTgAmount& amount, 
                            const std::string& description, 
                            std::chrono::minutes valid_for = std::chrono::minutes(60)) {
        std::string invoice_id = "INV_" + std::to_string(invoices_.size() + 1);
        invoices_[invoice_id] = Invoice(invoice_id, merchant, amount, description, valid_for);
        return invoice_id;
    }
    
    bool payInvoice(const std::string& invoice_id, const Address& payer) {
        auto it = invoices_.find(invoice_id);
        if (it == invoices_.end()) return false;
        
        Invoice& invoice = it->second;
        if (invoice.is_paid || invoice.isExpired()) return false;
        
        if (transfer(payer, invoice.merchant, invoice.amount)) {
            invoice.is_paid = true;
            invoice.payer = payer;
            return true;
        }
        
        return false;
    }
    
    std::optional<Invoice> getInvoice(const std::string& invoice_id) const {
        auto it = invoices_.find(invoice_id);
        if (it != invoices_.end()) {
            return it->second;
        }
        return std::nullopt;
    }
    
    // Statistics
    USDTgAmount getTotalSupply() const {
        USDTgAmount total(0.0);
        for (const auto& [addr, account] : accounts_hashed_) {
            total = total + account.getUSDTgBalance();
        }
        return total;
    }
    
    size_t getAccountCount() const { return accounts_hashed_.size(); }
    size_t getInvoiceCount() const { return invoices_.size(); }
    
    std::string getSystemStatus() const {
        std::ostringstream oss;
        oss << "OdixPay++ System Status:\n";
        oss << "  Total Accounts: " << getAccountCount() << "\n";
        oss << "  Total Supply: " << getTotalSupply().toString() << "\n";
        oss << "  Total Invoices: " << getInvoiceCount() << "\n";
        oss << "  Active Invoices: " << getActiveInvoiceCount() << "\n";
        return oss.str();
    }
    
private:
    size_t getActiveInvoiceCount() const {
        size_t count = 0;
        for (const auto& [id, invoice] : invoices_) {
            if (!invoice.is_paid && !invoice.isExpired()) count++;
        }
        return count;
    }
};

} // namespace usdtgverse::odixpay

// ============================================================================
// DEMO APPLICATION
// ============================================================================

int main() {
    std::cout << R"(
🌌 =============================================== 🌌
    USDTgVerse OdixPay++ API - C++ High Level
    Enterprise Payment System on Ultra-Fast C Core
🌌 =============================================== 🌌

)" << std::endl;
    
    using namespace usdtgverse::odixpay;
    
    // Create OdixPay API
    OdixPayAPI odixpay;
    
    // Create accounts
    auto alice = Address::fromSeed(0xAA);
    auto bob = Address::fromSeed(0xBB);
    auto merchant = Address::fromSeed(0xCC);
    
    std::cout << "👥 Creating accounts:\n";
    std::cout << "   Alice: " << alice.toString() << "\n";
    std::cout << "   Bob: " << bob.toString() << "\n";
    std::cout << "   Merchant: " << merchant.toString() << "\n\n";
    
    // Fund accounts
    odixpay.createAccount(alice, USDTgAmount(1000.0));
    odixpay.createAccount(bob, USDTgAmount(500.0));
    odixpay.createAccount(merchant, USDTgAmount(0.0));
    
    std::cout << "💰 Initial balances:\n";
    std::cout << "   " << alice.toString() << ": " << odixpay.getAccount(alice).getUSDTgBalance().toString() << "\n";
    std::cout << "   " << bob.toString() << ": " << odixpay.getAccount(bob).getUSDTgBalance().toString() << "\n";
    std::cout << "   " << merchant.toString() << ": " << odixpay.getAccount(merchant).getUSDTgBalance().toString() << "\n\n";
    
    // Test basic transfer
    std::cout << "💸 Testing transfer (Alice → Bob: 100 USDTg):\n";
    bool success = odixpay.transfer(alice, bob, USDTgAmount(100.0));
    std::cout << "   Result: " << (success ? "SUCCESS" : "FAILED") << "\n\n";
    
    // Test invoice system
    std::cout << "🧾 Testing Invoice System:\n";
    std::string invoice_id = odixpay.createInvoice(merchant, USDTgAmount(50.0), "Coffee and pastry");
    std::cout << "   Created invoice: " << invoice_id << "\n";
    
    auto invoice = odixpay.getInvoice(invoice_id);
    if (invoice.has_value()) {
        std::cout << "   " << invoice->toString() << "\n";
    }
    
    // Pay invoice
    std::cout << "\n💳 Bob pays invoice:\n";
    bool paid = odixpay.payInvoice(invoice_id, bob);
    std::cout << "   Payment result: " << (paid ? "SUCCESS" : "FAILED") << "\n";
    
    // Check updated invoice
    invoice = odixpay.getInvoice(invoice_id);
    if (invoice.has_value()) {
        std::cout << "   Updated: " << invoice->toString() << "\n";
    }
    
    std::cout << "\n💰 Final balances:\n";
    std::cout << "   " << alice.toString() << ": " << odixpay.getAccount(alice).getUSDTgBalance().toString() << "\n";
    std::cout << "   " << bob.toString() << ": " << odixpay.getAccount(bob).getUSDTgBalance().toString() << "\n";
    std::cout << "   " << merchant.toString() << ": " << odixpay.getAccount(merchant).getUSDTgBalance().toString() << "\n\n";
    
    std::cout << "📊 " << odixpay.getSystemStatus() << "\n";
    
    std::cout << "✅ OdixPay++ Features Demonstrated:\n";
    std::cout << "   💎 C++ high-level API on ultra-fast C core\n";
    std::cout << "   🏦 Invoice creation & payment system\n";
    std::cout << "   💸 Instant USDTg transfers\n";
    std::cout << "   📱 User-friendly C++ interface\n";
    std::cout << "   ⚡ Zero-copy performance (C core)\n";
    std::cout << "   🔒 Type-safe C++ wrappers\n\n";
    
    std::cout << "🚀 Perfect Architecture: C Core + C++ High-Level!\n";
    std::cout << "   Ultra Performance + Developer Friendly\n\n";
    
    return 0;
}
