// USDTgVerse Safari Extension - Background Script
// Real Blockchain Integration - No Demo Data

class USDTgVerseSafariBackground {
    constructor() {
        this.apiBase = 'https://api.usdtgverse.com/api/v1';
        this.init();
    }

    init() {
        // Listen for extension installation
        chrome.runtime.onInstalled.addListener(() => {
            console.log('USDTgVerse Safari Extension installed');
            this.initializeWallet();
        });

        // Listen for messages from content scripts
        chrome.runtime.onMessage.addListener((request, sender, sendResponse) => {
            this.handleMessage(request, sender, sendResponse);
            return true; // Keep message channel open for async response
        });
    }

    async initializeWallet() {
        // Check if wallet exists
        const walletAddress = await this.getFromStorage('walletAddress');
        
        if (!walletAddress) {
            // Generate new wallet
            await this.generateNewWallet();
        }
    }

    async getFromStorage(key) {
        return new Promise((resolve) => {
            chrome.storage.local.get([key], (result) => {
                resolve(result[key] || '');
            });
        });
    }

    async setToStorage(key, value) {
        return new Promise((resolve) => {
            chrome.storage.local.set({ [key]: value }, () => {
                resolve();
            });
        });
    }

    async generateNewWallet() {
        try {
            const response = await fetch(`${this.apiBase}/wallet/generate`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                }
            });
            
            const data = await response.json();
            const walletAddress = data.address;
            
            // Save to storage
            await this.setToStorage('walletAddress', walletAddress);
            
            // Request welcome airdrop
            await this.requestWelcomeAirDrop(walletAddress);
            
            console.log('New wallet generated:', walletAddress);
            
        } catch (error) {
            console.error('Error generating wallet:', error);
        }
    }

    async requestWelcomeAirDrop(walletAddress) {
        try {
            const response = await fetch(`${this.apiBase}/airdrop/welcome`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    address: walletAddress,
                    type: 'welcome'
                })
            });
            
            const data = await response.json();
            if (data.success) {
                console.log('Welcome airdrop requested for:', walletAddress);
            }
        } catch (error) {
            console.error('Error requesting airdrop:', error);
        }
    }

    async handleMessage(request, sender, sendResponse) {
        try {
            switch (request.action) {
                case 'getWalletAddress':
                    const walletAddress = await this.getFromStorage('walletAddress');
                    sendResponse({ success: true, address: walletAddress });
                    break;
                
                case 'getBalance':
                    const address = await this.getFromStorage('walletAddress');
                    const balance = await this.fetchBalance(address);
                    sendResponse({ success: true, balance: balance });
                    break;
                
                case 'sendTransaction':
                    const result = await this.sendTransaction(request.data);
                    sendResponse({ success: true, result: result });
                    break;
                
                case 'getTransactions':
                    const txAddress = await this.getFromStorage('walletAddress');
                    const transactions = await this.fetchTransactions(txAddress);
                    sendResponse({ success: true, transactions: transactions });
                    break;
                
                default:
                    sendResponse({ success: false, error: 'Unknown action' });
            }
        } catch (error) {
            console.error('Error handling message:', error);
            sendResponse({ success: false, error: error.message });
        }
    }

    async fetchBalance(walletAddress) {
        try {
            const response = await fetch(`${this.apiBase}/balance/${walletAddress}/usdtg`);
            const data = await response.json();
            return data.balance || 0.0;
        } catch (error) {
            console.error('Error fetching balance:', error);
            return 0.0;
        }
    }

    async fetchTransactions(walletAddress) {
        try {
            const response = await fetch(`${this.apiBase}/transactions/${walletAddress}`);
            const data = await response.json();
            return data.transactions || [];
        } catch (error) {
            console.error('Error fetching transactions:', error);
            return [];
        }
    }

    async sendTransaction(transactionData) {
        try {
            const response = await fetch(`${this.apiBase}/transaction/send`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify(transactionData)
            });
            
            const data = await response.json();
            return data;
        } catch (error) {
            console.error('Error sending transaction:', error);
            return { success: false, error: error.message };
        }
    }
}

// Initialize background script
new USDTgVerseSafariBackground();
