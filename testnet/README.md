# 🧪 USDTgVerse Testnet Environment
**World's First Quantum-Safe Blockchain Testnet**

## 🚀 Quick Start

### Prerequisites
- Docker 20.10+
- Docker Compose 2.0+
- 8GB+ RAM
- 50GB+ free disk space

### 🔧 Setup

1. **Clone and Configure**
   ```bash
   git clone https://github.com/usdtgverse/testnet
   cd testnet
   cp env.example .env
   # Edit .env with your configuration
   ```

2. **Start Testnet**
   ```bash
   docker-compose up -d
   ```

3. **Verify Services**
   ```bash
   docker-compose ps
   curl http://localhost:8545 -X POST -H "Content-Type: application/json" -d '{"jsonrpc":"2.0","method":"eth_blockNumber","params":[],"id":1}'
   ```

## 🌐 Services

| Service | Port | URL | Description |
|---------|------|-----|-------------|
| **Testnet Node** | 8545 | http://localhost:8545 | RPC Endpoint |
| **WebSocket** | 8546 | ws://localhost:8546 | WebSocket RPC |
| **Explorer** | 3000 | http://localhost:3000 | Block Explorer |
| **Faucet** | 3001 | http://localhost:3001 | Test Token Faucet |
| **Deployment** | 8080 | http://localhost:8080 | Contract Deployment |
| **Audit System** | 8081 | http://localhost:8081 | Smart Contract Auditing |
| **Grafana** | 3002 | http://localhost:3002 | Monitoring Dashboard |
| **Prometheus** | 9090 | http://localhost:9090 | Metrics Collection |

## ⚛️ Quantum-Safe Features

### 🔐 Cryptographic Security
- **256-bit quantum security** (equivalent to AES-1024)
- **CRYSTALS-Dilithium** signatures
- **CRYSTALS-Kyber** key exchange
- **Post-quantum address format**: `USDTg1q...`
- **Quantum-safe mnemonics**: 24-48 words

### 🌍 Network Configuration
- **Chain ID**: 2025
- **Network ID**: 2025
- **Block Time**: 1 second
- **Gas Limit**: 8,000,000
- **Native Currency**: tUSDTg (Testnet USDTg)

## 🎁 Testnet Faucet

### Get Test Tokens
```bash
curl -X POST http://localhost:3001/api/faucet \
  -H "Content-Type: application/json" \
  -d '{"address":"YOUR_WALLET_ADDRESS"}'
```

### Faucet Limits
- **Amount**: 1,000 tUSDTg per request
- **Rate Limit**: 1 request per address per day
- **Quantum Verification**: Required for addresses

## 🔧 Development Tools

### RPC Configuration
```javascript
// MetaMask/Web3 Configuration
const rpcUrl = 'http://localhost:8545';
const chainId = 2025;
const networkName = 'USDTgVerse Testnet';
const currencySymbol = 'tUSDTg';
const blockExplorerUrl = 'http://localhost:3000';
```

### Smart Contract Deployment
```bash
# Using Hardhat
npx hardhat run scripts/deploy.js --network usdtgverse-testnet

# Using Foundry
forge create --rpc-url http://localhost:8545 --private-key $PRIVATE_KEY src/Contract.sol:Contract
```

### SDK Integration
```c
// C SDK
#include "usdtgverse.h"

usdtg_init(USDTG_NETWORK_TESTNET, "http://localhost:8545");
```

```javascript
// JavaScript SDK
import { USDTgVerse } from 'usdtgverse-sdk';

const provider = new USDTgVerse.Provider('http://localhost:8545');
```

## 📊 Monitoring & Analytics

### Grafana Dashboards
- **Blockchain Metrics**: Block time, gas usage, transaction volume
- **Node Performance**: CPU, memory, disk usage
- **Network Health**: Peer connections, sync status
- **Security Metrics**: Quantum signature verification, attack detection

### Prometheus Metrics
```bash
# Query blockchain metrics
curl 'http://localhost:9090/api/v1/query?query=blockchain_block_height'

# Query transaction metrics
curl 'http://localhost:9090/api/v1/query?query=blockchain_transactions_total'
```

## 🔍 Smart Contract Auditing

### Automatic Audit
```bash
curl -X POST http://localhost:8081/api/v1/audits \
  -H "Content-Type: application/json" \
  -d '{
    "contract_id": "CONTRACT_ID",
    "audit_type": "automated",
    "title": "Security Audit"
  }'
```

### Audit Features
- **Quantum-safe analysis**
- **Vulnerability detection**
- **Gas optimization**
- **Code quality assessment**
- **Automated reporting**

## 🔄 State Management

### Reset Testnet State
```bash
docker-compose down -v
docker-compose up -d
```

### Backup State
```bash
docker exec usdtgverse-testnet-node /app/backup.sh
```

### Restore State
```bash
docker exec usdtgverse-testnet-node /app/restore.sh backup-file.tar.gz
```

## 🧪 Testing Scenarios

### Load Testing
```bash
# Generate transaction load
docker run --network testnet_usdtgverse-testnet \
  usdtgverse/load-tester:latest \
  --rpc-url http://usdtgverse-testnet:8545 \
  --transactions 1000 \
  --concurrent 10
```

### Quantum Security Testing
```bash
# Test quantum-safe features
docker run --network testnet_usdtgverse-testnet \
  usdtgverse/quantum-tester:latest \
  --rpc-url http://usdtgverse-testnet:8545 \
  --test-suite quantum-security
```

## 🔧 Configuration

### Environment Variables
```bash
# Core Configuration
NETWORK_ID=2025
CHAIN_ID=2025
BLOCK_TIME=1s
GAS_LIMIT=8000000

# Quantum Features
QUANTUM_SAFE=true
POST_QUANTUM_ENABLED=true
DILITHIUM_MODE=3
KYBER_SECURITY_LEVEL=3

# Faucet Configuration
FAUCET_AMOUNT=1000000000000000000000  # 1000 tUSDTg
FAUCET_RATE_LIMIT=1
FAUCET_COOLDOWN=86400  # 24 hours
```

### Genesis Configuration
- **Pre-funded accounts** with test tokens
- **Quantum-safe parameters** enabled
- **Governance contracts** pre-deployed
- **Faucet contract** with 1B tUSDTg supply

## 📚 API Documentation

### JSON-RPC Methods
```bash
# Standard Ethereum methods
eth_blockNumber, eth_getBalance, eth_sendTransaction

# USDTgVerse extensions
usdtg_getQuantumSecurity, usdtg_verifyQuantumSignature
```

### REST API Endpoints
```bash
# Explorer API
GET /api/v1/blocks
GET /api/v1/transactions
GET /api/v1/addresses/{address}

# Faucet API
POST /api/faucet
GET /api/faucet/status

# Audit API
POST /api/v1/audits
GET /api/v1/audits/{id}
```

## 🚨 Troubleshooting

### Common Issues

**Service won't start**
```bash
# Check logs
docker-compose logs usdtgverse-testnet

# Restart service
docker-compose restart usdtgverse-testnet
```

**RPC connection failed**
```bash
# Check if node is synced
curl -X POST http://localhost:8545 \
  -H "Content-Type: application/json" \
  -d '{"jsonrpc":"2.0","method":"eth_syncing","params":[],"id":1}'
```

**Faucet not working**
```bash
# Check faucet balance
curl http://localhost:3001/api/faucet/status

# Check rate limits
docker-compose logs faucet
```

### Performance Optimization
```bash
# Increase resources
export COMPOSE_DOCKER_CLI_BUILD=1
export DOCKER_BUILDKIT=1

# Optimize database
docker exec usdtgverse-postgres psql -U postgres -d usdtgverse_testnet -c "VACUUM ANALYZE;"
```

## 🔒 Security Considerations

### Production Deployment
- Change all default passwords
- Enable SSL/TLS certificates
- Configure firewall rules
- Set up monitoring alerts
- Enable backup automation

### Quantum Security
- All signatures use post-quantum algorithms
- Addresses are quantum-resistant
- Private keys use 256-bit entropy
- Mnemonics require 24+ words

## 🤝 Contributing

### Development Workflow
1. Fork the repository
2. Create feature branch
3. Run tests: `make test`
4. Submit pull request

### Testing
```bash
# Run all tests
make test

# Run specific test suite
make test-blockchain
make test-quantum
make test-api
```

## 📄 License

MIT License - see [LICENSE](LICENSE) file for details.

## 🆘 Support

- **Documentation**: https://docs.usdtgverse.com
- **Discord**: https://discord.gg/usdtgverse
- **GitHub Issues**: https://github.com/usdtgverse/testnet/issues
- **Email**: testnet@usdtgverse.com

---

**🚀 Ready to build the quantum-safe future!** ⚛️
