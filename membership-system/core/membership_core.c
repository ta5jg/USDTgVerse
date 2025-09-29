/*
==============================================
 File:        membership_core.c
 Author:      Irfan Gedik
 Created:     DD.MM.YYYY
 Last Update: 28.09.2025
 Version:     1.0

 Description:
   USDTgVerse Enterprise Membership System Implementation

 License:
   MIT License
==============================================
*/

#include "membership_core.h"
#include "../security/quantum_safe.c"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

// ==========================================
// HELPER FUNCTIONS
// ==========================================

static uint64_t current_timestamp_ms() {
    return (uint64_t)time(NULL) * 1000;
}

static void generate_user_id(char* user_id) {
    uint8_t random_bytes[16];
    RAND_bytes(random_bytes, 16);
    
    for (int i = 0; i < 16; i++) {
        sprintf(user_id + (i * 2), "%02x", random_bytes[i]);
    }
    user_id[32] = '\0';
}

static bool validate_email(const char* email) {
    if (!email) return false;
    
    const char* at_pos = strchr(email, '@');
    if (!at_pos || at_pos == email) return false;
    
    const char* dot_pos = strrchr(at_pos, '.');
    if (!dot_pos || dot_pos <= at_pos + 1) return false;
    
    return true;
}

static bool validate_country_code(const char* country) {
    if (!country || strlen(country) != 2) return false;
    
    // Basic ISO country code validation (simplified)
    char upper_country[3];
    upper_country[0] = toupper(country[0]);
    upper_country[1] = toupper(country[1]);
    upper_country[2] = '\0';
    
    return (upper_country[0] >= 'A' && upper_country[0] <= 'Z') &&
           (upper_country[1] >= 'A' && upper_country[1] <= 'Z');
}

// ==========================================
// ACCOUNT MANAGEMENT
// ==========================================

MembershipAccount* membership_account_create(const char* user_data, bool is_corporate) {
    MembershipAccount* account = malloc(sizeof(MembershipAccount));
    if (!account) return NULL;
    
    memset(account, 0, sizeof(MembershipAccount));
    
    // Generate unique user ID
    generate_user_id(account->user_id);
    
    // Set creation time
    account->created_at = current_timestamp_ms();
    account->status = STATUS_PENDING;
    account->kyc_level = KYC_LEVEL_NONE;
    account->is_corporate = is_corporate;
    
    // Initialize quantum signature
    quantum_signature_init(&account->quantum_signature);
    
    // Set default tier based on account type
    account->tier = is_corporate ? MEMBERSHIP_CORPORATE : MEMBERSHIP_INDIVIDUAL;
    
    // Set default limits
    membership_set_default_limits(account);
    
    printf("Created membership account: %s (Tier: %s)\n", 
           account->user_id, membership_tier_to_string(account->tier));
    
    return account;
}

int membership_account_verify(MembershipAccount* account) {
    if (!account) return -1;
    
    // Check basic account validity
    if (strlen(account->user_id) != 32) return -2; // Invalid user ID
    
    // Verify KYC data
    if (account->is_corporate) {
        CorporateKYCRecord* corp_kyc = &account->kyc_data.corporate;
        if (!corp_kyc->company_name[0]) return -3; // Missing company name
        if (!validate_country_code(corp_kyc->incorporation_country)) return -4;
    } else {
        KYCRecord* individual_kyc = &account->kyc_data.individual;
        if (!individual_kyc->name[0]) return -3; // Missing name
        if (!validate_email(individual_kyc->email)) return -4;
        if (!validate_country_code(individual_kyc->country)) return -5;
    }
    
    // Update account status
    account->status = STATUS_ACTIVE;
    account->last_compliance_check = current_timestamp_ms();
    
    printf("Verified membership account: %s\n", account->user_id);
    return 0;
}

int membership_account_update_tier(MembershipAccount* account, MembershipTier new_tier) {
    if (!account) return -1;
    
    // Validate tier upgrade eligibility
    if (new_tier < account->tier) {
        // Downgrade requires additional verification
        account->status = STATUS_PENDING;
        account->kyc_level = KYC_LEVEL_BASIC;
    }
    
    MembershipTier old_tier = account->tier;
    account->tier = new_tier;
    
    // Update benefits based on new tier
    membership_set_default_limits(account);
    
    printf("Updated account %s from %s to %s\n", 
           account->user_id, 
           membership_tier_to_string(old_tier),
           membership_tier_to_string(new_tier));
    
    return 0;
}

void membership_account_destroy(MembershipAccount* account) {
    if (!account) return;
    
    // Clean up documents
    if (account->is_corporate && account->kyc_data.corporate.corporate_documents) {
        free(account->kyc_data.corporate.corporate_documents);
    } else if (!account->is_corporate && account->kyc_data.individual.documents) {
        free(account->kyc_data.individual.documents);
    }
    
    // Clean up quantum signature
    quantum_signature_cleanup(&account->quantum_signature);
    
    free(account);
}

// ==========================================
// KYC FUNCTIONS
// ==========================================

int kyc_submit_document(MembershipAccount* account, DocumentInfo* document) {
    if (!account || !document) return -1;
    
    if (account->is_corporate) {
        CorporateKYCRecord* kyc = &account->kyc_data.corporate;
        
        if (kyc->corporate_document_count >= MAX_DOCUMENTS_PER_ACCOUNT) {
            return -2; // Too many documents
        }
        
        // Reallocate document array
        kyc->corporate_documents = realloc(kyc->corporate_documents, 
                                          sizeof(DocumentInfo) * (kyc->corporate_document_count + 1));
        
        if (!kyc->corporate_documents) return -3; // Memory error
        
        // Add document
        memcpy(&kyc->corporate_documents[kyc->corporate_document_count], document, sizeof(DocumentInfo));
        kyc->corporate_document_count++;
        
    } else {
        KYCRecord* kyc = &account->kyc_data.individual;
        
        if (kyc->document_count >= MAX_DOCUMENTS_PER_ACCOUNT) {
            return -2; // Too many documents
        }
        
        // Reallocate document array
        kyc->documents = realloc(kyc->documents, 
                               sizeof(DocumentInfo) * (kyc->document_count + 1));
        
        if (!kyc->documents) return -3; // Memory error
        
        // Add document
        memcpy(&kyc->documents[kyc->document_count], document, sizeof(DocumentInfo));
        kyc->document_count++;
    }
    
    printf("Document submitted for account: %s (Type: %d)\n", 
           account->user_id, document->type);
    
    return 0;
}

int kyc_verify_documents(MembershipAccount* account) {
    if (!account) return -1;
    
    bool all_docs_verified = true;
    uint32_t doc_count = 0;
    
    if account->is_corporate) {
        CorporateKYCRecord* kyc = &account->kyc_data.corporate;
        doc_count = kyc->corporate_document_count;
        
        for (uint32_t i = 0; i < doc_count; i++) {
            // Simulate document verification
            kyc->corporate_documents[i].is_verified = true;
            kyc->corporate_documents[i].verified_at = current_timestamp_ms();
            
            printf("Corporate document %d verified\n", i);
        }
        
        kyc->verified_at = current_timestamp_ms();
        kyc->expires_at = kyc->verified_at + (KYC_VALIDITY_DAYS * 24 * 60 * 60 * 1000);
        
    } else {
        KYCRecord* kyc = &account->kyc_data.individual;
        doc_count = kyc->document_count;
        
        for (uint32_t i = 0; i < doc_count; i++) {
            // Simulate document verification
            kyc->documents[i].is_verified = true;
            kyc->documents[i].verified_at = current_timestamp_ms();
            
            printf("Individual document %d verified\n", i);
        }
        
        kyc->verified_at = current_timestamp_ms();
        kyc->expires_at = kyc->verified_at + (KYC_VALIDITY_DAYS * 24 * 60 * 60 * 1000);
    }
    
    account->kyc_level = doc_count >= 3 ? KYC_LEVEL_ENHANCED : KYC_LEVEL_BASIC;
    
    printf("KYC documents verified for account: %s (Level: %s)\n", 
           account->user_id, kyc_level_to_string(account->kyc_level));
    
    return all_docs_verified ? 0 : 1;
}

int kyc_biometric_verification(MembershipAccount* account, const char* biometric_data) {
    if (!account || !biometric_data) return -1;
    
    if (account->is_corporate) {
        return -2; // Corporates don't need biometric verification
    }
    
    KYCRecord* kyc = &account->kyc_data.individual;
    
    // Simulate biometric hash generation
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, biometric_data, strlen(biometric_data));
    SHA256_Final((uint8_t*)kyc->face_hash, &sha256);
    
    // Convert hash to hex string
    char hex_hash[65];
    for (int i = 0; i < 32; i++) {
        sprintf(hex_hash + (i * 2), "%02x", (uint8_t)kyc->face_hash[i]);
    }
    hex_hash[64] = '\0';
    
    memcpy(kyc->face_hash, hex_hash, 65);
    kyc->biometric_verified = true;
    
    printf("Biometric verification completed for: %s\n", account->user_id);
    
    return 0;
}

// ==========================================
// COMPLIANCE FUNCTIONS
// ==========================================

int compliance_pep_screening(MembershipAccount* account) {
    if (!account) return -1;
    
    printf("Running PEP screening for: %s\n", account->user_id);
    
    if (account->is_corporate) {
        CorporateKYCRecord* kyc = &account->kyc_data.corporate;
        
        // Check beneficial owners
        for (uint32_t i = 0; i < kyc->beneficial_owner_count; i++) {
            if (kyc->beneficial_owners[i].is_pep) {
                printf("PEP detected: %s\n", kyc->beneficial_owners[i].name);
                account->pep_check_passed = false;
                return 1; // PEP found
            }
        }
        
        // Check company name against PEP database
        // (Simulated check)
        if (strstr(kyc->company_name, "government") || strstr(kyc->company_name, "ministry")) {
            account->pep_check_passed = false;
            return 1; // PEP found
        }
        
    } else {
        KYCRecord* kyc = &account->kyc_data.individual;
        
        // Check name against PEP database (simulated)
        if (strstr(kyc->name, "minister") || strstr(kyc->surname, "royal")) {
            account->pep_check_passed = false;
            return 1; // PEP found
        }
    }
    
    account->pep_check_passed = true;
    printf("PEP screening passed for: %s\n", account->user_id);
    
    return 0;
}

int compliance_sanctions_screening(MembershipAccount* account) {
    if (!account) return -1;
    
    printf("Running sanctions screening for: %s\n", account->user_id);
    
    // Simulate sanctions database check
    // In real implementation, this would check against OFAC, UN, EU sanctions lists
    
    account->sanctions_check_passed = true;
    account->last_compliance_check = current_timestamp_ms();
    
    printf("Sanctions screening passed for: %s\n", account->user_id);
    
    return 0;
}

// ==========================================
// TRADING PERMISSIONS
// ==========================================

bool membership_check_trading_permission(const MembershipAccount* account, TradingPermission permission) {
    if (!account || account->status != STATUS_ACTIVE) return false;
    
    if (permission < 1 || permission > 5) return false;
    
    return account->benefits.permissions[permission - 1];
}

uint64_t membership_get_trading_limit(const MembershipAccount* account, const char* limit_type) {
    if (!account || !limit_type) return 0;
    
    if (strcmp(limit_type, "daily") == 0) {
        return account->tier >= MEMBERSHIP_CORPORATE ? 
               CORPORATE_DAILY_LIMIT : INDIVIDUAL_DAILY_LIMIT;
    } else if (strcmp(limit_type, "monthly") == 0) {
        return account->tier >= MEMBERSHIP_CORPORATE ? 
               CORPORATE_DAILY_LIMIT * 30 : INDIVIDUAL_DAILY_LIMIT * 30;
    } else if (strcmp(limit_type, "single") == 0) {
        return account->tier >= MEMBERSHIP_CORPORATE ? 
               CORPORATE_DAILY_LIMIT / 10 : INDIVIDUAL_DAILY_LIMIT / 10;
    }
    
    return 0;
}

bool membership_check_withdrawal_limit(const MembershipAccount* account, uint64_t amount) {
    if (!account || account->status != STATUS_ACTIVE) return false;
    
    uint64_t daily_limit = account->benefits.limits.daily_withdrawal_limit;
    
    return amount <= daily_limit;
}

// ==========================================
// PREMIUM FEATURES
// ==========================================

bool membership_has_vip_features(const MembershipAccount* account) {
    if (!account) return false;
    
    return account->tier >= MEMBERSHIP_VIP ||
           account->kyc_level >= KYC_LEVEL_CERTIFIED;
}

bool membership_has_institutional_access(const MembershipAccount* account) {
    if (!account) return false;
    
    return account->tier >= MEMBERSHIP_INSTITUTIONAL;
}

bool membership_has_otc_trading(const MembershipAccount* account) {
    if (!account) return false;
    
    return account->tier >= MEMBERSHIP_CORPORATE &&
           account->kyc_level >= KYC_LEVEL_ENHANCED;
}

// ==========================================
// UTILITY FUNCTIONS
// ==========================================

const char* membership_tier_to_string(MembershipTier tier) {
    switch (tier) {
        case MEMBERSHIP_INDIVIDUAL: return "Individual";
        case MEMBERSHIP_PROFESSIONAL: return "Professional";
        case MEMBERSHIP_CORPORATE: return "Corporate";
        case MEMBERSHIP_INSTITUTIONAL: return "Institutional";
        case MEMBERSHIP_VIP: return "VIP";
        case MEMBERSHIP_PARTNER: return "Partner";
        default: return "Unknown";
    }
}

MembershipTier membership_string_to_tier(const char* tier_str) {
    if (!tier_str) return MEMBERSHIP_INDIVIDUAL;
    
    if (strcmp(tier_str, "Professional") == 0) return MEMBERSHIP_PROFESSIONAL;
    if (strcmp(tier_str, "Corporate") == 0) return MEMBERSHIP_CORPORATE;
    if (strcmp(tier_str, "Institutional") == 0) return MEMBERSHIP_INSTITUTIONAL;
    if (strcmp(tier_str, "VIP") == 0) return MEMBERSHIP_VIP;
    if (strcmp(tier_str, "Partner") == 0) return MEMBERSHIP_PARTNER;
    
    return MEMBERSHIP_INDIVIDUAL;
}

const char* kyc_level_to_string(KYCLevel level) {
    switch (level) {
        case KYC_LEVEL_NONE: return "None";
        case KYC_LEVEL_BASIC: return "Basic";
        case KYC_LEVEL_ENHANCED: return "Enhanced";
        case KYC_LEVEL_VERIFIED: return "Verified";
        case KYC_LEVEL_CERTIFIED: return "Certified";
        default: return "Unknown";
    }
}

bool membership_is_account_active(const MembershipAccount* account) {
    if (!account) return false;
    
    return account->status == STATUS_ACTIVE || 
           account->status == STATUS_VERIFIED;
}

void membership_set_default_limits(MembershipAccount* account) {
    if (!account) return;
    
    MembershipBenefits* benefits = &account->benefits;
    
    switch (account->tier) {
        case MEMBERSHIP_INDIVIDUAL:
            benefits->limists.daily_withdrawal_limit = INDIVIDUAL_DAILY_LIMIT;
            benefits->limits.monthly_trading_limit = INDIVIDUAL_DAILY_LIMIT * 30;
            benefits->limits.max_single_transaction = 10000LLU; // $10K
            benefits->limits.otc_trading_limit = 0; // No OTC for individuals
            memset(benefits->permissions, false, sizeof(benefits->permissions));
            benefits->permissions[TRADING_PERMISSION_SPOT - 1] = true;
            break;
            
        case MEMBERSHIP_PROFESSIONAL:
            benefits->limits.daily_withdrawal_limit = PROFESSIONAL_DAILY_LIMIT;
            benefits->limits.monthly_trading_limit = PROFESSIONAL_DAILY_LIMIT * 30;
            benefits->limits.max_single_transaction = 50000LLU; // $50K
            benefits->limits.otc_trading_limit = PROFESSIONAL_DAILY_LIMIT / 10;
            benefits->permissions[TRADING_PERMISSION_SPOT - 1] = true;
            benefits->permissions[TRADING_PERMISSION_MARGIN - 1] = true;
            break;
            
        case MEMBERSHIP_CORPORATE:
            benefits->limits.daily_withdrawal_limit = CORPORATE_DAILY_LIMIT;
            benefits->limits.monthly_trading_limit = CORPORATE_DAILY_LIMIT * 30;
            benefits->limits.max_single_transaction = 500000LLU; // $500K
            benefits->limits.otc_trading_limit = CORPORATE_DAILY_LIMIT / 5;
            memset(benefits->permissions, true, sizeof(benefits->permissions));
            benefits->has_dedicated_support = true;
            benefits->has_priority_withdrawal = true;
            break;
            
        case MEMBERSHIP_INSTITUTIONAL:
            benefits->limits.daily_withdrawal_limit = INSTITUTIONAL_DAILY_LIMIT;
            benefits->limits.monthly_trading_limit = INSTITUTIONAL_DAILY_LIMIT * 30;
            benefits->limits.max_single_transaction = INSTITUTIONAL_DAILY_LIMIT / 10;
            benefits->limits.otc_trading_limit = INSTITUTIONAL_DAILY_LIMIT / 2;
            memset(benefits->permissions, true, sizeof(benefits->permissions));
            benefits->has_automated_trading = true;
            telah_algorithmic_trading = true;
            benefits->has_smart_order_routing = true;
            benefits->has_dark_pool_access = true;
            benefits->has_prime_brokerage = true;
            break;
            
        case MEMBERSHIP_VIP:
            benefits->limits.daily_withdrawal_limit = VIP_UNLIMITED_AMOUNT;
            benefits->limits.monthly_trading_limit = VIP_UNLIMITED_AMOUNT;
            benefits->limits.max_single_transaction = VIP_UNLIMITED_AMOUNT;
            benefits->limits.otc_trading_limit = VIP_UNLIMITED_AMOUNT;
            memset(benefits->permissions, true, sizeof(benefits->permissions));
            benefits->has_dedicated_support = true;
            benefits->has_priority_withdrawal = true;
            benefits->has_advanced_charting = true;
            benefits->has_api_trading = true;
            benefits->has_programmatic_access = true;
            break;
            
        default:
            membership_set_default_limits(account); // Recursive fallback
            break;
    }
}
