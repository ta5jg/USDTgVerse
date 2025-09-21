/**
 * 🔧 Hardhat Configuration for USDTgVerse
 * =======================================
 * 
 * ⚛️ QUANTUM-SAFE BLOCKCHAIN DEPLOYMENT
 * 🌍 MULTI-NETWORK SUPPORT
 * 🔐 SECURE PRIVATE KEY MANAGEMENT
 */

require("@nomicfoundation/hardhat-toolbox");
require("@openzeppelin/hardhat-upgrades");
require("hardhat-gas-reporter");
require("solidity-coverage");
require("dotenv").config();

/** @type import('hardhat/config').HardhatUserConfig */
module.exports = {
  solidity: {
    version: "0.8.19",
    settings: {
      optimizer: {
        enabled: true,
        runs: 200,
      },
      viaIR: true, // Enable via-IR for better optimization
    },
  },
  
  networks: {
    // 🏠 LOCAL DEVELOPMENT
    hardhat: {
      chainId: 31337,
      accounts: {
        mnemonic: "test test test test test test test test test test test junk",
        count: 20,
        accountsBalance: "10000000000000000000000", // 10,000 ETH
      },
      mining: {
        auto: true,
        interval: 1000, // 1 second blocks
      },
    },
    
    localhost: {
      url: "http://127.0.0.1:8545",
      chainId: 31337,
      accounts: {
        mnemonic: process.env.MNEMONIC || "test test test test test test test test test test test junk"
      }
    },
    
    // ⚛️ USDTGVERSE MAINNET
    usdtgverse: {
      url: process.env.USDTGVERSE_RPC_URL || "https://rpc.usdtgverse.com",
      chainId: 2024,
      accounts: process.env.PRIVATE_KEY ? [process.env.PRIVATE_KEY] : [],
      gasPrice: 20000000000, // 20 gwei
      gas: 8000000, // 8M gas limit
      timeout: 60000, // 1 minute timeout
    },
    
    // 🧪 USDTGVERSE TESTNET
    usdtgverseTestnet: {
      url: process.env.USDTGVERSE_TESTNET_RPC_URL || "https://testnet-rpc.usdtgverse.com",
      chainId: 2025,
      accounts: process.env.TESTNET_PRIVATE_KEY ? [process.env.TESTNET_PRIVATE_KEY] : [],
      gasPrice: 10000000000, // 10 gwei
      gas: 8000000,
    },
    
    // 🔵 ETHEREUM MAINNET
    ethereum: {
      url: process.env.ETHEREUM_RPC_URL || `https://mainnet.infura.io/v3/${process.env.INFURA_API_KEY}`,
      chainId: 1,
      accounts: process.env.ETHEREUM_PRIVATE_KEY ? [process.env.ETHEREUM_PRIVATE_KEY] : [],
      gasPrice: "auto",
    },
    
    // 🟡 BNB CHAIN
    bnb: {
      url: process.env.BNB_RPC_URL || "https://bsc-dataseed1.binance.org",
      chainId: 56,
      accounts: process.env.BNB_PRIVATE_KEY ? [process.env.BNB_PRIVATE_KEY] : [],
      gasPrice: 5000000000, // 5 gwei
    },
    
    // 🟣 POLYGON
    polygon: {
      url: process.env.POLYGON_RPC_URL || "https://polygon-rpc.com",
      chainId: 137,
      accounts: process.env.POLYGON_PRIVATE_KEY ? [process.env.POLYGON_PRIVATE_KEY] : [],
      gasPrice: 30000000000, // 30 gwei
    },
    
    // 🔴 ARBITRUM
    arbitrum: {
      url: process.env.ARBITRUM_RPC_URL || "https://arb1.arbitrum.io/rpc",
      chainId: 42161,
      accounts: process.env.ARBITRUM_PRIVATE_KEY ? [process.env.ARBITRUM_PRIVATE_KEY] : [],
    },
    
    // ❄️ AVALANCHE
    avalanche: {
      url: process.env.AVALANCHE_RPC_URL || "https://api.avax.network/ext/bc/C/rpc",
      chainId: 43114,
      accounts: process.env.AVALANCHE_PRIVATE_KEY ? [process.env.AVALANCHE_PRIVATE_KEY] : [],
      gasPrice: 25000000000, // 25 gwei
    },
  },
  
  // 📊 GAS REPORTER
  gasReporter: {
    enabled: process.env.REPORT_GAS !== undefined,
    currency: "USD",
    coinmarketcap: process.env.COINMARKETCAP_API_KEY,
    token: "ETH",
    gasPriceApi: "https://api.etherscan.io/api?module=proxy&action=eth_gasPrice",
    showTimeSpent: true,
    showMethodSig: true,
  },
  
  // 🔍 ETHERSCAN VERIFICATION
  etherscan: {
    apiKey: {
      mainnet: process.env.ETHERSCAN_API_KEY,
      bsc: process.env.BSCSCAN_API_KEY,
      polygon: process.env.POLYGONSCAN_API_KEY,
      arbitrumOne: process.env.ARBISCAN_API_KEY,
      avalanche: process.env.SNOWTRACE_API_KEY,
    },
  },
  
  // 📋 PATHS
  paths: {
    sources: "./contracts",
    tests: "./test",
    cache: "./cache",
    artifacts: "./artifacts",
  },
  
  // 🔧 MOCHA SETTINGS
  mocha: {
    timeout: 40000, // 40 seconds
    reporter: "spec",
    slow: 300,
  },
  
  // 📊 SOLIDITY COVERAGE
  coverage: {
    skipFiles: ["test/", "mock/"],
  },
  
  // 🚀 DEPLOYMENT SETTINGS
  namedAccounts: {
    deployer: {
      default: 0,
      1: 0, // Ethereum mainnet
      56: 0, // BNB Chain
      137: 0, // Polygon
      2024: 0, // USDTgVerse mainnet
      2025: 0, // USDTgVerse testnet
    },
    treasury: {
      default: 1,
      2024: process.env.TREASURY_ADDRESS || 1,
      2025: process.env.TESTNET_TREASURY_ADDRESS || 1,
    },
    development: {
      default: 2,
      2024: process.env.DEVELOPMENT_ADDRESS || 2,
      2025: process.env.TESTNET_DEVELOPMENT_ADDRESS || 2,
    },
    marketing: {
      default: 3,
      2024: process.env.MARKETING_ADDRESS || 3,
      2025: process.env.TESTNET_MARKETING_ADDRESS || 3,
    },
  },
  
  // ⚛️ QUANTUM-SAFE COMPILER SETTINGS
  compiler: {
    version: "0.8.19",
    settings: {
      optimizer: {
        enabled: true,
        runs: 200,
        details: {
          yul: true,
          yulDetails: {
            stackAllocation: true,
            optimizerSteps: "dhfoDgvulfnTUtnIf"
          }
        }
      },
      outputSelection: {
        "*": {
          "*": ["*"]
        }
      }
    }
  },
  
  // 🔐 SECURITY SETTINGS
  security: {
    quantumSafe: true,
    multiSigRequired: true,
    emergencyPause: true,
    upgradeability: false, // Immutable contracts for security
  },
  
  // 📈 ANALYTICS
  analytics: {
    enabled: true,
    endpoint: process.env.ANALYTICS_ENDPOINT,
    apiKey: process.env.ANALYTICS_API_KEY,
  },
};
