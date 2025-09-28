/*
 * USDTgVerse Native IDE - Utility Functions
 * 
 * Created: 28.09.2025
 * Last Update: 28.09.2025
 * 
 * Description: Utility functions for USDTgVerse Native IDE
 * Author: USDTgVerse Development Team
 * License: MIT
 */

#include "../include/ide-core.h"
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/stat.h>

// Logging mutex
static pthread_mutex_t g_log_mutex = PTHREAD_MUTEX_INITIALIZER;

// Log info message
void log_info(const char *message) {
    pthread_mutex_lock(&g_log_mutex);
    printf("[INFO] %s: %s\n", get_timestamp(), message);
    pthread_mutex_unlock(&g_log_mutex);
}

// Log error message
void log_error(const char *message) {
    pthread_mutex_lock(&g_log_mutex);
    printf("[ERROR] %s: %s\n", get_timestamp(), message);
    pthread_mutex_unlock(&g_log_mutex);
}

// Log debug message
void log_debug(const char *message) {
    pthread_mutex_lock(&g_log_mutex);
    printf("[DEBUG] %s: %s\n", get_timestamp(), message);
    pthread_mutex_unlock(&g_log_mutex);
}

// Get current timestamp
char* get_timestamp() {
    static char timestamp[64];
    struct timeval tv;
    struct tm *tm_info;
    
    gettimeofday(&tv, NULL);
    tm_info = localtime(&tv.tv_sec);
    
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
    
    return timestamp;
}

// Create directory
int create_directory(const char *path) {
    if (!path) {
        log_error("Invalid path for create_directory");
        return -1;
    }
    
    if (mkdir(path, 0755) != 0) {
        if (errno != EEXIST) {
            log_error("Failed to create directory");
            return -1;
        }
    }
    
    log_info("Directory created successfully");
    return 0;
}

// Execute terminal command
int terminal_execute(const char *command, char *output, size_t max_size) {
    if (!command || !output || max_size == 0) {
        log_error("Invalid parameters for terminal_execute");
        return -1;
    }
    
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        log_error("Failed to execute command");
        return -1;
    }
    
    size_t total_read = 0;
    char buffer[1024];
    
    while (fgets(buffer, sizeof(buffer), pipe) != NULL && total_read < max_size - 1) {
        size_t len = strlen(buffer);
        if (total_read + len < max_size) {
            strcpy(output + total_read, buffer);
            total_read += len;
        }
    }
    
    pclose(pipe);
    output[total_read] = '\0';
    
    log_info("Command executed successfully");
    return 0;
}

// Get terminal output
int terminal_get_output(char *output, size_t max_size) {
    if (!output || max_size == 0) {
        log_error("Invalid parameters for terminal_get_output");
        return -1;
    }
    
    // Implementation would depend on the specific terminal integration
    // For now, return a placeholder
    strncpy(output, "Terminal output placeholder", max_size - 1);
    output[max_size - 1] = '\0';
    
    return 0;
}

// Project management functions
int project_create(const char *project_path) {
    if (!project_path) {
        log_error("Invalid project path");
        return -1;
    }
    
    // Create project directory
    if (create_directory(project_path) != 0) {
        return -1;
    }
    
    // Create project structure
    char src_path[512];
    char include_path[512];
    char build_path[512];
    
    snprintf(src_path, sizeof(src_path), "%s/src", project_path);
    snprintf(include_path, sizeof(include_path), "%s/include", project_path);
    snprintf(build_path, sizeof(build_path), "%s/build", project_path);
    
    create_directory(src_path);
    create_directory(include_path);
    create_directory(build_path);
    
    // Create basic project files
    char makefile_path[512];
    snprintf(makefile_path, sizeof(makefile_path), "%s/Makefile", project_path);
    
    FILE *makefile = fopen(makefile_path, "w");
    if (makefile) {
        fprintf(makefile, "# USDTgScript Project Makefile\n");
        fprintf(makefile, "CC = usdtgc\n");
        fprintf(makefile, "TARGET = project\n");
        fprintf(makefile, "SOURCES = src/*.usdtg\n");
        fprintf(makefile, "\n");
        fprintf(makefile, "all:\n");
        fprintf(makefile, "\t$(CC) $(SOURCES) -o $(TARGET)\n");
        fprintf(makefile, "\n");
        fprintf(makefile, "clean:\n");
        fprintf(makefile, "\trm -f $(TARGET)\n");
        fclose(makefile);
    }
    
    log_info("Project created successfully");
    return 0;
}

int project_open(const char *project_path) {
    if (!project_path) {
        log_error("Invalid project path");
        return -1;
    }
    
    // Check if project exists
    if (!file_exists(project_path)) {
        log_error("Project does not exist");
        return -1;
    }
    
    log_info("Project opened successfully");
    return 0;
}

int project_save(const char *project_path) {
    if (!project_path) {
        log_error("Invalid project path");
        return -1;
    }
    
    // Implementation would save project configuration
    log_info("Project saved successfully");
    return 0;
}

int project_close() {
    log_info("Project closed successfully");
    return 0;
}

// String utilities
char* str_trim(char *str) {
    if (!str) return NULL;
    
    char *end;
    
    // Trim leading space
    while (isspace((unsigned char)*str)) str++;
    
    if (*str == 0) return str;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator
    end[1] = '\0';
    
    return str;
}

int str_starts_with(const char *str, const char *prefix) {
    if (!str || !prefix) return 0;
    
    size_t len_prefix = strlen(prefix);
    size_t len_str = strlen(str);
    
    if (len_prefix > len_str) return 0;
    
    return strncmp(str, prefix, len_prefix) == 0;
}

int str_ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) return 0;
    
    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);
    
    if (len_suffix > len_str) return 0;
    
    return strncmp(str + len_str - len_suffix, suffix, len_suffix) == 0;
}

// Memory utilities
void* safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (!ptr) {
        log_error("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* safe_calloc(size_t num, size_t size) {
    void *ptr = calloc(num, size);
    if (!ptr) {
        log_error("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    return ptr;
}

void* safe_realloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr) {
        log_error("Memory reallocation failed");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

// File utilities
int file_get_extension(const char *file_path, char *extension, size_t max_size) {
    if (!file_path || !extension || max_size == 0) {
        return -1;
    }
    
    const char *dot = strrchr(file_path, '.');
    if (!dot || dot == file_path) {
        extension[0] = '\0';
        return 0;
    }
    
    strncpy(extension, dot + 1, max_size - 1);
    extension[max_size - 1] = '\0';
    
    return 0;
}

int file_get_basename(const char *file_path, char *basename, size_t max_size) {
    if (!file_path || !basename || max_size == 0) {
        return -1;
    }
    
    const char *slash = strrchr(file_path, '/');
    if (!slash) {
        strncpy(basename, file_path, max_size - 1);
    } else {
        strncpy(basename, slash + 1, max_size - 1);
    }
    
    basename[max_size - 1] = '\0';
    
    return 0;
}
