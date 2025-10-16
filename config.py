# config.py - USDTgVerse Social Media Bot Configuration

# Twitter/X API Configuration
TWITTER_API_KEY = "YOUR_TWITTER_API_KEY"
TWITTER_API_SECRET = "YOUR_TWITTER_API_SECRET"
TWITTER_ACCESS_TOKEN = "YOUR_TWITTER_ACCESS_TOKEN"
TWITTER_ACCESS_SECRET = "YOUR_TWITTER_ACCESS_SECRET"
TWITTER_BEARER_TOKEN = "YOUR_TWITTER_BEARER_TOKEN"

# LinkedIn Configuration
LINKEDIN_EMAIL = "your_email@usdtgverse.com"
LINKEDIN_PASSWORD = "your_linkedin_password"

# Instagram Configuration
INSTAGRAM_USERNAME = "usdtgverse"
INSTAGRAM_PASSWORD = "your_instagram_password"

# TikTok Configuration (Future)
TIKTOK_USERNAME = "usdtgverse"
TIKTOK_PASSWORD = "your_tiktok_password"

# Bot Settings
MIN_POST_INTERVAL = 7200  # 2 hours in seconds
MAX_POST_INTERVAL = 28800  # 8 hours in seconds
ENABLE_ANALYTICS = True
ENABLE_ENGAGEMENT_BOT = True

# Content Settings
MAX_HASHTAGS = 4
MIN_HASHTAGS = 2
ENABLE_TRENDING_TOPICS = True

# Platform Settings
PLATFORMS = {
    "twitter": True,
    "linkedin": True,
    "instagram": True,
    "tiktok": False  # Future implementation
}

# Analytics Settings
ANALYTICS_FILE = "social_media_analytics.json"
DAILY_REPORT_TIME = "09:00"  # Daily report time
REPORT_TIMEZONE = "UTC"

# Content Database
CONTENT_DATABASE = {
    "technical": [
        "🚀 USDTgVerse: Quantum-safe blockchain with 15,000+ TPS",
        "⚛️ Post-quantum cryptography: 50+ years future-proof security",
        "🌉 Cross-chain bridges: ETH, TRON, SOLANA support live",
        "💧 Liquidity mining: 100% APY for early adopters",
        "🏢 Enterprise OdixPay++: Complete B2B payment solutions",
        "🔒 Military-grade security: Quantum-resistant algorithms",
        "⚡ Lightning-fast transactions: Sub-second finality",
        "🌐 Multi-chain universe: 8 blockchain networks supported",
        "💰 Native USDTg stablecoin: $1.00 peg maintained",
        "🎯 Zero dependencies: Independent blockchain ecosystem"
    ],
    "news": [
        "📈 USDTgVerse TVL reaches $1M milestone!",
        "🎉 1000+ active users on USDTgVerse ecosystem",
        "🏆 USDTgVerse wins 'Best DeFi Innovation' award",
        "📊 Daily trading volume exceeds $500K",
        "🌟 Community grows to 5000+ members",
        "🚀 New partnership announced with major exchange",
        "💎 USDTgVerse listed on top-tier exchange",
        "🎪 Community AMA scheduled for this week",
        "🏅 USDTgVerse featured in top crypto media",
        "📱 Mobile wallet reaches 10K+ downloads"
    ],
    "educational": [
        "💡 What is quantum-safe cryptography?",
        "🔍 How does USDTgVerse AMM work?",
        "🌐 Understanding cross-chain bridges",
        "💰 Liquidity mining explained",
        "🏦 Enterprise blockchain solutions",
        "🔐 Post-quantum security benefits",
        "⚡ High-performance blockchain architecture",
        "🌉 Cross-chain interoperability explained",
        "💼 B2B payment solutions overview",
        "🎯 DeFi vs traditional finance"
    ],
    "community": [
        "👥 Join our Discord: discord.gg/usdtgverse",
        "📱 Download USDTgVerse Wallet: app.usdtgverse.com",
        "🎯 Community spotlight: Meet our developers",
        "🏆 Top liquidity providers this week",
        "💬 Share your USDTgVerse experience",
        "🎪 Community events: Join our AMA",
        "🌟 Developer spotlight: Building the future",
        "🏅 Community achievements unlocked",
        "📊 User-generated content highlights",
        "🎁 Community rewards program"
    ],
    "promotional": [
        "🎁 Limited time: 100% APY liquidity mining",
        "🔥 Early bird bonus: Extra 50% rewards",
        "🚀 Referral program: Earn 25 USDTg per friend",
        "💎 VIP membership: Exclusive benefits",
        "🎪 Community events: Join our AMA",
        "🏆 Trading competition: Win 1000 USDTg",
        "🎯 Special offer: 50% off premium features",
        "🌟 New user bonus: 100 USDTg welcome gift",
        "💎 Exclusive NFT collection launch",
        "🎁 Holiday special: Double rewards"
    ]
}

# Hashtag Database
HASHTAGS = [
    "#USDTgVerse", "#QuantumSafe", "#DeFi", "#Blockchain",
    "#Crypto", "#Web3", "#Innovation", "#Technology",
    "#Finance", "#Enterprise", "#Security", "#Future",
    "#QuantumComputing", "#PostQuantum", "#Cryptography",
    "#CrossChain", "#LiquidityMining", "#YieldFarming",
    "#B2B", "#Payments", "#Stablecoin", "#USDTg",
    "#Ethereum", "#Bitcoin", "#Solana", "#TRON",
    "#BNBChain", "#Polygon", "#Arbitrum", "#Avalanche"
]

# Trending Topics (Updated regularly)
TRENDING_TOPICS = [
    "Bitcoin ETF approval",
    "Ethereum upgrade",
    "DeFi summer",
    "NFT marketplace",
    "Web3 adoption",
    "Central bank digital currencies",
    "Quantum computing advances",
    "Cross-chain interoperability",
    "Enterprise blockchain",
    "Regulatory compliance"
]

# Engagement Bot Settings
ENGAGEMENT_SETTINGS = {
    "auto_like": True,
    "auto_retweet": True,
    "auto_follow": False,  # Be careful with this
    "max_likes_per_day": 100,
    "max_retweets_per_day": 50,
    "max_follows_per_day": 20
}

# Error Handling
ERROR_RETRY_ATTEMPTS = 3
ERROR_RETRY_DELAY = 300  # 5 minutes
LOG_LEVEL = "INFO"  # DEBUG, INFO, WARNING, ERROR

# Backup Settings
BACKUP_ENABLED = True
BACKUP_FREQUENCY = "daily"  # daily, weekly, monthly
BACKUP_LOCATION = "./backups/"

# Monitoring Settings
HEALTH_CHECK_INTERVAL = 3600  # 1 hour
ALERT_EMAIL = "alerts@usdtgverse.com"
ALERT_THRESHOLD = 5  # Failed posts before alert
