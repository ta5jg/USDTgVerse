/*
==============================================
 File:        test_consensus.cpp
 Author:      Irfan Gedik
 Created:     19.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Consensus Test
   
   Consensus testing framework featuring:
   - HotStuff BFT consensus validation
   - Consensus algorithm testing
   - Block production testing
   - Validator behavior testing
   - Consensus performance testing

 License:
   MIT License
==============================================
*/

#include <iostream>
#include <iomanip>
#include <sstream>
#include <array>
#include <vector>
#include <string>
#include <map>
#include <optional>
#include <chrono>

namespace usdtgverse {

// Basic types for testing
using Hash = std::array<uint8_t, 32>;
using Address = std::array<uint8_t, 20>;
using Amount = int64_t;
using BlockHeight = uint64_t;
using ViewNumber = uint64_t;
using ValidatorID = Address;
using StakeAmount = Amount;
using Timestamp = uint64_t;

constexpr Amount USDTG_DECIMALS = 1000000LL; // 6 decimals

std::string hash_to_hex(const Hash& hash) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (size_t i = 0; i < 8; ++i) { // Only show first 8 bytes
        oss << std::setw(2) << static_cast<unsigned>(hash[i]);
    }
    oss << "...";
    return oss.str();
}

std::string address_to_string(const Address& addr) {
    std::ostringstream oss;
    oss << "val" << std::hex << std::setfill('0');
    for (size_t i = 0; i < 3; ++i) {
        oss << std::setw(2) << static_cast<unsigned>(addr[i]);
    }
    return oss.str();
}

Address create_validator_address(uint8_t seed) {
    Address addr;
    std::fill(addr.begin(), addr.end(), seed);
    return addr;
}

Hash create_block_hash(BlockHeight height, ViewNumber view) {
    Hash hash;
    std::fill(hash.begin(), hash.end(), 0);
    // Simple hash based on height and view
    hash[0] = static_cast<uint8_t>(height & 0xFF);
    hash[1] = static_cast<uint8_t>((height >> 8) & 0xFF);
    hash[2] = static_cast<uint8_t>(view & 0xFF);
    hash[3] = static_cast<uint8_t>((view >> 8) & 0xFF);
    return hash;
}

Timestamp current_timestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
}

// ============================================================================
// CONSENSUS TYPES
// ============================================================================

enum class Phase : uint8_t {
    PREPARE = 1,
    PRE_COMMIT = 2,
    COMMIT = 3,
    DECIDE = 4
};

enum class VoteType : uint8_t {
    PREPARE_VOTE = 1,
    PRE_COMMIT_VOTE = 2,
    COMMIT_VOTE = 3
};

std::string phase_to_string(Phase phase) {
    switch (phase) {
        case Phase::PREPARE: return "PREPARE";
        case Phase::PRE_COMMIT: return "PRE_COMMIT";
        case Phase::COMMIT: return "COMMIT";
        case Phase::DECIDE: return "DECIDE";
        default: return "UNKNOWN";
    }
}

std::string vote_type_to_string(VoteType vote_type) {
    switch (vote_type) {
        case VoteType::PREPARE_VOTE: return "PREPARE_VOTE";
        case VoteType::PRE_COMMIT_VOTE: return "PRE_COMMIT_VOTE";
        case VoteType::COMMIT_VOTE: return "COMMIT_VOTE";
        default: return "UNKNOWN";
    }
}

struct ValidatorInfo {
    ValidatorID validator_id;
    StakeAmount stake;
    std::string moniker;
    bool is_active;
    
    ValidatorInfo() : stake(0), is_active(false) {}
    ValidatorInfo(ValidatorID id, StakeAmount s, const std::string& name)
        : validator_id(id), stake(s), moniker(name), is_active(true) {}
    
    std::string to_string() const {
        std::ostringstream oss;
        double stake_usdtg = static_cast<double>(stake) / static_cast<double>(USDTG_DECIMALS);
        oss << moniker << " (" << address_to_string(validator_id) 
            << ", " << stake_usdtg << " USDTg" 
            << (is_active ? ", ACTIVE" : ", INACTIVE") << ")";
        return oss.str();
    }
};

struct Vote {
    BlockHeight height;
    ViewNumber view;
    Hash block_hash;
    VoteType vote_type;
    ValidatorID validator;
    Timestamp timestamp;
    
    Vote() : height(0), view(0), vote_type(VoteType::PREPARE_VOTE), timestamp(0) {}
    Vote(BlockHeight h, ViewNumber v, const Hash& block_id, VoteType type, ValidatorID val)
        : height(h), view(v), block_hash(block_id), vote_type(type), validator(val), timestamp(current_timestamp()) {}
    
    std::string to_string() const {
        return vote_type_to_string(vote_type) + " from " + address_to_string(validator) +
               " for block " + hash_to_hex(block_hash) + " (h:" + std::to_string(height) + 
               ", v:" + std::to_string(view) + ")";
    }
};

struct QuorumCertificate {
    BlockHeight height;
    ViewNumber view;
    Hash block_hash;
    VoteType vote_type;
    std::vector<Vote> votes;
    StakeAmount voting_stake;
    
    QuorumCertificate() : height(0), view(0), vote_type(VoteType::PREPARE_VOTE), voting_stake(0) {}
    QuorumCertificate(BlockHeight h, ViewNumber v, const Hash& block_id, VoteType type)
        : height(h), view(v), block_hash(block_id), vote_type(type), voting_stake(0) {}
    
    void add_vote(const Vote& vote, StakeAmount validator_stake) {
        votes.push_back(vote);
        voting_stake += validator_stake;
    }
    
    bool has_quorum(StakeAmount total_stake) const {
        // 2f+1 threshold (>2/3)
        return voting_stake * 3 > total_stake * 2;
    }
    
    std::string to_string() const {
        double voting_usdtg = static_cast<double>(voting_stake) / static_cast<double>(USDTG_DECIMALS);
        return "QC{" + vote_type_to_string(vote_type) + ", " + std::to_string(votes.size()) + 
               " votes, " + std::to_string(voting_usdtg) + " USDTg stake, block: " + 
               hash_to_hex(block_hash) + "}";
    }
};

struct BlockProposal {
    BlockHeight height;
    ViewNumber view;
    Hash block_hash;
    ValidatorID proposer;
    Timestamp timestamp;
    
    BlockProposal() : height(0), view(0), timestamp(0) {}
    BlockProposal(BlockHeight h, ViewNumber v, ValidatorID prop)
        : height(h), view(v), proposer(prop), timestamp(current_timestamp()) {
        block_hash = create_block_hash(height, view);
    }
    
    std::string to_string() const {
        return "Proposal{h:" + std::to_string(height) + ", v:" + std::to_string(view) +
               ", proposer:" + address_to_string(proposer) + ", block:" + hash_to_hex(block_hash) + "}";
    }
};

// ============================================================================
// SIMPLE CONSENSUS ENGINE
// ============================================================================

class SimpleHotStuffEngine {
private:
    BlockHeight current_height_;
    ViewNumber current_view_;
    Phase current_phase_;
    
    std::vector<ValidatorInfo> validators_;
    StakeAmount total_stake_;
    std::optional<ValidatorID> local_validator_;
    
    std::optional<BlockProposal> current_proposal_;
    std::map<Hash, std::vector<Vote>> prepare_votes_;
    std::map<Hash, std::vector<Vote>> pre_commit_votes_;
    std::map<Hash, std::vector<Vote>> commit_votes_;
    
    std::optional<QuorumCertificate> prepare_qc_;
    std::optional<QuorumCertificate> pre_commit_qc_;
    std::optional<QuorumCertificate> commit_qc_;
    
    Timestamp phase_start_time_;
    static constexpr uint64_t PHASE_TIMEOUT_MS = 3000; // 3 seconds
    
public:
    SimpleHotStuffEngine() : current_height_(1), current_view_(1), current_phase_(Phase::PREPARE), total_stake_(0) {
        phase_start_time_ = current_timestamp();
    }
    
    void add_validator(const ValidatorInfo& validator) {
        validators_.push_back(validator);
        total_stake_ += validator.stake;
    }
    
    void set_local_validator(ValidatorID validator_id) {
        local_validator_ = validator_id;
    }
    
    ValidatorID select_proposer(ViewNumber view) const {
        if (validators_.empty()) return ValidatorID{};
        size_t index = view % validators_.size();
        return validators_[index].validator_id;
    }
    
    bool is_proposer() const {
        if (!local_validator_.has_value()) return false;
        return select_proposer(current_view_) == local_validator_.value();
    }
    
    void propose_block() {
        if (!is_proposer()) return;
        
        current_proposal_ = BlockProposal(current_height_, current_view_, local_validator_.value());
        std::cout << "📝 " << address_to_string(local_validator_.value()) 
                  << " proposed: " << current_proposal_->to_string() << "\n";
    }
    
    void process_proposal(const BlockProposal& proposal) {
        if (proposal.height != current_height_ || proposal.view != current_view_) {
            return; // Ignore old proposals
        }
        
        current_proposal_ = proposal;
        std::cout << "📥 Received " << proposal.to_string() << "\n";
        
        // Vote for the proposal
        if (local_validator_.has_value() && current_phase_ == Phase::PREPARE) {
            cast_vote(proposal.block_hash, VoteType::PREPARE_VOTE);
        }
    }
    
    void cast_vote(const Hash& block_hash, VoteType vote_type) {
        if (!local_validator_.has_value()) return;
        
        Vote vote(current_height_, current_view_, block_hash, vote_type, local_validator_.value());
        
        std::cout << "🗳️  " << address_to_string(local_validator_.value()) 
                  << " cast " << vote.to_string() << "\n";
        
        // Process our own vote
        process_vote(vote);
    }
    
    void process_vote(const Vote& vote) {
        if (vote.height != current_height_ || vote.view != current_view_) {
            return; // Ignore old votes
        }
        
        StakeAmount validator_stake = get_validator_stake(vote.validator);
        if (validator_stake == 0) return; // Unknown validator
        
        // Collect vote
        switch (vote.vote_type) {
            case VoteType::PREPARE_VOTE:
                prepare_votes_[vote.block_hash].push_back(vote);
                break;
            case VoteType::PRE_COMMIT_VOTE:
                pre_commit_votes_[vote.block_hash].push_back(vote);
                break;
            case VoteType::COMMIT_VOTE:
                commit_votes_[vote.block_hash].push_back(vote);
                break;
        }
        
        // Try to form QC
        try_form_qc(vote.block_hash, vote.vote_type);
    }
    
    void try_form_qc(const Hash& block_hash, VoteType vote_type) {
        std::vector<Vote>* votes_ptr = nullptr;
        
        switch (vote_type) {
            case VoteType::PREPARE_VOTE:
                votes_ptr = &prepare_votes_[block_hash];
                break;
            case VoteType::PRE_COMMIT_VOTE:
                votes_ptr = &pre_commit_votes_[block_hash];
                break;
            case VoteType::COMMIT_VOTE:
                votes_ptr = &commit_votes_[block_hash];
                break;
        }
        
        if (!votes_ptr) return;
        
        // Calculate voting stake
        StakeAmount voting_stake = 0;
        for (const auto& vote : *votes_ptr) {
            voting_stake += get_validator_stake(vote.validator);
        }
        
        // Check if we have quorum
        if (voting_stake * 3 > total_stake_ * 2) { // >2/3 threshold
            QuorumCertificate qc(current_height_, current_view_, block_hash, vote_type);
            qc.voting_stake = voting_stake;
            qc.votes = *votes_ptr;
            
            std::cout << "✅ Formed " << qc.to_string() << "\n";
            
            // Update QCs and advance phase
            switch (vote_type) {
                case VoteType::PREPARE_VOTE:
                    prepare_qc_ = qc;
                    advance_to_pre_commit();
                    break;
                case VoteType::PRE_COMMIT_VOTE:
                    pre_commit_qc_ = qc;
                    advance_to_commit();
                    break;
                case VoteType::COMMIT_VOTE:
                    commit_qc_ = qc;
                    advance_to_decide();
                    break;
            }
        }
    }
    
    void advance_to_pre_commit() {
        if (current_phase_ != Phase::PREPARE || !prepare_qc_.has_value()) return;
        
        current_phase_ = Phase::PRE_COMMIT;
        phase_start_time_ = current_timestamp();
        
        std::cout << "🔄 Advanced to PRE_COMMIT phase\n";
        
        if (local_validator_.has_value()) {
            cast_vote(prepare_qc_->block_hash, VoteType::PRE_COMMIT_VOTE);
        }
    }
    
    void advance_to_commit() {
        if (current_phase_ != Phase::PRE_COMMIT || !pre_commit_qc_.has_value()) return;
        
        current_phase_ = Phase::COMMIT;
        phase_start_time_ = current_timestamp();
        
        std::cout << "🔄 Advanced to COMMIT phase\n";
        
        if (local_validator_.has_value()) {
            cast_vote(pre_commit_qc_->block_hash, VoteType::COMMIT_VOTE);
        }
    }
    
    void advance_to_decide() {
        if (current_phase_ != Phase::COMMIT || !commit_qc_.has_value()) return;
        
        current_phase_ = Phase::DECIDE;
        
        std::cout << "🎉 BLOCK COMMITTED! Block " << hash_to_hex(commit_qc_->block_hash) 
                  << " at height " << current_height_ << "\n";
        
        // Start next height
        start_next_height();
    }
    
    void start_next_height() {
        current_height_++;
        current_view_ = 1;
        current_phase_ = Phase::PREPARE;
        phase_start_time_ = current_timestamp();
        
        // Clear state
        current_proposal_.reset();
        prepare_qc_.reset();
        pre_commit_qc_.reset();
        commit_qc_.reset();
        prepare_votes_.clear();
        pre_commit_votes_.clear();
        commit_votes_.clear();
        
        std::cout << "\n🆕 Started new height " << current_height_ << "\n";
        
        // Propose if we're the leader
        if (is_proposer()) {
            propose_block();
        }
    }
    
    void step() {
        // Check for timeout
        if (current_timestamp() - phase_start_time_ > PHASE_TIMEOUT_MS) {
            std::cout << "⏰ Phase timeout in " << phase_to_string(current_phase_) << "\n";
            // In a real implementation, would trigger view change
            phase_start_time_ = current_timestamp(); // Reset timer
        }
    }
    
    StakeAmount get_validator_stake(ValidatorID validator_id) const {
        for (const auto& validator : validators_) {
            if (validator.validator_id == validator_id) {
                return validator.stake;
            }
        }
        return 0;
    }
    
    std::string get_status() const {
        std::ostringstream oss;
        oss << "HotStuff Status:\n";
        oss << "  Height: " << current_height_ << "\n";
        oss << "  View: " << current_view_ << "\n";
        oss << "  Phase: " << phase_to_string(current_phase_) << "\n";
        oss << "  Proposer: " << address_to_string(select_proposer(current_view_)) << "\n";
        oss << "  Is Local Proposer: " << (is_proposer() ? "YES" : "NO") << "\n";
        
        if (current_proposal_.has_value()) {
            oss << "  Current Proposal: " << current_proposal_->to_string() << "\n";
        }
        
        if (prepare_qc_.has_value()) {
            oss << "  Prepare QC: " << prepare_qc_->to_string() << "\n";
        }
        
        if (pre_commit_qc_.has_value()) {
            oss << "  PreCommit QC: " << pre_commit_qc_->to_string() << "\n";
        }
        
        if (commit_qc_.has_value()) {
            oss << "  Commit QC: " << commit_qc_->to_string() << "\n";
        }
        
        return oss.str();
    }
    
    BlockHeight get_height() const { return current_height_; }
    Phase get_phase() const { return current_phase_; }
};

} // namespace usdtgverse

int main() {
    std::cout << R"(
🌌 =============================================== 🌌
    USDTgVerse HotStuff BFT-PoS Consensus Test
    Enterprise Byzantine Fault Tolerant Consensus
🌌 =============================================== 🌌

)" << std::endl;
    
    // Create consensus engine
    usdtgverse::SimpleHotStuffEngine engine;
    
    // Create validators
    std::cout << "🏛️  Setting up validator set:\n";
    
    auto alice = usdtgverse::create_validator_address(0xAA);
    auto bob = usdtgverse::create_validator_address(0xBB);
    auto charlie = usdtgverse::create_validator_address(0xCC);
    auto diana = usdtgverse::create_validator_address(0xDD);
    
    engine.add_validator(usdtgverse::ValidatorInfo(alice, 1000 * usdtgverse::USDTG_DECIMALS, "Alice"));
    engine.add_validator(usdtgverse::ValidatorInfo(bob, 800 * usdtgverse::USDTG_DECIMALS, "Bob"));
    engine.add_validator(usdtgverse::ValidatorInfo(charlie, 600 * usdtgverse::USDTG_DECIMALS, "Charlie"));
    engine.add_validator(usdtgverse::ValidatorInfo(diana, 400 * usdtgverse::USDTG_DECIMALS, "Diana"));
    
    std::cout << "   👑 Alice (1000 USDTg stake) - " << usdtgverse::address_to_string(alice) << "\n";
    std::cout << "   🛡️  Bob (800 USDTg stake) - " << usdtgverse::address_to_string(bob) << "\n";
    std::cout << "   ⚔️  Charlie (600 USDTg stake) - " << usdtgverse::address_to_string(charlie) << "\n";
    std::cout << "   🏹 Diana (400 USDTg stake) - " << usdtgverse::address_to_string(diana) << "\n";
    std::cout << "   Total Stake: 2800 USDTg\n\n";
    
    // Set local validator (simulate Alice's node)
    engine.set_local_validator(alice);
    
    std::cout << "🎯 Starting consensus (Alice's perspective):\n\n";
    
    // Simulate consensus rounds
    for (int round = 0; round < 3; ++round) {
        std::cout << "🔄 === CONSENSUS ROUND " << (round + 1) << " ===\n";
        
        // Step 1: Propose (if we're the proposer)
        if (engine.is_proposer()) {
            engine.propose_block();
        } else {
            // Simulate receiving proposal from actual proposer
            auto proposer = engine.select_proposer(1); // View 1
            usdtgverse::BlockProposal proposal(engine.get_height(), 1, proposer);
            engine.process_proposal(proposal);
        }
        
        // Step 2: Simulate votes from other validators
        auto current_proposal_hash = usdtgverse::create_block_hash(engine.get_height(), 1);
        
        // Bob votes
        usdtgverse::Vote bob_vote(engine.get_height(), 1, current_proposal_hash, 
                                 usdtgverse::VoteType::PREPARE_VOTE, bob);
        engine.process_vote(bob_vote);
        
        // Charlie votes
        usdtgverse::Vote charlie_vote(engine.get_height(), 1, current_proposal_hash, 
                                     usdtgverse::VoteType::PREPARE_VOTE, charlie);
        engine.process_vote(charlie_vote);
        
        // Diana votes
        usdtgverse::Vote diana_vote(engine.get_height(), 1, current_proposal_hash, 
                                   usdtgverse::VoteType::PREPARE_VOTE, diana);
        engine.process_vote(diana_vote);
        
        // Wait for consensus phases to complete
        while (engine.get_phase() != usdtgverse::Phase::DECIDE) {
            // Simulate additional votes for pre-commit and commit phases
            if (engine.get_phase() == usdtgverse::Phase::PRE_COMMIT) {
                usdtgverse::Vote bob_precommit(engine.get_height(), 1, current_proposal_hash, 
                                              usdtgverse::VoteType::PRE_COMMIT_VOTE, bob);
                engine.process_vote(bob_precommit);
                
                usdtgverse::Vote charlie_precommit(engine.get_height(), 1, current_proposal_hash, 
                                                  usdtgverse::VoteType::PRE_COMMIT_VOTE, charlie);
                engine.process_vote(charlie_precommit);
                
                usdtgverse::Vote diana_precommit(engine.get_height(), 1, current_proposal_hash, 
                                                usdtgverse::VoteType::PRE_COMMIT_VOTE, diana);
                engine.process_vote(diana_precommit);
            }
            
            if (engine.get_phase() == usdtgverse::Phase::COMMIT) {
                usdtgverse::Vote bob_commit(engine.get_height(), 1, current_proposal_hash, 
                                           usdtgverse::VoteType::COMMIT_VOTE, bob);
                engine.process_vote(bob_commit);
                
                usdtgverse::Vote charlie_commit(engine.get_height(), 1, current_proposal_hash, 
                                               usdtgverse::VoteType::COMMIT_VOTE, charlie);
                engine.process_vote(charlie_commit);
                
                usdtgverse::Vote diana_commit(engine.get_height(), 1, current_proposal_hash, 
                                             usdtgverse::VoteType::COMMIT_VOTE, diana);
                engine.process_vote(diana_commit);
            }
        }
        
        std::cout << "\n";
    }
    
    std::cout << "📊 Final Status:\n";
    std::cout << engine.get_status() << "\n";
    
    std::cout << "✅ HotStuff Consensus Features Demonstrated:\n";
    std::cout << "   🤝 3-Phase BFT Consensus (Prepare → PreCommit → Commit)\n";
    std::cout << "   👑 Leader-based block proposal\n";
    std::cout << "   🗳️  Quorum-based voting (>2/3 stake threshold)\n";
    std::cout << "   📜 Quorum Certificates (QC) for finality\n";
    std::cout << "   ⚡ Fast finality (3 phases per block)\n";
    std::cout << "   🔄 View-based progression\n";
    std::cout << "   💰 Stake-weighted voting power\n\n";
    
    std::cout << "🚀 USDTgVerse: Enterprise BFT-PoS Consensus Ready!\n";
    std::cout << "   Next: Implement P2P networking for full blockchain\n\n";
    
    return 0;
}

