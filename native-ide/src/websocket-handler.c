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

// Forward declaration for websocket_callback
int websocket_callback(struct lws *wsi, enum lws_callback_reasons reason,
                      void *user, void *in, size_t len);

// WebSocket protocols
static struct lws_protocols websocket_protocols[] = {
    {
        "usdtgverse-ide",
        websocket_callback,
        0,
        1024,
        0,
        NULL,
        0
    },
    { NULL, NULL, 0, 0, 0, NULL, 0 }
};

// WebSocket callback function
int websocket_callback(struct lws *wsi, enum lws_callback_reasons reason,
                      void *user, void *in, size_t len) {
    (void)user;
    
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            log_info("WebSocket connection established");
            g_ws_connection = wsi;
            break;
            
        case LWS_CALLBACK_RECEIVE:
            handle_websocket_message((char*)in, len);
            break;
            
        case LWS_CALLBACK_CLOSED:
            log_info("WebSocket connection closed");
            g_ws_connection = NULL;
            break;
            
        case LWS_CALLBACK_SERVER_WRITEABLE:
            // Handle writeable callback
            break;
            
        default:
            break;
    }
    return 0;
}

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
    struct lws_context_creation_info info = {0};
    info.port = port;
    info.protocols = websocket_protocols;
    info.gid = -1;
    info.uid = -1;
    
    g_ws_context = lws_create_context(&info);
    if (!g_ws_context) {
        log_error("Failed to create WebSocket context");
        return -1;
    }
    
    log_info("WebSocket server initialized");
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
