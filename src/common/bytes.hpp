#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <span>
#include <algorithm>

namespace usdtgverse {

/**
 * High-performance byte manipulation utilities
 * Designed for zero-copy operations where possible
 */
class Bytes {
private:
    std::vector<uint8_t> data_;

public:
    // Constructors
    Bytes() = default;
    explicit Bytes(size_t size) : data_(size) {}
    Bytes(const std::vector<uint8_t>& data) : data_(data) {}
    Bytes(std::vector<uint8_t>&& data) : data_(std::move(data)) {}
    Bytes(const uint8_t* ptr, size_t size) : data_(ptr, ptr + size) {}
    Bytes(std::initializer_list<uint8_t> init) : data_(init) {}
    
    // From string
    explicit Bytes(const std::string& str) : data_(str.begin(), str.end()) {}
    
    // From hex string
    static Bytes from_hex(const std::string& hex);
    
    // Access
    uint8_t* data() { return data_.data(); }
    const uint8_t* data() const { return data_.data(); }
    size_t size() const { return data_.size(); }
    bool empty() const { return data_.empty(); }
    
    // Element access
    uint8_t& operator[](size_t index) { return data_[index]; }
    const uint8_t& operator[](size_t index) const { return data_[index]; }
    
    uint8_t& at(size_t index) { return data_.at(index); }
    const uint8_t& at(size_t index) const { return data_.at(index); }
    
    // Iterators
    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }
    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }
    
    // Modifiers
    void reserve(size_t capacity) { data_.reserve(capacity); }
    void resize(size_t size) { data_.resize(size); }
    void clear() { data_.clear(); }
    
    void push_back(uint8_t byte) { data_.push_back(byte); }
    void append(const uint8_t* ptr, size_t size) { data_.insert(data_.end(), ptr, ptr + size); }
    void append(const Bytes& other) { data_.insert(data_.end(), other.begin(), other.end()); }
    void append(const std::vector<uint8_t>& other) { data_.insert(data_.end(), other.begin(), other.end()); }
    
    // Subrange operations
    Bytes slice(size_t start, size_t length) const;
    Bytes slice(size_t start) const; // From start to end
    
    // Comparison
    bool operator==(const Bytes& other) const { return data_ == other.data_; }
    bool operator!=(const Bytes& other) const { return data_ != other.data_; }
    bool operator<(const Bytes& other) const { return data_ < other.data_; }
    
    // Conversion
    std::string to_hex() const;
    std::string to_string() const { return std::string(data_.begin(), data_.end()); }
    std::vector<uint8_t> to_vector() const { return data_; }
    std::span<const uint8_t> to_span() const { return std::span<const uint8_t>(data_); }
    
    // Utility
    void zero_fill() { std::fill(data_.begin(), data_.end(), 0); }
    bool is_zero() const { return std::all_of(data_.begin(), data_.end(), [](uint8_t b) { return b == 0; }); }
    
    // Secure operations
    void secure_zero();  // Secure memory wiping
    
    // Endianness
    void reverse() { std::reverse(data_.begin(), data_.end()); }
    Bytes reversed() const;
    
    // Hashing
    Hash sha256() const;
    Hash blake3() const;
};

// ============================================================================
// SPAN UTILITIES (C++20 style)
// ============================================================================

using ByteSpan = std::span<const uint8_t>;
using MutableByteSpan = std::span<uint8_t>;

// Convert between different types
inline ByteSpan to_span(const Bytes& bytes) {
    return ByteSpan(bytes.data(), bytes.size());
}

inline ByteSpan to_span(const std::vector<uint8_t>& vec) {
    return ByteSpan(vec.data(), vec.size());
}

inline ByteSpan to_span(const std::string& str) {
    return ByteSpan(reinterpret_cast<const uint8_t*>(str.data()), str.size());
}

template<size_t N>
inline ByteSpan to_span(const std::array<uint8_t, N>& arr) {
    return ByteSpan(arr.data(), arr.size());
}

// ============================================================================
// ENDIANNESS UTILITIES
// ============================================================================

namespace endian {

// Little endian encoding/decoding
inline uint16_t encode_le16(uint16_t value) {
    return value; // Assume little endian system
}

inline uint32_t encode_le32(uint32_t value) {
    return value;
}

inline uint64_t encode_le64(uint64_t value) {
    return value;
}

inline uint16_t decode_le16(const uint8_t* data) {
    return static_cast<uint16_t>(data[0]) | 
           (static_cast<uint16_t>(data[1]) << 8);
}

inline uint32_t decode_le32(const uint8_t* data) {
    return static_cast<uint32_t>(data[0]) |
           (static_cast<uint32_t>(data[1]) << 8) |
           (static_cast<uint32_t>(data[2]) << 16) |
           (static_cast<uint32_t>(data[3]) << 24);
}

inline uint64_t decode_le64(const uint8_t* data) {
    return static_cast<uint64_t>(data[0]) |
           (static_cast<uint64_t>(data[1]) << 8) |
           (static_cast<uint64_t>(data[2]) << 16) |
           (static_cast<uint64_t>(data[3]) << 24) |
           (static_cast<uint64_t>(data[4]) << 32) |
           (static_cast<uint64_t>(data[5]) << 40) |
           (static_cast<uint64_t>(data[6]) << 48) |
           (static_cast<uint64_t>(data[7]) << 56);
}

// Big endian encoding/decoding
uint16_t encode_be16(uint16_t value);
uint32_t encode_be32(uint32_t value);
uint64_t encode_be64(uint64_t value);

uint16_t decode_be16(const uint8_t* data);
uint32_t decode_be32(const uint8_t* data);
uint64_t decode_be64(const uint8_t* data);

} // namespace endian

// ============================================================================
// VARINT ENCODING (Protocol Buffers style)
// ============================================================================

namespace varint {

// Encode variable-length integers
size_t encode_uint64(uint64_t value, uint8_t* buffer);
size_t encode_int64(int64_t value, uint8_t* buffer);

// Decode variable-length integers
size_t decode_uint64(const uint8_t* data, size_t max_len, uint64_t& result);
size_t decode_int64(const uint8_t* data, size_t max_len, int64_t& result);

// Calculate encoded size
size_t encoded_size_uint64(uint64_t value);
size_t encoded_size_int64(int64_t value);

// Convenience functions
Bytes encode_varint(uint64_t value);
std::optional<uint64_t> decode_varint(const Bytes& data, size_t& offset);

} // namespace varint

} // namespace usdtgverse

