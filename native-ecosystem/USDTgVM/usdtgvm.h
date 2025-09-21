/**
 * 🔧 USDTgVerse Virtual Machine (USDTGVM)
 * ======================================
 * 
 * ⚛️ WORLD'S FIRST QUANTUM-SAFE VIRTUAL MACHINE
 * 🚀 HIGH-PERFORMANCE SMART CONTRACT EXECUTION
 * 🔐 POST-QUANTUM CRYPTOGRAPHIC SECURITY
 * 
 * The USDTgVM is a C-based virtual machine designed specifically for
 * quantum-safe smart contract execution with superior performance
 * compared to the Ethereum Virtual Machine (EVM).
 */

#ifndef USDTGVM_H
#define USDTGVM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// 📊 VERSION INFORMATION
#define USDTGVM_VERSION_MAJOR 1
#define USDTGVM_VERSION_MINOR 0
#define USDTGVM_VERSION_PATCH 0
#define USDTGVM_VERSION_STRING "1.0.0-quantum-safe"

// ⚛️ QUANTUM SECURITY CONSTANTS
#define USDTGVM_QUANTUM_SECURITY_LEVEL 256
#define USDTGVM_ADDRESS_LENGTH 51
#define USDTGVM_HASH_LENGTH 32
#define USDTGVM_SIGNATURE_LENGTH 128
#define USDTGVM_MAX_STACK_SIZE 1024
#define USDTGVM_MAX_MEMORY_SIZE (16 * 1024 * 1024) // 16MB
#define USDTGVM_MAX_CODE_SIZE (24 * 1024) // 24KB
#define USDTGVM_MAX_CALLDATA_SIZE (1024 * 1024) // 1MB

// 🔧 OPCODE DEFINITIONS
typedef enum {
    // 🧮 ARITHMETIC OPERATIONS
    USDTGVM_OP_ADD = 0x01,
    USDTGVM_OP_MUL = 0x02,
    USDTGVM_OP_SUB = 0x03,
    USDTGVM_OP_DIV = 0x04,
    USDTGVM_OP_SDIV = 0x05,
    USDTGVM_OP_MOD = 0x06,
    USDTGVM_OP_SMOD = 0x07,
    USDTGVM_OP_ADDMOD = 0x08,
    USDTGVM_OP_MULMOD = 0x09,
    USDTGVM_OP_EXP = 0x0A,
    USDTGVM_OP_SIGNEXTEND = 0x0B,
    
    // 🔢 COMPARISON & BITWISE
    USDTGVM_OP_LT = 0x10,
    USDTGVM_OP_GT = 0x11,
    USDTGVM_OP_SLT = 0x12,
    USDTGVM_OP_SGT = 0x13,
    USDTGVM_OP_EQ = 0x14,
    USDTGVM_OP_ISZERO = 0x15,
    USDTGVM_OP_AND = 0x16,
    USDTGVM_OP_OR = 0x17,
    USDTGVM_OP_XOR = 0x18,
    USDTGVM_OP_NOT = 0x19,
    USDTGVM_OP_BYTE = 0x1A,
    USDTGVM_OP_SHL = 0x1B,
    USDTGVM_OP_SHR = 0x1C,
    USDTGVM_OP_SAR = 0x1D,
    
    // 🔐 QUANTUM-SAFE CRYPTO
    USDTGVM_OP_QUANTUM_HASH = 0x20,
    USDTGVM_OP_QUANTUM_SIGN = 0x21,
    USDTGVM_OP_QUANTUM_VERIFY = 0x22,
    USDTGVM_OP_QUANTUM_ENCRYPT = 0x23,
    USDTGVM_OP_QUANTUM_DECRYPT = 0x24,
    USDTGVM_OP_QUANTUM_KEYGEN = 0x25,
    USDTGVM_OP_DILITHIUM_SIGN = 0x26,
    USDTGVM_OP_DILITHIUM_VERIFY = 0x27,
    USDTGVM_OP_KYBER_ENCRYPT = 0x28,
    USDTGVM_OP_KYBER_DECRYPT = 0x29,
    
    // 🌍 ENVIRONMENT
    USDTGVM_OP_ADDRESS = 0x30,
    USDTGVM_OP_BALANCE = 0x31,
    USDTGVM_OP_ORIGIN = 0x32,
    USDTGVM_OP_CALLER = 0x33,
    USDTGVM_OP_CALLVALUE = 0x34,
    USDTGVM_OP_CALLDATALOAD = 0x35,
    USDTGVM_OP_CALLDATASIZE = 0x36,
    USDTGVM_OP_CALLDATACOPY = 0x37,
    USDTGVM_OP_CODESIZE = 0x38,
    USDTGVM_OP_CODECOPY = 0x39,
    USDTGVM_OP_GASPRICE = 0x3A,
    USDTGVM_OP_EXTCODESIZE = 0x3B,
    USDTGVM_OP_EXTCODECOPY = 0x3C,
    
    // 🏗️ BLOCK INFORMATION
    USDTGVM_OP_BLOCKHASH = 0x40,
    USDTGVM_OP_COINBASE = 0x41,
    USDTGVM_OP_TIMESTAMP = 0x42,
    USDTGVM_OP_NUMBER = 0x43,
    USDTGVM_OP_DIFFICULTY = 0x44,
    USDTGVM_OP_GASLIMIT = 0x45,
    USDTGVM_OP_CHAINID = 0x46,
    USDTGVM_OP_SELFBALANCE = 0x47,
    
    // 📚 STACK OPERATIONS
    USDTGVM_OP_POP = 0x50,
    USDTGVM_OP_MLOAD = 0x51,
    USDTGVM_OP_MSTORE = 0x52,
    USDTGVM_OP_MSTORE8 = 0x53,
    USDTGVM_OP_SLOAD = 0x54,
    USDTGVM_OP_SSTORE = 0x55,
    USDTGVM_OP_JUMP = 0x56,
    USDTGVM_OP_JUMPI = 0x57,
    USDTGVM_OP_PC = 0x58,
    USDTGVM_OP_MSIZE = 0x59,
    USDTGVM_OP_GAS = 0x5A,
    USDTGVM_OP_JUMPDEST = 0x5B,
    
    // 📦 PUSH OPERATIONS (0x60-0x7F)
    USDTGVM_OP_PUSH1 = 0x60,
    USDTGVM_OP_PUSH32 = 0x7F,
    
    // 🔄 DUP OPERATIONS (0x80-0x8F)
    USDTGVM_OP_DUP1 = 0x80,
    USDTGVM_OP_DUP16 = 0x8F,
    
    // 🔁 SWAP OPERATIONS (0x90-0x9F)
    USDTGVM_OP_SWAP1 = 0x90,
    USDTGVM_OP_SWAP16 = 0x9F,
    
    // 📝 LOG OPERATIONS
    USDTGVM_OP_LOG0 = 0xA0,
    USDTGVM_OP_LOG1 = 0xA1,
    USDTGVM_OP_LOG2 = 0xA2,
    USDTGVM_OP_LOG3 = 0xA3,
    USDTGVM_OP_LOG4 = 0xA4,
    
    // 🌉 BRIDGE OPERATIONS
    USDTGVM_OP_BRIDGE_TO_ETH = 0xB0,
    USDTGVM_OP_BRIDGE_TO_BNB = 0xB1,
    USDTGVM_OP_BRIDGE_TO_TRON = 0xB2,
    USDTGVM_OP_BRIDGE_TO_SOL = 0xB3,
    USDTGVM_OP_BRIDGE_FROM_ETH = 0xB4,
    USDTGVM_OP_BRIDGE_FROM_BNB = 0xB5,
    USDTGVM_OP_BRIDGE_FROM_TRON = 0xB6,
    USDTGVM_OP_BRIDGE_FROM_SOL = 0xB7,
    
    // 🔧 SYSTEM OPERATIONS
    USDTGVM_OP_CREATE = 0xF0,
    USDTGVM_OP_CALL = 0xF1,
    USDTGVM_OP_CALLCODE = 0xF2,
    USDTGVM_OP_RETURN = 0xF3,
    USDTGVM_OP_DELEGATECALL = 0xF4,
    USDTGVM_OP_CREATE2 = 0xF5,
    USDTGVM_OP_STATICCALL = 0xFA,
    USDTGVM_OP_REVERT = 0xFD,
    USDTGVM_OP_INVALID = 0xFE,
    USDTGVM_OP_SELFDESTRUCT = 0xFF
} usdtgvm_opcode_t;

// 📊 EXECUTION RESULT
typedef enum {
    USDTGVM_RESULT_SUCCESS = 0,
    USDTGVM_RESULT_REVERT = 1,
    USDTGVM_RESULT_OUT_OF_GAS = 2,
    USDTGVM_RESULT_INVALID_OPCODE = 3,
    USDTGVM_RESULT_STACK_OVERFLOW = 4,
    USDTGVM_RESULT_STACK_UNDERFLOW = 5,
    USDTGVM_RESULT_MEMORY_OVERFLOW = 6,
    USDTGVM_RESULT_INVALID_JUMP = 7,
    USDTGVM_RESULT_QUANTUM_SECURITY_VIOLATION = 8,
    USDTGVM_RESULT_BRIDGE_ERROR = 9,
    USDTGVM_RESULT_UNKNOWN_ERROR = 10
} usdtgvm_result_t;

// 🏗️ CORE STRUCTURES

/**
 * 📊 256-bit Quantum-Safe Integer
 */
typedef struct {
    uint64_t limbs[4]; // 4 x 64-bit = 256-bit
} usdtgvm_uint256_t;

/**
 * 🏠 Quantum-Safe Address
 */
typedef struct {
    char address[USDTGVM_ADDRESS_LENGTH + 1]; // USDTg1q format + null terminator
    bool quantum_safe;
} usdtgvm_address_t;

/**
 * 📚 VM Stack
 */
typedef struct {
    usdtgvm_uint256_t items[USDTGVM_MAX_STACK_SIZE];
    size_t size;
    size_t capacity;
} usdtgvm_stack_t;

/**
 * 💾 VM Memory
 */
typedef struct {
    uint8_t* data;
    size_t size;
    size_t capacity;
} usdtgvm_memory_t;

/**
 * 🗄️ VM Storage
 */
typedef struct {
    usdtgvm_uint256_t key;
    usdtgvm_uint256_t value;
    struct usdtgvm_storage_entry* next;
} usdtgvm_storage_entry_t;

typedef struct {
    usdtgvm_storage_entry_t* entries;
    size_t size;
} usdtgvm_storage_t;

/**
 * 🌍 Execution Context
 */
typedef struct {
    usdtgvm_address_t address;        // Contract address
    usdtgvm_address_t origin;         // Transaction origin
    usdtgvm_address_t caller;         // Message caller
    usdtgvm_uint256_t value;          // Message value
    uint8_t* calldata;                // Call data
    size_t calldata_size;             // Call data size
    uint64_t gas_limit;               // Gas limit
    uint64_t gas_used;                // Gas used
    uint64_t gas_price;               // Gas price
    uint64_t block_number;            // Block number
    uint64_t timestamp;               // Block timestamp
    uint64_t chain_id;                // Chain ID (2024 for mainnet, 2025 for testnet)
    bool quantum_safe_mode;           // Quantum-safe execution mode
} usdtgvm_context_t;

/**
 * 🖥️ Virtual Machine Instance
 */
typedef struct {
    usdtgvm_stack_t stack;            // Execution stack
    usdtgvm_memory_t memory;          // Runtime memory
    usdtgvm_storage_t storage;        // Persistent storage
    usdtgvm_context_t context;        // Execution context
    uint8_t* code;                    // Contract bytecode
    size_t code_size;                 // Code size
    size_t pc;                        // Program counter
    bool running;                     // Execution state
    usdtgvm_result_t result;          // Execution result
    uint8_t* return_data;             // Return data
    size_t return_data_size;          // Return data size
} usdtgvm_t;

/**
 * 📊 Gas Costs
 */
typedef struct {
    uint64_t base;                    // Base execution cost
    uint64_t very_low;                // Very low cost operations
    uint64_t low;                     // Low cost operations
    uint64_t mid;                     // Mid cost operations
    uint64_t high;                    // High cost operations
    uint64_t ext_code;                // External code access
    uint64_t balance;                 // Balance access
    uint64_t sload;                   // Storage load
    uint64_t sstore;                  // Storage store
    uint64_t jump_dest;               // Jump destination
    uint64_t log;                     // Log operation
    uint64_t log_data;                // Log data per byte
    uint64_t log_topic;               // Log topic
    uint64_t create;                  // Contract creation
    uint64_t call;                    // External call
    uint64_t call_value;              // Call with value
    uint64_t call_stipend;            // Call stipend
    uint64_t new_account;             // New account creation
    uint64_t suicide;                 // Self-destruct
    uint64_t memory;                  // Memory expansion
    uint64_t tx_data_zero;            // Transaction data (zero bytes)
    uint64_t tx_data_non_zero;        // Transaction data (non-zero bytes)
    uint64_t tx;                      // Transaction base cost
    uint64_t quantum_crypto;          // Quantum cryptographic operations
} usdtgvm_gas_costs_t;

// 🚀 CORE API FUNCTIONS

/**
 * 🔧 Initialize USDTgVM instance
 */
usdtgvm_t* usdtgvm_create(void);

/**
 * 🧹 Destroy USDTgVM instance
 */
void usdtgvm_destroy(usdtgvm_t* vm);

/**
 * 📥 Load bytecode into VM
 */
bool usdtgvm_load_code(usdtgvm_t* vm, const uint8_t* code, size_t code_size);

/**
 * 🔧 Set execution context
 */
bool usdtgvm_set_context(usdtgvm_t* vm, const usdtgvm_context_t* context);

/**
 * ▶️ Execute loaded bytecode
 */
usdtgvm_result_t usdtgvm_execute(usdtgvm_t* vm);

/**
 * 📞 Execute function call
 */
usdtgvm_result_t usdtgvm_call(
    usdtgvm_t* vm,
    const usdtgvm_address_t* target,
    const uint8_t* calldata,
    size_t calldata_size,
    usdtgvm_uint256_t value,
    uint64_t gas_limit
);

/**
 * 🏗️ Create new contract
 */
usdtgvm_result_t usdtgvm_create_contract(
    usdtgvm_t* vm,
    const uint8_t* init_code,
    size_t init_code_size,
    usdtgvm_uint256_t value,
    uint64_t gas_limit,
    usdtgvm_address_t* new_address
);

// 📚 STACK OPERATIONS

bool usdtgvm_stack_push(usdtgvm_stack_t* stack, const usdtgvm_uint256_t* value);
bool usdtgvm_stack_pop(usdtgvm_stack_t* stack, usdtgvm_uint256_t* value);
bool usdtgvm_stack_peek(const usdtgvm_stack_t* stack, size_t index, usdtgvm_uint256_t* value);
bool usdtgvm_stack_dup(usdtgvm_stack_t* stack, size_t index);
bool usdtgvm_stack_swap(usdtgvm_stack_t* stack, size_t index);

// 💾 MEMORY OPERATIONS

bool usdtgvm_memory_expand(usdtgvm_memory_t* memory, size_t new_size);
bool usdtgvm_memory_store(usdtgvm_memory_t* memory, size_t offset, const uint8_t* data, size_t size);
bool usdtgvm_memory_load(const usdtgvm_memory_t* memory, size_t offset, uint8_t* data, size_t size);

// 🗄️ STORAGE OPERATIONS

bool usdtgvm_storage_store(usdtgvm_storage_t* storage, const usdtgvm_uint256_t* key, const usdtgvm_uint256_t* value);
bool usdtgvm_storage_load(const usdtgvm_storage_t* storage, const usdtgvm_uint256_t* key, usdtgvm_uint256_t* value);

// 🧮 ARITHMETIC OPERATIONS

void usdtgvm_uint256_add(const usdtgvm_uint256_t* a, const usdtgvm_uint256_t* b, usdtgvm_uint256_t* result);
void usdtgvm_uint256_sub(const usdtgvm_uint256_t* a, const usdtgvm_uint256_t* b, usdtgvm_uint256_t* result);
void usdtgvm_uint256_mul(const usdtgvm_uint256_t* a, const usdtgvm_uint256_t* b, usdtgvm_uint256_t* result);
void usdtgvm_uint256_div(const usdtgvm_uint256_t* a, const usdtgvm_uint256_t* b, usdtgvm_uint256_t* result);
int usdtgvm_uint256_cmp(const usdtgvm_uint256_t* a, const usdtgvm_uint256_t* b);

// ⚛️ QUANTUM-SAFE CRYPTOGRAPHY

bool usdtgvm_quantum_hash(const uint8_t* input, size_t input_size, uint8_t output[USDTGVM_HASH_LENGTH]);
bool usdtgvm_quantum_sign(const uint8_t* private_key, const uint8_t* message, size_t message_size, uint8_t signature[USDTGVM_SIGNATURE_LENGTH]);
bool usdtgvm_quantum_verify(const uint8_t* public_key, const uint8_t* message, size_t message_size, const uint8_t signature[USDTGVM_SIGNATURE_LENGTH]);
bool usdtgvm_dilithium_sign(const uint8_t* private_key, const uint8_t* message, size_t message_size, uint8_t* signature, size_t* signature_size);
bool usdtgvm_dilithium_verify(const uint8_t* public_key, const uint8_t* message, size_t message_size, const uint8_t* signature, size_t signature_size);

// 🌉 CROSS-CHAIN BRIDGE

bool usdtgvm_bridge_to_ethereum(usdtgvm_t* vm, const usdtgvm_uint256_t* amount, const char* eth_address);
bool usdtgvm_bridge_to_bnb(usdtgvm_t* vm, const usdtgvm_uint256_t* amount, const char* bnb_address);
bool usdtgvm_bridge_to_tron(usdtgvm_t* vm, const usdtgvm_uint256_t* amount, const char* tron_address);
bool usdtgvm_bridge_to_solana(usdtgvm_t* vm, const usdtgvm_uint256_t* amount, const char* sol_address);

// ⛽ GAS CALCULATION

uint64_t usdtgvm_calculate_gas_cost(usdtgvm_opcode_t opcode, const usdtgvm_gas_costs_t* gas_costs);
bool usdtgvm_consume_gas(usdtgvm_t* vm, uint64_t gas_amount);

// 🔧 UTILITY FUNCTIONS

const char* usdtgvm_result_to_string(usdtgvm_result_t result);
const char* usdtgvm_opcode_to_string(usdtgvm_opcode_t opcode);
bool usdtgvm_address_from_string(const char* address_str, usdtgvm_address_t* address);
bool usdtgvm_address_to_string(const usdtgvm_address_t* address, char* address_str, size_t buffer_size);

// 📊 DEBUGGING AND PROFILING

typedef struct {
    uint64_t total_gas_used;
    uint64_t total_instructions;
    uint64_t quantum_operations;
    uint64_t bridge_operations;
    uint64_t storage_reads;
    uint64_t storage_writes;
    double execution_time;
} usdtgvm_profile_t;

bool usdtgvm_enable_profiling(usdtgvm_t* vm);
bool usdtgvm_get_profile(const usdtgvm_t* vm, usdtgvm_profile_t* profile);

#ifdef __cplusplus
}
#endif

#endif // USDTGVM_H
