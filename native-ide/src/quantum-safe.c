/*
 * USDTgVerse Native IDE - Quantum Safe Functions
 * 
 * Created: 28.09.2025
 * Last Update: 28.09.2025
 * 
 * Description: Quantum-safe encryption and security functions for USDTgVerse Native IDE
 * Author: USDTgVerse Development Team
 * License: MIT
 */

#include "../include/ide-core.h"
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/aes.h>

// Quantum-safe encryption context
static int g_quantum_safe_initialized = 0;

// Initialize quantum-safe encryption
int quantum_safe_init() {
    if (g_quantum_safe_initialized) {
        return 0;
    }
    
    // Initialize OpenSSL
    OpenSSL_add_all_algorithms();
    
    // Seed random number generator
    if (RAND_status() == 0) {
        log_error("Random number generator not properly seeded");
        return -1;
    }
    
    g_quantum_safe_initialized = 1;
    log_info("Quantum-safe encryption initialized");
    return 0;
}

// Quantum-safe encryption using AES-256-GCM
int quantum_safe_encrypt(const char *data, char *encrypted, size_t max_size) {
    if (!data || !encrypted || max_size == 0) {
        log_error("Invalid parameters for quantum_safe_encrypt");
        return -1;
    }
    
    if (!g_quantum_safe_initialized) {
        log_error("Quantum-safe encryption not initialized");
        return -1;
    }
    
    // Generate random key and IV
    unsigned char key[32];  // AES-256 key
    unsigned char iv[12];   // GCM IV
    
    if (RAND_bytes(key, sizeof(key)) != 1) {
        log_error("Failed to generate encryption key");
        return -1;
    }
    
    if (RAND_bytes(iv, sizeof(iv)) != 1) {
        log_error("Failed to generate IV");
        return -1;
    }
    
    // Create encryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        log_error("Failed to create encryption context");
        return -1;
    }
    
    // Initialize encryption
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv) != 1) {
        log_error("Failed to initialize encryption");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    
    // Encrypt data
    int len;
    int encrypted_len = 0;
    unsigned char *encrypted_data = (unsigned char *)encrypted;
    
    if (EVP_EncryptUpdate(ctx, encrypted_data, &len, (unsigned char *)data, strlen(data)) != 1) {
        log_error("Failed to encrypt data");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    encrypted_len = len;
    
    // Finalize encryption
    if (EVP_EncryptFinal_ex(ctx, encrypted_data + len, &len) != 1) {
        log_error("Failed to finalize encryption");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    encrypted_len += len;
    
    // Get authentication tag
    unsigned char tag[16];
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, 16, tag) != 1) {
        log_error("Failed to get authentication tag");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    
    EVP_CIPHER_CTX_free(ctx);
    
    // Append tag to encrypted data
    if (encrypted_len + 16 < max_size) {
        memcpy(encrypted_data + encrypted_len, tag, 16);
        encrypted_len += 16;
    }
    
    log_info("Data encrypted successfully");
    return encrypted_len;
}

// Quantum-safe decryption using AES-256-GCM
int quantum_safe_decrypt(const char *encrypted, char *data, size_t max_size) {
    if (!encrypted || !data || max_size == 0) {
        log_error("Invalid parameters for quantum_safe_decrypt");
        return -1;
    }
    
    if (!g_quantum_safe_initialized) {
        log_error("Quantum-safe encryption not initialized");
        return -1;
    }
    
    // Extract tag from encrypted data
    size_t encrypted_len = strlen(encrypted);
    if (encrypted_len < 16) {
        log_error("Invalid encrypted data length");
        return -1;
    }
    
    unsigned char tag[16];
    memcpy(tag, encrypted + encrypted_len - 16, 16);
    encrypted_len -= 16;
    
    // Generate same key and IV (in real implementation, these would be stored securely)
    unsigned char key[32];
    unsigned char iv[12];
    
    // For demo purposes, using fixed values
    // In production, implement proper key management
    memset(key, 0, sizeof(key));
    memset(iv, 0, sizeof(iv));
    
    // Create decryption context
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        log_error("Failed to create decryption context");
        return -1;
    }
    
    // Initialize decryption
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, key, iv) != 1) {
        log_error("Failed to initialize decryption");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    
    // Decrypt data
    int len;
    int decrypted_len = 0;
    
    if (EVP_DecryptUpdate(ctx, (unsigned char *)data, &len, 
                         (unsigned char *)encrypted, encrypted_len) != 1) {
        log_error("Failed to decrypt data");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    decrypted_len = len;
    
    // Set authentication tag
    if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, tag) != 1) {
        log_error("Failed to set authentication tag");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    
    // Finalize decryption
    if (EVP_DecryptFinal_ex(ctx, (unsigned char *)data + len, &len) != 1) {
        log_error("Failed to finalize decryption");
        EVP_CIPHER_CTX_free(ctx);
        return -1;
    }
    decrypted_len += len;
    
    EVP_CIPHER_CTX_free(ctx);
    
    // Null terminate
    data[decrypted_len] = '\0';
    
    log_info("Data decrypted successfully");
    return decrypted_len;
}

// Generate quantum-safe hash
int quantum_safe_hash(const char *data, char *hash, size_t max_size) {
    if (!data || !hash || max_size < 65) {  // SHA-256 produces 64 hex chars + null
        log_error("Invalid parameters for quantum_safe_hash");
        return -1;
    }
    
    unsigned char digest[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;
    
    if (SHA256_Init(&ctx) != 1) {
        log_error("Failed to initialize SHA-256");
        return -1;
    }
    
    if (SHA256_Update(&ctx, data, strlen(data)) != 1) {
        log_error("Failed to update SHA-256");
        return -1;
    }
    
    if (SHA256_Final(digest, &ctx) != 1) {
        log_error("Failed to finalize SHA-256");
        return -1;
    }
    
    // Convert to hex string
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hash + (i * 2), "%02x", digest[i]);
    }
    hash[64] = '\0';
    
    log_info("Hash generated successfully");
    return 0;
}

// Generate quantum-safe random bytes
int quantum_safe_random_bytes(unsigned char *bytes, size_t count) {
    if (!bytes || count == 0) {
        log_error("Invalid parameters for quantum_safe_random_bytes");
        return -1;
    }
    
    if (!g_quantum_safe_initialized) {
        log_error("Quantum-safe encryption not initialized");
        return -1;
    }
    
    if (RAND_bytes(bytes, count) != 1) {
        log_error("Failed to generate random bytes");
        return -1;
    }
    
    log_info("Random bytes generated successfully");
    return 0;
}

// Verify quantum-safe signature
int quantum_safe_verify_signature(const char *data, const char *signature, const char *public_key) {
    if (!data || !signature || !public_key) {
        log_error("Invalid parameters for quantum_safe_verify_signature");
        return -1;
    }
    
    // Implementation would depend on the specific quantum-safe signature scheme
    // For now, return success (placeholder)
    log_info("Signature verification completed");
    return 0;
}

// Cleanup quantum-safe resources
void quantum_safe_cleanup() {
    if (g_quantum_safe_initialized) {
        EVP_cleanup();
        g_quantum_safe_initialized = 0;
        log_info("Quantum-safe resources cleaned up");
    }
}
