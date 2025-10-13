/*
==============================================
 File:        bonus_manager.c
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Purchase Bonus System Implementation

 License:
   MIT License
==============================================
*/

#include "bonus_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>

// Data directory
#define DATA_DIR "/Users/irfangedik/usdtgverse-data/data"
#define BONUS_DB_FILE DATA_DIR "/bonuses.db"
#define USER_STATS_FILE DATA_DIR "/bonus_user_stats.db"

// Global statistics
static BonusSystemStats g_stats = {0};

// Initialize bonus system
bool bonus_init() {
    // Create data directory if not exists
    mkdir(DATA_DIR, 0755);
    
    // Initialize bonus database
    FILE* fp = fopen(BONUS_DB_FILE, "a+");
    if (!fp) {
        fprintf(stderr, "❌ Failed to initialize bonus database\n");
        return false;
    }
    
    // Write header if file is empty
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "# USDTgVerse Bonus Database\n");
        fprintf(fp, "# Created: %ld\n", time(NULL));
        fprintf(fp, "# Format: CSV with headers\n\n");
        fprintf(fp, "bonus_id,wallet_address,user_id,purchase_amount,bonus_amount,tier,bonus_percent,created_at,distributed_at,distributed,transaction_hash\n");
    }
    fclose(fp);
    
    // Initialize user stats database
    fp = fopen(USER_STATS_FILE, "a+");
    if (!fp) {
        fprintf(stderr, "❌ Failed to initialize user stats database\n");
        return false;
    }
    
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "# USDTgVerse Bonus User Statistics\n");
        fprintf(fp, "# Created: %ld\n", time(NULL));
        fprintf(fp, "# Format: CSV with headers\n\n");
        fprintf(fp, "wallet_address,total_purchases,total_bonuses,bonus_count,highest_tier,first_bonus,last_bonus,is_vip\n");
    }
    fclose(fp);
    
    printf("✅ Bonus system initialized\n");
    printf("📂 Database: %s\n", BONUS_DB_FILE);
    printf("💎 Tiers: Bronze → Silver → Gold → Platinum → Diamond\n");
    printf("🎁 Bonus: 10,000 USDTg → +10 USDTg (0.1%%)\n");
    
    return true;
}

// Get bonus tier for purchase amount
BonusTier bonus_get_tier(double purchase_amount) {
    if (purchase_amount >= BONUS_DIAMOND_THRESHOLD) {
        return BONUS_TIER_DIAMOND;
    } else if (purchase_amount >= BONUS_PLATINUM_THRESHOLD) {
        return BONUS_TIER_PLATINUM;
    } else if (purchase_amount >= BONUS_GOLD_THRESHOLD) {
        return BONUS_TIER_GOLD;
    } else if (purchase_amount >= BONUS_SILVER_THRESHOLD) {
        return BONUS_TIER_SILVER;
    } else if (purchase_amount >= BONUS_BRONZE_THRESHOLD) {
        return BONUS_TIER_BRONZE;
    }
    return BONUS_TIER_NONE;
}

// Get bonus percentage for tier
double bonus_get_percent(BonusTier tier) {
    switch (tier) {
        case BONUS_TIER_BRONZE: return BONUS_BRONZE_PERCENT;
        case BONUS_TIER_SILVER: return BONUS_SILVER_PERCENT;
        case BONUS_TIER_GOLD: return BONUS_GOLD_PERCENT;
        case BONUS_TIER_PLATINUM: return BONUS_PLATINUM_PERCENT;
        case BONUS_TIER_DIAMOND: return BONUS_DIAMOND_PERCENT;
        default: return 0.0;
    }
}

// Calculate bonus for purchase amount
double bonus_calculate(double purchase_amount) {
    BonusTier tier = bonus_get_tier(purchase_amount);
    
    switch (tier) {
        case BONUS_TIER_BRONZE:
            return BONUS_BRONZE_AMOUNT;
        case BONUS_TIER_SILVER:
            return BONUS_SILVER_AMOUNT;
        case BONUS_TIER_GOLD:
            return BONUS_GOLD_AMOUNT;
        case BONUS_TIER_PLATINUM:
            return BONUS_PLATINUM_AMOUNT;
        case BONUS_TIER_DIAMOND:
            return BONUS_DIAMOND_AMOUNT;
        default:
            return 0.0;
    }
}

// Check if purchase qualifies for bonus
bool bonus_qualifies(double purchase_amount) {
    return purchase_amount >= BONUS_BRONZE_THRESHOLD;
}

// Get tier name
const char* bonus_tier_name(BonusTier tier) {
    switch (tier) {
        case BONUS_TIER_BRONZE: return "Bronze";
        case BONUS_TIER_SILVER: return "Silver";
        case BONUS_TIER_GOLD: return "Gold";
        case BONUS_TIER_PLATINUM: return "Platinum";
        case BONUS_TIER_DIAMOND: return "Diamond";
        default: return "None";
    }
}

// Generate unique bonus ID
static void generate_bonus_id(char* buffer, size_t size) {
    snprintf(buffer, size, "BONUS_%ld_%d", time(NULL), rand() % 100000);
}

// Create bonus record for purchase
char* bonus_create(const char* wallet_address, const char* user_id, double purchase_amount) {
    // Check if qualifies
    if (!bonus_qualifies(purchase_amount)) {
        fprintf(stderr, "❌ Purchase amount too low for bonus\n");
        return NULL;
    }
    
    // Calculate bonus
    BonusTier tier = bonus_get_tier(purchase_amount);
    double bonus_amount = bonus_calculate(purchase_amount);
    double bonus_percent = bonus_get_percent(tier);
    
    // Create bonus record
    BonusRecord record = {0};
    generate_bonus_id(record.bonus_id, sizeof(record.bonus_id));
    strncpy(record.wallet_address, wallet_address, sizeof(record.wallet_address) - 1);
    strncpy(record.user_id, user_id, sizeof(record.user_id) - 1);
    record.purchase_amount = purchase_amount;
    record.bonus_amount = bonus_amount;
    record.tier = tier;
    record.bonus_percent = bonus_percent;
    record.created_at = time(NULL);
    record.distributed_at = 0;
    record.distributed = false;
    strcpy(record.transaction_hash, "pending");
    
    // Log to database
    if (!bonus_log_record(&record)) {
        fprintf(stderr, "❌ Failed to log bonus record\n");
        return NULL;
    }
    
    // Update user statistics
    bonus_update_user_stats(wallet_address, purchase_amount, bonus_amount, tier);
    
    // Update system statistics
    g_stats.total_bonuses++;
    g_stats.total_bonus_value += bonus_amount;
    g_stats.total_purchase_volume += purchase_amount;
    g_stats.bonuses_today++;
    
    switch (tier) {
        case BONUS_TIER_BRONZE:
            g_stats.bronze_count++;
            g_stats.bronze_value += bonus_amount;
            break;
        case BONUS_TIER_SILVER:
            g_stats.silver_count++;
            g_stats.silver_value += bonus_amount;
            break;
        case BONUS_TIER_GOLD:
            g_stats.gold_count++;
            g_stats.gold_value += bonus_amount;
            break;
        case BONUS_TIER_PLATINUM:
            g_stats.platinum_count++;
            g_stats.platinum_value += bonus_amount;
            break;
        case BONUS_TIER_DIAMOND:
            g_stats.diamond_count++;
            g_stats.diamond_value += bonus_amount;
            break;
        default:
            break;
    }
    
    printf("✅ Bonus created: %s\n", record.bonus_id);
    printf("💰 Purchase: %.2f USDTg\n", purchase_amount);
    printf("🎁 Bonus: %.2f USDTg (%s tier, %.2f%%)\n", 
           bonus_amount, bonus_tier_name(tier), bonus_percent);
    
    // Return bonus ID
    char* bonus_id = malloc(strlen(record.bonus_id) + 1);
    strcpy(bonus_id, record.bonus_id);
    return bonus_id;
}

// Distribute bonus to wallet
bool bonus_distribute(const char* bonus_id, const char* transaction_hash) {
    BonusRecord* record = bonus_get_record(bonus_id);
    if (!record) {
        fprintf(stderr, "❌ Bonus record not found\n");
        return false;
    }
    
    if (record->distributed) {
        fprintf(stderr, "⚠️  Bonus already distributed\n");
        free(record);
        return false;
    }
    
    // Mark as distributed
    record->distributed = true;
    record->distributed_at = time(NULL);
    strncpy(record->transaction_hash, transaction_hash, sizeof(record->transaction_hash) - 1);
    
    // Update database
    bonus_log_record(record);
    
    printf("✅ Bonus distributed: %s\n", bonus_id);
    printf("💎 Amount: %.2f USDTg\n", record->bonus_amount);
    printf("🔗 TX: %s\n", transaction_hash);
    
    free(record);
    return true;
}

// Get bonus record by ID
BonusRecord* bonus_get_record(const char* bonus_id) {
    FILE* fp = fopen(BONUS_DB_FILE, "r");
    if (!fp) return NULL;
    
    char line[2048];
    BonusRecord* record = NULL;
    
    // Skip header lines
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || strstr(line, "bonus_id,")) continue;
        
        // Check if bonus ID matches
        if (strstr(line, bonus_id)) {
            record = malloc(sizeof(BonusRecord));
            
            // Parse CSV line
            int tier_int, distributed_int;
            sscanf(line, "%[^,],%[^,],%[^,],%lf,%lf,%d,%lf,%ld,%ld,%d,%s",
                   record->bonus_id,
                   record->wallet_address,
                   record->user_id,
                   &record->purchase_amount,
                   &record->bonus_amount,
                   &tier_int,
                   &record->bonus_percent,
                   &record->created_at,
                   &record->distributed_at,
                   &distributed_int,
                   record->transaction_hash);
            
            record->tier = (BonusTier)tier_int;
            record->distributed = (bool)distributed_int;
            break;
        }
    }
    fclose(fp);
    
    return record;
}

// Get system bonus statistics
BonusSystemStats bonus_get_system_stats() {
    return g_stats;
}

// Log bonus record to database
bool bonus_log_record(const BonusRecord* record) {
    FILE* fp = fopen(BONUS_DB_FILE, "a");
    if (!fp) {
        fprintf(stderr, "❌ Failed to open bonus database\n");
        return false;
    }
    
    fprintf(fp, "%s,%s,%s,%.2f,%.2f,%d,%.2f,%ld,%ld,%d,%s\n",
            record->bonus_id,
            record->wallet_address,
            record->user_id,
            record->purchase_amount,
            record->bonus_amount,
            record->tier,
            record->bonus_percent,
            record->created_at,
            record->distributed_at,
            record->distributed,
            record->transaction_hash);
    
    fclose(fp);
    return true;
}

// Update user statistics
bool bonus_update_user_stats(const char* wallet_address, double purchase_amount, 
                              double bonus_amount, BonusTier tier) {
    // Read existing stats
    UserBonusStats stats = {0};
    bool found = false;
    
    FILE* fp = fopen(USER_STATS_FILE, "r");
    if (fp) {
        char line[2048];
        while (fgets(line, sizeof(line), fp)) {
            if (line[0] == '#' || strstr(line, "wallet_address,")) continue;
            
            if (strstr(line, wallet_address)) {
                int tier_int, vip_int;
                sscanf(line, "%[^,],%lf,%lf,%u,%d,%ld,%ld,%d",
                       stats.wallet_address,
                       &stats.total_purchases,
                       &stats.total_bonuses,
                       &stats.bonus_count,
                       &tier_int,
                       &stats.first_bonus,
                       &stats.last_bonus,
                       &vip_int);
                
                stats.highest_tier = (BonusTier)tier_int;
                stats.is_vip = (bool)vip_int;
                found = true;
                break;
            }
        }
        fclose(fp);
    }
    
    // Update stats
    if (!found) {
        strncpy(stats.wallet_address, wallet_address, sizeof(stats.wallet_address) - 1);
        stats.first_bonus = time(NULL);
    }
    
    stats.total_purchases += purchase_amount;
    stats.total_bonuses += bonus_amount;
    stats.bonus_count++;
    stats.last_bonus = time(NULL);
    
    if (tier > stats.highest_tier) {
        stats.highest_tier = tier;
    }
    
    // VIP status: 3+ bonuses or Diamond tier
    stats.is_vip = (stats.bonus_count >= 3) || (tier == BONUS_TIER_DIAMOND);
    
    // Write updated stats
    fp = fopen(USER_STATS_FILE, "a");
    if (!fp) return false;
    
    fprintf(fp, "%s,%.2f,%.2f,%u,%d,%ld,%ld,%d\n",
            stats.wallet_address,
            stats.total_purchases,
            stats.total_bonuses,
            stats.bonus_count,
            stats.highest_tier,
            stats.first_bonus,
            stats.last_bonus,
            stats.is_vip);
    
    fclose(fp);
    return true;
}

// Get user bonus statistics
UserBonusStats bonus_get_user_stats(const char* wallet_address) {
    UserBonusStats stats = {0};
    
    FILE* fp = fopen(USER_STATS_FILE, "r");
    if (!fp) return stats;
    
    char line[2048];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || strstr(line, "wallet_address,")) continue;
        
        if (strstr(line, wallet_address)) {
            int tier_int, vip_int;
            sscanf(line, "%[^,],%lf,%lf,%u,%d,%ld,%ld,%d",
                   stats.wallet_address,
                   &stats.total_purchases,
                   &stats.total_bonuses,
                   &stats.bonus_count,
                   &tier_int,
                   &stats.first_bonus,
                   &stats.last_bonus,
                   &vip_int);
            
            stats.highest_tier = (BonusTier)tier_int;
            stats.is_vip = (bool)vip_int;
            break;
        }
    }
    fclose(fp);
    
    return stats;
}

