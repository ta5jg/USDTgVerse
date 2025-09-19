#pragma once

#include <cstdint>
#include <array>
#include <vector>
#include <string>
#include <chrono>
#include <optional>

namespace usdtgverse {

// ============================================================================
// FUNDAMENTAL TYPES
// ============================================================================

// Hash types (32 bytes)
using Hash = std::array<uint8_t, 32>;
using BlockHash = Hash;
using TxHash = Hash;
using StateRoot = Hash;

// Message types for OdixPay++
enum MsgType : uint8_t {
    MSG_TRANSFER = 1,
    MSG_MULTISEND = 2,
    MSG_INVOICE = 10,
    MSG_PAY_INVOICE = 11,
    MSG_ESCROW_CREATE = 20,
    MSG_ESCROW_RELEASE = 21,
    MSG_SUBSCRIBE = 30,
    MSG_SET_SPENDING_LIMIT = 40,
    MSG_BATCH_PAY = 50,
};
using MerkleRoot = Hash;

// Address type (20 bytes + checksum)
using Address = std::array<uint8_t, 20>;

// Cryptographic types
using Signature = std::array<uint8_t, 64>;  // Ed25519
using PublicKey = std::array<uint8_t, 32>;  // Ed25519
using PrivateKey = std::array<uint8_t, 32>; // Ed25519
using SecpSignature = std::array<uint8_t, 65>; // secp256k1 (recovery)
using SecpPublicKey = std::array<uint8_t, 33>;  // secp256k1 compressed

// Blockchain primitives
using BlockHeight = uint64_t;
using Timestamp = uint64_t;
using Nonce = uint64_t;
using ChainID = std::string;

// USDTg and multi-asset support
using AssetID = uint32_t;
using Amount = __int128;  // 128-bit for precision

// Asset denomination
struct Coin {
    AssetID denom_id;
    Amount amount;
    
    Coin() : denom_id(0), amount(0) {}
    Coin(AssetID denom, Amount amt) : denom_id(denom), amount(amt) {}
    
    bool operator==(const Coin& other) const {
        return denom_id == other.denom_id && amount == other.amount;
    }
    
    bool operator!=(const Coin& other) const {
        return !(*this == other);
    }
};

// Fee structure
struct Fee {
    uint64_t max_fee;    // Maximum fee willing to pay
    uint64_t tip;        // Priority tip for validators
    uint32_t weight;     // Transaction weight/gas
    
    Fee() : max_fee(0), tip(0), weight(0) {}
    Fee(uint64_t max, uint64_t t, uint32_t w) : max_fee(max), tip(t), weight(w) {}
};

// ============================================================================
// CONSTANTS
// ============================================================================

// USDTg native token
constexpr AssetID USDTG_DENOM_ID = 0;
constexpr Amount USDTG_DECIMALS = 1000000000000000000LL; // 10^18
constexpr Amount INITIAL_USDTG_SUPPLY = Amount(1000000000LL) * USDTG_DECIMALS; // 1B USDTg

// Blockchain parameters
constexpr ChainID MAINNET_CHAIN_ID = "usdtgverse-1";
constexpr ChainID TESTNET_CHAIN_ID = "usdtgverse-testnet-1";
constexpr uint32_t BLOCK_TIME_MS = 1000; // 1 second blocks
constexpr uint32_t FINALITY_BLOCKS = 3;  // 3 block finality
constexpr size_t MAX_BLOCK_SIZE = 2 * 1024 * 1024; // 2MB
constexpr size_t MAX_TX_PER_BLOCK = 10000;

// Consensus parameters
constexpr size_t MIN_VALIDATOR_COUNT = 4;
constexpr size_t MAX_VALIDATOR_COUNT = 100;
constexpr double CONSENSUS_THRESHOLD = 2.0 / 3.0; // 2f+1 BFT threshold

// Network parameters
constexpr uint16_t DEFAULT_P2P_PORT = 26656;
constexpr uint16_t DEFAULT_RPC_PORT = 26657;
constexpr size_t MAX_PEERS = 50;
constexpr size_t MIN_PEERS = 4;

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Time utilities
inline Timestamp current_timestamp_ms() {
    return static_cast<Timestamp>(std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count());
}

inline Timestamp current_timestamp_us() {
    return static_cast<Timestamp>(std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count());
}

// Hash utilities
std::string hash_to_hex(const Hash& hash);
std::optional<Hash> hex_to_hash(const std::string& hex);
bool is_zero_hash(const Hash& hash);

// Address utilities
std::string address_to_string(const Address& addr);
std::optional<Address> string_to_address(const std::string& str);
bool is_valid_address(const Address& addr);

// Amount utilities
std::string amount_to_string(Amount amount, AssetID denom_id = USDTG_DENOM_ID);
std::optional<Amount> string_to_amount(const std::string& str);
double amount_to_double(Amount amount, AssetID denom_id = USDTG_DENOM_ID);
Amount double_to_amount(double value, AssetID denom_id = USDTG_DENOM_ID);

// USDTg specific utilities
inline std::string format_usdtg(Amount amount) {
    return amount_to_string(amount, USDTG_DENOM_ID) + " USDTg";
}

inline Amount parse_usdtg(const std::string& str) {
    auto amount = string_to_amount(str);
    return amount ? *amount : Amount(0);
}

// Validation utilities
bool is_valid_chain_id(const ChainID& chain_id);
bool is_valid_nonce(Nonce nonce);
bool is_valid_amount(Amount amount);
bool is_valid_fee(const Fee& fee);

// Error handling
enum class ErrorCode : uint32_t {
    SUCCESS = 0,
    INVALID_INPUT = 1,
    INSUFFICIENT_FUNDS = 2,
    INVALID_SIGNATURE = 3,
    INVALID_NONCE = 4,
    INVALID_FEE = 5,
    ACCOUNT_NOT_FOUND = 6,
    TRANSACTION_TOO_LARGE = 7,
    BLOCK_TOO_LARGE = 8,
    CONSENSUS_FAILURE = 9,
    NETWORK_ERROR = 10,
    STORAGE_ERROR = 11,
    CRYPTO_ERROR = 12,
    UNKNOWN_ERROR = 1000
};

std::string error_code_to_string(ErrorCode code);

// Result type for error handling
template<typename T>
struct Result {
    std::optional<T> value;
    ErrorCode error;
    std::string message;
    
    Result() : error(ErrorCode::UNKNOWN_ERROR) {}
    Result(T val) : value(std::move(val)), error(ErrorCode::SUCCESS) {}
    Result(ErrorCode err, const std::string& msg = "") : error(err), message(msg) {}
    
    bool is_ok() const { return error == ErrorCode::SUCCESS && value.has_value(); }
    bool is_error() const { return !is_ok(); }
    
    const T& unwrap() const { return *value; }
    T& unwrap() { return *value; }
    
    const T& unwrap_or(const T& default_value) const {
        return value ? *value : default_value;
    }
};

} // namespace usdtgverse
