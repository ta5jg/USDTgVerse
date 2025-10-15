/*
==============================================
 File:        USDTgG_contract.c
 Author:      Irfan Gedik
 Created:     26.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgG Governance Token Contract
   Simple C-based governance token for USDTgVerse
   
   Governance token implementation featuring:
   - Token management functions
   - Governance voting mechanisms
   - DAO integration
   - Security features
   - Performance optimizations

 License:
   MIT License
==============================================
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>

#define REQUIRE(cond, msg) do { \
    if (!(cond)) { \
        fprintf(stderr, "[USDTgG][ERROR] %s\n", msg); \
        exit(EXIT_FAILURE); \
    } \
} while(0)

// Contract state
typedef struct {
    char name[64];
    char symbol[16];
    uint64_t total_supply;     // total minted supply (token units, not decimals multiplier)
    uint8_t  decimals;         // number of decimal places (0..18 typical)
    uint64_t treasury_balance;
    uint64_t development_balance;
    uint64_t marketing_balance;
    uint64_t public_balance;
    uint64_t voting_period;    // seconds
    uint64_t execution_delay;  // seconds
    uint64_t min_validator_stake; // tokens
} USDTgG_Contract;

// Initialize contract
void usdtgg_init(USDTgG_Contract* contract) {
    strcpy(contract->name, "USDTgG Governance Token");
    strcpy(contract->symbol, "USDTgG");

    contract->total_supply = 100000000ULL; // 100M tokens (not scaled by decimals)
    contract->decimals = 18;               // keep 18 unless specified otherwise

    // Allocation plan (sum must be 1000 == 100.0%)
    const uint16_t TREASURY_BP   = 400;  // 40.0%
    const uint16_t DEV_BP        = 200;  // 20.0%
    const uint16_t MKT_BP        = 100;  // 10.0%
    const uint16_t PUBLIC_BP     = 300;  // 30.0%
    const uint16_t SUM_BP = TREASURY_BP + DEV_BP + MKT_BP + PUBLIC_BP;

    REQUIRE(SUM_BP == 1000, "Allocation basis points must sum to 100.0% (1000 bps)");

    // Compute balances from basis points to avoid manual mismatches
    contract->treasury_balance    = (contract->total_supply * TREASURY_BP) / 1000ULL;
    contract->development_balance = (contract->total_supply * DEV_BP) / 1000ULL;
    contract->marketing_balance   = (contract->total_supply * MKT_BP) / 1000ULL;
    contract->public_balance      = (contract->total_supply * PUBLIC_BP) / 1000ULL;

    contract->voting_period       = 259200ULL; // 3 days
    contract->execution_delay     = 86400ULL;  // 1 day
    contract->min_validator_stake = 10000ULL;  // 10K tokens

    // Final sanity check: sums must equal total supply
    uint64_t sum_balances = contract->treasury_balance + contract->development_balance +
                            contract->marketing_balance + contract->public_balance;
    REQUIRE(sum_balances == contract->total_supply, "Supply allocation mismatch");
}

static void usdtgg_validate(const USDTgG_Contract* c) {
    REQUIRE(c->decimals <= 18, "Decimals must be 0..18");
    uint64_t sum = c->treasury_balance + c->development_balance + c->marketing_balance + c->public_balance;
    REQUIRE(sum == c->total_supply, "Sum of balances must equal total supply");
}

static void usdtgg_set_governance(USDTgG_Contract* c, uint64_t voting_period_s, uint64_t execution_delay_s, uint64_t min_stake) {
    // Apply reasonable bounds
    REQUIRE(voting_period_s >= 3600 && voting_period_s <= 30ULL*24*3600, "Voting period out of bounds (1h..30d)");
    REQUIRE(execution_delay_s <= 14ULL*24*3600, "Execution delay too large (>14d)");
    REQUIRE(min_stake > 0, "Minimum validator stake must be > 0");

    c->voting_period = voting_period_s;
    c->execution_delay = execution_delay_s;
    c->min_validator_stake = min_stake;
}

// Get contract info
void usdtgg_get_info(USDTgG_Contract* contract) {
    printf("USDTgG Contract Information:\n");
    printf("  Name: %s\n", contract->name);
    printf("  Symbol: %s\n", contract->symbol);
    printf("  Decimals: %" PRIu8 "\n", contract->decimals);
    printf("  Total Supply: %" PRIu64 "\n", contract->total_supply);
    printf("  Treasury Balance: %" PRIu64 "\n", contract->treasury_balance);
    printf("  Development Balance: %" PRIu64 "\n", contract->development_balance);
    printf("  Marketing Balance: %" PRIu64 "\n", contract->marketing_balance);
    printf("  Public Balance: %" PRIu64 "\n", contract->public_balance);
    printf("  Voting Period: %" PRIu64 " seconds\n", contract->voting_period);
    printf("  Execution Delay: %" PRIu64 " seconds\n", contract->execution_delay);
    printf("  Min Validator Stake: %" PRIu64 "\n", contract->min_validator_stake);
}

int main() {
    USDTgG_Contract contract;
    usdtgg_init(&contract);
    usdtgg_validate(&contract);
    usdtgg_get_info(&contract);
    return 0;
}