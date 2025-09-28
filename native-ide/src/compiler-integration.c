/*
 * USDTgVerse Native IDE - Compiler Integration
 * 
 * Created: 28.09.2025
 * Last Update: 28.09.2025
 * 
 * Description: USDTgScript compiler integration for USDTgVerse Native IDE
 * Author: USDTgVerse Development Team
 * License: MIT
 */

#include "../include/ide-core.h"
#include <sys/wait.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

// Compile USDTgScript file
int compile_usdtgscript(const char *file_path, compile_info_t *info) {
    if (!file_path || !info) {
        log_error("Invalid parameters for compile_usdtgscript");
        return -1;
    }
    
    memset(info, 0, sizeof(compile_info_t));
    strncpy(info->input_file, file_path, sizeof(info->input_file) - 1);
    
    // Check if file exists
    if (!file_exists(file_path)) {
        snprintf(info->error_message, sizeof(info->error_message), 
                "File not found: %s", file_path);
        log_error(info->error_message);
        return -1;
    }
    
    // Build compiler command
    char command[512];
    snprintf(command, sizeof(command), "usdtgc %s", file_path);
    
    log_info("Compiling USDTgScript file");
    
    // Execute compiler
    int result = system(command);
    info->result_code = WEXITSTATUS(result);
    
    if (info->result_code == 0) {
        log_info("Compilation successful");
        return 0;
    } else {
        snprintf(info->error_message, sizeof(info->error_message), 
                "Compilation failed with code %d", info->result_code);
        log_error(info->error_message);
        return -1;
    }
}

// Run USDTgScript file
int run_usdtgscript(const char *file_path) {
    if (!file_path) {
        log_error("Invalid file path for run_usdtgscript");
        return -1;
    }
    
    // Check if file exists
    if (!file_exists(file_path)) {
        log_error("File not found for execution");
        return -1;
    }
    
    // Build run command
    char command[512];
    snprintf(command, sizeof(command), "usdtg-run %s", file_path);
    
    log_info("Running USDTgScript file");
    
    // Execute
    int result = system(command);
    int exit_code = WEXITSTATUS(result);
    
    if (exit_code == 0) {
        log_info("Execution successful");
        return 0;
    } else {
        log_error("Execution failed");
        return -1;
    }
}

// Debug USDTgScript file
int debug_usdtgscript(const char *file_path) {
    if (!file_path) {
        log_error("Invalid file path for debug_usdtgscript");
        return -1;
    }
    
    // Check if file exists
    if (!file_exists(file_path)) {
        log_error("File not found for debugging");
        return -1;
    }
    
    // Build debug command
    char command[512];
    snprintf(command, sizeof(command), "usdtg-debug %s", file_path);
    
    log_info("Debugging USDTgScript file");
    
    // Execute debugger
    int result = system(command);
    int exit_code = WEXITSTATUS(result);
    
    if (exit_code == 0) {
        log_info("Debug session completed");
        return 0;
    } else {
        log_error("Debug session failed");
        return -1;
    }
}

// Get compiler version
int get_compiler_version(char *version, size_t max_size) {
    if (!version || max_size == 0) {
        log_error("Invalid parameters for get_compiler_version");
        return -1;
    }
    
    // Execute version command
    FILE *pipe = popen("usdtgc --version", "r");
    if (!pipe) {
        log_error("Failed to get compiler version");
        return -1;
    }
    
    if (fgets(version, max_size, pipe) != NULL) {
        // Remove newline
        char *newline = strchr(version, '\n');
        if (newline) *newline = '\0';
        
        pclose(pipe);
        log_info("Compiler version retrieved");
        return 0;
    }
    
    pclose(pipe);
    log_error("Failed to read compiler version");
    return -1;
}

// Check compiler availability
int check_compiler_available() {
    // Try to execute compiler
    int result = system("usdtgc --version > /dev/null 2>&1");
    int exit_code = WEXITSTATUS(result);
    
    if (exit_code == 0) {
        log_info("USDTgScript compiler is available");
        return 1;
    } else {
        log_error("USDTgScript compiler is not available");
        return 0;
    }
}

// Get compilation output
int get_compilation_output(const char *file_path, char *output, size_t max_size) {
    if (!file_path || !output || max_size == 0) {
        log_error("Invalid parameters for get_compilation_output");
        return -1;
    }
    
    // Build command to capture output
    char command[512];
    snprintf(command, sizeof(command), "usdtgc %s 2>&1", file_path);
    
    // Execute and capture output
    FILE *pipe = popen(command, "r");
    if (!pipe) {
        log_error("Failed to execute compiler");
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
    
    log_info("Compilation output captured");
    return 0;
}

// Validate USDTgScript syntax
int validate_syntax(const char *file_path) {
    if (!file_path) {
        log_error("Invalid file path for validate_syntax");
        return -1;
    }
    
    // Build validation command
    char command[512];
    snprintf(command, sizeof(command), "usdtgc --syntax-check %s", file_path);
    
    log_info("Validating USDTgScript syntax");
    
    // Execute validation
    int result = system(command);
    int exit_code = WEXITSTATUS(result);
    
    if (exit_code == 0) {
        log_info("Syntax validation passed");
        return 0;
    } else {
        log_error("Syntax validation failed");
        return -1;
    }
}

// Get compiler help
int get_compiler_help(char *help, size_t max_size) {
    if (!help || max_size == 0) {
        log_error("Invalid parameters for get_compiler_help");
        return -1;
    }
    
    // Execute help command
    FILE *pipe = popen("usdtgc --help", "r");
    if (!pipe) {
        log_error("Failed to get compiler help");
        return -1;
    }
    
    size_t total_read = 0;
    char buffer[1024];
    
    while (fgets(buffer, sizeof(buffer), pipe) != NULL && total_read < max_size - 1) {
        size_t len = strlen(buffer);
        if (total_read + len < max_size) {
            strcpy(help + total_read, buffer);
            total_read += len;
        }
    }
    
    pclose(pipe);
    help[total_read] = '\0';
    
    log_info("Compiler help retrieved");
    return 0;
}
