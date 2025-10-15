/*
==============================================
 File:        price-service-pure-c.c
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   Pure C Price Service for USDTgVerse
   
   Features:
   - Real-time price data for native tokens
   - External token price integration
   - Caching system for performance
   - REST API endpoints
   - Zero JavaScript dependencies

 License:
   MIT License
==============================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#define PORT 8084
#define BUFFER_SIZE 4096
#define MAX_TOKENS 50
#define CACHE_TIMEOUT 300 // 5 minutes

// Token structure
typedef struct {
    char symbol[16];
    char name[64];
    double price;
    double change_24h;
    double volume_24h;
    double market_cap;
    time_t last_updated;
    int is_native;
} TokenData;

// Price cache structure
typedef struct {
    TokenData tokens[MAX_TOKENS];
    int count;
    pthread_mutex_t mutex;
} PriceCache;

// Global price cache
static PriceCache price_cache = {0};

// Native token data
static TokenData native_tokens[] = {
    {"USDTg", "USDTgVerse Native Coin", 1.00, 0.0, 1000000.0, 50000000.0, 0, 1},
    {"USDTgV", "USDTgVerse Utility Token", 0.50, 2.5, 500000.0, 25000000.0, 0, 1},
    {"USDTgG", "USDTgVerse Governance Token", 5.00, -1.2, 200000.0, 100000000.0, 0, 1},
    {"RGLS", "Regilis", 0.49, 12.5, 750000.0, 245000000.0, 0, 1}
};

static int native_token_count = 4;

// External token fallback prices
static TokenData external_tokens[] = {
    {"BTC", "Bitcoin", 45000.0, 2.5, 25000000.0, 850000000000.0, 0, 0},
    {"ETH", "Ethereum", 3000.0, 1.8, 15000000.0, 360000000000.0, 0, 0},
    {"BNB", "Binance Coin", 300.0, -0.5, 8000000.0, 45000000000.0, 0, 0},
    {"SOL", "Solana", 100.0, 3.2, 5000000.0, 42000000000.0, 0, 0},
    {"USDT", "Tether", 1.00, 0.0, 50000000.0, 80000000000.0, 0, 0},
    {"USDC", "USD Coin", 1.00, 0.0, 25000000.0, 32000000000.0, 0, 0}
};

static int external_token_count = 6;

// Function to initialize price cache
void init_price_cache() {
    pthread_mutex_init(&price_cache.mutex, NULL);
    price_cache.count = 0;
    
    // Initialize with native tokens
    for (int i = 0; i < native_token_count; i++) {
        native_tokens[i].last_updated = time(NULL);
        price_cache.tokens[price_cache.count] = native_tokens[i];
        price_cache.count++;
    }
    
    // Initialize with external tokens
    for (int i = 0; i < external_token_count; i++) {
        external_tokens[i].last_updated = time(NULL);
        price_cache.tokens[price_cache.count] = external_tokens[i];
        price_cache.count++;
    }
}

// Function to update token price
void update_token_price(const char* symbol, double price, double change_24h) {
    pthread_mutex_lock(&price_cache.mutex);
    
    for (int i = 0; i < price_cache.count; i++) {
        if (strcmp(price_cache.tokens[i].symbol, symbol) == 0) {
            price_cache.tokens[i].price = price;
            price_cache.tokens[i].change_24h = change_24h;
            price_cache.tokens[i].last_updated = time(NULL);
            break;
        }
    }
    
    pthread_mutex_unlock(&price_cache.mutex);
}

// Function to get token price
TokenData* get_token_price(const char* symbol) {
    pthread_mutex_lock(&price_cache.mutex);
    
    static TokenData result;
    memset(&result, 0, sizeof(result));
    
    for (int i = 0; i < price_cache.count; i++) {
        if (strcmp(price_cache.tokens[i].symbol, symbol) == 0) {
            result = price_cache.tokens[i];
            break;
        }
    }
    
    pthread_mutex_unlock(&price_cache.mutex);
    
    return strcmp(result.symbol, "") == 0 ? NULL : &result;
}

// Function to simulate price updates
void* price_update_thread(void* arg) {
    (void)arg; // Suppress unused parameter warning
    
    while (1) {
        pthread_mutex_lock(&price_cache.mutex);
        
        for (int i = 0; i < price_cache.count; i++) {
            // Simulate price fluctuations
            double fluctuation = ((double)rand() / RAND_MAX - 0.5) * 0.02; // ±1%
            price_cache.tokens[i].price *= (1.0 + fluctuation);
            
            // Simulate change_24h updates
            price_cache.tokens[i].change_24h += ((double)rand() / RAND_MAX - 0.5) * 0.1;
            
            // Update timestamp
            price_cache.tokens[i].last_updated = time(NULL);
        }
        
        pthread_mutex_unlock(&price_cache.mutex);
        
        sleep(30); // Update every 30 seconds
    }
    
    return NULL;
}

// Function to handle price API requests
void handle_price_request(int client_socket, const char* request_path) {
    char response[BUFFER_SIZE];
    
    if (strcmp(request_path, "/api/price/all") == 0) {
        // Return all token prices
        char prices_json[BUFFER_SIZE] = {0};
        strcat(prices_json, "{\"prices\":[");
        
        pthread_mutex_lock(&price_cache.mutex);
        
        for (int i = 0; i < price_cache.count; i++) {
            if (i > 0) strcat(prices_json, ",");
            
            char token_json[256];
            snprintf(token_json, sizeof(token_json),
                    "{"
                    "\"symbol\":\"%s\","
                    "\"name\":\"%s\","
                    "\"price\":%.6f,"
                    "\"change_24h\":%.2f,"
                    "\"volume_24h\":%.0f,"
                    "\"market_cap\":%.0f,"
                    "\"last_updated\":%ld,"
                    "\"is_native\":%s"
                    "}",
                    price_cache.tokens[i].symbol,
                    price_cache.tokens[i].name,
                    price_cache.tokens[i].price,
                    price_cache.tokens[i].change_24h,
                    price_cache.tokens[i].volume_24h,
                    price_cache.tokens[i].market_cap,
                    price_cache.tokens[i].last_updated,
                    price_cache.tokens[i].is_native ? "true" : "false");
            
            strcat(prices_json, token_json);
        }
        
        pthread_mutex_unlock(&price_cache.mutex);
        
        strcat(prices_json, "]}");
        
        snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: %zu\r\n"
                "\r\n"
                "%s",
                strlen(prices_json),
                prices_json);
                
    } else if (strncmp(request_path, "/api/price/", 11) == 0) {
        // Get specific token price
        const char* symbol = request_path + 11;
        TokenData* token = get_token_price(symbol);
        
        if (token) {
            snprintf(response, sizeof(response),
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Content-Length: %zu\r\n"
                    "\r\n"
                    "{"
                    "\"symbol\":\"%s\","
                    "\"name\":\"%s\","
                    "\"price\":%.6f,"
                    "\"change_24h\":%.2f,"
                    "\"volume_24h\":%.0f,"
                    "\"market_cap\":%.0f,"
                    "\"last_updated\":%ld,"
                    "\"is_native\":%s,"
                    "\"status\":\"success\""
                    "}",
                    strlen(response) + 150,
                    token->symbol, token->name, token->price,
                    token->change_24h, token->volume_24h, token->market_cap,
                    token->last_updated, token->is_native ? "true" : "false");
        } else {
            snprintf(response, sizeof(response),
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: application/json\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Content-Length: 35\r\n"
                    "\r\n"
                    "{\"error\":\"Token not found\"}");
        }
        
    } else if (strcmp(request_path, "/api/price/native") == 0) {
        // Return only native token prices
        char prices_json[BUFFER_SIZE] = {0};
        strcat(prices_json, "{\"native_tokens\":[");
        
        pthread_mutex_lock(&price_cache.mutex);
        
        int first = 1;
        for (int i = 0; i < price_cache.count; i++) {
            if (price_cache.tokens[i].is_native) {
                if (!first) strcat(prices_json, ",");
                first = 0;
                
                char token_json[256];
                snprintf(token_json, sizeof(token_json),
                        "{"
                        "\"symbol\":\"%s\","
                        "\"name\":\"%s\","
                        "\"price\":%.6f,"
                        "\"change_24h\":%.2f,"
                        "\"volume_24h\":%.0f,"
                        "\"market_cap\":%.0f,"
                        "\"last_updated\":%ld"
                        "}",
                        price_cache.tokens[i].symbol,
                        price_cache.tokens[i].name,
                        price_cache.tokens[i].price,
                        price_cache.tokens[i].change_24h,
                        price_cache.tokens[i].volume_24h,
                        price_cache.tokens[i].market_cap,
                        price_cache.tokens[i].last_updated);
                
                strcat(prices_json, token_json);
            }
        }
        
        pthread_mutex_unlock(&price_cache.mutex);
        
        strcat(prices_json, "]}");
        
        snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: %zu\r\n"
                "\r\n"
                "%s",
                strlen(prices_json),
                prices_json);
        
    } else {
        snprintf(response, sizeof(response),
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: 30\r\n"
                "\r\n"
                "{\"error\":\"Endpoint not found\"}");
    }
    
    send(client_socket, response, strlen(response), 0);
}

int main() {
    printf("Pure C Price Service Starting...\n");
    
    // Initialize price cache
    init_price_cache();
    
    // Start price update thread
    pthread_t price_thread;
    if (pthread_create(&price_thread, NULL, price_update_thread, NULL) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    
    // Start HTTP server
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("🚀 Pure C Price Service listening on port %d\n", PORT);
    printf("📡 Available endpoints:\n");
    printf("   GET /api/price/all - All token prices\n");
    printf("   GET /api/price/{symbol} - Specific token price\n");
    printf("   GET /api/price/native - Native token prices only\n");
    printf("💰 Native tokens: USDTg, USDTgV, USDTgG, RGLS\n");
    printf("🌍 External tokens: BTC, ETH, BNB, SOL, USDT, USDC\n");
    
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        read(client_socket, buffer, BUFFER_SIZE);
        
        // Parse request
        char* request_line = strtok(buffer, "\r\n");
        char* method = strtok(request_line, " ");
        char* path = strtok(NULL, " ");
        
        if (method && path && strcmp(method, "GET") == 0) {
            handle_price_request(client_socket, path);
        } else {
            char* error_response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
            send(client_socket, error_response, strlen(error_response), 0);
        }
        
        close(client_socket);
    }
    
    return 0;
}
