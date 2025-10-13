/*
==============================================
 File:        bonus_manager.h
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Purchase Bonus System
   
   Reward system for large purchases:
   - 10,000 USDTg purchase → +10 USDTg bonus
   - VIP tier system
   - Automatic bonus distribution
   - Whale incentive program

 Bonus Tiers:
   Bronze:   10,000 USDTg → +10 USDTg (0.10%)
   Silver:   50,000 USDTg → +75 USDTg (0.15%)
   Gold:     100,000 USDTg → +200 USDTg (0.20%)
   Platinum: 500,000 USDTg → +1,500 USDTg (0.30%)
   Diamond:  1,000,000 USDTg → +5,000 USDTg (0.50%)

 License:
   MIT License
==============================================
*/

#ifndef BONUS_MANAGER_H
#define BONUS_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Bonus tiers
typedef enum {
    BONUS_TIER_NONE = 0,
    BONUS_TIER_BRONZE = 1,      // 10K USDTg
    BONUS_TIER_SILVER = 2,      // 50K USDTg
    BONUS_TIER_GOLD = 3,        // 100K USDTg
    BONUS_TIER_PLATINUM = 4,    // 500K USDTg
    BONUS_TIER_DIAMOND = 5      // 1M USDTg
} BonusTier;

// Bonus configuration
#define BONUS_BRONZE_THRESHOLD 10000.0
#define BONUS_SILVER_THRESHOLD 50000.0
#define BONUS_GOLD_THRESHOLD 100000.0
#define BONUS_PLATINUM_THRESHOLD 500000.0
#define BONUS_DIAMOND_THRESHOLD 1000000.0

#define BONUS_BRONZE_AMOUNT 10.0
#define BONUS_SILVER_AMOUNT 75.0
#define BONUS_GOLD_AMOUNT 200.0
#define BONUS_PLATINUM_AMOUNT 1500.0
#define BONUS_DIAMOND_AMOUNT 5000.0

#define BONUS_BRONZE_PERCENT 0.10
#define BONUS_SILVER_PERCENT 0.15
#define BONUS_GOLD_PERCENT 0.20
#define BONUS_PLATINUM_PERCENT 0.30
#define BONUS_DIAMOND_PERCENT 0.50

// Bonus record
typedef struct {
    char bonus_id[64];
    char wallet_address[128];
    char user_id[64];
    double purchase_amount;
    double bonus_amount;
    BonusTier tier;
    double bonus_percent;
    time_t created_at;
    time_t distributed_at;
    bool distributed;
    char transaction_hash[128];
} BonusRecord;

// User bonus statistics
typedef struct {
    char wallet_address[128];
    double total_purchases;
    double total_bonuses;
    uint32_t bonus_count;
    BonusTier highest_tier;
    time_t first_bonus;
    time_t last_bonus;
    bool is_vip;
} UserBonusStats;

// System bonus statistics
typedef struct {
    uint64_t total_bonuses;
    double total_bonus_value;
    double total_purchase_volume;
    uint32_t bronze_count;
    uint32_t silver_count;
    uint32_t gold_count;
    uint32_t platinum_count;
    uint32_t diamond_count;
    double bronze_value;
    double silver_value;
    double gold_value;
    double platinum_value;
    double diamond_value;
    uint32_t bonuses_today;
    time_t last_reset;
} BonusSystemStats;

// Function declarations

/**
 * Initialize bonus system
 */
bool bonus_init();

/**
 * Calculate bonus for purchase amount
 * @param purchase_amount Purchase amount in USDTg
 * @return Bonus amount in USDTg
 */
double bonus_calculate(double purchase_amount);

/**
 * Get bonus tier for purchase amount
 * @param purchase_amount Purchase amount in USDTg
 * @return Bonus tier
 */
BonusTier bonus_get_tier(double purchase_amount);

/**
 * Get bonus percentage for tier
 * @param tier Bonus tier
 * @return Bonus percentage
 */
double bonus_get_percent(BonusTier tier);

/**
 * Create bonus record for purchase
 * @param wallet_address Wallet address
 * @param user_id User ID
 * @param purchase_amount Purchase amount
 * @return Bonus ID or NULL on failure
 */
char* bonus_create(const char* wallet_address, const char* user_id, double purchase_amount);

/**
 * Distribute bonus to wallet
 * @param bonus_id Bonus ID
 * @param transaction_hash Transaction hash
 * @return true if successful, false otherwise
 */
bool bonus_distribute(const char* bonus_id, const char* transaction_hash);

/**
 * Get bonus record by ID
 * @param bonus_id Bonus ID
 * @return Bonus record or NULL if not found
 */
BonusRecord* bonus_get_record(const char* bonus_id);

/**
 * Get user bonus statistics
 * @param wallet_address Wallet address
 * @return User bonus statistics
 */
UserBonusStats bonus_get_user_stats(const char* wallet_address);

/**
 * Get system bonus statistics
 * @return System bonus statistics
 */
BonusSystemStats bonus_get_system_stats();

/**
 * Check if purchase qualifies for bonus
 * @param purchase_amount Purchase amount
 * @return true if qualifies, false otherwise
 */
bool bonus_qualifies(double purchase_amount);

/**
 * Get tier name
 * @param tier Bonus tier
 * @return Tier name string
 */
const char* bonus_tier_name(BonusTier tier);

/**
 * Log bonus record to database
 * @param record Bonus record to log
 * @return true if successful, false otherwise
 */
bool bonus_log_record(const BonusRecord* record);

/**
 * Update user statistics
 * @param wallet_address Wallet address
 * @param purchase_amount Purchase amount
 * @param bonus_amount Bonus amount
 * @param tier Bonus tier
 * @return true if successful, false otherwise
 */
bool bonus_update_user_stats(const char* wallet_address, double purchase_amount, 
                              double bonus_amount, BonusTier tier);

#endif // BONUS_MANAGER_H

