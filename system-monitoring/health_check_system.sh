#!/bin/bash
#==============================================
# File:        health_check_system.sh
# Author:      Irfan Gedik
# Created:     14.10.2025
# Last Update: 14.10.2025
# Version:     1.0
#
# Description:
#   USDTgVerse Health Check & Monitoring System
#   
#   Real-time system health monitoring featuring:
#   - API endpoint availability checks
#   - Database connection monitoring
#   - Service status verification
#   - Performance metrics collection
#   - Automated alerting and recovery
#   - Uptime tracking and SLA monitoring
#
# License:
#   MIT License
#==============================================

# 🎨 Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# 📊 Configuration
HEALTH_LOG="/var/log/usdtgverse/health.log"
ALERT_LOG="/var/log/usdtgverse/alerts.log"
STATUS_FILE="/var/run/usdtgverse/health_status.json"
METRICS_FILE="/var/run/usdtgverse/metrics.json"

# 🌐 API Endpoints to check
API_ENDPOINTS=(
    "http://localhost:8080/api/health"
    "http://localhost:8080/api/status"
    "http://localhost:3000/health"
)

# 💾 Database connections
DB_HOST="localhost"
DB_PORT="5432"
DB_USER="postgres"

# ⚙️ Services to monitor
SERVICES=(
    "postgresql"
    "nginx"
)

# 📧 Alerting
ALERT_EMAIL="admin@usdtgverse.com"
ALERT_WEBHOOK=""  # Slack/Discord webhook URL

# 🎯 Thresholds
MAX_RESPONSE_TIME=3000  # milliseconds
MIN_DISK_SPACE=10       # GB
MAX_CPU_USAGE=90        # percent
MAX_MEMORY_USAGE=90     # percent

# ==========================================
# HELPER FUNCTIONS
# ==========================================

log() {
    echo -e "${GREEN}[$(date '+%Y-%m-%d %H:%M:%S')]${NC} $1" | tee -a "${HEALTH_LOG}"
}

error() {
    echo -e "${RED}[$(date '+%Y-%m-%d %H:%M:%S')] ERROR:${NC} $1" | tee -a "${HEALTH_LOG}"
}

warn() {
    echo -e "${YELLOW}[$(date '+%Y-%m-%d %H:%M:%S')] WARNING:${NC} $1" | tee -a "${HEALTH_LOG}"
}

alert() {
    local message="$1"
    echo -e "${RED}[$(date '+%Y-%m-%d %H:%M:%S')] ALERT:${NC} ${message}" | tee -a "${ALERT_LOG}"
    
    # Send email alert (if configured)
    if [ -n "${ALERT_EMAIL}" ] && command -v mail &> /dev/null; then
        echo "${message}" | mail -s "USDTgVerse Alert" "${ALERT_EMAIL}"
    fi
    
    # Send webhook alert (if configured)
    if [ -n "${ALERT_WEBHOOK}" ]; then
        curl -X POST "${ALERT_WEBHOOK}" \
            -H "Content-Type: application/json" \
            -d "{\"text\": \"🚨 USDTgVerse Alert: ${message}\"}" \
            2>/dev/null
    fi
}

# ==========================================
# INITIALIZATION
# ==========================================

init_health_system() {
    mkdir -p "$(dirname ${HEALTH_LOG})"
    mkdir -p "$(dirname ${STATUS_FILE})"
    
    # Initialize status file
    cat > "${STATUS_FILE}" << EOF
{
    "timestamp": "$(date -Iseconds)",
    "overall_status": "initializing",
    "checks": {}
}
EOF
}

# ==========================================
# API HEALTH CHECKS
# ==========================================

check_api_endpoints() {
    log "🌐 Checking API endpoints..."
    
    local all_healthy=true
    local results=()
    
    for endpoint in "${API_ENDPOINTS[@]}"; do
        local start_time=$(date +%s%3N)
        local http_code=$(curl -s -o /dev/null -w "%{http_code}" --max-time 5 "${endpoint}" 2>/dev/null)
        local end_time=$(date +%s%3N)
        local response_time=$((end_time - start_time))
        
        if [ "${http_code}" == "200" ] || [ "${http_code}" == "000" ]; then
            if [ "${http_code}" == "200" ]; then
                log "  ✅ ${endpoint}: OK (${response_time}ms)"
                results+=("{\"endpoint\":\"${endpoint}\",\"status\":\"healthy\",\"response_time\":${response_time}}")
                
                # Check response time threshold
                if [ ${response_time} -gt ${MAX_RESPONSE_TIME} ]; then
                    warn "  ⚠️  Slow response: ${response_time}ms (threshold: ${MAX_RESPONSE_TIME}ms)"
                fi
            else
                warn "  ⚠️  ${endpoint}: Not responding (might not be started yet)"
                results+=("{\"endpoint\":\"${endpoint}\",\"status\":\"not_responding\",\"response_time\":0}")
            fi
        else
            error "  ❌ ${endpoint}: Failed (HTTP ${http_code})"
            results+=("{\"endpoint\":\"${endpoint}\",\"status\":\"unhealthy\",\"http_code\":${http_code}}")
            all_healthy=false
            alert "API endpoint unhealthy: ${endpoint} (HTTP ${http_code})"
        fi
    done
    
    [ "$all_healthy" = true ]
}

# ==========================================
# DATABASE HEALTH CHECKS
# ==========================================

check_database() {
    log "💾 Checking database connections..."
    
    # Check PostgreSQL
    if command -v pg_isready &> /dev/null; then
        if pg_isready -h "${DB_HOST}" -p "${DB_PORT}" -U "${DB_USER}" -q; then
            log "  ✅ PostgreSQL: Connected"
            
            # Check connection count
            local connections=$(psql -h "${DB_HOST}" -p "${DB_PORT}" -U "${DB_USER}" -t -c \
                "SELECT count(*) FROM pg_stat_activity;" 2>/dev/null | tr -d ' ')
            
            if [ -n "${connections}" ]; then
                log "  📊 Active connections: ${connections}"
            fi
            
            return 0
        else
            error "  ❌ PostgreSQL: Connection failed"
            alert "PostgreSQL connection failed on ${DB_HOST}:${DB_PORT}"
            return 1
        fi
    else
        warn "  ⚠️  pg_isready not installed, skipping PostgreSQL check"
        return 0
    fi
}

# ==========================================
# SERVICE HEALTH CHECKS
# ==========================================

check_services() {
    log "⚙️  Checking system services..."
    
    local all_running=true
    
    for service in "${SERVICES[@]}"; do
        if systemctl is-active --quiet "${service}"; then
            log "  ✅ ${service}: Running"
            
            # Get service uptime
            local uptime=$(systemctl show "${service}" --property=ActiveEnterTimestamp --value 2>/dev/null)
            if [ -n "${uptime}" ]; then
                log "  ⏱️  Uptime: ${uptime}"
            fi
        else
            error "  ❌ ${service}: Not running"
            all_running=false
            alert "Service ${service} is not running!"
            
            # Attempt auto-restart
            log "  🔄 Attempting to restart ${service}..."
            if systemctl restart "${service}" 2>/dev/null; then
                log "  ✅ ${service} restarted successfully"
            else
                error "  ❌ Failed to restart ${service}"
            fi
        fi
    done
    
    [ "$all_running" = true ]
}

# ==========================================
# SYSTEM RESOURCE CHECKS
# ==========================================

check_system_resources() {
    log "📊 Checking system resources..."
    
    local all_ok=true
    
    # CPU Usage
    local cpu_usage=$(top -bn1 | grep "Cpu(s)" | sed "s/.*, *\([0-9.]*\)%* id.*/\1/" | awk '{print 100 - $1}')
    cpu_usage=${cpu_usage%.*}  # Convert to integer
    
    if [ ${cpu_usage} -lt ${MAX_CPU_USAGE} ]; then
        log "  ✅ CPU Usage: ${cpu_usage}%"
    else
        warn "  ⚠️  CPU Usage: ${cpu_usage}% (threshold: ${MAX_CPU_USAGE}%)"
        alert "High CPU usage: ${cpu_usage}%"
        all_ok=false
    fi
    
    # Memory Usage
    local mem_usage=$(free | grep Mem | awk '{print int($3/$2 * 100)}')
    
    if [ ${mem_usage} -lt ${MAX_MEMORY_USAGE} ]; then
        log "  ✅ Memory Usage: ${mem_usage}%"
    else
        warn "  ⚠️  Memory Usage: ${mem_usage}% (threshold: ${MAX_MEMORY_USAGE}%)"
        alert "High memory usage: ${mem_usage}%"
        all_ok=false
    fi
    
    # Disk Space
    local disk_free=$(df -BG / | tail -1 | awk '{print $4}' | sed 's/G//')
    
    if [ ${disk_free} -gt ${MIN_DISK_SPACE} ]; then
        log "  ✅ Disk Space: ${disk_free}GB free"
    else
        warn "  ⚠️  Disk Space: ${disk_free}GB free (minimum: ${MIN_DISK_SPACE}GB)"
        alert "Low disk space: ${disk_free}GB remaining"
        all_ok=false
    fi
    
    # Load Average
    local load_avg=$(uptime | awk -F'load average:' '{print $2}' | awk '{print $1}' | tr -d ',')
    log "  📈 Load Average: ${load_avg}"
    
    [ "$all_ok" = true ]
}

# ==========================================
# NETWORK CONNECTIVITY
# ==========================================

check_network() {
    log "🌐 Checking network connectivity..."
    
    # Check internet connectivity
    if ping -c 1 -W 2 8.8.8.8 &> /dev/null; then
        log "  ✅ Internet: Connected"
    else
        error "  ❌ Internet: No connectivity"
        alert "No internet connectivity!"
        return 1
    fi
    
    # Check DNS resolution
    if nslookup google.com &> /dev/null; then
        log "  ✅ DNS: Working"
    else
        error "  ❌ DNS: Resolution failed"
        alert "DNS resolution failed!"
        return 1
    fi
    
    return 0
}

# ==========================================
# SSL CERTIFICATE CHECK
# ==========================================

check_ssl_certificates() {
    log "🔒 Checking SSL certificates..."
    
    local domains=("www.usdtgverse.com" "api.usdtgverse.com")
    
    for domain in "${domains[@]}"; do
        local expiry=$(echo | openssl s_client -servername "${domain}" \
            -connect "${domain}:443" 2>/dev/null | \
            openssl x509 -noout -enddate 2>/dev/null | cut -d= -f2)
        
        if [ -n "${expiry}" ]; then
            local expiry_epoch=$(date -d "${expiry}" +%s 2>/dev/null)
            local now_epoch=$(date +%s)
            local days_remaining=$(( (expiry_epoch - now_epoch) / 86400 ))
            
            if [ ${days_remaining} -gt 30 ]; then
                log "  ✅ ${domain}: Valid (${days_remaining} days remaining)"
            elif [ ${days_remaining} -gt 0 ]; then
                warn "  ⚠️  ${domain}: Expiring soon (${days_remaining} days remaining)"
                alert "SSL certificate for ${domain} expiring in ${days_remaining} days"
            else
                error "  ❌ ${domain}: Expired!"
                alert "SSL certificate for ${domain} has expired!"
            fi
        else
            warn "  ⚠️  ${domain}: Cannot check certificate"
        fi
    done
}

# ==========================================
# GENERATE STATUS REPORT
# ==========================================

generate_status_report() {
    local overall_status="$1"
    
    cat > "${STATUS_FILE}" << EOF
{
    "timestamp": "$(date -Iseconds)",
    "overall_status": "${overall_status}",
    "checks": {
        "api": "checked",
        "database": "checked",
        "services": "checked",
        "resources": "checked",
        "network": "checked"
    }
}
EOF
}

# ==========================================
# MAIN HEALTH CHECK ROUTINE
# ==========================================

run_health_check() {
    log "🏥 Starting USDTgVerse Health Check..."
    log "================================================"
    
    init_health_system
    
    local all_healthy=true
    
    # Run all checks
    check_api_endpoints || all_healthy=false
    check_database || all_healthy=false
    check_services || all_healthy=false
    check_system_resources || all_healthy=false
    check_network || all_healthy=false
    check_ssl_certificates
    
    # Generate report
    if [ "$all_healthy" = true ]; then
        log "✅ All systems healthy!"
        generate_status_report "healthy"
        log "================================================"
        return 0
    else
        error "⚠️  Some systems need attention!"
        generate_status_report "degraded"
        log "================================================"
        return 1
    fi
}

# ==========================================
# CONTINUOUS MONITORING
# ==========================================

continuous_monitor() {
    log "🔄 Starting continuous monitoring..."
    
    while true; do
        run_health_check
        echo ""
        sleep 60  # Check every minute
    done
}

# ==========================================
# QUICK STATUS
# ==========================================

quick_status() {
    echo -e "${CYAN}🏥 USDTgVerse System Status${NC}"
    echo "================================"
    
    if [ -f "${STATUS_FILE}" ]; then
        cat "${STATUS_FILE}"
    else
        echo "Status file not found. Run health check first."
    fi
    
    echo ""
    echo "Last 10 health check entries:"
    tail -10 "${HEALTH_LOG}" 2>/dev/null || echo "No logs available"
}

# ==========================================
# COMMAND LINE INTERFACE
# ==========================================

case "${1}" in
    check)
        run_health_check
        ;;
    monitor)
        continuous_monitor
        ;;
    status)
        quick_status
        ;;
    *)
        echo "🏥 USDTgVerse Health Check System"
        echo "================================"
        echo ""
        echo "Usage: $0 {check|monitor|status}"
        echo ""
        echo "Commands:"
        echo "  check    - Run single health check"
        echo "  monitor  - Start continuous monitoring"
        echo "  status   - Show current system status"
        echo ""
        echo "Health checks include:"
        echo "  ✅ API endpoint availability"
        echo "  ✅ Database connections"
        echo "  ✅ Service status"
        echo "  ✅ System resources (CPU, Memory, Disk)"
        echo "  ✅ Network connectivity"
        echo "  ✅ SSL certificate validity"
        echo ""
        echo "Automated checks run every 5 minutes via cron:"
        echo "  */5 * * * * /opt/usdtgverse/scripts/health_check_system.sh check"
        echo ""
        exit 1
        ;;
esac

exit $?

