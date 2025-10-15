#!/bin/bash
#==============================================
# File:        setup_monitoring.sh
# Author:      Irfan Gedik
# Created:     14.10.2025
# Last Update: 14.10.2025
# Version:     1.0
#
# Description:
#   USDTgVerse Monitoring Setup Script
#   
#   Sets up automated monitoring and backup systems:
#   - Installs cron jobs for automated backups
#   - Configures health check monitoring
#   - Sets up log rotation
#   - Configures alert notifications
#
# License:
#   MIT License
#==============================================

set -e

echo "🚀 USDTgVerse Monitoring & Backup Setup"
echo "========================================"
echo ""

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo -e "${BLUE}📍 Script location: ${SCRIPT_DIR}${NC}"
echo ""

# ==========================================
# 1. CREATE DIRECTORIES
# ==========================================

echo -e "${GREEN}1️⃣ Creating directories...${NC}"

sudo mkdir -p /var/log/usdtgverse
sudo mkdir -p /var/run/usdtgverse
sudo mkdir -p /opt/usdtgverse/backups
sudo mkdir -p /opt/usdtgverse/config
sudo mkdir -p /opt/usdtgverse/scripts

echo "✅ Directories created"
echo ""

# ==========================================
# 2. COPY SCRIPTS
# ==========================================

echo -e "${GREEN}2️⃣ Copying scripts to /opt/usdtgverse/scripts...${NC}"

sudo cp "${SCRIPT_DIR}/automated_backup_system.sh" /opt/usdtgverse/scripts/
sudo cp "${SCRIPT_DIR}/health_check_system.sh" /opt/usdtgverse/scripts/
sudo chmod +x /opt/usdtgverse/scripts/*.sh

echo "✅ Scripts copied and made executable"
echo ""

# ==========================================
# 3. SETUP CRON JOBS
# ==========================================

echo -e "${GREEN}3️⃣ Setting up cron jobs...${NC}"

# Backup cron job (daily at 2 AM)
BACKUP_CRON="0 2 * * * /opt/usdtgverse/scripts/automated_backup_system.sh backup >> /var/log/usdtgverse/backup_cron.log 2>&1"

# Health check cron job (every 5 minutes)
HEALTH_CRON="*/5 * * * * /opt/usdtgverse/scripts/health_check_system.sh check >> /var/log/usdtgverse/health_cron.log 2>&1"

# Add to crontab (if not already present)
(crontab -l 2>/dev/null | grep -v "automated_backup_system.sh"; echo "${BACKUP_CRON}") | crontab -
(crontab -l 2>/dev/null | grep -v "health_check_system.sh"; echo "${HEALTH_CRON}") | crontab -

echo "✅ Cron jobs configured:"
echo "   - Daily backups at 2:00 AM"
echo "   - Health checks every 5 minutes"
echo ""

# ==========================================
# 4. VERIFY CRON JOBS
# ==========================================

echo -e "${GREEN}4️⃣ Verifying cron jobs...${NC}"
echo ""
crontab -l | grep -E "(backup|health_check)"
echo ""

# ==========================================
# 5. SETUP LOG ROTATION
# ==========================================

echo -e "${GREEN}5️⃣ Setting up log rotation...${NC}"

sudo tee /etc/logrotate.d/usdtgverse > /dev/null << 'EOF'
/var/log/usdtgverse/*.log {
    daily
    rotate 30
    compress
    delaycompress
    missingok
    notifempty
    create 0644 root root
    sharedscripts
}
EOF

echo "✅ Log rotation configured (30 days retention)"
echo ""

# ==========================================
# 6. INITIAL TEST RUN
# ==========================================

echo -e "${GREEN}6️⃣ Running initial tests...${NC}"
echo ""

echo -e "${BLUE}Testing backup system...${NC}"
sudo /opt/usdtgverse/scripts/automated_backup_system.sh stats
echo ""

echo -e "${BLUE}Testing health check system...${NC}"
/opt/usdtgverse/scripts/health_check_system.sh check
echo ""

# ==========================================
# 7. COMPLETION SUMMARY
# ==========================================

echo -e "${GREEN}🎉 Setup Complete!${NC}"
echo "================================"
echo ""
echo "✅ Automated Backup System:"
echo "   - Runs daily at 2:00 AM"
echo "   - Backs up PostgreSQL databases"
echo "   - Backs up file system"
echo "   - 30-day retention"
echo "   - Encrypted backups"
echo "   - Manual run: sudo /opt/usdtgverse/scripts/automated_backup_system.sh backup"
echo ""
echo "✅ Health Check System:"
echo "   - Runs every 5 minutes"
echo "   - Monitors API endpoints"
echo "   - Checks database connections"
echo "   - Monitors system resources"
echo "   - Auto-recovery for failed services"
echo "   - Manual run: /opt/usdtgverse/scripts/health_check_system.sh check"
echo ""
echo "📊 View logs:"
echo "   - Backup logs: tail -f /var/log/usdtgverse/backup.log"
echo "   - Health logs: tail -f /var/log/usdtgverse/health.log"
echo "   - Alert logs: tail -f /var/log/usdtgverse/alerts.log"
echo ""
echo "📋 Check cron jobs:"
echo "   crontab -l"
echo ""
echo "🔧 Manage backups:"
echo "   sudo /opt/usdtgverse/scripts/automated_backup_system.sh stats"
echo "   sudo /opt/usdtgverse/scripts/automated_backup_system.sh verify"
echo "   sudo /opt/usdtgverse/scripts/automated_backup_system.sh cleanup"
echo ""
echo -e "${GREEN}🌟 USDTgVerse Monitoring & Backup System is now active!${NC}"
echo ""

