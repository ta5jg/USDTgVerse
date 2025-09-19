#include "common/types.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace usdtgverse {

// ============================================================================
// HASH UTILITIES
// ============================================================================

std::string hash_to_hex(const Hash& hash) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (const auto& byte : hash) {
        oss << std::setw(2) << static_cast<unsigned>(byte);
    }
    return oss.str();
}

std::optional<Hash> hex_to_hash(const std::string& hex) {
    if (hex.length() != 64) {
        return std::nullopt;
    }
    
    Hash hash;
    try {
        for (size_t i = 0; i < 32; ++i) {
            std::string byte_str = hex.substr(i * 2, 2);
            hash[i] = static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16));
        }
        return hash;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

bool is_zero_hash(const Hash& hash) {
    return std::all_of(hash.begin(), hash.end(), [](uint8_t b) { return b == 0; });
}

// ============================================================================
// ADDRESS UTILITIES
// ============================================================================

std::string address_to_string(const Address& addr) {
    std::ostringstream oss;
    oss << "usdtg1"; // Bech32-style prefix
    oss << std::hex << std::setfill('0');
    for (const auto& byte : addr) {
        oss << std::setw(2) << static_cast<unsigned>(byte);
    }
    return oss.str();
}

std::optional<Address> string_to_address(const std::string& str) {
    if (str.length() != 46 || str.substr(0, 6) != "usdtg1") {
        return std::nullopt;
    }
    
    std::string hex_part = str.substr(6);
    if (hex_part.length() != 40) {
        return std::nullopt;
    }
    
    Address addr;
    try {
        for (size_t i = 0; i < 20; ++i) {
            std::string byte_str = hex_part.substr(i * 2, 2);
            addr[i] = static_cast<uint8_t>(std::stoul(byte_str, nullptr, 16));
        }
        return addr;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

bool is_valid_address(const Address& addr) {
    // Check if address is not all zeros
    return !std::all_of(addr.begin(), addr.end(), [](uint8_t b) { return b == 0; });
}

// ============================================================================
// AMOUNT UTILITIES
// ============================================================================

std::string amount_to_string(Amount amount, AssetID denom_id) {
    if (denom_id == USDTG_DENOM_ID) {
        // USDTg with 18 decimals
        double value = static_cast<double>(amount) / static_cast<double>(USDTG_DECIMALS);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << value;
        return oss.str();
    } else {
        // Other assets - assume 18 decimals for now
        double value = static_cast<double>(amount) / static_cast<double>(USDTG_DECIMALS);
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6) << value;
        return oss.str();
    }
}

std::optional<Amount> string_to_amount(const std::string& str) {
    try {
        double value = std::stod(str);
        if (value < 0) {
            return std::nullopt;
        }
        return static_cast<Amount>(value * static_cast<double>(USDTG_DECIMALS));
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

double amount_to_double(Amount amount, AssetID denom_id) {
    if (denom_id == USDTG_DENOM_ID) {
        return static_cast<double>(amount) / static_cast<double>(USDTG_DECIMALS);
    } else {
        return static_cast<double>(amount) / static_cast<double>(USDTG_DECIMALS);
    }
}

Amount double_to_amount(double value, AssetID denom_id) {
    if (value < 0) {
        return Amount(0);
    }
    
    if (denom_id == USDTG_DENOM_ID) {
        return static_cast<Amount>(value * static_cast<double>(USDTG_DECIMALS));
    } else {
        return static_cast<Amount>(value * static_cast<double>(USDTG_DECIMALS));
    }
}

// ============================================================================
// VALIDATION UTILITIES
// ============================================================================

bool is_valid_chain_id(const ChainID& chain_id) {
    return !chain_id.empty() && chain_id.length() <= 64;
}

bool is_valid_nonce(Nonce nonce) {
    return true; // All nonces are valid
}

bool is_valid_amount(Amount amount) {
    return amount >= 0;
}

bool is_valid_fee(const Fee& fee) {
    return fee.max_fee >= fee.tip && fee.weight > 0;
}

// ============================================================================
// ERROR HANDLING
// ============================================================================

std::string error_code_to_string(ErrorCode code) {
    switch (code) {
        case ErrorCode::SUCCESS: return "Success";
        case ErrorCode::INVALID_INPUT: return "Invalid input";
        case ErrorCode::INSUFFICIENT_FUNDS: return "Insufficient funds";
        case ErrorCode::INVALID_SIGNATURE: return "Invalid signature";
        case ErrorCode::INVALID_NONCE: return "Invalid nonce";
        case ErrorCode::INVALID_FEE: return "Invalid fee";
        case ErrorCode::ACCOUNT_NOT_FOUND: return "Account not found";
        case ErrorCode::TRANSACTION_TOO_LARGE: return "Transaction too large";
        case ErrorCode::BLOCK_TOO_LARGE: return "Block too large";
        case ErrorCode::CONSENSUS_FAILURE: return "Consensus failure";
        case ErrorCode::NETWORK_ERROR: return "Network error";
        case ErrorCode::STORAGE_ERROR: return "Storage error";
        case ErrorCode::CRYPTO_ERROR: return "Cryptography error";
        case ErrorCode::UNKNOWN_ERROR: return "Unknown error";
        default: return "Unknown error code";
    }
}

} // namespace usdtgverse

