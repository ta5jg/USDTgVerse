/*
==============================================
 File:        membership_core.h
 Author:      Irfan Gedik
 Created:     DD.MM.YYYY
 Last Update: 28.09.2025
 Version:     1.0

 Description:
   USDTgVerse Enterprise Membership System Core
   
   Comprehensive membership and KYC system featuring:
   - Individual and Corporate membership tiers
   - Advanced KYC/AML verification
   - Multi-level compliance
   - Enterprise trading privileges
   - VIP trading access
   - Institutional liquidity pools
   - Quantum-safe identity verification

 License:
   MIT License
==============================================
*/

#ifndef MEMBERSHIP_CORE_H
#define MEMBERSHIP_CORE_H

// Core Includes
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Quantum-Safe Includes
#include "../security/quantum_safe.h"

// ==========================================
// TYPES AND ENUMS
// ==========================================

typedef enum {
    MEMBERSHIP_INDIVIDUAL = 0,
    MEMBERSHIP_PROFESSIONAL = 1,
    MEMBERSHIP_CORPORATE = 2,
    MEMBERSHIP_INSTITUTIONAL = 3,
    MEMBERSHIP_VIP = 4,
    MEMBERSHIP_PARTNER = 5
} MembershipTier;

typedef enum {
    KYC_LEVEL_NONE = 0,
    KYC_LEVEL_BASIC = 1,
    KYC_LEVEL_ENHANCED = 2,
    KYC_LEVEL_VERIFIED = 3,
    KYC_LEVEL_CERTIFIED = 4
} KYCLevel;

typedef enum {
    STATUS_PENDING = 0,
    STATUS_ACTIVE = 1,
    STATUS_SUSPENDED = 2,
    STATUS_BANNED = 3,
    STATUS_VERIFIED = 4
} AccountStatus;

typedef enum {
    DOC_TYPE_PASSPORT = 1,
    DOC_TYPE_NATIONAL_ID = 2,
    DOC_TYPE_DRIVING_LICENCE = 3,
    DOC_TYPE_UTILITY_BILL = 4,
    DOC_TYPE_BANK_STATEMENT = 5,
    DOC_TYPE_COMPANY_REGISTRATION = 6,
    DOC_TYPE_TAX_CERTIFICATE = 7
} DocumentType;

typedef enum {
    TRADING_PERMISSION_SPOT = 1,
    TRADING_PERMISSION_MARGIN = 2,
    TRADING_PERMISSION_FUTURES = 3,
    TRADING_PERMISSION_DERIVATIVES = 4,
    TRADING_PERMISSION_OTC = 5
} TradingPermission;

// ==========================================
// DATA STRUCTURES
// ==========================================

typedef struct {
    uint8_t hash[32];     // Document hash
    char type;            // DocumentType
    uint64_t uploaded_at;
    uint64_t verified_at;
    bool is_verified;
    char mime_type[32];
    uint64_t file_size;
} DocumentInfo;

typedef struct {
    char name[128];
    char surname[128];
    char email[256];
    char phone[32];
    char country[3];       // ISO country code
    char nationality[3];  // ISO country code
    char date_of_birth[11]; // YYYY-MM-DD
    char address[512];
    char city[128];
    char postal_code[16];
    
    // KYC fields
    KYCLevel kyc_level;
    bool biometric_verified;
    bool liveness_check_passed;
    char face_hash[64];   // Biometric hash
    char fingerprints_hash[64];
    
    // Document verification
    DocumentInfo* documents;
    uint32_t document_count;
    
    // Quantum-safe verification
    QuantumSignature quantum_signature;
    
    uint64_t verified_at;
    uint64_t expires_at;
} KYCRecord;

typedef struct {
    char company_name[256];
    char legal_name[256];
    char tax_id[64];
    char registration_number[64];
    char legal_type[64];  // LLC, Corp, Ltd, etc.
    char incorporation_country[3];
    char incorporation_date[11];
    
    // Beneficial ownership (25%+ ownership)
    struct {
        char name[128];
        char ownership_percentage[8];
        char nationality[3];
        bool is_pep;        // Politically Exposed Person
        bool is_sanctioned;
    } beneficial_owners[10];
    uint32_t beneficial_owner_count;
    
    // Corporate documents
    DocumentInfo* corporate_documents;
    uint32_t corporate_document_count;
    
    // Banking information
    struct {
        char bank_name[256];
        char account_number[64];
        char routing_number[32];
        char swift_code[16];
    } primary_bank_account;
    
    // Compliance
    bool aml_compliant;
    bool fatf_compliant;
    bool sanctions_screening_passed;
    
    uint64_t verified_at;
    uint64_t expires_at;
} CorporateKYCRecord;

typedef struct {
    // Trading limits based on membership tier
    struct {
        uint64_t daily_withdrawal_limit;
        uint64_t monthly_trading_limit;
        uint64_t max_single_transaction;
        uint64_t otc_trading_limit;
    } limits;
    
    // Trading permissions
    bool permissions[8];  // TradingPermission bitfield
    
    // VIP features
    bool has_dedicated_support;
    bool has_priority_withdrawal;
    bool has_advanced_charting;
    bool has_api_trading;
    bool has_programmatic_access;
    
    // Institutional features
    bool has_automated_trading;
    bool telah_algorithmic_trading;
    bool has_smart_order_routing;
    bool has_dark_pool_access;
    bool has_prime_brokerage;
} MembershipBenefits;

typedef struct {
    char user_id[64];      // UUID or hashed identifier
    char wallet_address[76]; // Quantum-safe address
    
    // Membership info
    MembershipTier tier;
    AccountStatus status;
    KYCLevel kyc_level;
    
    // Personal/Corporate data
    union {
        KYCRecord individual;
        CorporateKYCRecord corporate;
    } kyc_data;
    bool is_corporate;
    
    // Trading info
    MembershipBenefits benefits;
    
    // Account info
    uint64_t created_at;
    uint64_t last_login;
    uint64_t last_trade;
    char ip_addresses[5][46];  // IPv6 max length
    uint32_t ip_count;
    
    // Security
    bool two_factor_enabled;
    bool biometric_login_enabled;
    uint32_t failed_login_attempts;
    uint64_t last_failed_login;
    
    // Activity tracking
    uint64_t total_volume_30d;
    uint64_t total_volume_all_time;
    uint32_t number_of_trades;
    char preferred_languages[3][8]; // ISO language codes
    uint32_t language_count;
    
    // Quantum-safe verification
    QuantumSignature quantum_signature;
    
    // Compliance flags
    bool pep_check_passed;
    bool sanctions_check_passed;
    bool aml_monitoring_active;
    uint64_t last_compliance_check;
} MembershipAccount;

// ==========================================
// CORE FUNCTIONS
// ==========================================

// Account Management
MembershipAccount* membership_account_create(const char* user_data, bool is_corporate);
int membership_account_verify(MembershipAccount* account);
int membership_account_update_tier(MembershipAccount* account, MembershipTier new_tier);
void membership_account_destroy(MembershipAccount* account);

// KYC Functions
int kyc_submit_document(MembershipAccount* account, DocumentInfo* document);
int kyc_verify_documents(MembershipAccount* account);
int kyc_enhanced_verification(MembershipAccount* account);
int kyc_biometric_verification(MembershipAccount* account, const char* biometric_data);
int kyc_liveness_check(MembershipAccount* account);

// Compliance & Verification
int compliance_pep_screening(MembershipAccount* account);
int compliance_sanctions_screening(MembershipAccount* account);
int compliance_aml_check(MembershipAccount* account);
int compliance_adverse_media_check(MembershipAccount* account);
int compliance_watchlist_check(MembershipAccount* account);

// Trading Permissions
bool membership_check_trading_permission(const MembershipAccount* account, TradingPermission permission);
uint64_t membership_get_trading_limit(const MembershipAccount* account, const char* limit_type);
bool membership_check_withdrawal_limit(const MembershipAccount* account, uint64_t amount);
int membership_update_trading_volume(MembershipAccount* account, uint64_t trade_amount);

// Premium Features
bool membership_has_vip_features(const MembershipAccount* account);
bool membership_has_institutional_access(const MembershipAccount* account);
bool membership_has_otc_trading(const MembershipAccount* account);
bool membership_has_prime_brokerage(const MembershipAccount* account);

// Security Functions
int membership_enable_two_factor(MembershipAccount* account, const char* secret_key);
bool membership_verify_two_factor(MembershipAccount* account, const char* token);
int membership_enable_biometric_login(MembershipAccount* account);
int membership_secure_login(MembershipAccount* account, const char* ip_address, const char* device_fingerprint);

// Quantum-Safe Functions
int membership_create_quantum_signature(MembershipAccount* account);
bool membership_verify_quantum_signature(const MembershipAccount* account);
int membership_quantum_safe_identity_proof(MembershipAccount* account);

// Reporting & Analytics
int membership_generate_compliance_report(const MembershipAccount* account);
int membership_calculate_risk_score(const MembershipAccount* account);
int membership_update_activity_log(MembershipAccount* account, const char* activity);

// Utility Functions
const char* membership_tier_to_string(MembershipTier tier);
MembershipTier membership_string_to_tier(const char* tier_str);
const char* kyc_level_to_string(KYCLevel level);
bool membership_is_account_active(const MembershipAccount* account);
uint64_t membership_get_account_age(const MembershipAccount* account);

// ==========================================
// CONSTANTS
// ==========================================

#define MAX_DOCUMENTS_PER_ACCOUNT 20
#define MAX_BENEFICIAL_OWNERS 10
#define MAX_IP_ADDRESSES 5
#define MAX_PREFERRED_LANGUAGES 3
#define KYC_VALIDITY_DAYS 365
#define COMPLIANCE_CHECK_INTERVAL 86400  // 24 hours

// Trading limits by tier (in USDTg)
#define INDIVIDUAL_DAILY_LIMIT 50000LLU    // $50K
#define PROFESSIONAL_DAILY_LIMIT 500000LLU // $500K
#define CORPORATE_DAILY_LIMIT 5000000LLU   // $5M
#define INSTITUTIONAL_DAILY_LIMIT 50000000LLU // $50M
#define VIP_TIMITLESS_AMOUNT 999999999999LLU

#endif // MEMBERSHIP_CORE_H
