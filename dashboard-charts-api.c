/*
==============================================
 File:        dashboard-charts-api.c
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   Pure C Dashboard Charts API Server
   
   Generates real-time chart data for:
   - Price charts (USDTg, RGLS, USDTgV, USDTgG)
   - Volume charts (24h trading volume)
   - No JavaScript dependencies
   - Ultra-fast Pure C performance

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

#define PORT 8081
#define BUFFER_SIZE 2048
#define MAX_CHART_POINTS 24

// Chart data structures
typedef struct {
    double price;
    double volume;
    long timestamp;
} ChartPoint;

typedef struct {
    ChartPoint points[MAX_CHART_POINTS];
    int count;
    char symbol[10];
} ChartData;

// Global chart data
ChartData usdtg_chart, rgls_chart, usdtgv_chart, usdtgg_chart;

// Initialize chart data with realistic values
void initialize_chart_data() {
    srand(time(NULL));
    
    // Initialize USDTg chart (stablecoin around $1.00)
    strcpy(usdtg_chart.symbol, "USDTg");
    usdtg_chart.count = MAX_CHART_POINTS;
    for (int i = 0; i < MAX_CHART_POINTS; i++) {
        usdtg_chart.points[i].price = 0.98 + (rand() % 40) / 1000.0; // 0.98-1.02
        usdtg_chart.points[i].volume = 30000 + rand() % 40000;
        usdtg_chart.points[i].timestamp = time(NULL) - (MAX_CHART_POINTS - i) * 3600;
    }
    
    // Initialize RGLS chart (growing token)
    strcpy(rgls_chart.symbol, "RGLS");
    rgls_chart.count = MAX_CHART_POINTS;
    for (int i = 0; i < MAX_CHART_POINTS; i++) {
        double base_price = 0.45 + i * 0.001; // Slight upward trend
        rgls_chart.points[i].price = base_price + (rand() % 20) / 1000.0;
        rgls_chart.points[i].volume = 15000 + rand() % 25000;
        rgls_chart.points[i].timestamp = time(NULL) - (MAX_CHART_POINTS - i) * 3600;
    }
    
    // Initialize USDTgV chart
    strcpy(usdtgv_chart.symbol, "USDTgV");
    usdtgv_chart.count = MAX_CHART_POINTS;
    for (int i = 0; i < MAX_CHART_POINTS; i++) {
        usdtgv_chart.points[i].price = 0.48 + (rand() % 40) / 1000.0; // 0.48-0.52
        usdtgv_chart.points[i].volume = 20000 + rand() % 30000;
        usdtgv_chart.points[i].timestamp = time(NULL) - (MAX_CHART_POINTS - i) * 3600;
    }
    
    // Initialize USDTgG chart
    strcpy(usdtgg_chart.symbol, "USDTgG");
    usdtgg_chart.count = MAX_CHART_POINTS;
    for (int i = 0; i < MAX_CHART_POINTS; i++) {
        usdtgg_chart.points[i].price = 1.95 + (rand() % 40) / 1000.0; // 1.95-1.99
        usdtgg_chart.points[i].volume = 10000 + rand() % 20000;
        usdtgg_chart.points[i].timestamp = time(NULL) - (MAX_CHART_POINTS - i) * 3600;
    }
}

// Update chart data (simulate real-time)
void update_chart_data() {
    // Shift all data points and add new ones
    for (int i = 0; i < MAX_CHART_POINTS - 1; i++) {
        usdtg_chart.points[i] = usdtg_chart.points[i + 1];
        rgls_chart.points[i] = rgls_chart.points[i + 1];
        usdtgv_chart.points[i] = usdtgv_chart.points[i + 1];
        usdtgg_chart.points[i] = usdtgg_chart.points[i + 1];
    }
    
    // Add new data points
    int last = MAX_CHART_POINTS - 1;
    
    // USDTg (stable)
    usdtg_chart.points[last].price = 0.98 + (rand() % 40) / 1000.0;
    usdtg_chart.points[last].volume = 30000 + rand() % 40000;
    usdtg_chart.points[last].timestamp = time(NULL);
    
    // RGLS (growing)
    double last_rgls = rgls_chart.points[last-1].price;
    rgls_chart.points[last].price = last_rgls + (rand() % 6 - 3) / 1000.0;
    if (rgls_chart.points[last].price < 0.4) rgls_chart.points[last].price = 0.4;
    if (rgls_chart.points[last].price > 0.6) rgls_chart.points[last].price = 0.6;
    rgls_chart.points[last].volume = 15000 + rand() % 25000;
    rgls_chart.points[last].timestamp = time(NULL);
    
    // USDTgV
    usdtgv_chart.points[last].price = 0.48 + (rand() % 40) / 1000.0;
    usdtgv_chart.points[last].volume = 20000 + rand() % 30000;
    usdtgv_chart.points[last].timestamp = time(NULL);
    
    // USDTgG
    usdtgg_chart.points[last].price = 1.95 + (rand() % 40) / 1000.0;
    usdtgg_chart.points[last].volume = 10000 + rand() % 20000;
    usdtgg_chart.points[last].timestamp = time(NULL);
}

// Generate JSON response for chart data
void generate_chart_json(char* buffer, const char* symbol) {
    ChartData* chart = NULL;
    
    if (strcmp(symbol, "USDTg") == 0) chart = &usdtg_chart;
    else if (strcmp(symbol, "RGLS") == 0) chart = &rgls_chart;
    else if (strcmp(symbol, "USDTgV") == 0) chart = &usdtgv_chart;
    else if (strcmp(symbol, "USDTgG") == 0) chart = &usdtgg_chart;
    
    if (!chart) {
        strcpy(buffer, "{\"error\": \"Invalid symbol\"}");
        return;
    }
    
    strcpy(buffer, "\"");
    strcat(buffer, chart->symbol);
    strcat(buffer, "\": {");
    strcat(buffer, "\"symbol\": \"");
    strcat(buffer, chart->symbol);
    strcat(buffer, "\",");
    strcat(buffer, "\"data\": [");
    
    for (int i = 0; i < chart->count; i++) {
        char point[200];
        snprintf(point, sizeof(point), 
                "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f}",
                chart->points[i].timestamp,
                chart->points[i].price,
                chart->points[i].volume);
        
        strcat(buffer, point);
        if (i < chart->count - 1) strcat(buffer, ",");
    }
    
    strcat(buffer, "]}");
}

// Handle chart API requests
void handle_chart_request(int client_socket, const char* request) {
    char response_buffer[BUFFER_SIZE * 4];
    char* symbol_start = strstr(request, "symbol=");
    
    if (!symbol_start) {
        // Return all charts
        strcpy(response_buffer, "{");
        strcat(response_buffer, "\"charts\": {");
        
        char usdtg_json[1000], rgls_json[1000], usdtgv_json[1000], usdtgg_json[1000];
        generate_chart_json(usdtg_json, "USDTg");
        generate_chart_json(rgls_json, "RGLS");
        generate_chart_json(usdtgv_json, "USDTgV");
        generate_chart_json(usdtgg_json, "USDTgG");
        
        strcat(response_buffer, usdtg_json);
        strcat(response_buffer, ",");
        strcat(response_buffer, rgls_json);
        strcat(response_buffer, ",");
        strcat(response_buffer, usdtgv_json);
        strcat(response_buffer, ",");
        strcat(response_buffer, usdtgg_json);
        strcat(response_buffer, "}");
        strcat(response_buffer, "}");
    } else {
        // Extract symbol
        char symbol[20];
        symbol_start += 7; // Skip "symbol="
        int i = 0;
        while (*symbol_start && *symbol_start != ' ' && *symbol_start != '&' && i < 19) {
            symbol[i++] = *symbol_start++;
        }
        symbol[i] = '\0';
        
        generate_chart_json(response_buffer, symbol);
    }
    
    // Send HTTP response
    char http_response[BUFFER_SIZE * 6];
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
    printf("🚀 Starting Pure C Dashboard Charts API Server...\n");
    
    // Initialize chart data
    initialize_chart_data();
    
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
    
    // Set socket options (simplified for compatibility)
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
    
    printf("✅ Pure C Charts API Server listening on port %d\n", PORT);
    printf("📊 Serving real-time chart data for USDTg, RGLS, USDTgV, USDTgG\n");
    
    // Main server loop
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;
        }
        
        read(client_socket, buffer, BUFFER_SIZE);
        
        // Check for chart API requests
        if (strncmp(buffer, "GET /api/charts", 15) == 0) {
            update_chart_data(); // Update data before serving
            handle_chart_request(client_socket, buffer);
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
