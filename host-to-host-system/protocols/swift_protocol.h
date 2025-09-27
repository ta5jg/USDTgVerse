/*
==============================================
 File:        swift_protocol.h
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse H2H SWIFT Protocol Handler
   
   SWIFT (Society for Worldwide Interbank Financial Telecommunication) 
   protocol implementation featuring:
   - SWIFT MT message support
   - ISO 20022 compatibility
   - Real-time message processing
   - Advanced security features
   - Compliance and audit trails
   - Quantum-safe encryption

 License:
   MIT License
==============================================
*/

#ifndef USDTGVERSE_SWIFT_PROTOCOL_H
#define USDTGVERSE_SWIFT_PROTOCOL_H

#include "../core/h2h_core.h"
#include <stdint.h>
#include <stdbool.h>

// SWIFT Message Types (MT)
typedef enum {
    SWIFT_MT_103 = 103,           // Single Customer Credit Transfer
    SWIFT_MT_200 = 200,           // Financial Institution Transfer
    SWIFT_MT_202 = 202,           // General Financial Institution Transfer
    SWIFT_MT_205 = 205,           // Financial Institution Transfer for its Own Account
    SWIFT_MT_940 = 940,           // Customer Statement Message
    SWIFT_MT_942 = 942,           // Interim Transaction Report
    SWIFT_MT_950 = 950,           // Statement Message
    SWIFT_MT_970 = 970,           // Netting Statement
    SWIFT_MT_996 = 996,           // Query Message
    SWIFT_MT_999 = 999            // Free Format Message
} swift_message_type_t;

// SWIFT Message Status
typedef enum {
    SWIFT_STATUS_ACCEPTED = 1,
    SWIFT_STATUS_REJECTED = 2,
    SWIFT_STATUS_PENDING = 3,
    SWIFT_STATUS_PROCESSING = 4,
    SWIFT_STATUS_COMPLETED = 5,
    SWIFT_STATUS_FAILED = 6
} swift_message_status_t;

// SWIFT Message Structure
typedef struct {
    char message_type[4];         // MT103, MT200, etc.
    char sender_bic[12];          // Sender BIC code
    char receiver_bic[12];        // Receiver BIC code
    char message_priority[1];     // Message priority (N, U, S)
    char delivery_monitoring[1];  // Delivery monitoring (1, 2, 3)
    char obsolescence_period[1];  // Obsolescence period (003, 020, 180)
    char transaction_reference[16]; // Transaction reference
    char related_reference[16];   // Related reference
    char value_date[6];           // Value date (YYMMDD)
    char currency[3];             // Currency code
    uint64_t amount;              // Amount in smallest unit
    char ordering_customer[35];   // Ordering customer
    char ordering_institution[35]; // Ordering institution
    char sender_correspondent[35]; // Sender correspondent
    char receiver_correspondent[35]; // Receiver correspondent
    char third_reimbursement[35]; // Third reimbursement institution
    char intermediary_institution[35]; // Intermediary institution
    char account_with_institution[35]; // Account with institution
    char beneficiary_customer[35]; // Beneficiary customer
    char remittance_information[140]; // Remittance information
    char sender_to_receiver[140]; // Sender to receiver information
    char regulatory_reporting[35]; // Regulatory reporting
    char sender_charges[3];       // Sender charges
    char receiver_charges[3];     // Receiver charges
    char sender_charges_details[6]; // Sender charges details
    char receiver_charges_details[6]; // Receiver charges details
    char quantum_signature[512];  // Quantum-safe signature
    time_t created_at;
    time_t processed_at;
    swift_message_status_t status;
} swift_message_t;

// SWIFT Connection Configuration
typedef struct {
    char swift_bic[12];           // SWIFT BIC code
    char swift_address[256];      // SWIFT address
    char certificate_path[256];   // Certificate path
    char private_key_path[256];   // Private key path
    char ca_certificate_path[256]; // CA certificate path
    bool use_ssl;
    uint32_t timeout_seconds;
    uint32_t retry_count;
    char quantum_key[512];        // Quantum-safe encryption key
    bool enable_compression;
    bool enable_encryption;
} swift_connection_config_t;

// SWIFT Session Information
typedef struct {
    char session_id[64];
    char swift_bic[12];
    char swift_address[256];
    bool is_connected;
    bool is_authenticated;
    time_t connected_at;
    time_t last_activity;
    uint32_t messages_sent;
    uint32_t messages_received;
    uint32_t bytes_sent;
    uint32_t bytes_received;
    char status[32];
    char error_message[256];
} swift_session_t;

// SWIFT Protocol Functions
typedef struct {
    // Core Functions
    int (*initialize)(void);
    int (*shutdown)(void);
    int (*connect)(swift_connection_config_t *config, swift_session_t *session);
    int (*disconnect)(swift_session_t *session);
    
    // Message Processing
    int (*send_message)(swift_session_t *session, swift_message_t *message);
    int (*receive_message)(swift_session_t *session, swift_message_t *message);
    int (*process_message)(swift_message_t *message);
    int (*validate_message)(swift_message_t *message);
    int (*format_message)(swift_message_t *message, char *formatted_data, size_t max_size);
    int (*parse_message)(char *formatted_data, swift_message_t *message);
    
    // Message Types
    int (*create_mt103)(swift_message_t *message, char *sender_bic, char *receiver_bic, 
                       char *currency, uint64_t amount, char *beneficiary, char *remittance_info);
    int (*create_mt200)(swift_message_t *message, char *sender_bic, char *receiver_bic,
                       char *currency, uint64_t amount, char *ordering_institution);
    int (*create_mt940)(swift_message_t *message, char *sender_bic, char *receiver_bic,
                       char *account_number, char *currency);
    int (*create_mt942)(swift_message_t *message, char *sender_bic, char *receiver_bic,
                       char *account_number, char *currency);
    
    // Security Functions
    int (*encrypt_message)(swift_message_t *message, char *encrypted_data, size_t data_size);
    int (*decrypt_message)(char *encrypted_data, swift_message_t *message, size_t data_size);
    int (*sign_message)(swift_message_t *message, char *signature);
    int (*verify_signature)(swift_message_t *message, char *signature);
    int (*generate_quantum_signature)(char *data, char *signature);
    int (*verify_quantum_signature)(char *data, char *signature);
    
    // Validation Functions
    bool (*is_valid_bic)(const char *bic);
    bool (*is_valid_currency)(const char *currency);
    bool (*is_valid_amount)(uint64_t amount);
    bool (*is_valid_message_type)(const char *message_type);
    int (*validate_bic)(const char *bic);
    int (*validate_currency)(const char *currency);
    int (*validate_amount)(uint64_t amount);
    
    // Utility Functions
    const char *(*message_type_to_string)(swift_message_type_t type);
    const char *(*status_to_string)(swift_message_status_t status);
    swift_message_type_t (*string_to_message_type)(const char *type_str);
    swift_message_status_t (*string_to_status)(const char *status_str);
    
    // Error Handling
    int (*get_last_error)(char *error_message, size_t max_size);
    int (*get_error_code)(void);
    void (*clear_error)(void);
} swift_protocol_t;

// Global SWIFT Protocol Instance
extern swift_protocol_t *g_swift_protocol;

// Core API Functions
int swift_protocol_init(void);
int swift_protocol_cleanup(void);
swift_protocol_t *swift_protocol_get_instance(void);

// Utility Functions
const char *swift_message_type_to_string(swift_message_type_t type);
const char *swift_status_to_string(swift_message_status_t status);
swift_message_type_t string_to_swift_message_type(const char *type_str);
swift_message_status_t string_to_swift_status(const char *status_str);

// Validation Functions
bool swift_is_valid_bic(const char *bic);
bool swift_is_valid_currency(const char *currency);
bool swift_is_valid_amount(uint64_t amount);
bool swift_is_valid_message_type(const char *message_type);
int swift_validate_bic(const char *bic);
int swift_validate_currency(const char *currency);
int swift_validate_amount(uint64_t amount);

// Message Creation Functions
int swift_create_mt103(swift_message_t *message, char *sender_bic, char *receiver_bic,
                      char *currency, uint64_t amount, char *beneficiary, char *remittance_info);
int swift_create_mt200(swift_message_t *message, char *sender_bic, char *receiver_bic,
                      char *currency, uint64_t amount, char *ordering_institution);
int swift_create_mt940(swift_message_t *message, char *sender_bic, char *receiver_bic,
                      char *account_number, char *currency);
int swift_create_mt942(swift_message_t *message, char *sender_bic, char *receiver_bic,
                      char *account_number, char *currency);

// Security Functions
int swift_encrypt_message(swift_message_t *message, char *encrypted_data, size_t data_size);
int swift_decrypt_message(char *encrypted_data, swift_message_t *message, size_t data_size);
int swift_sign_message(swift_message_t *message, char *signature);
int swift_verify_signature(swift_message_t *message, char *signature);
int swift_generate_quantum_signature(char *data, char *signature);
int swift_verify_quantum_signature(char *data, char *signature);

// Configuration Functions
int swift_load_config(const char *config_file, swift_connection_config_t *config);
int swift_save_config(const char *config_file, swift_connection_config_t *config);
int swift_set_default_bic(const char *bic);
int swift_set_default_currency(const char *currency);

// Logging Functions
int swift_log_info(const char *message, ...);
int swift_log_warning(const char *message, ...);
int swift_log_error(const char *message, ...);
int swift_log_debug(const char *message, ...);

// Constants
#define SWIFT_MAX_BIC_LEN 12
#define SWIFT_MAX_CURRENCY_LEN 3
#define SWIFT_MAX_ACCOUNT_LEN 35
#define SWIFT_MAX_REFERENCE_LEN 16
#define SWIFT_MAX_REMITTANCE_LEN 140
#define SWIFT_MAX_SENDER_INFO_LEN 140
#define SWIFT_MAX_REGULATORY_LEN 35
#define SWIFT_MAX_CHARGES_LEN 3
#define SWIFT_MAX_CHARGES_DETAILS_LEN 6
#define SWIFT_MAX_SIGNATURE_LEN 512
#define SWIFT_MAX_MESSAGE_LEN 2048

// SWIFT Message Types
#define SWIFT_MT_103_STR "103"
#define SWIFT_MT_200_STR "200"
#define SWIFT_MT_202_STR "202"
#define SWIFT_MT_205_STR "205"
#define SWIFT_MT_940_STR "940"
#define SWIFT_MT_942_STR "942"
#define SWIFT_MT_950_STR "950"
#define SWIFT_MT_970_STR "970"
#define SWIFT_MT_996_STR "996"
#define SWIFT_MT_999_STR "999"

// SWIFT Status Codes
#define SWIFT_STATUS_ACCEPTED_STR "ACCEPTED"
#define SWIFT_STATUS_REJECTED_STR "REJECTED"
#define SWIFT_STATUS_PENDING_STR "PENDING"
#define SWIFT_STATUS_PROCESSING_STR "PROCESSING"
#define SWIFT_STATUS_COMPLETED_STR "COMPLETED"
#define SWIFT_STATUS_FAILED_STR "FAILED"

// Error Codes
#define SWIFT_ERROR_NONE 0
#define SWIFT_ERROR_INVALID_CONFIG -1
#define SWIFT_ERROR_INVALID_MESSAGE -2
#define SWIFT_ERROR_INVALID_BIC -3
#define SWIFT_ERROR_INVALID_CURRENCY -4
#define SWIFT_ERROR_INVALID_AMOUNT -5
#define SWIFT_ERROR_CONNECTION_FAILED -6
#define SWIFT_ERROR_AUTHENTICATION_FAILED -7
#define SWIFT_ERROR_PROTOCOL_ERROR -8
#define SWIFT_ERROR_ENCRYPTION_ERROR -9
#define SWIFT_ERROR_DECRYPTION_ERROR -10
#define SWIFT_ERROR_SIGNATURE_ERROR -11
#define SWIFT_ERROR_VALIDATION_ERROR -12
#define SWIFT_ERROR_TIMEOUT_ERROR -13
#define SWIFT_ERROR_NETWORK_ERROR -14

#endif // USDTGVERSE_SWIFT_PROTOCOL_H
