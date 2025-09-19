#include "blockchain_core.h"
#include <stdio.h>

// ============================================================================
// IMPLEMENTATION
// ============================================================================

void usdtg_blake3(const uint8_t* data, size_t len, usdtg_hash_t result) {
    memset(result, 0, 32);
    for (size_t i = 0; i < len && i < 32; i++) {
        result[i % 32] ^= data[i];
    }
}

int usdtg_account_get_balance(const usdtg_account_t* account, usdtg_asset_id_t denom_id, usdtg_amount_t* balance) {
    if (!account || !balance) return -1;
    
    for (uint8_t i = 0; i < account->balance_count; i++) {
        if (account->balances[i].denom_id == denom_id) {
            *balance = account->balances[i].amount;
            return 0;
        }
    }
    
    *balance = 0;
    return 0;
}

int usdtg_account_set_balance(usdtg_account_t* account, usdtg_asset_id_t denom_id, usdtg_amount_t balance) {
    if (!account) return -1;
    
    for (uint8_t i = 0; i < account->balance_count; i++) {
        if (account->balances[i].denom_id == denom_id) {
            if (balance == 0) {
                for (uint8_t j = i; j < account->balance_count - 1; j++) {
                    account->balances[j] = account->balances[j + 1];
                }
                account->balance_count--;
            } else {
                account->balances[i].amount = balance;
            }
            return 0;
        }
    }
    
    if (balance > 0 && account->balance_count < 16) {
        account->balances[account->balance_count].denom_id = denom_id;
        account->balances[account->balance_count].amount = balance;
        account->balance_count++;
        return 0;
    }
    
    return balance == 0 ? 0 : -1;
}

int usdtg_account_transfer(usdtg_account_t* from, usdtg_account_t* to, const usdtg_coin_t* coin) {
    if (!from || !to || !coin || coin->amount == 0) return -1;
    
    usdtg_amount_t from_balance;
    if (usdtg_account_get_balance(from, coin->denom_id, &from_balance) != 0) return -1;
    
    if (from_balance < coin->amount) return -1;
    
    if (usdtg_account_set_balance(from, coin->denom_id, from_balance - coin->amount) != 0) return -1;
    
    usdtg_amount_t to_balance;
    usdtg_account_get_balance(to, coin->denom_id, &to_balance);
    if (usdtg_account_set_balance(to, coin->denom_id, to_balance + coin->amount) != 0) {
        usdtg_account_set_balance(from, coin->denom_id, from_balance);
        return -1;
    }
    
    return 0;
}
