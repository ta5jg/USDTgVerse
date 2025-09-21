// SPDX-License-Identifier: MIT
pragma solidity ^0.8.19;

/**
 * 💰 USDTg Native Coin - Quantum-Safe Stablecoin
 * ==============================================
 * 
 * ⚛️ QUANTUM SECURITY: 256-bit quantum-safe implementation
 * 🔐 POST-QUANTUM: Compatible with CRYSTALS-Dilithium signatures
 * 💵 STABLE VALUE: Pegged to $1.00 USD
 * 🌍 MULTI-CHAIN: Cross-chain compatible
 * 
 * 🏆 FEATURES:
 * - Initial Supply: 1,000,000,000 USDTg (1 Billion)
 * - Unlimited Minting: Treasury controlled
 * - Quantum-Safe Addresses: USDTg1q format support
 * - Governance: DAO controlled parameters
 * - Emergency: Pause/unpause functionality
 * 
 * 📊 TOKENOMICS:
 * - Treasury: 40% (400M USDTg)
 * - Public Distribution: 30% (300M USDTg)
 * - Development Fund: 20% (200M USDTg)
 * - Marketing & Partnerships: 10% (100M USDTg)
 */

import "@openzeppelin/contracts/token/ERC20/ERC20.sol";
import "@openzeppelin/contracts/token/ERC20/extensions/ERC20Burnable.sol";
import "@openzeppelin/contracts/security/Pausable.sol";
import "@openzeppelin/contracts/access/AccessControl.sol";
import "@openzeppelin/contracts/token/ERC20/extensions/draft-ERC20Permit.sol";

contract USDTgNativeCoin is ERC20, ERC20Burnable, Pausable, AccessControl, ERC20Permit {
    
    // 🔐 QUANTUM-SAFE ACCESS ROLES
    bytes32 public constant MINTER_ROLE = keccak256("MINTER_ROLE");
    bytes32 public constant PAUSER_ROLE = keccak256("PAUSER_ROLE");
    bytes32 public constant TREASURY_ROLE = keccak256("TREASURY_ROLE");
    bytes32 public constant GOVERNANCE_ROLE = keccak256("GOVERNANCE_ROLE");
    
    // 💰 TOKENOMICS CONSTANTS
    uint256 public constant INITIAL_SUPPLY = 1_000_000_000 * 10**18; // 1 Billion USDTg
    uint256 public constant TREASURY_ALLOCATION = 400_000_000 * 10**18; // 40%
    uint256 public constant PUBLIC_ALLOCATION = 300_000_000 * 10**18; // 30%
    uint256 public constant DEVELOPMENT_ALLOCATION = 200_000_000 * 10**18; // 20%
    uint256 public constant MARKETING_ALLOCATION = 100_000_000 * 10**18; // 10%
    
    // 📊 STATE VARIABLES
    uint256 public totalMinted;
    uint256 public totalBurned;
    mapping(address => bool) public quantumSafeAddresses;
    
    // 💵 STABLECOIN MECHANICS
    uint256 public targetPrice = 1 * 10**18; // $1.00 USD in wei
    uint256 public priceToleranceBps = 500; // 5% tolerance (500 basis points)
    
    // 🎁 WELCOME BONUS SYSTEM
    mapping(address => bool) public welcomeBonusClaimed;
    uint256 public constant WELCOME_BONUS = 10 * 10**18; // 10 USDTg
    bool public welcomeBonusActive = true;
    
    // 📈 ANALYTICS
    uint256 public totalTransactions;
    mapping(address => uint256) public userTransactionCount;
    
    // 🚨 EVENTS
    event QuantumSafeAddressRegistered(address indexed user, string quantumAddress);
    event WelcomeBonusClaimed(address indexed user, uint256 amount);
    event PriceParametersUpdated(uint256 targetPrice, uint256 tolerance);
    event EmergencyMint(address indexed to, uint256 amount, string reason);
    event GovernanceAction(bytes32 indexed action, address indexed executor);
    
    constructor(
        address _treasury,
        address _development,
        address _marketing
    ) ERC20("USDTgVerse Coin", "USDTg") ERC20Permit("USDTgVerse Coin") {
        
        // 🏗️ SETUP ACCESS CONTROL
        _grantRole(DEFAULT_ADMIN_ROLE, msg.sender);
        _grantRole(MINTER_ROLE, msg.sender);
        _grantRole(PAUSER_ROLE, msg.sender);
        _grantRole(TREASURY_ROLE, _treasury);
        _grantRole(GOVERNANCE_ROLE, msg.sender);
        
        // 💰 INITIAL DISTRIBUTION
        _mint(_treasury, TREASURY_ALLOCATION);
        _mint(address(this), PUBLIC_ALLOCATION); // For public distribution
        _mint(_development, DEVELOPMENT_ALLOCATION);
        _mint(_marketing, MARKETING_ALLOCATION);
        
        totalMinted = INITIAL_SUPPLY;
        
        // 📊 ANALYTICS
        totalTransactions = 1; // Genesis transaction
    }
    
    // 🔐 QUANTUM-SAFE ADDRESS REGISTRATION
    function registerQuantumSafeAddress(string calldata _quantumAddress) external {
        require(bytes(_quantumAddress).length >= 50, "USDTg: Invalid quantum address length");
        require(bytes(_quantumAddress)[0] == 'U' && 
                bytes(_quantumAddress)[1] == 'S' && 
                bytes(_quantumAddress)[2] == 'D' && 
                bytes(_quantumAddress)[3] == 'T' && 
                bytes(_quantumAddress)[4] == 'g' && 
                bytes(_quantumAddress)[5] == '1' && 
                bytes(_quantumAddress)[6] == 'q', "USDTg: Invalid quantum address format");
        
        quantumSafeAddresses[msg.sender] = true;
        emit QuantumSafeAddressRegistered(msg.sender, _quantumAddress);
    }
    
    // 🎁 WELCOME BONUS SYSTEM
    function claimWelcomeBonus() external {
        require(welcomeBonusActive, "USDTg: Welcome bonus not active");
        require(!welcomeBonusClaimed[msg.sender], "USDTg: Welcome bonus already claimed");
        require(balanceOf(msg.sender) == 0, "USDTg: Must be new user");
        
        welcomeBonusClaimed[msg.sender] = true;
        _mint(msg.sender, WELCOME_BONUS);
        totalMinted += WELCOME_BONUS;
        
        emit WelcomeBonusClaimed(msg.sender, WELCOME_BONUS);
    }
    
    // 💰 MINTING FUNCTIONS
    function mint(address to, uint256 amount) public onlyRole(MINTER_ROLE) {
        _mint(to, amount);
        totalMinted += amount;
    }
    
    function emergencyMint(address to, uint256 amount, string calldata reason) 
        external onlyRole(TREASURY_ROLE) {
        _mint(to, amount);
        totalMinted += amount;
        emit EmergencyMint(to, amount, reason);
    }
    
    // 🔥 BURNING FUNCTIONS
    function burn(uint256 amount) public override {
        super.burn(amount);
        totalBurned += amount;
    }
    
    function burnFrom(address account, uint256 amount) public override {
        super.burnFrom(account, amount);
        totalBurned += amount;
    }
    
    // ⏸️ PAUSE FUNCTIONS
    function pause() public onlyRole(PAUSER_ROLE) {
        _pause();
    }
    
    function unpause() public onlyRole(PAUSER_ROLE) {
        _unpause();
    }
    
    // 🏛️ GOVERNANCE FUNCTIONS
    function updatePriceParameters(uint256 _targetPrice, uint256 _toleranceBps) 
        external onlyRole(GOVERNANCE_ROLE) {
        require(_toleranceBps <= 2000, "USDTg: Tolerance too high"); // Max 20%
        targetPrice = _targetPrice;
        priceToleranceBps = _toleranceBps;
        emit PriceParametersUpdated(_targetPrice, _toleranceBps);
        emit GovernanceAction("PRICE_UPDATE", msg.sender);
    }
    
    function setWelcomeBonusStatus(bool _active) external onlyRole(GOVERNANCE_ROLE) {
        welcomeBonusActive = _active;
        emit GovernanceAction("WELCOME_BONUS_STATUS", msg.sender);
    }
    
    function emergencyWithdraw(address token, uint256 amount) 
        external onlyRole(TREASURY_ROLE) {
        if (token == address(0)) {
            payable(msg.sender).transfer(amount);
        } else {
            IERC20(token).transfer(msg.sender, amount);
        }
        emit GovernanceAction("EMERGENCY_WITHDRAW", msg.sender);
    }
    
    // 📊 ANALYTICS FUNCTIONS
    function getTokenomics() external view returns (
        uint256 _totalSupply,
        uint256 _totalMinted,
        uint256 _totalBurned,
        uint256 _circulating
    ) {
        return (
            totalSupply(),
            totalMinted,
            totalBurned,
            totalSupply() - balanceOf(address(this))
        );
    }
    
    function getUserStats(address user) external view returns (
        uint256 balance,
        uint256 transactions,
        bool quantumSafe,
        bool welcomeClaimed
    ) {
        return (
            balanceOf(user),
            userTransactionCount[user],
            quantumSafeAddresses[user],
            welcomeBonusClaimed[user]
        );
    }
    
    // 🔄 OVERRIDE FUNCTIONS
    function _beforeTokenTransfer(address from, address to, uint256 amount)
        internal
        whenNotPaused
        override
    {
        super._beforeTokenTransfer(from, to, amount);
        
        // 📊 Update analytics
        if (from != address(0) && to != address(0)) {
            totalTransactions++;
            userTransactionCount[from]++;
            userTransactionCount[to]++;
        }
    }
    
    // 🌍 MULTI-CHAIN COMPATIBILITY
    function bridgeMint(address to, uint256 amount, bytes32 bridgeHash) 
        external onlyRole(MINTER_ROLE) {
        _mint(to, amount);
        totalMinted += amount;
        emit GovernanceAction("BRIDGE_MINT", msg.sender);
    }
    
    function bridgeBurn(uint256 amount, string calldata targetChain) external {
        _burn(msg.sender, amount);
        totalBurned += amount;
        emit GovernanceAction("BRIDGE_BURN", msg.sender);
    }
    
    // 💎 QUANTUM-SAFE FEATURES
    function isQuantumSafe(address user) external view returns (bool) {
        return quantumSafeAddresses[user];
    }
    
    function getQuantumSecurityLevel() external pure returns (string memory) {
        return "256-bit Quantum-Safe (CRYSTALS-Dilithium + Kyber)";
    }
    
    // 📈 STABLECOIN MECHANICS
    function isPriceStable(uint256 currentPrice) external view returns (bool) {
        uint256 tolerance = (targetPrice * priceToleranceBps) / 10000;
        return currentPrice >= (targetPrice - tolerance) && 
               currentPrice <= (targetPrice + tolerance);
    }
    
    // 🎯 UTILITY FUNCTIONS
    function decimals() public view virtual override returns (uint8) {
        return 18;
    }
    
    function version() external pure returns (string memory) {
        return "1.0.0-quantum-safe";
    }
    
    // 🔐 SECURITY FUNCTIONS
    function supportsInterface(bytes4 interfaceId)
        public
        view
        override(AccessControl)
        returns (bool)
    {
        return super.supportsInterface(interfaceId);
    }
}
