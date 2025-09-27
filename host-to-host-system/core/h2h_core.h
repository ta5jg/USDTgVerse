/*
==============================================
 File:        h2h_core.h
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Host-to-Host (H2H) System Core
   
   Enterprise-grade bank-to-bank communication system featuring:
   - Direct host-to-host communication
   - Multi-protocol support (SWIFT, ISO 20022, FIX, etc.)
   - Real-time message processing
   - Advanced security and encryption
   - Transaction monitoring and analytics
   - Compliance and audit trails
   - High availability and failover
   - Quantum-safe communication

 License:
   MIT License
==============================================
*/

#ifndef USDTGVERSE_H2H_CORE_H
#define USDTGVERSE_H2H_CORE_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// H2H Message Types
typedef enum {
    H2H_MSG_PAYMENT_INSTRUCTION = 1,
    H2H_MSG_PAYMENT_CONFIRMATION = 2,
    H2H_MSG_PAYMENT_REJECTION = 3,
    H2H_MSG_BALANCE_INQUIRY = 4,
    H2H_MSG_BALANCE_RESPONSE = 5,
    H2H_MSG_STATEMENT_REQUEST = 6,
    H2H_MSG_STATEMENT_RESPONSE = 7,
    H2H_MSG_FX_RATE_REQUEST = 8,
    H2H_MSG_FX_RATE_RESPONSE = 9,
    H2H_MSG_ACCOUNT_VERIFICATION = 10,
    H2H_MSG_ACCOUNT_VERIFICATION_RESPONSE = 11,
    H2H_MSG_BULK_PAYMENT = 12,
    H2H_MSG_BULK_PAYMENT_RESPONSE = 13,
    H2H_MSG_HEARTBEAT = 14,
    H2H_MSG_ERROR = 15
} h2h_message_type_t;

// H2H Message Status
typedef enum {
    H2H_STATUS_PENDING = 1,
    H2H_STATUS_PROCESSING = 2,
    H2H_STATUS_COMPLETED = 3,
    H2H_STATUS_FAILED = 4,
    H2H_STATUS_REJECTED = 5,
    H2H_STATUS_CANCELLED = 6,
    H2H_STATUS_TIMEOUT = 7
} h2h_message_status_t;

// H2H Protocol Types
typedef enum {
    H2H_PROTOCOL_SWIFT = 1,
    H2H_PROTOCOL_ISO20022 = 2,
    H2H_PROTOCOL_FIX = 3,
    H2H_PROTOCOL_ACH = 4,
    H2H_PROTOCOL_SEPA = 5,
    H2H_PROTOCOL_FEDWIRE = 6,
    H2H_PROTOCOL_CHAPS = 7,
    H2H_PROTOCOL_TARGET2 = 8,
    H2H_PROTOCOL_CUSTOM = 9
} h2h_protocol_t;

// H2H Message Structure
typedef struct {
    uint64_t message_id;
    h2h_message_type_t message_type;
    h2h_protocol_t protocol;
    char sender_bic[12];
    char receiver_bic[12];
    char sender_account[34];
    char receiver_account[34];
    char currency[4];
    uint64_t amount;
    char reference_number[64];
    char description[256];
    h2h_message_status_t status;
    time_t created_at;
    time_t processed_at;
    time_t completed_at;
    char ip_address[45];
    char session_id[64];
    char correlation_id[64];
    char quantum_signature[512];
    char metadata[1024];
} h2h_message_t;

// H2H Connection Configuration
typedef struct {
    char host_name[256];
    char ip_address[45];
    uint16_t port;
    char protocol[32];
    bool use_ssl;
    char ssl_cert[512];
    char ssl_key[512];
    char ssl_ca[512];
    uint32_t timeout_seconds;
    uint32_t retry_count;
    uint32_t heartbeat_interval;
    bool enable_compression;
    bool enable_encryption;
    char encryption_key[256];
    char quantum_key[512];
} h2h_connection_config_t;

// H2H Session Information
typedef struct {
    char session_id[64];
    char host_name[256];
    char ip_address[45];
    uint16_t port;
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
} h2h_session_t;

// H2H Protocol Handler
typedef struct {
    h2h_protocol_t protocol_type;
    char protocol_name[64];
    char version[16];
    bool is_enabled;
    char handler_function[128];
    char parser_function[128];
    char validator_function[128];
    char formatter_function[128];
} h2h_protocol_handler_t;

// H2H Security Configuration
typedef struct {
    bool enable_quantum_encryption;
    char quantum_key[512];
    bool enable_traditional_encryption;
    char encryption_algorithm[32];
    char encryption_key[256];
    bool enable_digital_signatures;
    char signature_algorithm[32];
    char private_key[512];
    char public_key[512];
    bool enable_message_authentication;
    char mac_algorithm[32];
    char mac_key[256];
    bool enable_audit_logging;
    char audit_log_path[256];
} h2h_security_config_t;

// H2H Analytics Data
typedef struct {
    time_t timestamp;
    char host_name[256];
    uint32_t messages_processed;
    uint32_t messages_successful;
    uint32_t messages_failed;
    uint32_t bytes_processed;
    double processing_time_avg;
    double processing_time_max;
    double processing_time_min;
    uint32_t active_sessions;
    uint32_t total_sessions;
    char status[32];
} h2h_analytics_t;

// H2H System Core Functions
typedef struct {
    // Core Functions
    int (*initialize)(void);
    int (*shutdown)(void);
    int (*start_server)(void);
    int (*stop_server)(void);
    int (*connect_to_host)(h2h_connection_config_t *config, h2h_session_t *session);
    int (*disconnect_from_host)(h2h_session_t *session);
    
    // Message Processing
    int (*send_message)(h2h_session_t *session, h2h_message_t *message);
    int (*receive_message)(h2h_session_t *session, h2h_message_t *message);
    int (*process_message)(h2h_message_t *message);
    int (*queue_message)(h2h_message_t *message);
    int (*dequeue_message)(h2h_message_t *message);
    
    // Protocol Management
    int (*register_protocol)(h2h_protocol_handler_t *handler);
    int (*unregister_protocol)(h2h_protocol_t protocol_type);
    int (*get_protocol_handler)(h2h_protocol_t protocol_type, h2h_protocol_handler_t *handler);
    int (*list_protocols)(h2h_protocol_handler_t *handlers, int max_count);
    
    // Session Management
    int (*create_session)(h2h_connection_config_t *config, h2h_session_t *session);
    int (*destroy_session)(h2h_session_t *session);
    int (*get_session)(char *session_id, h2h_session_t *session);
    int (*list_sessions)(h2h_session_t *sessions, int max_count);
    int (*update_session_status)(h2h_session_t *session, char *status);
    
    // Security Functions
    int (*encrypt_message)(h2h_message_t *message, char *encrypted_data, size_t data_size);
    int (*decrypt_message)(char *encrypted_data, h2h_message_t *message, size_t data_size);
    int (*sign_message)(h2h_message_t *message, char *signature);
    int (*verify_signature)(h2h_message_t *message, char *signature);
    int (*generate_quantum_signature)(char *data, char *signature);
    int (*verify_quantum_signature)(char *data, char *signature);
    
    // Validation Functions
    int (*validate_message)(h2h_message_t *message);
    int (*validate_connection)(h2h_connection_config_t *config);
    int (*validate_session)(h2h_session_t *session);
    int (*check_compliance)(h2h_message_t *message);
    
    // Monitoring and Analytics
    int (*get_analytics)(char *host_name, time_t start_time, time_t end_time, h2h_analytics_t *analytics);
    int (*get_performance_metrics)(char *host_name, char *metrics_json);
    int (*get_health_status)(char *host_name, char *status_json);
    int (*get_connection_status)(char *host_name, char *status_json);
    
    // Error Handling
    int (*get_last_error)(char *error_message, size_t max_size);
    int (*get_error_code)(void);
    void (*clear_error)(void);
    int (*log_error)(char *error_message, int error_code);
} h2h_system_t;

// Global H2H System Instance
extern h2h_system_t *g_h2h_system;

// Core API Functions
int h2h_system_init(void);
int h2h_system_cleanup(void);
h2h_system_t *h2h_system_get_instance(void);

// Utility Functions
const char *h2h_message_type_to_string(h2h_message_type_t type);
const char *h2h_message_status_to_string(h2h_message_status_t status);
const char *h2h_protocol_to_string(h2h_protocol_t protocol);
h2h_message_type_t string_to_h2h_message_type(const char *type_str);
h2h_message_status_t string_to_h2h_message_status(const char *status_str);
h2h_protocol_t string_to_h2h_protocol(const char *protocol_str);

// Validation Functions
bool h2h_is_valid_bic(const char *bic);
bool h2h_is_valid_iban(const char *iban);
bool h2h_is_valid_account_number(const char *account);
bool h2h_is_valid_currency(const char *currency);
bool h2h_is_valid_amount(uint64_t amount);
bool h2h_is_valid_ip_address(const char *ip_address);

// Conversion Functions
uint64_t h2h_currency_to_smallest_unit(double amount, const char *currency);
double h2h_smallest_unit_to_currency(uint64_t amount, const char *currency);
int h2h_convert_currency(uint64_t amount, const char *from_currency, const char *to_currency, uint64_t *converted_amount);

// Security Functions
int h2h_generate_message_id(uint64_t *message_id);
int h2h_generate_session_id(char *session_id, size_t max_size);
int h2h_generate_correlation_id(char *correlation_id, size_t max_size);
int h2h_hash_message_data(const char *data, char *hash, size_t hash_size);
int h2h_sign_message_data(const char *data, char *signature, size_t signature_size);

// Configuration Functions
int h2h_load_config(const char *config_file);
int h2h_save_config(const char *config_file);
int h2h_set_default_protocol(h2h_protocol_t protocol);
int h2h_set_default_currency(const char *currency);

// Logging Functions
int h2h_log_info(const char *message, ...);
int h2h_log_warning(const char *message, ...);
int h2h_log_error(const char *message, ...);
int h2h_log_debug(const char *message, ...);

// Constants
#define H2H_MAX_MESSAGE_ID_LEN 64
#define H2H_MAX_SESSION_ID_LEN 64
#define H2H_MAX_CORRELATION_ID_LEN 64
#define H2H_MAX_BIC_LEN 12
#define H2H_MAX_ACCOUNT_LEN 34
#define H2H_MAX_CURRENCY_LEN 4
#define H2H_MAX_REFERENCE_LEN 64
#define H2H_MAX_DESCRIPTION_LEN 256
#define H2H_MAX_IP_ADDRESS_LEN 45
#define H2H_MAX_HOST_NAME_LEN 256
#define H2H_MAX_PROTOCOL_LEN 32
#define H2H_MAX_STATUS_LEN 32
#define H2H_MAX_ERROR_MESSAGE_LEN 256
#define H2H_MAX_SIGNATURE_LEN 512
#define H2H_MAX_METADATA_LEN 1024

// Default Configuration
#define H2H_DEFAULT_PORT 8443
#define H2H_DEFAULT_TIMEOUT_SECONDS 30
#define H2H_DEFAULT_RETRY_COUNT 3
#define H2H_DEFAULT_HEARTBEAT_INTERVAL 60
#define H2H_DEFAULT_MAX_SESSIONS 100
#define H2H_DEFAULT_MAX_MESSAGE_SIZE 1048576

// Error Codes
#define H2H_ERROR_NONE 0
#define H2H_ERROR_INVALID_CONFIG -1
#define H2H_ERROR_INVALID_MESSAGE -2
#define H2H_ERROR_INVALID_SESSION -3
#define H2H_ERROR_CONNECTION_FAILED -4
#define H2H_ERROR_AUTHENTICATION_FAILED -5
#define H2H_ERROR_PROTOCOL_ERROR -6
#define H2H_ERROR_ENCRYPTION_ERROR -7
#define H2H_ERROR_DECRYPTION_ERROR -8
#define H2H_ERROR_SIGNATURE_ERROR -9
#define H2H_ERROR_VALIDATION_ERROR -10
#define H2H_ERROR_TIMEOUT_ERROR -11
#define H2H_ERROR_NETWORK_ERROR -12
#define H2H_ERROR_SYSTEM_ERROR -13

#endif // USDTGVERSE_H2H_CORE_H
