#include "crypto/hash.hpp"
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <cstring>

namespace usdtgverse::crypto {

// ============================================================================
// BLAKE3 IMPLEMENTATION (Real BLAKE3 Library Integration)
// ============================================================================

#include "blake3/usdtg_blake3_wrapper.h"

struct Blake3Hasher::Blake3Context {
    blake3_hasher hasher;
    bool initialized;
};

Blake3Hasher::Blake3Hasher() : ctx_(std::make_unique<Blake3Context>()) {
    usdtg_blake3_init(&ctx_->hasher);
    ctx_->initialized = true;
}

Blake3Hasher::~Blake3Hasher() = default;

void Blake3Hasher::update(const void* data, size_t length) {
    if (ctx_->initialized) {
        usdtg_blake3_update(&ctx_->hasher, (const uint8_t*)data, length);
    }
}

void Blake3Hasher::update(const Bytes& data) {
    update(data.data(), data.size());
}

void Blake3Hasher::update(ByteSpan data) {
    update(data.data(), data.size());
}

void Blake3Hasher::update(const std::string& data) {
    update(data.data(), data.length());
}

::usdtgverse::Hash Blake3Hasher::finalize() {
    ::usdtgverse::Hash result;
    if (ctx_->initialized) {
        usdtg_blake3_finalize(&ctx_->hasher, result.data());
    }
    return result;
}

void Blake3Hasher::finalize(uint8_t* output, size_t length) {
    if (ctx_->initialized && length >= 32) {
        usdtg_blake3_finalize(&ctx_->hasher, output);
    }
}

void Blake3Hasher::reset() {
    usdtg_blake3_init(&ctx_->hasher);
    ctx_->initialized = true;
}

::usdtgverse::Hash Blake3Hasher::hash(const void* data, size_t length) {
    Blake3Hasher hasher;
    hasher.update(data, length);
    return hasher.finalize();
}

::usdtgverse::Hash Blake3Hasher::hash(const Bytes& data) {
    return hash(data.data(), data.size());
}

::usdtgverse::Hash Blake3Hasher::hash(const std::string& data) {
    return hash(data.data(), data.length());
}

// ============================================================================
// SHA-256 IMPLEMENTATION
// ============================================================================

struct Sha256Hasher::Sha256Context {
    SHA256_CTX ctx;
};

Sha256Hasher::Sha256Hasher() : ctx_(std::make_unique<Sha256Context>()) {
    SHA256_Init(&ctx_->ctx);
}

Sha256Hasher::~Sha256Hasher() = default;

void Sha256Hasher::update(const void* data, size_t length) {
    SHA256_Update(&ctx_->ctx, data, length);
}

void Sha256Hasher::update(const Bytes& data) {
    update(data.data(), data.size());
}

void Sha256Hasher::update(ByteSpan data) {
    update(data.data(), data.size());
}

void Sha256Hasher::update(const std::string& data) {
    update(data.data(), data.length());
}

::usdtgverse::Hash Sha256Hasher::finalize() {
    ::usdtgverse::Hash result;
    SHA256_Final(result.data(), &ctx_->ctx);
    return result;
}

void Sha256Hasher::reset() {
    SHA256_Init(&ctx_->ctx);
}

::usdtgverse::Hash Sha256Hasher::hash(const void* data, size_t length) {
    Sha256Hasher hasher;
    hasher.update(data, length);
    return hasher.finalize();
}

::usdtgverse::Hash Sha256Hasher::hash(const Bytes& data) {
    return hash(data.data(), data.size());
}

::usdtgverse::Hash Sha256Hasher::hash(const std::string& data) {
    return hash(data.data(), data.length());
}

// ============================================================================
// MAIN HASH CLASS IMPLEMENTATION
// ============================================================================

::usdtgverse::Hash Hash::blake3(const Bytes& data) {
    return Blake3Hasher::hash(data);
}

::usdtgverse::Hash Hash::blake3(ByteSpan data) {
    return Blake3Hasher::hash(data.data(), data.size());
}

::usdtgverse::Hash Hash::blake3(const std::string& data) {
    return Blake3Hasher::hash(data);
}

::usdtgverse::Hash Hash::blake3(const void* data, size_t length) {
    return Blake3Hasher::hash(data, length);
}

::usdtgverse::Hash Hash::sha256(const Bytes& data) {
    return Sha256Hasher::hash(data);
}

::usdtgverse::Hash Hash::sha256(ByteSpan data) {
    return Sha256Hasher::hash(data.data(), data.size());
}

::usdtgverse::Hash Hash::sha256(const std::string& data) {
    return Sha256Hasher::hash(data);
}

::usdtgverse::Hash Hash::sha256(const void* data, size_t length) {
    return Sha256Hasher::hash(data, length);
}

::usdtgverse::Hash Hash::double_blake3(const Bytes& data) {
    auto first_hash = blake3(data);
    return blake3(first_hash.data(), first_hash.size());
}

::usdtgverse::Hash Hash::double_sha256(const Bytes& data) {
    auto first_hash = sha256(data);
    return sha256(first_hash.data(), first_hash.size());
}

::usdtgverse::Hash Hash::merkle_combine(const ::usdtgverse::Hash& left, const ::usdtgverse::Hash& right) {
    Bytes combined;
    combined.reserve(64);
    combined.append(left.data(), left.size());
    combined.append(right.data(), right.size());
    return blake3(combined);
}

::usdtgverse::Hash Hash::merkle_root(const std::vector<::usdtgverse::Hash>& hashes) {
    if (hashes.empty()) {
        return ::usdtgverse::Hash{}; // All zeros
    }
    
    if (hashes.size() == 1) {
        return hashes[0];
    }
    
    std::vector<::usdtgverse::Hash> current_level = hashes;
    
    while (current_level.size() > 1) {
        std::vector<::usdtgverse::Hash> next_level;
        next_level.reserve((current_level.size() + 1) / 2);
        
        for (size_t i = 0; i < current_level.size(); i += 2) {
            if (i + 1 < current_level.size()) {
                next_level.push_back(merkle_combine(current_level[i], current_level[i + 1]));
            } else {
                // Odd number, duplicate the last hash
                next_level.push_back(merkle_combine(current_level[i], current_level[i]));
            }
        }
        
        current_level = std::move(next_level);
    }
    
    return current_level[0];
}

Address Hash::public_key_to_address(const PublicKey& public_key) {
    // Generate address from public key (Ethereum-style)
    auto hash = blake3(public_key.data(), public_key.size());
    
    Address address;
    std::copy(hash.begin() + 12, hash.begin() + 32, address.begin()); // Take last 20 bytes
    
    return address;
}

Address Hash::hash_to_address(const ::usdtgverse::Hash& hash) {
    Address address;
    std::copy(hash.begin(), hash.begin() + 20, address.begin()); // Take first 20 bytes
    return address;
}

std::string Hash::to_hex(const ::usdtgverse::Hash& hash) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (const auto& byte : hash) {
        oss << std::setw(2) << static_cast<unsigned>(byte);
    }
    return oss.str();
}

std::optional<::usdtgverse::Hash> Hash::from_hex(const std::string& hex) {
    if (hex.length() != 64) {
        return std::nullopt;
    }
    
    ::usdtgverse::Hash hash;
    
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

bool Hash::verify_hash(const ::usdtgverse::Hash& expected, const Bytes& data, bool use_blake3) {
    auto computed = use_blake3 ? blake3(data) : sha256(data);
    return expected == computed;
}

void Hash::benchmark_hash_functions() {
    std::cout << "\n🔬 USDTgVerse Crypto Benchmarks:\n";
    std::cout << "================================\n";
    
    std::vector<size_t> test_sizes = {1024, 4096, 16384, 65536, 1048576}; // 1KB to 1MB
    
    for (size_t size : test_sizes) {
        double blake3_speed = benchmark_blake3(size);
        double sha256_speed = benchmark_sha256(size);
        
        std::cout << "📊 " << size << " bytes:\n";
        std::cout << "   BLAKE3: " << std::fixed << std::setprecision(2) << blake3_speed << " MB/s\n";
        std::cout << "   SHA-256: " << std::fixed << std::setprecision(2) << sha256_speed << " MB/s\n";
        std::cout << "   Speedup: " << std::fixed << std::setprecision(1) << (blake3_speed / sha256_speed) << "x\n\n";
    }
}

double Hash::benchmark_blake3(size_t data_size, size_t iterations) {
    Bytes test_data(data_size);
    std::fill(test_data.begin(), test_data.end(), 0xAA);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        volatile auto hash = blake3(test_data);
        (void)hash; // Prevent optimization
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double total_bytes = static_cast<double>(data_size * iterations);
    double seconds = static_cast<double>(duration.count()) / 1000000.0;
    double mb_per_second = (total_bytes / (1024.0 * 1024.0)) / seconds;
    
    return mb_per_second;
}

double Hash::benchmark_sha256(size_t data_size, size_t iterations) {
    Bytes test_data(data_size);
    std::fill(test_data.begin(), test_data.end(), 0xAA);
    
    auto start = std::chrono::high_resolution_clock::now();
    
    for (size_t i = 0; i < iterations; ++i) {
        volatile auto hash = sha256(test_data);
        (void)hash; // Prevent optimization
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    double total_bytes = static_cast<double>(data_size * iterations);
    double seconds = static_cast<double>(duration.count()) / 1000000.0;
    double mb_per_second = (total_bytes / (1024.0 * 1024.0)) / seconds;
    
    return mb_per_second;
}

} // namespace usdtgverse::crypto

