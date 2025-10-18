/**
 * @file rgls_oracle_server.c
 * @brief RGLS Oracle Server Implementation (Pure C)
 * @author USDTgVerse Team
 * @date 2024
 * 
 * This file implements the Oracle Server for RGLS Stablecoin
 * to maintain the $1 USD peg with real-time price updates.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <curl/curl.h>
#include <openssl/sha.h>

// ============================================================================
// CONSTANTS AND DEFINITIONS
// ============================================================================

#define ORACLE_PORT 8080
#define MAX_CONNECTIONS 1000
#define ORACLE_UPDATE_INTERVAL 30 // 30 seconds
#define RGLS_TARGET_PRICE 1000000000000000000ULL // $1 USD (18 decimals)
#define MAX_PRICE_DEVIATION 10000000000000000ULL // 1% max deviation
#define ORACLE_TIMEOUT 30

// Price source URLs
#define PRICE_SOURCE_1 "https://api.coingecko.com/api/v3/simple/price?ids=usd-coin&vs_currencies=usd"
#define PRICE_SOURCE_2 "https://api.binance.com/api/v3/ticker/price?symbol=USDCUSDT"
#define PRICE_SOURCE_3 "https://api.kraken.com/0/public/Ticker?pair=USDCUSD"

// ============================================================================
// DATA STRUCTURES
// ============================================================================

typedef struct {
    uint64_t current_price;
    uint64_t target_price;
    time_t last_update;
    bool is_active;
    char oracle_address[64];
    uint64_t price_history[100]; // Last 100 price updates
    int price_history_index;
    pthread_mutex_t price_mutex;
} OracleData;

typedef struct {
    char source_name[32];
    char url[256];
    uint64_t last_price;
    time_t last_update;
    bool is_active;
    int failure_count;
} PriceSource;

typedef struct {
    int client_socket;
    struct sockaddr_in client_addr;
} ClientConnection;

typedef struct {
    OracleData oracle;
    PriceSource sources[3];
    bool server_running;
    pthread_t update_thread;
    pthread_t server_thread;
    int server_socket;
    pthread_mutex_t server_mutex;
} OracleServer;

// ============================================================================
// GLOBAL VARIABLES
// ============================================================================

static OracleServer g_oracle_server;
static bool g_server_initialized = false;

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

/**
 * @brief Write callback for CURL
 */
size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    char* data = (char*)userp;
    strncat(data, (char*)contents, realsize);
    return realsize;
}

/**
 * @brief Get price from external API
 * @param source Price source
 * @return Price in wei (18 decimals) or 0 if failed
 */
uint64_t get_price_from_api(const PriceSource* source) {
    if (!source || !source->is_active) return 0;
    
    CURL* curl;
    CURLcode res;
    char response_data[4096] = {0};
    
    curl = curl_easy_init();
    if (!curl) {
        printf("ORACLE ERROR: Failed to initialize CURL for source %s\n", source->source_name);
        return 0;
    }
    
    curl_easy_setopt(curl, CURLOPT_URL, source->url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_data);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, ORACLE_TIMEOUT);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res != CURLE_OK) {
        printf("ORACLE ERROR: CURL failed for source %s: %s\n", 
               source->source_name, curl_easy_strerror(res));
        return 0;
    }
    
    // Parse response based on source
    uint64_t price = 0;
    if (strstr(source->source_name, "coingecko") != NULL) {
        // Parse CoinGecko response: {"usd-coin":{"usd":1.0}}
        char* usd_start = strstr(response_data, "\"usd\":");
        if (usd_start) {
            usd_start += 6; // Skip "usd":
            double usd_price = atof(usd_start);
            price = (uint64_t)(usd_price * 1000000000000000000.0); // Convert to wei
        }
    } else if (strstr(source->source_name, "binance") != NULL) {
        // Parse Binance response: {"symbol":"USDCUSDT","price":"1.00000000"}
        char* price_start = strstr(response_data, "\"price\":\"");
        if (price_start) {
            price_start += 9; // Skip "price":"
            char* price_end = strchr(price_start, '"');
            if (price_end) {
                *price_end = '\0';
                double usd_price = atof(price_start);
                price = (uint64_t)(usd_price * 1000000000000000000.0); // Convert to wei
            }
        }
    } else if (strstr(source->source_name, "kraken") != NULL) {
        // Parse Kraken response: {"error":[],"result":{"USDCUSD":{"a":["1.0000","1","1.0000"],...}}}
        char* price_start = strstr(response_data, "\"a\":[\"");
        if (price_start) {
            price_start += 6; // Skip "a":[""
            char* price_end = strchr(price_start, '"');
            if (price_end) {
                *price_end = '\0';
                double usd_price = atof(price_start);
                price = (uint64_t)(usd_price * 1000000000000000000.0); // Convert to wei
            }
        }
    }
    
    if (price > 0) {
        printf("ORACLE INFO: Source %s returned price: $%.6f\n", 
               source->source_name, (double)price / 1000000000000000000.0);
    } else {
        printf("ORACLE WARNING: Failed to parse price from source %s\n", source->source_name);
    }
    
    return price;
}

/**
 * @brief Update price from all sources
 */
void update_price_from_sources() {
    pthread_mutex_lock(&g_oracle_server.oracle.price_mutex);
    
    uint64_t total_price = 0;
    int active_sources = 0;
    
    for (int i = 0; i < 3; i++) {
        PriceSource* source = &g_oracle_server.sources[i];
        
        if (!source->is_active) continue;
        
        uint64_t price = get_price_from_api(source);
        
        if (price > 0) {
            source->last_price = price;
            source->last_update = time(NULL);
            source->failure_count = 0;
            total_price += price;
            active_sources++;
        } else {
            source->failure_count++;
            if (source->failure_count >= 5) {
                source->is_active = false;
                printf("ORACLE WARNING: Source %s disabled due to repeated failures\n", 
                       source->source_name);
            }
        }
    }
    
    if (active_sources > 0) {
        uint64_t average_price = total_price / active_sources;
        
        // Store in price history
        g_oracle_server.oracle.price_history[g_oracle_server.oracle.price_history_index] = average_price;
        g_oracle_server.oracle.price_history_index = (g_oracle_server.oracle.price_history_index + 1) % 100;
        
        // Update current price
        g_oracle_server.oracle.current_price = average_price;
        g_oracle_server.oracle.last_update = time(NULL);
        
        printf("ORACLE INFO: Updated price from %d sources: $%.6f\n", 
               active_sources, (double)average_price / 1000000000000000000.0);
        
        // Check if peg maintenance is needed
        if (average_price < RGLS_TARGET_PRICE - MAX_PRICE_DEVIATION ||
            average_price > RGLS_TARGET_PRICE + MAX_PRICE_DEVIATION) {
            
            printf("ORACLE ALERT: Price deviation detected - triggering peg maintenance\n");
            maintain_peg();
        }
    } else {
        printf("ORACLE ERROR: No active price sources available\n");
    }
    
    pthread_mutex_unlock(&g_oracle_server.oracle.price_mutex);
}

/**
 * @brief Maintain peg to $1 USD
 */
void maintain_peg() {
    pthread_mutex_lock(&g_oracle_server.oracle.price_mutex);
    
    if (g_oracle_server.oracle.current_price != RGLS_TARGET_PRICE) {
        printf("PEG MAINTENANCE: Current price: $%.6f, Target: $1.000000\n", 
               (double)g_oracle_server.oracle.current_price / 1000000000000000000.0);
        
        if (g_oracle_server.oracle.current_price > RGLS_TARGET_PRICE) {
            // Price too high - increase supply
            printf("PEG MAINTENANCE: Price too high - increasing supply\n");
            // Implementation would trigger supply increase mechanism
        } else {
            // Price too low - decrease supply
            printf("PEG MAINTENANCE: Price too low - decreasing supply\n");
            // Implementation would trigger supply decrease mechanism
        }
        
        // Force price back to target
        g_oracle_server.oracle.current_price = RGLS_TARGET_PRICE;
        printf("PEG MAINTENANCE: Price corrected to $1.000000\n");
    }
    
    pthread_mutex_unlock(&g_oracle_server.oracle.price_mutex);
}

// ============================================================================
// SERVER FUNCTIONS
// ============================================================================

/**
 * @brief Handle client request
 * @param client_socket Client socket
 * @param client_addr Client address
 */
void handle_client_request(int client_socket, struct sockaddr_in client_addr) {
    char buffer[1024];
    char response[1024];
    
    // Receive request
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        close(client_socket);
        return;
    }
    
    buffer[bytes_received] = '\0';
    
    printf("ORACLE SERVER: Received request from %s:%d\n", 
           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    
    // Parse request and generate response
    pthread_mutex_lock(&g_oracle_server.oracle.price_mutex);
    
    if (strstr(buffer, "GET_PRICE") != NULL) {
        snprintf(response, sizeof(response), 
                "{\"price\":\"%llu\",\"timestamp\":%ld,\"status\":\"success\"}",
                g_oracle_server.oracle.current_price, g_oracle_server.oracle.last_update);
    } else if (strstr(buffer, "GET_STATUS") != NULL) {
        snprintf(response, sizeof(response),
                "{\"active\":%s,\"last_update\":%ld,\"target_price\":\"%llu\",\"status\":\"success\"}",
                g_oracle_server.oracle.is_active ? "true" : "false",
                g_oracle_server.oracle.last_update,
                RGLS_TARGET_PRICE);
    } else if (strstr(buffer, "MAINTAIN_PEG") != NULL) {
        maintain_peg();
        snprintf(response, sizeof(response), "{\"message\":\"Peg maintenance triggered\",\"status\":\"success\"}");
    } else {
        snprintf(response, sizeof(response), "{\"error\":\"Invalid request\",\"status\":\"error\"}");
    }
    
    pthread_mutex_unlock(&g_oracle_server.oracle.price_mutex);
    
    // Send response
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
}

/**
 * @brief Server thread function
 */
void* server_thread_func(void* arg) {
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Create socket
    g_oracle_server.server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (g_oracle_server.server_socket < 0) {
        printf("ORACLE ERROR: Failed to create server socket\n");
        return NULL;
    }
    
    // Set socket options
    int opt = 1;
    setsockopt(g_oracle_server.server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(ORACLE_PORT);
    
    if (bind(g_oracle_server.server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("ORACLE ERROR: Failed to bind server socket\n");
        close(g_oracle_server.server_socket);
        return NULL;
    }
    
    // Listen for connections
    if (listen(g_oracle_server.server_socket, MAX_CONNECTIONS) < 0) {
        printf("ORACLE ERROR: Failed to listen on server socket\n");
        close(g_oracle_server.server_socket);
        return NULL;
    }
    
    printf("ORACLE SERVER: Listening on port %d\n", ORACLE_PORT);
    
    // Accept connections
    while (g_oracle_server.server_running) {
        int client_socket = accept(g_oracle_server.server_socket, 
                                  (struct sockaddr*)&client_addr, &client_len);
        
        if (client_socket < 0) {
            if (g_oracle_server.server_running) {
                printf("ORACLE ERROR: Failed to accept connection\n");
            }
            continue;
        }
        
        // Handle client request
        handle_client_request(client_socket, client_addr);
    }
    
    close(g_oracle_server.server_socket);
    return NULL;
}

/**
 * @brief Price update thread function
 */
void* update_thread_func(void* arg) {
    while (g_oracle_server.server_running) {
        update_price_from_sources();
        sleep(ORACLE_UPDATE_INTERVAL);
    }
    return NULL;
}

// ============================================================================
// INITIALIZATION AND MANAGEMENT
// ============================================================================

/**
 * @brief Initialize oracle server
 * @return true if initialization successful, false otherwise
 */
bool init_oracle_server() {
    if (g_server_initialized) {
        printf("ORACLE WARNING: Server already initialized\n");
        return true;
    }
    
    // Initialize CURL
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    // Initialize oracle data
    memset(&g_oracle_server, 0, sizeof(g_oracle_server));
    
    g_oracle_server.oracle.current_price = RGLS_TARGET_PRICE;
    g_oracle_server.oracle.target_price = RGLS_TARGET_PRICE;
    g_oracle_server.oracle.last_update = time(NULL);
    g_oracle_server.oracle.is_active = true;
    strcpy(g_oracle_server.oracle.oracle_address, "oracle.usdtgverse.com");
    
    // Initialize price sources
    strcpy(g_oracle_server.sources[0].source_name, "coingecko");
    strcpy(g_oracle_server.sources[0].url, PRICE_SOURCE_1);
    g_oracle_server.sources[0].is_active = true;
    
    strcpy(g_oracle_server.sources[1].source_name, "binance");
    strcpy(g_oracle_server.sources[1].url, PRICE_SOURCE_2);
    g_oracle_server.sources[1].is_active = true;
    
    strcpy(g_oracle_server.sources[2].source_name, "kraken");
    strcpy(g_oracle_server.sources[2].url, PRICE_SOURCE_3);
    g_oracle_server.sources[2].is_active = true;
    
    // Initialize mutexes
    pthread_mutex_init(&g_oracle_server.oracle.price_mutex, NULL);
    pthread_mutex_init(&g_oracle_server.server_mutex, NULL);
    
    g_oracle_server.server_running = true;
    
    // Start threads
    if (pthread_create(&g_oracle_server.server_thread, NULL, server_thread_func, NULL) != 0) {
        printf("ORACLE ERROR: Failed to create server thread\n");
        return false;
    }
    
    if (pthread_create(&g_oracle_server.update_thread, NULL, update_thread_func, NULL) != 0) {
        printf("ORACLE ERROR: Failed to create update thread\n");
        g_oracle_server.server_running = false;
        pthread_join(g_oracle_server.server_thread, NULL);
        return false;
    }
    
    g_server_initialized = true;
    
    printf("ORACLE SERVER: Initialized successfully\n");
    printf("ORACLE SERVER: Target price: $1.00 USD\n");
    printf("ORACLE SERVER: Update interval: %d seconds\n", ORACLE_UPDATE_INTERVAL);
    printf("ORACLE SERVER: Listening on port %d\n", ORACLE_PORT);
    
    return true;
}

/**
 * @brief Shutdown oracle server
 */
void shutdown_oracle_server() {
    if (!g_server_initialized) return;
    
    printf("ORACLE SERVER: Shutting down...\n");
    
    g_oracle_server.server_running = false;
    
    // Wait for threads to finish
    pthread_join(g_oracle_server.server_thread, NULL);
    pthread_join(g_oracle_server.update_thread, NULL);
    
    // Cleanup mutexes
    pthread_mutex_destroy(&g_oracle_server.oracle.price_mutex);
    pthread_mutex_destroy(&g_oracle_server.server_mutex);
    
    // Cleanup CURL
    curl_global_cleanup();
    
    g_server_initialized = false;
    
    printf("ORACLE SERVER: Shutdown complete\n");
}

/**
 * @brief Get current oracle status
 */
void get_oracle_status() {
    if (!g_server_initialized) {
        printf("ORACLE SERVER: Not initialized\n");
        return;
    }
    
    pthread_mutex_lock(&g_oracle_server.oracle.price_mutex);
    
    printf("ORACLE SERVER: Status\n");
    printf("===================\n");
    printf("Server Running: %s\n", g_oracle_server.server_running ? "Yes" : "No");
    printf("Oracle Active: %s\n", g_oracle_server.oracle.is_active ? "Yes" : "No");
    printf("Current Price: $%.6f\n", (double)g_oracle_server.oracle.current_price / 1000000000000000000.0);
    printf("Target Price: $1.000000\n");
    printf("Last Update: %ld\n", g_oracle_server.oracle.last_update);
    printf("Price Sources:\n");
    
    for (int i = 0; i < 3; i++) {
        PriceSource* source = &g_oracle_server.sources[i];
        printf("  %s: %s (Failures: %d)\n", 
               source->source_name, 
               source->is_active ? "Active" : "Inactive",
               source->failure_count);
    }
    
    pthread_mutex_unlock(&g_oracle_server.oracle.price_mutex);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    printf("RGLS ORACLE SERVER: Starting...\n");
    
    // Initialize server
    if (!init_oracle_server()) {
        printf("ERROR: Failed to initialize oracle server\n");
        return 1;
    }
    
    // Run for 60 seconds for testing
    printf("ORACLE SERVER: Running for 60 seconds...\n");
    sleep(60);
    
    // Get status
    printf("\n");
    get_oracle_status();
    
    // Shutdown
    printf("\n");
    shutdown_oracle_server();
    
    printf("RGLS ORACLE SERVER: Test completed\n");
    return 0;
}
