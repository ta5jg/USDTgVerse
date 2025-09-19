# 🌌 USDTgVerse - Enterprise Native C++ Blockchain

## 🔥 The Most Advanced Independent Blockchain Architecture

**USDTgVerse** is a professional-grade, enterprise-level blockchain built with **pure C++20** and **zero framework dependencies**. Featuring native USDTg stablecoin, HotStuff BFT-PoS consensus, and OdixPay++ payment system.

## 🏗️ Architecture Overview

```
USDTgVerse Enterprise Stack
├── 🧱 Common Layer
│   ├── Bytes manipulation (zero-copy)
│   ├── Varint encoding (Protocol Buffers style)
│   ├── Endianness utilities
│   └── High-performance logging
├── 🔐 Cryptography Layer
│   ├── BLAKE3 (ultra-fast hashing)
│   ├── SHA-256 (compatibility)
│   ├── Ed25519 (default signatures)
│   ├── secp256k1 (Bitcoin compatibility)
│   └── Noise IK (P2P handshake)
├── 📦 Codec Layer (USDTg-Bin)
│   ├── Deterministic serialization
│   ├── Varint + length-prefixed
│   ├── Zero floating point
│   └── Canonical field ordering
├── 🗄️ Storage Layer
│   ├── IKV interface abstraction
│   ├── RocksDB backend (production)
│   ├── Memory backend (testing)
│   └── MMAP log backend (archival)
├── 🌳 Merkle Layer
│   ├── Sparse Merkle Tree (SMT)
│   ├── Merkle proofs
│   └── State commitments
├── ⛓️ Core Blockchain
│   ├── Multi-asset transactions
│   ├── Account model (nonce + balances)
│   ├── Block structure + commitments
│   ├── State machine (deterministic)
│   └── Mempool + fee market
├── 🤝 Consensus (HotStuff BFT-PoS)
│   ├── Proposer rotation
│   ├── Quorum Certificates (QC)
│   ├── Safety + liveness rules
│   ├── Validator staking
│   └── Slashing conditions
├── 🌐 P2P Networking
│   ├── Noise encrypted transport
│   ├── Gossip protocol
│   ├── Peer reputation
│   └── Anti-flood protection
├── 💳 OdixPay++ (Native)
│   ├── Invoice system
│   ├── Escrow contracts
│   ├── Subscription payments
│   ├── Batch transactions
│   ├── Spending limits
│   └── Fee sponsorship
└── 🛠️ Tools & SDK
    ├── Key management (keytool)
    ├── Genesis generator
    ├── USDTgWallet C++ SDK
    └── CLI utilities
```

## 💎 Native USDTg Token

- **🪙 Native Coin**: No contract address, blockchain's own currency
- **🎯 1 USD = 1 USDTg**: Algorithmic stability mechanism
- **♾️ Unlimited Supply**: Mint/burn based on economic demand
- **⚡ Microsecond TXs**: Native C++ performance
- **🌐 Multi-Chain**: Bridge to ETH, TRON, SOLANA

## 🚀 Performance Targets

| Metric | Target | Achieved |
|--------|--------|----------|
| **Transaction Speed** | 100,000+ TPS | ⚡ |
| **Block Time** | 1 second | ✅ |
| **Finality** | 3 seconds | ✅ |
| **Memory Usage** | <100MB full node | 💾 |
| **CPU Usage** | <5% modern hardware | 🔥 |
| **Network Latency** | <10ms between nodes | 🌐 |

## 🛠️ Build Requirements

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt update
sudo apt install build-essential cmake pkg-config
sudo apt install libssl-dev libsodium-dev libsecp256k1-dev
sudo apt install librocksdb-dev libgtest-dev

# macOS
brew install cmake openssl libsodium secp256k1
brew install rocksdb googletest
```

### Build Process
```bash
# Clone repository
git clone https://github.com/USDTgVerse/USDTgVerse.git
cd USDTgVerse

# Configure build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build with maximum parallelism
make -j$(nproc)

# Run tests
make test

# Install system-wide
sudo make install
```

### Advanced Build Options
```bash
# Performance build (maximum optimization)
cmake -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_FLAGS="-O3 -march=native -flto" \
      -DUSDTGVERSE_USE_ROCKSDB=ON \
      -DUSDTGVERSE_USE_LIBSODIUM=ON \
      ..

# Debug build (with sanitizers)
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_FLAGS="-fsanitize=address,undefined" \
      -DUSDTGVERSE_BUILD_TESTS=ON \
      -DUSDTGVERSE_ENABLE_FUZZING=ON \
      ..

# Minimal build (no external dependencies)
cmake -DCMAKE_BUILD_TYPE=Release \
      -DUSDTGVERSE_USE_ROCKSDB=OFF \
      -DUSDTGVERSE_USE_LIBSODIUM=OFF \
      -DUSDTGVERSE_USE_SECP256K1=OFF \
      ..
```

## 🎯 Usage

### Start Validator Node
```bash
# Generate validator keys
usdtg-keytool generate --type validator --output validator.key

# Create genesis (first time only)
usdtg-genesis create --validators validator1.pub,validator2.pub,validator3.pub

# Start validator
./usdtgverse --config validator.toml --role validator
```

### Start RPC Node
```bash
./usdtgverse --config rpc.toml --role rpc --rpc-port 26657
```

### CLI Operations
```bash
# Check balance
usdtg-keytool balance --address usdtg1abc...

# Send transaction
usdtg-keytool send --from alice.key --to usdtg1def... --amount 100.50

# Create invoice (OdixPay++)
usdtg-keytool invoice --merchant merchant.key --amount 50.00 --due-days 7

# Pay invoice
usdtg-keytool pay-invoice --payer payer.key --invoice-id 0x123...
```

## 💳 OdixPay++ Features

### Native Payment Messages
- **📄 Invoices**: Create, pay, track invoices on-chain
- **🔒 Escrow**: Trustless escrow with optional arbitration
- **🔄 Subscriptions**: Recurring payment automation
- **📦 Batch Payments**: Multiple payments in single transaction
- **💰 Spending Limits**: Daily/monthly spending controls
- **🎫 Fee Sponsorship**: Merchants can pay user fees

### Example Usage
```cpp
// Create invoice
TxBuilder builder(MAINNET_CHAIN_ID);
builder.invoice(merchant_addr, customer_addr, 
               Coin{USDTG_DENOM_ID, double_to_amount(99.99)}, 
               current_timestamp_ms() + 7 * 24 * 60 * 60 * 1000); // 7 days

// Set up escrow
builder.escrow_open(buyer_addr, seller_addr,
                   Coin{USDTG_DENOM_ID, double_to_amount(500.00)},
                   current_timestamp_ms() + 30 * 24 * 60 * 60 * 1000); // 30 days

auto tx = builder.build();
```

## 🧪 Testing

### Unit Tests
```bash
# Run all tests
./build/usdtgverse_tests

# Run specific test suite
./build/usdtgverse_tests --gtest_filter="TxTest.*"
```

### Fuzzing (Clang only)
```bash
# Build with fuzzing
cmake -DUSDTGVERSE_ENABLE_FUZZING=ON ..
make

# Run fuzzers
./build/fuzz_tx_decode corpus/
./build/fuzz_consensus_msg corpus/
```

### Integration Tests
```bash
# Start local testnet
./scripts/start_testnet.sh

# Run integration tests
./tests/integration/run_tests.sh
```

## 📊 Monitoring

### Metrics Endpoint
```bash
curl http://localhost:26657/metrics
```

### Key Metrics
- `usdtgverse_blocks_total`: Total blocks mined
- `usdtgverse_transactions_total`: Total transactions processed
- `usdtgverse_usdtg_supply`: Current USDTg supply
- `usdtgverse_usdtg_price`: Current USDTg price
- `usdtgverse_consensus_rounds`: Consensus rounds
- `usdtgverse_p2p_peers`: Connected peers

## 🔒 Security

### Cryptographic Guarantees
- **Ed25519**: 128-bit security level
- **BLAKE3**: Cryptographically secure hashing
- **Noise IK**: Perfect forward secrecy
- **BFT Consensus**: Byzantine fault tolerance

### Audit Trail
- **Deterministic Execution**: Same input → same output
- **Cryptographic Commitments**: All state changes provable
- **Replay Protection**: Nonce-based transaction ordering
- **Slashing**: Economic penalties for misbehavior

## 🌐 Network Information

### Mainnet
- **Chain ID**: `usdtgverse-1`
- **Native Token**: USDTg
- **Consensus**: HotStuff BFT-PoS
- **Block Time**: 1 second
- **Finality**: 3 seconds

### Testnet
- **Chain ID**: `usdtgverse-testnet-1`
- **Faucet**: https://faucet.usdtgverse.com
- **Explorer**: https://explorer-testnet.usdtgverse.com

## 📚 Documentation

- **[Technical Whitepaper](docs/whitepaper.pdf)**: Detailed architecture
- **[API Reference](docs/api.md)**: RPC API documentation
- **[Developer Guide](docs/developer.md)**: Building on USDTgVerse
- **[Validator Guide](docs/validator.md)**: Running a validator
- **[OdixPay++ Spec](docs/odixpay.md)**: Payment system specification

## 🤝 Contributing

We welcome contributions! Please read our [Contributing Guide](CONTRIBUTING.md).

### Development Setup
```bash
# Install development tools
./scripts/install_dev_tools.sh

# Format code
make format

# Run linter
make lint

# Run full test suite
make test-all
```

## 📄 License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

---

**🌌 USDTgVerse: Where Independence Meets Enterprise Performance**

*Built with ❤️ and pure C++ for maximum speed, security, and independence*