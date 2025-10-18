/**
 * @file usdtgverse_oracle_server_simple.c
 * @brief USDTgVerse Oracle Server Implementation (Pure C - Simplified)
 * @author USDTgVerse Team
 * @date 2024
 * 
 * This file implements the Oracle Server for all USDTgVerse coins
 * including RGLS Stablecoin with $1 USD peg.
 * Simplified version without CURL dependencies.
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

// ============================================================================
// CONSTANTS AND DEFINITIONS
// ============================================================================

#define ORACLE_PORT 8080
#define MAX_CONNECTIONS 1000
#define ORACLE_UPDATE_INTERVAL 30 // 30 seconds

// Coin prices (in USD)
#define USDTG_PRICE 0.85  // USDTg Native Coin
#define USDTGV_PRICE 0.12 // USDTgV Voting Coin
#define USDTGG_PRICE 0.08 // USDTgG Governance Coin
#define RGLS_PRICE 1.00   // RGLS Stablecoin (pegged to $1)

// ============================================================================
// DATA STRUCTURES
// ============================================================================

typedef struct {
    char symbol[16];
    char name[64];
    double current_price;
    double target_price;
    time_t last_update;
    bool is_stablecoin;
    char description[128];
} CoinData;

typedef struct {
    CoinData coins[4]; // USDTg, USDTgV, USDTgG, RGLS
    bool server_running;
    pthread_t update_thread;
    pthread_t server_thread;
    int server_socket;
    pthread_mutex_t price_mutex;
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
 * @brief Simulate market price updates
 */
void simulate_market_updates() {
    pthread_mutex_lock(&g_oracle_server.price_mutex);
    
    // Simulate market fluctuations
    double btc_factor = 1.0 + (rand() % 200 - 100) / 10000.0; // ±1% variation
    double eth_factor = 1.0 + (rand() % 200 - 100) / 10000.0; // ±1% variation
    
    // Update USDTg prices based on simulated market correlation
    g_oracle_server.coins[0].current_price = USDTG_PRICE * btc_factor; // Correlate with BTC
    g_oracle_server.coins[1].current_price = USDTGV_PRICE * eth_factor; // Correlate with ETH
    g_oracle_server.coins[2].current_price = USDTGG_PRICE * btc_factor; // Correlate with BTC
    
    // RGLS is always $1.00 (stablecoin)
    g_oracle_server.coins[3].current_price = RGLS_PRICE;
    
    // Update timestamps
    for (int i = 0; i < 4; i++) {
        g_oracle_server.coins[i].last_update = time(NULL);
    }
    
    printf("ORACLE INFO: Updated prices - USDTg: $%.4f, USDTgV: $%.4f, USDTgG: $%.4f, RGLS: $%.2f\n",
           g_oracle_server.coins[0].current_price,
           g_oracle_server.coins[1].current_price,
           g_oracle_server.coins[2].current_price,
           g_oracle_server.coins[3].current_price);
    
    pthread_mutex_unlock(&g_oracle_server.price_mutex);
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
    pthread_mutex_lock(&g_oracle_server.price_mutex);
    
    if (strstr(buffer, "GET_ALL_PRICES") != NULL) {
        snprintf(response, sizeof(response),
                "{\"prices\":["
                "{\"symbol\":\"%s\",\"name\":\"%s\",\"price\":%.4f,\"stablecoin\":%s},"
                "{\"symbol\":\"%s\",\"name\":\"%s\",\"price\":%.4f,\"stablecoin\":%s},"
                "{\"symbol\":\"%s\",\"name\":\"%s\",\"price\":%.4f,\"stablecoin\":%s},"
                "{\"symbol\":\"%s\",\"name\":\"%s\",\"price\":%.2f,\"stablecoin\":%s}"
                "],\"timestamp\":%ld,\"status\":\"success\"}",
                g_oracle_server.coins[0].symbol, g_oracle_server.coins[0].name, g_oracle_server.coins[0].current_price, g_oracle_server.coins[0].is_stablecoin ? "true" : "false",
                g_oracle_server.coins[1].symbol, g_oracle_server.coins[1].name, g_oracle_server.coins[1].current_price, g_oracle_server.coins[1].is_stablecoin ? "true" : "false",
                g_oracle_server.coins[2].symbol, g_oracle_server.coins[2].name, g_oracle_server.coins[2].current_price, g_oracle_server.coins[2].is_stablecoin ? "true" : "false",
                g_oracle_server.coins[3].symbol, g_oracle_server.coins[3].name, g_oracle_server.coins[3].current_price, g_oracle_server.coins[3].is_stablecoin ? "true" : "false",
                time(NULL));
    } else if (strstr(buffer, "GET_RGLS_PRICE") != NULL) {
        snprintf(response, sizeof(response),
                "{\"symbol\":\"RGLS\",\"price\":%.2f,\"stablecoin\":true,\"peg\":\"USD\",\"timestamp\":%ld,\"status\":\"success\"}",
                g_oracle_server.coins[3].current_price, g_oracle_server.coins[3].last_update);
    } else if (strstr(buffer, "GET_STATUS") != NULL) {
        snprintf(response, sizeof(response),
                "{\"active\":true,\"last_update\":%ld,\"coins\":4,\"status\":\"success\"}",
                g_oracle_server.coins[0].last_update);
    } else {
        snprintf(response, sizeof(response), "{\"error\":\"Invalid request\",\"status\":\"error\"}");
    }
    
    pthread_mutex_unlock(&g_oracle_server.price_mutex);
    
    // Send response
    send(client_socket, response, strlen(response), 0);
    close(client_socket);
}

/**
 * @brief Server thread function
 */
void* server_thread_func(void* arg) {
    (void)arg; // Suppress unused parameter warning
    
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
    (void)arg; // Suppress unused parameter warning
    
    while (g_oracle_server.server_running) {
        simulate_market_updates();
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
    
    // Initialize oracle data
    memset(&g_oracle_server, 0, sizeof(g_oracle_server));
    
    // Initialize coins
    strcpy(g_oracle_server.coins[0].symbol, "USDTg");
    strcpy(g_oracle_server.coins[0].name, "USDTgVerse Native Coin");
    g_oracle_server.coins[0].current_price = USDTG_PRICE;
    g_oracle_server.coins[0].target_price = USDTG_PRICE;
    g_oracle_server.coins[0].is_stablecoin = false;
    strcpy(g_oracle_server.coins[0].description, "Native utility coin");
    
    strcpy(g_oracle_server.coins[1].symbol, "USDTgV");
    strcpy(g_oracle_server.coins[1].name, "USDTgVerse Voting Coin");
    g_oracle_server.coins[1].current_price = USDTGV_PRICE;
    g_oracle_server.coins[1].target_price = USDTGV_PRICE;
    g_oracle_server.coins[1].is_stablecoin = false;
    strcpy(g_oracle_server.coins[1].description, "Governance voting coin");
    
    strcpy(g_oracle_server.coins[2].symbol, "USDTgG");
    strcpy(g_oracle_server.coins[2].name, "USDTgVerse Governance Coin");
    g_oracle_server.coins[2].current_price = USDTGG_PRICE;
    g_oracle_server.coins[2].target_price = USDTGG_PRICE;
    g_oracle_server.coins[2].is_stablecoin = false;
    strcpy(g_oracle_server.coins[2].description, "Governance utility coin");
    
    strcpy(g_oracle_server.coins[3].symbol, "RGLS");
    strcpy(g_oracle_server.coins[3].name, "Regilis Stablecoin");
    g_oracle_server.coins[3].current_price = RGLS_PRICE;
    g_oracle_server.coins[3].target_price = RGLS_PRICE;
    g_oracle_server.coins[3].is_stablecoin = true;
    strcpy(g_oracle_server.coins[3].description, "USD-pegged stablecoin");
    
    // Initialize mutexes
    pthread_mutex_init(&g_oracle_server.price_mutex, NULL);
    
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
    printf("ORACLE SERVER: Supporting 4 coins (USDTg, USDTgV, USDTgG, RGLS)\n");
    printf("ORACLE SERVER: RGLS pegged to $1.00 USD\n");
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
    pthread_mutex_destroy(&g_oracle_server.price_mutex);
    
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
    
    pthread_mutex_lock(&g_oracle_server.price_mutex);
    
    printf("ORACLE SERVER: Status\n");
    printf("===================\n");
    printf("Server Running: %s\n", g_oracle_server.server_running ? "Yes" : "No");
    printf("Supported Coins:\n");
    
    for (int i = 0; i < 4; i++) {
        CoinData* coin = &g_oracle_server.coins[i];
        printf("  %s (%s): $%.4f %s\n", 
               coin->symbol, coin->name, coin->current_price,
               coin->is_stablecoin ? "(Stablecoin)" : "");
    }
    
    pthread_mutex_unlock(&g_oracle_server.price_mutex);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    printf("USDTGVERSE ORACLE SERVER: Starting...\n");
    
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
    
    printf("USDTGVERSE ORACLE SERVER: Test completed\n");
    return 0;
}
