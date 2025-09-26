/*
==============================================
 File:        popup.js
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgWallet Chrome Extension - Popup Controller
   
   Quantum-Safe Web3 Wallet with 256-bit quantum security
   Multi-chain support and cross-platform consistency featuring:
   - Quantum-safe Web3 wallet
   - 256-bit quantum security
   - Multi-chain support
   - Cross-platform consistency
   - Browser extension integration

 License:
   MIT License
==============================================
*/

class USDTgWalletPopup {
    constructor() {
        this.currentWallet = null;
        this.currentNetwork = 'USDTgVerse';
        this.walletsList = [];
        this.isInitialized = false;
        
        this.init();
    }
    
    async init() {
        console.log('🚀 Initializing USDTgWallet Chrome Extension...');
        
        // 📊 Check if wallet exists
        await this.checkWalletExists();
        
        // 🎯 Setup event listeners
        this.setupEventListeners();
        
        // 🔄 Load wallet data if exists
        if (this.currentWallet) {
            await this.loadWalletData();
        }
        
        this.isInitialized = true;
        console.log('✅ USDTgWallet initialized successfully');
    }
    
    /**
     * 📊 CHECK IF WALLET EXISTS
     */
    async checkWalletExists() {
        try {
            const walletsList = await QuantumSafeCrypto.getWalletsList();
            this.walletsList = walletsList;
            
            if (walletsList.length > 0) {
                // Load first wallet as current
                const firstWalletName = walletsList[0];
                const walletData = await chrome.storage.local.get([`quantum_wallet_${firstWalletName}`]);
                
                if (walletData[`quantum_wallet_${firstWalletName}`]) {
                    this.currentWallet = await QuantumSafeCrypto.decryptWalletData(
                        walletData[`quantum_wallet_${firstWalletName}`]
                    );
                    this.showWalletView();
                } else {
                    this.showSetupScreen();
                }
            } else {
                this.showSetupScreen();
            }
        } catch (error) {
            console.error('Error checking wallet:', error);
            this.showSetupScreen();
        }
    }
    
    /**
     * 🎯 SETUP EVENT LISTENERS
     */
    setupEventListeners() {
        // 🔐 Wallet Setup
        document.getElementById('create-wallet-btn')?.addEventListener('click', () => {
            this.showModal('create-wallet-modal');
        });
        
        document.getElementById('import-wallet-btn')?.addEventListener('click', () => {
            this.showModal('import-wallet-modal');
        });
        
        // 🔐 Create Wallet Modal
        document.getElementById('close-create-modal')?.addEventListener('click', () => {
            this.hideModal('create-wallet-modal');
        });
        
        document.getElementById('confirm-create-btn')?.addEventListener('click', () => {
            this.createQuantumWallet();
        });
        
        // 📱 Import Wallet Modal
        document.getElementById('close-import-modal')?.addEventListener('click', () => {
            this.hideModal('import-wallet-modal');
        });
        
        document.getElementById('confirm-import-btn')?.addEventListener('click', () => {
            this.importQuantumWallet();
        });
        
        document.getElementById('qr-scan-btn')?.addEventListener('click', () => {
            this.showNotification('📷 QR Scanner will be implemented in next version');
        });
        
        // 🚀 Quick Actions
        document.getElementById('send-btn')?.addEventListener('click', () => {
            this.showNotification('📤 Send feature coming soon');
        });
        
        document.getElementById('receive-btn')?.addEventListener('click', () => {
            this.showModal('receive-modal');
        });
        
        document.getElementById('swap-btn')?.addEventListener('click', () => {
            this.showNotification('🔄 Swap feature coming soon');
        });
        
        document.getElementById('bridge-btn')?.addEventListener('click', () => {
            this.showNotification('🌉 Bridge feature coming soon');
        });
        
        // 📥 Receive Modal
        document.getElementById('close-receive-modal')?.addEventListener('click', () => {
            this.hideModal('receive-modal');
        });
        
        document.getElementById('receive-network')?.addEventListener('change', (e) => {
            this.updateReceiveAddress(e.target.value);
        });
        
        document.getElementById('copy-address-btn')?.addEventListener('click', () => {
            this.copyToClipboard();
        });
        
        document.getElementById('share-qr-btn')?.addEventListener('click', () => {
            this.shareQRCode();
        });
        
        // ⚙️ Settings
        document.getElementById('wallet-menu-btn')?.addEventListener('click', () => {
            this.showModal('settings-modal');
        });
        
        document.getElementById('close-settings-modal')?.addEventListener('click', () => {
            this.hideModal('settings-modal');
        });
        
        document.getElementById('backup-wallet-btn')?.addEventListener('click', () => {
            this.backupWallet();
        });
        
        document.getElementById('add-wallet-btn')?.addEventListener('click', () => {
            this.hideModal('settings-modal');
            this.showModal('create-wallet-modal');
        });
        
        document.getElementById('about-btn')?.addEventListener('click', () => {
            this.showAbout();
        });
        
        // 🌐 Network Selector
        document.getElementById('network-selector')?.addEventListener('click', () => {
            this.showNetworkSelector();
        });
        
        // 📢 Notifications
        document.getElementById('close-notification')?.addEventListener('click', () => {
            this.hideNotification();
        });
    }
    
    /**
     * 🔐 CREATE QUANTUM WALLET
     */
    async createQuantumWallet() {
        const walletName = document.getElementById('new-wallet-name').value.trim();
        
        if (!walletName) {
            this.showNotification('❌ Please enter a wallet name');
            return;
        }
        
        this.showLoading('Creating quantum-safe wallet...');
        
        try {
            // 🔐 Generate quantum-safe mnemonic
            const mnemonic = await QuantumSafeCrypto.generateQuantumSafeMnemonic(24);
            
            // 🏠 Generate quantum-safe addresses
            const addresses = await QuantumSafeCrypto.generateQuantumSafeAddresses(mnemonic);
            
            // 🔑 Create wallet data
            const walletData = {
                name: walletName,
                mnemonic: mnemonic,
                addresses: addresses,
                network: 'USDTgVerse',
                createdAt: Date.now(),
                quantumSafe: true,
                balance: {
                    USDTg: 10.00, // Welcome bonus
                    total: 10.00
                }
            };
            
            // 💾 Store wallet securely
            await QuantumSafeCrypto.securelyStoreQuantumWallet(walletData);
            
            // 📊 Update wallets list
            this.walletsList.push(walletName);
            await chrome.storage.local.set({ 'quantum_wallets_list': this.walletsList });
            
            this.currentWallet = walletData;
            
            this.hideLoading();
            this.hideModal('create-wallet-modal');
            this.showWalletView();
            this.showNotification('🎉 Quantum wallet created successfully! 10 USDTg bonus added.');
            
        } catch (error) {
            console.error('Error creating wallet:', error);
            this.hideLoading();
            this.showNotification('❌ Failed to create wallet. Please try again.');
        }
    }
    
    /**
     * 📱 IMPORT QUANTUM WALLET
     */
    async importQuantumWallet() {
        const walletName = document.getElementById('import-wallet-name').value.trim();
        const seedPhrase = document.getElementById('seed-phrase').value.trim();
        const network = document.getElementById('import-network').value;
        
        if (!walletName || !seedPhrase) {
            this.showNotification('❌ Please fill in all fields');
            return;
        }
        
        // 🛡️ Validate quantum-safe mnemonic
        if (!await QuantumSafeCrypto.validateQuantumSafeMnemonic(seedPhrase)) {
            this.showNotification('❌ For quantum-safe security, please enter a 24-48 word seed phrase with minimum 256-bit entropy.');
            return;
        }
        
        this.showLoading('Importing quantum-safe wallet...');
        
        try {
            // 🏠 Generate addresses from mnemonic
            const addresses = await QuantumSafeCrypto.generateQuantumSafeAddresses(seedPhrase);
            
            // 🔑 Create wallet data
            const walletData = {
                name: walletName,
                mnemonic: seedPhrase,
                addresses: addresses,
                network: network,
                createdAt: Date.now(),
                quantumSafe: true,
                imported: true,
                balance: {
                    USDTg: 0.00,
                    total: 0.00
                }
            };
            
            // 💾 Store wallet securely
            await QuantumSafeCrypto.securelyStoreQuantumWallet(walletData);
            
            // 📊 Update wallets list
            this.walletsList.push(walletName);
            await chrome.storage.local.set({ 'quantum_wallets_list': this.walletsList });
            
            this.currentWallet = walletData;
            
            this.hideLoading();
            this.hideModal('import-wallet-modal');
            this.showWalletView();
            this.showNotification('🎉 Quantum wallet imported successfully!');
            
        } catch (error) {
            console.error('Error importing wallet:', error);
            this.hideLoading();
            this.showNotification('❌ Failed to import wallet. Please check your seed phrase.');
        }
    }
    
    /**
     * 📊 LOAD WALLET DATA
     */
    async loadWalletData() {
        if (!this.currentWallet) return;
        
        // 📝 Update UI with wallet data
        document.getElementById('wallet-name').textContent = this.currentWallet.name;
        document.getElementById('total-value').textContent = `$${this.currentWallet.balance.total.toFixed(2)}`;
        document.getElementById('usdtg-balance').textContent = this.currentWallet.balance.USDTg.toFixed(2);
        document.getElementById('current-network').textContent = this.currentWallet.network || 'USDTgVerse';
        
        // 📈 Load assets
        this.loadAssetsList();
        
        // 📋 Load transactions
        this.loadTransactionsList();
    }
    
    /**
     * 📈 LOAD ASSETS LIST
     */
    loadAssetsList() {
        const assetsList = document.getElementById('assets-list');
        if (!assetsList) return;
        
        const assets = [
            { symbol: 'USDTg', name: 'USDTgVerse Token', balance: this.currentWallet.balance.USDTg, price: 1.00 },
            { symbol: 'USDT', name: 'Tether USD', balance: 0.00, price: 1.00 },
            { symbol: 'USDC', name: 'USD Coin', balance: 0.00, price: 1.00 }
        ];
        
        assetsList.innerHTML = assets.map(asset => `
            <div class="asset-item">
                <div class="asset-icon">${asset.symbol.substring(0, 2)}</div>
                <div class="asset-info">
                    <div class="asset-symbol">${asset.symbol}</div>
                    <div class="asset-name">${asset.name}</div>
                </div>
                <div class="asset-balance">
                    <div class="balance-amount">${asset.balance.toFixed(6)}</div>
                    <div class="balance-value">$${(asset.balance * asset.price).toFixed(2)}</div>
                </div>
            </div>
        `).join('');
    }
    
    /**
     * 📋 LOAD TRANSACTIONS LIST
     */
    loadTransactionsList() {
        const transactionsList = document.getElementById('transactions-list');
        if (!transactionsList) return;
        
        const transactions = this.currentWallet.imported ? 
            [] : // No demo transactions for imported wallets
            [
                { type: 'received', amount: 10.00, asset: 'USDTg', hash: 'quantum_welcome_bonus', time: 'Just now' }
            ];
        
        if (transactions.length === 0) {
            transactionsList.innerHTML = '<div class="no-transactions">No transactions yet</div>';
            return;
        }
        
        transactionsList.innerHTML = transactions.map(tx => `
            <div class="transaction-item">
                <div class="transaction-icon ${tx.type}">${tx.type === 'received' ? '📥' : '📤'}</div>
                <div class="transaction-info">
                    <div class="transaction-type">${tx.type === 'received' ? 'Received' : 'Sent'}</div>
                    <div class="transaction-hash">${tx.hash}</div>
                </div>
                <div class="transaction-amount">
                    <div class="amount ${tx.type}">${tx.type === 'received' ? '+' : '-'}${tx.amount} ${tx.asset}</div>
                    <div class="time">${tx.time}</div>
                </div>
            </div>
        `).join('');
    }
    
    /**
     * 📥 UPDATE RECEIVE ADDRESS
     */
    async updateReceiveAddress(network) {
        if (!this.currentWallet || !this.currentWallet.addresses) return;
        
        const address = this.currentWallet.addresses[network] || 'USDTg1qABC123...XYZ789';
        document.getElementById('wallet-address').textContent = address;
        
        // 📱 Generate QR code (placeholder)
        this.generateQRCode(address, network);
    }
    
    /**
     * 📱 GENERATE QR CODE
     */
    generateQRCode(address, network) {
        const qrDisplay = document.getElementById('qr-code-display');
        if (!qrDisplay) return;
        
        // For now, show placeholder - in production, use QR code library
        qrDisplay.innerHTML = `
            <div class="qr-placeholder">
                <div class="qr-code-mock">📱</div>
                <p>QR Code for ${network}</p>
                <small>${address.substring(0, 20)}...</small>
            </div>
        `;
    }
    
    /**
     * 📋 COPY TO CLIPBOARD
     */
    async copyToClipboard() {
        const address = document.getElementById('wallet-address').textContent;
        
        try {
            await navigator.clipboard.writeText(address);
            this.showNotification('📋 Address copied to clipboard');
        } catch (error) {
            console.error('Failed to copy:', error);
            this.showNotification('❌ Failed to copy address');
        }
    }
    
    /**
     * 📤 SHARE QR CODE
     */
    shareQRCode() {
        const address = document.getElementById('wallet-address').textContent;
        const network = document.getElementById('receive-network').value;
        
        if (navigator.share) {
            navigator.share({
                title: 'USDTgWallet Address',
                text: `My ${network} wallet address: ${address}`,
                url: `https://usdtgverse.com/address/${address}`
            });
        } else {
            this.copyToClipboard();
        }
    }
    
    /**
     * 💾 BACKUP WALLET
     */
    async backupWallet() {
        if (!this.currentWallet) return;
        
        const backupData = {
            name: this.currentWallet.name,
            mnemonic: this.currentWallet.mnemonic,
            network: this.currentWallet.network,
            quantum_safe: true,
            backup_date: new Date().toISOString()
        };
        
        const backupText = JSON.stringify(backupData, null, 2);
        
        // Create download
        const blob = new Blob([backupText], { type: 'application/json' });
        const url = URL.createObjectURL(blob);
        
        const a = document.createElement('a');
        a.href = url;
        a.download = `usdtgwallet-backup-${this.currentWallet.name}-${Date.now()}.json`;
        document.body.appendChild(a);
        a.click();
        document.body.removeChild(a);
        URL.revokeObjectURL(url);
        
        this.hideModal('settings-modal');
        this.showNotification('💾 Wallet backup downloaded successfully');
    }
    
    /**
     * ℹ️ SHOW ABOUT
     */
    showAbout() {
        const securityLevel = QuantumSafeCrypto.getQuantumSecurityLevel();
        
        const aboutText = `🔐 USDTgWallet Chrome Extension
        
⚛️ Quantum Security Level: ${securityLevel.security_equivalent}
🔑 Entropy: ${securityLevel.entropy_bits}-bit
📝 Mnemonic: ${securityLevel.min_words}-${securityLevel.max_words} words
🌐 Networks: 8 supported chains
🛡️ Post-Quantum Ready: ${securityLevel.post_quantum_ready ? 'Yes' : 'No'}

Version: ${securityLevel.version}
Platform: ${securityLevel.platform}

🌟 World's first quantum-safe Web3 wallet`;
        
        alert(aboutText);
        this.hideModal('settings-modal');
    }
    
    /**
     * 🎨 UI HELPER METHODS
     */
    showSetupScreen() {
        document.getElementById('setup-screen').classList.remove('hidden');
        document.getElementById('wallet-view').classList.add('hidden');
    }
    
    showWalletView() {
        document.getElementById('setup-screen').classList.add('hidden');
        document.getElementById('wallet-view').classList.remove('hidden');
    }
    
    showModal(modalId) {
        document.getElementById(modalId).classList.remove('hidden');
    }
    
    hideModal(modalId) {
        document.getElementById(modalId).classList.add('hidden');
    }
    
    showLoading(message) {
        document.getElementById('loading-message').textContent = message;
        document.getElementById('loading-overlay').classList.remove('hidden');
    }
    
    hideLoading() {
        document.getElementById('loading-overlay').classList.add('hidden');
    }
    
    showNotification(message) {
        document.getElementById('notification-message').textContent = message;
        document.getElementById('notification-toast').classList.remove('hidden');
        
        // Auto hide after 3 seconds
        setTimeout(() => {
            this.hideNotification();
        }, 3000);
    }
    
    hideNotification() {
        document.getElementById('notification-toast').classList.add('hidden');
    }
    
    showNetworkSelector() {
        // TODO: Implement network selector dropdown
        this.showNotification('🌐 Network selector coming soon');
    }
}

// 🚀 Initialize when DOM is loaded
document.addEventListener('DOMContentLoaded', () => {
    window.usdtgWallet = new USDTgWalletPopup();
});

// 🔄 Handle extension reload
chrome.runtime.onStartup.addListener(() => {
    console.log('🔄 USDTgWallet extension reloaded');
});
