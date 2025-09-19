/*
 * USDTgVerse Blockchain Core - Pure C Header
 * Ultra-high performance, deterministic, audit-ready
 */

#ifndef USDTGVERSE_CORE_H
#define USDTGVERSE_CORE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// CORE TYPES (C Style)
// ============================================================================

typedef uint8_t usdtg_hash_t[32];
typedef uint8_t usdtg_address_t[20];
typedef uint8_t usdtg_signature_t[64];
typedef uint8_t usdtg_pubkey_t[32];
typedef uint8_t usdtg_privkey_t[32];

typedef uint64_t usdtg_amount_t;
typedef uint64_t usdtg_height_t;
typedef uint64_t usdtg_nonce_t;
typedef uint64_t usdtg_timestamp_t;
typedef uint32_t usdtg_asset_id_t;

#define USDTG_DENOM_ID 1
#define USDTG_DECIMALS 1000000ULL

typedef struct {
    usdtg_asset_id_t denom_id;
    usdtg_amount_t amount;
} usdtg_coin_t;

typedef struct {
    usdtg_nonce_t nonce;
    usdtg_coin_t balances[16];
    uint8_t balance_count;
} usdtg_account_t;

// ============================================================================
// CORE FUNCTIONS
// ============================================================================

void usdtg_blake3(const uint8_t* data, size_t len, usdtg_hash_t result);
int usdtg_account_get_balance(const usdtg_account_t* account, usdtg_asset_id_t denom_id, usdtg_amount_t* balance);
int usdtg_account_set_balance(usdtg_account_t* account, usdtg_asset_id_t denom_id, usdtg_amount_t balance);
int usdtg_account_transfer(usdtg_account_t* from, usdtg_account_t* to, const usdtg_coin_t* coin);

#ifdef __cplusplus
}
#endif

#endif // USDTGVERSE_CORE_H
