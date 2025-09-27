/*
==============================================
 File:        direct_ip_payment.h
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Direct IP-to-IP Payment System
   
   Advanced server-to-server payment processing featuring:
   - Direct IP-to-IP communication
   - Quantum-safe encryption
   - Real-time payment processing
   - Multi-currency support
   - Advanced security features
   - Load balancing and failover
   - Performance monitoring
   - API and SDK support

 License:
   MIT License
==============================================
*/

#ifndef DIRECT_IP_PAYMENT_H
#define DIRECT_IP_PAYMENT_H

#include "../core/payment_core.h"
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Direct IP Payment Configuration
typedef struct {
    char server_ip[45];
    uint16_t server_port;
    char server_key[256];
    char server_cert[512];
    bool use_ssl;
    uint32_t timeout_ms;
    char api_version[16];
    char quantum_key[512];
    char encryption_key[256];
    bool enable_compression;
    bool enable_keepalive;
    uint32_t max_connections;
    uint32_t connection_timeout;
    char load_balancer_url[256];
    char failover_servers[1024];
    bool enable_monitoring;
    char monitoring_endpoint[256];
} direct_ip_config_t;

// Direct IP Payment Request
typedef struct {
    char request_id[64];
    char merchant_id[64];
    char customer_id[64];
    char payment_method[32];
    char currency[16];
    uint64_t amount;
    char description[256];
    char reference_number[64];
    char ip_address[45];
    char user_agent[512];
    char metadata[1024];
    char quantum_signature[512];
    time_t timestamp;
    uint32_t nonce;
    char checksum[64];
} direct_ip_request_t;

// Direct IP Payment Response
typedef struct {
    char request_id[64];
    char transaction_id[64];
    char status[32];
    char message[256];
    char reference_number[64];
    char payment_url[512];
    char qr_code[512];
    char quantum_signature[512];
    char metadata[1024];
    time_t timestamp;
    uint32_t response_code;
    char checksum[64];
} direct_ip_response_t;

// Direct IP Payment Connection
typedef struct {
    int socket_fd;
    struct sockaddr_in server_addr;
    bool is_connected;
    bool use_ssl;
    SSL *ssl_connection;
    SSL_CTX *ssl_context;
    time_t connected_at;
    time_t last_activity;
    uint32_t bytes_sent;
    uint32_t bytes_received;
    char server_ip[45];
    uint16_t server_port;
} direct_ip_connection_t;

// Direct IP Payment Server
typedef struct {
    int server_socket;
    struct sockaddr_in server_addr;
    bool is_running;
    bool use_ssl;
    SSL_CTX *ssl_context;
    uint32_t max_clients;
    uint32_t current_clients;
    direct_ip_connection_t *connections;
    pthread_t server_thread;
    pthread_mutex_t mutex;
    char server_ip[45];
    uint16_t server_port;
    char server_key[256];
    char server_cert[512];
} direct_ip_server_t;

// Direct IP Payment Functions
typedef struct {
    // Core Functions
    int (*initialize)(direct_ip_config_t *config);
    int (*shutdown)(void);
    int (*process_payment)(direct_ip_request_t *request, direct_ip_response_t *response);
    int (*refund_payment)(char *transaction_id, uint64_t refund_amount, direct_ip_response_t *response);
    int (*cancel_payment)(char *transaction_id, direct_ip_response_t *response);
    
    // Connection Management
    int (*connect_to_server)(char *server_ip, uint16_t server_port, direct_ip_connection_t *connection);
    int (*disconnect_from_server)(direct_ip_connection_t *connection);
    int (*send_request)(direct_ip_connection_t *connection, direct_ip_request_t *request);
    int (*receive_response)(direct_ip_connection_t *connection, direct_ip_response_t *response);
    int (*ping_server)(direct_ip_connection_t *connection);
    
    // Server Management
    int (*start_server)(direct_ip_config_t *config, direct_ip_server_t *server);
    int (*stop_server)(direct_ip_server_t *server);
    int (*handle_client_connection)(direct_ip_server_t *server, int client_socket);
    int (*process_client_request)(int client_socket, direct_ip_request_t *request, direct_ip_response_t *response);
    
    // SSL/TLS Management
    int (*init_ssl_context)(SSL_CTX **ssl_context, char *cert_file, char *key_file);
    int (*cleanup_ssl_context)(SSL_CTX *ssl_context);
    int (*establish_ssl_connection)(direct_ip_connection_t *connection);
    int (*close_ssl_connection)(direct_ip_connection_t *connection);
    
    // Security Functions
    int (*encrypt_request)(direct_ip_request_t *request, char *encrypted_data, size_t data_size);
    int (*decrypt_request)(char *encrypted_data, direct_ip_request_t *request, size_t data_size);
    int (*encrypt_response)(direct_ip_response_t *response, char *encrypted_data, size_t data_size);
    int (*decrypt_response)(char *encrypted_data, direct_ip_response_t *response, size_t data_size);
    int (*generate_quantum_signature)(char *data, char *signature);
    int (*verify_quantum_signature)(char *data, char *signature);
    int (*validate_request)(direct_ip_request_t *request);
    int (*validate_response)(direct_ip_response_t *response);
    
    // Compression Functions
    int (*compress_data)(char *data, size_t data_size, char *compressed_data, size_t *compressed_size);
    int (*decompress_data)(char *compressed_data, size_t compressed_size, char *data, size_t *data_size);
    
    // Load Balancing and Failover
    int (*select_server)(char *server_list, char *selected_server, size_t max_size);
    int (*check_server_health)(char *server_ip, uint16_t server_port);
    int (*failover_to_backup)(direct_ip_connection_t *connection);
    int (*update_server_status)(char *server_ip, uint16_t server_port, bool is_healthy);
    
    // Performance Monitoring
    int (*start_monitoring)(char *monitoring_endpoint);
    int (*stop_monitoring)(void);
    int (*get_performance_metrics)(char *metrics_json);
    int (*log_performance_data)(char *metric_name, double value, time_t timestamp);
    
    // Error Handling
    int (*get_last_error)(char *error_message, size_t max_size);
    int (*get_error_code)(void);
    void (*clear_error)(void);
} direct_ip_payment_t;

// Global Direct IP Payment Instance
extern direct_ip_payment_t *g_direct_ip_payment;

// Core API Functions
int direct_ip_payment_init(direct_ip_config_t *config);
int direct_ip_payment_cleanup(void);
direct_ip_payment_t *direct_ip_payment_get_instance(void);

// Utility Functions
const char *direct_ip_payment_status_to_string(const char *status);
const char *direct_ip_payment_currency_to_string(const char *currency);
bool direct_ip_payment_is_valid_ip(const char *ip_address);
bool direct_ip_payment_is_valid_port(uint16_t port);
bool direct_ip_payment_is_valid_currency(const char *currency);
bool direct_ip_payment_is_valid_status(const char *status);

// Conversion Functions
uint64_t direct_ip_payment_currency_to_smallest_unit(double amount, const char *currency);
double direct_ip_payment_smallest_unit_to_currency(uint64_t amount, const char *currency);
int direct_ip_payment_convert_currency(uint64_t amount, const char *from_currency, const char *to_currency, uint64_t *converted_amount);

// Security Functions
int direct_ip_payment_generate_request_id(char *request_id, size_t max_size);
int direct_ip_payment_generate_transaction_id(char *transaction_id, size_t max_size);
int direct_ip_payment_generate_reference_number(char *reference_number, size_t max_size);
int direct_ip_payment_hash_data(const char *data, char *hash, size_t hash_size);
int direct_ip_payment_sign_data(const char *data, char *signature, size_t signature_size);
int direct_ip_payment_verify_signature(const char *data, const char *signature);

// Network Functions
int direct_ip_payment_resolve_hostname(const char *hostname, char *ip_address, size_t max_size);
int direct_ip_payment_check_connectivity(const char *ip_address, uint16_t port);
int direct_ip_payment_get_local_ip(char *ip_address, size_t max_size);
int direct_ip_payment_get_public_ip(char *ip_address, size_t max_size);

// Configuration Functions
int direct_ip_payment_load_config(const char *config_file, direct_ip_config_t *config);
int direct_ip_payment_save_config(const char *config_file, direct_ip_config_t *config);
int direct_ip_payment_set_default_server(const char *server_ip, uint16_t server_port);
int direct_ip_payment_set_default_currency(const char *currency);

// Logging Functions
int direct_ip_payment_log_info(const char *message, ...);
int direct_ip_payment_log_warning(const char *message, ...);
int direct_ip_payment_log_error(const char *message, ...);
int direct_ip_payment_log_debug(const char *message, ...);

// Constants
#define DIRECT_IP_PAYMENT_MAX_REQUEST_ID_LEN 64
#define DIRECT_IP_PAYMENT_MAX_TRANSACTION_ID_LEN 64
#define DIRECT_IP_PAYMENT_MAX_CURRENCY_LEN 16
#define DIRECT_IP_PAYMENT_MAX_STATUS_LEN 32
#define DIRECT_IP_PAYMENT_MAX_DESCRIPTION_LEN 256
#define DIRECT_IP_PAYMENT_MAX_METADATA_LEN 1024
#define DIRECT_IP_PAYMENT_MAX_SIGNATURE_LEN 512
#define DIRECT_IP_PAYMENT_MAX_IP_LEN 45
#define DIRECT_IP_PAYMENT_MAX_SERVER_KEY_LEN 256
#define DIRECT_IP_PAYMENT_MAX_SERVER_CERT_LEN 512
#define DIRECT_IP_PAYMENT_MAX_QUANTUM_KEY_LEN 512
#define DIRECT_IP_PAYMENT_MAX_ENCRYPTION_KEY_LEN 256

// Default Configuration
#define DIRECT_IP_PAYMENT_DEFAULT_PORT 8443
#define DIRECT_IP_PAYMENT_DEFAULT_TIMEOUT_MS 30000
#define DIRECT_IP_PAYMENT_DEFAULT_MAX_CONNECTIONS 100
#define DIRECT_IP_PAYMENT_DEFAULT_CONNECTION_TIMEOUT 60
#define DIRECT_IP_PAYMENT_DEFAULT_API_VERSION "1.0"

// Supported Currencies
#define DIRECT_IP_PAYMENT_CURRENCY_USDTG "USDTg"
#define DIRECT_IP_PAYMENT_CURRENCY_USDTGV "USDTgV"
#define DIRECT_IP_PAYMENT_CURRENCY_USDTGG "USDTgG"
#define DIRECT_IP_PAYMENT_CURRENCY_USD "USD"
#define DIRECT_IP_PAYMENT_CURRENCY_EUR "EUR"
#define DIRECT_IP_PAYMENT_CURRENCY_GBP "GBP"
#define DIRECT_IP_PAYMENT_CURRENCY_BTC "BTC"
#define DIRECT_IP_PAYMENT_CURRENCY_ETH "ETH"

// Transaction Statuses
#define DIRECT_IP_PAYMENT_STATUS_PENDING "pending"
#define DIRECT_IP_PAYMENT_STATUS_PROCESSING "processing"
#define DIRECT_IP_PAYMENT_STATUS_COMPLETED "completed"
#define DIRECT_IP_PAYMENT_STATUS_FAILED "failed"
#define DIRECT_IP_PAYMENT_STATUS_CANCELLED "cancelled"
#define DIRECT_IP_PAYMENT_STATUS_REFUNDED "refunded"

// Response Codes
#define DIRECT_IP_PAYMENT_RESPONSE_SUCCESS 200
#define DIRECT_IP_PAYMENT_RESPONSE_BAD_REQUEST 400
#define DIRECT_IP_PAYMENT_RESPONSE_UNAUTHORIZED 401
#define DIRECT_IP_PAYMENT_RESPONSE_FORBIDDEN 403
#define DIRECT_IP_PAYMENT_RESPONSE_NOT_FOUND 404
#define DIRECT_IP_PAYMENT_RESPONSE_INTERNAL_ERROR 500
#define DIRECT_IP_PAYMENT_RESPONSE_SERVICE_UNAVAILABLE 503

// Error Codes
#define DIRECT_IP_PAYMENT_ERROR_NONE 0
#define DIRECT_IP_PAYMENT_ERROR_INVALID_CONFIG -1
#define DIRECT_IP_PAYMENT_ERROR_INVALID_REQUEST -2
#define DIRECT_IP_PAYMENT_ERROR_INVALID_RESPONSE -3
#define DIRECT_IP_PAYMENT_ERROR_NETWORK_ERROR -4
#define DIRECT_IP_PAYMENT_ERROR_CONNECTION_ERROR -5
#define DIRECT_IP_PAYMENT_ERROR_SSL_ERROR -6
#define DIRECT_IP_PAYMENT_ERROR_ENCRYPTION_ERROR -7
#define DIRECT_IP_PAYMENT_ERROR_DECRYPTION_ERROR -8
#define DIRECT_IP_PAYMENT_ERROR_SIGNATURE_ERROR -9
#define DIRECT_IP_PAYMENT_ERROR_VALIDATION_ERROR -10
#define DIRECT_IP_PAYMENT_ERROR_TIMEOUT_ERROR -11
#define DIRECT_IP_PAYMENT_ERROR_SERVER_ERROR -12

#endif // DIRECT_IP_PAYMENT_H
