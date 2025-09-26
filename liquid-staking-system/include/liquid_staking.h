//
//  liquid_staking.h
//  USDTgVerse Liquid Staking Derivatives System
//
//  Created by USDTgVerse Development Team
//  Copyright © 2024 USDTgVerse. All rights reserved.
//

#ifndef LIQUID_STAKING_H
#define LIQUID_STAKING_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

// Forward declarations
typedef struct LiquidStakingSystem LiquidStakingSystem;
typedef struct StakingPool StakingPool;
typedef struct Validator Validator;
typedef struct StakingPosition StakingPosition;
typedef struct LiquidDerivative LiquidDerivative;
typedef struct SlashingProtection SlashingProtection;

// Staking pool types
typedef enum {
    POOL_PUBLIC = 0,        // Public staking pool
    POOL_PRIVATE,           // Private staking pool
    POOL_INSTITUTIONAL,     // Institutional staking pool
    POOL_DELEGATED,         // Delegated staking pool
    POOL_QUANTUM_SAFE       // Quantum-safe staking pool
} StakingPoolType;

// Validator status
typedef enum {
    VALIDATOR_ACTIVE = 0,   // Active validator
    VALIDATOR_INACTIVE,     // Inactive validator
    VALIDATOR_SLASHED,      // Slashed validator
    VALIDATOR_JAILED,       // Jailed validator
    VALIDATOR_UNBONDING     // Unbonding validator
} ValidatorStatus;

// Staking position types
typedef enum {
    POSITION_STAKED = 0,    // Staked position
    POSITION_UNSTAKING,     // Unstaking position
    POSITION_UNSTAKED,      // Unstaked position
    POSITION_SLASHED        // Slashed position
} StakingPositionType;

// Liquid derivative types
typedef enum {
    DERIVATIVE_STAKED = 0,  // stUSDTg (staked USDTg)
    DERIVATIVE_REWARD,      // rUSDTg (reward USDTg)
    DERIVATIVE_PENALTY,     // pUSDTg (penalty USDTg)
    DERIVATIVE_VALIDATOR    // vUSDTg (validator USDTg)
} LiquidDerivativeType;

// Slashing reasons
typedef enum {
    SLASH_DOUBLE_SIGN = 0,  // Double signing
    SLASH_DOWNTIME,         // Downtime
    SLASH_MALICIOUS,        // Malicious behavior
    SLASH_TECHNICAL,        // Technical failure
    SLASH_GOVERNANCE        // Governance violation
} SlashingReason;

// Validator structure
typedef struct {
    char validator_address[42];
    char operator_address[42];
    char moniker[64];
    ValidatorStatus status;
    uint64_t total_stake;
    uint64_t self_stake;
    uint64_t delegated_stake;
    uint64_t commission_rate;        // Basis points (10000 = 100%)
    uint64_t max_commission_rate;
    uint64_t max_commission_change;
    time_t commission_last_updated;
    uint64_t voting_power;
    uint64_t uptime_percentage;
    uint64_t total_rewards;
    uint64_t total_penalties;
    time_t created_at;
    time_t last_active;
    bool is_quantum_safe;
    char public_key[128];
    char consensus_pubkey[128];
    char description[256];
    char website[128];
    char security_contact[128];
} Validator;

// Staking pool structure
typedef struct {
    char pool_id[64];
    char pool_name[64];
    StakingPoolType pool_type;
    char operator_address[42];
    uint64_t total_stake;
    uint64_t total_delegated;
    uint64_t total_rewards;
    uint64_t total_fees;
    uint64_t min_stake_amount;
    uint64_t max_stake_amount;
    uint64_t commission_rate;
    uint64_t performance_fee;
    time_t created_at;
    time_t last_updated;
    bool is_active;
    bool is_quantum_safe;
    char description[256];
    char website[128];
    Validator** validators;
    size_t validator_count;
    size_t validator_capacity;
} StakingPool;

// Staking position structure
typedef struct {
    char position_id[64];
    char staker_address[42];
    char pool_id[64];
    char validator_address[42];
    StakingPositionType position_type;
    uint64_t staked_amount;
    uint64_t unstaking_amount;
    uint64_t rewards_earned;
    uint64_t penalties_incurred;
    uint64_t liquid_derivatives;
    time_t staked_at;
    time_t unstaking_started;
    time_t unstaking_completed;
    time_t last_claim;
    bool is_active;
    bool is_quantum_safe;
    char staking_signature[128];
    char unstaking_signature[128];
} StakingPosition;

// Liquid derivative structure
typedef struct {
    char derivative_id[64];
    char underlying_token[42];
    LiquidDerivativeType derivative_type;
    char staker_address[42];
    char pool_id[64];
    uint64_t underlying_amount;
    uint64_t derivative_amount;
    uint64_t exchange_rate;
    uint64_t total_supply;
    uint64_t total_underlying;
    time_t created_at;
    time_t last_updated;
    bool is_active;
    bool is_transferable;
    bool is_redeemable;
    char derivative_name[32];
    char derivative_symbol[16];
    uint8_t decimals;
} LiquidDerivative;

// Slashing protection structure
typedef struct {
    char protection_id[64];
    char validator_address[42];
    char staker_address[42];
    uint64_t protected_amount;
    uint64_t slashing_amount;
    SlashingReason slashing_reason;
    time_t protection_start;
    time_t protection_end;
    bool is_active;
    bool is_claimed;
    char protection_signature[128];
    char slashing_proof[256];
} SlashingProtection;

// Validator functions
Validator* validator_create(const char* operator_address, const char* moniker);
void validator_destroy(Validator* validator);
bool validator_set_commission_rate(Validator* validator, uint64_t commission_rate);
bool validator_update_status(Validator* validator, ValidatorStatus status);
bool validator_add_stake(Validator* validator, uint64_t amount);
bool validator_remove_stake(Validator* validator, uint64_t amount);
bool validator_add_rewards(Validator* validator, uint64_t amount);
bool validator_add_penalties(Validator* validator, uint64_t amount);
bool validator_update_uptime(Validator* validator, uint64_t uptime_percentage);
bool validator_set_quantum_safe(Validator* validator, bool is_quantum_safe);
char* validator_get_address(Validator* validator);
ValidatorStatus validator_get_status(Validator* validator);
uint64_t validator_get_total_stake(Validator* validator);
uint64_t validator_get_commission_rate(Validator* validator);
bool validator_is_active(Validator* validator);
bool validator_is_quantum_safe(Validator* validator);

// Staking pool functions
StakingPool* staking_pool_create(const char* pool_name, StakingPoolType pool_type, const char* operator_address);
void staking_pool_destroy(StakingPool* pool);
bool staking_pool_add_validator(StakingPool* pool, Validator* validator);
bool staking_pool_remove_validator(StakingPool* pool, const char* validator_address);
bool staking_pool_set_commission_rate(StakingPool* pool, uint64_t commission_rate);
bool staking_pool_set_performance_fee(StakingPool* pool, uint64_t performance_fee);
bool staking_pool_set_stake_limits(StakingPool* pool, uint64_t min_amount, uint64_t max_amount);
bool staking_pool_add_stake(StakingPool* pool, uint64_t amount);
bool staking_pool_remove_stake(StakingPool* pool, uint64_t amount);
bool staking_pool_distribute_rewards(StakingPool* pool, uint64_t total_rewards);
bool staking_pool_activate(StakingPool* pool);
bool staking_pool_deactivate(StakingPool* pool);
char* staking_pool_get_id(StakingPool* pool);
char* staking_pool_get_name(StakingPool* pool);
StakingPoolType staking_pool_get_type(StakingPool* pool);
uint64_t staking_pool_get_total_stake(StakingPool* pool);
uint64_t staking_pool_get_commission_rate(StakingPool* pool);
bool staking_pool_is_active(StakingPool* pool);
size_t staking_pool_get_validator_count(StakingPool* pool);
Validator** staking_pool_get_validators(StakingPool* pool, size_t* count);

// Staking position functions
StakingPosition* staking_position_create(const char* staker_address, const char* pool_id, const char* validator_address);
void staking_position_destroy(StakingPosition* position);
bool staking_position_stake(StakingPosition* position, uint64_t amount);
bool staking_position_unstake(StakingPosition* position, uint64_t amount);
bool staking_position_claim_rewards(StakingPosition* position);
bool staking_position_add_rewards(StakingPosition* position, uint64_t amount);
bool staking_position_add_penalties(StakingPosition* position, uint64_t amount);
bool staking_position_update_type(StakingPosition* position, StakingPositionType type);
bool staking_position_set_quantum_safe(StakingPosition* position, bool is_quantum_safe);
char* staking_position_get_id(StakingPosition* position);
char* staking_position_get_staker_address(StakingPosition* position);
char* staking_position_get_pool_id(StakingPosition* position);
StakingPositionType staking_position_get_type(StakingPosition* position);
uint64_t staking_position_get_staked_amount(StakingPosition* position);
uint64_t staking_position_get_rewards_earned(StakingPosition* position);
bool staking_position_is_active(StakingPosition* position);
bool staking_position_is_quantum_safe(StakingPosition* position);

// Liquid derivative functions
LiquidDerivative* liquid_derivative_create(const char* underlying_token, LiquidDerivativeType derivative_type, const char* staker_address);
void liquid_derivative_destroy(LiquidDerivative* derivative);
bool liquid_derivative_mint(LiquidDerivative* derivative, uint64_t underlying_amount);
bool liquid_derivative_burn(LiquidDerivative* derivative, uint64_t derivative_amount);
bool liquid_derivative_transfer(LiquidDerivative* derivative, const char* to_address, uint64_t amount);
bool liquid_derivative_redeem(LiquidDerivative* derivative, uint64_t derivative_amount);
bool liquid_derivative_update_exchange_rate(LiquidDerivative* derivative, uint64_t new_rate);
bool liquid_derivative_set_transferable(LiquidDerivative* derivative, bool is_transferable);
bool liquid_derivative_set_redeemable(LiquidDerivative* derivative, bool is_redeemable);
char* liquid_derivative_get_id(LiquidDerivative* derivative);
char* liquid_derivative_get_underlying_token(LiquidDerivative* derivative);
LiquidDerivativeType liquid_derivative_get_type(LiquidDerivative* derivative);
uint64_t liquid_derivative_get_underlying_amount(LiquidDerivative* derivative);
uint64_t liquid_derivative_get_derivative_amount(LiquidDerivative* derivative);
uint64_t liquid_derivative_get_exchange_rate(LiquidDerivative* derivative);
uint64_t liquid_derivative_get_total_supply(LiquidDerivative* derivative);
bool liquid_derivative_is_active(LiquidDerivative* derivative);
bool liquid_derivative_is_transferable(LiquidDerivative* derivative);
bool liquid_derivative_is_redeemable(LiquidDerivative* derivative);

// Slashing protection functions
SlashingProtection* slashing_protection_create(const char* validator_address, const char* staker_address, uint64_t protected_amount);
void slashing_protection_destroy(SlashingProtection* protection);
bool slashing_protection_activate(SlashingProtection* protection, time_t protection_duration);
bool slashing_protection_deactivate(SlashingProtection* protection);
bool slashing_protection_claim(SlashingProtection* protection);
bool slashing_protection_set_slashing_amount(SlashingProtection* protection, uint64_t slashing_amount, SlashingReason reason);
bool slashing_protection_set_slashing_proof(SlashingProtection* protection, const char* proof);
char* slashing_protection_get_id(SlashingProtection* protection);
char* slashing_protection_get_validator_address(SlashingProtection* protection);
char* slashing_protection_get_staker_address(SlashingProtection* protection);
uint64_t slashing_protection_get_protected_amount(SlashingProtection* protection);
uint64_t slashing_protection_get_slashing_amount(SlashingProtection* protection);
SlashingReason slashing_protection_get_slashing_reason(SlashingProtection* protection);
bool slashing_protection_is_active(SlashingProtection* protection);
bool slashing_protection_is_claimed(SlashingProtection* protection);
time_t slashing_protection_get_protection_end(SlashingProtection* protection);

// Liquid staking system functions
LiquidStakingSystem* liquid_staking_system_create(void);
void liquid_staking_system_destroy(LiquidStakingSystem* system);
bool liquid_staking_system_initialize(LiquidStakingSystem* system);
bool liquid_staking_system_register_validator(LiquidStakingSystem* system, const char* operator_address, const char* moniker);
bool liquid_staking_system_create_pool(LiquidStakingSystem* system, const char* pool_name, StakingPoolType pool_type, const char* operator_address);
bool liquid_staking_system_stake(LiquidStakingSystem* system, const char* staker_address, const char* pool_id, uint64_t amount);
bool liquid_staking_system_unstake(LiquidStakingSystem* system, const char* staker_address, const char* pool_id, uint64_t amount);
bool liquid_staking_system_claim_rewards(LiquidStakingSystem* system, const char* staker_address, const char* pool_id);
bool liquid_staking_system_mint_derivative(LiquidStakingSystem* system, const char* staker_address, const char* pool_id, uint64_t amount);
bool liquid_staking_system_burn_derivative(LiquidStakingSystem* system, const char* staker_address, const char* pool_id, uint64_t amount);
bool liquid_staking_system_transfer_derivative(LiquidStakingSystem* system, const char* from_address, const char* to_address, const char* pool_id, uint64_t amount);
bool liquid_staking_system_redeem_derivative(LiquidStakingSystem* system, const char* staker_address, const char* pool_id, uint64_t amount);
bool liquid_staking_system_slash_validator(LiquidStakingSystem* system, const char* validator_address, uint64_t slashing_amount, SlashingReason reason);
bool liquid_staking_system_activate_slashing_protection(LiquidStakingSystem* system, const char* validator_address, const char* staker_address, uint64_t protected_amount);
bool liquid_staking_system_claim_slashing_protection(LiquidStakingSystem* system, const char* protection_id);
Validator* liquid_staking_system_get_validator(LiquidStakingSystem* system, const char* validator_address);
StakingPool* liquid_staking_system_get_pool(LiquidStakingSystem* system, const char* pool_id);
StakingPosition* liquid_staking_system_get_position(LiquidStakingSystem* system, const char* staker_address, const char* pool_id);
LiquidDerivative* liquid_staking_system_get_derivative(LiquidStakingSystem* system, const char* staker_address, const char* pool_id);
SlashingProtection* liquid_staking_system_get_slashing_protection(LiquidStakingSystem* system, const char* protection_id);

// System management functions
bool liquid_staking_system_activate(LiquidStakingSystem* system);
bool liquid_staking_system_deactivate(LiquidStakingSystem* system);
bool liquid_staking_system_is_active(LiquidStakingSystem* system);
bool liquid_staking_system_update_validator_performance(LiquidStakingSystem* system);
bool liquid_staking_system_distribute_rewards(LiquidStakingSystem* system);
bool liquid_staking_system_process_slashing(LiquidStakingSystem* system);
bool liquid_staking_system_update_exchange_rates(LiquidStakingSystem* system);

// Configuration functions
void liquid_staking_system_set_min_stake_amount(LiquidStakingSystem* system, uint64_t min_amount);
void liquid_staking_system_set_max_stake_amount(LiquidStakingSystem* system, uint64_t max_amount);
void liquid_staking_system_set_unstaking_period(LiquidStakingSystem* system, time_t period);
void liquid_staking_system_set_slashing_percentage(LiquidStakingSystem* system, uint64_t percentage);
void liquid_staking_system_enable_quantum_safe_staking(LiquidStakingSystem* system, bool enable);
void liquid_staking_system_set_commission_rate(LiquidStakingSystem* system, uint64_t commission_rate);

// Statistics functions
size_t liquid_staking_system_get_total_validators(LiquidStakingSystem* system);
size_t liquid_staking_system_get_active_validators(LiquidStakingSystem* system);
size_t liquid_staking_system_get_total_pools(LiquidStakingSystem* system);
size_t liquid_staking_system_get_active_pools(LiquidStakingSystem* system);
size_t liquid_staking_system_get_total_positions(LiquidStakingSystem* system);
size_t liquid_staking_system_get_total_derivatives(LiquidStakingSystem* system);
uint64_t liquid_staking_system_get_total_staked(LiquidStakingSystem* system);
uint64_t liquid_staking_system_get_total_rewards(LiquidStakingSystem* system);
uint64_t liquid_staking_system_get_total_slashed(LiquidStakingSystem* system);
double liquid_staking_system_get_average_apy(LiquidStakingSystem* system);
char* liquid_staking_system_generate_report(LiquidStakingSystem* system);

// Utility functions
bool liquid_staking_validate_address(const char* address);
bool liquid_staking_validate_amount(uint64_t amount);
bool liquid_staking_validate_commission_rate(uint64_t commission_rate);
char* liquid_staking_generate_pool_id(const char* pool_name, const char* operator_address);
char* liquid_staking_generate_position_id(const char* staker_address, const char* pool_id);
char* liquid_staking_generate_derivative_id(const char* staker_address, const char* pool_id);
char* liquid_staking_generate_protection_id(const char* validator_address, const char* staker_address);
uint64_t liquid_staking_calculate_rewards(uint64_t staked_amount, uint64_t apy, time_t staking_duration);
uint64_t liquid_staking_calculate_slashing(uint64_t staked_amount, uint64_t slashing_percentage);
bool liquid_staking_is_validator_active(Validator* validator);
bool liquid_staking_is_pool_active(StakingPool* pool);
bool liquid_staking_is_position_active(StakingPosition* position);
bool liquid_staking_is_derivative_active(LiquidDerivative* derivative);

#endif // LIQUID_STAKING_H
