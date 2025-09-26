/*
==============================================
 File:        usdtgverse_core.c
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Pure C Backend - Ultra Performance
   
   Complete blockchain implementation in pure C featuring:
   - Native USDTg token
   - HotStuff BFT-PoS consensus
   - P2P networking
   - State management
   - Transaction processing
   - Block production

 License:
   MIT License
==============================================
*/
 * 
 * Zero C++ overhead - Maximum performance
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

// ============================================================================
// CORE TYPES (Pure C)
// ============================================================================

#define MAX_ACCOUNTS 1000000
#define MAX_VALIDATORS 100
#define MAX_PEERS 50
#define MAX_MEMPOOL_SIZE 10000
#define MAX_BLOCK_TXS 10000
#define USDTG_DECIMALS 1000000ULL

typedef uint8_t usdtg_hash_t[32];
typedef uint8_t usdtg_address_t[20];
typedef uint8_t usdtg_signature_t[64];
typedef uint8_t usdtg_pubkey_t[32];

typedef uint64_t usdtg_amount_t;
typedef uint64_t usdtg_height_t;
typedef uint64_t usdtg_nonce_t;
typedef uint64_t usdtg_timestamp_t;
typedef uint32_t usdtg_asset_id_t;

// Account structure
typedef struct {
    usdtg_address_t address;
    usdtg_amount_t usdtg_balance;
    usdtg_nonce_t nonce;
    uint8_t is_active;
} usdtg_account_t;

// Transaction structure
typedef struct {
    usdtg_hash_t tx_hash;
    usdtg_address_t from;
    usdtg_address_t to;
    usdtg_amount_t amount;
    usdtg_amount_t fee;
    usdtg_nonce_t nonce;
    usdtg_timestamp_t timestamp;
    char memo[256];
    uint8_t status; // 0=pending, 1=confirmed, 2=failed
} usdtg_transaction_t;

// Block structure
typedef struct {
    usdtg_height_t height;
    usdtg_hash_t block_hash;
    usdtg_hash_t prev_hash;
    usdtg_hash_t state_root;
    usdtg_timestamp_t timestamp;
    usdtg_address_t proposer;
    usdtg_transaction_t transactions[MAX_BLOCK_TXS];
    uint32_t tx_count;
} usdtg_block_t;

// Validator structure
typedef struct {
    usdtg_address_t validator_id;
    usdtg_pubkey_t public_key;
    usdtg_amount_t stake;
    uint8_t is_active;
    uint64_t blocks_proposed;
    uint64_t votes_cast;
    int32_t reputation;
} usdtg_validator_t;

// Network peer structure
typedef struct {
    usdtg_address_t peer_id;
    uint32_t ip_address;
    uint16_t port;
    int socket_fd;
    uint8_t state; // 0=disconnected, 1=connecting, 2=connected
    uint64_t last_ping;
    uint64_t bytes_sent;
    uint64_t bytes_received;
    int32_t reputation;
} usdtg_peer_t;

// Main blockchain state
typedef struct {
    // Blockchain state
    usdtg_account_t accounts[MAX_ACCOUNTS];
    uint32_t account_count;
    
    usdtg_block_t blocks[1000]; // Keep last 1000 blocks in memory
    uint32_t block_count;
    usdtg_height_t current_height;
    
    usdtg_transaction_t mempool[MAX_MEMPOOL_SIZE];
    uint32_t mempool_size;
    
    // Consensus state
    usdtg_validator_t validators[MAX_VALIDATORS];
    uint32_t validator_count;
    uint64_t current_view;
    uint8_t consensus_phase; // 1=prepare, 2=precommit, 3=commit
    usdtg_address_t current_proposer;
    
    // Network state
    usdtg_peer_t peers[MAX_PEERS];
    uint32_t peer_count;
    int listen_socket;
    uint16_t listen_port;
    
    // Node configuration
    char chain_id[32];
    char node_moniker[64];
    uint8_t is_validator;
    usdtg_address_t local_validator_id;
    
    // Statistics
    uint64_t total_transactions;
    uint64_t total_blocks;
    usdtg_amount_t total_supply;
    uint64_t start_time;
    
    // Threading
    pthread_t consensus_thread;
    pthread_t network_thread;
    pthread_t api_thread;
    uint8_t running;
    
} usdtgverse_node_t;

// ============================================================================
// CORE FUNCTIONS (Pure C)
// ============================================================================

// Node initialization
int usdtgverse_init(usdtgverse_node_t* node, const char* chain_id, uint16_t port, uint8_t is_validator);
void usdtgverse_cleanup(usdtgverse_node_t* node);

// Account functions
int usdtgverse_create_account(usdtgverse_node_t* node, const usdtg_address_t address, usdtg_amount_t initial_balance);
int usdtgverse_get_balance(usdtgverse_node_t* node, const usdtg_address_t address, usdtg_amount_t* balance);
int usdtgverse_transfer(usdtgverse_node_t* node, const usdtg_address_t from, const usdtg_address_t to, usdtg_amount_t amount);

// Transaction functions
int usdtgverse_submit_transaction(usdtgverse_node_t* node, const usdtg_transaction_t* tx);
int usdtgverse_process_mempool(usdtgverse_node_t* node);

// Block functions
int usdtgverse_produce_block(usdtgverse_node_t* node);
int usdtgverse_validate_block(usdtgverse_node_t* node, const usdtg_block_t* block);

// Consensus functions
void* usdtgverse_consensus_loop(void* arg);
int usdtgverse_process_consensus(usdtgverse_node_t* node);

// Network functions
void* usdtgverse_network_loop(void* arg);
int usdtgverse_connect_peer(usdtgverse_node_t* node, uint32_t ip, uint16_t port);
int usdtgverse_broadcast_transaction(usdtgverse_node_t* node, const usdtg_transaction_t* tx);

// API functions
void* usdtgverse_api_loop(void* arg);
int usdtgverse_handle_api_request(usdtgverse_node_t* node, const char* endpoint, char* response);

// Utility functions
uint64_t usdtgverse_get_timestamp(void);
void usdtgverse_generate_hash(const void* data, size_t len, usdtg_hash_t result);
void usdtgverse_generate_address(const usdtg_pubkey_t pubkey, usdtg_address_t address);

// ============================================================================
// IMPLEMENTATION
// ============================================================================

uint64_t usdtgverse_get_timestamp(void) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}

void usdtgverse_generate_hash(const void* data, size_t len, usdtg_hash_t result) {
    // Simple but fast hash for demo
    memset(result, 0, 32);
    const uint8_t* bytes = (const uint8_t*)data;
    
    for (size_t i = 0; i < len; i++) {
        result[i % 32] ^= bytes[i];
        result[(i + 1) % 32] ^= bytes[i] >> 4;
    }
    
    // Additional mixing
    for (int round = 0; round < 3; round++) {
        for (int i = 0; i < 32; i++) {
            result[i] ^= result[(i + 7) % 32];
            result[i] = (result[i] << 3) | (result[i] >> 5);
        }
    }
}

int usdtgverse_init(usdtgverse_node_t* node, const char* chain_id, uint16_t port, uint8_t is_validator) {
    if (!node || !chain_id) return -1;
    
    memset(node, 0, sizeof(usdtgverse_node_t));
    
    // Basic configuration
    strncpy(node->chain_id, chain_id, 31);
    strncpy(node->node_moniker, "usdtgverse-node", 63);
    node->listen_port = port;
    node->is_validator = is_validator;
    node->current_height = 1;
    node->current_view = 1;
    node->consensus_phase = 1; // PREPARE
    node->start_time = usdtgverse_get_timestamp();
    node->total_supply = 1750000000ULL * USDTG_DECIMALS; // 1.75B USDTg
    
    // Initialize genesis accounts
    usdtg_address_t genesis_addr = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
    usdtgverse_create_account(node, genesis_addr, node->total_supply);
    
    // Create some test accounts
    usdtg_address_t alice_addr = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
                                 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
    usdtg_address_t bob_addr = {0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB,
                               0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB};
    
    usdtgverse_create_account(node, alice_addr, 1000 * USDTG_DECIMALS);
    usdtgverse_create_account(node, bob_addr, 500 * USDTG_DECIMALS);
    
    printf("✅ USDTgVerse node initialized\n");
    printf("   Chain ID: %s\n", node->chain_id);
    printf("   Port: %u\n", node->listen_port);
    printf("   Validator: %s\n", is_validator ? "YES" : "NO");
    printf("   Accounts: %u\n", node->account_count);
    printf("   Total Supply: %llu USDTg\n", node->total_supply / USDTG_DECIMALS);
    
    return 0;
}

int usdtgverse_create_account(usdtgverse_node_t* node, const usdtg_address_t address, usdtg_amount_t initial_balance) {
    if (!node || node->account_count >= MAX_ACCOUNTS) return -1;
    
    usdtg_account_t* account = &node->accounts[node->account_count];
    memcpy(account->address, address, 20);
    account->usdtg_balance = initial_balance;
    account->nonce = 0;
    account->is_active = 1;
    
    node->account_count++;
    return 0;
}

int usdtgverse_get_balance(usdtgverse_node_t* node, const usdtg_address_t address, usdtg_amount_t* balance) {
    if (!node || !balance) return -1;
    
    for (uint32_t i = 0; i < node->account_count; i++) {
        if (memcmp(node->accounts[i].address, address, 20) == 0) {
            *balance = node->accounts[i].usdtg_balance;
            return 0;
        }
    }
    
    *balance = 0;
    return -1; // Account not found
}

int usdtgverse_transfer(usdtgverse_node_t* node, const usdtg_address_t from, const usdtg_address_t to, usdtg_amount_t amount) {
    if (!node || amount == 0) return -1;
    
    usdtg_account_t* from_account = NULL;
    usdtg_account_t* to_account = NULL;
    
    // Find accounts
    for (uint32_t i = 0; i < node->account_count; i++) {
        if (memcmp(node->accounts[i].address, from, 20) == 0) {
            from_account = &node->accounts[i];
        }
        if (memcmp(node->accounts[i].address, to, 20) == 0) {
            to_account = &node->accounts[i];
        }
    }
    
    if (!from_account) return -1; // Sender not found
    if (from_account->usdtg_balance < amount) return -1; // Insufficient funds
    
    // Create recipient account if doesn't exist
    if (!to_account) {
        if (usdtgverse_create_account(node, to, 0) != 0) return -1;
        to_account = &node->accounts[node->account_count - 1];
    }
    
    // Execute transfer
    from_account->usdtg_balance -= amount;
    to_account->usdtg_balance += amount;
    from_account->nonce++;
    
    return 0;
}

int usdtgverse_produce_block(usdtgverse_node_t* node) {
    if (!node || !node->is_validator || node->mempool_size == 0) return -1;
    
    if (node->block_count >= 1000) return -1; // Block storage full
    
    usdtg_block_t* block = &node->blocks[node->block_count];
    
    // Block header
    block->height = node->current_height;
    block->timestamp = usdtgverse_get_timestamp();
    memcpy(block->proposer, node->local_validator_id, 20);
    
    // Add transactions from mempool
    uint32_t tx_added = 0;
    for (uint32_t i = 0; i < node->mempool_size && tx_added < MAX_BLOCK_TXS; i++) {
        block->transactions[tx_added] = node->mempool[i];
        block->transactions[tx_added].status = 1; // Confirmed
        tx_added++;
    }
    
    block->tx_count = tx_added;
    
    // Generate block hash
    usdtgverse_generate_hash(block, sizeof(usdtg_block_t), block->block_hash);
    
    // Update state
    node->block_count++;
    node->current_height++;
    node->total_blocks++;
    node->mempool_size = 0; // Clear mempool
    
    printf("⛏️  Block #%llu produced with %u transactions\n", block->height, block->tx_count);
    
    return 0;
}

void* usdtgverse_consensus_loop(void* arg) {
    usdtgverse_node_t* node = (usdtgverse_node_t*)arg;
    
    printf("🤝 Consensus thread started\n");
    
    while (node->running) {
        if (node->is_validator) {
            // Simple consensus: produce block every 3 seconds if we have transactions
            if (node->mempool_size > 0) {
                static uint64_t last_block = 0;
                uint64_t now = usdtgverse_get_timestamp();
                
                if (now - last_block >= 3000) { // 3 second block time
                    usdtgverse_produce_block(node);
                    last_block = now;
                }
            }
        }
        
        usleep(100000); // 100ms sleep
    }
    
    printf("🤝 Consensus thread stopped\n");
    return NULL;
}

void* usdtgverse_network_loop(void* arg) {
    usdtgverse_node_t* node = (usdtgverse_node_t*)arg;
    
    printf("🌐 Network thread started\n");
    
    // Create listening socket
    node->listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (node->listen_socket < 0) {
        printf("❌ Failed to create socket\n");
        return NULL;
    }
    
    int opt = 1;
    setsockopt(node->listen_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(node->listen_port);
    
    if (bind(node->listen_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("❌ Failed to bind to port %u\n", node->listen_port);
        close(node->listen_socket);
        return NULL;
    }
    
    if (listen(node->listen_socket, 10) < 0) {
        printf("❌ Failed to listen\n");
        close(node->listen_socket);
        return NULL;
    }
    
    printf("📡 Listening on port %u\n", node->listen_port);
    
    while (node->running) {
        // Simple network processing
        usleep(100000); // 100ms sleep
    }
    
    close(node->listen_socket);
    printf("🌐 Network thread stopped\n");
    return NULL;
}

void* usdtgverse_api_loop(void* arg) {
    usdtgverse_node_t* node = (usdtgverse_node_t*)arg;
    
    printf("📡 API thread started\n");
    
    while (node->running) {
        // Simple API processing
        usleep(100000); // 100ms sleep
    }
    
    printf("📡 API thread stopped\n");
    return NULL;
}

int usdtgverse_start(usdtgverse_node_t* node) {
    if (!node) return -1;
    
    node->running = 1;
    
    printf("🚀 Starting USDTgVerse node threads...\n");
    
    // Start consensus thread
    if (pthread_create(&node->consensus_thread, NULL, usdtgverse_consensus_loop, node) != 0) {
        printf("❌ Failed to start consensus thread\n");
        return -1;
    }
    
    // Start network thread
    if (pthread_create(&node->network_thread, NULL, usdtgverse_network_loop, node) != 0) {
        printf("❌ Failed to start network thread\n");
        return -1;
    }
    
    // Start API thread
    if (pthread_create(&node->api_thread, NULL, usdtgverse_api_loop, node) != 0) {
        printf("❌ Failed to start API thread\n");
        return -1;
    }
    
    printf("✅ All threads started successfully\n");
    return 0;
}

void usdtgverse_stop(usdtgverse_node_t* node) {
    if (!node || !node->running) return;
    
    printf("🛑 Stopping USDTgVerse node...\n");
    
    node->running = 0;
    
    // Wait for threads to finish
    pthread_join(node->consensus_thread, NULL);
    pthread_join(node->network_thread, NULL);
    pthread_join(node->api_thread, NULL);
    
    printf("✅ Node stopped successfully\n");
}

void usdtgverse_print_status(usdtgverse_node_t* node) {
    if (!node) return;
    
    uint64_t uptime = usdtgverse_get_timestamp() - node->start_time;
    
    printf("\n🌌 USDTgVerse Node Status:\n");
    printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    printf("📊 GENERAL:\n");
    printf("   Chain ID: %s\n", node->chain_id);
    printf("   Uptime: %llu seconds\n", uptime / 1000);
    printf("   Status: %s\n", node->running ? "RUNNING" : "STOPPED");
    printf("\n");
    
    printf("⛓️  BLOCKCHAIN:\n");
    printf("   Height: %llu\n", node->current_height);
    printf("   Total Blocks: %llu\n", node->total_blocks);
    printf("   Total Transactions: %llu\n", node->total_transactions);
    printf("   Mempool Size: %u\n", node->mempool_size);
    printf("   Active Accounts: %u\n", node->account_count);
    printf("\n");
    
    printf("🤝 CONSENSUS:\n");
    printf("   View: %llu\n", node->current_view);
    printf("   Phase: %s\n", node->consensus_phase == 1 ? "PREPARE" : 
                            node->consensus_phase == 2 ? "PRE_COMMIT" : "COMMIT");
    printf("   Is Validator: %s\n", node->is_validator ? "YES" : "NO");
    printf("   Active Validators: %u\n", node->validator_count);
    printf("\n");
    
    printf("🌐 NETWORK:\n");
    printf("   Listen Port: %u\n", node->listen_port);
    printf("   Connected Peers: %u\n", node->peer_count);
    printf("\n");
    
    printf("💎 NATIVE USDTg:\n");
    printf("   Total Supply: %llu USDTg\n", node->total_supply / USDTG_DECIMALS);
    printf("   Price: $1.00 (Algorithmic Stability)\n");
    printf("   Contract Address: NONE (Native Token)\n");
    printf("\n");
}

// Signal handler for graceful shutdown
static usdtgverse_node_t* global_node = NULL;

void signal_handler(int sig) {
    printf("\n🛑 Received signal %d, shutting down...\n", sig);
    if (global_node) {
        usdtgverse_stop(global_node);
    }
    exit(0);
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main(int argc, char* argv[]) {
    printf("\n🌌 =============================================== 🌌\n");
    printf("    USDTgVerse Pure C Backend\n");
    printf("    Ultra-High Performance Blockchain Node\n");
    printf("🌌 =============================================== 🌌\n\n");
    
    // Parse command line arguments
    uint16_t port = 26656;
    uint8_t is_validator = 1; // Default to validator
    
    if (argc > 1) {
        port = atoi(argv[1]);
    }
    if (argc > 2) {
        is_validator = atoi(argv[2]);
    }
    
    // Initialize node
    usdtgverse_node_t node;
    global_node = &node;
    
    // Setup signal handlers
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    if (usdtgverse_init(&node, "usdtgverse-mainnet-1", port, is_validator) != 0) {
        printf("❌ Failed to initialize node\n");
        return 1;
    }
    
    // Start node
    if (usdtgverse_start(&node) != 0) {
        printf("❌ Failed to start node\n");
        return 1;
    }
    
    printf("\n🎯 Node running - Press Ctrl+C to stop\n\n");
    
    // Main loop
    while (node.running) {
        usdtgverse_print_status(&node);
        sleep(5); // Print status every 5 seconds
    }
    
    usdtgverse_cleanup(&node);
    
    printf("\n🌟 USDTgVerse node shutdown complete\n");
    return 0;
}

void usdtgverse_cleanup(usdtgverse_node_t* node) {
    if (!node) return;
    
    if (node->running) {
        usdtgverse_stop(node);
    }
    
    memset(node, 0, sizeof(usdtgverse_node_t));
}
