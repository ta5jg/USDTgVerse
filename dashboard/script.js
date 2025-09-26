/*
==============================================
 File:        script.js
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   USDTgVerse Dashboard JavaScript
   
   Interactive JavaScript functionality for the USDTgVerse dashboard including:
   - Navigation handling
   - Real-time data updates
   - Chart rendering
   - User interactions
   - API communications

 License:
   MIT License
==============================================
*/
document.addEventListener('DOMContentLoaded', function() {
    // Navigation
    const navLinks = document.querySelectorAll('.nav-link');
    const sections = document.querySelectorAll('.dashboard-section');

    navLinks.forEach(link => {
        link.addEventListener('click', function(e) {
            e.preventDefault();
            
            // Remove active class from all links and sections
            navLinks.forEach(l => l.classList.remove('active'));
            sections.forEach(s => s.classList.remove('active'));
            
            // Add active class to clicked link
            this.classList.add('active');
            
            // Show corresponding section
            const targetId = this.getAttribute('href').substring(1);
            const targetSection = document.getElementById(targetId);
            if (targetSection) {
                targetSection.classList.add('active');
            }
        });
    });

    // Initialize Charts
    function initializeCharts() {
        // Price Chart
        const priceCtx = document.getElementById('priceChart');
        if (priceCtx) {
            new Chart(priceCtx, {
                type: 'line',
                data: {
                    labels: ['1h', '2h', '3h', '4h', '5h', '6h', '7h', '8h', '9h', '10h', '11h', '12h'],
                    datasets: [{
                        label: 'USDTg Price',
                        data: [1.00, 1.01, 0.99, 1.02, 1.00, 0.98, 1.01, 1.03, 1.00, 0.99, 1.01, 1.00],
                        borderColor: '#667eea',
                        backgroundColor: 'rgba(102, 126, 234, 0.1)',
                        tension: 0.4,
                        fill: true
                    }]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    plugins: {
                        legend: {
                            display: false
                        }
                    },
                    scales: {
                        y: {
                            beginAtZero: false,
                            min: 0.95,
                            max: 1.05
                        }
                    }
                }
            });
        }

        // Volume Chart
        const volumeCtx = document.getElementById('volumeChart');
        if (volumeCtx) {
            new Chart(volumeCtx, {
                type: 'bar',
                data: {
                    labels: ['1h', '2h', '3h', '4h', '5h', '6h', '7h', '8h', '9h', '10h', '11h', '12h'],
                    datasets: [{
                        label: 'Volume',
                        data: [45000, 52000, 48000, 61000, 55000, 67000, 59000, 72000, 68000, 75000, 71000, 78000],
                        backgroundColor: 'rgba(118, 75, 162, 0.8)',
                        borderColor: '#764ba2',
                        borderWidth: 1
                    }]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    plugins: {
                        legend: {
                            display: false
                        }
                    },
                    scales: {
                        y: {
                            beginAtZero: true
                        }
                    }
                }
            });
        }
    }

    // Update real-time data
    function updateRealTimeData() {
        // Update token prices
        const tokenPrices = document.querySelectorAll('.tokens-table .price');
        tokenPrices.forEach((price, index) => {
            const basePrices = [1.00, 0.50, 5.00];
            const variation = (Math.random() - 0.5) * 0.1;
            const newPrice = basePrices[index] + variation;
            price.textContent = `$${newPrice.toFixed(2)}`;
        });

        // Update 24h changes
        const changes = document.querySelectorAll('.tokens-table .change');
        changes.forEach((change, index) => {
            if (index === 0) return; // Skip USDTg (stable)
            const changeValue = (Math.random() - 0.5) * 5;
            const isPositive = changeValue >= 0;
            change.textContent = `${isPositive ? '+' : ''}${changeValue.toFixed(2)}%`;
            change.className = `change ${isPositive ? 'positive' : 'negative'}`;
        });

        // Update DEX pair prices
        const pairPrices = document.querySelectorAll('.pair-price');
        pairPrices.forEach((price, index) => {
            const basePrices = [0.5, 5.0];
            const variation = (Math.random() - 0.5) * 0.05;
            const newPrice = basePrices[index] + variation;
            price.textContent = `${newPrice.toFixed(3)} ${index === 0 ? 'USDTgV' : 'USDTgG'}`;
        });

        // Update CEX prices
        const cexPrices = document.querySelectorAll('.exchange-table .price');
        cexPrices.forEach((price, index) => {
            const basePrices = [1.00, 0.50, 5.00];
            const variation = (Math.random() - 0.5) * 0.1;
            const newPrice = basePrices[index] + variation;
            price.textContent = `$${newPrice.toFixed(2)}`;
        });

        // Update volumes
        const volumes = document.querySelectorAll('.volume');
        volumes.forEach(volume => {
            const newVolume = (Math.random() * 100000).toFixed(0);
            volume.textContent = `$${newVolume}`;
        });

        // Update block height
        const blockHeight = document.querySelector('.metric-value');
        if (blockHeight && blockHeight.textContent.includes(',')) {
            const currentHeight = parseInt(blockHeight.textContent.replace(/,/g, ''));
            const newHeight = currentHeight + Math.floor(Math.random() * 10) + 1;
            blockHeight.textContent = newHeight.toLocaleString();
        }

        // Update TVL
        const tvl = document.querySelector('.metric-value');
        if (tvl && tvl.textContent.includes('$')) {
            const currentTVL = parseFloat(tvl.textContent.replace(/[$,]/g, ''));
            const variation = (Math.random() - 0.5) * 100000;
            const newTVL = currentTVL + variation;
            tvl.textContent = `$${newTVL.toLocaleString()}`;
        }
    }

    // Add transaction
    function addTransaction() {
        const transactionsList = document.querySelector('.transactions-list');
        if (!transactionsList) return;

        const transactionTypes = ['Swap', 'Stake', 'Vote', 'Transfer'];
        const icons = ['🔄', '💎', '🏛️', '💸'];
        const amounts = ['1,000 USDTg → 2,000 USDTgV', '500 USDTgV staked', 'Proposal #42', '100 USDTg transferred'];
        
        const randomIndex = Math.floor(Math.random() * transactionTypes.length);
        
        const transactionItem = document.createElement('div');
        transactionItem.className = 'transaction-item';
        transactionItem.innerHTML = `
            <div class="tx-icon">${icons[randomIndex]}</div>
            <div class="tx-details">
                <div class="tx-type">${transactionTypes[randomIndex]}</div>
                <div class="tx-amount">${amounts[randomIndex]}</div>
                <div class="tx-time">Just now</div>
            </div>
            <div class="tx-status success">Success</div>
        `;
        
        transactionsList.insertBefore(transactionItem, transactionsList.firstChild);
        
        // Remove old transactions if more than 5
        const transactions = transactionsList.querySelectorAll('.transaction-item');
        if (transactions.length > 5) {
            transactions[transactions.length - 1].remove();
        }
    }

    // Update proposal votes
    function updateProposalVotes() {
        const voteFor = document.querySelector('.vote-for');
        if (voteFor) {
            const currentWidth = parseFloat(voteFor.style.width);
            const variation = (Math.random() - 0.5) * 5;
            const newWidth = Math.max(0, Math.min(100, currentWidth + variation));
            voteFor.style.width = `${newWidth}%`;
            
            const forCount = document.querySelector('.vote-counts .for');
            const againstCount = document.querySelector('.vote-counts .against');
            if (forCount && againstCount) {
                forCount.textContent = `${newWidth.toFixed(0)}% For`;
                againstCount.textContent = `${(100 - newWidth).toFixed(0)}% Against`;
            }
        }
    }

    // Initialize everything
    initializeCharts();
    
    // Update data every 5 seconds
    setInterval(updateRealTimeData, 5000);
    
    // Add transaction every 30 seconds
    setInterval(addTransaction, 30000);
    
    // Update proposal votes every 10 seconds
    setInterval(updateProposalVotes, 10000);

    // Add click handlers for trading buttons
    const tradeButtons = document.querySelectorAll('.btn-primary');
    tradeButtons.forEach(button => {
        if (button.textContent.includes('Trade')) {
            button.addEventListener('click', function() {
                const card = this.closest('.pair-card, .pool-card, tr');
                let pairName = 'Unknown';
                
                if (card) {
                    const nameElement = card.querySelector('.pair-name, .pool-name, td:first-child');
                    if (nameElement) {
                        pairName = nameElement.textContent;
                    }
                }
                
                // Redirect to trading interface
                window.open('../trading/trade-interface.html', '_blank');
            });
        }
    });

    // Add click handlers for staking buttons
    const stakeButtons = document.querySelectorAll('.btn-primary');
    stakeButtons.forEach(button => {
        if (button.textContent.includes('Stake')) {
            button.addEventListener('click', function() {
                const card = this.closest('.pool-card');
                let poolName = 'Unknown';
                
                if (card) {
                    const nameElement = card.querySelector('.pool-name');
                    if (nameElement) {
                        poolName = nameElement.textContent;
                    }
                }
                
                // Redirect to staking interface
                window.open('../staking/stake-interface.html', '_blank');
            });
        }
    });

    // Add click handlers for vote buttons
    const voteButtons = document.querySelectorAll('.btn-primary');
    voteButtons.forEach(button => {
        if (button.textContent.includes('Vote')) {
            button.addEventListener('click', function() {
                const card = this.closest('.proposal-card');
                let proposalTitle = 'Unknown';
                
                if (card) {
                    const titleElement = card.querySelector('.proposal-title');
                    if (titleElement) {
                        proposalTitle = titleElement.textContent;
                    }
                }
                
                alert(`Voting on ${proposalTitle} - Redirecting to governance interface...`);
            });
        }
    });

    // Add click handlers for connect wallet button
    const connectWalletBtn = document.querySelector('.header-actions .btn-primary');
    if (connectWalletBtn) {
        connectWalletBtn.addEventListener('click', function() {
            alert('Connecting to USDTgVerse Wallet...');
            this.textContent = 'Connected';
            this.style.background = '#28a745';
        });
    }

    // Add responsive sidebar toggle for mobile
    function createMobileMenu() {
        const sidebar = document.querySelector('.sidebar');
        const mainContent = document.querySelector('.main-content');
        
        // Create mobile menu button
        const menuButton = document.createElement('button');
        menuButton.className = 'mobile-menu-btn';
        menuButton.innerHTML = '☰';
        menuButton.style.cssText = `
            position: fixed;
            top: 1rem;
            left: 1rem;
            z-index: 1001;
            background: #667eea;
            color: white;
            border: none;
            padding: 0.5rem;
            border-radius: 4px;
            cursor: pointer;
            display: none;
        `;
        
        document.body.appendChild(menuButton);
        
        menuButton.addEventListener('click', function() {
            sidebar.classList.toggle('open');
        });
        
        // Show/hide menu button based on screen size
        function checkScreenSize() {
            if (window.innerWidth <= 768) {
                menuButton.style.display = 'block';
                sidebar.classList.remove('open');
            } else {
                menuButton.style.display = 'none';
                sidebar.classList.remove('open');
            }
        }
        
        window.addEventListener('resize', checkScreenSize);
        checkScreenSize();
    }

    createMobileMenu();

    console.log('USDTgVerse Dashboard Loaded Successfully!');
    console.log('🚀 Dashboard Features:');
    console.log('  📊 Real-time price updates');
    console.log('  📈 Interactive charts');
    console.log('  💰 Token trading interface');
    console.log('  💎 Staking pools');
    console.log('  🏛️ Governance voting');
    console.log('  📱 Mobile responsive');
});
