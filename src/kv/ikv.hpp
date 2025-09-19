#pragma once

#include "common/bytes.hpp"
#include <string>
#include <vector>
#include <optional>
#include <memory>

namespace usdtgverse::kv {

/**
 * IKV - Key-Value Store Interface
 * 
 * Abstract interface for key-value storage backends.
 * Supports multiple implementations:
 * - MemoryKV: In-memory storage (for testing)
 * - RocksDB: Production-grade persistent storage
 * - MMAP: Memory-mapped file storage
 */

class IKV {
public:
    virtual ~IKV() = default;
    
    // ========================================================================
    // BASIC OPERATIONS
    // ========================================================================
    
    // Get value by key
    virtual bool get(const std::string& key, std::string& value) const = 0;
    virtual std::optional<std::string> get(const std::string& key) const = 0;
    
    // Put key-value pair
    virtual bool put(const std::string& key, const std::string& value) = 0;
    
    // Delete key
    virtual bool del(const std::string& key) = 0;
    
    // Check if key exists
    virtual bool exists(const std::string& key) const = 0;
    
    // ========================================================================
    // BATCH OPERATIONS
    // ========================================================================
    
    // Batch write operations
    virtual bool batch_put(const std::vector<std::pair<std::string, std::string>>& pairs) = 0;
    virtual bool batch_del(const std::vector<std::string>& keys) = 0;
    
    // ========================================================================
    // ITERATION
    // ========================================================================
    
    // Iterator interface
    class Iterator {
    public:
        virtual ~Iterator() = default;
        virtual bool valid() const = 0;
        virtual void next() = 0;
        virtual std::string key() const = 0;
        virtual std::string value() const = 0;
    };
    
    // Create iterator
    virtual std::unique_ptr<Iterator> iterator() const = 0;
    virtual std::unique_ptr<Iterator> iterator(const std::string& prefix) const = 0;
    
    // Get all keys with prefix
    virtual std::vector<std::string> get_keys_with_prefix(const std::string& prefix) const = 0;
    
    // ========================================================================
    // TRANSACTIONS
    // ========================================================================
    
    // Transaction interface for atomic operations
    class Transaction {
    public:
        virtual ~Transaction() = default;
        virtual bool put(const std::string& key, const std::string& value) = 0;
        virtual bool del(const std::string& key) = 0;
        virtual bool commit() = 0;
        virtual bool rollback() = 0;
    };
    
    // Begin transaction
    virtual std::unique_ptr<Transaction> begin_transaction() = 0;
    
    // ========================================================================
    // STATISTICS & MANAGEMENT
    // ========================================================================
    
    // Get statistics
    virtual size_t size() const = 0;
    virtual size_t memory_usage() const = 0;
    virtual std::string get_stats() const = 0;
    
    // Management operations
    virtual bool sync() = 0;  // Force sync to disk
    virtual bool compact() = 0;  // Compact storage
    virtual bool backup(const std::string& backup_path) = 0;
    virtual bool restore(const std::string& backup_path) = 0;
    
    // ========================================================================
    // CONVENIENCE METHODS
    // ========================================================================
    
    // Typed operations
    template<typename T>
    bool get_typed(const std::string& key, T& value) const {
        std::string data;
        if (!get(key, data)) {
            return false;
        }
        // In real implementation, would use proper serialization
        // For now, just basic conversion
        return true;
    }
    
    template<typename T>
    bool put_typed(const std::string& key, const T& value) {
        // In real implementation, would use proper serialization
        // For now, just basic conversion
        return true;
    }
};

// ============================================================================
// MEMORY KV IMPLEMENTATION
// ============================================================================

class MemoryKV : public IKV {
private:
    mutable std::unordered_map<std::string, std::string> data_;
    mutable std::mutex mutex_;
    
    class MemoryIterator : public Iterator {
    private:
        std::unordered_map<std::string, std::string>::const_iterator it_;
        std::unordered_map<std::string, std::string>::const_iterator end_;
        std::string prefix_;
        
    public:
        MemoryIterator(const std::unordered_map<std::string, std::string>& data, const std::string& prefix = "");
        bool valid() const override;
        void next() override;
        std::string key() const override;
        std::string value() const override;
    };
    
    class MemoryTransaction : public Transaction {
    private:
        MemoryKV* kv_;
        std::unordered_map<std::string, std::string> pending_puts_;
        std::unordered_set<std::string> pending_deletes_;
        bool committed_;
        
    public:
        explicit MemoryTransaction(MemoryKV* kv);
        bool put(const std::string& key, const std::string& value) override;
        bool del(const std::string& key) override;
        bool commit() override;
        bool rollback() override;
    };
    
public:
    MemoryKV() = default;
    ~MemoryKV() = default;
    
    // Basic operations
    bool get(const std::string& key, std::string& value) const override;
    std::optional<std::string> get(const std::string& key) const override;
    bool put(const std::string& key, const std::string& value) override;
    bool del(const std::string& key) override;
    bool exists(const std::string& key) const override;
    
    // Batch operations
    bool batch_put(const std::vector<std::pair<std::string, std::string>>& pairs) override;
    bool batch_del(const std::vector<std::string>& keys) override;
    
    // Iteration
    std::unique_ptr<Iterator> iterator() const override;
    std::unique_ptr<Iterator> iterator(const std::string& prefix) const override;
    std::vector<std::string> get_keys_with_prefix(const std::string& prefix) const override;
    
    // Transactions
    std::unique_ptr<Transaction> begin_transaction() override;
    
    // Statistics
    size_t size() const override;
    size_t memory_usage() const override;
    std::string get_stats() const override;
    
    // Management
    bool sync() override { return true; } // No-op for memory
    bool compact() override { return true; } // No-op for memory
    bool backup(const std::string& backup_path) override;
    bool restore(const std::string& backup_path) override;
    
    // Testing helpers
    void clear();
    std::unordered_map<std::string, std::string> get_all_data() const;
};

// ============================================================================
// FACTORY FUNCTIONS
// ============================================================================

// Create KV store instances
std::shared_ptr<IKV> create_memory_kv();
std::shared_ptr<IKV> create_rocksdb_kv(const std::string& db_path);
std::shared_ptr<IKV> create_mmap_kv(const std::string& file_path);

// Utility functions
std::string escape_key(const std::string& key);
std::string unescape_key(const std::string& escaped_key);
std::string join_keys(const std::vector<std::string>& parts);
std::vector<std::string> split_key(const std::string& key, char delimiter = '/');

} // namespace usdtgverse::kv

