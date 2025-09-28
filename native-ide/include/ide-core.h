/*
 * USDTgVerse Native IDE - Core Header
 * 
 * Created: 28.09.2025
 * Last Update: 28.09.2025
 * 
 * Description: Core definitions and structures for USDTgVerse Native IDE
 * Author: USDTgVerse Development Team
 * License: MIT
 */

#ifndef IDE_CORE_H
#define IDE_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <libwebsockets.h>
#include <json-c/json.h>

// IDE Configuration
#define IDE_VERSION "1.0.0"
#define IDE_NAME "USDTgVerse Native IDE"
#define WEBSOCKET_PORT 8081
#define WEB_SERVER_PORT 8000
#define MAX_FILE_SIZE 1024 * 1024  // 1MB
#define MAX_PROJECT_PATH 256
#define MAX_FILE_PATH 256
#define MAX_CONTENT_SIZE 1024 * 1024  // 1MB

// IDE Context Structure
typedef struct {
    char project_path[MAX_PROJECT_PATH];
    char current_file[MAX_FILE_PATH];
    int compiler_pid;
    int websocket_port;
    int web_server_port;
    int is_running;
    pthread_mutex_t mutex;
} ide_context_t;

// File Operations
typedef struct {
    char path[MAX_FILE_PATH];
    char content[MAX_CONTENT_SIZE];
    size_t content_size;
    time_t last_modified;
} file_info_t;

// Compiler Integration
typedef struct {
    char input_file[MAX_FILE_PATH];
    char output_file[MAX_FILE_PATH];
    int optimization_level;
    int quantum_safety;
    int result_code;
    char error_message[1024];
} compile_info_t;

// WebSocket Message Types
typedef enum {
    MSG_SAVE_FILE,
    MSG_LOAD_FILE,
    MSG_COMPILE,
    MSG_RUN,
    MSG_DEBUG,
    MSG_GET_FILES,
    MSG_CREATE_FILE,
    MSG_DELETE_FILE,
    MSG_TERMINAL_COMMAND
} message_type_t;

// WebSocket Message Structure
typedef struct {
    message_type_t type;
    char data[1024];
    size_t data_size;
} websocket_message_t;

// Function Declarations

// Core IDE Functions
int ide_init(ide_context_t *ctx);
int ide_cleanup(ide_context_t *ctx);
int ide_run(ide_context_t *ctx);

// File Management
int file_save(const char *file_path, const char *content);
int file_load(const char *file_path, char *content, size_t max_size);
int file_create(const char *file_path);
int file_delete(const char *file_path);
int file_exists(const char *file_path);
time_t file_get_mtime(const char *file_path);

// Compiler Integration
int compile_usdtgscript(const char *file_path, compile_info_t *info);
int run_usdtgscript(const char *file_path);
int debug_usdtgscript(const char *file_path);

// WebSocket Functions
int websocket_init(int port);
int websocket_cleanup();
void websocket_send_message(const char *message);
void websocket_broadcast(const char *message);

// Quantum Safe Functions
int quantum_safe_init();
int quantum_safe_encrypt(const char *data, char *encrypted, size_t max_size);
int quantum_safe_decrypt(const char *encrypted, char *data, size_t max_size);

// Utility Functions
void log_info(const char *message);
void log_error(const char *message);
void log_debug(const char *message);
char* get_timestamp();
int create_directory(const char *path);

// Project Management
int project_create(const char *project_path);
int project_open(const char *project_path);
int project_save(const char *project_path);
int project_close();

// Terminal Integration
int terminal_execute(const char *command, char *output, size_t max_size);
int terminal_get_output(char *output, size_t max_size);

#endif // IDE_CORE_H
