#include "core/account.hpp"
#include "common/types.hpp"
#include <algorithm>
#include <sstream>

namespace usdtgverse::core {

// ============================================================================
// ACCOUNT IMPLEMENTATION
// ============================================================================

Account::Account() : nonce(0) {}

Account::Account(Nonce n) : nonce(n) {}

bool Account::is_empty() const {
    return nonce == 0 && balances.empty() && !spending_limit.has_value();
}

Amount Account::get_balance(AssetID denom_id) const {
    auto it = balances.find(denom_id);
    return (it != balances.end()) ? it->second : Amount(0);
}

void Account::set_balance(AssetID denom_id, Amount amount) {
    if (amount > 0) {
        balances[denom_id] = amount;
    } else {
        balances.erase(denom_id);
    }
}

void Account::add_balance(AssetID denom_id, Amount amount) {
    if (amount > 0) {
        balances[denom_id] = get_balance(denom_id) + amount;
    }
}

bool Account::subtract_balance(AssetID denom_id, Amount amount) {
    Amount current_balance = get_balance(denom_id);
    if (current_balance < amount) {
        return false; // Insufficient funds
    }
    
    Amount new_balance = current_balance - amount;
    set_balance(denom_id, new_balance);
    return true;
}

Amount Account::get_usdtg_balance() const {
    return get_balance(USDTG_DENOM_ID);
}

void Account::set_usdtg_balance(Amount amount) {
    set_balance(USDTG_DENOM_ID, amount);
}

bool Account::has_sufficient_balance(AssetID denom_id, Amount required_amount) const {
    return get_balance(denom_id) >= required_amount;
}

bool Account::has_sufficient_usdtg(Amount required_amount) const {
    return has_sufficient_balance(USDTG_DENOM_ID, required_amount);
}

std::vector<AssetID> Account::get_asset_denoms() const {
    std::vector<AssetID> denoms;
    denoms.reserve(balances.size());
    
    for (const auto& [denom_id, balance] : balances) {
        if (balance > 0) {
            denoms.push_back(denom_id);
        }
    }
    
    std::sort(denoms.begin(), denoms.end());
    return denoms;
}

Amount Account::get_total_balance_value() const {
    // For now, only count USDTg. In the future, we could add price oracles
    // to calculate USD value of all assets
    return get_usdtg_balance();
}

void Account::set_spending_limit(const SpendingLimit& limit) {
    spending_limit = limit;
}

void Account::clear_spending_limit() {
    spending_limit.reset();
}

bool Account::has_spending_limit() const {
    return spending_limit.has_value();
}

const SpendingLimit& Account::get_spending_limit() const {
    static SpendingLimit empty_limit;
    return spending_limit.has_value() ? spending_limit.value() : empty_limit;
}

bool Account::check_spending_limit(const Coin& amount, Timestamp current_time) {
    if (!spending_limit.has_value()) {
        return true; // No limit set
    }
    
    return spending_limit->check_and_update(amount, current_time);
}

std::string Account::to_string() const {
    std::ostringstream oss;
    oss << "Account{\n";
    oss << "  nonce: " << nonce << "\n";
    oss << "  balances: {\n";
    
    for (const auto& [denom_id, balance] : balances) {
        if (denom_id == USDTG_DENOM_ID) {
            double usdtg_value = static_cast<double>(balance) / static_cast<double>(USDTG_DECIMALS);
            oss << "    USDTg: " << usdtg_value << "\n";
        } else {
            oss << "    denom_" << denom_id << ": " << balance << "\n";
        }
    }
    
    oss << "  }\n";
    
    if (spending_limit.has_value()) {
        oss << "  spending_limit: " << spending_limit->to_string() << "\n";
    }
    
    oss << "}";
    return oss.str();
}

// ============================================================================
// SPENDING LIMIT IMPLEMENTATION
// ============================================================================

SpendingLimit::SpendingLimit() : daily_limit(0), monthly_limit(0), spent_today(0), spent_this_month(0) {
    auto now = current_timestamp_ms();
    last_reset_day = now;
    last_reset_month = now;
}

SpendingLimit::SpendingLimit(Amount daily, Amount monthly) 
    : daily_limit(daily), monthly_limit(monthly), spent_today(0), spent_this_month(0) {
    auto now = current_timestamp_ms();
    last_reset_day = now;
    last_reset_month = now;
}

bool SpendingLimit::is_valid() const {
    return daily_limit >= 0 && monthly_limit >= 0 && 
           spent_today >= 0 && spent_this_month >= 0;
}

bool SpendingLimit::check_daily_limit(Amount amount, Timestamp current_time) {
    reset_if_needed(current_time);
    
    if (daily_limit == 0) {
        return true; // No daily limit
    }
    
    return (spent_today + amount) <= daily_limit;
}

bool SpendingLimit::check_monthly_limit(Amount amount, Timestamp current_time) {
    reset_if_needed(current_time);
    
    if (monthly_limit == 0) {
        return true; // No monthly limit
    }
    
    return (spent_this_month + amount) <= monthly_limit;
}

bool SpendingLimit::check_and_update(const Coin& coin, Timestamp current_time) {
    // Only apply limits to USDTg for now
    if (coin.denom_id != USDTG_DENOM_ID) {
        return true;
    }
    
    Amount amount = coin.amount;
    
    reset_if_needed(current_time);
    
    // Check both daily and monthly limits
    if (!check_daily_limit(amount, current_time)) {
        return false;
    }
    
    if (!check_monthly_limit(amount, current_time)) {
        return false;
    }
    
    // Update spent amounts
    spent_today += amount;
    spent_this_month += amount;
    
    return true;
}

void SpendingLimit::reset_if_needed(Timestamp current_time) {
    // Reset daily spending if a day has passed
    if (should_reset_daily(current_time)) {
        spent_today = 0;
        last_reset_day = current_time;
    }
    
    // Reset monthly spending if a month has passed
    if (should_reset_monthly(current_time)) {
        spent_this_month = 0;
        last_reset_month = current_time;
    }
}

bool SpendingLimit::should_reset_daily(Timestamp current_time) const {
    // Reset if more than 24 hours have passed
    constexpr uint64_t DAY_MS = 24 * 60 * 60 * 1000;
    return (current_time - last_reset_day) >= DAY_MS;
}

bool SpendingLimit::should_reset_monthly(Timestamp current_time) const {
    // Reset if more than 30 days have passed
    constexpr uint64_t MONTH_MS = 30 * 24 * 60 * 60 * 1000;
    return (current_time - last_reset_month) >= MONTH_MS;
}

Amount SpendingLimit::get_remaining_daily_limit(Timestamp current_time) {
    reset_if_needed(current_time);
    
    if (daily_limit == 0) {
        return Amount(-1); // Unlimited
    }
    
    return std::max(Amount(0), daily_limit - spent_today);
}

Amount SpendingLimit::get_remaining_monthly_limit(Timestamp current_time) {
    reset_if_needed(current_time);
    
    if (monthly_limit == 0) {
        return Amount(-1); // Unlimited
    }
    
    return std::max(Amount(0), monthly_limit - spent_this_month);
}

std::string SpendingLimit::to_string() const {
    std::ostringstream oss;
    oss << "SpendingLimit{";
    
    if (daily_limit > 0) {
        double daily_usdtg = static_cast<double>(daily_limit) / static_cast<double>(USDTG_DECIMALS);
        oss << "daily: " << daily_usdtg << " USDTg";
    } else {
        oss << "daily: unlimited";
    }
    
    oss << ", ";
    
    if (monthly_limit > 0) {
        double monthly_usdtg = static_cast<double>(monthly_limit) / static_cast<double>(USDTG_DECIMALS);
        oss << "monthly: " << monthly_usdtg << " USDTg";
    } else {
        oss << "monthly: unlimited";
    }
    
    oss << "}";
    return oss.str();
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

Account create_genesis_account(Amount initial_usdtg_balance) {
    Account account;
    account.set_usdtg_balance(initial_usdtg_balance);
    return account;
}

Account create_validator_account(Amount stake_amount) {
    Account account;
    account.set_usdtg_balance(stake_amount);
    // Validators might have different spending limits or other properties
    return account;
}

bool transfer_between_accounts(Account& from_account, Account& to_account, const Coin& amount) {
    if (!from_account.subtract_balance(amount.denom_id, amount.amount)) {
        return false; // Insufficient funds
    }
    
    to_account.add_balance(amount.denom_id, amount.amount);
    return true;
}

std::vector<std::pair<AssetID, Amount>> get_all_balances(const Account& account) {
    std::vector<std::pair<AssetID, Amount>> balances;
    
    for (const auto& [denom_id, balance] : account.balances) {
        if (balance > 0) {
            balances.emplace_back(denom_id, balance);
        }
    }
    
    // Sort by denom_id for deterministic ordering
    std::sort(balances.begin(), balances.end());
    return balances;
}

bool is_valid_account(const Account& account) {
    // Check that all balances are non-negative
    for (const auto& [denom_id, balance] : account.balances) {
        if (balance < 0) {
            return false;
        }
    }
    
    // Check spending limit validity
    if (account.has_spending_limit()) {
        if (!account.get_spending_limit().is_valid()) {
            return false;
        }
    }
    
    return true;
}

} // namespace usdtgverse::core

