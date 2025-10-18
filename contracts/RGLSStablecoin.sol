// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

/**
 * @title RGLS Stablecoin Contract
 * @dev Regilis Native Coin - $1 Pegged Stablecoin
 * @author USDTgVerse Team
 * @notice This contract implements a stablecoin pegged to $1 USD
 *         with anti-fraud mechanisms and trading protection
 */
contract RGLSStablecoin {
    // ============================================================================
    // STATE VARIABLES
    // ============================================================================
    
    string public constant name = "Regilis Stablecoin";
    string public constant symbol = "RGLS";
    uint8 public constant decimals = 18;
    uint256 public constant totalSupply = 500000000000 * 10**18; // 500 Billion tokens
    
    // Peg mechanism
    uint256 public constant PEG_PRICE = 1 * 10**18; // $1 USD
    address public oracleAddress;
    
    // Balances and allowances
    mapping(address => uint256) public balanceOf;
    mapping(address => mapping(address => uint256)) public allowance;
    
    // Anti-fraud mechanisms
    mapping(address => bool) public blacklisted;
    mapping(address => uint256) public dailyLimit;
    mapping(address => uint256) public lastTradeTime;
    mapping(address => uint256) public suspiciousActivityCount;
    
    // Trading protection system
    mapping(address => uint256) public maxTradePercentage; // %0.5 = 50 basis points
    mapping(address => uint256) public stopLossPercentage; // %50 = 5000 basis points
    mapping(address => uint256) public takeProfitPercentage; // %50 = 5000 basis points
    mapping(address => uint256) public emergencyStopPercentage; // %20 = 2000 basis points
    
    // Trading positions
    struct TradingPosition {
        uint256 amount;
        uint256 entryPrice;
        uint256 currentPrice;
        uint256 maxLossAmount;
        uint256 maxProfitAmount;
        bool isActive;
        uint256 startTime;
    }
    
    mapping(address => TradingPosition) public tradingPositions;
    
    // Access control
    address public owner;
    mapping(address => bool) public authorizedOperators;
    
    // Constants
    uint256 public constant MAX_DAILY_TRADE_PERCENTAGE = 50; // %0.5 = 50 basis points
    uint256 public constant MAX_TRADE_AMOUNT = 1000000 * 10**18; // $1M max per trade
    uint256 public constant SUSPICIOUS_ACTIVITY_THRESHOLD = 5;
    uint256 public constant BLACKLIST_DURATION = 86400; // 24 hours
    
    // ============================================================================
    // EVENTS
    // ============================================================================
    
    event Transfer(address indexed from, address indexed to, uint256 value);
    event Approval(address indexed owner, address indexed spender, uint256 value);
    event PegUpdated(uint256 newPrice);
    event TradeLimitUpdated(address indexed user, uint256 limit);
    event AntiFraudTriggered(address indexed user, string reason);
    event TradingPositionOpened(address indexed user, uint256 amount, uint256 entryPrice);
    event TradingPositionClosed(address indexed user, string reason, int256 pnl);
    event UserBlacklisted(address indexed user, uint256 until);
    event UserWhitelisted(address indexed user);
    
    // ============================================================================
    // MODIFIERS
    // ============================================================================
    
    modifier onlyOwner() {
        require(msg.sender == owner, "RGLS: Only owner can call this function");
        _;
    }
    
    modifier onlyAuthorized() {
        require(authorizedOperators[msg.sender] || msg.sender == owner, "RGLS: Only authorized operators");
        _;
    }
    
    modifier notBlacklisted(address user) {
        require(!blacklisted[user], "RGLS: User is blacklisted");
        _;
    }
    
    modifier validAmount(uint256 amount) {
        require(amount > 0, "RGLS: Amount must be greater than zero");
        require(amount <= totalSupply, "RGLS: Amount exceeds total supply");
        _;
    }
    
    // ============================================================================
    // CONSTRUCTOR
    // ============================================================================
    
    constructor(address _oracleAddress) {
        owner = msg.sender;
        oracleAddress = _oracleAddress;
        authorizedOperators[msg.sender] = true;
        
        // Initialize total supply
        balanceOf[msg.sender] = totalSupply;
        emit Transfer(address(0), msg.sender, totalSupply);
        
        // Set default trading protection parameters
        maxTradePercentage[msg.sender] = MAX_DAILY_TRADE_PERCENTAGE;
        stopLossPercentage[msg.sender] = 5000; // 50%
        takeProfitPercentage[msg.sender] = 5000; // 50%
        emergencyStopPercentage[msg.sender] = 2000; // 20%
    }
    
    // ============================================================================
    // ERC20 FUNCTIONS
    // ============================================================================
    
    function transfer(address to, uint256 amount) external notBlacklisted(msg.sender) notBlacklisted(to) validAmount(amount) returns (bool) {
        require(balanceOf[msg.sender] >= amount, "RGLS: Insufficient balance");
        
        // Check anti-fraud mechanisms
        require(checkAntiFraud(msg.sender, amount), "RGLS: Anti-fraud check failed");
        
        // Check trading protection
        require(checkTradingProtection(msg.sender, amount), "RGLS: Trading protection check failed");
        
        balanceOf[msg.sender] -= amount;
        balanceOf[to] += amount;
        
        emit Transfer(msg.sender, to, amount);
        return true;
    }
    
    function transferFrom(address from, address to, uint256 amount) external notBlacklisted(from) notBlacklisted(to) validAmount(amount) returns (bool) {
        require(balanceOf[from] >= amount, "RGLS: Insufficient balance");
        require(allowance[from][msg.sender] >= amount, "RGLS: Insufficient allowance");
        
        // Check anti-fraud mechanisms
        require(checkAntiFraud(from, amount), "RGLS: Anti-fraud check failed");
        
        // Check trading protection
        require(checkTradingProtection(from, amount), "RGLS: Trading protection check failed");
        
        balanceOf[from] -= amount;
        balanceOf[to] += amount;
        allowance[from][msg.sender] -= amount;
        
        emit Transfer(from, to, amount);
        return true;
    }
    
    function approve(address spender, uint256 amount) external notBlacklisted(msg.sender) notBlacklisted(spender) returns (bool) {
        allowance[msg.sender][spender] = amount;
        emit Approval(msg.sender, spender, amount);
        return true;
    }
    
    // ============================================================================
    // ANTI-FRAUD MECHANISMS
    // ============================================================================
    
    function checkAntiFraud(address user, uint256 amount) internal returns (bool) {
        // Check daily trade limit
        if (amount > dailyLimit[user]) {
            suspiciousActivityCount[user]++;
            if (suspiciousActivityCount[user] >= SUSPICIOUS_ACTIVITY_THRESHOLD) {
                blacklistUser(user);
                emit AntiFraudTriggered(user, "Daily limit exceeded");
                return false;
            }
            emit AntiFraudTriggered(user, "Amount exceeds daily limit");
            return false;
        }
        
        // Check trade percentage
        uint256 maxTradeAmount = (balanceOf[user] * maxTradePercentage[user]) / 10000;
        if (amount > maxTradeAmount) {
            suspiciousActivityCount[user]++;
            emit AntiFraudTriggered(user, "Amount exceeds percentage limit");
            return false;
        }
        
        // Check maximum trade amount
        if (amount > MAX_TRADE_AMOUNT) {
            suspiciousActivityCount[user]++;
            emit AntiFraudTriggered(user, "Amount exceeds maximum limit");
            return false;
        }
        
        // Reset suspicious activity counter on successful transaction
        suspiciousActivityCount[user] = 0;
        return true;
    }
    
    function blacklistUser(address user) internal {
        blacklisted[user] = true;
        emit UserBlacklisted(user, block.timestamp + BLACKLIST_DURATION);
    }
    
    function whitelistUser(address user) external onlyAuthorized {
        blacklisted[user] = false;
        suspiciousActivityCount[user] = 0;
        emit UserWhitelisted(user);
    }
    
    // ============================================================================
    // TRADING PROTECTION SYSTEM
    // ============================================================================
    
    function checkTradingProtection(address user, uint256 amount) internal returns (bool) {
        TradingPosition storage position = tradingPositions[user];
        
        // Check maximum trade percentage
        uint256 maxTradeAmount = (balanceOf[user] * maxTradePercentage[user]) / 10000;
        if (amount > maxTradeAmount) {
            emit AntiFraudTriggered(user, "Trade amount exceeds maximum percentage");
            return false;
        }
        
        // If position is active, check stop loss and take profit
        if (position.isActive) {
            int256 pnl = calculatePnL(user);
            
            // Check stop loss
            if (pnl < 0 && uint256(-pnl) >= position.maxLossAmount) {
                closePosition(user, "Stop loss triggered");
                return false;
            }
            
            // Check take profit
            if (pnl > 0 && uint256(pnl) >= position.maxProfitAmount) {
                closePosition(user, "Take profit triggered");
                return false;
            }
            
            // Check emergency stop (20% loss)
            if (pnl < 0 && uint256(-pnl) >= (balanceOf[user] * emergencyStopPercentage[user]) / 10000) {
                closePosition(user, "Emergency stop triggered");
                return false;
            }
        }
        
        return true;
    }
    
    function openTradingPosition(uint256 amount, uint256 entryPrice) external notBlacklisted(msg.sender) {
        require(balanceOf[msg.sender] >= amount, "RGLS: Insufficient balance");
        require(!tradingPositions[msg.sender].isActive, "RGLS: Position already active");
        
        TradingPosition storage position = tradingPositions[msg.sender];
        position.amount = amount;
        position.entryPrice = entryPrice;
        position.currentPrice = entryPrice;
        position.maxLossAmount = (amount * stopLossPercentage[msg.sender]) / 10000;
        position.maxProfitAmount = (amount * takeProfitPercentage[msg.sender]) / 10000;
        position.isActive = true;
        position.startTime = block.timestamp;
        
        emit TradingPositionOpened(msg.sender, amount, entryPrice);
    }
    
    function closePosition(address user, string memory reason) internal {
        TradingPosition storage position = tradingPositions[user];
        require(position.isActive, "RGLS: No active position");
        
        int256 pnl = calculatePnL(user);
        
        // Update user's balance
        if (pnl > 0) {
            balanceOf[user] += uint256(pnl);
        } else {
            balanceOf[user] -= uint256(-pnl);
        }
        
        // Reset position
        position.isActive = false;
        position.amount = 0;
        position.entryPrice = 0;
        position.currentPrice = 0;
        position.maxLossAmount = 0;
        position.maxProfitAmount = 0;
        position.startTime = 0;
        
        emit TradingPositionClosed(user, reason, pnl);
    }
    
    function calculatePnL(address user) public view returns (int256) {
        TradingPosition storage position = tradingPositions[user];
        if (!position.isActive) {
            return 0;
        }
        
        int256 priceDiff = int256(position.currentPrice) - int256(position.entryPrice);
        return (priceDiff * int256(position.amount)) / int256(position.entryPrice);
    }
    
    // ============================================================================
    // ADMIN FUNCTIONS
    // ============================================================================
    
    function setOracleAddress(address _oracleAddress) external onlyOwner {
        oracleAddress = _oracleAddress;
    }
    
    function setDailyLimit(address user, uint256 limit) external onlyAuthorized {
        dailyLimit[user] = limit;
        emit TradeLimitUpdated(user, limit);
    }
    
    function setMaxTradePercentage(address user, uint256 percentage) external onlyAuthorized {
        require(percentage <= 1000, "RGLS: Percentage too high"); // Max 10%
        maxTradePercentage[user] = percentage;
    }
    
    function setStopLossPercentage(address user, uint256 percentage) external onlyAuthorized {
        require(percentage <= 10000, "RGLS: Percentage too high"); // Max 100%
        stopLossPercentage[user] = percentage;
    }
    
    function setTakeProfitPercentage(address user, uint256 percentage) external onlyAuthorized {
        require(percentage <= 10000, "RGLS: Percentage too high"); // Max 100%
        takeProfitPercentage[user] = percentage;
    }
    
    function setEmergencyStopPercentage(address user, uint256 percentage) external onlyAuthorized {
        require(percentage <= 10000, "RGLS: Percentage too high"); // Max 100%
        emergencyStopPercentage[user] = percentage;
    }
    
    function addAuthorizedOperator(address operator) external onlyOwner {
        authorizedOperators[operator] = true;
    }
    
    function removeAuthorizedOperator(address operator) external onlyOwner {
        authorizedOperators[operator] = false;
    }
    
    // ============================================================================
    // PEG MAINTENANCE
    // ============================================================================
    
    function updatePrice(uint256 newPrice) external onlyAuthorized {
        require(newPrice == PEG_PRICE, "RGLS: Price must equal peg price");
        emit PegUpdated(newPrice);
    }
    
    function maintainPeg() external onlyAuthorized {
        // This function would be called by the oracle to maintain the $1 peg
        // Implementation would depend on the specific peg maintenance strategy
        emit PegUpdated(PEG_PRICE);
    }
    
    // ============================================================================
    // VIEW FUNCTIONS
    // ============================================================================
    
    function getTradingPosition(address user) external view returns (
        uint256 amount,
        uint256 entryPrice,
        uint256 currentPrice,
        uint256 maxLossAmount,
        uint256 maxProfitAmount,
        bool isActive,
        uint256 startTime
    ) {
        TradingPosition storage position = tradingPositions[user];
        return (
            position.amount,
            position.entryPrice,
            position.currentPrice,
            position.maxLossAmount,
            position.maxProfitAmount,
            position.isActive,
            position.startTime
        );
    }
    
    function getAntiFraudStatus(address user) external view returns (
        bool isBlacklisted,
        uint256 dailyLimit,
        uint256 suspiciousActivityCount,
        uint256 lastTradeTime
    ) {
        return (
            blacklisted[user],
            dailyLimit[user],
            suspiciousActivityCount[user],
            lastTradeTime[user]
        );
    }
    
    function getTradingProtectionSettings(address user) external view returns (
        uint256 maxTradePercentage,
        uint256 stopLossPercentage,
        uint256 takeProfitPercentage,
        uint256 emergencyStopPercentage
    ) {
        return (
            maxTradePercentage[user],
            stopLossPercentage[user],
            takeProfitPercentage[user],
            emergencyStopPercentage[user]
        );
    }
}
