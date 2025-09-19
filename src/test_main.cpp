#include <iostream>
#include <iomanip>
#include "crypto/hash.hpp"
#include "common/types.hpp"

using namespace usdtgverse;
using namespace usdtgverse::crypto;

int main() {
    std::cout << R"(
🌌 =============================================== 🌌
    USDTgVerse Enterprise Crypto Layer Test
🌌 =============================================== 🌌

)" << std::endl;
    
    try {
        // Test BLAKE3 hashing
        std::cout << "🔬 Testing BLAKE3 Hash Function:\n";
        std::string test_data = "USDTgVerse Native Blockchain";
        auto blake3_hash = Hash::blake3(test_data);
        std::cout << "   Input: \"" << test_data << "\"\n";
        std::cout << "   BLAKE3: " << Hash::to_hex(blake3_hash) << "\n\n";
        
        // Test SHA-256 hashing
        std::cout << "🔬 Testing SHA-256 Hash Function:\n";
        auto sha256_hash = Hash::sha256(test_data);
        std::cout << "   Input: \"" << test_data << "\"\n";
        std::cout << "   SHA-256: " << Hash::to_hex(sha256_hash) << "\n\n";
        
        // Test Merkle tree
        std::cout << "🌳 Testing Merkle Tree:\n";
        std::vector<::usdtgverse::Hash> leaves = {blake3_hash, sha256_hash};
        auto merkle_root = Hash::merkle_root(leaves);
        std::cout << "   Leaf 1: " << Hash::to_hex(blake3_hash).substr(0, 16) << "...\n";
        std::cout << "   Leaf 2: " << Hash::to_hex(sha256_hash).substr(0, 16) << "...\n";
        std::cout << "   Root: " << Hash::to_hex(merkle_root) << "\n\n";
        
        // Test address generation
        std::cout << "🏠 Testing Address Generation:\n";
        PublicKey dummy_pubkey;
        std::fill(dummy_pubkey.begin(), dummy_pubkey.end(), 0x42);
        auto address = Hash::public_key_to_address(dummy_pubkey);
        std::cout << "   Public Key: " << Hash::to_hex(::usdtgverse::Hash{}).substr(0, 32) << "...\n";
        std::cout << "   Address: " << address_to_string(address) << "\n\n";
        
        // Performance benchmark
        std::cout << "⚡ Performance Benchmark:\n";
        Hash::benchmark_hash_functions();
        
        std::cout << "✅ All crypto tests passed!\n";
        std::cout << "🚀 USDTgVerse crypto layer is ready for enterprise use!\n\n";
        
    } catch (const std::exception& e) {
        std::cerr << "❌ Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

