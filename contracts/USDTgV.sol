// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

/**
 * ⚡ USDTgV - USDTgVerse Utility Token
 * ===================================
 * 
 * ⚛️ QUANTUM-SAFE UTILITY TOKEN
 * 🚀 OPTIMIZED FOR USDTGVM
 * 🌍 ECOSYSTEM UTILITY TOKEN
 * 💰 NON-STABLE TOKEN (Volatile)
 * 
 * Purpose: General ecosystem utility, staking, rewards, fees
 * Supply: 500,000,000 USDTgV (500 Million)
 * Price Target: $0.50 - $2.00 USDTg
 */

import "@openzeppelin/contracts/token/ERC20/ERC20.sol";
import "@openzeppelin/contracts/access/Ownable.sol";
import "@openzeppelin/contracts/security/ReentrancyGuard.sol";
import "@openzeppelin/contracts/security/Pausable.sol";

contract USDTgV is ERC20, Ownable, ReentrancyGuard, Pausable {
    
    // 📊 TOKEN METADATA
    uint8 private constant DECIMALS = 18;
    uint256 public constant INITIAL_SUPPLY = 500000000 * 10**18; // 500M USDTgV
    
    // 🏛️ GOVERNANCE
    address public treasury;
    address public development;
    address public marketing;
    
    // 💎 UTILITY FEATURES
    mapping(address => uint256) public stakingBalances;
    mapping(address => uint256) public stakingRewards;
    mapping(address => uint256) public lastStakeTime;
    uint256 public stakingRewardRate = 10; // 10% APY
    
    // 🔄 ECOSYSTEM INTEGRATION
    mapping(address => bool) public ecosystemPartners;
    mapping(string => uint256) public feeDiscounts;
    uint256 public baseFee = 100; // 1% base fee
    
    // 📊 ANALYTICS
    uint256 public totalTransactions;
    mapping(address => uint256) public userTransactionCount;
    uint256 public totalMinted;
    uint256 public totalBurned;
    uint256 public totalStaked;
    
    // 🛡️ ANTI-FRAUD MEASURES
    mapping(address => bool) public blacklistedAddresses;
    mapping(address => uint256) public dailyTransferLimits;
    mapping(address => uint256) public lastTransferDate;
    uint256 public maxDailyTransferLimit = 50_000 * 10**18; // 50K USDTgV per day
    
    // 🚨 EVENTS
    event Staked(address indexed user, uint256 amount);
    event Unstaked(address indexed user, uint256 amount);
    event RewardsClaimed(address indexed user, uint256 amount);
    event EcosystemPartnerAdded(address indexed partner);
    event FeeDiscountUpdated(string indexed service, uint256 discount);
    event BlacklistUpdated(address indexed account, bool isBlacklisted);
    
    // 🔒 MODIFIERS
    modifier onlyTreasury() {
        require(msg.sender == treasury, "Not treasury");
        _;
    }
    
    modifier validAddress(address addr) {
        require(addr != address(0), "Invalid address");
        _;
    }
    
    modifier notBlacklisted(address addr) {
        require(!blacklistedAddresses[addr], "Address is blacklisted");
        _;
    }
    
    // 🏗️ CONSTRUCTOR
    constructor(
        address _treasury,
        address _development,
        address _marketing
    ) ERC20("USDTgVerse Utility Token", "USDTgV") {
        // Set addresses
        treasury = _treasury;
        development = _development;
        marketing = _marketing;
        
        // Token distribution
        uint256 treasuryAmount = 200000000 * 10**18; // 40%
        uint256 publicAmount = 150000000 * 10**18;   // 30%
        uint256 devAmount = 100000000 * 10**18;      // 20%
        uint256 marketingAmount = 50000000 * 10**18; // 10%
        
        // Distribute tokens
        _mint(treasury, treasuryAmount);
        _mint(address(this), publicAmount); // For public distribution
        _mint(development, devAmount);
        _mint(marketing, marketingAmount);
        
        totalMinted = INITIAL_SUPPLY;
        totalTransactions = 1; // Genesis transaction
        
        // Setup ecosystem partners
        ecosystemPartners[treasury] = true;
        ecosystemPartners[development] = true;
        ecosystemPartners[marketing] = true;
        
        // Setup fee discounts
        feeDiscounts["staking"] = 50; // 50% discount for staking
        feeDiscounts["trading"] = 25; // 25% discount for trading
        feeDiscounts["bridge"] = 30;  // 30% discount for bridging
    }
    
    // 💰 CORE TOKEN FUNCTIONS
    function decimals() public pure override returns (uint8) {
        return DECIMALS;
    }
    
    function transfer(address to, uint256 amount) 
        public 
        override 
        validAddress(to) 
        notBlacklisted(msg.sender)
        notBlacklisted(to)
        whenNotPaused
        returns (bool) {
        
        return _transfer(msg.sender, to, amount);
    }
    
    function transferFrom(address from, address to, uint256 amount) 
        public 
        override 
        validAddress(from) 
        validAddress(to) 
        whenNotPaused
        returns (bool) {
        
        return super.transferFrom(from, to, amount);
    }
    
    // 🔄 INTERNAL TRANSFER LOGIC
    function _transfer(address from, address to, uint256 amount) 
        internal 
        override
        returns (bool) {
        
        // Update analytics
        totalTransactions++;
        userTransactionCount[from]++;
        userTransactionCount[to]++;
        
        return super._transfer(from, to, amount);
    }
    
    // 💎 STAKING FUNCTIONS
    function stake(uint256 amount) external nonReentrant whenNotPaused returns (bool) {
        require(balanceOf(msg.sender) >= amount, "Insufficient balance");
        require(amount > 0, "Amount must be positive");
        
        // Transfer tokens to staking pool
        _transfer(msg.sender, address(this), amount);
        stakingBalances[msg.sender] += amount;
        totalStaked += amount;
        lastStakeTime[msg.sender] = block.timestamp;
        
        emit Staked(msg.sender, amount);
        return true;
    }
    
    function unstake(uint256 amount) external nonReentrant whenNotPaused returns (bool) {
        require(stakingBalances[msg.sender] >= amount, "Insufficient staked balance");
        require(amount > 0, "Amount must be positive");
        
        // Calculate rewards first
        claimRewards();
        
        // Unstake tokens
        stakingBalances[msg.sender] -= amount;
        _transfer(address(this), msg.sender, amount);
        totalStaked -= amount;
        
        emit Unstaked(msg.sender, amount);
        return true;
    }
    
    function claimRewards() public nonReentrant whenNotPaused returns (uint256) {
        require(stakingBalances[msg.sender] > 0, "No staked balance");
        
        uint256 stakedAmount = stakingBalances[msg.sender];
        uint256 stakingDuration = block.timestamp - lastStakeTime[msg.sender];
        uint256 rewards = (stakedAmount * stakingRewardRate * stakingDuration) / (365 days * 100);
        
        if (rewards > 0) {
            stakingRewards[msg.sender] += rewards;
            _mint(msg.sender, rewards);
            totalMinted += rewards;
            lastStakeTime[msg.sender] = block.timestamp;
            
            emit RewardsClaimed(msg.sender, rewards);
        }
        
        return rewards;
    }
    
    // 🔄 ECOSYSTEM INTEGRATION
    function addEcosystemPartner(address partner) 
        external 
        onlyOwner 
        validAddress(partner) {
        
        ecosystemPartners[partner] = true;
        emit EcosystemPartnerAdded(partner);
    }
    
    function setFeeDiscount(string memory service, uint256 discount) 
        external 
        onlyOwner {
        
        require(discount <= 100, "Discount cannot exceed 100%");
        feeDiscounts[service] = discount;
        emit FeeDiscountUpdated(service, discount);
    }
    
    function calculateFee(string memory service, uint256 amount) 
        external 
        view 
        returns (uint256) {
        
        uint256 discount = feeDiscounts[service];
        uint256 fee = (amount * baseFee) / 10000;
        uint256 discountedFee = fee - (fee * discount) / 100;
        
        return discountedFee;
    }
    
    // 🛡️ ANTI-FRAUD FUNCTIONS
    function blacklistAddress(address account, bool isBlacklisted) 
        external 
        onlyOwner 
        validAddress(account) {
        
        blacklistedAddresses[account] = isBlacklisted;
        emit BlacklistUpdated(account, isBlacklisted);
    }
    
    function setDailyTransferLimit(address account, uint256 limit) 
        external 
        onlyOwner 
        validAddress(account) {
        
        dailyTransferLimits[account] = limit;
    }
    
    // 🚨 EMERGENCY FUNCTIONS
    function pause() external onlyOwner {
        _pause();
    }
    
    function unpause() external onlyOwner {
        _unpause();
    }
    
    // 📊 ANALYTICS FUNCTIONS
    function getUtilityStats() external view returns (
        uint256 _totalSupply,
        uint256 _totalStaked,
        uint256 _totalTransactions,
        uint256 _stakingRewardRate
    ) {
        return (
            totalSupply(),
            totalStaked,
            totalTransactions,
            stakingRewardRate
        );
    }
    
    function getUserUtilityStats(address user) external view returns (
        uint256 balance,
        uint256 stakedBalance,
        uint256 pendingRewards,
        uint256 transactions
    ) {
        uint256 stakedAmount = stakingBalances[user];
        uint256 stakingDuration = block.timestamp - lastStakeTime[user];
        uint256 pendingRewards = (stakedAmount * stakingRewardRate * stakingDuration) / (365 days * 100);
        
        return (
            balanceOf(user),
            stakedAmount,
            pendingRewards,
            userTransactionCount[user]
        );
    }
}
