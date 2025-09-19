#pragma once

#include "common/types.hpp"
#include "common/bytes.hpp"
#include "crypto/keys.hpp"
#include <vector>
#include <optional>

namespace usdtgverse::consensus {

/**
 * HotStuff BFT-PoS Consensus Messages
 * 
 * Enterprise-grade Byzantine Fault Tolerant Proof-of-Stake consensus
 * based on HotStuff algorithm with USDTgVerse enhancements:
 * 
 * - 3-Phase Consensus: Prepare → PreCommit → Commit
 * - View-based leader rotation
 * - Quorum Certificates (QC) for finality
 * - Slashing for malicious behavior
 * - Fast finality (3 seconds)
 */

// ============================================================================
// CONSENSUS TYPES
// ============================================================================

using ValidatorID = Address;
using ViewNumber = uint64_t;
using StakeAmount = Amount;

// Consensus phases
enum class Phase : uint8_t {
    PREPARE = 1,
    PRE_COMMIT = 2,
    COMMIT = 3,
    DECIDE = 4
};

// Vote types
enum class VoteType : uint8_t {
    PREPARE_VOTE = 1,
    PRE_COMMIT_VOTE = 2,
    COMMIT_VOTE = 3
};

// ============================================================================
// BLOCK PROPOSAL
// ============================================================================

struct BlockProposal {
    BlockHeight height;
    ViewNumber view;
    Hash block_hash;
    Hash parent_hash;
    ValidatorID proposer;
    Timestamp timestamp;
    
    // Previous QC that justifies this block
    Hash justify_qc_hash;
    
    // Block content hash (transactions, state root, etc.)
    Hash content_hash;
    
    // Proposer signature
    Signature proposer_signature;
    
    BlockProposal();
    BlockProposal(BlockHeight h, ViewNumber v, const Hash& block_id, 
                  const Hash& parent, ValidatorID prop, Timestamp ts);
    
    Hash calculate_hash() const;
    Bytes encode() const;
    static std::optional<BlockProposal> decode(const Bytes& data);
    bool is_valid() const;
    std::string to_string() const;
};

// ============================================================================
// VOTES
// ============================================================================

struct Vote {
    BlockHeight height;
    ViewNumber view;
    Hash block_hash;
    VoteType vote_type;
    ValidatorID validator;
    
    // Validator signature over (height, view, block_hash, vote_type)
    Signature signature;
    
    Vote();
    Vote(BlockHeight h, ViewNumber v, const Hash& block_id, 
         VoteType type, ValidatorID val);
    
    Hash calculate_hash() const;
    Bytes encode() const;
    static std::optional<Vote> decode(const Bytes& data);
    bool is_valid() const;
    std::string to_string() const;
    
    // Verify signature
    bool verify_signature(const PublicKey& validator_pubkey) const;
};

// ============================================================================
// QUORUM CERTIFICATE (QC)
// ============================================================================

struct QuorumCertificate {
    BlockHeight height;
    ViewNumber view;
    Hash block_hash;
    VoteType vote_type;
    
    // Aggregated votes (simplified - in real implementation would use BLS)
    std::vector<Vote> votes;
    
    // QC metadata
    Timestamp created_at;
    size_t total_stake;
    size_t voting_stake;
    
    QuorumCertificate();
    QuorumCertificate(BlockHeight h, ViewNumber v, const Hash& block_id, VoteType type);
    
    Hash calculate_hash() const;
    Bytes encode() const;
    static std::optional<QuorumCertificate> decode(const Bytes& data);
    
    bool is_valid() const;
    bool has_quorum(size_t total_validator_stake) const;
    void add_vote(const Vote& vote, StakeAmount validator_stake);
    
    std::string to_string() const;
};

// ============================================================================
// VIEW CHANGE
// ============================================================================

struct NewView {
    ViewNumber new_view;
    ValidatorID validator;
    
    // Highest QC this validator has seen
    QuorumCertificate highest_qc;
    
    // Timeout certificate (if view change due to timeout)
    std::optional<QuorumCertificate> timeout_qc;
    
    // Validator signature
    Signature signature;
    Timestamp timestamp;
    
    NewView();
    NewView(ViewNumber view, ValidatorID val, const QuorumCertificate& qc);
    
    Hash calculate_hash() const;
    Bytes encode() const;
    static std::optional<NewView> decode(const Bytes& data);
    bool is_valid() const;
    std::string to_string() const;
};

// ============================================================================
// TIMEOUT
// ============================================================================

struct TimeoutMessage {
    ViewNumber view;
    ValidatorID validator;
    
    // Highest QC this validator has for the view
    std::optional<QuorumCertificate> highest_qc;
    
    // Validator signature
    Signature signature;
    Timestamp timestamp;
    
    TimeoutMessage();
    TimeoutMessage(ViewNumber v, ValidatorID val);
    
    Hash calculate_hash() const;
    Bytes encode() const;
    static std::optional<TimeoutMessage> decode(const Bytes& data);
    bool is_valid() const;
    std::string to_string() const;
};

// ============================================================================
// VALIDATOR INFO
// ============================================================================

struct ValidatorInfo {
    ValidatorID validator_id;
    PublicKey public_key;
    StakeAmount stake;
    
    // Validator metadata
    std::string moniker;
    std::string website;
    double commission_rate;
    
    // Status
    bool is_active;
    bool is_jailed;
    BlockHeight jailed_until;
    
    // Performance metrics
    uint64_t blocks_proposed;
    uint64_t votes_cast;
    uint64_t missed_blocks;
    double uptime_percentage;
    
    ValidatorInfo();
    ValidatorInfo(ValidatorID id, const PublicKey& pubkey, StakeAmount stake_amount);
    
    bool is_eligible(BlockHeight current_height) const;
    double voting_power_ratio(StakeAmount total_stake) const;
    std::string to_string() const;
};

// ============================================================================
// VALIDATOR SET
// ============================================================================

class ValidatorSet {
private:
    std::vector<ValidatorInfo> validators_;
    StakeAmount total_stake_;
    BlockHeight height_;
    Hash validators_hash_;
    
public:
    ValidatorSet();
    explicit ValidatorSet(const std::vector<ValidatorInfo>& validators, BlockHeight height = 0);
    
    // Validator management
    void add_validator(const ValidatorInfo& validator);
    void remove_validator(ValidatorID validator_id);
    void update_validator(const ValidatorInfo& validator);
    
    // Queries
    std::optional<ValidatorInfo> get_validator(ValidatorID validator_id) const;
    std::vector<ValidatorInfo> get_active_validators() const;
    size_t size() const { return validators_.size(); }
    bool empty() const { return validators_.empty(); }
    
    // Stake calculations
    StakeAmount get_total_stake() const { return total_stake_; }
    StakeAmount get_validator_stake(ValidatorID validator_id) const;
    double get_voting_power(ValidatorID validator_id) const;
    
    // Consensus thresholds
    StakeAmount get_quorum_threshold() const; // 2f+1 threshold
    bool has_quorum(StakeAmount voting_stake) const;
    
    // Leader selection
    ValidatorID select_proposer(ViewNumber view) const;
    std::vector<ValidatorID> get_committee(ViewNumber view, size_t committee_size) const;
    
    // Validation
    bool is_valid() const;
    Hash calculate_hash() const;
    void update_hash();
    
    // Serialization
    Bytes encode() const;
    static std::optional<ValidatorSet> decode(const Bytes& data);
    
    std::string to_string() const;
    
private:
    void recalculate_total_stake();
    uint64_t deterministic_random(ViewNumber view, size_t index) const;
};

// ============================================================================
// CONSENSUS STATE
// ============================================================================

struct ConsensusState {
    BlockHeight height;
    ViewNumber view;
    Phase current_phase;
    
    // Current round state
    std::optional<BlockProposal> current_proposal;
    std::optional<QuorumCertificate> prepare_qc;
    std::optional<QuorumCertificate> pre_commit_qc;
    std::optional<QuorumCertificate> commit_qc;
    
    // Highest QCs seen
    QuorumCertificate highest_prepare_qc;
    QuorumCertificate highest_pre_commit_qc;
    QuorumCertificate highest_commit_qc;
    
    // View change state
    std::vector<NewView> new_view_messages;
    std::vector<TimeoutMessage> timeout_messages;
    
    // Validator set for this height
    ValidatorSet validator_set;
    
    // Local validator info
    std::optional<ValidatorID> local_validator_id;
    bool is_proposer;
    
    // Timing
    Timestamp view_start_time;
    uint64_t view_timeout_ms;
    
    ConsensusState();
    ConsensusState(BlockHeight h, ViewNumber v, const ValidatorSet& validators);
    
    void reset_for_new_view(ViewNumber new_view);
    void reset_for_new_height(BlockHeight new_height);
    
    bool should_timeout() const;
    ValidatorID get_current_proposer() const;
    
    std::string to_string() const;
};

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

// Message validation
bool is_valid_phase_transition(Phase from, Phase to);
bool is_valid_vote_for_phase(VoteType vote_type, Phase phase);

// Timing utilities
uint64_t calculate_view_timeout(ViewNumber view, uint64_t base_timeout_ms = 3000);
bool is_timeout_expired(Timestamp start_time, uint64_t timeout_ms);

// Stake utilities
StakeAmount calculate_quorum_threshold(StakeAmount total_stake);
double calculate_voting_power_percentage(StakeAmount validator_stake, StakeAmount total_stake);

// Hash utilities
Hash calculate_vote_hash(BlockHeight height, ViewNumber view, const Hash& block_hash, VoteType vote_type);
Hash calculate_qc_hash(const QuorumCertificate& qc);

// String utilities
std::string phase_to_string(Phase phase);
std::string vote_type_to_string(VoteType vote_type);
Phase string_to_phase(const std::string& phase_str);
VoteType string_to_vote_type(const std::string& vote_str);

} // namespace usdtgverse::consensus

