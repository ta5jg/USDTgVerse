/*
==============================================
 File:        bonus_api_server.c
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Bonus API Server
   
   Pure C REST API server for bonus management:
   - POST /api/bonus/create - Create bonus for purchase
   - POST /api/bonus/distribute - Distribute bonus
   - GET /api/bonus/status - Get bonus status
   - GET /api/bonus/user-stats - Get user statistics
   - GET /api/bonus/system-stats - Get system statistics
   - GET /api/bonus/calculate - Calculate bonus for amount

 License:
   MIT License
==============================================
*/

#include "bonus_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 3007
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
    
    start++;
    while (*start == ' ' || *start == '\t' || *start == '"') start++;
    
    char* end = start;
    while (*end && *end != '"' && *end != ',' && *end != '}' && *end != '\n') end++;
    
    int len = end - start;
    char* value = malloc(len + 1);
    strncpy(value, start, len);
    value[len] = '\0';
    
    return value;
}

// Handle bonus creation
static void handle_create_bonus(int client_socket, const char* body) {
    char* wallet_address = get_json_field(body, "wallet_address");
    char* user_id = get_json_field(body, "user_id");
    char* amount_str = get_json_field(body, "purchase_amount");
    
    if (!wallet_address || !user_id || !amount_str) {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Missing required fields\"}");
        return;
    }
    
    double purchase_amount = atof(amount_str);
    
    if (!bonus_qualifies(purchase_amount)) {
        char response[1024];
        snprintf(response, sizeof(response),
                "{\"success\":false,\"error\":\"Purchase amount too low\",\"minimum\":%.2f}",
                BONUS_BRONZE_THRESHOLD);
        send_response(client_socket, 400, "Bad Request", "application/json", response);
        free(wallet_address);
        free(user_id);
        free(amount_str);
        return;
    }
    
    char* bonus_id = bonus_create(wallet_address, user_id, purchase_amount);
    
    if (bonus_id) {
        BonusTier tier = bonus_get_tier(purchase_amount);
        double bonus_amount = bonus_calculate(purchase_amount);
        double bonus_percent = bonus_get_percent(tier);
        
        char response[2048];
        snprintf(response, sizeof(response),
                "{\"success\":true,\"bonus_id\":\"%s\",\"purchase_amount\":%.2f,"
                "\"bonus_amount\":%.2f,\"tier\":\"%s\",\"bonus_percent\":%.2f,"
                "\"message\":\"Congratulations! You earned %.2f USDTg bonus (%s tier)\"}",
                bonus_id, purchase_amount, bonus_amount, bonus_tier_name(tier),
                bonus_percent, bonus_amount, bonus_tier_name(tier));
        
        send_response(client_socket, 200, "OK", "application/json", response);
        free(bonus_id);
    } else {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Bonus creation failed\"}");
    }
    
    free(wallet_address);
    free(user_id);
    free(amount_str);
}

// Handle bonus distribution
static void handle_distribute_bonus(int client_socket, const char* body) {
    char* bonus_id = get_json_field(body, "bonus_id");
    char* tx_hash = get_json_field(body, "transaction_hash");
    
    if (!bonus_id || !tx_hash) {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Missing required fields\"}");
        return;
    }
    
    if (bonus_distribute(bonus_id, tx_hash)) {
        char response[1024];
        snprintf(response, sizeof(response),
                "{\"success\":true,\"bonus_id\":\"%s\",\"transaction_hash\":\"%s\","
                "\"message\":\"Bonus distributed successfully\"}",
                bonus_id, tx_hash);
        
        send_response(client_socket, 200, "OK", "application/json", response);
    } else {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Bonus distribution failed\"}");
    }
    
    free(bonus_id);
    free(tx_hash);
}

// Handle bonus calculation
static void handle_calculate_bonus(int client_socket, const char* query) {
    char* amount_param = strstr(query, "amount=");
    if (!amount_param) {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Missing amount parameter\"}");
        return;
    }
    
    double purchase_amount;
    sscanf(amount_param + 7, "%lf", &purchase_amount);
    
    BonusTier tier = bonus_get_tier(purchase_amount);
    double bonus_amount = bonus_calculate(purchase_amount);
    double bonus_percent = bonus_get_percent(tier);
    bool qualifies = bonus_qualifies(purchase_amount);
    
    char response[2048];
    snprintf(response, sizeof(response),
            "{\"success\":true,\"purchase_amount\":%.2f,\"bonus_amount\":%.2f,"
            "\"tier\":\"%s\",\"bonus_percent\":%.2f,\"qualifies\":%s,"
            "\"minimum_purchase\":%.2f,\"tier_thresholds\":{"
            "\"bronze\":%.2f,\"silver\":%.2f,\"gold\":%.2f,\"platinum\":%.2f,\"diamond\":%.2f}}",
            purchase_amount, bonus_amount, bonus_tier_name(tier), bonus_percent,
            qualifies ? "true" : "false", BONUS_BRONZE_THRESHOLD,
            BONUS_BRONZE_THRESHOLD, BONUS_SILVER_THRESHOLD, BONUS_GOLD_THRESHOLD,
            BONUS_PLATINUM_THRESHOLD, BONUS_DIAMOND_THRESHOLD);
    
    send_response(client_socket, 200, "OK", "application/json", response);
}

// Handle user statistics
static void handle_user_stats(int client_socket, const char* query) {
    char* wallet_param = strstr(query, "wallet_address=");
    if (!wallet_param) {
        send_response(client_socket, 400, "Bad Request", "application/json",
                     "{\"success\":false,\"error\":\"Missing wallet_address parameter\"}");
        return;
    }
    
    char wallet_address[256];
    sscanf(wallet_param + 15, "%[^&]", wallet_address);
    
    UserBonusStats stats = bonus_get_user_stats(wallet_address);
    
    char response[2048];
    snprintf(response, sizeof(response),
            "{\"success\":true,\"wallet_address\":\"%s\",\"total_purchases\":%.2f,"
            "\"total_bonuses\":%.2f,\"bonus_count\":%u,\"highest_tier\":\"%s\","
            "\"is_vip\":%s,\"first_bonus\":%ld,\"last_bonus\":%ld}",
            wallet_address, stats.total_purchases, stats.total_bonuses,
            stats.bonus_count, bonus_tier_name(stats.highest_tier),
            stats.is_vip ? "true" : "false", stats.first_bonus, stats.last_bonus);
    
    send_response(client_socket, 200, "OK", "application/json", response);
}

// Handle system statistics
static void handle_system_stats(int client_socket) {
    BonusSystemStats stats = bonus_get_system_stats();
    
    char response[4096];
    snprintf(response, sizeof(response),
            "{\"success\":true,\"total_bonuses\":%llu,\"total_bonus_value\":%.2f,"
            "\"total_purchase_volume\":%.2f,\"bonuses_today\":%u,"
            "\"tier_breakdown\":{"
            "\"bronze\":{\"count\":%u,\"value\":%.2f},"
            "\"silver\":{\"count\":%u,\"value\":%.2f},"
            "\"gold\":{\"count\":%u,\"value\":%.2f},"
            "\"platinum\":{\"count\":%u,\"value\":%.2f},"
            "\"diamond\":{\"count\":%u,\"value\":%.2f}}}",
            stats.total_bonuses, stats.total_bonus_value, stats.total_purchase_volume,
            stats.bonuses_today,
            stats.bronze_count, stats.bronze_value,
            stats.silver_count, stats.silver_value,
            stats.gold_count, stats.gold_value,
            stats.platinum_count, stats.platinum_value,
            stats.diamond_count, stats.diamond_value);
    
    send_response(client_socket, 200, "OK", "application/json", response);
}

// Main server
int main() {
    printf("🚀 USDTgVerse Pure C Native Bonus API Server\n");
    printf("============================================\n");
    printf("📅 Starting: %s", ctime(&(time_t){time(NULL)}));
    printf("🌐 Port: %d\n", PORT);
    printf("💎 Bonus System: 10,000 USDTg → +10 USDTg\n");
    printf("🏆 Tiers: Bronze → Silver → Gold → Platinum → Diamond\n");
    printf("============================================\n\n");
    
    // Initialize bonus system
    if (!bonus_init()) {
        fprintf(stderr, "❌ Failed to initialize bonus system\n");
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
        else if (strcmp(path, "/api/bonus/create") == 0 && strcmp(method, "POST") == 0) {
            handle_create_bonus(client_socket, body);
        }
        else if (strcmp(path, "/api/bonus/distribute") == 0 && strcmp(method, "POST") == 0) {
            handle_distribute_bonus(client_socket, body);
        }
        else if (strncmp(path, "/api/bonus/calculate", 20) == 0 && strcmp(method, "GET") == 0) {
            handle_calculate_bonus(client_socket, path);
        }
        else if (strncmp(path, "/api/bonus/user-stats", 21) == 0 && strcmp(method, "GET") == 0) {
            handle_user_stats(client_socket, path);
        }
        else if (strcmp(path, "/api/bonus/system-stats") == 0 && strcmp(method, "GET") == 0) {
            handle_system_stats(client_socket);
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

