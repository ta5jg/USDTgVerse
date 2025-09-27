/*
==============================================
 File:        script.js
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Website JavaScript
   
   Interactive JavaScript functionality for the USDTgVerse website including:
   - Smooth scrolling navigation
   - Interactive animations
   - Form handling
   - User interface enhancements
   - Responsive behavior

 License:
   MIT License
==============================================
*/
document.addEventListener('DOMContentLoaded', function() {
    // Mobile Menu Toggle Functionality
    function initWebsiteMobileMenu() {
        const mobileMenuToggle = document.getElementById('mobile-menu-toggle');
        const desktopMenu = document.getElementById('desktop-menu');
        
        console.log('Website mobile menu elements:', { mobileMenuToggle, desktopMenu });
        
        if (mobileMenuToggle && desktopMenu) {
            mobileMenuToggle.addEventListener('click', function(e) {
                e.preventDefault();
                e.stopPropagation();
                
                console.log('Website hamburger menu clicked');
                
                if (desktopMenu.style.display === 'none' || desktopMenu.style.display === '') {
                    desktopMenu.style.display = 'flex';
                    desktopMenu.style.flexDirection = 'column';
                    desktopMenu.style.position = 'fixed';
                    desktopMenu.style.top = '80px';
                    desktopMenu.style.left = '0';
                    desktopMenu.style.right = '0';
                    desktopMenu.style.background = 'rgba(0, 0, 0, 0.98)';
                    desktopMenu.style.backdropFilter = 'blur(20px)';
                    desktopMenu.style.padding = '30px 20px';
                    desktopMenu.style.borderRadius = '0 0 15px 15px';
                    desktopMenu.style.border = '2px solid rgba(255, 255, 255, 0.2)';
                    desktopMenu.style.borderTop = 'none';
                    desktopMenu.style.zIndex = '9999';
                    desktopMenu.style.boxShadow = '0 10px 30px rgba(0, 0, 0, 0.5)';
                    mobileMenuToggle.innerHTML = '✕';
                    console.log('Website mobile menu opened');
                } else {
                    desktopMenu.style.display = 'none';
                    mobileMenuToggle.innerHTML = '☰';
                    console.log('Website mobile menu closed');
                }
            });
        } else {
            console.log('Website mobile menu elements not found');
        }
    }
    
    // Initialize website mobile menu
    initWebsiteMobileMenu();
    
    // Close mobile menu when clicking outside
    document.addEventListener('click', function(e) {
        const mobileMenuToggle = document.getElementById('mobile-menu-toggle');
        const desktopMenu = document.getElementById('desktop-menu');
        
        if (mobileMenuToggle && desktopMenu && 
            !mobileMenuToggle.contains(e.target) && 
            !desktopMenu.contains(e.target)) {
            if (window.innerWidth <= 1024) {
                desktopMenu.style.display = 'none';
                mobileMenuToggle.innerHTML = '☰';
            }
        }
    });
    
    // Handle window resize
    window.addEventListener('resize', function() {
        const mobileMenuToggle = document.getElementById('mobile-menu-toggle');
        const desktopMenu = document.getElementById('desktop-menu');
        
        if (window.innerWidth > 1024) {
            if (desktopMenu) {
                desktopMenu.style.display = 'flex';
                desktopMenu.style.flexDirection = 'row';
                desktopMenu.style.position = 'static';
                desktopMenu.style.background = 'transparent';
                desktopMenu.style.backdropFilter = 'none';
                desktopMenu.style.padding = '0';
                desktopMenu.style.borderRadius = '0';
                desktopMenu.style.border = 'none';
            }
            if (mobileMenuToggle) {
                mobileMenuToggle.innerHTML = '☰';
            }
        } else {
            if (desktopMenu) {
                desktopMenu.style.display = 'none';
            }
        }
    });

    // Smooth scrolling for navigation links
    const navLinks = document.querySelectorAll('a[href^="#"]');
    navLinks.forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            const targetId = this.getAttribute('href');
            const targetSection = document.querySelector(targetId);
            if (targetSection) {
                targetSection.scrollIntoView({
                    behavior: 'smooth',
                    block: 'start'
                });
            }
        });
    });

    // Update token prices in real-time (simulation)
    function updateTokenPrices() {
        const prices = {
            'USDTg': 1.00,
            'USDTgV': 0.50 + (Math.random() - 0.5) * 0.1,
            'USDTgG': 5.00 + (Math.random() - 0.5) * 0.5
        };

        // Update token prices in the tokens section
        const tokenCards = document.querySelectorAll('.token-card');
        tokenCards.forEach((card, index) => {
            const priceElement = card.querySelector('.token-price');
            if (priceElement) {
                const tokens = ['USDTg', 'USDTgV', 'USDTgG'];
                const price = prices[tokens[index]];
                priceElement.textContent = `$${price.toFixed(2)}`;
                
                // Add price change indicator
                if (index > 0) { // Skip USDTg (stable)
                    const change = (Math.random() - 0.5) * 2;
                    const changeElement = document.createElement('span');
                    changeElement.className = `price-change ${change >= 0 ? 'positive' : 'negative'}`;
                    changeElement.textContent = `${change >= 0 ? '+' : ''}${change.toFixed(2)}%`;
                    priceElement.appendChild(changeElement);
                }
            }
        });

        // Update DEX pair prices
        const pairItems = document.querySelectorAll('.pair-item');
        pairItems.forEach((item, index) => {
            const priceElement = item.querySelector('.pair-price');
            if (priceElement && index === 0) { // USDTg/USDTgV
                const price = 0.5 + (Math.random() - 0.5) * 0.05;
                priceElement.textContent = `${price.toFixed(3)} USDTgV`;
            } else if (priceElement && index === 1) { // USDTg/USDTgG
                const price = 5.0 + (Math.random() - 0.5) * 0.2;
                priceElement.textContent = `${price.toFixed(2)} USDTgG`;
            }
        });

        // Update CEX pair prices
        const exchangePairs = document.querySelectorAll('.exchange-pair');
        exchangePairs.forEach((pair, index) => {
            const priceElement = pair.querySelector('.pair-price');
            const changeElement = pair.querySelector('.pair-change');
            const volumeElement = pair.querySelector('.pair-volume');
            
            if (priceElement) {
                const basePrices = [1.00, 0.50, 5.00];
                const price = basePrices[index] + (Math.random() - 0.5) * 0.1;
                priceElement.textContent = `$${price.toFixed(2)}`;
            }
            
            if (changeElement) {
                const change = (Math.random() - 0.5) * 4;
                changeElement.textContent = `${change >= 0 ? '+' : ''}${change.toFixed(1)}%`;
                changeElement.className = `pair-change ${change >= 0 ? 'positive' : 'negative'}`;
            }
            
            if (volumeElement) {
                const volume = (Math.random() * 100000).toFixed(0);
                volumeElement.textContent = `$${volume}`;
            }
        });
    }

    // Update prices every 5 seconds
    setInterval(updateTokenPrices, 5000);

    // Add CSS for price changes
    const style = document.createElement('style');
    style.textContent = `
        .price-change {
            font-size: 0.8rem;
            margin-left: 0.5rem;
            padding: 0.2rem 0.5rem;
            border-radius: 4px;
        }
        .price-change.positive {
            background: #d4edda;
            color: #155724;
        }
        .price-change.negative {
            background: #f8d7da;
            color: #721c24;
        }
        .pair-change.positive {
            color: #28a745;
        }
        .pair-change.negative {
            color: #dc3545;
        }
    `;
    document.head.appendChild(style);

    // Add loading animation for token cards
    const tokenCards = document.querySelectorAll('.token-card');
    tokenCards.forEach(card => {
        card.addEventListener('mouseenter', function() {
            this.style.transform = 'translateY(-5px) scale(1.02)';
        });
        
        card.addEventListener('mouseleave', function() {
            this.style.transform = 'translateY(0) scale(1)';
        });
    });

    // Add click handlers for trading pairs
    const pairItems = document.querySelectorAll('.pair-item');
    pairItems.forEach(item => {
        item.addEventListener('click', function() {
            const pairName = this.querySelector('.pair-name').textContent;
            alert(`Trading ${pairName} - Redirecting to DEX...`);
        });
    });

    // Add click handlers for exchange pairs
    const exchangePairs = document.querySelectorAll('.exchange-pair');
    exchangePairs.forEach(pair => {
        pair.addEventListener('click', function() {
            const pairSymbol = this.querySelector('.pair-symbol').textContent;
            alert(`Trading ${pairSymbol} - Redirecting to CEX...`);
        });
    });

    // Initialize tooltips for addresses
    const addresses = document.querySelectorAll('.token-address, .pair-address');
    addresses.forEach(address => {
        address.addEventListener('click', function() {
            navigator.clipboard.writeText(this.textContent).then(() => {
                const originalText = this.textContent;
                this.textContent = 'Copied!';
                setTimeout(() => {
                    this.textContent = originalText;
                }, 2000);
            });
        });
        
        address.style.cursor = 'pointer';
        address.title = 'Click to copy address';
    });

    // Remove scroll effect for header to prevent color change
    // window.addEventListener('scroll', function() {
    //     const header = document.querySelector('.header');
    //     if (window.scrollY > 100) {
    //         header.style.background = 'rgba(255, 255, 255, 0.98)';
    //     } else {
    //         header.style.background = 'rgba(255, 255, 255, 0.95)';
    //     }
    // });

    // Add animation for hero stats
    const stats = document.querySelectorAll('.stat-value');
    stats.forEach(stat => {
        const finalValue = stat.textContent;
        let currentValue = 0;
        const increment = parseFloat(finalValue.replace('$', '')) / 100;
        
        const timer = setInterval(() => {
            currentValue += increment;
            if (currentValue >= parseFloat(finalValue.replace('$', ''))) {
                stat.textContent = finalValue;
                clearInterval(timer);
            } else {
                stat.textContent = '$' + currentValue.toFixed(2);
            }
        }, 20);
    });

    console.log('USDTgVerse Website Loaded Successfully!');
    console.log('🚀 USDTgVerse Enterprise Blockchain Ecosystem');
    console.log('📍 Network: usdtgverse-mainnet-1');
    console.log('💰 Tokens: USDTg, USDTgV, USDTgG');
    console.log('🌐 DEX & CEX: Active');
});
