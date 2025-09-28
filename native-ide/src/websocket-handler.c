/*
 * USDTgVerse Native IDE - WebSocket Handler
 * 
 * Created: 28.09.2025
 * Last Update: 28.09.2025
 * 
 * Description: WebSocket communication handler for USDTgVerse Native IDE
 * Author: USDTgVerse Development Team
 * License: MIT
 */

#include "../include/ide-core.h"
#include <libwebsockets.h>

// Global WebSocket context
static struct lws_context *g_ws_context = NULL;
static struct lws *g_ws_connection = NULL;

// websocket_callback is implemented in usdtgverse-ide-core.c

// WebSocket protocols are defined in usdtgverse-ide-core.c

// websocket_callback function is implemented in usdtgverse-ide-core.c

// Forward declarations for handler functions
void handle_save_file(json_object *json);
void handle_load_file(json_object *json);
void handle_compile(json_object *json);
void handle_run(json_object *json);
void handle_get_files(json_object *json);
void handle_create_file(json_object *json);
void handle_delete_file(json_object *json);
void handle_terminal_command(json_object *json);
void handle_ping(json_object *json);

// Handle WebSocket messages (implemented in usdtgverse-ide-core.c)
// This is just a placeholder to avoid multiple definition errors

// Handler functions are implemented in usdtgverse-ide-core.c
// This file only contains WebSocket server management functions

// All handler functions are implemented in usdtgverse-ide-core.c
// This file only contains WebSocket server management functions

// Initialize WebSocket server
int websocket_init(int port) {
    (void)port;
    // WebSocket initialization is handled in usdtgverse-ide-core.c
    log_info("WebSocket server initialization delegated to core");
    return 0;
}

// Cleanup WebSocket server
int websocket_cleanup() {
    if (g_ws_context) {
        lws_context_destroy(g_ws_context);
        g_ws_context = NULL;
    }
    
    log_info("WebSocket server cleaned up");
    return 0;
}

// WebSocket message functions are implemented in usdtgverse-ide-core.c
