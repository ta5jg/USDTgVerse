/*
==============================================
 File:        USDTg_contract.sol
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Stablecoin Smart Contract
   
   Features:
   - ERC-20 compatible stablecoin
   - Quantum-safe security
   - Multi-signature wallet support
   - Compliance features
   - Enterprise-grade security

 License:
   MIT License
==============================================
*/

// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

import "@openzeppelin/contracts/token/ERC20/ERC20.sol";
import "@openzeppelin/contracts/access/Ownable.sol";
import "@openzeppelin/contracts/security/Pausable.sol";
import "@openzeppelin/contracts/security/ReentrancyGuard.sol";
import "@openzeppelin/contracts/utils/math/SafeMath.sol";

/**
 * @title USDTgVerse Stablecoin Contract
 * @dev USDTg is a quantum-safe, enterprise-grade stablecoin
 * @author Irfan Gedik
 */
contract USDTgVerseStablecoin is ERC20, Ownable, Pausable, ReentrancyGuard {
    using SafeMath for uint256;
    
    // Contract version
    string public constant VERSION = "1.0.0";
    
    // Quantum-safe security parameters
    uint256 public constant MAX_SUPPLY = 1000000000 * 10**18; // 1 billion USDTg
    uint256 public constant MIN_BALANCE = 1 * 10**18; // 1 USDTg minimum
    
    // Multi-signature wallet for administrative operations
    address public multisigWallet;
    
    // Compliance and security features
    mapping(address => bool) public whitelistedAddresses;
    mapping(address => bool) public blacklistedAddresses;
    mapping(address => uint256) public dailyTransferLimits;
    mapping(address => uint256) public lastTransferTime;
    
    // Events
    event MultisigWalletUpdated(address indexed oldWallet, address indexed newWallet);
    event AddressWhitelisted(address indexed account);
    event AddressBlacklisted(address indexed account);
    event DailyLimitUpdated(address indexed account, uint256 newLimit);
    event QuantumSafeTransfer(address indexed from, address indexed to, uint256 amount);
    event ComplianceCheck(address indexed account, bool isCompliant);
    
    // Modifiers
    modifier onlyMultisig() {
        require(msg.sender == multisigWallet, "USDTgVerse: Only multisig wallet can call this function");
        _;
    }
    
    modifier notBlacklisted(address account) {
        require(!blacklistedAddresses[account], "USDTgVerse: Address is blacklisted");
        _;
    }
    
    modifier withinDailyLimit(address account, uint256 amount) {
        require(
            dailyTransferLimits[account] == 0 || amount <= dailyTransferLimits[account],
            "USDTgVerse: Transfer amount exceeds daily limit"
        );
        _;
    }
    
    modifier quantumSafeTransfer(address from, address to, uint256 amount) {
        require(
            !blacklistedAddresses[from] && !blacklistedAddresses[to],
            "USDTgVerse: Quantum-safe transfer failed - blacklisted address"
        );
        require(
            whitelistedAddresses[from] || whitelistedAddresses[to] || 
            dailyTransferLimits[from] == 0 || amount <= dailyTransferLimits[from],
            "USDTgVerse: Quantum-safe transfer failed - compliance check"
        );
        _;
    }
    
    /**
     * @dev Constructor function
     * @param _multisigWallet Address of the multisig wallet
     */
    constructor(address _multisigWallet) ERC20("USDTgVerse Stablecoin", "USDTg") {
        require(_multisigWallet != address(0), "USDTgVerse: Invalid multisig wallet address");
        
        multisigWallet = _multisigWallet;
        
        // Mint initial supply to multisig wallet
        _mint(multisigWallet, MAX_SUPPLY);
        
        // Set initial daily limits for compliance
        dailyTransferLimits[multisigWallet] = MAX_SUPPLY;
        
        emit MultisigWalletUpdated(address(0), _multisigWallet);
    }
    
    /**
     * @dev Override transfer function with quantum-safe security
     */
    function transfer(address to, uint256 amount) 
        public 
        override 
        whenNotPaused 
        notBlacklisted(msg.sender) 
        notBlacklisted(to)
        withinDailyLimit(msg.sender, amount)
        quantumSafeTransfer(msg.sender, to, amount)
        returns (bool) 
    {
        require(amount >= MIN_BALANCE, "USDTgVerse: Transfer amount below minimum");
        
        bool success = super.transfer(to, amount);
        
        if (success) {
            emit QuantumSafeTransfer(msg.sender, to, amount);
            lastTransferTime[msg.sender] = block.timestamp;
        }
        
        return success;
    }
    
    /**
     * @dev Override transferFrom function with quantum-safe security
     */
    function transferFrom(address from, address to, uint256 amount) 
        public 
        override 
        whenNotPaused 
        notBlacklisted(from) 
        notBlacklisted(to)
        withinDailyLimit(from, amount)
        quantumSafeTransfer(from, to, amount)
        returns (bool) 
    {
        require(amount >= MIN_BALANCE, "USDTgVerse: Transfer amount below minimum");
        
        bool success = super.transferFrom(from, to, amount);
        
        if (success) {
            emit QuantumSafeTransfer(from, to, amount);
            lastTransferTime[from] = block.timestamp;
        }
        
        return success;
    }
    
    /**
     * @dev Mint new tokens (only multisig wallet)
     */
    function mint(address to, uint256 amount) 
        external 
        onlyMultisig 
        whenNotPaused 
        nonReentrant 
    {
        require(to != address(0), "USDTgVerse: Cannot mint to zero address");
        require(amount > 0, "USDTgVerse: Mint amount must be greater than zero");
        require(totalSupply().add(amount) <= MAX_SUPPLY, "USDTgVerse: Mint amount exceeds maximum supply");
        
        _mint(to, amount);
        
        emit QuantumSafeTransfer(address(0), to, amount);
    }
    
    /**
     * @dev Burn tokens (only multisig wallet)
     */
    function burn(address from, uint256 amount) 
        external 
        onlyMultisig 
        whenNotPaused 
        nonReentrant 
    {
        require(from != address(0), "USDTgVerse: Cannot burn from zero address");
        require(amount > 0, "USDTgVerse: Burn amount must be greater than zero");
        require(balanceOf(from) >= amount, "USDTgVerse: Insufficient balance to burn");
        
        _burn(from, amount);
        
        emit QuantumSafeTransfer(from, address(0), amount);
    }
    
    /**
     * @dev Update multisig wallet address
     */
    function updateMultisigWallet(address newMultisigWallet) 
        external 
        onlyMultisig 
        nonReentrant 
    {
        require(newMultisigWallet != address(0), "USDTgVerse: Invalid multisig wallet address");
        require(newMultisigWallet != multisigWallet, "USDTgVerse: Same multisig wallet address");
        
        address oldWallet = multisigWallet;
        multisigWallet = newMultisigWallet;
        
        // Transfer ownership to new multisig wallet
        _transferOwnership(newMultisigWallet);
        
        emit MultisigWalletUpdated(oldWallet, newMultisigWallet);
    }
    
    /**
     * @dev Whitelist an address for unlimited transfers
     */
    function whitelistAddress(address account) 
        external 
        onlyMultisig 
        nonReentrant 
    {
        require(account != address(0), "USDTgVerse: Invalid address");
        require(!whitelistedAddresses[account], "USDTgVerse: Address already whitelisted");
        
        whitelistedAddresses[account] = true;
        dailyTransferLimits[account] = 0; // Unlimited transfers
        
        emit AddressWhitelisted(account);
        emit ComplianceCheck(account, true);
    }
    
    /**
     * @dev Blacklist an address
     */
    function blacklistAddress(address account) 
        external 
        onlyMultisig 
        nonReentrant 
    {
        require(account != address(0), "USDTgVerse: Invalid address");
        require(account != multisigWallet, "USDTgVerse: Cannot blacklist multisig wallet");
        require(!blacklistedAddresses[account], "USDTgVerse: Address already blacklisted");
        
        blacklistedAddresses[account] = true;
        whitelistedAddresses[account] = false;
        dailyTransferLimits[account] = 0;
        
        emit AddressBlacklisted(account);
        emit ComplianceCheck(account, false);
    }
    
    /**
     * @dev Remove address from blacklist
     */
    function removeFromBlacklist(address account) 
        external 
        onlyMultisig 
        nonReentrant 
    {
        require(account != address(0), "USDTgVerse: Invalid address");
        require(blacklistedAddresses[account], "USDTgVerse: Address not blacklisted");
        
        blacklistedAddresses[account] = false;
        
        emit ComplianceCheck(account, true);
    }
    
    /**
     * @dev Set daily transfer limit for an address
     */
    function setDailyTransferLimit(address account, uint256 limit) 
        external 
        onlyMultisig 
        nonReentrant 
    {
        require(account != address(0), "USDTgVerse: Invalid address");
        
        dailyTransferLimits[account] = limit;
        
        emit DailyLimitUpdated(account, limit);
        emit ComplianceCheck(account, limit > 0);
    }
    
    /**
     * @dev Pause contract (only multisig wallet)
     */
    function pause() external onlyMultisig {
        _pause();
    }
    
    /**
     * @dev Unpause contract (only multisig wallet)
     */
    function unpause() external onlyMultisig {
        _unpause();
    }
    
    /**
     * @dev Get contract information
     */
    function getContractInfo() external view returns (
        string memory name,
        string memory symbol,
        uint256 totalSupply,
        uint256 maxSupply,
        address multisig,
        bool paused
    ) {
        return (
            name(),
            symbol(),
            totalSupply(),
            MAX_SUPPLY,
            multisigWallet,
            paused()
        );
    }
    
    /**
     * @dev Get address compliance status
     */
    function getAddressCompliance(address account) external view returns (
        bool isWhitelisted,
        bool isBlacklisted,
        uint256 dailyLimit,
        uint256 lastTransfer
    ) {
        return (
            whitelistedAddresses[account],
            blacklistedAddresses[account],
            dailyTransferLimits[account],
            lastTransferTime[account]
        );
    }
    
    /**
     * @dev Emergency function to recover accidentally sent tokens
     */
    function recoverTokens(address token, uint256 amount) 
        external 
        onlyMultisig 
        nonReentrant 
    {
        require(token != address(this), "USDTgVerse: Cannot recover USDTg tokens");
        require(amount > 0, "USDTgVerse: Amount must be greater than zero");
        
        IERC20(token).transfer(multisigWallet, amount);
    }
    
    /**
     * @dev Emergency function to recover accidentally sent ETH
     */
    function recoverETH() external onlyMultisig nonReentrant {
        uint256 balance = address(this).balance;
        require(balance > 0, "USDTgVerse: No ETH to recover");
        
        payable(multisigWallet).transfer(balance);
    }
    
    /**
     * @dev Receive function for ETH
     */
    receive() external payable {
        // Contract can receive ETH
    }
    
    /**
     * @dev Fallback function
     */
    fallback() external payable {
        // Fallback function
    }
}
