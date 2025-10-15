/*
==============================================
 File:        bridge-api-server.c
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   Multi-Chain Bridge API Server
   
   Handles cross-chain token transfers between:
   - Binance Smart Chain (BSC)
   - Tron (TRX)
   - Ethereum (ETH)
   - Polygon, Avalanche, Arbitrum, Optimism
   - USDTgVerse Native Chain

 License:
   MIT License
==============================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>

#define PORT 8082
#define BUFFER_SIZE 4096
#define MAX_TRANSFERS 1000

// Transfer structure
typedef struct {
    char id[64];
    char from_chain[32];
    char from_token[16];
    char to_chain[32];
    char to_token[16];
    double amount;
    double exchange_rate;
    double bridge_fee;
    char status[16]; // pending, completed, failed
    long timestamp;
    char tx_hash[128];
} TransferRecord;

// Exchange rates structure
typedef struct {
    char from_token[16];
    char to_token[16];
    double rate;
    double fee_percent;
} ExchangeRate;

// Global variables
TransferRecord transfers[MAX_TRANSFERS];
int transfer_count = 0;
ExchangeRate exchange_rates[] = {
    {"USDT", "USDTg", 1.0, 0.1},
    {"USDC", "USDTg", 1.0, 0.1},
    {"USDT", "RGLS", 2.04, 0.1},
    {"USDC", "RGLS", 2.04, 0.1},
    {"USDT", "USDTgV", 2.08, 0.1},
    {"USDC", "USDTgV", 2.08, 0.1},
    {"USDT", "USDTgG", 0.5, 0.1},
    {"USDC", "USDTgG", 0.5, 0.1},
    {"BNB", "USDTg", 600.0, 0.15},
    {"ETH", "USDTg", 2000.0, 0.15},
    {"TRX", "USDTg", 0.1, 0.1},
    {"MATIC", "USDTg", 1.5, 0.1}
};
int rate_count = sizeof(exchange_rates) / sizeof(exchange_rates[0]);

// Generate unique transfer ID
void generate_transfer_id(char* id) {
    snprintf(id, 64, "TX_%ld_%d", time(NULL), rand() % 10000);
}

// Find exchange rate
double find_exchange_rate(const char* from_token, const char* to_token, double* fee_percent) {
    for (int i = 0; i < rate_count; i++) {
        if (strcmp(exchange_rates[i].from_token, from_token) == 0 &&
            strcmp(exchange_rates[i].to_token, to_token) == 0) {
            *fee_percent = exchange_rates[i].fee_percent;
            return exchange_rates[i].rate;
        }
    }
    *fee_percent = 0.1; // Default fee
    return 1.0; // Default rate
}

// Create new transfer
TransferRecord* create_transfer(const char* from_chain, const char* from_token,
                               const char* to_chain, const char* to_token,
                               double amount) {
    if (transfer_count >= MAX_TRANSFERS) {
        return NULL;
    }
    
    TransferRecord* transfer = &transfers[transfer_count++];
    
    generate_transfer_id(transfer->id);
    strcpy(transfer->from_chain, from_chain);
    strcpy(transfer->from_token, from_token);
    strcpy(transfer->to_chain, to_chain);
    strcpy(transfer->to_token, to_token);
    transfer->amount = amount;
    
    double fee_percent;
    transfer->exchange_rate = find_exchange_rate(from_token, to_token, &fee_percent);
    transfer->bridge_fee = amount * fee_percent / 100.0;
    
    strcpy(transfer->status, "pending");
    transfer->timestamp = time(NULL);
    
    // Generate mock transaction hash
    snprintf(transfer->tx_hash, 128, "0x%lx%lx%lx", 
             time(NULL), rand(), rand());
    
    return transfer;
}

// Get transfer by ID
TransferRecord* get_transfer_by_id(const char* id) {
    for (int i = 0; i < transfer_count; i++) {
        if (strcmp(transfers[i].id, id) == 0) {
            return &transfers[i];
        }
    }
    return NULL;
}

// Handle exchange rate request
void handle_exchange_rate_request(int client_socket, const char* request) {
    char response_buffer[BUFFER_SIZE];
    
    // Extract parameters from request
    char from_token[16] = "";
    char to_token[16] = "";
    
    char* from_start = strstr(request, "from=");
    char* to_start = strstr(request, "to=");
    
    if (from_start) {
        from_start += 5;
        int i = 0;
        while (*from_start && *from_start != ' ' && *from_start != '&' && i < 15) {
            from_token[i++] = *from_start++;
        }
        from_token[i] = '\0';
    }
    
    if (to_start) {
        to_start += 3;
        int i = 0;
        while (*to_start && *to_start != ' ' && *to_start != '&' && i < 15) {
            to_token[i++] = *to_start++;
        }
        to_token[i] = '\0';
    }
    
    if (strlen(from_token) > 0 && strlen(to_token) > 0) {
        double fee_percent;
        double rate = find_exchange_rate(from_token, to_token, &fee_percent);
        
        snprintf(response_buffer, sizeof(response_buffer),
                "{\"from_token\": \"%s\", \"to_token\": \"%s\", \"rate\": %.6f, \"fee_percent\": %.2f}",
                from_token, to_token, rate, fee_percent);
    } else {
        // Return all available rates
        strcpy(response_buffer, "{\"rates\": [");
        for (int i = 0; i < rate_count; i++) {
            char rate_str[200];
            snprintf(rate_str, sizeof(rate_str),
                    "%s{\"from\": \"%s\", \"to\": \"%s\", \"rate\": %.6f, \"fee\": %.2f}",
                    i > 0 ? "," : "",
                    exchange_rates[i].from_token,
                    exchange_rates[i].to_token,
                    exchange_rates[i].rate,
                    exchange_rates[i].fee_percent);
            strcat(response_buffer, rate_str);
        }
        strcat(response_buffer, "]}");
    }
    
    // Send HTTP response
    char http_response[BUFFER_SIZE * 2];
    snprintf(http_response, sizeof(http_response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "%s",
            strlen(response_buffer),
            response_buffer);
    
    send(client_socket, http_response, strlen(http_response), 0);
}

// Handle transfer creation
void handle_transfer_request(int client_socket, const char* request) {
    char response_buffer[BUFFER_SIZE];
    
    // Extract transfer parameters
    char from_chain[32] = "";
    char from_token[16] = "";
    char to_chain[32] = "";
    char to_token[16] = "";
    double amount = 0.0;
    
    // Parse JSON-like parameters (simplified)
    char* from_chain_start = strstr(request, "from_chain=");
    char* from_token_start = strstr(request, "from_token=");
    char* to_chain_start = strstr(request, "to_chain=");
    char* to_token_start = strstr(request, "to_token=");
    char* amount_start = strstr(request, "amount=");
    
    if (from_chain_start) {
        from_chain_start += 11;
        int i = 0;
        while (*from_chain_start && *from_chain_start != '&' && i < 31) {
            from_chain[i++] = *from_chain_start++;
        }
        from_chain[i] = '\0';
    }
    
    if (from_token_start) {
        from_token_start += 11;
        int i = 0;
        while (*from_token_start && *from_token_start != '&' && i < 15) {
            from_token[i++] = *from_token_start++;
        }
        from_token[i] = '\0';
    }
    
    if (to_chain_start) {
        to_chain_start += 9;
        int i = 0;
        while (*to_chain_start && *to_chain_start != '&' && i < 31) {
            to_chain[i++] = *to_chain_start++;
        }
        to_chain[i] = '\0';
    }
    
    if (to_token_start) {
        to_token_start += 9;
        int i = 0;
        while (*to_token_start && *to_token_start != '&' && i < 15) {
            to_token[i++] = *to_token_start++;
        }
        to_token[i] = '\0';
    }
    
    if (amount_start) {
        amount = atof(amount_start + 7);
    }
    
    if (strlen(from_chain) > 0 && strlen(from_token) > 0 && 
        strlen(to_chain) > 0 && strlen(to_token) > 0 && amount > 0) {
        
        TransferRecord* transfer = create_transfer(from_chain, from_token, 
                                                  to_chain, to_token, amount);
        
        if (transfer) {
            snprintf(response_buffer, sizeof(response_buffer),
                    "{\"success\": true, \"transfer_id\": \"%s\", \"status\": \"%s\", "
                    "\"exchange_rate\": %.6f, \"bridge_fee\": %.6f, \"tx_hash\": \"%s\"}",
                    transfer->id, transfer->status, transfer->exchange_rate, 
                    transfer->bridge_fee, transfer->tx_hash);
        } else {
            strcpy(response_buffer, "{\"success\": false, \"error\": \"Transfer limit exceeded\"}");
        }
    } else {
        strcpy(response_buffer, "{\"success\": false, \"error\": \"Invalid parameters\"}");
    }
    
    // Send HTTP response
    char http_response[BUFFER_SIZE * 2];
    snprintf(http_response, sizeof(http_response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "%s",
            strlen(response_buffer),
            response_buffer);
    
    send(client_socket, http_response, strlen(http_response), 0);
}

// Handle transfer status request
void handle_status_request(int client_socket, const char* request) {
    char response_buffer[BUFFER_SIZE];
    
    // Extract transfer ID
    char transfer_id[64] = "";
    char* id_start = strstr(request, "id=");
    
    if (id_start) {
        id_start += 3;
        int i = 0;
        while (*id_start && *id_start != ' ' && *id_start != '&' && i < 63) {
            transfer_id[i++] = *id_start++;
        }
        transfer_id[i] = '\0';
    }
    
    if (strlen(transfer_id) > 0) {
        TransferRecord* transfer = get_transfer_by_id(transfer_id);
        
        if (transfer) {
            snprintf(response_buffer, sizeof(response_buffer),
                    "{\"transfer_id\": \"%s\", \"status\": \"%s\", \"from_chain\": \"%s\", "
                    "\"from_token\": \"%s\", \"to_chain\": \"%s\", \"to_token\": \"%s\", "
                    "\"amount\": %.6f, \"exchange_rate\": %.6f, \"bridge_fee\": %.6f, "
                    "\"tx_hash\": \"%s\", \"timestamp\": %ld}",
                    transfer->id, transfer->status, transfer->from_chain, transfer->from_token,
                    transfer->to_chain, transfer->to_token, transfer->amount,
                    transfer->exchange_rate, transfer->bridge_fee, transfer->tx_hash,
                    transfer->timestamp);
        } else {
            strcpy(response_buffer, "{\"error\": \"Transfer not found\"}");
        }
    } else {
        // Return all transfers
        strcpy(response_buffer, "{\"transfers\": [");
        for (int i = 0; i < transfer_count; i++) {
            char transfer_str[500];
            snprintf(transfer_str, sizeof(transfer_str),
                    "%s{\"id\": \"%s\", \"status\": \"%s\", \"from\": \"%s %s\", \"to\": \"%s %s\", \"amount\": %.6f, \"timestamp\": %ld}",
                    i > 0 ? "," : "",
                    transfers[i].id, transfers[i].status, transfers[i].from_chain, transfers[i].from_token,
                    transfers[i].to_chain, transfers[i].to_token, transfers[i].amount, transfers[i].timestamp);
            strcat(response_buffer, transfer_str);
        }
        strcat(response_buffer, "]}");
    }
    
    // Send HTTP response
    char http_response[BUFFER_SIZE * 2];
    snprintf(http_response, sizeof(http_response),
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: application/json\r\n"
            "Access-Control-Allow-Origin: *\r\n"
            "Content-Length: %zu\r\n"
            "\r\n"
            "%s",
            strlen(response_buffer),
            response_buffer);
    
    send(client_socket, http_response, strlen(http_response), 0);
}

int main() {
    printf("🌉 Starting Multi-Chain Bridge API Server...\n");
    
    srand(time(NULL));
    
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
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        printf("Warning: setsockopt failed, continuing anyway...\n");
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
    
    printf("✅ Multi-Chain Bridge API Server listening on port %d\n", PORT);
    printf("🌐 Supporting: BSC, Tron, Ethereum, Polygon, Avalanche, Arbitrum, Optimism\n");
    printf("💰 Tokens: USDT, USDC, BNB, ETH, TRX, MATIC → USDTg, RGLS, USDTgV, USDTgG\n");
    
    // Main server loop
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }
        
        read(client_socket, buffer, BUFFER_SIZE);
        
        // Route requests
        if (strncmp(buffer, "GET /api/exchange-rate", 22) == 0) {
            handle_exchange_rate_request(client_socket, buffer);
        } else if (strncmp(buffer, "POST /api/transfer", 17) == 0) {
            handle_transfer_request(client_socket, buffer);
        } else if (strncmp(buffer, "GET /api/transfer", 16) == 0) {
            handle_status_request(client_socket, buffer);
        } else {
            // 404 for other requests
            char *not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            send(client_socket, not_found, strlen(not_found), 0);
        }
        
        close(client_socket);
        memset(buffer, 0, BUFFER_SIZE);
    }
    
    return 0;
}
