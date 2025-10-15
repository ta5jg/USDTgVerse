/*
==============================================
 File:        bonus-manager.js
 Author:      USDTG GROUP TECHNOLOGY LLC
 Created:     2025-10-13
 Last Update: 2025-10-13
 Version:     1.0

 Description:
   USDTgVerse Web Wallet Bonus Manager
   
   JavaScript client for bonus system:
   - 10,000 USDTg → +10 USDTg bonus
   - VIP tier system (Bronze to Diamond)
   - Automatic bonus notifications

 License:
   MIT License
==============================================
*/

class BonusManager {
    constructor() {
        this.apiBase = 'http://localhost:3007';
        this.minPurchaseBonus = 10000.0;
        this.tiers = {
            bronze: 10000,
            silver: 50000,
            gold: 100000,
            platinum: 500000,
            diamond: 1000000
        };
    }

    /**
     * Check if purchase qualifies for bonus and create bonus record
     */
    async checkPurchaseBonus(walletAddress, userId, purchaseAmount) {
        try {
            if (purchaseAmount < this.minPurchaseBonus) {
                console.log(`Purchase amount too low. Minimum: ${this.minPurchaseBonus} USDTg`);
                return {
                    qualified: false,
                    message: `Minimum purchase for bonus: ${this.minPurchaseBonus} USDTg`
                };
            }

            const response = await fetch(`${this.apiBase}/api/bonus/create`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    wallet_address: walletAddress,
                    user_id: userId,
                    purchase_amount: purchaseAmount
                })
            });

            const data = await response.json();

            if (data.success) {
                console.log('🎁 BONUS EARNED!');
                console.log('💰 Amount:', data.bonus_amount, 'USDTg');
                console.log('🏆 Tier:', data.tier);
                console.log('📝', data.message);

                this.showBonusNotification(
                    data.bonus_amount,
                    data.tier,
                    data.bonus_percent,
                    data.message
                );

                return {
                    qualified: true,
                    bonusId: data.bonus_id,
                    bonusAmount: data.bonus_amount,
                    tier: data.tier,
                    bonusPercent: data.bonus_percent,
                    message: data.message
                };
            } else {
                console.error('❌ Bonus failed:', data.error);
                return {
                    qualified: false,
                    error: data.error
                };
            }
        } catch (error) {
            console.error('❌ Bonus check failed:', error);
            return {
                qualified: false,
                error: error.message
            };
        }
    }

    /**
     * Calculate bonus for purchase amount
     */
    async calculateBonus(purchaseAmount) {
        try {
            const response = await fetch(
                `${this.apiBase}/api/bonus/calculate?amount=${purchaseAmount}`
            );

            const data = await response.json();

            if (data.success) {
                return {
                    success: true,
                    purchaseAmount: data.purchase_amount,
                    bonusAmount: data.bonus_amount,
                    tier: data.tier,
                    bonusPercent: data.bonus_percent,
                    qualifies: data.qualifies,
                    minimumPurchase: data.minimum_purchase,
                    tierThresholds: data.tier_thresholds
                };
            } else {
                return {
                    success: false,
                    error: data.error
                };
            }
        } catch (error) {
            console.error('❌ Bonus calculation failed:', error);
            return {
                success: false,
                error: error.message
            };
        }
    }

    /**
     * Get user bonus statistics
     */
    async getUserStats(walletAddress) {
        try {
            const response = await fetch(
                `${this.apiBase}/api/bonus/user-stats?wallet_address=${walletAddress}`
            );

            const data = await response.json();

            if (data.success) {
                return {
                    success: true,
                    walletAddress: data.wallet_address,
                    totalPurchases: data.total_purchases,
                    totalBonuses: data.total_bonuses,
                    bonusCount: data.bonus_count,
                    highestTier: data.highest_tier,
                    isVip: data.is_vip,
                    firstBonus: data.first_bonus,
                    lastBonus: data.last_bonus
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
     * Get system bonus statistics
     */
    async getSystemStats() {
        try {
            const response = await fetch(`${this.apiBase}/api/bonus/system-stats`);
            const data = await response.json();

            if (data.success) {
                return {
                    success: true,
                    totalBonuses: data.total_bonuses,
                    totalBonusValue: data.total_bonus_value,
                    totalPurchaseVolume: data.total_purchase_volume,
                    bonusesToday: data.bonuses_today,
                    tierBreakdown: data.tier_breakdown
                };
            } else {
                return {
                    success: false,
                    error: data.error
                };
            }
        } catch (error) {
            console.error('❌ System stats fetch failed:', error);
            return {
                success: false,
                error: error.message
            };
        }
    }

    /**
     * Show bonus notification
     */
    showBonusNotification(bonusAmount, tier, bonusPercent, message) {
        const tierColors = {
            'Bronze': 'linear-gradient(135deg, #CD7F32 0%, #B87333 100%)',
            'Silver': 'linear-gradient(135deg, #C0C0C0 0%, #A8A8A8 100%)',
            'Gold': 'linear-gradient(135deg, #FFD700 0%, #FFA500 100%)',
            'Platinum': 'linear-gradient(135deg, #E5E4E2 0%, #B0C4DE 100%)',
            'Diamond': 'linear-gradient(135deg, #B9F2FF 0%, #00CED1 100%)'
        };

        const tierEmojis = {
            'Bronze': '🥉',
            'Silver': '🥈',
            'Gold': '🥇',
            'Platinum': '💎',
            'Diamond': '💠'
        };

        const notification = document.createElement('div');
        notification.className = 'bonus-notification';
        notification.innerHTML = `
            <div style="background: ${tierColors[tier] || tierColors['Bronze']}; 
                        color: white; padding: 25px; border-radius: 15px; 
                        box-shadow: 0 15px 50px rgba(0,0,0,0.4); 
                        position: fixed; top: 20px; right: 20px; 
                        max-width: 450px; z-index: 10000; animation: bonusSlideIn 0.6s cubic-bezier(0.68, -0.55, 0.265, 1.55);">
                <h3 style="margin: 0 0 15px 0; font-size: 24px; font-weight: bold;">
                    ${tierEmojis[tier]} ${tier} Tier Bonus!
                </h3>
                <p style="margin: 0 0 15px 0; font-size: 16px;">${message}</p>
                <div style="background: rgba(255,255,255,0.25); padding: 15px; border-radius: 10px; margin-top: 15px;">
                    <div style="display: flex; justify-content: space-between; margin-bottom: 10px;">
                        <span style="font-size: 14px;">💰 Bonus Amount:</span>
                        <span style="font-size: 18px; font-weight: bold;">${bonusAmount} USDTg</span>
                    </div>
                    <div style="display: flex; justify-content: space-between; margin-bottom: 10px;">
                        <span style="font-size: 14px;">🏆 Tier:</span>
                        <span style="font-size: 16px; font-weight: bold;">${tier}</span>
                    </div>
                    <div style="display: flex; justify-content: space-between;">
                        <span style="font-size: 14px;">📊 Bonus Rate:</span>
                        <span style="font-size: 16px; font-weight: bold;">${bonusPercent}%</span>
                    </div>
                </div>
                <div style="margin-top: 15px; padding: 10px; background: rgba(255,255,255,0.15); border-radius: 8px; text-align: center;">
                    <p style="margin: 0; font-size: 13px;">🎯 Keep purchasing to unlock higher tiers!</p>
                </div>
            </div>
        `;

        document.body.appendChild(notification);

        setTimeout(() => {
            notification.style.animation = 'bonusSlideOut 0.5s';
            setTimeout(() => notification.remove(), 500);
        }, 10000);
    }

    /**
     * Show tier progress indicator
     */
    showTierProgress(currentPurchase, currentTier) {
        const nextTierThreshold = this.getNextTierThreshold(currentTier);
        if (!nextTierThreshold) return;

        const progress = (currentPurchase / nextTierThreshold) * 100;
        const remaining = nextTierThreshold - currentPurchase;

        console.log(`📊 Tier Progress: ${progress.toFixed(2)}%`);
        console.log(`🎯 Next Tier: ${remaining.toFixed(2)} USDTg remaining`);
    }

    /**
     * Get next tier threshold
     */
    getNextTierThreshold(currentTier) {
        const tierOrder = ['Bronze', 'Silver', 'Gold', 'Platinum', 'Diamond'];
        const currentIndex = tierOrder.indexOf(currentTier);
        
        if (currentIndex === -1 || currentIndex === tierOrder.length - 1) {
            return null;
        }

        const nextTier = tierOrder[currentIndex + 1];
        return this.tiers[nextTier.toLowerCase()];
    }
}

// Add CSS animations
const style = document.createElement('style');
style.textContent = `
    @keyframes bonusSlideIn {
        from {
            transform: translateX(500px) scale(0.8);
            opacity: 0;
        }
        to {
            transform: translateX(0) scale(1);
            opacity: 1;
        }
    }
    
    @keyframes bonusSlideOut {
        from {
            transform: translateX(0) scale(1);
            opacity: 1;
        }
        to {
            transform: translateX(500px) scale(0.8);
            opacity: 0;
        }
    }
`;
document.head.appendChild(style);

// Export for use
if (typeof module !== 'undefined' && module.exports) {
    module.exports = BonusManager;
}

