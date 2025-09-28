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

// WebSocket protocols
static struct lws_protocols websocket_protocols[] = {
    {
        "usdtgverse-ide",
        websocket_callback,
        0,
        1024,
    },
    { NULL, NULL, 0, 0 }
};

// WebSocket callback function
static int websocket_callback(struct lws *wsi, enum lws_callback_reasons reason,
                             void *user, void *in, size_t len) {
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

// Handle WebSocket messages
void handle_websocket_message(const char *message, size_t len) {
    if (!message || len == 0) {
        log_error("Invalid WebSocket message");
        return;
    }
    
    // Parse JSON message
    json_object *json = json_tokener_parse(message);
    if (!json) {
        log_error("Failed to parse JSON message");
        return;
    }
    
    // Get message type
    json_object *type_obj;
    if (json_object_object_get_ex(json, "type", &type_obj)) {
        const char *type = json_object_get_string(type_obj);
        
        // Handle different message types
        if (strcmp(type, "save_file") == 0) {
            handle_save_file(json);
        } else if (strcmp(type, "load_file") == 0) {
            handle_load_file(json);
        } else if (strcmp(type, "compile") == 0) {
            handle_compile(json);
        } else if (strcmp(type, "run") == 0) {
            handle_run(json);
        } else if (strcmp(type, "get_files") == 0) {
            handle_get_files(json);
        } else if (strcmp(type, "create_file") == 0) {
            handle_create_file(json);
        } else if (strcmp(type, "delete_file") == 0) {
            handle_delete_file(json);
        } else if (strcmp(type, "terminal_command") == 0) {
            handle_terminal_command(json);
        } else if (strcmp(type, "ping") == 0) {
            handle_ping(json);
        } else {
            log_error("Unknown message type");
        }
    }
    
    json_object_put(json);
}

// Handle save file request
void handle_save_file(json_object *json) {
    json_object *file_obj, *content_obj;
    if (json_object_object_get_ex(json, "file", &file_obj) &&
        json_object_object_get_ex(json, "content", &content_obj)) {
        
        const char *file_path = json_object_get_string(file_obj);
        const char *content = json_object_get_string(content_obj);
        
        if (file_save(file_path, content) == 0) {
            log_info("File saved successfully");
            websocket_send_message("{\"type\":\"file_saved\",\"status\":\"success\"}");
        } else {
            log_error("Failed to save file");
            websocket_send_message("{\"type\":\"file_saved\",\"status\":\"error\"}");
        }
    }
}

// Handle load file request
void handle_load_file(json_object *json) {
    json_object *file_obj;
    if (json_object_object_get_ex(json, "file", &file_obj)) {
        const char *file_path = json_object_get_string(file_obj);
        char content[MAX_CONTENT_SIZE];
        
        if (file_load(file_path, content, sizeof(content)) == 0) {
            char response[2048];
            snprintf(response, sizeof(response), 
                    "{\"type\":\"file_loaded\",\"content\":\"%s\",\"status\":\"success\"}", 
                    content);
            websocket_send_message(response);
        } else {
            websocket_send_message("{\"type\":\"file_loaded\",\"status\":\"error\"}");
        }
    }
}

// Handle compile request
void handle_compile(json_object *json) {
    json_object *file_obj;
    if (json_object_object_get_ex(json, "file", &file_obj)) {
        const char *file_path = json_object_get_string(file_obj);
        compile_info_t compile_info = {0};
        
        if (compile_usdtgscript(file_path, &compile_info) == 0) {
            char response[1024];
            snprintf(response, sizeof(response), 
                    "{\"type\":\"compile_result\",\"status\":\"success\",\"result_code\":%d}", 
                    compile_info.result_code);
            websocket_send_message(response);
        } else {
            char response[1024];
            snprintf(response, sizeof(response), 
                    "{\"type\":\"compile_result\",\"status\":\"error\",\"error\":\"%s\"}", 
                    compile_info.error_message);
            websocket_send_message(response);
        }
    }
}

// Handle run request
void handle_run(json_object *json) {
    json_object *file_obj;
    if (json_object_object_get_ex(json, "file", &file_obj)) {
        const char *file_path = json_object_get_string(file_obj);
        
        if (run_usdtgscript(file_path) == 0) {
            websocket_send_message("{\"type\":\"run_result\",\"status\":\"success\"}");
        } else {
            websocket_send_message("{\"type\":\"run_result\",\"status\":\"error\"}");
        }
    }
}

// Handle get files request
void handle_get_files(json_object *json) {
    // Implementation for getting project files
    websocket_send_message("{\"type\":\"files_list\",\"files\":[]}");
}

// Handle create file request
void handle_create_file(json_object *json) {
    json_object *file_obj;
    if (json_object_object_get_ex(json, "file", &file_obj)) {
        const char *file_path = json_object_get_string(file_obj);
        
        if (file_create(file_path) == 0) {
            websocket_send_message("{\"type\":\"file_created\",\"status\":\"success\"}");
        } else {
            websocket_send_message("{\"type\":\"file_created\",\"status\":\"error\"}");
        }
    }
}

// Handle delete file request
void handle_delete_file(json_object *json) {
    json_object *file_obj;
    if (json_object_object_get_ex(json, "file", &file_obj)) {
        const char *file_path = json_object_get_string(file_obj);
        
        if (file_delete(file_path) == 0) {
            websocket_send_message("{\"type\":\"file_deleted\",\"status\":\"success\"}");
        } else {
            websocket_send_message("{\"type\":\"file_deleted\",\"status\":\"error\"}");
        }
    }
}

// Handle terminal command request
void handle_terminal_command(json_object *json) {
    json_object *command_obj;
    if (json_object_object_get_ex(json, "command", &command_obj)) {
        const char *command = json_object_get_string(command_obj);
        char output[1024];
        
        if (terminal_execute(command, output, sizeof(output)) == 0) {
            char response[2048];
            snprintf(response, sizeof(response), 
                    "{\"type\":\"terminal_output\",\"output\":\"%s\"}", 
                    output);
            websocket_send_message(response);
        }
    }
}

// Handle ping request
void handle_ping(json_object *json) {
    websocket_send_message("{\"type\":\"pong\",\"timestamp\":\"%s\"}", get_timestamp());
}

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

// Send WebSocket message
void websocket_send_message(const char *message) {
    if (!g_ws_connection || !message) {
        return;
    }
    
    // Implementation for sending WebSocket message
    // This would need to be implemented based on the specific WebSocket library
}

// Broadcast message to all connected clients
void websocket_broadcast(const char *message) {
    if (!message) {
        return;
    }
    
    // Implementation for broadcasting to all clients
    // This would need to be implemented based on the specific WebSocket library
}
