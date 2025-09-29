/*
==============================================
 File:        membership_controller.c
 Author:      Irfan Gedik
 Created:     DD.MM.YYYY
 Last Update: 28.09.2025
 Version:     1.0

 Description:
   USDTgVerse Membership API Controller
   
   RESTful API endpoints for membership management:
   - Account registration and tier management
   - KYC document submission and verification
   - Compliance screening and monitoring
   - Trading permission management
   - Premium feature access control

 License:
   MIT License
==============================================
*/

#include "membership_controller.h"
#include "../core/membership_core.h"
#include "../database/membership_db.h"
#include "../security/quantum_safe.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

// ==========================================
// API RESPONSE STRUCTURES
// ==========================================

typedef struct {
    int status_code;
    char* message;
    char* data;
    char* timestamp;
} APIResponse;

typedef struct {
    char* account_type;        // "individual", "corporate"
    char* tier_requested;     // "Professional", "Corporate", etc.
    char* email;
    char* country;
    char* company_name;       // Required for corporate
    char* tax_id;            // Required for corporate
} RegistrationRequest;

typedef struct {
    char* document_type;
    char* document_format;    // "pdf", "jpg", "png"
    char* document_content;  // Base64 encoded
    char* metadata;          // Additional document info
} DocumentSubmission;

typedef struct {
    char* account_id;
    char* transaction_id;
    char* transaction_type;
    uint64_t amount;
    char* currency;
    char* timestamp;
} TradingPermissionRequest;

// ==========================================
// API RESPONSE FUNCTIONS
// ==========================================

static char* create_api_response(int status_code, const char* message, const char* data) {
    time_t now = time(NULL);
    char timestamp[32];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));
    
    char* response = malloc(1024);
    snprintf(response, 1024, 
        "{"
        "\"status\": %d,"
        "\"message\": \"%s\","
        "\"data\": %s,"
        "\"timestamp\": \"%s\""
        "}",
        status_code, message ? message : "", data ? data : "null", timestamp
    );
    
    return response;
}

static char* serialize_account_info(const MembershipAccount* account) {
    char buffer[2048];
    snprintf(buffer, sizeof(buffer),
        "{"
        "\"account_id\": \"%s\","
        "\"wallet_address\": \"%s\","
        "\"tier\": \"%s\","
        "\"status\": \"%s\","
        "\"kyc_level\": \"%s\","
        "\"is_corporate\": %s,"
        "\"created_at\": %llu,"
        "\"trading_limits\": {"
            "\"daily_withdrawal\": %llu,"
            "\"monthly_trading\": %llu,"
            "\"max_single_transaction\": %llu"
        "},"
        "\"verified\": %s"
        "}",
        account->user_id,
        account->wallet_address,
        membership_tier_to_string(account->tier),
        "active",
        kyc_level_to_string(account->kyc_level),
        account->is_corporate ? "true" : "false",
        account->created_at,
        account->benefits.limits.daily_withdrawal_limit,
        account->benefits.limits.monthly_trading_limit,
        account->benefits.limits.max_single_transaction,
        membership_is_account_active(account) ? "true" : "false"
    );
    
    char* result = malloc(strlen(buffer) + 1);
    strcpy(result, buffer);
    return result;
}

// ==========================================
// ACCOUNT REGISTRATION ENDPOINTS
// ==========================================

char* api_register_individual(const char* request_data) {
    printf("Processing individual registration request\n");
    
    // Parse JSON request (simplified parsing for demo)
    const char* email = strstr(request_data, "\"email\":\"");
    const char* country = strstr(request_data, "\"country\":\"");
    
    if (!email || !country) {
        return create_api_response(400, "Missing required fields: email, country", NULL);
    }
    
    // Extract values (simplified)
    char user_email[256] = {0};
    char user_country[3] = {0};
    
    const char* start = email + 9; // Skip "email":""
    const char* end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(user_email, start, len < 255 ? len : 255);
    }
    
    start = country + 11; // Skip "country":""
    end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(user_country, start, len < 2 ? len : 2);
    }
    
    // Validate input
    if (strlen(user_email) == 0 || strlen(user_country) == 0) {
        return create_api_response(400, "Invalid email or country format", NULL);
    }
    
    // Create membership account
    char user_data[512];
    snprintf(user_data, sizeof(user_data), 
        "{\"email\":\"%s\",\"country\":\"%s\",\"type\":\"individual\"}", 
        user_email, user_country);
    
    MembershipAccount* account = membership_account_create(user_data, false);
    if (!account) {
        return create_api_response(500, "Failed to create account", NULL);
    }
    
    // Save to database
    int db_result = membership_db_save_account(account);
    if (db_result != 0) {
        membership_account_destroy(account);
        return create_api_response(500, "Failed to save account to database", NULL);
    }
    
    char* account_info = serialize_account_info(account);
    char* response = create_api_response(201, "Account created successfully", account_info);
    
    membership_account_destroy(account);
    free(account_info);
    
    printf("Individual registration completed for: %s\n", user_email);
    return response;
}

char* api_register_corporate(const char* request_data) {
    printf("Processing corporate registration request\n");
    
    // Parse JSON request (simplified)
    const char* company_name = strstr(request_data, "\"company_name\":\"");
    const char* email = strstr(request_data, "\"email\":\"");
    const char* tax_id = strstr(request_data, "\"tax_id\":\"");
    const char* country = strstr(request_data, "\"country\":\"");
    
    if (!company_name || !email || !tax_id || !country) {
        return create_api_response(400, "Missing required fields: company_name, email, tax_id, country", NULL);
    }
    
    // Extract values (simplified parsing)
    char corp_name[256] = {0};
    char corp_email[256] = {0};
    char corp_tax_id[64] = {0};
    char corp_country[3] = {0};
    
    const char* start = company_name + 15; // Skip "company_name":""
    const char* end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(corp_name, start, len < 255 ? len : 255);
    }
    
    start = email + 9; // Skip "email":""
    end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(corp_email, start, len < 255 ? len : 255);
    }
    
    start = tax_id + 10; // Skip "tax_id":""
    end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(corp_tax_id, start, len < 63 ? len : 63);
    }
    
    start = country + 11; // Skip "country":""
    end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(corp_country, start, len < 2 ? len : 2);
    }
    
    // Validate corporate data
    if (strlen(corp_name) == 0 || strlen(corp_email) == 0 || 
        strlen(corp_tax_id) == 0 || strlen(corp_country) == 0) {
        return create_api_response(400, "Invalid corporate registration data", NULL);
    }
    
    // Create corporate account
    char corp_data[1024];
    snprintf(corp_data, sizeof(corp_data), 
        "{\"company_name\":\"%s\",\"email\":\"%s\",\"tax_id\":\"%s\","
        "\"country\":\"%s\",\"type\":\"corporate\"}", 
        corp_name, corp_email, corp_tax_id, corp_country);
    
    MembershipAccount* account = membership_account_create(corp_data, true);
    if (!account) {
        return create_api_response(500, "Failed to create corporate account", NULL);
    }
    
    // Set up corporate KYC structure
    CorporateKYCRecord* kyc = &account->kyc_data.corporate;
    strncpy(kyc->company_name, corp_name, sizeof(kyc->company_name) - 1);
    strncpy(kyc->legal_name, corp_name, sizeof(kyc->legal_name) - 1);
    strncpy(kyc->tax_id, corp_tax_id, sizeof(kyc->tax_id) - 1);
    strncpy(kyc->incorporation_country, corp_country, sizeof(kyc->incorporation_country) - 1);
    
    // Save to database
    int db_result = membership_db_save_account(account);
    if (db_result != 0) {
        membership_account_destroy(account);
        return create_api_response(500, "Failed to save corporate account", NULL);
    }
    
    char* account_info = serialize_account_info(account);
    char* response = create_api_response(201, "Corporate account created successfully", account_info);
    
    membership_account_destroy(account);
    free(account_info);
    
    printf("Corporate registration completed for: %s\n", corp_name);
    return response;
}

// ==========================================
// KYC DOCUMENT ENDPOINTS
// ==========================================

char* api_submit_document(const char* request_data) {
    printf("Processing document submission\n");
    
    // Parse request (simplified)
    const char* account_id = strstr(request_data, "\"account_id\":\"");
    const char* doc_type = strstr(request_data, "\"document_type\":\"");
    const char* content = strstr(request_data, "\"document_content\":\"");
    
    if (!account_id || !doc_type || !content) {
        return create_api_response(400, "Missing required fields: account_id, document_type, document_content", NULL);
    }
    
    // Extract account ID
    char acc_id[64] = {0};
    const char* start = account_id + 14; // Skip "account_id":""
    const char* end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(acc_id, start, len < 63 ? len : 63);
    }
    
    // Extract document type
    char doc_type_str[64] = {0};
    start = doc_type + 17; // Skip "document_type":""
    end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(doc_type_str, start, len < 63 ? len : 63);
    }
    
    // Load account from database
    MembershipAccount* account = membership_db_load_account(acc_id);
    if (!account) {
        return create_api_response(404, "Account not found", NULL);
    }
    
    // Create document info
    DocumentInfo doc_info;
    memset(&doc_info, 0, sizeof(doc_info));
    doc_info.type = atoi(doc_type_str);
    doc_info.uploaded_at = time(NULL) * 1000;
    doc_info.mime_type[0] = 'p'; doc_info.mime_type[1] = 'd'; doc_info.mime_type[2] = 'f';
    doc_info.mime_type[3] = '\0';
    doc_info.file_size = strlen(content);
    
    // Generate document hash
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, content, strlen(content));
    SHA256_Final(doc_info.hash, &sha256);
    
    // Submit document
    int result = kyc_submit_document(account, &doc_info);
    if (result != 0) {
        membership_account_destroy(account);
        return create_api_response(500, "Failed to submit document", NULL);
    }
    
    // Save updated account
    int db_result = membership_db_save_account(account);
    
    char* response;
    if (db_result == 0) {
        char success_msg[256];
        snprintf(success_msg, sizeof(success_msg), 
            "Document submitted successfully for account %s", acc_id);
        response = create_api_response(200, success_msg, NULL);
    } else {
        response = create_api_response(500, "Failed to save document submission", NULL);
    }
    
    membership_account_destroy(account);
    return response;
}

char* api_verify_documents(const char* request_data) {
    printf("Processing document verification\n");
    
    // Parse account ID
    const char* account_id = strstr(request_data, "\"account_id\":\"");
    if (!account_id) {
        return create_api_response(400, "Missing account_id", NULL);
    }
    
    char acc_id[64] = {0};
    const char* start = account_id + 14; // Skip "account_id":""
    const char* end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(acc_id, start, len < 63 ? len : 63);
    }
    
    // Load account
    MembershipAccount* account = membership_db_load_account(acc_id);
    if (!account) {
        return create_api_response(404, "Account not found", NULL);
    }
    
    // Run document verification
    int verify_result = kyc_verify_documents(account);
    
    // Run compliance checks
    int pep_result = compliance_pep_screening(account);
    int sanctions_result = compliance_sanctions_screening(account);
    
    // Update account verification status
    if (verify_result == 0 && pep_result == 0 && sanctions_result == 0) {
        account->status = STATUS_VERIFIED;
        account->kyc_level = KYC_LEVEL_ENHANCED;
    }
    
    // Save updated account
    int db_result = membership_db_save_account(account);
    
    char* response;
    if (db_result == 0) {
        char verification_msg[512];
        snprintf(verification_msg, sizeof(verification_msg),
            "Verification completed - KYC: %d, PEP: %d, Sanctions: %d, Final Status: %s",
            verify_result, pep_result, sanctions_result,
            (verify_result == 0 && pep_result == 0 && sanctions_result == 0) ? "VERIFIED" : "PENDING"
        );
        response = create_api_response(200, verification_msg, NULL);
    } else {
        response = create_api_response(500, "Failed to save verification results", NULL);
    }
    
    membership_account_destroy(account);
    return response;
}

// ==========================================
// TRADING PERMISSION ENDPOINTS
// ==========================================

char* api_check_trading_permission(const char* request_data) {
    printf("Checking trading permission\n");
    
    // Parse request
    const char* account_id = strstr(request_data, "\"account_id\":\"");
    const char* permission_str = strstr(request_data, "\"permission\":\"");
    
    if (!account_id || !permission_str) {
        return create_api_response(400, "Missing account_id or permission", NULL);
    }
    
    char acc_id[64] = {0};
    const char* start = account_id + 14; // Skip "account_id":""
    const char* end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(acc_id, start, len < 63 ? len : 63);
    }
    
    char perm_str[32] = {0};
    start = permission_str + 14; // Skip "permission":""
    end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(perm_str, start, len < 31 ? len : 31);
    }
    
    // Load account
    MembershipAccount* account = membership_db_load_account(acc_id);
    if (!account) {
        return create_api_response(404, "Account not found", NULL);
    }
    
    // Check permission
    TradingPermission permission = atoi(perm_str);
    bool has_permission = membership_check_trading_permission(account, permission);
    
    char perm_data[256];
    snprintf(perm_data, sizeof(perm_data),
        "{"
        "\"account_id\": \"%s\","
        "\"permission_type\": \"%s\","
        "\"has_permission\": %s,"
        "\"tier\": \"%s\","
        "\"kyc_level\": \"%s\""
        "}",
        acc_id,
        perm_str,
        has_permission ? "true" : "false",
        membership_tier_to_string(account->tier),
        kyc_level_to_string(account->kyc_level)
    );
    
    char* response = create_api_response(200, 
        has_permission ? "Permission granted" : "Permission denied", perm_data);
    
    membership_account_destroy(account);
    return response;
}

char* api_get_account_info(const char* request_data) {
    printf("Retrieving account information\n");
    
    // Parse account ID
    const char* account_id = strstr(request_data, "\"account_id\":\"");
    if (!account_id) {
        return create_api_response(400, "Missing account_id", NULL);
    }
    
    char acc_id[64] = {0};
    const char* start = account_id + 14; // Skip "account_id":""
    const char* end = strchr(start, '"');
    if (end) {
        size_t len = end - start;
        strncpy(acc_id, start, len < 63 ? len : 63);
    }
    
    // Load account
    MembershipAccount* account = membership_db_load_account(acc_id);
    if (!account) {
        return create_api_response(404, "Account not found", NULL);
    }
    
    // Serialize account info
    char* account_info = serialize_account_info(account);
    char* response = create_api_response(200, "Account information retrieved", account_info);
    
    membership_account_destroy(account);
    free(account_info);
    
    return response;
}

// ==========================================
// MEMBERSHIP TIER MANAGEMENT
// ==========================================

uint64_t api_start_membership_verification_api(const char* account_id) {
    if (!account_id) return 1;
    
    MembershipAccount* account = membership_db_load_account(account_id);
    if (!account) return 2;
    
    // Run enhanced verification
    int kyc_result = kyc_enhanced_verification(account);
    int compliance_result = compliance_pep_screening(account);
    
    if (kyc_result == 0 && compliance_result == 0) {
        // Upgrade tier if eligible
        if (account->tier < MEMBERSHIP_INSTITUTIONAL) {
            membership_account_update_tier(account, (MembershipTier)(account->tier + 1));
        }
    }
    
    // Save updated account
    membership_db_save_account(account);
    membership_account_destroy(account);
    
    return 0;
}

// ==========================================
// API ROUTING FUNCTION
// ==========================================

char* handle_membership_api_request(const char* endpoint, const char* method, const char* request_data) {
    if (!endpoint || !method) {
        return create_api_response(400, "Invalid API request", NULL);
    }
    
    printf("Handling membership API: %s %s\n", method, endpoint);
    
    // Route to appropriate handler
    if (strcmp(endpoint, "/api/v1/membership/register/individual") == 0 && strcmp(method, "POST") == 0) {
        return api_register_individual(request_data);
    }
    else if (strcmp(endpoint, "/api/v1/membership/register/corporate") == 0 && strcmp(method, "POST") == 0) {
        return api_register_corporate(request_data);
    }
    else if (strcmp(endpoint, "/api/v1/membership/documents/submit") == 0 && strcmp(method, "POST") == 0) {
        return api_submit_document(request_data);
    }
    else if (strcmp(endpoint, "/api/v1/membership/documents/verify") == 0 && strcmp(method, "POST") == 0) {
        return api_verify_documents(request_data);
    }
    else if (strcmp(endpoint, "/api/v1/membership/permissions/check") == 0 && strcmp(method, "POST") == 0) {
        return api_check_trading_permission(request_data);
    }
    else if (strcmp(endpoint, "/api/v1/membership/account/info") == 0 && strcmp(method, "GET") == 0) {
        return api_get_account_info(request_data);
    }
    else {
        return create_api_response(404, "Endpoint not found", NULL);
    }
}
