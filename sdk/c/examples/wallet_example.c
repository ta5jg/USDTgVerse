/**
 * 🔑 USDTgVerse C SDK - Wallet Example
 * ===================================
 * 
 * This example demonstrates:
 * - Creating quantum-safe wallets
 * - Importing wallets from mnemonic
 * - Generating addresses for multiple networks
 * - Exporting private keys
 * - Validating addresses
 */

#include "../include/usdtgverse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_separator(const char* title) {
    printf("\n");
    printf("════════════════════════════════════════════════════════════════\n");
    printf("  %s\n", title);
    printf("════════════════════════════════════════════════════════════════\n");
}

void print_wallet_info(const usdtg_wallet_t* wallet) {
    printf("📍 Address: %s\n", wallet->address);
    printf("🌍 Network: %d\n", wallet->network);
    printf("⚛️ Quantum Safe: %s\n", wallet->quantum_safe ? "Yes" : "No");
    printf("🔢 Nonce: %llu\n", (unsigned long long)wallet->nonce);
    printf("💰 Balance: %llu wei\n", (unsigned long long)wallet->balance);
    printf("🔑 Mnemonic: %s\n", wallet->mnemonic);
    
    // Export private key
    char private_key_hex[129];
    usdtg_error_t result = usdtg_wallet_export_private_key(wallet, private_key_hex, sizeof(private_key_hex));
    if (result == USDTG_SUCCESS) {
        printf("🔐 Private Key: %s\n", private_key_hex);
    } else {
        printf("❌ Failed to export private key: %s\n", usdtg_error_message(result));
    }
}

int main() {
    printf("🚀 USDTgVerse C SDK - Wallet Example\n");
    printf("SDK Version: %s\n", usdtg_version());
    
    // Initialize SDK
    print_separator("🔧 SDK INITIALIZATION");
    usdtg_error_t result = usdtg_init(USDTG_NETWORK_MAINNET, "https://rpc.usdtgverse.com");
    if (result != USDTG_SUCCESS) {
        printf("❌ Failed to initialize SDK: %s\n", usdtg_error_message(result));
        return 1;
    }
    printf("✅ SDK initialized successfully!\n");
    
    // Create new quantum-safe wallet
    print_separator("🆕 CREATING QUANTUM-SAFE WALLET");
    usdtg_wallet_t new_wallet;
    result = usdtg_wallet_create(&new_wallet, 24, USDTG_NETWORK_MAINNET);
    if (result != USDTG_SUCCESS) {
        printf("❌ Failed to create wallet: %s\n", usdtg_error_message(result));
        usdtg_cleanup();
        return 1;
    }
    
    printf("✅ New quantum-safe wallet created!\n");
    print_wallet_info(&new_wallet);
    
    // Create wallets for different networks
    print_separator("🌍 MULTI-NETWORK WALLET CREATION");
    
    const struct {
        usdtg_network_t network;
        const char* name;
    } networks[] = {
        {USDTG_NETWORK_TESTNET, "USDTgVerse Testnet"},
        {USDTG_NETWORK_ETHEREUM, "Ethereum Mainnet"},
        {USDTG_NETWORK_BNB, "BNB Chain"},
        {USDTG_NETWORK_POLYGON, "Polygon"},
        {USDTG_NETWORK_ARBITRUM, "Arbitrum"},
        {USDTG_NETWORK_AVALANCHE, "Avalanche"}
    };
    
    for (size_t i = 0; i < sizeof(networks) / sizeof(networks[0]); i++) {
        usdtg_wallet_t network_wallet;
        result = usdtg_wallet_create(&network_wallet, 24, networks[i].network);
        
        if (result == USDTG_SUCCESS) {
            printf("\n🌐 %s Wallet:\n", networks[i].name);
            printf("   Address: %s\n", network_wallet.address);
            printf("   Quantum Safe: %s\n", network_wallet.quantum_safe ? "Yes" : "No");
        } else {
            printf("❌ Failed to create %s wallet: %s\n", 
                   networks[i].name, usdtg_error_message(result));
        }
    }
    
    // Import wallet from mnemonic
    print_separator("📥 IMPORTING WALLET FROM MNEMONIC");
    
    // Use the mnemonic from the previously created wallet
    usdtg_wallet_t imported_wallet;
    result = usdtg_wallet_import(&imported_wallet, new_wallet.mnemonic, USDTG_NETWORK_MAINNET);
    
    if (result == USDTG_SUCCESS) {
        printf("✅ Wallet imported successfully!\n");
        print_wallet_info(&imported_wallet);
        
        // Verify addresses match
        if (strcmp(new_wallet.address, imported_wallet.address) == 0) {
            printf("✅ Address verification: PASSED\n");
        } else {
            printf("❌ Address verification: FAILED\n");
            printf("   Original: %s\n", new_wallet.address);
            printf("   Imported: %s\n", imported_wallet.address);
        }
    } else {
        printf("❌ Failed to import wallet: %s\n", usdtg_error_message(result));
    }
    
    // Address validation examples
    print_separator("✅ ADDRESS VALIDATION");
    
    const struct {
        const char* address;
        usdtg_network_t network;
        const char* description;
    } test_addresses[] = {
        {new_wallet.address, USDTG_NETWORK_MAINNET, "Generated USDTgVerse address"},
        {"USDTg1qw2e3r4t5y6u7i8o9p0a1s2d3f4g5h6j7k8l9z0x1c2v3b4n5m", USDTG_NETWORK_MAINNET, "Valid USDTgVerse format"},
        {"0x742d35cc6634c0532925a3b8d2cc7a1c4c1aa6d1", USDTG_NETWORK_ETHEREUM, "Valid Ethereum address"},
        {"invalid_address", USDTG_NETWORK_MAINNET, "Invalid address"},
        {"0x742d35cc6634c0532925a3b8d2cc7a1c4c1aa6d1", USDTG_NETWORK_MAINNET, "Ethereum address on USDTgVerse network"}
    };
    
    for (size_t i = 0; i < sizeof(test_addresses) / sizeof(test_addresses[0]); i++) {
        bool is_valid = usdtg_wallet_validate_address(test_addresses[i].address, test_addresses[i].network);
        printf("%s %s: %s\n", 
               is_valid ? "✅" : "❌",
               test_addresses[i].description,
               test_addresses[i].address);
    }
    
    // Quantum security demonstration
    print_separator("⚛️ QUANTUM SECURITY FEATURES");
    
    printf("🔐 Quantum Security Level: %d-bit\n", USDTG_QUANTUM_SECURITY_LEVEL);
    printf("🔑 Private Key Length: %d bytes\n", USDTG_PRIVATE_KEY_LENGTH);
    printf("🔓 Public Key Length: %d bytes\n", USDTG_PUBLIC_KEY_LENGTH);
    printf("✍️ Signature Length: %d bytes\n", USDTG_SIGNATURE_LENGTH);
    printf("🏠 Address Length: %d characters\n", USDTG_ADDRESS_LENGTH);
    
    // Try to create wallet with insufficient mnemonic words (should fail)
    printf("\n🚨 Testing quantum security enforcement:\n");
    usdtg_wallet_t insecure_wallet;
    result = usdtg_wallet_create(&insecure_wallet, 12, USDTG_NETWORK_MAINNET);
    if (result == USDTG_ERROR_QUANTUM_SECURITY_VIOLATION) {
        printf("✅ Quantum security enforced: 12-word mnemonic rejected\n");
    } else {
        printf("❌ Security vulnerability: 12-word mnemonic accepted\n");
    }
    
    // Network configuration information
    print_separator("🌐 NETWORK CONFIGURATIONS");
    
    const usdtg_network_t networks_to_check[] = {
        USDTG_NETWORK_MAINNET,
        USDTG_NETWORK_TESTNET,
        USDTG_NETWORK_ETHEREUM
    };
    
    for (size_t i = 0; i < sizeof(networks_to_check) / sizeof(networks_to_check[0]); i++) {
        usdtg_network_config_t config;
        result = usdtg_network_get_config(networks_to_check[i], &config);
        
        if (result == USDTG_SUCCESS) {
            printf("\n🌍 %s:\n", config.name);
            printf("   Chain ID: %llu\n", (unsigned long long)config.chain_id);
            printf("   RPC URL: %s\n", config.rpc_url);
            printf("   Explorer: %s\n", config.explorer_url);
            printf("   Currency: %s\n", config.currency_symbol);
            printf("   Gas Limit: %llu\n", (unsigned long long)config.gas_limit);
            printf("   Gas Price: %llu gwei\n", (unsigned long long)config.gas_price / 1000000000);
            printf("   Quantum Safe: %s\n", config.quantum_safe ? "Yes" : "No");
        }
    }
    
    // Get current block number
    print_separator("📊 BLOCKCHAIN STATUS");
    
    uint64_t block_number;
    result = usdtg_network_get_block_number(&block_number);
    if (result == USDTG_SUCCESS) {
        printf("📦 Current Block Number: %llu\n", (unsigned long long)block_number);
    } else {
        printf("❌ Failed to get block number: %s\n", usdtg_error_message(result));
    }
    
    // Utility functions demonstration
    print_separator("🛠️ UTILITY FUNCTIONS");
    
    // Convert bytes to hex
    uint8_t sample_bytes[] = {0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    char hex_string[32];
    result = usdtg_utils_bytes_to_hex(sample_bytes, sizeof(sample_bytes), hex_string, sizeof(hex_string));
    if (result == USDTG_SUCCESS) {
        printf("📝 Bytes to Hex: ");
        for (size_t i = 0; i < sizeof(sample_bytes); i++) {
            printf("%02x ", sample_bytes[i]);
        }
        printf("→ %s\n", hex_string);
    }
    
    // Convert hex to bytes
    const char* test_hex = "123456789abcdef0";
    uint8_t converted_bytes[16];
    size_t actual_length;
    result = usdtg_utils_hex_to_bytes(test_hex, converted_bytes, sizeof(converted_bytes), &actual_length);
    if (result == USDTG_SUCCESS) {
        printf("🔢 Hex to Bytes: %s → ", test_hex);
        for (size_t i = 0; i < actual_length; i++) {
            printf("%02x ", converted_bytes[i]);
        }
        printf("(%zu bytes)\n", actual_length);
    }
    
    // Final summary
    print_separator("🎉 EXAMPLE COMPLETE");
    
    printf("✅ Successfully demonstrated:\n");
    printf("   • Quantum-safe wallet creation\n");
    printf("   • Multi-network wallet support\n");
    printf("   • Wallet import/export functionality\n");
    printf("   • Address validation\n");
    printf("   • Quantum security enforcement\n");
    printf("   • Network configuration queries\n");
    printf("   • Utility functions\n");
    
    printf("\n🚀 USDTgVerse C SDK is ready for production use!\n");
    
    // Cleanup
    usdtg_cleanup();
    
    return 0;
}
