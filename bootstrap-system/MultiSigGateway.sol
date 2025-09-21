// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

/**
 * 🔐 USDTgVerse Multi-Sig Gateway - Ethereum Side Bridge
 * =====================================================
 * 
 * This contract runs on Ethereum and handles:
 * 1. Receiving USDT/USDC/ETH deposits
 * 2. Emitting events for USDTgVerse bridge
 * 3. Multi-signature security for withdrawals
 * 4. Emergency controls and limits
 */

import "@openzeppelin/contracts/token/ERC20/IERC20.sol";
import "@openzeppelin/contracts/token/ERC20/utils/SafeERC20.sol";
import "@openzeppelin/contracts/security/ReentrancyGuard.sol";
import "@openzeppelin/contracts/security/Pausable.sol";
import "@openzeppelin/contracts/access/Ownable.sol";

contract USDTgVerseMultiSigGateway is ReentrancyGuard, Pausable, Ownable {
    using SafeERC20 for IERC20;
    
    // 📊 GATEWAY METADATA
    string public constant name = "USDTgVerse Multi-Sig Gateway";
    string public constant version = "1.0.0-bootstrap";
    
    // 🔐 MULTI-SIG CONFIGURATION
    struct MultiSigConfig {
        address[] signers;
        uint256 threshold;
        uint256 nonce;
        mapping(address => bool) isSigner;
        mapping(bytes32 => uint256) confirmations;
        mapping(bytes32 => mapping(address => bool)) hasConfirmed;
    }
    
    // 💰 SUPPORTED TOKEN STRUCTURE
    struct SupportedToken {
        address tokenAddress;
        string symbol;
        uint8 decimals;
        bool active;
        uint256 minDeposit;
        uint256 maxDeposit;
        uint256 dailyLimit;
        uint256 dailyVolume;
        uint256 lastReset;
        uint256 totalDeposited;
    }
    
    // 🔄 DEPOSIT STRUCTURE
    struct Deposit {
        uint256 depositId;
        address user;
        address token;
        uint256 amount;
        string usdtgAddress;
        uint256 timestamp;
        bytes32 txHash;
        bool processed;
    }
    
    // 🗄️ STORAGE
    MultiSigConfig private multiSig;
    mapping(address => SupportedToken) public supportedTokens;
    address[] public tokenList;
    mapping(uint256 => Deposit) public deposits;
    mapping(address => uint256[]) public userDeposits;
    uint256 public nextDepositId = 1;
    
    // 📊 STATISTICS
    uint256 public totalDeposits;
    uint256 public totalVolume;
    mapping(address => uint256) public tokenVolumes;
    
    // ⚙️ CONFIGURATION
    uint256 public constant MAX_DAILY_LIMIT = 1000000 * 10**6; // 1M USDT
    uint256 public bridgeFee = 30; // 0.3% in basis points
    address public feeRecipient;
    
    // 🚨 EVENTS
    event DepositMade(
        uint256 indexed depositId,
        address indexed user,
        address indexed token,
        uint256 amount,
        string usdtgAddress,
        bytes32 txHash
    );
    
    event WithdrawalExecuted(
        address indexed token,
        address indexed to,
        uint256 amount,
        bytes32 indexed operationHash
    );
    
    event SignerAdded(address indexed signer);
    event SignerRemoved(address indexed signer);
    event ThresholdChanged(uint256 newThreshold);
    event TokenAdded(address indexed token, string symbol);
    event TokenUpdated(address indexed token, bool active);
    
    // 🔒 MODIFIERS
    modifier onlySigner() {
        require(multiSig.isSigner[msg.sender], "Not a signer");
        _;
    }
    
    modifier validToken(address token) {
        require(supportedTokens[token].active, "Token not supported");
        _;
    }
    
    modifier validUSDTgAddress(string memory usdtgAddress) {
        require(bytes(usdtgAddress).length >= 50, "Invalid USDTgVerse address");
        require(
            bytes(usdtgAddress)[0] == 'U' &&
            bytes(usdtgAddress)[1] == 'S' &&
            bytes(usdtgAddress)[2] == 'D' &&
            bytes(usdtgAddress)[3] == 'T' &&
            bytes(usdtgAddress)[4] == 'g' &&
            bytes(usdtgAddress)[5] == '1' &&
            bytes(usdtgAddress)[6] == 'q',
            "Invalid USDTgVerse address format"
        );
        _;
    }
    
    // 🏗️ CONSTRUCTOR
    constructor(
        address[] memory _signers,
        uint256 _threshold,
        address _feeRecipient
    ) {
        require(_signers.length >= 3, "Minimum 3 signers required");
        require(_threshold >= 2 && _threshold <= _signers.length, "Invalid threshold");
        require(_feeRecipient != address(0), "Invalid fee recipient");
        
        // Setup multi-sig
        multiSig.threshold = _threshold;
        for (uint256 i = 0; i < _signers.length; i++) {
            address signer = _signers[i];
            require(signer != address(0), "Invalid signer");
            require(!multiSig.isSigner[signer], "Duplicate signer");
            
            multiSig.signers.push(signer);
            multiSig.isSigner[signer] = true;
        }
        
        feeRecipient = _feeRecipient;
        
        // Setup initial supported tokens
        _setupInitialTokens();
    }
    
    // 🌍 SETUP INITIAL TOKENS
    function _setupInitialTokens() internal {
        // USDT
        address usdtAddress = 0xdAC17F958D2ee523a2206206994597C13D831ec7;
        supportedTokens[usdtAddress] = SupportedToken({
            tokenAddress: usdtAddress,
            symbol: "USDT",
            decimals: 6,
            active: true,
            minDeposit: 10 * 10**6, // 10 USDT
            maxDeposit: 100000 * 10**6, // 100k USDT
            dailyLimit: 500000 * 10**6, // 500k USDT
            dailyVolume: 0,
            lastReset: block.timestamp,
            totalDeposited: 0
        });
        tokenList.push(usdtAddress);
        
        // USDC
        address usdcAddress = 0xA0b86a33E6441b8aD62c6dF2F8e4a8f1b1e2e1c0;
        supportedTokens[usdcAddress] = SupportedToken({
            tokenAddress: usdcAddress,
            symbol: "USDC",
            decimals: 6,
            active: true,
            minDeposit: 10 * 10**6,
            maxDeposit: 100000 * 10**6,
            dailyLimit: 500000 * 10**6,
            dailyVolume: 0,
            lastReset: block.timestamp,
            totalDeposited: 0
        });
        tokenList.push(usdcAddress);
    }
    
    // 💰 DEPOSIT FUNCTION
    function deposit(
        address token,
        uint256 amount,
        string calldata usdtgAddress
    ) external 
        nonReentrant 
        whenNotPaused 
        validToken(token) 
        validUSDTgAddress(usdtgAddress) 
    {
        require(amount > 0, "Invalid amount");
        
        SupportedToken storage tokenInfo = supportedTokens[token];
        
        // Check limits
        require(amount >= tokenInfo.minDeposit, "Below minimum deposit");
        require(amount <= tokenInfo.maxDeposit, "Above maximum deposit");
        
        // Check daily limit
        _checkDailyLimit(token, amount);
        
        // Calculate fee
        uint256 fee = (amount * bridgeFee) / 10000;
        uint256 netAmount = amount - fee;
        
        // Transfer tokens
        IERC20(token).safeTransferFrom(msg.sender, address(this), amount);
        
        // Send fee to fee recipient
        if (fee > 0) {
            IERC20(token).safeTransfer(feeRecipient, fee);
        }
        
        // Create deposit record
        uint256 depositId = nextDepositId++;
        deposits[depositId] = Deposit({
            depositId: depositId,
            user: msg.sender,
            token: token,
            amount: netAmount,
            usdtgAddress: usdtgAddress,
            timestamp: block.timestamp,
            txHash: blockhash(block.number - 1),
            processed: false
        });
        
        // Update user deposits
        userDeposits[msg.sender].push(depositId);
        
        // Update statistics
        totalDeposits++;
        totalVolume += amount;
        tokenVolumes[token] += amount;
        tokenInfo.totalDeposited += amount;
        tokenInfo.dailyVolume += amount;
        
        emit DepositMade(
            depositId,
            msg.sender,
            token,
            netAmount,
            usdtgAddress,
            blockhash(block.number - 1)
        );
    }
    
    // 💰 ETH DEPOSIT
    function depositETH(string calldata usdtgAddress) 
        external 
        payable 
        nonReentrant 
        whenNotPaused 
        validUSDTgAddress(usdtgAddress) 
    {
        require(msg.value > 0, "Invalid amount");
        require(msg.value >= 0.01 ether, "Minimum 0.01 ETH");
        require(msg.value <= 100 ether, "Maximum 100 ETH");
        
        // Calculate fee
        uint256 fee = (msg.value * bridgeFee) / 10000;
        uint256 netAmount = msg.value - fee;
        
        // Send fee to fee recipient
        if (fee > 0) {
            payable(feeRecipient).transfer(fee);
        }
        
        // Create deposit record
        uint256 depositId = nextDepositId++;
        deposits[depositId] = Deposit({
            depositId: depositId,
            user: msg.sender,
            token: address(0), // ETH
            amount: netAmount,
            usdtgAddress: usdtgAddress,
            timestamp: block.timestamp,
            txHash: blockhash(block.number - 1),
            processed: false
        });
        
        // Update user deposits
        userDeposits[msg.sender].push(depositId);
        
        // Update statistics
        totalDeposits++;
        totalVolume += msg.value;
        tokenVolumes[address(0)] += msg.value;
        
        emit DepositMade(
            depositId,
            msg.sender,
            address(0),
            netAmount,
            usdtgAddress,
            blockhash(block.number - 1)
        );
    }
    
    // 📊 CHECK DAILY LIMIT
    function _checkDailyLimit(address token, uint256 amount) internal {
        SupportedToken storage tokenInfo = supportedTokens[token];
        
        // Reset daily volume if needed
        if (block.timestamp >= tokenInfo.lastReset + 86400) {
            tokenInfo.dailyVolume = 0;
            tokenInfo.lastReset = block.timestamp;
        }
        
        require(tokenInfo.dailyVolume + amount <= tokenInfo.dailyLimit, "Daily limit exceeded");
    }
    
    // 🔐 MULTI-SIG WITHDRAWAL
    function proposeWithdrawal(
        address token,
        address to,
        uint256 amount,
        uint256 nonce
    ) external onlySigner returns (bytes32 operationHash) {
        operationHash = keccak256(abi.encodePacked(
            "WITHDRAW",
            token,
            to,
            amount,
            nonce,
            block.chainid
        ));
        
        require(multiSig.confirmations[operationHash] == 0, "Operation already exists");
        require(nonce == multiSig.nonce + 1, "Invalid nonce");
        
        // First confirmation
        multiSig.confirmations[operationHash] = 1;
        multiSig.hasConfirmed[operationHash][msg.sender] = true;
        
        return operationHash;
    }
    
    function confirmWithdrawal(bytes32 operationHash) external onlySigner {
        require(multiSig.confirmations[operationHash] > 0, "Operation does not exist");
        require(!multiSig.hasConfirmed[operationHash][msg.sender], "Already confirmed");
        
        multiSig.hasConfirmed[operationHash][msg.sender] = true;
        multiSig.confirmations[operationHash]++;
    }
    
    function executeWithdrawal(
        address token,
        address to,
        uint256 amount,
        uint256 nonce
    ) external onlySigner nonReentrant {
        bytes32 operationHash = keccak256(abi.encodePacked(
            "WITHDRAW",
            token,
            to,
            amount,
            nonce,
            block.chainid
        ));
        
        require(multiSig.confirmations[operationHash] >= multiSig.threshold, "Insufficient confirmations");
        require(nonce == multiSig.nonce + 1, "Invalid nonce");
        
        // Execute withdrawal
        if (token == address(0)) {
            // ETH withdrawal
            require(address(this).balance >= amount, "Insufficient ETH balance");
            payable(to).transfer(amount);
        } else {
            // Token withdrawal
            require(IERC20(token).balanceOf(address(this)) >= amount, "Insufficient token balance");
            IERC20(token).safeTransfer(to, amount);
        }
        
        // Update nonce
        multiSig.nonce++;
        
        // Clear confirmations
        delete multiSig.confirmations[operationHash];
        for (uint256 i = 0; i < multiSig.signers.length; i++) {
            delete multiSig.hasConfirmed[operationHash][multiSig.signers[i]];
        }
        
        emit WithdrawalExecuted(token, to, amount, operationHash);
    }
    
    // 📊 QUERY FUNCTIONS
    function getDeposit(uint256 depositId) external view returns (Deposit memory) {
        return deposits[depositId];
    }
    
    function getUserDeposits(address user) external view returns (uint256[] memory) {
        return userDeposits[user];
    }
    
    function getSupportedTokens() external view returns (address[] memory) {
        return tokenList;
    }
    
    function getSigners() external view returns (address[] memory) {
        return multiSig.signers;
    }
    
    function getThreshold() external view returns (uint256) {
        return multiSig.threshold;
    }
    
    function getConfirmations(bytes32 operationHash) external view returns (uint256) {
        return multiSig.confirmations[operationHash];
    }
    
    function hasConfirmed(bytes32 operationHash, address signer) external view returns (bool) {
        return multiSig.hasConfirmed[operationHash][signer];
    }
    
    function getGatewayStats() external view returns (
        uint256 _totalDeposits,
        uint256 _totalVolume,
        uint256 _supportedTokens
    ) {
        return (totalDeposits, totalVolume, tokenList.length);
    }
    
    // 🔧 ADMIN FUNCTIONS
    function addSupportedToken(
        address token,
        string calldata symbol,
        uint8 decimals,
        uint256 minDeposit,
        uint256 maxDeposit,
        uint256 dailyLimit
    ) external onlyOwner {
        require(token != address(0), "Invalid token");
        require(!supportedTokens[token].active, "Token already supported");
        
        supportedTokens[token] = SupportedToken({
            tokenAddress: token,
            symbol: symbol,
            decimals: decimals,
            active: true,
            minDeposit: minDeposit,
            maxDeposit: maxDeposit,
            dailyLimit: dailyLimit,
            dailyVolume: 0,
            lastReset: block.timestamp,
            totalDeposited: 0
        });
        
        tokenList.push(token);
        
        emit TokenAdded(token, symbol);
    }
    
    function updateTokenStatus(address token, bool active) external onlyOwner {
        require(supportedTokens[token].tokenAddress == token, "Token not found");
        supportedTokens[token].active = active;
        
        emit TokenUpdated(token, active);
    }
    
    function updateBridgeFee(uint256 newFee) external onlyOwner {
        require(newFee <= 1000, "Fee too high"); // Max 10%
        bridgeFee = newFee;
    }
    
    function updateFeeRecipient(address newRecipient) external onlyOwner {
        require(newRecipient != address(0), "Invalid recipient");
        feeRecipient = newRecipient;
    }
    
    // 🔐 MULTI-SIG MANAGEMENT
    function addSigner(address newSigner) external onlyOwner {
        require(newSigner != address(0), "Invalid signer");
        require(!multiSig.isSigner[newSigner], "Already a signer");
        
        multiSig.signers.push(newSigner);
        multiSig.isSigner[newSigner] = true;
        
        emit SignerAdded(newSigner);
    }
    
    function removeSigner(address signer) external onlyOwner {
        require(multiSig.isSigner[signer], "Not a signer");
        require(multiSig.signers.length > multiSig.threshold, "Cannot remove, would break threshold");
        
        // Remove from signers array
        for (uint256 i = 0; i < multiSig.signers.length; i++) {
            if (multiSig.signers[i] == signer) {
                multiSig.signers[i] = multiSig.signers[multiSig.signers.length - 1];
                multiSig.signers.pop();
                break;
            }
        }
        
        multiSig.isSigner[signer] = false;
        
        emit SignerRemoved(signer);
    }
    
    function updateThreshold(uint256 newThreshold) external onlyOwner {
        require(newThreshold >= 2, "Threshold too low");
        require(newThreshold <= multiSig.signers.length, "Threshold too high");
        
        multiSig.threshold = newThreshold;
        
        emit ThresholdChanged(newThreshold);
    }
    
    // 🏥 EMERGENCY FUNCTIONS
    function pause() external onlyOwner {
        _pause();
    }
    
    function unpause() external onlyOwner {
        _unpause();
    }
    
    function emergencyWithdraw(address token, address to, uint256 amount) external onlyOwner {
        if (token == address(0)) {
            payable(to).transfer(amount);
        } else {
            IERC20(token).safeTransfer(to, amount);
        }
    }
    
    // 💰 RECEIVE ETH
    receive() external payable {
        // Allow contract to receive ETH
    }
}
