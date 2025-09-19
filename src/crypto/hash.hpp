#pragma once

#include "common/types.hpp"
#include "common/bytes.hpp"

namespace usdtgverse::crypto {

/**
 * High-Performance Cryptographic Hash Functions
 * 
 * USDTgVerse uses BLAKE3 as primary hash (ultra-fast)
 * and SHA-256 for compatibility with existing systems.
 * 
 * Performance targets:
 * - BLAKE3: >1GB/s on modern CPUs
 * - SHA-256: >500MB/s with hardware acceleration
 */
class Hash {
public:
    // BLAKE3 - Primary hash function (fastest)
    static ::usdtgverse::Hash blake3(const Bytes& data);
    static ::usdtgverse::Hash blake3(ByteSpan data);
    static ::usdtgverse::Hash blake3(const std::string& data);
    static ::usdtgverse::Hash blake3(const void* data, size_t length);
    
    // SHA-256 - Compatibility hash function
    static ::usdtgverse::Hash sha256(const Bytes& data);
    static ::usdtgverse::Hash sha256(ByteSpan data);
    static ::usdtgverse::Hash sha256(const std::string& data);
    static ::usdtgverse::Hash sha256(const void* data, size_t length);
    
    // Double hashing (for extra security)
    static ::usdtgverse::Hash double_blake3(const Bytes& data);
    static ::usdtgverse::Hash double_sha256(const Bytes& data);
    
    // Merkle tree operations
    static ::usdtgverse::Hash merkle_combine(const ::usdtgverse::Hash& left, const ::usdtgverse::Hash& right);
    static ::usdtgverse::Hash merkle_root(const std::vector<::usdtgverse::Hash>& hashes);
    
    // Key derivation
    static ::usdtgverse::Hash derive_key(const Bytes& seed, const std::string& info);
    static std::vector<uint8_t> hkdf_expand(const Bytes& prk, const std::string& info, size_t length);
    
    // Address generation
    static Address public_key_to_address(const PublicKey& public_key);
    static Address hash_to_address(const ::usdtgverse::Hash& hash);
    
    // Utility functions
    static std::string to_hex(const ::usdtgverse::Hash& hash);
    static std::optional<::usdtgverse::Hash> from_hex(const std::string& hex);
    static bool verify_hash(const ::usdtgverse::Hash& expected, const Bytes& data, bool use_blake3 = true);
    
    // Benchmarking
    static void benchmark_hash_functions();
    static double benchmark_blake3(size_t data_size, size_t iterations = 1000);
    static double benchmark_sha256(size_t data_size, size_t iterations = 1000);
};

// ============================================================================
// BLAKE3 IMPLEMENTATION
// ============================================================================

class Blake3Hasher {
private:
    struct Blake3Context;
    std::unique_ptr<Blake3Context> ctx_;

public:
    Blake3Hasher();
    ~Blake3Hasher();
    
    // Streaming interface
    void update(const void* data, size_t length);
    void update(const Bytes& data);
    void update(ByteSpan data);
    void update(const std::string& data);
    
    // Finalize hash
    ::usdtgverse::Hash finalize();
    void finalize(uint8_t* output, size_t length);
    
    // Reset for reuse
    void reset();
    
    // One-shot convenience
    static ::usdtgverse::Hash hash(const void* data, size_t length);
    static ::usdtgverse::Hash hash(const Bytes& data);
    static ::usdtgverse::Hash hash(const std::string& data);
};

// ============================================================================
// SHA-256 IMPLEMENTATION
// ============================================================================

class Sha256Hasher {
private:
    struct Sha256Context;
    std::unique_ptr<Sha256Context> ctx_;

public:
    Sha256Hasher();
    ~Sha256Hasher();
    
    // Streaming interface
    void update(const void* data, size_t length);
    void update(const Bytes& data);
    void update(ByteSpan data);
    void update(const std::string& data);
    
    // Finalize hash
    ::usdtgverse::Hash finalize();
    
    // Reset for reuse
    void reset();
    
    // One-shot convenience
    static ::usdtgverse::Hash hash(const void* data, size_t length);
    static ::usdtgverse::Hash hash(const Bytes& data);
    static ::usdtgverse::Hash hash(const std::string& data);
};

// ============================================================================
// CONVENIENCE FUNCTIONS
// ============================================================================

// Primary hash function (BLAKE3)
inline ::usdtgverse::Hash hash_data(const Bytes& data) {
    return Hash::blake3(data);
}

inline ::usdtgverse::Hash hash_data(const std::string& data) {
    return Hash::blake3(data);
}

inline ::usdtgverse::Hash hash_data(const void* data, size_t length) {
    return Hash::blake3(data, length);
}

// Block ID calculation (SHA-256 for compatibility)
inline BlockHash calculate_block_id(const Bytes& block_header) {
    return Hash::sha256(block_header);
}

// Transaction ID calculation (BLAKE3 for speed)
inline TxHash calculate_tx_id(const Bytes& transaction_data) {
    return Hash::blake3(transaction_data);
}

// State root calculation (BLAKE3)
inline StateRoot calculate_state_root(const std::vector<::usdtgverse::Hash>& account_hashes) {
    return Hash::merkle_root(account_hashes);
}

} // namespace usdtgverse::crypto

