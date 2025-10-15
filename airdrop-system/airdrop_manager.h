/*
==============================================
 File:        airdrop_manager.h
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Airdrop Management System
   
   Secure airdrop system featuring:
   - 10 USDTg initial airdrop for new wallets
   - Locked tokens until purchase verification
   - Fee-only usage restriction
   - Anti-abuse protection
   - Purchase verification system

 Security Rules:
   1. New wallet receives 10 USDTg (locked)
   2. Locked tokens can ONLY be used for transaction fees
   3. Unlocked after user purchases 50-100 USDTg
   4. One airdrop per verified identity (KYC)
   5. Anti-fraud detection system

 License:
   MIT License
==============================================
*/

#ifndef AIRDROP_MANAGER_H
#define AIRDROP_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Airdrop configuration
#define AIRDROP_AMOUNT 10.0                    // 10 USDTg per new wallet
#define MIN_PURCHASE_UNLOCK 50.0               // Minimum 50 USDTg purchase to unlock
#define MAX_PURCHASE_UNLOCK 100.0              // Recommended 100 USDTg purchase
#define MAX_AIRDROPS_PER_DAY 10000             // Daily limit for system protection
#define AIRDROP_EXPIRY_DAYS 365                // Airdrop expires after 1 year if not used

// Airdrop status
typedef enum {
    AIRDROP_STATUS_PENDING = 0,                // Airdrop requested but not sent
    AIRDROP_STATUS_LOCKED = 1,                 // Airdrop sent but locked (fee-only)
    AIRDROP_STATUS_UNLOCKED = 2,               // Unlocked after purchase
    AIRDROP_STATUS_EXPIRED = 3,                // Expired (not used within time limit)
    AIRDROP_STATUS_REVOKED = 4,                // Revoked due to fraud
    AIRDROP_STATUS_USED = 5                    // Fully used for fees
} AirdropStatus;

// Airdrop record
typedef struct {
    char airdrop_id[64];                       // Unique airdrop ID
    char wallet_address[128];                  // Recipient wallet address
    char user_id[64];                          // User ID
    double airdrop_amount;                     // Amount (10 USDTg)
    double locked_amount;                      // Currently locked amount
    double used_amount;                        // Amount used for fees
    double purchase_amount;                    // Amount user purchased
    AirdropStatus status;                      // Current status
    time_t created_at;                         // Creation timestamp
    time_t unlocked_at;                        // Unlock timestamp (0 if locked)
    time_t expires_at;                         // Expiration timestamp
    time_t last_used_at;                       // Last fee payment timestamp
    char device_fingerprint[256];              // Device fingerprint for fraud detection
    char ip_address[64];                       // IP address
    uint32_t fee_usage_count;                  // Number of times used for fees
    bool kyc_verified;                         // KYC verification status
} AirdropRecord;

// Airdrop statistics
typedef struct {
    uint64_t total_airdrops;                   // Total airdrops sent
    uint64_t locked_airdrops;                  // Currently locked
    uint64_t unlocked_airdrops;                // Unlocked airdrops
    uint64_t expired_airdrops;                 // Expired airdrops
    uint64_t revoked_airdrops;                 // Revoked due to fraud
    double total_airdrop_value;                // Total value distributed
    double locked_value;                       // Total locked value
    double used_for_fees;                      // Total used for fees
    uint32_t airdrops_today;                   // Airdrops sent today
    time_t last_reset;                         // Last daily reset
} AirdropStats;

// Function declarations

/**
 * Initialize airdrop system
 */
bool airdrop_init();

/**
 * Create new airdrop for wallet
 * @param wallet_address Recipient wallet address
 * @param user_id User ID
 * @param device_fingerprint Device fingerprint
 * @param ip_address IP address
 * @return Airdrop ID or NULL on failure
 */
char* airdrop_create(const char* wallet_address, const char* user_id, 
                     const char* device_fingerprint, const char* ip_address);

/**
 * Check if wallet is eligible for airdrop
 * @param wallet_address Wallet address to check
 * @param device_fingerprint Device fingerprint
 * @param ip_address IP address
 * @return true if eligible, false otherwise
 */
bool airdrop_check_eligibility(const char* wallet_address, 
                                const char* device_fingerprint,
                                const char* ip_address);

/**
 * Use locked airdrop for transaction fee
 * @param wallet_address Wallet address
 * @param fee_amount Fee amount to deduct
 * @return true if successful, false otherwise
 */
bool airdrop_use_for_fee(const char* wallet_address, double fee_amount);

/**
 * Verify purchase and unlock airdrop if threshold met
 * @param wallet_address Wallet address
 * @param purchase_amount Amount purchased
 * @return true if unlocked, false otherwise
 */
bool airdrop_verify_purchase(const char* wallet_address, double purchase_amount);

/**
 * Get airdrop record for wallet
 * @param wallet_address Wallet address
 * @return Airdrop record or NULL if not found
 */
AirdropRecord* airdrop_get_record(const char* wallet_address);

/**
 * Get airdrop statistics
 * @return Airdrop statistics
 */
AirdropStats airdrop_get_stats();

/**
 * Check if airdrop can be used for fee
 * @param wallet_address Wallet address
 * @param fee_amount Fee amount
 * @return true if can be used, false otherwise
 */
bool airdrop_can_use_for_fee(const char* wallet_address, double fee_amount);

/**
 * Revoke airdrop (fraud detection)
 * @param wallet_address Wallet address
 * @param reason Revocation reason
 * @return true if revoked, false otherwise
 */
bool airdrop_revoke(const char* wallet_address, const char* reason);

/**
 * Clean up expired airdrops
 * @return Number of airdrops cleaned up
 */
uint32_t airdrop_cleanup_expired();

/**
 * Check daily limit
 * @return true if under limit, false otherwise
 */
bool airdrop_check_daily_limit();

/**
 * Log airdrop record to database
 * @param record Airdrop record to log
 * @return true if successful, false otherwise
 */
bool airdrop_log_record(const AirdropRecord* record);

/**
 * Detect fraud patterns
 * @param wallet_address Wallet address
 * @param device_fingerprint Device fingerprint
 * @param ip_address IP address
 * @return true if fraud detected, false otherwise
 */
bool airdrop_detect_fraud(const char* wallet_address,
                          const char* device_fingerprint,
                          const char* ip_address);

#endif // AIRDROP_MANAGER_H

