/*
==============================================
 File:        institutional_liquidity.c
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Institutional Liquidity Infrastructure
   
   Advanced institutional liquidity system featuring:
   - Large-scale trading infrastructure
   - Professional market making
   - Risk management systems
   - Compliance and reporting
   - High-frequency trading support

 License:
   MIT License
==============================================
*/

#include "institutional_liquidity.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

// Prime Brokerage Implementation
struct PrimeBrokerage {
    char institution_id[64];
    char institution_name[128];
    InstitutionType institution_type;
    char primary_address[42];
    char custody_address[42];
    uint64_t total_assets;
    uint64_t available_credit;
    uint64_t used_credit;
    uint64_t margin_requirement;
    uint64_t collateral_value;
    RiskLevel risk_level;
    time_t created_at;
    time_t last_updated;
    bool is_active;
    bool is_verified;
    char compliance_status[32];
    char regulatory_license[64];
    char contact_email[128];
    char contact_phone[32];
    pthread_mutex_t mutex;
};

PrimeBrokerage* prime_brokerage_create(const char* institution_name, InstitutionType institution_type, const char* primary_address) {
    if (!institution_name || !primary_address) return NULL;
    
    PrimeBrokerage* brokerage = malloc(sizeof(PrimeBrokerage));
    if (!brokerage) return NULL;
    
    strncpy(brokerage->institution_name, institution_name, sizeof(brokerage->institution_name) - 1);
    brokerage->institution_name[sizeof(brokerage->institution_name) - 1] = '\0';
    
    brokerage->institution_type = institution_type;
    strncpy(brokerage->primary_address, primary_address, sizeof(brokerage->primary_address) - 1);
    brokerage->primary_address[sizeof(brokerage->primary_address) - 1] = '\0';
    
    brokerage->total_assets = 0;
    brokerage->available_credit = 0;
    brokerage->used_credit = 0;
    brokerage->margin_requirement = 0;
    brokerage->collateral_value = 0;
    brokerage->risk_level = RISK_LOW;
    brokerage->created_at = time(NULL);
    brokerage->last_updated = time(NULL);
    brokerage->is_active = false;
    brokerage->is_verified = false;
    
    // Clear optional fields
    brokerage->custody_address[0] = '\0';
    brokerage->compliance_status[0] = '\0';
    brokerage->regulatory_license[0] = '\0';
    brokerage->contact_email[0] = '\0';
    brokerage->contact_phone[0] = '\0';
    
    // Generate institution ID
    char* institution_id = institutional_liquidity_generate_institution_id(institution_name, institution_type);
    if (institution_id) {
        strncpy(brokerage->institution_id, institution_id, sizeof(brokerage->institution_id) - 1);
        brokerage->institution_id[sizeof(brokerage->institution_id) - 1] = '\0';
        free(institution_id);
    } else {
        brokerage->institution_id[0] = '\0';
    }
    
    pthread_mutex_init(&brokerage->mutex, NULL);
    
    return brokerage;
}

void prime_brokerage_destroy(PrimeBrokerage* brokerage) {
    if (!brokerage) return;
    
    pthread_mutex_destroy(&brokerage->mutex);
    free(brokerage);
}

bool prime_brokerage_set_credit_limit(PrimeBrokerage* brokerage, uint64_t credit_limit) {
    if (!brokerage) return false;
    
    pthread_mutex_lock(&brokerage->mutex);
    
    brokerage->available_credit = credit_limit;
    brokerage->last_updated = time(NULL);
    
    pthread_mutex_unlock(&brokerage->mutex);
    return true;
}

bool prime_brokerage_add_collateral(PrimeBrokerage* brokerage, uint64_t collateral_amount) {
    if (!brokerage || collateral_amount == 0) return false;
    
    pthread_mutex_lock(&brokerage->mutex);
    
    brokerage->collateral_value += collateral_amount;
    brokerage->last_updated = time(NULL);
    
    pthread_mutex_unlock(&brokerage->mutex);
    return true;
}

bool prime_brokerage_verify_institution(PrimeBrokerage* brokerage) {
    if (!brokerage) return false;
    
    pthread_mutex_lock(&brokerage->mutex);
    
    brokerage->is_verified = true;
    strcpy(brokerage->compliance_status, "VERIFIED");
    brokerage->last_updated = time(NULL);
    
    pthread_mutex_unlock(&brokerage->mutex);
    return true;
}

bool prime_brokerage_is_active(PrimeBrokerage* brokerage) {
    if (!brokerage) return false;
    
    pthread_mutex_lock(&brokerage->mutex);
    bool active = brokerage->is_active;
    pthread_mutex_unlock(&brokerage->mutex);
    
    return active;
}

bool prime_brokerage_is_verified(PrimeBrokerage* brokerage) {
    if (!brokerage) return false;
    
    pthread_mutex_lock(&brokerage->mutex);
    bool verified = brokerage->is_verified;
    pthread_mutex_unlock(&brokerage->mutex);
    
    return verified;
}

// Institutional Custody Implementation
struct InstitutionalCustody {
    char custody_id[64];
    char institution_id[64];
    char asset_address[42];
    char asset_type[32];
    uint64_t total_balance;
    uint64_t available_balance;
    uint64_t locked_balance;
    uint64_t staked_balance;
    time_t created_at;
    time_t last_updated;
    bool is_active;
    bool is_insured;
    uint64_t insurance_coverage;
    char custody_provider[64];
    char security_level[32];
    char backup_location[128];
    pthread_mutex_t mutex;
};

InstitutionalCustody* institutional_custody_create(const char* institution_id, const char* asset_address, const char* asset_type) {
    if (!institution_id || !asset_address || !asset_type) return NULL;
    
    InstitutionalCustody* custody = malloc(sizeof(InstitutionalCustody));
    if (!custody) return NULL;
    
    strncpy(custody->institution_id, institution_id, sizeof(custody->institution_id) - 1);
    custody->institution_id[sizeof(custody->institution_id) - 1] = '\0';
    
    strncpy(custody->asset_address, asset_address, sizeof(custody->asset_address) - 1);
    custody->asset_address[sizeof(custody->asset_address) - 1] = '\0';
    
    strncpy(custody->asset_type, asset_type, sizeof(custody->asset_type) - 1);
    custody->asset_type[sizeof(custody->asset_type) - 1] = '\0';
    
    custody->total_balance = 0;
    custody->available_balance = 0;
    custody->locked_balance = 0;
    custody->staked_balance = 0;
    custody->created_at = time(NULL);
    custody->last_updated = time(NULL);
    custody->is_active = false;
    custody->is_insured = false;
    custody->insurance_coverage = 0;
    
    // Clear optional fields
    custody->custody_provider[0] = '\0';
    custody->security_level[0] = '\0';
    custody->backup_location[0] = '\0';
    
    // Generate custody ID
    char* custody_id = institutional_liquidity_generate_custody_id(institution_id, asset_address);
    if (custody_id) {
        strncpy(custody->custody_id, custody_id, sizeof(custody->custody_id) - 1);
        custody->custody_id[sizeof(custody->custody_id) - 1] = '\0';
        free(custody_id);
    } else {
        custody->custody_id[0] = '\0';
    }
    
    pthread_mutex_init(&custody->mutex, NULL);
    
    return custody;
}

void institutional_custody_destroy(InstitutionalCustody* custody) {
    if (!custody) return;
    
    pthread_mutex_destroy(&custody->mutex);
    free(custody);
}

bool institutional_custody_deposit(InstitutionalCustody* custody, uint64_t amount) {
    if (!custody || amount == 0) return false;
    
    pthread_mutex_lock(&custody->mutex);
    
    custody->total_balance += amount;
    custody->available_balance += amount;
    custody->last_updated = time(NULL);
    
    pthread_mutex_unlock(&custody->mutex);
    return true;
}

bool institutional_custody_withdraw(InstitutionalCustody* custody, uint64_t amount) {
    if (!custody || amount == 0 || amount > custody->available_balance) return false;
    
    pthread_mutex_lock(&custody->mutex);
    
    custody->total_balance -= amount;
    custody->available_balance -= amount;
    custody->last_updated = time(NULL);
    
    pthread_mutex_unlock(&custody->mutex);
    return true;
}

bool institutional_custody_is_active(InstitutionalCustody* custody) {
    if (!custody) return false;
    
    pthread_mutex_lock(&custody->mutex);
    bool active = custody->is_active;
    pthread_mutex_unlock(&custody->mutex);
    
    return active;
}

// Dark Pool Implementation
struct DarkPool {
    char pool_id[64];
    char pool_name[64];
    char operator_address[42];
    uint64_t total_liquidity;
    uint64_t available_liquidity;
    uint64_t min_trade_size;
    uint64_t max_trade_size;
    uint64_t total_volume;
    uint64_t total_trades;
    time_t created_at;
    time_t last_updated;
    bool is_active;
    bool is_private;
    char access_level[32];
    uint64_t participation_fee;
    uint64_t execution_fee;
    char supported_assets[256];
    pthread_mutex_t mutex;
};

DarkPool* dark_pool_create(const char* pool_name, const char* operator_address) {
    if (!pool_name || !operator_address) return NULL;
    
    DarkPool* pool = malloc(sizeof(DarkPool));
    if (!pool) return NULL;
    
    strncpy(pool->pool_name, pool_name, sizeof(pool->pool_name) - 1);
    pool->pool_name[sizeof(pool->pool_name) - 1] = '\0';
    
    strncpy(pool->operator_address, operator_address, sizeof(pool->operator_address) - 1);
    pool->operator_address[sizeof(pool->operator_address) - 1] = '\0';
    
    pool->total_liquidity = 0;
    pool->available_liquidity = 0;
    pool->min_trade_size = 1000000; // 1 USDTg minimum
    pool->max_trade_size = 1000000000000; // 1M USDTg maximum
    pool->total_volume = 0;
    pool->total_trades = 0;
    pool->created_at = time(NULL);
    pool->last_updated = time(NULL);
    pool->is_active = false;
    pool->is_private = true;
    pool->participation_fee = 100; // 0.01%
    pool->execution_fee = 50; // 0.005%
    
    // Clear optional fields
    strcpy(pool->access_level, "PRIVATE");
    pool->supported_assets[0] = '\0';
    
    // Generate pool ID
    char* pool_id = institutional_liquidity_generate_pool_id(pool_name, operator_address);
    if (pool_id) {
        strncpy(pool->pool_id, pool_id, sizeof(pool->pool_id) - 1);
        pool->pool_id[sizeof(pool->pool_id) - 1] = '\0';
        free(pool_id);
    } else {
        pool->pool_id[0] = '\0';
    }
    
    pthread_mutex_init(&pool->mutex, NULL);
    
    return pool;
}

void dark_pool_destroy(DarkPool* pool) {
    if (!pool) return;
    
    pthread_mutex_destroy(&pool->mutex);
    free(pool);
}

bool dark_pool_add_liquidity(DarkPool* pool, uint64_t amount) {
    if (!pool || amount == 0) return false;
    
    pthread_mutex_lock(&pool->mutex);
    
    pool->total_liquidity += amount;
    pool->available_liquidity += amount;
    pool->last_updated = time(NULL);
    
    pthread_mutex_unlock(&pool->mutex);
    return true;
}

bool dark_pool_is_active(DarkPool* pool) {
    if (!pool) return false;
    
    pthread_mutex_lock(&pool->mutex);
    bool active = pool->is_active;
    pthread_mutex_unlock(&pool->mutex);
    
    return active;
}

// Algorithmic Trading Implementation
struct AlgorithmicTrading {
    char algo_id[64];
    char institution_id[64];
    char algo_name[64];
    char algo_type[32];
    uint64_t total_volume;
    uint64_t total_trades;
    uint64_t total_pnl;
    double success_rate;
    double average_slippage;
    time_t created_at;
    time_t last_updated;
    bool is_active;
    bool is_approved;
    char risk_parameters[256];
    char performance_metrics[256];
    char market_conditions[128];
    pthread_mutex_t mutex;
};

AlgorithmicTrading* algorithmic_trading_create(const char* institution_id, const char* algo_name, const char* algo_type) {
    if (!institution_id || !algo_name || !algo_type) return NULL;
    
    AlgorithmicTrading* algo = malloc(sizeof(AlgorithmicTrading));
    if (!algo) return NULL;
    
    strncpy(algo->institution_id, institution_id, sizeof(algo->institution_id) - 1);
    algo->institution_id[sizeof(algo->institution_id) - 1] = '\0';
    
    strncpy(algo->algo_name, algo_name, sizeof(algo->algo_name) - 1);
    algo->algo_name[sizeof(algo->algo_name) - 1] = '\0';
    
    strncpy(algo->algo_type, algo_type, sizeof(algo->algo_type) - 1);
    algo->algo_type[sizeof(algo->algo_type) - 1] = '\0';
    
    algo->total_volume = 0;
    algo->total_trades = 0;
    algo->total_pnl = 0;
    algo->success_rate = 0.0;
    algo->average_slippage = 0.0;
    algo->created_at = time(NULL);
    algo->last_updated = time(NULL);
    algo->is_active = false;
    algo->is_approved = false;
    
    // Clear optional fields
    algo->risk_parameters[0] = '\0';
    algo->performance_metrics[0] = '\0';
    algo->market_conditions[0] = '\0';
    
    // Generate algo ID
    char* algo_id = institutional_liquidity_generate_algo_id(institution_id, algo_name);
    if (algo_id) {
        strncpy(algo->algo_id, algo_id, sizeof(algo->algo_id) - 1);
        algo->algo_id[sizeof(algo->algo_id) - 1] = '\0';
        free(algo_id);
    } else {
        algo->algo_id[0] = '\0';
    }
    
    pthread_mutex_init(&algo->mutex, NULL);
    
    return algo;
}

void algorithmic_trading_destroy(AlgorithmicTrading* algo) {
    if (!algo) return;
    
    pthread_mutex_destroy(&algo->mutex);
    free(algo);
}

bool algorithmic_trading_is_active(AlgorithmicTrading* algo) {
    if (!algo) return false;
    
    pthread_mutex_lock(&algo->mutex);
    bool active = algo->is_active;
    pthread_mutex_unlock(&algo->mutex);
    
    return active;
}

// Margin Trading Implementation
struct MarginTrading {
    char margin_id[64];
    char institution_id[64];
    char collateral_asset[42];
    char borrowed_asset[42];
    uint64_t collateral_amount;
    uint64_t borrowed_amount;
    uint64_t margin_ratio;
    uint64_t liquidation_threshold;
    uint64_t interest_rate;
    time_t created_at;
    time_t last_updated;
    bool is_active;
    bool is_liquidated;
    char margin_type[32];
    uint64_t maintenance_margin;
    uint64_t initial_margin;
    pthread_mutex_t mutex;
};

MarginTrading* margin_trading_create(const char* institution_id, const char* collateral_asset, const char* borrowed_asset) {
    if (!institution_id || !collateral_asset || !borrowed_asset) return NULL;
    
    MarginTrading* margin = malloc(sizeof(MarginTrading));
    if (!margin) return NULL;
    
    strncpy(margin->institution_id, institution_id, sizeof(margin->institution_id) - 1);
    margin->institution_id[sizeof(margin->institution_id) - 1] = '\0';
    
    strncpy(margin->collateral_asset, collateral_asset, sizeof(margin->collateral_asset) - 1);
    margin->collateral_asset[sizeof(margin->collateral_asset) - 1] = '\0';
    
    strncpy(margin->borrowed_asset, borrowed_asset, sizeof(margin->borrowed_asset) - 1);
    margin->borrowed_asset[sizeof(margin->borrowed_asset) - 1] = '\0';
    
    margin->collateral_amount = 0;
    margin->borrowed_amount = 0;
    margin->margin_ratio = 0;
    margin->liquidation_threshold = 8000; // 80% default
    margin->interest_rate = 500; // 5% default
    margin->created_at = time(NULL);
    margin->last_updated = time(NULL);
    margin->is_active = false;
    margin->is_liquidated = false;
    margin->maintenance_margin = 10000; // 100% default
    margin->initial_margin = 15000; // 150% default
    
    // Clear optional fields
    strcpy(margin->margin_type, "CROSS");
    
    // Generate margin ID
    char* margin_id = institutional_liquidity_generate_margin_id(institution_id, collateral_asset);
    if (margin_id) {
        strncpy(margin->margin_id, margin_id, sizeof(margin->margin_id) - 1);
        margin->margin_id[sizeof(margin->margin_id) - 1] = '\0';
        free(margin_id);
    } else {
        margin->margin_id[0] = '\0';
    }
    
    pthread_mutex_init(&margin->mutex, NULL);
    
    return margin;
}

void margin_trading_destroy(MarginTrading* margin) {
    if (!margin) return;
    
    pthread_mutex_destroy(&margin->mutex);
    free(margin);
}

bool margin_trading_is_active(MarginTrading* margin) {
    if (!margin) return false;
    
    pthread_mutex_lock(&margin->mutex);
    bool active = margin->is_active;
    pthread_mutex_unlock(&margin->mutex);
    
    return active;
}

// Institutional Liquidity System Implementation
struct InstitutionalLiquiditySystem {
    PrimeBrokerage** brokerages;
    size_t brokerage_count;
    size_t brokerage_capacity;
    InstitutionalCustody** custodies;
    size_t custody_count;
    size_t custody_capacity;
    DarkPool** dark_pools;
    size_t dark_pool_count;
    size_t dark_pool_capacity;
    AlgorithmicTrading** algorithmic_tradings;
    size_t algorithmic_count;
    size_t algorithmic_capacity;
    MarginTrading** margin_tradings;
    size_t margin_count;
    size_t margin_capacity;
    DerivativesMarket** derivatives;
    size_t derivative_count;
    size_t derivative_capacity;
    InsuranceMarketplace** insurances;
    size_t insurance_count;
    size_t insurance_capacity;
    
    uint64_t min_trade_size;
    uint64_t max_trade_size;
    uint64_t default_margin_requirement;
    uint64_t default_interest_rate;
    bool quantum_safe_trading_enabled;
    RiskLevel risk_management_level;
    
    size_t total_institutions;
    size_t active_institutions;
    size_t total_dark_pools;
    size_t total_algorithmic_strategies;
    size_t total_margin_positions;
    size_t total_derivatives;
    size_t total_insurance_policies;
    uint64_t total_liquidity;
    uint64_t total_volume;
    uint64_t total_assets_under_custody;
    
    bool is_active;
    pthread_mutex_t mutex;
};

InstitutionalLiquiditySystem* institutional_liquidity_system_create(void) {
    InstitutionalLiquiditySystem* system = malloc(sizeof(InstitutionalLiquiditySystem));
    if (!system) return NULL;
    
    system->brokerages = NULL;
    system->brokerage_count = 0;
    system->brokerage_capacity = 0;
    system->custodies = NULL;
    system->custody_count = 0;
    system->custody_capacity = 0;
    system->dark_pools = NULL;
    system->dark_pool_count = 0;
    system->dark_pool_capacity = 0;
    system->algorithmic_tradings = NULL;
    system->algorithmic_count = 0;
    system->algorithmic_capacity = 0;
    system->margin_tradings = NULL;
    system->margin_count = 0;
    system->margin_capacity = 0;
    system->derivatives = NULL;
    system->derivative_count = 0;
    system->derivative_capacity = 0;
    system->insurances = NULL;
    system->insurance_count = 0;
    system->insurance_capacity = 0;
    
    system->min_trade_size = 1000000; // 1 USDTg
    system->max_trade_size = 1000000000000; // 1M USDTg
    system->default_margin_requirement = 15000; // 150%
    system->default_interest_rate = 500; // 5%
    system->quantum_safe_trading_enabled = true;
    system->risk_management_level = RISK_MEDIUM;
    
    system->total_institutions = 0;
    system->active_institutions = 0;
    system->total_dark_pools = 0;
    system->total_algorithmic_strategies = 0;
    system->total_margin_positions = 0;
    system->total_derivatives = 0;
    system->total_insurance_policies = 0;
    system->total_liquidity = 0;
    system->total_volume = 0;
    system->total_assets_under_custody = 0;
    
    system->is_active = false;
    
    pthread_mutex_init(&system->mutex, NULL);
    
    return system;
}

void institutional_liquidity_system_destroy(InstitutionalLiquiditySystem* system) {
    if (!system) return;
    
    pthread_mutex_lock(&system->mutex);
    
    // Destroy brokerages
    if (system->brokerages) {
        for (size_t i = 0; i < system->brokerage_count; i++) {
            prime_brokerage_destroy(system->brokerages[i]);
        }
        free(system->brokerages);
    }
    
    // Destroy custodies
    if (system->custodies) {
        for (size_t i = 0; i < system->custody_count; i++) {
            institutional_custody_destroy(system->custodies[i]);
        }
        free(system->custodies);
    }
    
    // Destroy dark pools
    if (system->dark_pools) {
        for (size_t i = 0; i < system->dark_pool_count; i++) {
            dark_pool_destroy(system->dark_pools[i]);
        }
        free(system->dark_pools);
    }
    
    // Destroy algorithmic tradings
    if (system->algorithmic_tradings) {
        for (size_t i = 0; i < system->algorithmic_count; i++) {
            algorithmic_trading_destroy(system->algorithmic_tradings[i]);
        }
        free(system->algorithmic_tradings);
    }
    
    // Destroy margin tradings
    if (system->margin_tradings) {
        for (size_t i = 0; i < system->margin_count; i++) {
            margin_trading_destroy(system->margin_tradings[i]);
        }
        free(system->margin_tradings);
    }
    
    pthread_mutex_unlock(&system->mutex);
    pthread_mutex_destroy(&system->mutex);
    free(system);
}

bool institutional_liquidity_system_activate(InstitutionalLiquiditySystem* system) {
    if (!system) return false;
    
    pthread_mutex_lock(&system->mutex);
    system->is_active = true;
    pthread_mutex_unlock(&system->mutex);
    
    return true;
}

bool institutional_liquidity_system_is_active(InstitutionalLiquiditySystem* system) {
    if (!system) return false;
    
    pthread_mutex_lock(&system->mutex);
    bool active = system->is_active;
    pthread_mutex_unlock(&system->mutex);
    
    return active;
}

// Utility functions
bool institutional_liquidity_validate_address(const char* address) {
    if (!address) return false;
    return strlen(address) == 42 && address[0] == '0' && address[1] == 'x';
}

bool institutional_liquidity_validate_amount(uint64_t amount) {
    return amount > 0;
}

char* institutional_liquidity_generate_institution_id(const char* institution_name, InstitutionType institution_type) {
    if (!institution_name) return NULL;
    
    char* id = malloc(64);
    if (!id) return NULL;
    
    snprintf(id, 64, "inst_%s_%d_%lx", institution_name, (int)institution_type, (unsigned long)time(NULL));
    return id;
}

char* institutional_liquidity_generate_custody_id(const char* institution_id, const char* asset_address) {
    if (!institution_id || !asset_address) return NULL;
    
    char* id = malloc(64);
    if (!id) return NULL;
    
    snprintf(id, 64, "cust_%s_%s_%lx", institution_id, asset_address, (unsigned long)time(NULL));
    return id;
}

char* institutional_liquidity_generate_pool_id(const char* pool_name, const char* operator_address) {
    if (!pool_name || !operator_address) return NULL;
    
    char* id = malloc(64);
    if (!id) return NULL;
    
    snprintf(id, 64, "pool_%s_%s_%lx", pool_name, operator_address, (unsigned long)time(NULL));
    return id;
}

char* institutional_liquidity_generate_algo_id(const char* institution_id, const char* algo_name) {
    if (!institution_id || !algo_name) return NULL;
    
    char* id = malloc(64);
    if (!id) return NULL;
    
    snprintf(id, 64, "algo_%s_%s_%lx", institution_id, algo_name, (unsigned long)time(NULL));
    return id;
}

char* institutional_liquidity_generate_margin_id(const char* institution_id, const char* collateral_asset) {
    if (!institution_id || !collateral_asset) return NULL;
    
    char* id = malloc(64);
    if (!id) return NULL;
    
    snprintf(id, 64, "margin_%s_%s_%lx", institution_id, collateral_asset, (unsigned long)time(NULL));
    return id;
}

uint64_t institutional_liquidity_calculate_margin_ratio(uint64_t collateral, uint64_t borrowed) {
    if (borrowed == 0) return 0;
    return (collateral * 10000) / borrowed; // Return as basis points
}

bool institutional_liquidity_is_institution_verified(PrimeBrokerage* brokerage) {
    return brokerage && brokerage->is_verified;
}

bool institutional_liquidity_is_custody_active(InstitutionalCustody* custody) {
    return custody && custody->is_active;
}

bool institutional_liquidity_is_dark_pool_active(DarkPool* pool) {
    return pool && pool->is_active;
}

bool institutional_liquidity_is_algorithmic_trading_active(AlgorithmicTrading* algo) {
    return algo && algo->is_active;
}

bool institutional_liquidity_is_margin_trading_active(MarginTrading* margin) {
    return margin && margin->is_active;
}
