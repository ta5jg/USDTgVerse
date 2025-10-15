#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>

#define PORT 8081
#define BUFFER_SIZE 4096
#define MAX_CHART_POINTS 24

typedef struct {
    long timestamp;
    double price;
    double volume;
} ChartPoint;

typedef struct {
    char symbol[10];
    ChartPoint points[MAX_CHART_POINTS];
    int count;
} ChartData;

ChartData usdtg_chart, rgls_chart, usdtgv_chart, usdtgg_chart;

void initialize_chart_data() {
    srand(time(NULL));
    
    strcpy(usdtg_chart.symbol, "USDTg");
    usdtg_chart.count = MAX_CHART_POINTS;
    for (int i = 0; i < MAX_CHART_POINTS; i++) {
        usdtg_chart.points[i].price = 0.98 + (rand() % 40) / 1000.0;
        usdtg_chart.points[i].volume = 30000 + rand() % 40000;
        usdtg_chart.points[i].timestamp = time(NULL) - (MAX_CHART_POINTS - i) * 3600;
    }
    
    strcpy(rgls_chart.symbol, "RGLS");
    rgls_chart.count = MAX_CHART_POINTS;
    for (int i = 0; i < MAX_CHART_POINTS; i++) {
        double base_price = 0.45 + i * 0.001;
        rgls_chart.points[i].price = base_price + (rand() % 20) / 1000.0;
        rgls_chart.points[i].volume = 15000 + rand() % 25000;
        rgls_chart.points[i].timestamp = time(NULL) - (MAX_CHART_POINTS - i) * 3600;
    }
    
    strcpy(usdtgv_chart.symbol, "USDTgV");
    usdtgv_chart.count = MAX_CHART_POINTS;
    for (int i = 0; i < MAX_CHART_POINTS; i++) {
        usdtgv_chart.points[i].price = 0.48 + (rand() % 40) / 1000.0;
        usdtgv_chart.points[i].volume = 20000 + rand() % 30000;
        usdtgv_chart.points[i].timestamp = time(NULL) - (MAX_CHART_POINTS - i) * 3600;
    }

    strcpy(usdtgg_chart.symbol, "USDTgG");
    usdtgg_chart.count = MAX_CHART_POINTS;
    for (int i = 0; i < MAX_CHART_POINTS; i++) {
        usdtgg_chart.points[i].price = 1.95 + (rand() % 80) / 1000.0;
        usdtgg_chart.points[i].volume = 10000 + rand() % 20000;
        usdtgg_chart.points[i].timestamp = time(NULL) - (MAX_CHART_POINTS - i) * 3600;
    }
}

void update_chart_data() {
    for (int i = 0; i < MAX_CHART_POINTS - 1; i++) {
        usdtg_chart.points[i] = usdtg_chart.points[i + 1];
        rgls_chart.points[i] = rgls_chart.points[i + 1];
        usdtgv_chart.points[i] = usdtgv_chart.points[i + 1];
        usdtgg_chart.points[i] = usdtgg_chart.points[i + 1];
    }
    
    int last = MAX_CHART_POINTS - 1;
    
    usdtg_chart.points[last].price = 0.98 + (rand() % 40) / 1000.0;
    usdtg_chart.points[last].volume = 30000 + rand() % 40000;
    usdtg_chart.points[last].timestamp = time(NULL);
    
    double last_rgls = rgls_chart.points[last-1].price;
    rgls_chart.points[last].price = last_rgls + (rand() % 6 - 3) / 1000.0;
    if (rgls_chart.points[last].price < 0.4) rgls_chart.points[last].price = 0.4;
    rgls_chart.points[last].volume = 15000 + rand() % 25000;
    rgls_chart.points[last].timestamp = time(NULL);

    double last_usdtgv = usdtgv_chart.points[last-1].price;
    usdtgv_chart.points[last].price = last_usdtgv + (rand() % 8 - 4) / 1000.0;
    if (usdtgv_chart.points[last].price < 0.45) usdtgv_chart.points[last].price = 0.45;
    usdtgv_chart.points[last].volume = 20000 + rand() % 30000;
    usdtgv_chart.points[last].timestamp = time(NULL);

    double last_usdtgg = usdtgg_chart.points[last-1].price;
    usdtgg_chart.points[last].price = last_usdtgg + (rand() % 10 - 5) / 1000.0;
    if (usdtgg_chart.points[last].price < 1.9) usdtgg_chart.points[last].price = 1.9;
    usdtgg_chart.points[last].volume = 10000 + rand() % 20000;
    usdtgg_chart.points[last].timestamp = time(NULL);
}

void handle_charts_request(int client_socket) {
    update_chart_data();
    
    char json_response[BUFFER_SIZE * 8];
    snprintf(json_response, sizeof(json_response),
        "{"
        "\"charts\": {"
        "\"USDTg\": {"
        "\"symbol\": \"USDTg\","
        "\"data\": ["
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f}"
        "]},"
        "\"RGLS\": {"
        "\"symbol\": \"RGLS\","
        "\"data\": ["
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f}"
        "]},"
        "\"USDTgV\": {"
        "\"symbol\": \"USDTgV\","
        "\"data\": ["
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f}"
        "]},"
        "\"USDTgG\": {"
        "\"symbol\": \"USDTgG\","
        "\"data\": ["
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f},"
        "{\"time\": %ld, \"price\": %.4f, \"volume\": %.0f}"
        "]}"
        "}"
        "}",
        // USDTg data points
        usdtg_chart.points[0].timestamp, usdtg_chart.points[0].price, usdtg_chart.points[0].volume,
        usdtg_chart.points[1].timestamp, usdtg_chart.points[1].price, usdtg_chart.points[1].volume,
        usdtg_chart.points[2].timestamp, usdtg_chart.points[2].price, usdtg_chart.points[2].volume,
        usdtg_chart.points[3].timestamp, usdtg_chart.points[3].price, usdtg_chart.points[3].volume,
        usdtg_chart.points[4].timestamp, usdtg_chart.points[4].price, usdtg_chart.points[4].volume,
        usdtg_chart.points[5].timestamp, usdtg_chart.points[5].price, usdtg_chart.points[5].volume,
        usdtg_chart.points[6].timestamp, usdtg_chart.points[6].price, usdtg_chart.points[6].volume,
        usdtg_chart.points[7].timestamp, usdtg_chart.points[7].price, usdtg_chart.points[7].volume,
        usdtg_chart.points[8].timestamp, usdtg_chart.points[8].price, usdtg_chart.points[8].volume,
        usdtg_chart.points[9].timestamp, usdtg_chart.points[9].price, usdtg_chart.points[9].volume,
        usdtg_chart.points[10].timestamp, usdtg_chart.points[10].price, usdtg_chart.points[10].volume,
        usdtg_chart.points[11].timestamp, usdtg_chart.points[11].price, usdtg_chart.points[11].volume,
        usdtg_chart.points[12].timestamp, usdtg_chart.points[12].price, usdtg_chart.points[12].volume,
        usdtg_chart.points[13].timestamp, usdtg_chart.points[13].price, usdtg_chart.points[13].volume,
        usdtg_chart.points[14].timestamp, usdtg_chart.points[14].price, usdtg_chart.points[14].volume,
        usdtg_chart.points[15].timestamp, usdtg_chart.points[15].price, usdtg_chart.points[15].volume,
        usdtg_chart.points[16].timestamp, usdtg_chart.points[16].price, usdtg_chart.points[16].volume,
        usdtg_chart.points[17].timestamp, usdtg_chart.points[17].price, usdtg_chart.points[17].volume,
        usdtg_chart.points[18].timestamp, usdtg_chart.points[18].price, usdtg_chart.points[18].volume,
        usdtg_chart.points[19].timestamp, usdtg_chart.points[19].price, usdtg_chart.points[19].volume,
        usdtg_chart.points[20].timestamp, usdtg_chart.points[20].price, usdtg_chart.points[20].volume,
        usdtg_chart.points[21].timestamp, usdtg_chart.points[21].price, usdtg_chart.points[21].volume,
        usdtg_chart.points[22].timestamp, usdtg_chart.points[22].price, usdtg_chart.points[22].volume,
        usdtg_chart.points[23].timestamp, usdtg_chart.points[23].price, usdtg_chart.points[23].volume,
        // RGLS data points
        rgls_chart.points[0].timestamp, rgls_chart.points[0].price, rgls_chart.points[0].volume,
        rgls_chart.points[1].timestamp, rgls_chart.points[1].price, rgls_chart.points[1].volume,
        rgls_chart.points[2].timestamp, rgls_chart.points[2].price, rgls_chart.points[2].volume,
        rgls_chart.points[3].timestamp, rgls_chart.points[3].price, rgls_chart.points[3].volume,
        rgls_chart.points[4].timestamp, rgls_chart.points[4].price, rgls_chart.points[4].volume,
        rgls_chart.points[5].timestamp, rgls_chart.points[5].price, rgls_chart.points[5].volume,
        rgls_chart.points[6].timestamp, rgls_chart.points[6].price, rgls_chart.points[6].volume,
        rgls_chart.points[7].timestamp, rgls_chart.points[7].price, rgls_chart.points[7].volume,
        rgls_chart.points[8].timestamp, rgls_chart.points[8].price, rgls_chart.points[8].volume,
        rgls_chart.points[9].timestamp, rgls_chart.points[9].price, rgls_chart.points[9].volume,
        rgls_chart.points[10].timestamp, rgls_chart.points[10].price, rgls_chart.points[10].volume,
        rgls_chart.points[11].timestamp, rgls_chart.points[11].price, rgls_chart.points[11].volume,
        rgls_chart.points[12].timestamp, rgls_chart.points[12].price, rgls_chart.points[12].volume,
        rgls_chart.points[13].timestamp, rgls_chart.points[13].price, rgls_chart.points[13].volume,
        rgls_chart.points[14].timestamp, rgls_chart.points[14].price, rgls_chart.points[14].volume,
        rgls_chart.points[15].timestamp, rgls_chart.points[15].price, rgls_chart.points[15].volume,
        rgls_chart.points[16].timestamp, rgls_chart.points[16].price, rgls_chart.points[16].volume,
        rgls_chart.points[17].timestamp, rgls_chart.points[17].price, rgls_chart.points[17].volume,
        rgls_chart.points[18].timestamp, rgls_chart.points[18].price, rgls_chart.points[18].volume,
        rgls_chart.points[19].timestamp, rgls_chart.points[19].price, rgls_chart.points[19].volume,
        rgls_chart.points[20].timestamp, rgls_chart.points[20].price, rgls_chart.points[20].volume,
        rgls_chart.points[21].timestamp, rgls_chart.points[21].price, rgls_chart.points[21].volume,
        rgls_chart.points[22].timestamp, rgls_chart.points[22].price, rgls_chart.points[22].volume,
        rgls_chart.points[23].timestamp, rgls_chart.points[23].price, rgls_chart.points[23].volume,
        // USDTgV data points
        usdtgv_chart.points[0].timestamp, usdtgv_chart.points[0].price, usdtgv_chart.points[0].volume,
        usdtgv_chart.points[1].timestamp, usdtgv_chart.points[1].price, usdtgv_chart.points[1].volume,
        usdtgv_chart.points[2].timestamp, usdtgv_chart.points[2].price, usdtgv_chart.points[2].volume,
        usdtgv_chart.points[3].timestamp, usdtgv_chart.points[3].price, usdtgv_chart.points[3].volume,
        usdtgv_chart.points[4].timestamp, usdtgv_chart.points[4].price, usdtgv_chart.points[4].volume,
        usdtgv_chart.points[5].timestamp, usdtgv_chart.points[5].price, usdtgv_chart.points[5].volume,
        usdtgv_chart.points[6].timestamp, usdtgv_chart.points[6].price, usdtgv_chart.points[6].volume,
        usdtgv_chart.points[7].timestamp, usdtgv_chart.points[7].price, usdtgv_chart.points[7].volume,
        usdtgv_chart.points[8].timestamp, usdtgv_chart.points[8].price, usdtgv_chart.points[8].volume,
        usdtgv_chart.points[9].timestamp, usdtgv_chart.points[9].price, usdtgv_chart.points[9].volume,
        usdtgv_chart.points[10].timestamp, usdtgv_chart.points[10].price, usdtgv_chart.points[10].volume,
        usdtgv_chart.points[11].timestamp, usdtgv_chart.points[11].price, usdtgv_chart.points[11].volume,
        usdtgv_chart.points[12].timestamp, usdtgv_chart.points[12].price, usdtgv_chart.points[12].volume,
        usdtgv_chart.points[13].timestamp, usdtgv_chart.points[13].price, usdtgv_chart.points[13].volume,
        usdtgv_chart.points[14].timestamp, usdtgv_chart.points[14].price, usdtgv_chart.points[14].volume,
        usdtgv_chart.points[15].timestamp, usdtgv_chart.points[15].price, usdtgv_chart.points[15].volume,
        usdtgv_chart.points[16].timestamp, usdtgv_chart.points[16].price, usdtgv_chart.points[16].volume,
        usdtgv_chart.points[17].timestamp, usdtgv_chart.points[17].price, usdtgv_chart.points[17].volume,
        usdtgv_chart.points[18].timestamp, usdtgv_chart.points[18].price, usdtgv_chart.points[18].volume,
        usdtgv_chart.points[19].timestamp, usdtgv_chart.points[19].price, usdtgv_chart.points[19].volume,
        usdtgv_chart.points[20].timestamp, usdtgv_chart.points[20].price, usdtgv_chart.points[20].volume,
        usdtgv_chart.points[21].timestamp, usdtgv_chart.points[21].price, usdtgv_chart.points[21].volume,
        usdtgv_chart.points[22].timestamp, usdtgv_chart.points[22].price, usdtgv_chart.points[22].volume,
        usdtgv_chart.points[23].timestamp, usdtgv_chart.points[23].price, usdtgv_chart.points[23].volume,
        // USDTgG data points
        usdtgg_chart.points[0].timestamp, usdtgg_chart.points[0].price, usdtgg_chart.points[0].volume,
        usdtgg_chart.points[1].timestamp, usdtgg_chart.points[1].price, usdtgg_chart.points[1].volume,
        usdtgg_chart.points[2].timestamp, usdtgg_chart.points[2].price, usdtgg_chart.points[2].volume,
        usdtgg_chart.points[3].timestamp, usdtgg_chart.points[3].price, usdtgg_chart.points[3].volume,
        usdtgg_chart.points[4].timestamp, usdtgg_chart.points[4].price, usdtgg_chart.points[4].volume,
        usdtgg_chart.points[5].timestamp, usdtgg_chart.points[5].price, usdtgg_chart.points[5].volume,
        usdtgg_chart.points[6].timestamp, usdtgg_chart.points[6].price, usdtgg_chart.points[6].volume,
        usdtgg_chart.points[7].timestamp, usdtgg_chart.points[7].price, usdtgg_chart.points[7].volume,
        usdtgg_chart.points[8].timestamp, usdtgg_chart.points[8].price, usdtgg_chart.points[8].volume,
        usdtgg_chart.points[9].timestamp, usdtgg_chart.points[9].price, usdtgg_chart.points[9].volume,
        usdtgg_chart.points[10].timestamp, usdtgg_chart.points[10].price, usdtgg_chart.points[10].volume,
        usdtgg_chart.points[11].timestamp, usdtgg_chart.points[11].price, usdtgg_chart.points[11].volume,
        usdtgg_chart.points[12].timestamp, usdtgg_chart.points[12].price, usdtgg_chart.points[12].volume,
        usdtgg_chart.points[13].timestamp, usdtgg_chart.points[13].price, usdtgg_chart.points[13].volume,
        usdtgg_chart.points[14].timestamp, usdtgg_chart.points[14].price, usdtgg_chart.points[14].volume,
        usdtgg_chart.points[15].timestamp, usdtgg_chart.points[15].price, usdtgg_chart.points[15].volume,
        usdtgg_chart.points[16].timestamp, usdtgg_chart.points[16].price, usdtgg_chart.points[16].volume,
        usdtgg_chart.points[17].timestamp, usdtgg_chart.points[17].price, usdtgg_chart.points[17].volume,
        usdtgg_chart.points[18].timestamp, usdtgg_chart.points[18].price, usdtgg_chart.points[18].volume,
        usdtgg_chart.points[19].timestamp, usdtgg_chart.points[19].price, usdtgg_chart.points[19].volume,
        usdtgg_chart.points[20].timestamp, usdtgg_chart.points[20].price, usdtgg_chart.points[20].volume,
        usdtgg_chart.points[21].timestamp, usdtgg_chart.points[21].price, usdtgg_chart.points[21].volume,
        usdtgg_chart.points[22].timestamp, usdtgg_chart.points[22].price, usdtgg_chart.points[22].volume,
        usdtgg_chart.points[23].timestamp, usdtgg_chart.points[23].price, usdtgg_chart.points[23].volume
    );
    
    char http_response[BUFFER_SIZE * 10];
    snprintf(http_response, sizeof(http_response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Content-Length: %zu\r\n"
        "\r\n"
        "%s",
        strlen(json_response),
        json_response);
    
    send(client_socket, http_response, strlen(http_response), 0);
}

int main() {
    initialize_chart_data();
    
    int server_fd, client_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        printf("Warning: setsockopt failed, continuing anyway...\n");
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Pure C Dashboard Charts API Server listening on port %d\n", PORT);
    
    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        read(client_socket, buffer, BUFFER_SIZE);
        
        if (strncmp(buffer, "GET /api/charts HTTP/1.1", 24) == 0) {
            handle_charts_request(client_socket);
        } else {
            char *not_found_response = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
            send(client_socket, not_found_response, strlen(not_found_response), 0);
        }
        close(client_socket);
    }
    return 0;
}
