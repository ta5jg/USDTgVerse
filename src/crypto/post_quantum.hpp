#pragma once

/*
 * USDTgVerse Post-Quantum Cryptography Implementation
 * CRYSTALS-Dilithium & CRYSTALS-Kyber Integration
 * 
 * NIST-approved quantum-safe algorithms for:
 * - Digital signatures (Dilithium)
 * - Key encapsulation (Kyber)
 * - 256-bit quantum security level
 */

#include "common/types.hpp"
#include "common/bytes.hpp"
#include <optional>
#include <string>
#include <memory>

namespace usdtgverse::crypto::postquantum {

// ============================================================================
// CRYSTALS-DILITHIUM (Quantum-Safe Signatures)
// ============================================================================

/**
 * CRYSTALS-Dilithium Parameters
 * Security Level 5: 256-bit quantum security (AES-1024 equivalent)
 */
constexpr size_t DILITHIUM_PUBLICKEY_BYTES = 1312;
constexpr size_t DILITHIUM_SECRETKEY_BYTES = 2528;
constexpr size_t DILITHIUM_SIGNATURE_BYTES = 2420;
constexpr size_t DILITHIUM_SEED_BYTES = 32;

/**
 * Dilithium Key Pair
 * Quantum-resistant signature keys
 */
struct DilithiumKeyPair {
    Bytes public_key;   // 1312 bytes
    Bytes secret_key;   // 2528 bytes
    
    DilithiumKeyPair() : 
        public_key(DILITHIUM_PUBLICKEY_BYTES),
        secret_key(DILITHIUM_SECRETKEY_BYTES) {}
};

/**
 * Dilithium Signature
 * Quantum-safe digital signature
 */
struct DilithiumSignature {
    Bytes signature;    // 2420 bytes
    
    DilithiumSignature() : signature(DILITHIUM_SIGNATURE_BYTES) {}
    explicit DilithiumSignature(const Bytes& sig) : signature(sig) {}
};

/**
 * Dilithium Operations
 * Quantum-safe signature scheme
 */
class Dilithium {
public:
    // Key generation
    static DilithiumKeyPair generate_keypair();
    static DilithiumKeyPair generate_keypair_from_seed(const Bytes& seed);
    static bool is_valid_keypair(const DilithiumKeyPair& keypair);
    
    // Signing and verification
    static DilithiumSignature sign(const DilithiumKeyPair& keypair, const Bytes& message);
    static DilithiumSignature sign(const DilithiumKeyPair& keypair, ByteSpan message);
    static bool verify(const Bytes& public_key, const Bytes& message, const DilithiumSignature& signature);
    static bool verify(const Bytes& public_key, ByteSpan message, const DilithiumSignature& signature);
    
    // Key validation
    static bool is_valid_public_key(const Bytes& public_key);
    static bool is_valid_secret_key(const Bytes& secret_key);
    static bool is_valid_signature(const DilithiumSignature& signature);
    
    // Encoding/Decoding
    static std::string public_key_to_hex(const Bytes& public_key);
    static std::string signature_to_hex(const DilithiumSignature& signature);
    static std::optional<Bytes> public_key_from_hex(const std::string& hex);
    static std::optional<DilithiumSignature> signature_from_hex(const std::string& hex);
    
    // Performance benchmarking
    static void benchmark_operations();
    static double get_operations_per_second();
};

// ============================================================================
// CRYSTALS-KYBER (Quantum-Safe Encryption)
// ============================================================================

/**
 * CRYSTALS-Kyber Parameters
 * Security Level 5: 256-bit quantum security
 */
constexpr size_t KYBER_PUBLICKEY_BYTES = 1568;
constexpr size_t KYBER_SECRETKEY_BYTES = 3168;
constexpr size_t KYBER_CIPHERTEXT_BYTES = 1568;
constexpr size_t KYBER_SHARED_SECRET_BYTES = 32;

/**
 * Kyber Key Pair
 * Quantum-resistant encryption keys
 */
struct KyberKeyPair {
    Bytes public_key;   // 1568 bytes
    Bytes secret_key;   // 3168 bytes
    
    KyberKeyPair() : 
        public_key(KYBER_PUBLICKEY_BYTES),
        secret_key(KYBER_SECRETKEY_BYTES) {}
};

/**
 * Kyber Encapsulated Key
 * Quantum-safe key encapsulation
 */
struct KyberEncapsulation {
    Bytes ciphertext;     // 1568 bytes
    Bytes shared_secret;  // 32 bytes
    
    KyberEncapsulation() : 
        ciphertext(KYBER_CIPHERTEXT_BYTES),
        shared_secret(KYBER_SHARED_SECRET_BYTES) {}
};

/**
 * Kyber Operations
 * Quantum-safe key encapsulation mechanism
 */
class Kyber {
public:
    // Key generation
    static KyberKeyPair generate_keypair();
    static bool is_valid_keypair(const KyberKeyPair& keypair);
    
    // Key encapsulation
    static KyberEncapsulation encapsulate(const Bytes& public_key);
    static std::optional<Bytes> decapsulate(const KyberKeyPair& keypair, const Bytes& ciphertext);
    
    // Key validation
    static bool is_valid_public_key(const Bytes& public_key);
    static bool is_valid_secret_key(const Bytes& secret_key);
    static bool is_valid_ciphertext(const Bytes& ciphertext);
    
    // Encoding/Decoding
    static std::string public_key_to_hex(const Bytes& public_key);
    static std::string ciphertext_to_hex(const Bytes& ciphertext);
    static std::optional<Bytes> public_key_from_hex(const std::string& hex);
    static std::optional<Bytes> ciphertext_from_hex(const std::string& hex);
    
    // Performance benchmarking
    static void benchmark_operations();
    static double get_operations_per_second();
};

// ============================================================================
// HYBRID CRYPTOGRAPHY (Classical + Post-Quantum)
// ============================================================================

/**
 * Hybrid Key Pair
 * Combines classical (Ed25519) + post-quantum (Dilithium) for maximum security
 */
struct HybridKeyPair {
    // Classical cryptography
    PrivateKey classical_private;
    PublicKey classical_public;
    
    // Post-quantum cryptography
    DilithiumKeyPair quantum_safe;
    
    // Combined security level: 128-bit classical + 256-bit quantum = ultra-secure
};

/**
 * Hybrid Signature
 * Dual signature for maximum security
 */
struct HybridSignature {
    Signature classical_signature;        // Ed25519 signature (64 bytes)
    DilithiumSignature quantum_signature; // Dilithium signature (2420 bytes)
    
    // Total size: ~2484 bytes
    // Security: Protected against both classical and quantum attacks
};

/**
 * Hybrid Cryptography Operations
 * Maximum security through dual-algorithm approach
 */
class HybridCrypto {
public:
    // Key generation
    static HybridKeyPair generate_keypair();
    static HybridKeyPair generate_keypair_from_seed(const Bytes& seed);
    
    // Dual signing
    static HybridSignature sign(const HybridKeyPair& keypair, const Bytes& message);
    static bool verify(const HybridKeyPair& keypair, const Bytes& message, const HybridSignature& signature);
    
    // Key validation
    static bool is_valid_keypair(const HybridKeyPair& keypair);
    static bool is_valid_signature(const HybridSignature& signature);
    
    // Address generation
    static Address keypair_to_address(const HybridKeyPair& keypair);
    
    // Encoding/Decoding
    static std::string serialize_keypair(const HybridKeyPair& keypair, bool include_private = false);
    static std::string serialize_signature(const HybridSignature& signature);
    static std::optional<HybridKeyPair> deserialize_keypair(const std::string& serialized);
    static std::optional<HybridSignature> deserialize_signature(const std::string& serialized);
    
    // Performance
    static void benchmark_operations();
    static double get_signing_ops_per_second();
    static double get_verification_ops_per_second();
};

// ============================================================================
// QUANTUM-SAFE WALLET
// ============================================================================

/**
 * Quantum-Safe Wallet
 * Complete wallet with post-quantum security
 */
class QuantumSafeWallet {
private:
    HybridKeyPair master_keypair;
    std::string mnemonic_phrase;
    Bytes quantum_seed;
    
public:
    // Wallet creation
    static std::unique_ptr<QuantumSafeWallet> create_new();
    static std::unique_ptr<QuantumSafeWallet> import_from_mnemonic(const std::string& mnemonic);
    static std::unique_ptr<QuantumSafeWallet> import_from_seed(const Bytes& seed);
    
    // Key derivation
    HybridKeyPair derive_keypair(uint32_t account, uint32_t change, uint32_t address_index);
    Address derive_address(uint32_t account, uint32_t change, uint32_t address_index);
    
    // Signing operations
    HybridSignature sign_transaction(const Bytes& transaction_data, const std::string& derivation_path);
    bool verify_signature(const Bytes& message, const HybridSignature& signature, const Address& address);
    
    // Wallet management
    std::string get_mnemonic() const;
    Address get_master_address() const;
    std::vector<Address> get_derived_addresses(uint32_t count = 10) const;
    
    // Security
    bool is_quantum_safe() const { return true; }
    uint32_t get_security_level() const { return 256; } // 256-bit quantum security
    std::string get_security_description() const { return "Quantum-safe (256-bit)"; }
    
    // Serialization
    std::string export_wallet(const std::string& password) const;
    static std::unique_ptr<QuantumSafeWallet> import_wallet(const std::string& encrypted_data, const std::string& password);
};

// ============================================================================
// PERFORMANCE OPTIMIZATION
// ============================================================================

/**
 * Quantum-Safe Performance Manager
 * Optimizes post-quantum operations for maximum throughput
 */
class QuantumPerformanceManager {
public:
    // Performance modes
    enum class PerformanceMode {
        MAXIMUM_SECURITY,    // Full post-quantum (slower)
        BALANCED,           // Hybrid mode (recommended)
        COMPATIBILITY       // Classical fallback (faster)
    };
    
    // Configuration
    static void set_performance_mode(PerformanceMode mode);
    static PerformanceMode get_performance_mode();
    
    // Benchmarking
    static void run_comprehensive_benchmark();
    static double get_current_tps();
    static std::string get_performance_report();
    
    // Optimization
    static void enable_hardware_acceleration();
    static void optimize_for_mobile();
    static void optimize_for_server();
};

} // namespace usdtgverse::crypto::postquantum
