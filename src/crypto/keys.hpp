#pragma once

#include "common/types.hpp"
#include "common/bytes.hpp"
#include <optional>
#include <string>

namespace usdtgverse::crypto {

/**
 * Ed25519 Key Management
 * 
 * USDTgVerse uses Ed25519 as the primary signature scheme:
 * - 128-bit security level
 * - Fast signature generation and verification
 * - Deterministic signatures
 * - Small key and signature sizes
 */
class Ed25519 {
public:
    // Key generation
    static std::pair<PrivateKey, PublicKey> generate_keypair();
    static std::pair<PrivateKey, PublicKey> generate_keypair_from_seed(const Bytes& seed);
    static PublicKey derive_public_key(const PrivateKey& private_key);
    
    // Signing and verification
    static Signature sign(const PrivateKey& private_key, const Bytes& message);
    static Signature sign(const PrivateKey& private_key, ByteSpan message);
    static bool verify(const PublicKey& public_key, const Bytes& message, const Signature& signature);
    static bool verify(const PublicKey& public_key, ByteSpan message, const Signature& signature);
    
    // Key validation
    static bool is_valid_private_key(const PrivateKey& private_key);
    static bool is_valid_public_key(const PublicKey& public_key);
    static bool is_valid_signature(const Signature& signature);
    
    // Key derivation (BIP32-Ed25519 style)
    static PrivateKey derive_private_key(const PrivateKey& parent_key, uint32_t index);
    static PublicKey derive_public_key_unhardened(const PublicKey& parent_key, uint32_t index);
    
    // Encoding/Decoding
    static std::string private_key_to_hex(const PrivateKey& key);
    static std::string public_key_to_hex(const PublicKey& key);
    static std::string signature_to_hex(const Signature& sig);
    
    static std::optional<PrivateKey> private_key_from_hex(const std::string& hex);
    static std::optional<PublicKey> public_key_from_hex(const std::string& hex);
    static std::optional<Signature> signature_from_hex(const std::string& hex);
    
    // Address generation
    static Address public_key_to_address(const PublicKey& public_key);
    
    // Benchmarking
    static void benchmark_operations();
};

/**
 * secp256k1 Support (Bitcoin/Ethereum compatibility)
 * Optional - only used for cross-chain compatibility
 */
class Secp256k1 {
public:
    // Key generation
    static std::pair<PrivateKey, SecpPublicKey> generate_keypair();
    static SecpPublicKey derive_public_key(const PrivateKey& private_key);
    
    // Signing and verification
    static SecpSignature sign(const PrivateKey& private_key, const ::usdtgverse::Hash& message_hash);
    static bool verify(const SecpPublicKey& public_key, const ::usdtgverse::Hash& message_hash, const SecpSignature& signature);
    
    // Recovery (Ethereum-style)
    static std::optional<SecpPublicKey> recover_public_key(const ::usdtgverse::Hash& message_hash, const SecpSignature& signature);
    
    // Address generation (Ethereum-style)
    static Address public_key_to_ethereum_address(const SecpPublicKey& public_key);
    
    // Validation
    static bool is_valid_private_key(const PrivateKey& private_key);
    static bool is_valid_public_key(const SecpPublicKey& public_key);
    
    // Encoding
    static std::string public_key_to_hex(const SecpPublicKey& key);
    static std::optional<SecpPublicKey> public_key_from_hex(const std::string& hex);
};

/**
 * Key Derivation Functions
 * BIP32-Ed25519 implementation for hierarchical deterministic keys
 */
class KeyDerivation {
public:
    struct ExtendedKey {
        PrivateKey private_key;
        PublicKey public_key;
        Bytes chain_code;
        uint32_t depth;
        uint32_t parent_fingerprint;
        uint32_t child_index;
        
        ExtendedKey() : depth(0), parent_fingerprint(0), child_index(0) {}
    };
    
    // Master key generation
    static ExtendedKey generate_master_key(const Bytes& seed);
    static ExtendedKey generate_master_key_from_mnemonic(const std::string& mnemonic, const std::string& passphrase = "");
    
    // Child key derivation
    static ExtendedKey derive_child_key(const ExtendedKey& parent, uint32_t index, bool hardened = true);
    static ExtendedKey derive_key_path(const ExtendedKey& master, const std::string& path); // e.g., "m/44'/2024'/0'/0/0"
    
    // Serialization
    static std::string serialize_extended_key(const ExtendedKey& key, bool include_private = false);
    static std::optional<ExtendedKey> deserialize_extended_key(const std::string& serialized);
    
    // Validation
    static bool is_valid_derivation_path(const std::string& path);
    static std::vector<uint32_t> parse_derivation_path(const std::string& path);
};

/**
 * Mnemonic Support (BIP39)
 * For user-friendly key backup and recovery
 */
class Mnemonic {
public:
    // Generate mnemonic
    static std::string generate_mnemonic(size_t entropy_bits = 256); // 24 words default
    static std::string generate_mnemonic_from_entropy(const Bytes& entropy);
    
    // Validation
    static bool validate_mnemonic(const std::string& mnemonic);
    static std::vector<std::string> split_mnemonic(const std::string& mnemonic);
    
    // Seed generation
    static Bytes mnemonic_to_seed(const std::string& mnemonic, const std::string& passphrase = "");
    
    // Entropy
    static Bytes generate_entropy(size_t bits = 256);
    static Bytes mnemonic_to_entropy(const std::string& mnemonic);
    
    // Wordlist
    static const std::vector<std::string>& get_english_wordlist();
    static bool is_valid_word(const std::string& word);
};

/**
 * Secure Random Number Generation
 */
class SecureRandom {
public:
    // Generate cryptographically secure random bytes
    static Bytes generate_bytes(size_t length);
    static uint32_t generate_uint32();
    static uint64_t generate_uint64();
    
    // Generate random keys
    static PrivateKey generate_private_key();
    static Bytes generate_seed(size_t length = 32);
    static Bytes generate_salt(size_t length = 16);
    
    // Validation
    static bool validate_entropy(const Bytes& data);
    static double calculate_entropy(const Bytes& data);
};

} // namespace usdtgverse::crypto

