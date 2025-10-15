/*
==============================================
 File:        airdrop-manager.js
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Browser Extension Airdrop Manager
   
   Shared airdrop manager for Chrome, Safari, Firefox:
   - Request 10 USDTg locked airdrop
   - Pay fees with locked airdrop
   - Verify purchases and unlock
   - Cross-browser compatible

 License:
   MIT License
==============================================
*/

class BrowserAirdropManager {
    constructor() {
        this.apiBase = 'http://localhost:3006';
        this.airdropAmount = 10.0;
        this.minPurchaseUnlock = 50.0;
    }

    /**
     * Request 10 USDTg airdrop for new wallet (LOCKED - Fee only)
     */
    async requestAirdrop(walletAddress, userId) {
        try {
            const deviceFingerprint = await this.getDeviceFingerprint();
            
            const response = await fetch(`${this.apiBase}/api/airdrop/create`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    wallet_address: walletAddress,
                    user_id: userId,
                    device_fingerprint: deviceFingerprint,
                    ip_address: '0.0.0.0'
                })
            });

            const data = await response.json();

            if (data.success) {
                console.log('[Airdrop] ✅ Received:', data.airdrop_id);
                console.log('[Airdrop] 💰 Amount:', data.amount, 'USDTg (LOCKED)');
                
                // Store airdrop info in extension storage
                await this.storeAirdropInfo(walletAddress, data);
                
                // Show notification
                this.showBrowserNotification(
                    'Welcome Airdrop!',
                    `You received ${data.amount} USDTg (Locked for fees only)`
                );

                return {
                    success: true,
                    airdropId: data.airdrop_id,
                    amount: data.amount,
                    message: data.message
                };
            } else {
                console.error('[Airdrop] ❌ Failed:', data.error);
                return {
                    success: false,
                    error: data.error
                };
            }
        } catch (error) {
            console.error('[Airdrop] ❌ Request failed:', error);
            return {
                success: false,
                error: error.message
            };
        }
    }

    /**
     * Use locked airdrop to pay transaction fee
     */
    async payFeeWithAirdrop(walletAddress, feeAmount) {
        try {
            const response = await fetch(`${this.apiBase}/api/airdrop/use-fee`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    wallet_address: walletAddress,
                    fee_amount: feeAmount
                })
            });

            const data = await response.json();

            if (data.success) {
                console.log('[Airdrop] ✅ Fee paid:', feeAmount, 'USDTg');
                console.log('[Airdrop] 💰 Remaining:', data.remaining, 'USDTg');
                
                // Update stored airdrop info
                await this.updateAirdropBalance(walletAddress, data.remaining);
                
                return true;
            } else {
                console.error('[Airdrop] ❌ Fee payment failed');
                return false;
            }
        } catch (error) {
            console.error('[Airdrop] ❌ Fee payment failed:', error);
            return false;
        }
    }

    /**
     * Verify purchase and unlock airdrop if threshold met
     */
    async verifyPurchaseAndUnlock(walletAddress, purchaseAmount) {
        try {
            const response = await fetch(`${this.apiBase}/api/airdrop/verify-purchase`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    wallet_address: walletAddress,
                    purchase_amount: purchaseAmount
                })
            });

            const data = await response.json();

            if (data.success) {
                if (data.unlocked) {
                    console.log('[Airdrop] 🔓 UNLOCKED!');
                    
                    // Update stored airdrop info
                    await this.markAirdropUnlocked(walletAddress);
                    
                    // Show notification
                    this.showBrowserNotification(
                        'Airdrop Unlocked!',
                        '10 USDTg now available for all transactions'
                    );
                    
                    return {
                        unlocked: true,
                        message: data.message
                    };
                } else {
                    console.log('[Airdrop] 💰 Need', data.remaining_to_unlock, 'more');
                    return {
                        unlocked: false,
                        remainingToUnlock: data.remaining_to_unlock,
                        message: data.message
                    };
                }
            } else {
                return {
                    unlocked: false,
                    error: data.error
                };
            }
        } catch (error) {
            console.error('[Airdrop] ❌ Verification failed:', error);
            return {
                unlocked: false,
                error: error.message
            };
        }
    }

    /**
     * Get airdrop status for wallet
     */
    async getAirdropStatus(walletAddress) {
        try {
            const response = await fetch(
                `${this.apiBase}/api/airdrop/status?wallet_address=${walletAddress}`
            );

            const data = await response.json();

            if (data.success) {
                return {
                    success: true,
                    airdropId: data.airdrop_id,
                    walletAddress: data.wallet_address,
                    airdropAmount: data.airdrop_amount,
                    lockedAmount: data.locked_amount,
                    usedAmount: data.used_amount,
                    purchaseAmount: data.purchase_amount,
                    status: data.status,
                    unlocked: data.unlocked,
                    feeUsageCount: data.fee_usage_count
                };
            } else {
                return {
                    success: false,
                    error: data.error
                };
            }
        } catch (error) {
            console.error('[Airdrop] ❌ Status check failed:', error);
            return {
                success: false,
                error: error.message
            };
        }
    }

    /**
     * Store airdrop info in extension storage
     */
    async storeAirdropInfo(walletAddress, airdropData) {
        try {
            const storage = this.getStorage();
            const key = `airdrop_${walletAddress}`;
            
            const data = {
                airdropId: airdropData.airdrop_id,
                amount: airdropData.amount,
                lockedAmount: airdropData.amount,
                usedAmount: 0,
                purchaseAmount: 0,
                unlocked: false,
                createdAt: Date.now()
            };
            
            await storage.set({ [key]: data });
            console.log('[Airdrop] 💾 Stored airdrop info');
        } catch (error) {
            console.error('[Airdrop] ❌ Storage failed:', error);
        }
    }

    /**
     * Update airdrop balance in storage
     */
    async updateAirdropBalance(walletAddress, newBalance) {
        try {
            const storage = this.getStorage();
            const key = `airdrop_${walletAddress}`;
            
            const result = await storage.get(key);
            if (result[key]) {
                result[key].lockedAmount = newBalance;
                await storage.set({ [key]: result[key] });
            }
        } catch (error) {
            console.error('[Airdrop] ❌ Balance update failed:', error);
        }
    }

    /**
     * Mark airdrop as unlocked in storage
     */
    async markAirdropUnlocked(walletAddress) {
        try {
            const storage = this.getStorage();
            const key = `airdrop_${walletAddress}`;
            
            const result = await storage.get(key);
            if (result[key]) {
                result[key].unlocked = true;
                await storage.set({ [key]: result[key] });
            }
        } catch (error) {
            console.error('[Airdrop] ❌ Unlock mark failed:', error);
        }
    }

    /**
     * Get device fingerprint for fraud detection
     */
    async getDeviceFingerprint() {
        const browser = this.detectBrowser();
        const platform = navigator.platform;
        const language = navigator.language;
        const timestamp = Date.now();
        
        const fingerprint = `${browser}_${platform}_${language}_${timestamp}`;
        return this.simpleHash(fingerprint);
    }

    /**
     * Detect browser type
     */
    detectBrowser() {
        const userAgent = navigator.userAgent;
        
        if (userAgent.indexOf('Chrome') > -1) return 'Chrome';
        if (userAgent.indexOf('Safari') > -1) return 'Safari';
        if (userAgent.indexOf('Firefox') > -1) return 'Firefox';
        if (userAgent.indexOf('Edge') > -1) return 'Edge';
        
        return 'Unknown';
    }

    /**
     * Get cross-browser storage API
     */
    getStorage() {
        // Chrome/Edge
        if (typeof chrome !== 'undefined' && chrome.storage) {
            return {
                get: (key) => new Promise((resolve) => {
                    chrome.storage.local.get(key, resolve);
                }),
                set: (data) => new Promise((resolve) => {
                    chrome.storage.local.set(data, resolve);
                })
            };
        }
        
        // Firefox
        if (typeof browser !== 'undefined' && browser.storage) {
            return browser.storage.local;
        }
        
        // Fallback to localStorage
        return {
            get: (key) => Promise.resolve({
                [key]: JSON.parse(localStorage.getItem(key) || 'null')
            }),
            set: (data) => {
                Object.keys(data).forEach(key => {
                    localStorage.setItem(key, JSON.stringify(data[key]));
                });
                return Promise.resolve();
            }
        };
    }

    /**
     * Show browser notification
     */
    showBrowserNotification(title, message) {
        // Chrome/Edge
        if (typeof chrome !== 'undefined' && chrome.notifications) {
            chrome.notifications.create({
                type: 'basic',
                iconUrl: 'icons/icon128.png',
                title: title,
                message: message
            });
            return;
        }
        
        // Firefox
        if (typeof browser !== 'undefined' && browser.notifications) {
            browser.notifications.create({
                type: 'basic',
                iconUrl: 'icons/icon128.png',
                title: title,
                message: message
            });
            return;
        }
        
        // Fallback to web notifications
        if ('Notification' in window && Notification.permission === 'granted') {
            new Notification(title, {
                body: message,
                icon: 'icons/icon128.png'
            });
        }
    }

    /**
     * Simple hash function
     */
    simpleHash(str) {
        let hash = 0;
        for (let i = 0; i < str.length; i++) {
            const char = str.charCodeAt(i);
            hash = ((hash << 5) - hash) + char;
            hash = hash & hash;
        }
        return Math.abs(hash).toString(36);
    }
}

// Export for use
if (typeof module !== 'undefined' && module.exports) {
    module.exports = BrowserAirdropManager;
}

