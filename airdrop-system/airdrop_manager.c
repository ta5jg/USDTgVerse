/*
==============================================
 File:        airdrop_manager.c
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Airdrop Management System Implementation
   
   Secure airdrop system with locked tokens and
   purchase verification.

 License:
   MIT License
==============================================
*/

#include "airdrop_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

// Data directory
#define DATA_DIR "/Users/irfangedik/usdtgverse-data/data"
#define AIRDROP_DB_FILE DATA_DIR "/airdrops.db"
#define FRAUD_LOG_FILE DATA_DIR "/airdrop_fraud.log"

// Global statistics
static AirdropStats g_stats = {0};

// Initialize airdrop system
bool airdrop_init() {
    // Create data directory if not exists
    mkdir(DATA_DIR, 0755);
    
    // Initialize stats file if not exists
    FILE* fp = fopen(AIRDROP_DB_FILE, "a+");
    if (!fp) {
        fprintf(stderr, "❌ Failed to initialize airdrop database\n");
        return false;
    }
    
    // Write header if file is empty
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        fprintf(fp, "# USDTgVerse Airdrop Database\n");
        fprintf(fp, "# Created: %ld\n", time(NULL));
        fprintf(fp, "# Format: CSV with headers\n\n");
        fprintf(fp, "airdrop_id,wallet_address,user_id,airdrop_amount,locked_amount,used_amount,purchase_amount,status,created_at,unlocked_at,expires_at,last_used_at,device_fingerprint,ip_address,fee_usage_count,kyc_verified\n");
    }
    fclose(fp);
    
    printf("✅ Airdrop system initialized\n");
    printf("📂 Database: %s\n", AIRDROP_DB_FILE);
    printf("💰 Airdrop amount: %.2f USDTg (locked)\n", AIRDROP_AMOUNT);
    printf("🔓 Unlock threshold: %.2f - %.2f USDTg purchase\n", MIN_PURCHASE_UNLOCK, MAX_PURCHASE_UNLOCK);
    printf("⚠️  Locked tokens can ONLY be used for transaction fees\n");
    
    return true;
}

// Generate unique airdrop ID
static void generate_airdrop_id(char* buffer, size_t size) {
    snprintf(buffer, size, "AIRDROP_%ld_%d", time(NULL), rand() % 100000);
}

// Check if wallet is eligible for airdrop
bool airdrop_check_eligibility(const char* wallet_address, 
                                const char* device_fingerprint,
                                const char* ip_address) {
    // Check daily limit
    if (!airdrop_check_daily_limit()) {
        fprintf(stderr, "❌ Daily airdrop limit reached\n");
        return false;
    }
    
    // Check if wallet already received airdrop
    FILE* fp = fopen(AIRDROP_DB_FILE, "r");
    if (!fp) return true; // If file doesn't exist, eligible
    
    char line[2048];
    bool found = false;
    
    // Skip header lines
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || strstr(line, "airdrop_id,")) continue;
        
        // Check if wallet address exists
        if (strstr(line, wallet_address)) {
            found = true;
            break;
        }
    }
    fclose(fp);
    
    if (found) {
        fprintf(stderr, "❌ Wallet already received airdrop\n");
        return false;
    }
    
    // Check for fraud patterns
    if (airdrop_detect_fraud(wallet_address, device_fingerprint, ip_address)) {
        fprintf(stderr, "❌ Fraud detected - airdrop denied\n");
        return false;
    }
    
    return true;
}

// Create new airdrop for wallet
char* airdrop_create(const char* wallet_address, const char* user_id, 
                     const char* device_fingerprint, const char* ip_address) {
    // Check eligibility
    if (!airdrop_check_eligibility(wallet_address, device_fingerprint, ip_address)) {
        return NULL;
    }
    
    // Create airdrop record
    AirdropRecord record = {0};
    generate_airdrop_id(record.airdrop_id, sizeof(record.airdrop_id));
    strncpy(record.wallet_address, wallet_address, sizeof(record.wallet_address) - 1);
    strncpy(record.user_id, user_id, sizeof(record.user_id) - 1);
    strncpy(record.device_fingerprint, device_fingerprint, sizeof(record.device_fingerprint) - 1);
    strncpy(record.ip_address, ip_address, sizeof(record.ip_address) - 1);
    
    record.airdrop_amount = AIRDROP_AMOUNT;
    record.locked_amount = AIRDROP_AMOUNT;
    record.used_amount = 0.0;
    record.purchase_amount = 0.0;
    record.status = AIRDROP_STATUS_LOCKED;
    record.created_at = time(NULL);
    record.unlocked_at = 0;
    record.expires_at = time(NULL) + (AIRDROP_EXPIRY_DAYS * 24 * 3600);
    record.last_used_at = 0;
    record.fee_usage_count = 0;
    record.kyc_verified = false;
    
    // Log to database
    if (!airdrop_log_record(&record)) {
        fprintf(stderr, "❌ Failed to log airdrop record\n");
        return NULL;
    }
    
    // Update statistics
    g_stats.total_airdrops++;
    g_stats.locked_airdrops++;
    g_stats.total_airdrop_value += AIRDROP_AMOUNT;
    g_stats.locked_value += AIRDROP_AMOUNT;
    g_stats.airdrops_today++;
    
    printf("✅ Airdrop created: %s\n", record.airdrop_id);
    printf("💰 Amount: %.2f USDTg (LOCKED - Fee only)\n", AIRDROP_AMOUNT);
    printf("🔓 Unlock after purchasing %.2f+ USDTg\n", MIN_PURCHASE_UNLOCK);
    
    // Return airdrop ID
    char* airdrop_id = malloc(strlen(record.airdrop_id) + 1);
    strcpy(airdrop_id, record.airdrop_id);
    return airdrop_id;
}

// Use locked airdrop for transaction fee
bool airdrop_use_for_fee(const char* wallet_address, double fee_amount) {
    if (!airdrop_can_use_for_fee(wallet_address, fee_amount)) {
        return false;
    }
    
    // Get airdrop record
    AirdropRecord* record = airdrop_get_record(wallet_address);
    if (!record) {
        fprintf(stderr, "❌ No airdrop found for wallet\n");
        return false;
    }
    
    // Deduct fee from locked amount
    record->locked_amount -= fee_amount;
    record->used_amount += fee_amount;
    record->last_used_at = time(NULL);
    record->fee_usage_count++;
    
    // Update status if fully used
    if (record->locked_amount <= 0.001) {
        record->status = AIRDROP_STATUS_USED;
    }
    
    // Update database
    airdrop_log_record(record);
    
    // Update statistics
    g_stats.used_for_fees += fee_amount;
    g_stats.locked_value -= fee_amount;
    
    printf("✅ Fee paid from airdrop: %.6f USDTg\n", fee_amount);
    printf("💰 Remaining locked: %.6f USDTg\n", record->locked_amount);
    
    free(record);
    return true;
}

// Verify purchase and unlock airdrop if threshold met
bool airdrop_verify_purchase(const char* wallet_address, double purchase_amount) {
    // Get airdrop record
    AirdropRecord* record = airdrop_get_record(wallet_address);
    if (!record) {
        return false;
    }
    
    // Update purchase amount
    record->purchase_amount += purchase_amount;
    
    // Check if unlock threshold met
    if (record->purchase_amount >= MIN_PURCHASE_UNLOCK && record->status == AIRDROP_STATUS_LOCKED) {
        record->status = AIRDROP_STATUS_UNLOCKED;
        record->unlocked_at = time(NULL);
        
        // Update statistics
        g_stats.locked_airdrops--;
        g_stats.unlocked_airdrops++;
        
        printf("🔓 AIRDROP UNLOCKED!\n");
        printf("💰 %.2f USDTg now available for all transactions\n", record->locked_amount);
        printf("🎉 Purchase threshold met: %.2f USDTg\n", record->purchase_amount);
    } else {
        printf("💰 Purchase recorded: %.2f USDTg\n", purchase_amount);
        printf("🔒 Need %.2f more USDTg to unlock airdrop\n", 
               MIN_PURCHASE_UNLOCK - record->purchase_amount);
    }
    
    // Update database
    airdrop_log_record(record);
    
    free(record);
    return true;
}

// Get airdrop record for wallet
AirdropRecord* airdrop_get_record(const char* wallet_address) {
    FILE* fp = fopen(AIRDROP_DB_FILE, "r");
    if (!fp) return NULL;
    
    char line[2048];
    AirdropRecord* record = NULL;
    
    // Skip header lines
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || strstr(line, "airdrop_id,")) continue;
        
        // Check if wallet address matches
        if (strstr(line, wallet_address)) {
            record = malloc(sizeof(AirdropRecord));
            
            // Parse CSV line
            sscanf(line, "%[^,],%[^,],%[^,],%lf,%lf,%lf,%lf,%d,%ld,%ld,%ld,%ld,%[^,],%[^,],%u,%d",
                   record->airdrop_id,
                   record->wallet_address,
                   record->user_id,
                   &record->airdrop_amount,
                   &record->locked_amount,
                   &record->used_amount,
                   &record->purchase_amount,
                   (int*)&record->status,
                   &record->created_at,
                   &record->unlocked_at,
                   &record->expires_at,
                   &record->last_used_at,
                   record->device_fingerprint,
                   record->ip_address,
                   &record->fee_usage_count,
                   (int*)&record->kyc_verified);
            break;
        }
    }
    fclose(fp);
    
    return record;
}

// Check if airdrop can be used for fee
bool airdrop_can_use_for_fee(const char* wallet_address, double fee_amount) {
    AirdropRecord* record = airdrop_get_record(wallet_address);
    if (!record) {
        return false;
    }
    
    // Check if airdrop is locked or unlocked (both can pay fees)
    if (record->status != AIRDROP_STATUS_LOCKED && 
        record->status != AIRDROP_STATUS_UNLOCKED) {
        free(record);
        return false;
    }
    
    // Check if enough balance
    if (record->locked_amount < fee_amount) {
        fprintf(stderr, "❌ Insufficient airdrop balance for fee\n");
        free(record);
        return false;
    }
    
    // Check if expired
    if (time(NULL) > record->expires_at) {
        fprintf(stderr, "❌ Airdrop expired\n");
        free(record);
        return false;
    }
    
    free(record);
    return true;
}

// Get airdrop statistics
AirdropStats airdrop_get_stats() {
    return g_stats;
}

// Check daily limit
bool airdrop_check_daily_limit() {
    time_t now = time(NULL);
    
    // Reset daily counter if new day
    if (now - g_stats.last_reset > 86400) {
        g_stats.airdrops_today = 0;
        g_stats.last_reset = now;
    }
    
    return g_stats.airdrops_today < MAX_AIRDROPS_PER_DAY;
}

// Log airdrop record to database
bool airdrop_log_record(const AirdropRecord* record) {
    FILE* fp = fopen(AIRDROP_DB_FILE, "a");
    if (!fp) {
        fprintf(stderr, "❌ Failed to open airdrop database\n");
        return false;
    }
    
    fprintf(fp, "%s,%s,%s,%.6f,%.6f,%.6f,%.6f,%d,%ld,%ld,%ld,%ld,%s,%s,%u,%d\n",
            record->airdrop_id,
            record->wallet_address,
            record->user_id,
            record->airdrop_amount,
            record->locked_amount,
            record->used_amount,
            record->purchase_amount,
            record->status,
            record->created_at,
            record->unlocked_at,
            record->expires_at,
            record->last_used_at,
            record->device_fingerprint,
            record->ip_address,
            record->fee_usage_count,
            record->kyc_verified);
    
    fclose(fp);
    return true;
}

// Detect fraud patterns
bool airdrop_detect_fraud(const char* wallet_address,
                          const char* device_fingerprint,
                          const char* ip_address) {
    FILE* fp = fopen(AIRDROP_DB_FILE, "r");
    if (!fp) return false;
    
    char line[2048];
    int same_device_count = 0;
    int same_ip_count = 0;
    
    // Skip header lines
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || strstr(line, "airdrop_id,")) continue;
        
        // Check for same device fingerprint
        if (strstr(line, device_fingerprint)) {
            same_device_count++;
        }
        
        // Check for same IP address
        if (strstr(line, ip_address)) {
            same_ip_count++;
        }
    }
    fclose(fp);
    
    // Fraud detection rules
    if (same_device_count >= 3) {
        fprintf(stderr, "🚨 FRAUD: Multiple airdrops from same device\n");
        return true;
    }
    
    if (same_ip_count >= 5) {
        fprintf(stderr, "🚨 FRAUD: Multiple airdrops from same IP\n");
        return true;
    }
    
    return false;
}

// Revoke airdrop
bool airdrop_revoke(const char* wallet_address, const char* reason) {
    AirdropRecord* record = airdrop_get_record(wallet_address);
    if (!record) {
        return false;
    }
    
    record->status = AIRDROP_STATUS_REVOKED;
    airdrop_log_record(record);
    
    // Log fraud
    FILE* fp = fopen(FRAUD_LOG_FILE, "a");
    if (fp) {
        fprintf(fp, "[%ld] Revoked: %s - Reason: %s\n", 
                time(NULL), wallet_address, reason);
        fclose(fp);
    }
    
    printf("⚠️  Airdrop revoked: %s\n", wallet_address);
    printf("📝 Reason: %s\n", reason);
    
    free(record);
    return true;
}

// Clean up expired airdrops
uint32_t airdrop_cleanup_expired() {
    // TODO: Implement cleanup logic
    return 0;
}

