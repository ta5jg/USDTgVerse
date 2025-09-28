/*
 * USDTgVerse Native IDE - File Manager
 * 
 * Created: 28.09.2025
 * Last Update: 28.09.2025
 * 
 * Description: File management functions for USDTgVerse Native IDE
 * Author: USDTgVerse Development Team
 * License: MIT
 */

#include "../include/ide-core.h"
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

// Save file content
int file_save(const char *file_path, const char *content) {
    if (!file_path || !content) {
        log_error("Invalid parameters for file_save");
        return -1;
    }
    
    FILE *file = fopen(file_path, "w");
    if (!file) {
        log_error("Failed to open file for writing");
        return -1;
    }
    
    size_t content_len = strlen(content);
    size_t written = fwrite(content, 1, content_len, file);
    fclose(file);
    
    if (written != content_len) {
        log_error("Failed to write all content to file");
        return -1;
    }
    
    log_info("File saved successfully");
    return 0;
}

// Load file content
int file_load(const char *file_path, char *content, size_t max_size) {
    if (!file_path || !content || max_size == 0) {
        log_error("Invalid parameters for file_load");
        return -1;
    }
    
    FILE *file = fopen(file_path, "r");
    if (!file) {
        log_error("Failed to open file for reading");
        return -1;
    }
    
    size_t read_size = fread(content, 1, max_size - 1, file);
    fclose(file);
    
    content[read_size] = '\0';
    
    log_info("File loaded successfully");
    return 0;
}

// Create new file
int file_create(const char *file_path) {
    if (!file_path) {
        log_error("Invalid file path for file_create");
        return -1;
    }
    
    FILE *file = fopen(file_path, "w");
    if (!file) {
        log_error("Failed to create file");
        return -1;
    }
    
    fclose(file);
    log_info("File created successfully");
    return 0;
}

// Delete file
int file_delete(const char *file_path) {
    if (!file_path) {
        log_error("Invalid file path for file_delete");
        return -1;
    }
    
    if (unlink(file_path) != 0) {
        log_error("Failed to delete file");
        return -1;
    }
    
    log_info("File deleted successfully");
    return 0;
}

// Check if file exists
int file_exists(const char *file_path) {
    if (!file_path) return 0;
    
    struct stat st;
    return (stat(file_path, &st) == 0);
}

// Get file modification time
time_t file_get_mtime(const char *file_path) {
    if (!file_path) return 0;
    
    struct stat st;
    if (stat(file_path, &st) == 0) {
        return st.st_mtime;
    }
    
    return 0;
}

// create_directory function is implemented in ide-utils.c

// Get file size
long file_get_size(const char *file_path) {
    if (!file_path) return -1;
    
    struct stat st;
    if (stat(file_path, &st) == 0) {
        return st.st_size;
    }
    
    return -1;
}

// Check if path is directory
int is_directory(const char *path) {
    if (!path) return 0;
    
    struct stat st;
    if (stat(path, &st) == 0) {
        return S_ISDIR(st.st_mode);
    }
    
    return 0;
}

// List directory contents
int list_directory(const char *dir_path, char *output, size_t max_size) {
    if (!dir_path || !output || max_size == 0) {
        log_error("Invalid parameters for list_directory");
        return -1;
    }
    
    DIR *dir = opendir(dir_path);
    if (!dir) {
        log_error("Failed to open directory");
        return -1;
    }
    
    struct dirent *entry;
    size_t output_len = 0;
    
    output[0] = '\0';
    
    while ((entry = readdir(dir)) != NULL) {
        // Skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }
        
        // Check if we have enough space
        size_t name_len = strlen(entry->d_name);
        if (output_len + name_len + 2 > max_size) {
            break;
        }
        
        // Add to output
        if (output_len > 0) {
            strcat(output, "\n");
            output_len++;
        }
        
        strcat(output, entry->d_name);
        output_len += name_len;
    }
    
    closedir(dir);
    log_info("Directory listed successfully");
    return 0;
}

// Copy file
int file_copy(const char *src_path, const char *dest_path) {
    if (!src_path || !dest_path) {
        log_error("Invalid parameters for file_copy");
        return -1;
    }
    
    FILE *src = fopen(src_path, "rb");
    if (!src) {
        log_error("Failed to open source file");
        return -1;
    }
    
    FILE *dest = fopen(dest_path, "wb");
    if (!dest) {
        fclose(src);
        log_error("Failed to open destination file");
        return -1;
    }
    
    char buffer[4096];
    size_t bytes_read;
    
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        if (fwrite(buffer, 1, bytes_read, dest) != bytes_read) {
            fclose(src);
            fclose(dest);
            log_error("Failed to write to destination file");
            return -1;
        }
    }
    
    fclose(src);
    fclose(dest);
    
    log_info("File copied successfully");
    return 0;
}

// Move file
int file_move(const char *src_path, const char *dest_path) {
    if (!src_path || !dest_path) {
        log_error("Invalid parameters for file_move");
        return -1;
    }
    
    if (rename(src_path, dest_path) != 0) {
        log_error("Failed to move file");
        return -1;
    }
    
    log_info("File moved successfully");
    return 0;
}
