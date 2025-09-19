#pragma once

#include "consensus/hotstuff/messages.hpp"
#include "core/state.hpp"
#include "core/tx.hpp"
#include "core/block.hpp"
#include <memory>
#include <functional>
#include <queue>

namespace usdtgverse::consensus {

/**
 * HotStuff BFT-PoS Consensus Engine
 * 
 * Enterprise-grade consensus engine implementing HotStuff algorithm
 * with Proof-of-Stake and USDTgVerse enhancements:
 * 
 * Features:
 * - 3-Phase BFT consensus (Prepare → PreCommit → Commit)
 * - Fast finality (3 seconds)
 * - View-based leader rotation
 * - Automatic slashing for malicious behavior
 * - Dynamic validator set management
 * - High throughput (100,000+ TPS capability)
 */

// ============================================================================
// EVENT CALLBACKS
// ============================================================================

// Callback types for consensus events
using BlockCommittedCallback = std::function<void(const Block&, const QuorumCertificate&)>;
using ValidatorSlashedCallback = std::function<void(ValidatorID, const std::string& reason)>;
using ViewChangedCallback = std::function<void(ViewNumber, ValidatorID new_proposer)>;
using ConsensusErrorCallback = std::function<void(const std::string& error)>;

// Network callbacks
using BroadcastProposalCallback = std::function<void(const BlockProposal&)>;
using BroadcastVoteCallback = std::function<void(const Vote&)>;
using BroadcastNewViewCallback = std::function<void(const NewView&)>;
using BroadcastTimeoutCallback = std::function<void(const TimeoutMessage&)>;

// ============================================================================
// CONSENSUS ENGINE
// ============================================================================

class HotStuffEngine {
private:
    // Core state
    ConsensusState state_;
    std::shared_ptr<core::WorldState> world_state_;
    
    // Configuration
    struct Config {
        uint64_t base_timeout_ms = 3000;        // 3 second base timeout
        uint64_t max_timeout_ms = 30000;        // 30 second max timeout
        double timeout_multiplier = 1.5;        // Exponential backoff
        size_t max_block_size = 2 * 1024 * 1024; // 2MB blocks
        size_t max_txs_per_block = 10000;       // 10k transactions
        bool enable_slashing = true;            // Slashing enabled
        double slashing_percentage = 0.05;      // 5% slashing
    } config_;
    
    // Local validator info
    std::optional<ValidatorID> local_validator_;
    std::optional<PrivateKey> validator_private_key_;
    std::optional<PublicKey> validator_public_key_;
    
    // Vote collections
    std::map<Hash, std::vector<Vote>> prepare_votes_;
    std::map<Hash, std::vector<Vote>> pre_commit_votes_;
    std::map<Hash, std::vector<Vote>> commit_votes_;
    
    // Pending transactions
    std::queue<core::Tx> pending_transactions_;
    
    // Event callbacks
    BlockCommittedCallback on_block_committed_;
    ValidatorSlashedCallback on_validator_slashed_;
    ViewChangedCallback on_view_changed_;
    ConsensusErrorCallback on_consensus_error_;
    
    // Network callbacks
    BroadcastProposalCallback broadcast_proposal_;
    BroadcastVoteCallback broadcast_vote_;
    BroadcastNewViewCallback broadcast_new_view_;
    BroadcastTimeoutCallback broadcast_timeout_;
    
    // Timing
    Timestamp last_activity_;
    std::optional<Timestamp> view_timeout_;
    
public:
    HotStuffEngine(std::shared_ptr<core::WorldState> world_state);
    ~HotStuffEngine();
    
    // ========================================================================
    // INITIALIZATION
    // ========================================================================
    
    // Initialize as validator
    bool initialize_validator(ValidatorID validator_id, 
                            const PrivateKey& private_key,
                            const PublicKey& public_key);
    
    // Initialize validator set
    void initialize_validator_set(const ValidatorSet& validators);
    
    // Start consensus for new height
    void start_height(BlockHeight height);
    
    // ========================================================================
    // MESSAGE PROCESSING
    // ========================================================================
    
    // Process incoming messages
    void on_proposal(const BlockProposal& proposal);
    void on_vote(const Vote& vote);
    void on_new_view(const NewView& new_view);
    void on_timeout(const TimeoutMessage& timeout);
    
    // ========================================================================
    // CONSENSUS LOGIC
    // ========================================================================
    
    // Main consensus step
    void step();
    
    // Phase handlers
    void handle_prepare_phase();
    void handle_pre_commit_phase();
    void handle_commit_phase();
    void handle_decide_phase();
    
    // Proposer duties
    void propose_block();
    Block create_block();
    
    // Voting
    void cast_vote(const Hash& block_hash, VoteType vote_type);
    bool should_vote(const BlockProposal& proposal, VoteType vote_type);
    
    // View change
    void trigger_view_change();
    void process_view_change();
    
    // ========================================================================
    // TRANSACTION MANAGEMENT
    // ========================================================================
    
    // Add transaction to mempool
    void add_transaction(const core::Tx& tx);
    
    // Get pending transactions for block
    std::vector<core::Tx> get_pending_transactions(size_t max_count);
    
    // ========================================================================
    // VALIDATOR MANAGEMENT
    // ========================================================================
    
    // Update validator set
    void update_validator_set(const ValidatorSet& new_validators);
    
    // Slashing
    void slash_validator(ValidatorID validator_id, const std::string& reason);
    void detect_and_slash_equivocation();
    void detect_and_slash_liveness_faults();
    
    // ========================================================================
    // STATE QUERIES
    // ========================================================================
    
    // Current state
    BlockHeight get_current_height() const { return state_.height; }
    ViewNumber get_current_view() const { return state_.view; }
    Phase get_current_phase() const { return state_.current_phase; }
    
    // Validator info
    bool is_validator() const { return local_validator_.has_value(); }
    bool is_proposer() const { return state_.is_proposer; }
    ValidatorID get_current_proposer() const { return state_.get_current_proposer(); }
    
    // Consensus progress
    bool has_committed_block() const { return state_.commit_qc.has_value(); }
    std::optional<QuorumCertificate> get_latest_commit_qc() const { return state_.commit_qc; }
    
    // ========================================================================
    // CALLBACKS
    // ========================================================================
    
    void set_block_committed_callback(BlockCommittedCallback callback) {
        on_block_committed_ = std::move(callback);
    }
    
    void set_validator_slashed_callback(ValidatorSlashedCallback callback) {
        on_validator_slashed_ = std::move(callback);
    }
    
    void set_view_changed_callback(ViewChangedCallback callback) {
        on_view_changed_ = std::move(callback);
    }
    
    void set_consensus_error_callback(ConsensusErrorCallback callback) {
        on_consensus_error_ = std::move(callback);
    }
    
    // Network callbacks
    void set_broadcast_proposal_callback(BroadcastProposalCallback callback) {
        broadcast_proposal_ = std::move(callback);
    }
    
    void set_broadcast_vote_callback(BroadcastVoteCallback callback) {
        broadcast_vote_ = std::move(callback);
    }
    
    void set_broadcast_new_view_callback(BroadcastNewViewCallback callback) {
        broadcast_new_view_ = std::move(callback);
    }
    
    void set_broadcast_timeout_callback(BroadcastTimeoutCallback callback) {
        broadcast_timeout_ = std::move(callback);
    }
    
    // ========================================================================
    // CONFIGURATION
    // ========================================================================
    
    void set_base_timeout(uint64_t timeout_ms) { config_.base_timeout_ms = timeout_ms; }
    void set_max_block_size(size_t size) { config_.max_block_size = size; }
    void set_max_txs_per_block(size_t count) { config_.max_txs_per_block = count; }
    void enable_slashing(bool enabled) { config_.enable_slashing = enabled; }
    
    // ========================================================================
    // MONITORING & DEBUGGING
    // ========================================================================
    
    std::string get_status() const;
    std::string get_detailed_status() const;
    
    // Metrics
    struct Metrics {
        uint64_t blocks_proposed = 0;
        uint64_t votes_cast = 0;
        uint64_t view_changes = 0;
        uint64_t timeouts = 0;
        uint64_t slashing_events = 0;
        double average_block_time_ms = 0.0;
        double average_finality_time_ms = 0.0;
    } metrics_;
    
    const Metrics& get_metrics() const { return metrics_; }
    void reset_metrics() { metrics_ = Metrics{}; }
    
private:
    // ========================================================================
    // INTERNAL HELPERS
    // ========================================================================
    
    // QC management
    std::optional<QuorumCertificate> try_form_qc(const Hash& block_hash, VoteType vote_type);
    bool verify_qc(const QuorumCertificate& qc);
    void update_highest_qc(const QuorumCertificate& qc);
    
    // Vote management
    void collect_vote(const Vote& vote);
    bool verify_vote(const Vote& vote);
    StakeAmount get_voting_stake(const std::vector<Vote>& votes);
    
    // Block validation
    bool validate_proposal(const BlockProposal& proposal);
    bool validate_block_content(const Block& block);
    
    // Timing
    void update_view_timeout();
    bool is_view_timeout_expired() const;
    void reset_view_timer();
    
    // Safety checks
    bool is_safe_to_vote(const Hash& block_hash, VoteType vote_type);
    bool violates_safety(const Vote& vote);
    
    // Liveness
    void check_liveness();
    void advance_view_if_stuck();
    
    // Utilities
    Signature sign_message(const Bytes& message);
    bool verify_signature(const Bytes& message, const Signature& signature, const PublicKey& public_key);
    
    // Error handling
    void handle_error(const std::string& error);
    void log_consensus_event(const std::string& event);
};

// ============================================================================
// CONSENSUS FACTORY
// ============================================================================

class ConsensusFactory {
public:
    static std::unique_ptr<HotStuffEngine> create_hotstuff_engine(
        std::shared_ptr<core::WorldState> world_state);
    
    static ValidatorSet create_genesis_validator_set(
        const std::vector<std::pair<ValidatorID, StakeAmount>>& validators);
    
    static std::vector<core::Tx> create_validator_registration_txs(
        const std::vector<ValidatorInfo>& validators);
};

} // namespace usdtgverse::consensus

