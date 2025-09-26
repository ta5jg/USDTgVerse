<!--
==============================================
 File:        GRAFANA_MONITORING_TUTORIAL.md
 Author:      Irfan Gedik
 Created:     27.09.2025
 Last Update: 27.09.2025
 Version:     1.0

 Description:
   Grafana Monitoring Tutorial - USDTgVerse
   
   Comprehensive monitoring tutorial including:
   - Access information and credentials
   - Dashboard configuration and setup
   - Monitoring metrics and alerts
   - Performance analysis and optimization
   - Troubleshooting and maintenance

 License:
   MIT License
==============================================
-->

# 📊 Grafana Monitoring Tutorial - USDTgVerse

## 🌐 Access Information

**🔗 Monitoring Dashboard:** https://monitor.usdtgverse.com  
**👤 Default Username:** `admin`  
**🔑 Default Password:** `admin`  
**🖥️ Server:** FRA1 (Frankfurt) - 104.248.251.209

---

## 📋 Step-by-Step Setup Guide

### 1️⃣ First Login & Password Setup

1. **🌐 Open:** https://monitor.usdtgverse.com
2. **👤 Login:** admin / admin
3. **🔑 Set New Password:** Choose strong password (min 8 chars)
4. **✅ Confirm:** Click "Submit" to complete setup

### 2️⃣ Add Prometheus Data Source

1. **⚙️ Navigate:** Left menu → "Administration" → "Data Sources"
2. **➕ Add:** Click "Add data source" button
3. **📈 Select:** Choose "Prometheus" from the list
4. **🔗 Configure:**
   - **Name:** `USDTgVerse Prometheus`
   - **URL:** `http://localhost:9090`
   - **Access:** Server (default)
   - **Scrape interval:** 15s
5. **💾 Save:** Click "Save & Test" (should show green checkmark)

### 3️⃣ Create USDTgVerse Dashboard

1. **📊 Navigate:** Left menu → "Dashboards" → "New Dashboard"
2. **➕ Add Panel:** Click "Add visualization"
3. **📈 Select:** Choose "USDTgVerse Prometheus" data source

---

## 📊 Essential USDTgVerse Metrics

### 🔧 Blockchain Node Metrics

#### **Node Health Status**
```promql
up{job="usdtgverse-nodes"}
```
*Shows which blockchain nodes are online (1 = up, 0 = down)*

#### **Current Block Height**
```promql
usdtgverse_block_height
```
*Real-time blockchain height across all 3 nodes*

#### **Transactions Per Second (TPS)**
```promql
rate(usdtgverse_transactions_total[1m])
```
*Current transaction processing rate*

#### **Validator Performance**
```promql
usdtgverse_validator_uptime_percentage
```
*Validator node uptime percentage*

### 🌉 Bridge Network Metrics

#### **Bridge Transfer Volume**
```promql
sum(usdtgverse_bridge_volume_usd) by (network)
```
*Total USD volume by blockchain network*

#### **Bridge Success Rate**
```promql
(sum(usdtgverse_bridge_transfers_successful) / sum(usdtgverse_bridge_transfers_total)) * 100
```
*Bridge transfer success percentage*

#### **Cross-Chain Activity**
```promql
rate(usdtgverse_bridge_transfers_total[5m]) by (from_network, to_network)
```
*Bridge transfer rate between networks*

### 💰 Financial Metrics

#### **USDTg Price Stability**
```promql
usdtgverse_usdtg_price_usd
```
*USDTg price (should stay at $1.00)*

#### **Total Supply**
```promql
usdtgverse_total_supply_usdtg
```
*Total USDTg in circulation*

#### **Daily Trading Volume**
```promql
increase(usdtgverse_trading_volume_usd[24h])
```
*24-hour trading volume across DEX/CEX*

### 🖥️ System Performance Metrics

#### **CPU Usage**
```promql
100 - (avg by (instance) (rate(node_cpu_seconds_total{mode="idle"}[5m])) * 100)
```
*CPU usage percentage across all nodes*

#### **Memory Usage**
```promql
(node_memory_MemTotal_bytes - node_memory_MemAvailable_bytes) / node_memory_MemTotal_bytes * 100
```
*Memory usage percentage*

#### **Disk Usage**
```promql
100 - ((node_filesystem_avail_bytes{mountpoint="/"} / node_filesystem_size_bytes{mountpoint="/"}) * 100)
```
*Disk usage percentage*

---

## 📈 Dashboard Panel Configurations

### 🎯 Panel 1: Node Status Overview

**📊 Visualization:** Stat Panel  
**📈 Query:** `up{job="usdtgverse-nodes"}`  
**🎨 Display:**
- **Value:** Current status
- **Color:** Green (up) / Red (down)
- **Threshold:** 1 = healthy, 0 = down

### 🎯 Panel 2: Block Height Progress

**📊 Visualization:** Time Series  
**📈 Query:** `usdtgverse_block_height`  
**🎨 Display:**
- **Y-axis:** Block number
- **X-axis:** Time
- **Line:** Smooth, ascending

### 🎯 Panel 3: Bridge Network Activity

**📊 Visualization:** Bar Chart  
**📈 Query:** `sum(rate(usdtgverse_bridge_transfers_total[1h])) by (network)`  
**🎨 Display:**
- **X-axis:** Blockchain networks
- **Y-axis:** Transfer rate
- **Colors:** Network-specific

### 🎯 Panel 4: System Resources

**📊 Visualization:** Gauge  
**📈 Queries:**
- CPU: `100 - (avg(rate(node_cpu_seconds_total{mode="idle"}[5m])) * 100)`
- Memory: `(1 - (node_memory_MemAvailable_bytes / node_memory_MemTotal_bytes)) * 100`
- Disk: `100 - ((node_filesystem_avail_bytes / node_filesystem_size_bytes) * 100)`

---

## 🚨 Alert Rules Setup

### ⚠️ Critical Alerts

#### **Node Down Alert**
```promql
up{job="usdtgverse-nodes"} == 0
```
**Threshold:** Any node down  
**Action:** Immediate notification

#### **High Error Rate**
```promql
rate(usdtgverse_errors_total[5m]) > 0.1
```
**Threshold:** >10% error rate  
**Action:** Alert after 2 minutes

#### **Bridge Failure**
```promql
rate(usdtgverse_bridge_transfers_failed[5m]) > 0.05
```
**Threshold:** >5% bridge failures  
**Action:** Immediate notification

### 📧 Notification Channels

1. **Email Alerts:**
   - **SMTP:** Configure email server
   - **Recipients:** admin@usdtgverse.com
   - **Frequency:** Immediate + digest

2. **Slack Integration:**
   - **Webhook:** Slack channel integration
   - **Format:** Rich notifications with metrics
   - **Channels:** #alerts, #monitoring

---

## 🎨 Dashboard Customization

### 🌌 USDTgVerse Theme

**🎨 Dashboard Settings:**
- **Theme:** Dark
- **Background:** USDTgVerse gradient
- **Colors:** Green (#4CAF50) primary
- **Logo:** USDTgVerse branding

### 📱 Mobile Optimization

**📱 Mobile View:**
- **Responsive:** Auto-adjust for mobile
- **Touch-friendly:** Large buttons/panels
- **Simplified:** Essential metrics only

---

## 🔧 Advanced Features

### 📊 Custom Metrics

Create custom USDTgVerse-specific metrics:

```promql
# OdixPay++ Transaction Volume
sum(rate(odixpay_transactions_total[1h])) by (feature_type)

# Multi-Chain Bridge Efficiency  
avg(usdtgverse_bridge_confirmation_time_seconds) by (network)

# Validator Performance Score
(usdtgverse_validator_blocks_proposed / usdtgverse_validator_blocks_expected) * 100
```

### 🔄 Auto-Refresh

**⏰ Refresh Intervals:**
- **Real-time panels:** 5 seconds
- **Historical charts:** 30 seconds  
- **Summary stats:** 1 minute

### 📤 Export & Sharing

**📊 Dashboard Export:**
- **JSON:** Export dashboard configuration
- **PDF:** Generate reports
- **PNG:** Static snapshots

---

## 💡 Pro Tips

### 🎯 Monitoring Best Practices

1. **📊 Start Simple:** Begin with basic node health
2. **📈 Add Gradually:** Expand metrics over time
3. **🚨 Set Alerts:** Critical thresholds first
4. **📱 Mobile-First:** Design for mobile viewing
5. **🔄 Regular Review:** Update dashboards monthly

### 🌟 USDTgVerse Specific

1. **🌉 Bridge Focus:** Monitor cross-chain health
2. **💰 Financial Metrics:** Track USDTg stability
3. **🌍 Geographic:** Monitor by region (NYC3, SFO2, FRA1)
4. **💼 OdixPay++:** Enterprise payment monitoring

---

**🌌 USDTgVerse Monitoring Excellence**  
*Professional blockchain monitoring with Grafana*
