/*
==============================================
 File:        deploy_usdtg_native.js
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTg Native Coin Deployment Script
   
   USDTg Native Coin deployment script featuring:
   - Mainnet deployment automation
   - Token supply configuration
   - Quantum-safe stablecoin setup
   - Treasury and distribution management
   - Deployment verification

 License:
   MIT License
==============================================
*/

/**
 * USDTg Native Coin Deployment Script
 * ===================================
 * 
 * 🚀 DEPLOYS USDTg NATIVE COIN TO MAINNET
 * 💰 1 BILLION USDTg INITIAL SUPPLY
 * 🏛️ QUANTUM-SAFE STABLECOIN
 * 
 * Tokenomics:
 * - Treasury: 40% (400M USDTg)
 * - Public Distribution: 30% (300M USDTg)
 * - Development Fund: 20% (200M USDTg)
 * - Marketing & Partnerships: 10% (100M USDTg)
 */

const { ethers } = require("hardhat");
const fs = require("fs");
const path = require("path");

async function main() {
    console.log("🚀 Starting USDTg Native Coin Deployment...");
    
    // Get deployer account
    const [deployer] = await ethers.getSigners();
    console.log("📝 Deploying contracts with account:", deployer.address);
    console.log("💰 Account balance:", ethers.utils.formatEther(await deployer.getBalance()), "ETH");
    
    // Deployment configuration
    const deploymentConfig = {
        // Tokenomics
        initialSupply: ethers.utils.parseEther("1000000000"), // 1 Billion USDTg
        treasuryAllocation: ethers.utils.parseEther("400000000"), // 40%
        publicAllocation: ethers.utils.parseEther("300000000"), // 30%
        developmentAllocation: ethers.utils.parseEther("200000000"), // 20%
        marketingAllocation: ethers.utils.parseEther("100000000"), // 10%
        
        // Stablecoin mechanics
        targetPrice: ethers.utils.parseEther("1"), // $1.00 USD
        priceToleranceBps: 500, // 5% tolerance
        
        // Welcome bonus
        welcomeBonus: ethers.utils.parseEther("10"), // 10 USDTg
        
        // Gas settings
        gasLimit: 5000000,
        gasPrice: ethers.utils.parseUnits("20", "gwei")
    };
    
    console.log("⚙️ Deployment Configuration:");
    console.log("  Initial Supply:", ethers.utils.formatEther(deploymentConfig.initialSupply), "USDTg");
    console.log("  Treasury Allocation:", ethers.utils.formatEther(deploymentConfig.treasuryAllocation), "USDTg");
    console.log("  Public Allocation:", ethers.utils.formatEther(deploymentConfig.publicAllocation), "USDTg");
    console.log("  Development Allocation:", ethers.utils.formatEther(deploymentConfig.developmentAllocation), "USDTg");
    console.log("  Marketing Allocation:", ethers.utils.formatEther(deploymentConfig.marketingAllocation), "USDTg");
    
    try {
        // Deploy USDTg Native Coin
        console.log("\n📦 Deploying USDTg Native Coin...");
        const USDTgNativeCoin = await ethers.getContractFactory("USDTgNativeCoin");
        
        const usdtgCoin = await USDTgNativeCoin.deploy(
            deployer.address, // Treasury
            deployer.address, // Development
            deployer.address, // Marketing
            deploymentConfig.targetPrice,
            deploymentConfig.priceToleranceBps,
            deploymentConfig.welcomeBonus,
            {
                gasLimit: deploymentConfig.gasLimit,
                gasPrice: deploymentConfig.gasPrice
            }
        );
        
        await usdtgCoin.deployed();
        console.log("✅ USDTg Native Coin deployed to:", usdtgCoin.address);
        
        // Verify deployment
        console.log("\n🔍 Verifying deployment...");
        const name = await usdtgCoin.name();
        const symbol = await usdtgCoin.symbol();
        const decimals = await usdtgCoin.decimals();
        const totalSupply = await usdtgCoin.totalSupply();
        
        console.log("📊 Contract Details:");
        console.log("  Name:", name);
        console.log("  Symbol:", symbol);
        console.log("  Decimals:", decimals.toString());
        console.log("  Total Supply:", ethers.utils.formatEther(totalSupply), "USDTg");
        
        // Distribute tokens according to tokenomics
        console.log("\n💰 Distributing tokens according to tokenomics...");
        
        // Treasury allocation (40%)
        console.log("  📈 Allocating to Treasury (40%)...");
        await usdtgCoin.transfer(deployer.address, deploymentConfig.treasuryAllocation);
        
        // Public allocation (30%) - will be distributed later
        console.log("  🌍 Public allocation (30%) reserved for distribution");
        
        // Development allocation (20%)
        console.log("  🔧 Allocating to Development Fund (20%)...");
        await usdtgCoin.transfer(deployer.address, deploymentConfig.developmentAllocation);
        
        // Marketing allocation (10%)
        console.log("  📢 Allocating to Marketing Fund (10%)...");
        await usdtgCoin.transfer(deployer.address, deploymentConfig.marketingAllocation);
        
        // Verify distributions
        const deployerBalance = await usdtgCoin.balanceOf(deployer.address);
        console.log("  💰 Deployer balance:", ethers.utils.formatEther(deployerBalance), "USDTg");
        
        // Save deployment info
        const deploymentInfo = {
            network: "mainnet",
            contractAddress: usdtgCoin.address,
            deployer: deployer.address,
            deploymentTime: new Date().toISOString(),
            transactionHash: usdtgCoin.deployTransaction.hash,
            gasUsed: usdtgCoin.deployTransaction.gasLimit.toString(),
            tokenomics: {
                totalSupply: ethers.utils.formatEther(totalSupply),
                treasuryAllocation: ethers.utils.formatEther(deploymentConfig.treasuryAllocation),
                publicAllocation: ethers.utils.formatEther(deploymentConfig.publicAllocation),
                developmentAllocation: ethers.utils.formatEther(deploymentConfig.developmentAllocation),
                marketingAllocation: ethers.utils.formatEther(deploymentConfig.marketingAllocation)
            },
            contractDetails: {
                name: name,
                symbol: symbol,
                decimals: decimals.toString(),
                targetPrice: ethers.utils.formatEther(deploymentConfig.targetPrice),
                priceToleranceBps: deploymentConfig.priceToleranceBps,
                welcomeBonus: ethers.utils.formatEther(deploymentConfig.welcomeBonus)
            }
        };
        
        // Save to file
        const deploymentPath = path.join(__dirname, "../config/deployment_mainnet.json");
        fs.writeFileSync(deploymentPath, JSON.stringify(deploymentInfo, null, 2));
        console.log("💾 Deployment info saved to:", deploymentPath);
        
        // Generate verification command
        console.log("\n🔐 Contract Verification Command:");
        console.log(`npx hardhat verify --network mainnet ${usdtgCoin.address} "${deployer.address}" "${deployer.address}" "${deployer.address}" "${ethers.utils.formatEther(deploymentConfig.targetPrice)}" "${deploymentConfig.priceToleranceBps}" "${ethers.utils.formatEther(deploymentConfig.welcomeBonus)}"`);
        
        console.log("\n🎉 USDTg Native Coin Deployment Completed Successfully!");
        console.log("📊 Contract Address:", usdtgCoin.address);
        console.log("🌐 Network: Mainnet");
        console.log("💰 Total Supply: 1,000,000,000 USDTg");
        console.log("🏛️ Treasury: 400,000,000 USDTg (40%)");
        console.log("🌍 Public: 300,000,000 USDTg (30%)");
        console.log("🔧 Development: 200,000,000 USDTg (20%)");
        console.log("📢 Marketing: 100,000,000 USDTg (10%)");
        
    } catch (error) {
        console.error("❌ Deployment failed:", error);
        process.exit(1);
    }
}

main()
    .then(() => process.exit(0))
    .catch((error) => {
        console.error("❌ Script failed:", error);
        process.exit(1);
    });
