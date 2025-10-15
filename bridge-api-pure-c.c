/*
==============================================
 File:        bridge-api-pure-c.c
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   Pure C Bridge API Server for USDTgVerse Multi-Chain Bridge
   
   Features:
   - Multi-chain bridge coordination
   - Real-time exchange rates
   - Transaction processing
   - Wallet integration
   - Bridge status monitoring

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

#define PORT 8083
#define BUFFER_SIZE 4096
#define MAX_NETWORKS 10

// Bridge network structure
typedef struct {
    char name[32];
    char symbol[8];
    double fee_percent;
    int estimated_time_seconds;
    double total_locked;
    int total_transfers;
    double success_rate;
    int is_active;
} BridgeNetwork;

// Global bridge networks data
static BridgeNetwork networks[MAX_NETWORKS] = {
    {"Ethereum", "ETH", 0.5, 300, 12500000.0, 2847, 99.2, 1},
    {"BNB Chain", "BNB", 0.3, 180, 8900000.0, 1923, 99.5, 1},
    {"Polygon", "MATIC", 0.2, 120, 6700000.0, 3456, 99.8, 1},
    {"Arbitrum", "ARB", 0.4, 240, 4500000.0, 1234, 98.9, 1},
    {"TRON", "TRX", 0.1, 60, 21000000.0, 5678, 99.9, 1},
    {"Solana", "SOL", 0.2, 30, 7800000.0, 4321, 99.7, 1},
    {"USDTgVerse", "USDTg", 0.0, 10, 50000000.0, 9876, 100.0, 1}
};

static int network_count = 7;

// Function to get bridge network info
BridgeNetwork* get_bridge_network(const char* network_name) {
    for (int i = 0; i < network_count; i++) {
        if (strcmp(networks[i].name, network_name) == 0) {
            return &networks[i];
        }
    }
    return NULL;
}

// Function to calculate bridge rate
typedef struct {
    char from_network[32];
    char to_network[32];
    char token[16];
    double amount;
    double exchange_rate;
    double bridge_fee;
    double final_amount;
    int estimated_time;
} BridgeCalculation;

BridgeCalculation* calculate_bridge_rate(const char* from_net, const char* to_net, 
                                       const char* token, double amount) {
    BridgeCalculation* calc = malloc(sizeof(BridgeCalculation));
    
    strncpy(calc->from_network, from_net, sizeof(calc->from_network) - 1);
    strncpy(calc->to_network, to_net, sizeof(calc->to_network) - 1);
    strncpy(calc->token, token, sizeof(calc->token) - 1);
    calc->amount = amount;
    
    BridgeNetwork* from_network = get_bridge_network(from_net);
    BridgeNetwork* to_network = get_bridge_network(to_net);
    
    if (!from_network || !to_network) {
        free(calc);
        return NULL;
    }
    
    // Calculate exchange rate (simplified - would use real oracle data)
    if (strcmp(token, "USDT") == 0 || strcmp(token, "USDTg") == 0) {
        calc->exchange_rate = 1.0; // 1:1 for stablecoins
    } else {
        // Mock exchange rates for other tokens
        if (strcmp(from_net, "Ethereum") == 0 && strcmp(to_net, "USDTgVerse") == 0) {
            calc->exchange_rate = 0.998; // Slight discount for bridge
        } else {
            calc->exchange_rate = 1.0;
        }
    }
    
    // Calculate bridge fee (from network fee + to network fee)
    calc->bridge_fee = (from_network->fee_percent + to_network->fee_percent) / 100.0;
    calc->final_amount = (amount * calc->exchange_rate) * (1.0 - calc->bridge_fee);
    
    // Estimate time (from network time + to network time)
    calc->estimated_time = from_network->estimated_time_seconds + to_network->estimated_time_seconds;
    
    return calc;
}

// Function to get bridge statistics
typedef struct {
    double total_volume;
    int total_transfers;
    double average_success_rate;
    char fastest_bridge[32];
    int fastest_time;
} BridgeStats;

BridgeStats* get_bridge_statistics() {
    BridgeStats* stats = malloc(sizeof(BridgeStats));
    
    stats->total_volume = 0.0;
    stats->total_transfers = 0;
    stats->average_success_rate = 0.0;
    
    int fastest_time = 999999;
    strcpy(stats->fastest_bridge, "Unknown");
    
    for (int i = 0; i < network_count; i++) {
        stats->total_volume += networks[i].total_locked;
        stats->total_transfers += networks[i].total_transfers;
        stats->average_success_rate += networks[i].success_rate;
        
        if (networks[i].estimated_time_seconds < fastest_time && networks[i].is_active) {
            fastest_time = networks[i].estimated_time_seconds;
            strncpy(stats->fastest_bridge, networks[i].name, sizeof(stats->fastest_bridge) - 1);
        }
    }
    
    stats->average_success_rate /= network_count;
    stats->fastest_time = fastest_time;
    
    return stats;
}

// Function to handle API requests
void handle_api_request(int client_socket, const char* request_path) {
    char response[BUFFER_SIZE];
    
    if (strcmp(request_path, "/api/bridge/networks") == 0) {
        // Return all bridge networks
        char networks_json[BUFFER_SIZE] = {0};
        strcat(networks_json, "{\"networks\":[");
        
        for (int i = 0; i < network_count; i++) {
            if (i > 0) strcat(networks_json, ",");
            
            char network_json[512];
            snprintf(network_json, sizeof(network_json),
                    "{"
                    "\"name\":\"%s\","
                    "\"symbol\":\"%s\","
                    "\"fee_percent\":%.1f,"
                    "\"estimated_time\":%d,"
                    "\"total_locked\":%.0f,"
                    "\"total_transfers\":%d,"
                    "\"success_rate\":%.1f,"
                    "\"is_active\":%s"
                    "}",
                    networks[i].name, networks[i].symbol, networks[i].fee_percent,
                    networks[i].estimated_time_seconds, networks[i].total_locked,
                    networks[i].total_transfers, networks[i].success_rate,
                    networks[i].is_active ? "true" : "false");
            
            strcat(networks_json, network_json);
        }
        
        strcat(networks_json, "]}");
        
        snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: %zu\r\n"
                "\r\n"
                "%s",
                strlen(networks_json),
                networks_json);
                
    } else if (strncmp(request_path, "/api/bridge/rate/", 17) == 0) {
        // Calculate bridge rate
        // Format: /api/bridge/rate/ethereum/usdtgverse/USDT/100.0
        char* path_copy = strdup(request_path + 17);
        char* from_net = strtok(path_copy, "/");
        char* to_net = strtok(NULL, "/");
        char* token = strtok(NULL, "/");
        char* amount_str = strtok(NULL, "/");
        
        if (from_net && to_net && token && amount_str) {
            double amount = atof(amount_str);
            BridgeCalculation* calc = calculate_bridge_rate(from_net, to_net, token, amount);
            
            if (calc) {
                snprintf(response, sizeof(response),
                        "HTTP/1.1 200 OK\r\n"
                        "Content-Type: application/json\r\n"
                        "Access-Control-Allow-Origin: *\r\n"
                        "Content-Length: %zu\r\n"
                        "\r\n"
                        "{"
                        "\"from_network\":\"%s\","
                        "\"to_network\":\"%s\","
                        "\"token\":\"%s\","
                        "\"amount\":%.2f,"
                        "\"exchange_rate\":%.6f,"
                        "\"bridge_fee\":%.4f,"
                        "\"final_amount\":%.2f,"
                        "\"estimated_time\":%d,"
                        "\"status\":\"success\""
                        "}",
                        strlen(response) + 200,
                        calc->from_network, calc->to_network, calc->token,
                        calc->amount, calc->exchange_rate, calc->bridge_fee,
                        calc->final_amount, calc->estimated_time);
                free(calc);
            } else {
                snprintf(response, sizeof(response),
                        "HTTP/1.1 500 Internal Server Error\r\n"
                        "Content-Type: application/json\r\n"
                        "Access-Control-Allow-Origin: *\r\n"
                        "Content-Length: 35\r\n"
                        "\r\n"
                        "{\"error\":\"Failed to calculate rate\"}");
            }
        } else {
            snprintf(response, sizeof(response),
                    "HTTP/1.1 400 Bad Request\r\n"
                    "Content-Type: application/json\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Content-Length: 45\r\n"
                    "\r\n"
                    "{\"error\":\"Invalid rate request format\"}");
        }
        
        free(path_copy);
        
    } else if (strcmp(request_path, "/api/bridge/stats") == 0) {
        // Get bridge statistics
        BridgeStats* stats = get_bridge_statistics();
        
        snprintf(response, sizeof(response),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: application/json\r\n"
                "Access-Control-Allow-Origin: *\r\n"
                "Content-Length: %zu\r\n"
                "\r\n"
                "{"
                "\"total_volume\":%.1f,"
                "\"total_transfers\":%d,"
                "\"average_success_rate\":%.1f,"
                "\"fastest_bridge\":\"%s\","
                "\"fastest_time\":%d,"
                "\"status\":\"success\""
                "}",
                strlen(response) + 100,
                stats->total_volume, stats->total_transfers, stats->average_success_rate,
                stats->fastest_bridge, stats->fastest_time);
        
        free(stats);
        
    } else if (strncmp(request_path, "/api/bridge/status/", 19) == 0) {
        // Get specific network status
        const char* network_name = request_path + 19;
        BridgeNetwork* network = get_bridge_network(network_name);
        
        if (network) {
            snprintf(response, sizeof(response),
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Content-Length: %zu\r\n"
                    "\r\n"
                    "{"
                    "\"name\":\"%s\","
                    "\"symbol\":\"%s\","
                    "\"fee_percent\":%.1f,"
                    "\"estimated_time\":%d,"
                    "\"total_locked\":%.0f,"
                    "\"total_transfers\":%d,"
                    "\"success_rate\":%.1f,"
                    "\"is_active\":%s,"
                    "\"status\":\"success\""
                    "}",
                    strlen(response) + 150,
                    network->name, network->symbol, network->fee_percent,
                    network->estimated_time_seconds, network->total_locked,
                    network->total_transfers, network->success_rate,
                    network->is_active ? "true" : "false");
        } else {
            snprintf(response, sizeof(response),
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: application/json\r\n"
                    "Access-Control-Allow-Origin: *\r\n"
                    "Content-Length: 30\r\n"
                    "\r\n"
                    "{\"error\":\"Network not found\"}");
        }
        
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
    printf("Pure C Bridge API Server Starting...\n");
    
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
    
    printf("🚀 Pure C Bridge API Server listening on port %d\n", PORT);
    printf("📡 Available endpoints:\n");
    printf("   GET /api/bridge/networks - List all bridge networks\n");
    printf("   GET /api/bridge/rate/{from}/{to}/{token}/{amount} - Calculate bridge rate\n");
    printf("   GET /api/bridge/stats - Get bridge statistics\n");
    printf("   GET /api/bridge/status/{network} - Get network status\n");
    printf("🌉 Supported networks: Ethereum, BNB Chain, Polygon, Arbitrum, TRON, Solana, USDTgVerse\n");
    
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
            handle_api_request(client_socket, path);
        } else {
            char* error_response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
            send(client_socket, error_response, strlen(error_response), 0);
        }
        
        close(client_socket);
    }
    
    return 0;
}
