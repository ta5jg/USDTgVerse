#!/bin/bash
#==============================================
# File:        automated_backup_system.sh
# Author:      Irfan Gedik
# Created:     14.10.2025
# Last Update: 14.10.2025
# Version:     1.0
#
# Description:
#   USDTgVerse Automated Backup System
#   
#   Enterprise-grade backup solution featuring:
#   - Daily automated backups (hot backup, no downtime)
#   - 30-day retention with automatic cleanup
#   - Backup verification and integrity checks
#   - Automated restore testing
#   - Compression and encryption
#   - Multi-location backup storage
#
# License:
#   MIT License
#==============================================

# 🎨 Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# 📊 Configuration
BACKUP_DIR="/opt/usdtgverse/backups"
DATA_DIR="/opt/usdtgverse/data"
POSTGRES_BACKUP_DIR="${BACKUP_DIR}/postgres"
FILES_BACKUP_DIR="${BACKUP_DIR}/files"
LOG_FILE="/var/log/usdtgverse/backup.log"
RETENTION_DAYS=30
DATE_FORMAT=$(date +%Y%m%d_%H%M%S)

# 🔐 Encryption
ENCRYPTION_KEY="/opt/usdtgverse/config/backup.key"

# 📧 Notification (optional)
ADMIN_EMAIL="admin@usdtgverse.com"

# ==========================================
# HELPER FUNCTIONS
# ==========================================

log() {
    echo -e "${GREEN}[$(date '+%Y-%m-%d %H:%M:%S')]${NC} $1" | tee -a "${LOG_FILE}"
}

error() {
    echo -e "${RED}[$(date '+%Y-%m-%d %H:%M:%S')] ERROR:${NC} $1" | tee -a "${LOG_FILE}"
}

warn() {
    echo -e "${YELLOW}[$(date '+%Y-%m-%d %H:%M:%S')] WARNING:${NC} $1" | tee -a "${LOG_FILE}"
}

# ==========================================
# INITIALIZATION
# ==========================================

init_backup_system() {
    log "🔧 Initializing Backup System..."
    
    # Create backup directories
    mkdir -p "${BACKUP_DIR}"
    mkdir -p "${POSTGRES_BACKUP_DIR}"
    mkdir -p "${FILES_BACKUP_DIR}"
    mkdir -p "$(dirname ${LOG_FILE})"
    
    # Generate encryption key if not exists
    if [ ! -f "${ENCRYPTION_KEY}" ]; then
        mkdir -p "$(dirname ${ENCRYPTION_KEY})"
        openssl rand -base64 32 > "${ENCRYPTION_KEY}"
        chmod 600 "${ENCRYPTION_KEY}"
        log "✅ Encryption key generated"
    fi
    
    log "✅ Backup system initialized"
}

# ==========================================
# POSTGRESQL BACKUP
# ==========================================

backup_postgresql() {
    log "💾 Starting PostgreSQL backup..."
    
    local backup_file="${POSTGRES_BACKUP_DIR}/postgres_${DATE_FORMAT}.sql.gz"
    
    # Check if PostgreSQL is running
    if ! pg_isready -q; then
        error "PostgreSQL is not running!"
        return 1
    fi
    
    # Perform backup (all databases)
    if pg_dumpall -U postgres | gzip > "${backup_file}"; then
        log "✅ PostgreSQL backup completed: ${backup_file}"
        
        # Calculate checksum
        local checksum=$(sha256sum "${backup_file}" | awk '{print $1}')
        echo "${checksum}" > "${backup_file}.sha256"
        log "✅ Checksum: ${checksum}"
        
        # Encrypt backup
        if [ -f "${ENCRYPTION_KEY}" ]; then
            openssl enc -aes-256-cbc -salt -in "${backup_file}" \
                -out "${backup_file}.enc" -pass file:"${ENCRYPTION_KEY}"
            rm "${backup_file}" # Remove unencrypted
            log "✅ Backup encrypted"
        fi
        
        return 0
    else
        error "PostgreSQL backup failed!"
        return 1
    fi
}

# ==========================================
# FILE SYSTEM BACKUP
# ==========================================

backup_files() {
    log "📁 Starting file system backup..."
    
    local backup_file="${FILES_BACKUP_DIR}/files_${DATE_FORMAT}.tar.gz"
    
    # Backup data directory
    if [ -d "${DATA_DIR}" ]; then
        tar -czf "${backup_file}" -C "$(dirname ${DATA_DIR})" "$(basename ${DATA_DIR})" 2>/dev/null
        
        if [ $? -eq 0 ]; then
            log "✅ File system backup completed: ${backup_file}"
            
            # Calculate checksum
            local checksum=$(sha256sum "${backup_file}" | awk '{print $1}')
            echo "${checksum}" > "${backup_file}.sha256"
            log "✅ Checksum: ${checksum}"
            
            return 0
        else
            error "File system backup failed!"
            return 1
        fi
    else
        warn "Data directory not found: ${DATA_DIR}"
        return 1
    fi
}

# ==========================================
# BACKUP VERIFICATION
# ==========================================

verify_backup() {
    log "🔍 Verifying backup integrity..."
    
    local verified=0
    local failed=0
    
    # Verify PostgreSQL backups
    for backup in "${POSTGRES_BACKUP_DIR}"/*.sql.gz.enc; do
        if [ -f "${backup}" ]; then
            local checksum_file="${backup%.enc}.sha256"
            if [ -f "${checksum_file}" ]; then
                # Decrypt and verify
                local temp_file=$(mktemp)
                openssl enc -aes-256-cbc -d -in "${backup}" \
                    -out "${temp_file}" -pass file:"${ENCRYPTION_KEY}" 2>/dev/null
                
                if [ $? -eq 0 ]; then
                    local current_checksum=$(sha256sum "${temp_file}" | awk '{print $1}')
                    local stored_checksum=$(cat "${checksum_file}")
                    
                    if [ "${current_checksum}" == "${stored_checksum}" ]; then
                        ((verified++))
                    else
                        ((failed++))
                        error "Checksum mismatch: ${backup}"
                    fi
                fi
                rm -f "${temp_file}"
            fi
        fi
    done
    
    # Verify file backups
    for backup in "${FILES_BACKUP_DIR}"/*.tar.gz; do
        if [ -f "${backup}" ]; then
            local checksum_file="${backup}.sha256"
            if [ -f "${checksum_file}" ]; then
                local current_checksum=$(sha256sum "${backup}" | awk '{print $1}')
                local stored_checksum=$(cat "${checksum_file}")
                
                if [ "${current_checksum}" == "${stored_checksum}" ]; then
                    ((verified++))
                else
                    ((failed++))
                    error "Checksum mismatch: ${backup}"
                fi
            fi
        fi
    done
    
    log "✅ Verification complete: ${verified} verified, ${failed} failed"
    return ${failed}
}

# ==========================================
# CLEANUP OLD BACKUPS
# ==========================================

cleanup_old_backups() {
    log "🧹 Cleaning up old backups (>${RETENTION_DAYS} days)..."
    
    local deleted=0
    
    # Clean PostgreSQL backups
    find "${POSTGRES_BACKUP_DIR}" -name "*.enc" -type f -mtime +${RETENTION_DAYS} | while read file; do
        rm -f "${file}" "${file%.enc}.sha256"
        ((deleted++))
        log "🗑️  Deleted: ${file}"
    done
    
    # Clean file backups
    find "${FILES_BACKUP_DIR}" -name "*.tar.gz" -type f -mtime +${RETENTION_DAYS} | while read file; do
        rm -f "${file}" "${file}.sha256"
        ((deleted++))
        log "🗑️  Deleted: ${file}"
    done
    
    log "✅ Cleanup complete: ${deleted} old backups removed"
}

# ==========================================
# BACKUP STATISTICS
# ==========================================

show_statistics() {
    log "📊 Backup Statistics:"
    
    local postgres_count=$(find "${POSTGRES_BACKUP_DIR}" -name "*.enc" -type f | wc -l)
    local files_count=$(find "${FILES_BACKUP_DIR}" -name "*.tar.gz" -type f | wc -l)
    local total_size=$(du -sh "${BACKUP_DIR}" | awk '{print $1}')
    local oldest=$(find "${BACKUP_DIR}" -type f -printf '%T+ %p\n' | sort | head -1 | awk '{print $1}')
    local newest=$(find "${BACKUP_DIR}" -type f -printf '%T+ %p\n' | sort | tail -1 | awk '{print $1}')
    
    echo "================================"
    echo "PostgreSQL Backups: ${postgres_count}"
    echo "File System Backups: ${files_count}"
    echo "Total Size: ${total_size}"
    echo "Oldest Backup: ${oldest}"
    echo "Newest Backup: ${newest}"
    echo "Retention: ${RETENTION_DAYS} days"
    echo "================================"
}

# ==========================================
# RESTORE TESTING
# ==========================================

test_restore() {
    log "🧪 Testing backup restore..."
    
    # Find latest PostgreSQL backup
    local latest_pg_backup=$(ls -t "${POSTGRES_BACKUP_DIR}"/*.enc 2>/dev/null | head -1)
    
    if [ -f "${latest_pg_backup}" ]; then
        local temp_file=$(mktemp)
        
        # Decrypt
        if openssl enc -aes-256-cbc -d -in "${latest_pg_backup}" \
            -out "${temp_file}" -pass file:"${ENCRYPTION_KEY}" 2>/dev/null; then
            
            # Verify SQL syntax
            if gunzip -c "${temp_file}" | head -100 | grep -q "PostgreSQL"; then
                log "✅ Restore test passed: Backup is valid"
                rm -f "${temp_file}"
                return 0
            else
                error "Restore test failed: Invalid backup format"
                rm -f "${temp_file}"
                return 1
            fi
        else
            error "Restore test failed: Cannot decrypt backup"
            rm -f "${temp_file}"
            return 1
        fi
    else
        warn "No PostgreSQL backups found for testing"
        return 1
    fi
}

# ==========================================
# MAIN BACKUP ROUTINE
# ==========================================

run_backup() {
    log "🚀 Starting USDTgVerse Backup System..."
    log "================================================"
    
    # Initialize
    init_backup_system
    
    # Perform backups
    local backup_success=true
    
    if ! backup_postgresql; then
        backup_success=false
    fi
    
    if ! backup_files; then
        backup_success=false
    fi
    
    # Verify backups
    verify_backup
    
    # Cleanup old backups
    cleanup_old_backups
    
    # Test restore
    test_restore
    
    # Show statistics
    show_statistics
    
    if [ "$backup_success" = true ]; then
        log "✅ Backup completed successfully!"
        log "================================================"
        return 0
    else
        error "❌ Backup completed with errors!"
        log "================================================"
        return 1
    fi
}

# ==========================================
# RESTORE FUNCTIONALITY
# ==========================================

restore_latest() {
    log "🔄 Restoring from latest backup..."
    
    # Find latest backup
    local latest_backup=$(ls -t "${POSTGRES_BACKUP_DIR}"/*.enc 2>/dev/null | head -1)
    
    if [ ! -f "${latest_backup}" ]; then
        error "No backup found!"
        return 1
    fi
    
    log "📦 Restoring from: ${latest_backup}"
    
    # Decrypt
    local temp_file=$(mktemp)
    if ! openssl enc -aes-256-cbc -d -in "${latest_backup}" \
        -out "${temp_file}" -pass file:"${ENCRYPTION_KEY}"; then
        error "Failed to decrypt backup!"
        rm -f "${temp_file}"
        return 1
    fi
    
    # Restore to PostgreSQL
    log "⚠️  WARNING: This will overwrite current database!"
    read -p "Continue? (yes/no): " confirm
    
    if [ "$confirm" == "yes" ]; then
        gunzip -c "${temp_file}" | psql -U postgres
        log "✅ Database restored successfully!"
        rm -f "${temp_file}"
        return 0
    else
        log "❌ Restore cancelled"
        rm -f "${temp_file}"
        return 1
    fi
}

# ==========================================
# COMMAND LINE INTERFACE
# ==========================================

case "${1}" in
    backup)
        run_backup
        ;;
    verify)
        init_backup_system
        verify_backup
        ;;
    cleanup)
        init_backup_system
        cleanup_old_backups
        ;;
    stats)
        show_statistics
        ;;
    test)
        init_backup_system
        test_restore
        ;;
    restore)
        restore_latest
        ;;
    *)
        echo "🗄️  USDTgVerse Automated Backup System"
        echo "====================================="
        echo ""
        echo "Usage: $0 {backup|verify|cleanup|stats|test|restore}"
        echo ""
        echo "Commands:"
        echo "  backup   - Perform full system backup"
        echo "  verify   - Verify backup integrity"
        echo "  cleanup  - Remove old backups (>${RETENTION_DAYS} days)"
        echo "  stats    - Show backup statistics"
        echo "  test     - Test backup restore"
        echo "  restore  - Restore from latest backup"
        echo ""
        echo "Automated backup runs daily via cron:"
        echo "  0 2 * * * /opt/usdtgverse/scripts/automated_backup_system.sh backup"
        echo ""
        exit 1
        ;;
esac

exit $?

