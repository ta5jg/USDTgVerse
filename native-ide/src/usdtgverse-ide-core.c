/*
 * USDTgVerse Native IDE - Core Implementation
 * 
 * Created: 28.09.2025
 * Last Update: 28.09.2025
 * 
 * Description: Main core implementation for USDTgVerse Native IDE
 * Author: USDTgVerse Development Team
 * License: MIT
 */

#include "../include/ide-core.h"

// Global IDE context
static ide_context_t g_ide_ctx = {0};

// WebSocket context
static struct lws_context *g_ws_context = NULL;

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

// Forward declarations
void handle_websocket_message(const char *message, size_t len);
void handle_save_file(json_object *json);
void handle_load_file(json_object *json);
void handle_compile(json_object *json);
void handle_run(json_object *json);
void handle_get_files(json_object *json);
void handle_create_file(json_object *json);
void handle_delete_file(json_object *json);
void handle_terminal_command(json_object *json);

// WebSocket callback function
static int websocket_callback(struct lws *wsi, enum lws_callback_reasons reason,
                             void *user, void *in, size_t len) {
    (void)wsi;
    (void)user;
    
    switch (reason) {
        case LWS_CALLBACK_ESTABLISHED:
            log_info("WebSocket connection established");
            break;
            
        case LWS_CALLBACK_RECEIVE:
            handle_websocket_message((char*)in, len);
            break;
            
        case LWS_CALLBACK_CLOSED:
            log_info("WebSocket connection closed");
            break;
            
        default:
            break;
    }
    return 0;
}

// Handle WebSocket messages
void handle_websocket_message(const char *message, size_t len) {
    (void)len;
    
    json_object *json = json_tokener_parse(message);
    if (!json) {
        log_error("Failed to parse JSON message");
        return;
    }
    
    json_object *type_obj;
    if (json_object_object_get_ex(json, "type", &type_obj)) {
        const char *type = json_object_get_string(type_obj);
        
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
    (void)json;
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

// Initialize IDE
int ide_init(ide_context_t *ctx) {
    if (!ctx) return -1;
    
    memset(ctx, 0, sizeof(ide_context_t));
    ctx->websocket_port = WEBSOCKET_PORT;
    ctx->web_server_port = WEB_SERVER_PORT;
    ctx->is_running = 1;
    
    // Initialize mutex
    if (pthread_mutex_init(&ctx->mutex, NULL) != 0) {
        log_error("Failed to initialize mutex");
        return -1;
    }
    
    // Initialize quantum safe
    if (quantum_safe_init() != 0) {
        log_error("Failed to initialize quantum safe");
        return -1;
    }
    
    // Initialize WebSocket server
    struct lws_context_creation_info info = {0};
    info.port = ctx->websocket_port;
    info.protocols = websocket_protocols;
    info.gid = -1;
    info.uid = -1;
    
    g_ws_context = lws_create_context(&info);
    if (!g_ws_context) {
        log_error("Failed to create WebSocket context");
        return -1;
    }
    
    log_info("USDTgVerse IDE initialized successfully");
    return 0;
}

// Cleanup IDE
int ide_cleanup(ide_context_t *ctx) {
    if (!ctx) return -1;
    
    ctx->is_running = 0;
    
    // Cleanup WebSocket
    if (g_ws_context) {
        lws_context_destroy(g_ws_context);
        g_ws_context = NULL;
    }
    
    // Cleanup mutex
    pthread_mutex_destroy(&ctx->mutex);
    
    log_info("USDTgVerse IDE cleaned up");
    return 0;
}

// Run IDE main loop
int ide_run(ide_context_t *ctx) {
    if (!ctx) return -1;
    
    log_info("Starting USDTgVerse IDE main loop");
    
    while (ctx->is_running) {
        lws_service(g_ws_context, 50);
    }
    
    return 0;
}

// Send WebSocket message
void websocket_send_message(const char *message) {
    if (!g_ws_context || !message) return;
    
    // Implementation for sending WebSocket message
    // This would need to be implemented based on the specific WebSocket library
}

// Main function
int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    
    printf("🚀 %s v%s\n", IDE_NAME, IDE_VERSION);
    printf("⚛️ Quantum-safe development environment\n");
    printf("🌐 WebSocket server on port %d\n", WEBSOCKET_PORT);
    printf("📁 Web server on port %d\n", WEB_SERVER_PORT);
    
    // Initialize IDE
    if (ide_init(&g_ide_ctx) != 0) {
        log_error("Failed to initialize IDE");
        return -1;
    }
    
    // Run IDE
    int result = ide_run(&g_ide_ctx);
    
    // Cleanup
    ide_cleanup(&g_ide_ctx);
    
    return result;
}
