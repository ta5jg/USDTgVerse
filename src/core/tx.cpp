#include "core/tx.hpp"
#include "crypto/hash.hpp"
#include "codec/encode.hpp"
#include <algorithm>

namespace usdtgverse::core {

// ============================================================================
// COIN IMPLEMENTATION
// ============================================================================

Coin::Coin() : denom_id(USDTG_DENOM_ID), amount(0) {}

Coin::Coin(AssetID denom, Amount amt) : denom_id(denom), amount(amt) {}

bool Coin::is_valid() const {
    return amount >= 0;
}

bool Coin::is_zero() const {
    return amount == 0;
}

bool Coin::is_usdtg() const {
    return denom_id == USDTG_DENOM_ID;
}

std::string Coin::to_string() const {
    if (is_usdtg()) {
        double value = static_cast<double>(amount) / static_cast<double>(USDTG_DECIMALS);
        return std::to_string(value) + " USDTg";
    } else {
        return std::to_string(amount) + " (denom:" + std::to_string(denom_id) + ")";
    }
}

bool Coin::operator==(const Coin& other) const {
    return denom_id == other.denom_id && amount == other.amount;
}

bool Coin::operator!=(const Coin& other) const {
    return !(*this == other);
}

Coin Coin::operator+(const Coin& other) const {
    if (denom_id != other.denom_id) {
        throw std::invalid_argument("Cannot add coins of different denominations");
    }
    return Coin(denom_id, amount + other.amount);
}

Coin Coin::operator-(const Coin& other) const {
    if (denom_id != other.denom_id) {
        throw std::invalid_argument("Cannot subtract coins of different denominations");
    }
    if (amount < other.amount) {
        throw std::invalid_argument("Insufficient funds for subtraction");
    }
    return Coin(denom_id, amount - other.amount);
}

// ============================================================================
// FEE IMPLEMENTATION
// ============================================================================

Fee::Fee() : max_fee(0), tip(0), weight(1000) {}

Fee::Fee(FeeAmount max, FeeAmount t, uint32_t w) : max_fee(max), tip(t), weight(w) {}

bool Fee::is_valid() const {
    return max_fee >= tip && weight > 0;
}

FeeAmount Fee::total_fee() const {
    return max_fee + tip;
}

double Fee::fee_per_weight() const {
    return static_cast<double>(total_fee()) / static_cast<double>(weight);
}

std::string Fee::to_string() const {
    double max_usdtg = static_cast<double>(max_fee) / static_cast<double>(USDTG_DECIMALS);
    double tip_usdtg = static_cast<double>(tip) / static_cast<double>(USDTG_DECIMALS);
    return "Fee{max: " + std::to_string(max_usdtg) + " USDTg, tip: " + 
           std::to_string(tip_usdtg) + " USDTg, weight: " + std::to_string(weight) + "}";
}

// ============================================================================
// MSG IMPLEMENTATION
// ============================================================================

Msg::Msg() : type(MSG_TRANSFER) {}

Msg::Msg(MsgType t, const Bytes& data) : type(t), payload(data) {}

bool Msg::is_valid() const {
    return payload.size() <= MAX_MSG_SIZE;
}

uint32_t Msg::calculate_weight() const {
    // Base weight + payload size
    uint32_t base_weight = 1000;
    uint32_t payload_weight = static_cast<uint32_t>(payload.size()) * 10;
    
    // Different message types have different weights
    switch (type) {
        case MSG_TRANSFER: return base_weight + payload_weight;
        case MSG_MULTISEND: return base_weight * 2 + payload_weight;
        case MSG_INVOICE: return base_weight + payload_weight;
        case MSG_PAY_INVOICE: return base_weight + payload_weight;
        case MSG_ESCROW_CREATE: return base_weight * 3 + payload_weight;
        case MSG_ESCROW_RELEASE: return base_weight + payload_weight;
        case MSG_SUBSCRIBE: return base_weight + payload_weight;
        case MSG_SET_SPENDING_LIMIT: return base_weight + payload_weight;
        case MSG_BATCH_PAY: return base_weight * 5 + payload_weight;
        default: return base_weight + payload_weight;
    }
}

std::string Msg::type_to_string() const {
    switch (type) {
        case MSG_TRANSFER: return "Transfer";
        case MSG_MULTISEND: return "MultiSend";
        case MSG_INVOICE: return "Invoice";
        case MSG_PAY_INVOICE: return "PayInvoice";
        case MSG_ESCROW_CREATE: return "EscrowCreate";
        case MSG_ESCROW_RELEASE: return "EscrowRelease";
        case MSG_SUBSCRIBE: return "Subscribe";
        case MSG_SET_SPENDING_LIMIT: return "SetSpendingLimit";
        case MSG_BATCH_PAY: return "BatchPay";
        default: return "Unknown";
    }
}

// ============================================================================
// TX SIGNATURE IMPLEMENTATION
// ============================================================================

TxSignature::TxSignature() {}

TxSignature::TxSignature(const PublicKey& pk, const Signature& sig) 
    : public_key(pk), signature(sig) {}

bool TxSignature::is_valid() const {
    // Basic validation - actual verification happens in state machine
    return std::any_of(public_key.begin(), public_key.end(), [](uint8_t b) { return b != 0; }) &&
           std::any_of(signature.begin(), signature.end(), [](uint8_t b) { return b != 0; });
}

Address TxSignature::signer_address() const {
    return crypto::Hash::public_key_to_address(public_key);
}

// ============================================================================
// TRANSACTION IMPLEMENTATION
// ============================================================================

Tx::Tx() : nonce(0), gas_limit(DEFAULT_GAS_LIMIT) {}

bool Tx::is_valid() const {
    // Basic validation
    if (chain_id.empty() || chain_id.length() > 64) return false;
    if (msgs.empty() || msgs.size() > MAX_MSGS_PER_TX) return false;
    if (!fee.is_valid()) return false;
    if (signatures.empty()) return false;
    if (memo.length() > MAX_MEMO_SIZE) return false;
    
    // Validate all messages
    for (const auto& msg : msgs) {
        if (!msg.is_valid()) return false;
    }
    
    // Validate all signatures
    for (const auto& sig : signatures) {
        if (!sig.is_valid()) return false;
    }
    
    return true;
}

uint32_t Tx::calculate_total_weight() const {
    uint32_t total = 0;
    
    // Base transaction weight
    total += 2000;
    
    // Message weights
    for (const auto& msg : msgs) {
        total += msg.calculate_weight();
    }
    
    // Signature weights
    total += static_cast<uint32_t>(signatures.size()) * 1000;
    
    // Memo weight
    total += static_cast<uint32_t>(memo.length()) * 10;
    
    return total;
}

Hash Tx::calculate_hash() const {
    // Encode transaction for hashing
    Bytes tx_bytes = encode_for_signing();
    return crypto::Hash::blake3(tx_bytes);
}

Bytes Tx::encode_for_signing() const {
    // Deterministic encoding for signing (without signatures)
    Bytes result;
    
    // Chain ID
    result.append_string(chain_id);
    
    // Nonce
    result.append_varint(nonce);
    
    // Messages
    result.append_varint(msgs.size());
    for (const auto& msg : msgs) {
        result.append_byte(static_cast<uint8_t>(msg.type));
        result.append_length_prefixed(msg.payload);
    }
    
    // Fee
    result.append_varint(fee.max_fee);
    result.append_varint(fee.tip);
    result.append_varint(fee.weight);
    
    // Gas limit
    result.append_varint(gas_limit);
    
    // Memo
    result.append_string(memo);
    
    return result;
}

Bytes Tx::encode_complete() const {
    // Complete encoding including signatures
    Bytes result = encode_for_signing();
    
    // Signatures
    result.append_varint(signatures.size());
    for (const auto& sig : signatures) {
        result.append(sig.public_key.data(), sig.public_key.size());
        result.append(sig.signature.data(), sig.signature.size());
    }
    
    return result;
}

std::vector<Address> Tx::get_signers() const {
    std::vector<Address> signers;
    signers.reserve(signatures.size());
    
    for (const auto& sig : signatures) {
        signers.push_back(sig.signer_address());
    }
    
    return signers;
}

bool Tx::has_signer(const Address& address) const {
    return std::any_of(signatures.begin(), signatures.end(),
        [&address](const TxSignature& sig) {
            return sig.signer_address() == address;
        });
}

std::string Tx::to_string() const {
    std::ostringstream oss;
    oss << "Tx{\n";
    oss << "  chain_id: " << chain_id << "\n";
    oss << "  nonce: " << nonce << "\n";
    oss << "  msgs: [";
    for (size_t i = 0; i < msgs.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << msgs[i].type_to_string();
    }
    oss << "]\n";
    oss << "  fee: " << fee.to_string() << "\n";
    oss << "  gas_limit: " << gas_limit << "\n";
    oss << "  memo: \"" << memo << "\"\n";
    oss << "  signatures: " << signatures.size() << " sigs\n";
    oss << "  hash: " << crypto::Hash::to_hex(calculate_hash()).substr(0, 16) << "...\n";
    oss << "}";
    return oss.str();
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

namespace {
    Bytes encode_transfer_msg(const Address& to, const Coin& amount) {
        Bytes result;
        result.append(to.data(), to.size());
        result.append_varint(amount.denom_id);
        result.append_varint(static_cast<uint64_t>(amount.amount));
        return result;
    }
    
    Bytes encode_multisend_msg(const std::vector<std::pair<Address, Coin>>& outputs) {
        Bytes result;
        result.append_varint(outputs.size());
        for (const auto& [addr, coin] : outputs) {
            result.append(addr.data(), addr.size());
            result.append_varint(coin.denom_id);
            result.append_varint(static_cast<uint64_t>(coin.amount));
        }
        return result;
    }
}

// ============================================================================
// TX BUILDER
// ============================================================================

TxBuilder::TxBuilder(const std::string& chain) {
    tx_.chain_id = chain;
    tx_.nonce = 0;
    tx_.gas_limit = DEFAULT_GAS_LIMIT;
    tx_.fee = Fee(1000000, 0, 1000); // Default fee: 0.001 USDTg
}

TxBuilder& TxBuilder::set_nonce(Nonce n) {
    tx_.nonce = n;
    return *this;
}

TxBuilder& TxBuilder::set_fee(const Fee& f) {
    tx_.fee = f;
    return *this;
}

TxBuilder& TxBuilder::set_gas_limit(uint64_t limit) {
    tx_.gas_limit = limit;
    return *this;
}

TxBuilder& TxBuilder::set_memo(const std::string& m) {
    tx_.memo = m;
    return *this;
}

TxBuilder& TxBuilder::add_transfer(const Address& to, const Coin& amount) {
    Bytes payload = encode_transfer_msg(to, amount);
    tx_.msgs.emplace_back(MSG_TRANSFER, payload);
    return *this;
}

TxBuilder& TxBuilder::add_multisend(const std::vector<std::pair<Address, Coin>>& outputs) {
    Bytes payload = encode_multisend_msg(outputs);
    tx_.msgs.emplace_back(MSG_MULTISEND, payload);
    return *this;
}

TxBuilder& TxBuilder::add_custom_msg(MsgType type, const Bytes& payload) {
    tx_.msgs.emplace_back(type, payload);
    return *this;
}

TxBuilder& TxBuilder::add_signature(const PublicKey& public_key, const Signature& signature) {
    tx_.signatures.emplace_back(public_key, signature);
    return *this;
}

Tx TxBuilder::build() const {
    // Update fee weight based on calculated weight
    Tx result = tx_;
    result.fee.weight = result.calculate_total_weight();
    return result;
}

// ============================================================================
// CONVENIENCE FUNCTIONS
// ============================================================================

Tx create_transfer_tx(const std::string& chain_id, Nonce nonce, 
                     const Address& from, const Address& to, 
                     const Coin& amount, const Fee& fee) {
    return TxBuilder(chain_id)
        .set_nonce(nonce)
        .set_fee(fee)
        .add_transfer(to, amount)
        .build();
}

Tx create_usdtg_transfer(const std::string& chain_id, Nonce nonce,
                        const Address& from, const Address& to,
                        Amount usdtg_amount, const Fee& fee) {
    Coin usdtg_coin(USDTG_DENOM_ID, usdtg_amount);
    return create_transfer_tx(chain_id, nonce, from, to, usdtg_coin, fee);
}

bool is_usdtg_transfer(const Tx& tx) {
    if (tx.msgs.size() != 1) return false;
    if (tx.msgs[0].type != MSG_TRANSFER) return false;
    
    // Would need to decode payload to check denom_id
    // For now, assume single transfer is USDTg
    return true;
}

Amount extract_usdtg_amount(const Tx& tx) {
    // Simplified - would need proper payload decoding
    return 0; // Placeholder
}

} // namespace usdtgverse::core

