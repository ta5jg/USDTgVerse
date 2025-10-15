/*
==============================================
 File:        airdrop_api_server.c
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Airdrop API Server
   
   Pure C REST API server for airdrop management:
   - POST /api/airdrop/create - Create new airdrop
   - POST /api/airdrop/use-fee - Use airdrop for fee
   - POST /api/airdrop/verify-purchase - Verify purchase
   - GET /api/airdrop/status - Get airdrop status
   - GET /api/airdrop/stats - Get system statistics

 License:
   MIT License
==============================================
*/

#include "airdrop_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 3006
#define BUFFER_SIZE 65536

// HTTP response helper
static void send_response(int client_socket, int status_code, const char* status_text, 
                         const char* content_type, const char* body) {
    char response[BUFFER_SIZE];
    int body_len = body ? strlen(body) : 0;
    
    snprintf(response, sizeof(response),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %d\r\n"
             "Access-Control-Allow-Origin: *\r\n"
             "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
             "Access-Control-Allow-Headers: Content-Type\r\n"
             "Connection: close\r\n"
             "\r\n"
             "%s",
             status_code, status_text, content_type, body_len, body ? body : "");
    
    send(client_socket, response, strlen(response), 0);
}

// Parse JSON field
static char* get_json_field(const char* json, const char* field) {
    char search[256];
    snprintf(search, sizeof(search), "\"%s\"", field);
    
    char* start = strstr(json, search);
    if (!start) return NULL;
    
    start = strchr(start + strlen(search), ':');
    if (!start) return NULL;
    
    // Skip whitespace and quotes
    start++;
    while (*start == ' ' || *start == '\t' || *start == '"') start++;
    
    // Find end
    char* end = start;
    while (*end && *end != '"' && *end != ',' && *end != '}' && *end != '\n') end++;
    
    // Copy value
    int len = end - start;
    char* value = malloc(len + 1);
    strncpy(value, start, len);
    value[len] = '\0';
    
    return value;
}

// Handle airdrop creation
static void handle_create_airdrop(int client_socket, const char* body) {
    char* wallet_address = get_json_field(body, "wallet_address");
    char* user_id = get_json_field(body, "user_id");
    char* device_fingerprint = get_json_field(body, "device_fingerprint");
    char* ip_address = get_json_field(body, "ip_address");
    
    if (!wallet_address || !user_id) {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Missing required fields\"}");
        return;
    }
    
    // Create airdrop
    char* airdrop_id = airdrop_create(wallet_address, user_id, 
                                      device_fingerprint ? device_fingerprint : "unknown",
                                      ip_address ? ip_address : "0.0.0.0");
    
    if (airdrop_id) {
        char response[1024];
        snprintf(response, sizeof(response),
                "{\"success\":true,\"airdrop_id\":\"%s\",\"amount\":%.2f,\"status\":\"locked\","
                "\"message\":\"10 USDTg airdrop created (LOCKED - Fee only). Purchase 50+ USDTg to unlock.\"}",
                airdrop_id, AIRDROP_AMOUNT);
        
        send_response(client_socket, 200, "OK", "application/json", response);
        free(airdrop_id);
    } else {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Airdrop creation failed\"}");
    }
    
    free(wallet_address);
    free(user_id);
    if (device_fingerprint) free(device_fingerprint);
    if (ip_address) free(ip_address);
}

// Handle fee payment
static void handle_use_fee(int client_socket, const char* body) {
    char* wallet_address = get_json_field(body, "wallet_address");
    char* fee_str = get_json_field(body, "fee_amount");
    
    if (!wallet_address || !fee_str) {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Missing required fields\"}");
        return;
    }
    
    double fee_amount = atof(fee_str);
    
    if (airdrop_use_for_fee(wallet_address, fee_amount)) {
        AirdropRecord* record = airdrop_get_record(wallet_address);
        char response[1024];
        snprintf(response, sizeof(response),
                "{\"success\":true,\"fee_paid\":%.6f,\"remaining\":%.6f,\"status\":\"%s\"}",
                fee_amount, record ? record->locked_amount : 0.0,
                record && record->status == AIRDROP_STATUS_UNLOCKED ? "unlocked" : "locked");
        
        send_response(client_socket, 200, "OK", "application/json", response);
        if (record) free(record);
    } else {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Fee payment failed\"}");
    }
    
    free(wallet_address);
    free(fee_str);
}

// Handle purchase verification
static void handle_verify_purchase(int client_socket, const char* body) {
    char* wallet_address = get_json_field(body, "wallet_address");
    char* amount_str = get_json_field(body, "purchase_amount");
    
    if (!wallet_address || !amount_str) {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Missing required fields\"}");
        return;
    }
    
    double purchase_amount = atof(amount_str);
    
    if (airdrop_verify_purchase(wallet_address, purchase_amount)) {
        AirdropRecord* record = airdrop_get_record(wallet_address);
        char response[1024];
        
        bool unlocked = record && record->status == AIRDROP_STATUS_UNLOCKED;
        snprintf(response, sizeof(response),
                "{\"success\":true,\"purchase_amount\":%.2f,\"total_purchased\":%.2f,"
                "\"unlocked\":%s,\"remaining_to_unlock\":%.2f,\"message\":\"%s\"}",
                purchase_amount, record ? record->purchase_amount : 0.0,
                unlocked ? "true" : "false",
                record ? (MIN_PURCHASE_UNLOCK - record->purchase_amount) : MIN_PURCHASE_UNLOCK,
                unlocked ? "Airdrop unlocked! Full access granted." : 
                          "Purchase recorded. Buy more to unlock airdrop.");
        
        send_response(client_socket, 200, "OK", "application/json", response);
        if (record) free(record);
    } else {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Purchase verification failed\"}");
    }
    
    free(wallet_address);
    free(amount_str);
}

// Handle status check
static void handle_get_status(int client_socket, const char* query) {
    // Extract wallet_address from query string
    char* wallet_param = strstr(query, "wallet_address=");
    if (!wallet_param) {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Missing wallet_address parameter\"}");
        return;
    }
    
    char wallet_address[256];
    sscanf(wallet_param + 15, "%[^&]", wallet_address);
    
    AirdropRecord* record = airdrop_get_record(wallet_address);
    if (record) {
        char response[2048];
        snprintf(response, sizeof(response),
                "{\"success\":true,\"airdrop_id\":\"%s\",\"wallet_address\":\"%s\","
                "\"airdrop_amount\":%.2f,\"locked_amount\":%.6f,\"used_amount\":%.6f,"
                "\"purchase_amount\":%.2f,\"status\":\"%s\",\"unlocked\":%s,"
                "\"can_use_for_fees\":true,\"expires_at\":%ld,\"fee_usage_count\":%u}",
                record->airdrop_id, record->wallet_address,
                record->airdrop_amount, record->locked_amount, record->used_amount,
                record->purchase_amount,
                record->status == AIRDROP_STATUS_LOCKED ? "locked" :
                record->status == AIRDROP_STATUS_UNLOCKED ? "unlocked" : "other",
                record->status == AIRDROP_STATUS_UNLOCKED ? "true" : "false",
                record->expires_at, record->fee_usage_count);
        
        send_response(client_socket, 200, "OK", "application/json", response);
        free(record);
    } else {
        send_response(client_socket, 404, "Not Found", "application/json",
                     "{\"success\":false,\"error\":\"No airdrop found for this wallet\"}");
    }
}

// Handle statistics
static void handle_get_stats(int client_socket) {
    AirdropStats stats = airdrop_get_stats();
    
    char response[2048];
    snprintf(response, sizeof(response),
            "{\"success\":true,\"total_airdrops\":%lu,\"locked_airdrops\":%lu,"
            "\"unlocked_airdrops\":%lu,\"expired_airdrops\":%lu,\"revoked_airdrops\":%lu,"
            "\"total_airdrop_value\":%.2f,\"locked_value\":%.2f,\"used_for_fees\":%.2f,"
            "\"airdrops_today\":%u,\"daily_limit\":%d}",
            stats.total_airdrops, stats.locked_airdrops, stats.unlocked_airdrops,
            stats.expired_airdrops, stats.revoked_airdrops, stats.total_airdrop_value,
            stats.locked_value, stats.used_for_fees, stats.airdrops_today, MAX_AIRDROPS_PER_DAY);
    
    send_response(client_socket, 200, "OK", "application/json", response);
}

// Main server
int main() {
    printf("🚀 USDTgVerse Pure C Native Airdrop API Server\n");
    printf("==============================================\n");
    printf("📅 Starting: %s", ctime(&(time_t){time(NULL)}));
    printf("🌐 Port: %d\n", PORT);
    printf("💰 Airdrop: %.2f USDTg (LOCKED)\n", AIRDROP_AMOUNT);
    printf("🔓 Unlock: %.2f - %.2f USDTg purchase\n", MIN_PURCHASE_UNLOCK, MAX_PURCHASE_UNLOCK);
    printf("⚠️  Locked tokens: FEE ONLY\n");
    printf("==============================================\n\n");
    
    // Initialize airdrop system
    if (!airdrop_init()) {
        fprintf(stderr, "❌ Failed to initialize airdrop system\n");
        return 1;
    }
    
    // Create socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("❌ Socket creation failed");
        return 1;
    }
    
    // Set socket options
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Bind socket
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("❌ Failed to bind port");
        close(server_socket);
        return 1;
    }
    
    // Listen
    if (listen(server_socket, 10) < 0) {
        perror("❌ Listen failed");
        close(server_socket);
        return 1;
    }
    
    printf("✅ Server started successfully\n");
    printf("📡 Listening on http://localhost:%d\n\n", PORT);
    
    // Accept connections
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            perror("❌ Accept failed");
            continue;
        }
        
        // Read request
        char buffer[BUFFER_SIZE] = {0};
        read(client_socket, buffer, sizeof(buffer) - 1);
        
        // Parse request
        char method[16], path[256];
        sscanf(buffer, "%s %s", method, path);
        
        // Find body
        char* body = strstr(buffer, "\r\n\r\n");
        if (body) body += 4;
        
        printf("📨 %s %s\n", method, path);
        
        // Handle OPTIONS (CORS preflight)
        if (strcmp(method, "OPTIONS") == 0) {
            send_response(client_socket, 200, "OK", "text/plain", "");
        }
        // Route requests
        else if (strcmp(path, "/api/airdrop/create") == 0 && strcmp(method, "POST") == 0) {
            handle_create_airdrop(client_socket, body);
        }
        else if (strcmp(path, "/api/airdrop/use-fee") == 0 && strcmp(method, "POST") == 0) {
            handle_use_fee(client_socket, body);
        }
        else if (strcmp(path, "/api/airdrop/verify-purchase") == 0 && strcmp(method, "POST") == 0) {
            handle_verify_purchase(client_socket, body);
        }
        else if (strncmp(path, "/api/airdrop/status", 19) == 0 && strcmp(method, "GET") == 0) {
            handle_get_status(client_socket, path);
        }
        else if (strcmp(path, "/api/airdrop/stats") == 0 && strcmp(method, "GET") == 0) {
            handle_get_stats(client_socket);
        }
        else {
            send_response(client_socket, 404, "Not Found", "application/json",
                         "{\"error\":\"Endpoint not found\"}");
        }
        
        close(client_socket);
    }
    
    close(server_socket);
    return 0;
}

