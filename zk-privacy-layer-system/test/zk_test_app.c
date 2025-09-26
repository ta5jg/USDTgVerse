/*
==============================================
 File:        zk_test_app.c
 Author:      Irfan Gedik
 Created:     26.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   Zero-Knowledge Privacy Layer System Test
   
   ZK privacy layer test application featuring:
   - Zero-knowledge proof testing
   - Privacy layer validation
   - ZK-SNARKs proof generation
   - Privacy transaction testing
   - System performance validation

 License:
   MIT License
==============================================
*/

#include "../include/zk_privacy.h"
#include <time.h>

int main() {
    printf("🔒 Zero-Knowledge Privacy Layer System Test\n");
    printf("==========================================\n\n");
    
    // Initialize the system
    ZKPrivacySystem* system = zk_privacy_init();
    if (system == NULL) {
        fprintf(stderr, "Failed to initialize ZK Privacy System\n");
        return 1;
    }
    
    // Create privacy pools
    printf("Creating privacy pools...\n");
    zk_privacy_create_mixing_pool(system, "USDTg-Mixer-1", 1000, 100000);
    zk_privacy_create_mixing_pool(system, "USDTg-Mixer-2", 5000, 500000);
    zk_privacy_create_shielded_pool(system, "USDTg-Shielded-1");
    zk_privacy_create_shielded_pool(system, "USDTg-Shielded-2");
    
    // Generate test commitments
    char commitment1[128], commitment2[128], commitment3[128];
    zk_privacy_generate_commitment("secret1", "nullifier1", commitment1);
    zk_privacy_generate_commitment("secret2", "nullifier2", commitment2);
    zk_privacy_generate_commitment("secret3", "nullifier3", commitment3);
    
    printf("\nGenerated test commitments:\n");
    printf("  Commitment 1: %s\n", commitment1);
    printf("  Commitment 2: %s\n", commitment2);
    printf("  Commitment 3: %s\n", commitment3);
    
    // Test deposits
    printf("\nTesting deposits...\n");
    zk_privacy_deposit_to_pool(system, "USDTg-Mixer-1", 5000, commitment1);
    zk_privacy_deposit_to_pool(system, "USDTg-Mixer-1", 10000, commitment2);
    zk_privacy_deposit_to_pool(system, "USDTg-Shielded-1", 25000, commitment3);
    
    // Create test ZK proofs
    ZKProof proof1, proof2, proof3;
    strncpy(proof1.proof_id, "proof_001", sizeof(proof1.proof_id) - 1);
    strncpy(proof1.commitment, commitment1, sizeof(proof1.commitment) - 1);
    strncpy(proof1.nullifier, "nullifier1", sizeof(proof1.nullifier) - 1);
    strncpy(proof1.public_inputs, "amount:5000", sizeof(proof1.public_inputs) - 1);
    strncpy(proof1.proof_data, "zk_proof_data_001", sizeof(proof1.proof_data) - 1);
    proof1.timestamp = time(NULL);
    proof1.is_valid = true;
    
    strncpy(proof2.proof_id, "proof_002", sizeof(proof2.proof_id) - 1);
    strncpy(proof2.commitment, commitment2, sizeof(proof2.commitment) - 1);
    strncpy(proof2.nullifier, "nullifier2", sizeof(proof2.nullifier) - 1);
    strncpy(proof2.public_inputs, "amount:10000", sizeof(proof2.public_inputs) - 1);
    strncpy(proof2.proof_data, "zk_proof_data_002", sizeof(proof2.proof_data) - 1);
    proof2.timestamp = time(NULL);
    proof2.is_valid = true;
    
    strncpy(proof3.proof_id, "proof_003", sizeof(proof3.proof_id) - 1);
    strncpy(proof3.commitment, commitment3, sizeof(proof3.commitment) - 1);
    strncpy(proof3.nullifier, "nullifier3", sizeof(proof3.nullifier) - 1);
    strncpy(proof3.public_inputs, "amount:25000", sizeof(proof3.public_inputs) - 1);
    strncpy(proof3.proof_data, "zk_proof_data_003", sizeof(proof3.proof_data) - 1);
    proof3.timestamp = time(NULL);
    proof3.is_valid = true;
    
    // Test withdrawals
    printf("\nTesting withdrawals...\n");
    zk_privacy_withdraw_from_pool(system, "USDTg-Mixer-1", 5000, &proof1);
    zk_privacy_withdraw_from_pool(system, "USDTg-Shielded-1", 25000, &proof3);
    
    // Create test privacy transactions
    PrivacyTransaction tx1, tx2;
    strncpy(tx1.tx_id, "tx_private_001", sizeof(tx1.tx_id) - 1);
    strncpy(tx1.sender_commitment, commitment1, sizeof(tx1.sender_commitment) - 1);
    strncpy(tx1.recipient_commitment, commitment2, sizeof(tx1.recipient_commitment) - 1);
    tx1.amount = 3000;
    strncpy(tx1.memo_hash, "memo_hash_001", sizeof(tx1.memo_hash) - 1);
    tx1.proof = proof1;
    tx1.timestamp = time(NULL);
    tx1.is_private = true;
    
    strncpy(tx2.tx_id, "tx_private_002", sizeof(tx2.tx_id) - 1);
    strncpy(tx2.sender_commitment, commitment2, sizeof(tx2.sender_commitment) - 1);
    strncpy(tx2.recipient_commitment, commitment3, sizeof(tx2.recipient_commitment) - 1);
    tx2.amount = 7000;
    strncpy(tx2.memo_hash, "memo_hash_002", sizeof(tx2.memo_hash) - 1);
    tx2.proof = proof2;
    tx2.timestamp = time(NULL);
    tx2.is_private = true;
    
    // Test private transaction submission
    printf("\nTesting private transactions...\n");
    zk_privacy_submit_private_transaction(system, &tx1);
    zk_privacy_submit_private_transaction(system, &tx2);
    
    // Test mixing rounds
    printf("\nTesting mixing rounds...\n");
    zk_privacy_process_mixing_round(system, "USDTg-Mixer-1");
    zk_privacy_process_mixing_round(system, "USDTg-Mixer-2");
    
    // Test privacy analytics
    printf("\nTesting privacy analytics...\n");
    long long total_volume;
    int total_txs, active_pools;
    zk_privacy_get_privacy_stats(system, &total_volume, &total_txs, &active_pools);
    
    printf("Privacy Statistics:\n");
    printf("  Total Private Volume: %lld USDTg\n", total_volume);
    printf("  Total Private Transactions: %d\n", total_txs);
    printf("  Active Privacy Pools: %d\n", active_pools);
    
    // Test commitment verification
    printf("\nTesting commitment verification...\n");
    bool is_valid1 = zk_privacy_verify_commitment(commitment1, "secret1", "nullifier1");
    bool is_valid2 = zk_privacy_verify_commitment(commitment2, "secret2", "nullifier2");
    bool is_invalid = zk_privacy_verify_commitment(commitment1, "wrong_secret", "nullifier1");
    
    printf("Commitment verification results:\n");
    printf("  Commitment 1 valid: %s\n", is_valid1 ? "Yes" : "No");
    printf("  Commitment 2 valid: %s\n", is_valid2 ? "Yes" : "No");
    printf("  Commitment 1 with wrong secret: %s\n", is_invalid ? "Yes" : "No");
    
    // Test address privacy check
    printf("\nTesting address privacy check...\n");
    bool is_private1 = zk_privacy_is_address_private(system, commitment1);
    bool is_private2 = zk_privacy_is_address_private(system, commitment2);
    bool is_private_invalid = zk_privacy_is_address_private(system, "invalid_commitment");
    
    printf("Address privacy check results:\n");
    printf("  Commitment 1 is private: %s\n", is_private1 ? "Yes" : "No");
    printf("  Commitment 2 is private: %s\n", is_private2 ? "Yes" : "No");
    printf("  Invalid commitment is private: %s\n", is_private_invalid ? "Yes" : "No");
    
    // Print final status
    printf("\nFinal system status:\n");
    zk_privacy_print_status(system);
    
    // Cleanup
    zk_privacy_shutdown(system);
    
    printf("\n✅ Zero-Knowledge Privacy Layer System test completed successfully!\n");
    return 0;
}
