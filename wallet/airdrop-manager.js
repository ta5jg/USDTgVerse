/*
==============================================
 File:        airdrop-manager.js
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Web Wallet Airdrop Manager
   
   JavaScript client for airdrop system:
   - Request 10 USDTg locked airdrop
   - Pay fees with locked airdrop
   - Verify purchases and unlock
   - Display airdrop status

 License:
   MIT License
==============================================
*/

class AirdropManager {
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
            const deviceFingerprint = this.getDeviceFingerprint();
            
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
                console.log('✅ Airdrop received:', data.airdrop_id);
                console.log('💰 Amount:', data.amount, 'USDTg (LOCKED - Fee only)');
                console.log('📝', data.message);

                this.showAirdropNotification(data.amount, data.message);

                return {
                    success: true,
                    airdropId: data.airdrop_id,
                    amount: data.amount,
                    message: data.message
                };
            } else {
                console.error('❌ Airdrop failed:', data.error);
                return {
                    success: false,
                    error: data.error
                };
            }
        } catch (error) {
            console.error('❌ Airdrop request failed:', error);
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
                console.log('✅ Fee paid from airdrop:', feeAmount, 'USDTg');
                console.log('💰 Remaining locked:', data.remaining, 'USDTg');
                return true;
            } else {
                console.error('❌ Fee payment failed:', data.error);
                return false;
            }
        } catch (error) {
            console.error('❌ Fee payment failed:', error);
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
                    console.log('🔓 AIRDROP UNLOCKED!');
                    console.log('💰 10 USDTg now available for all transactions');
                    this.showUnlockNotification();
                    return {
                        unlocked: true,
                        message: data.message
                    };
                } else {
                    console.log('💰 Purchase recorded. Need', data.remaining_to_unlock, 'more USDTg to unlock');
                    return {
                        unlocked: false,
                        remainingToUnlock: data.remaining_to_unlock,
                        message: data.message
                    };
                }
            } else {
                console.error('❌ Purchase verification failed:', data.error);
                return {
                    unlocked: false,
                    error: data.error
                };
            }
        } catch (error) {
            console.error('❌ Purchase verification failed:', error);
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
            console.error('❌ Status check failed:', error);
            return {
                success: false,
                error: error.message
            };
        }
    }

    /**
     * Get system statistics
     */
    async getSystemStats() {
        try {
            const response = await fetch(`${this.apiBase}/api/airdrop/stats`);
            const data = await response.json();

            if (data.success) {
                return {
                    success: true,
                    totalAirdrops: data.total_airdrops,
                    lockedAirdrops: data.locked_airdrops,
                    unlockedAirdrops: data.unlocked_airdrops,
                    totalValue: data.total_airdrop_value,
                    lockedValue: data.locked_value,
                    usedForFees: data.used_for_fees,
                    airdropsToday: data.airdrops_today,
                    dailyLimit: data.daily_limit
                };
            } else {
                return {
                    success: false,
                    error: data.error
                };
            }
        } catch (error) {
            console.error('❌ Stats fetch failed:', error);
            return {
                success: false,
                error: error.message
            };
        }
    }

    /**
     * Show airdrop notification
     */
    showAirdropNotification(amount, message) {
        const notification = document.createElement('div');
        notification.className = 'airdrop-notification';
        notification.innerHTML = `
            <div style="background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); 
                        color: white; padding: 20px; border-radius: 12px; 
                        box-shadow: 0 10px 40px rgba(0,0,0,0.3); 
                        position: fixed; top: 20px; right: 20px; 
                        max-width: 400px; z-index: 10000; animation: slideIn 0.5s;">
                <h3 style="margin: 0 0 10px 0; font-size: 18px;">🎉 Welcome Airdrop!</h3>
                <p style="margin: 0 0 10px 0; font-size: 14px;">${message}</p>
                <div style="background: rgba(255,255,255,0.2); padding: 10px; border-radius: 8px; margin-top: 10px;">
                    <p style="margin: 0; font-size: 12px;">💰 Amount: ${amount} USDTg</p>
                    <p style="margin: 5px 0 0 0; font-size: 12px;">🔒 Locked for fees only</p>
                    <p style="margin: 5px 0 0 0; font-size: 12px;">🔓 Purchase ${this.minPurchaseUnlock}+ USDTg to unlock</p>
                </div>
            </div>
        `;

        document.body.appendChild(notification);

        setTimeout(() => {
            notification.style.animation = 'slideOut 0.5s';
            setTimeout(() => notification.remove(), 500);
        }, 8000);
    }

    /**
     * Show unlock notification
     */
    showUnlockNotification() {
        const notification = document.createElement('div');
        notification.className = 'unlock-notification';
        notification.innerHTML = `
            <div style="background: linear-gradient(135deg, #11998e 0%, #38ef7d 100%); 
                        color: white; padding: 20px; border-radius: 12px; 
                        box-shadow: 0 10px 40px rgba(0,0,0,0.3); 
                        position: fixed; top: 20px; right: 20px; 
                        max-width: 400px; z-index: 10000; animation: slideIn 0.5s;">
                <h3 style="margin: 0 0 10px 0; font-size: 18px;">🔓 Airdrop Unlocked!</h3>
                <p style="margin: 0 0 10px 0; font-size: 14px;">Congratulations! Your airdrop is now fully unlocked.</p>
                <div style="background: rgba(255,255,255,0.2); padding: 10px; border-radius: 8px; margin-top: 10px;">
                    <p style="margin: 0; font-size: 12px;">💎 You can now use all 10 USDTg for any transaction</p>
                    <p style="margin: 5px 0 0 0; font-size: 12px;">🎉 Thank you for your purchase!</p>
                </div>
            </div>
        `;

        document.body.appendChild(notification);

        setTimeout(() => {
            notification.style.animation = 'slideOut 0.5s';
            setTimeout(() => notification.remove(), 500);
        }, 8000);
    }

    /**
     * Get device fingerprint for fraud detection
     */
    getDeviceFingerprint() {
        const canvas = document.createElement('canvas');
        const ctx = canvas.getContext('2d');
        ctx.textBaseline = 'top';
        ctx.font = '14px Arial';
        ctx.fillText('USDTgVerse', 2, 2);
        
        const fingerprint = canvas.toDataURL();
        const hash = this.simpleHash(fingerprint);
        
        return `WEB_${hash}_${navigator.userAgent.substring(0, 20)}`;
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

// Add CSS animations
const style = document.createElement('style');
style.textContent = `
    @keyframes slideIn {
        from {
            transform: translateX(400px);
            opacity: 0;
        }
        to {
            transform: translateX(0);
            opacity: 1;
        }
    }
    
    @keyframes slideOut {
        from {
            transform: translateX(0);
            opacity: 1;
        }
        to {
            transform: translateX(400px);
            opacity: 0;
        }
    }
`;
document.head.appendChild(style);

// Export for use
if (typeof module !== 'undefined' && module.exports) {
    module.exports = AirdropManager;
}

