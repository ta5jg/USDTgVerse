#include "usdtg_blake3_wrapper.h"
#include <string.h>

// USDTgVerse BLAKE3 wrapper implementation
void usdtg_blake3_hash(const uint8_t* data, size_t len, uint8_t result[32]) {
    blake3_hasher hasher;
    blake3_hasher_init(&hasher);
    blake3_hasher_update(&hasher, data, len);
    blake3_hasher_finalize(&hasher, result, 32);
}

void usdtg_blake3_init(blake3_hasher* hasher) {
    blake3_hasher_init(hasher);
}

void usdtg_blake3_update(blake3_hasher* hasher, const uint8_t* data, size_t len) {
    blake3_hasher_update(hasher, data, len);
}

void usdtg_blake3_finalize(blake3_hasher* hasher, uint8_t result[32]) {
    blake3_hasher_finalize(hasher, result, 32);
}

// Convenience functions
void usdtg_blake3_hash_string(const char* str, uint8_t result[32]) {
    usdtg_blake3_hash((const uint8_t*)str, strlen(str), result);
}

void usdtg_blake3_hash_bytes(const uint8_t* data, size_t len, uint8_t result[32]) {
    usdtg_blake3_hash(data, len, result);
}
