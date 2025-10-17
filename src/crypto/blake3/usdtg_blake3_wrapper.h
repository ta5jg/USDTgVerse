#ifndef USDTG_BLAKE3_WRAPPER_H
#define USDTG_BLAKE3_WRAPPER_H

#include "blake3.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// USDTgVerse BLAKE3 wrapper functions
void usdtg_blake3_hash(const uint8_t* data, size_t len, uint8_t result[32]);
void usdtg_blake3_init(blake3_hasher* hasher);
void usdtg_blake3_update(blake3_hasher* hasher, const uint8_t* data, size_t len);
void usdtg_blake3_finalize(blake3_hasher* hasher, uint8_t result[32]);

// Convenience functions
void usdtg_blake3_hash_string(const char* str, uint8_t result[32]);
void usdtg_blake3_hash_bytes(const uint8_t* data, size_t len, uint8_t result[32]);

#ifdef __cplusplus
}
#endif

#endif // USDTG_BLAKE3_WRAPPER_H
