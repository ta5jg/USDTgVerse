-- 🔍 USDTgVerse Audit System Database Schema
-- ==========================================
-- 
-- ⚛️ QUANTUM-SAFE SMART CONTRACT AUDIT SYSTEM
-- 🔐 ENTERPRISE-GRADE SECURITY ANALYSIS
-- 📊 COMPREHENSIVE VULNERABILITY DETECTION
-- 
-- Database: PostgreSQL 15+
-- Version: 1.0.0
-- Security Level: 256-bit Quantum-Safe

-- 🗄️ DATABASE SETUP
CREATE DATABASE usdtgverse_audit
    WITH 
    ENCODING = 'UTF8'
    LC_COLLATE = 'en_US.UTF-8'
    LC_CTYPE = 'en_US.UTF-8'
    TEMPLATE = template0;

\c usdtgverse_audit;

-- 🔐 ENABLE EXTENSIONS
CREATE EXTENSION IF NOT EXISTS "uuid-ossp";
CREATE EXTENSION IF NOT EXISTS "pgcrypto";
CREATE EXTENSION IF NOT EXISTS "pg_trgm";
CREATE EXTENSION IF NOT EXISTS "btree_gin";

-- 📊 CUSTOM TYPES
CREATE TYPE audit_status AS ENUM (
    'pending',
    'in_progress', 
    'completed',
    'failed',
    'cancelled'
);

CREATE TYPE vulnerability_severity AS ENUM (
    'critical',
    'high',
    'medium',
    'low',
    'info'
);

CREATE TYPE contract_type AS ENUM (
    'erc20',
    'erc721',
    'erc1155',
    'defi',
    'dao',
    'bridge',
    'staking',
    'custom'
);

CREATE TYPE network_type AS ENUM (
    'usdtgverse_mainnet',
    'usdtgverse_testnet',
    'ethereum',
    'bnb_chain',
    'polygon',
    'arbitrum',
    'avalanche',
    'tron',
    'solana'
);

CREATE TYPE audit_type AS ENUM (
    'automated',
    'manual',
    'hybrid',
    'emergency'
);

-- 👥 USERS TABLE
CREATE TABLE users (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    username VARCHAR(50) UNIQUE NOT NULL,
    email VARCHAR(255) UNIQUE NOT NULL,
    password_hash VARCHAR(255) NOT NULL,
    wallet_address VARCHAR(100),
    is_auditor BOOLEAN DEFAULT FALSE,
    is_verified BOOLEAN DEFAULT FALSE,
    reputation_score INTEGER DEFAULT 0,
    total_audits INTEGER DEFAULT 0,
    successful_audits INTEGER DEFAULT 0,
    quantum_safe_verified BOOLEAN DEFAULT FALSE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    last_login TIMESTAMP WITH TIME ZONE,
    
    -- 🔐 Security fields
    two_factor_enabled BOOLEAN DEFAULT FALSE,
    account_locked BOOLEAN DEFAULT FALSE,
    failed_login_attempts INTEGER DEFAULT 0,
    last_failed_login TIMESTAMP WITH TIME ZONE,
    
    -- 📊 Metadata
    metadata JSONB DEFAULT '{}',
    
    CONSTRAINT valid_email CHECK (email ~* '^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$'),
    CONSTRAINT valid_username CHECK (username ~* '^[a-zA-Z0-9_]{3,50}$'),
    CONSTRAINT valid_reputation CHECK (reputation_score >= 0 AND reputation_score <= 10000)
);

-- 📄 CONTRACTS TABLE
CREATE TABLE contracts (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(255) NOT NULL,
    symbol VARCHAR(20),
    contract_address VARCHAR(100),
    deployer_address VARCHAR(100) NOT NULL,
    network network_type NOT NULL,
    contract_type contract_type NOT NULL,
    
    -- 📝 Contract source
    source_code TEXT NOT NULL,
    bytecode TEXT,
    abi JSONB,
    compiler_version VARCHAR(50),
    optimization_enabled BOOLEAN DEFAULT FALSE,
    optimization_runs INTEGER DEFAULT 200,
    
    -- 🔐 Security features
    quantum_safe BOOLEAN DEFAULT FALSE,
    uses_openzeppelin BOOLEAN DEFAULT FALSE,
    has_upgradeable_proxy BOOLEAN DEFAULT FALSE,
    has_multisig BOOLEAN DEFAULT FALSE,
    has_timelock BOOLEAN DEFAULT FALSE,
    
    -- 📊 Contract metrics
    total_supply NUMERIC(78, 0),
    decimals INTEGER,
    lines_of_code INTEGER,
    complexity_score INTEGER,
    gas_usage_estimate BIGINT,
    
    -- 🕒 Timestamps
    deployed_at TIMESTAMP WITH TIME ZONE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    
    -- 📊 Metadata
    metadata JSONB DEFAULT '{}',
    
    CONSTRAINT valid_decimals CHECK (decimals >= 0 AND decimals <= 77),
    CONSTRAINT valid_complexity CHECK (complexity_score >= 0 AND complexity_score <= 10000)
);

-- 🔍 AUDITS TABLE
CREATE TABLE audits (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    contract_id UUID NOT NULL REFERENCES contracts(id) ON DELETE CASCADE,
    auditor_id UUID REFERENCES users(id) ON DELETE SET NULL,
    
    -- 📋 Audit details
    audit_type audit_type NOT NULL DEFAULT 'automated',
    status audit_status NOT NULL DEFAULT 'pending',
    title VARCHAR(255) NOT NULL,
    description TEXT,
    
    -- 📊 Scoring
    security_score INTEGER CHECK (security_score >= 0 AND security_score <= 100),
    gas_efficiency_score INTEGER CHECK (gas_efficiency_score >= 0 AND gas_efficiency_score <= 100),
    code_quality_score INTEGER CHECK (code_quality_score >= 0 AND code_quality_score <= 100),
    overall_score INTEGER CHECK (overall_score >= 0 AND overall_score <= 100),
    
    -- 🔍 Analysis results
    total_issues INTEGER DEFAULT 0,
    critical_issues INTEGER DEFAULT 0,
    high_issues INTEGER DEFAULT 0,
    medium_issues INTEGER DEFAULT 0,
    low_issues INTEGER DEFAULT 0,
    info_issues INTEGER DEFAULT 0,
    
    -- ⏱️ Timing
    estimated_duration INTERVAL,
    actual_duration INTERVAL,
    started_at TIMESTAMP WITH TIME ZONE,
    completed_at TIMESTAMP WITH TIME ZONE,
    
    -- 🕒 Timestamps
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    
    -- 📊 Metadata
    audit_config JSONB DEFAULT '{}',
    results JSONB DEFAULT '{}',
    
    CONSTRAINT valid_scores CHECK (
        (security_score IS NULL OR security_score >= 0) AND
        (gas_efficiency_score IS NULL OR gas_efficiency_score >= 0) AND
        (code_quality_score IS NULL OR code_quality_score >= 0) AND
        (overall_score IS NULL OR overall_score >= 0)
    )
);

-- 🚨 VULNERABILITIES TABLE
CREATE TABLE vulnerabilities (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    audit_id UUID NOT NULL REFERENCES audits(id) ON DELETE CASCADE,
    
    -- 🔍 Vulnerability details
    title VARCHAR(255) NOT NULL,
    description TEXT NOT NULL,
    severity vulnerability_severity NOT NULL,
    category VARCHAR(100) NOT NULL,
    cwe_id INTEGER, -- Common Weakness Enumeration ID
    
    -- 📍 Location
    file_name VARCHAR(255),
    line_number INTEGER,
    column_number INTEGER,
    function_name VARCHAR(255),
    
    -- 📝 Code context
    vulnerable_code TEXT,
    suggested_fix TEXT,
    fix_complexity VARCHAR(50), -- 'easy', 'medium', 'hard'
    
    -- 🔗 References
    references TEXT[], -- Array of reference URLs
    
    -- ✅ Status
    is_confirmed BOOLEAN DEFAULT TRUE,
    is_false_positive BOOLEAN DEFAULT FALSE,
    is_fixed BOOLEAN DEFAULT FALSE,
    fixed_at TIMESTAMP WITH TIME ZONE,
    
    -- 🕒 Timestamps
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    
    -- 📊 Metadata
    metadata JSONB DEFAULT '{}',
    
    CONSTRAINT valid_line_number CHECK (line_number > 0),
    CONSTRAINT valid_column_number CHECK (column_number > 0)
);

-- 📊 AUDIT_REPORTS TABLE
CREATE TABLE audit_reports (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    audit_id UUID NOT NULL REFERENCES audits(id) ON DELETE CASCADE,
    
    -- 📄 Report details
    title VARCHAR(255) NOT NULL,
    executive_summary TEXT,
    detailed_findings TEXT,
    recommendations TEXT,
    
    -- 📊 Report metrics
    total_pages INTEGER,
    word_count INTEGER,
    
    -- 📁 File attachments
    pdf_file_path VARCHAR(500),
    json_report JSONB,
    html_report TEXT,
    
    -- 🔐 Security
    report_hash VARCHAR(64), -- SHA-256 hash for integrity
    digital_signature TEXT,
    
    -- 👥 Review
    reviewed_by UUID REFERENCES users(id),
    reviewed_at TIMESTAMP WITH TIME ZONE,
    is_public BOOLEAN DEFAULT FALSE,
    
    -- 🕒 Timestamps
    generated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    
    -- 📊 Metadata
    metadata JSONB DEFAULT '{}'
);

-- 🏷️ TAGS TABLE
CREATE TABLE tags (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    name VARCHAR(50) UNIQUE NOT NULL,
    description TEXT,
    color VARCHAR(7), -- Hex color code
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    
    CONSTRAINT valid_tag_name CHECK (name ~* '^[a-zA-Z0-9_-]{1,50}$'),
    CONSTRAINT valid_color CHECK (color ~* '^#[0-9A-Fa-f]{6}$')
);

-- 🔗 CONTRACT_TAGS TABLE (Many-to-many relationship)
CREATE TABLE contract_tags (
    contract_id UUID NOT NULL REFERENCES contracts(id) ON DELETE CASCADE,
    tag_id UUID NOT NULL REFERENCES tags(id) ON DELETE CASCADE,
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    
    PRIMARY KEY (contract_id, tag_id)
);

-- 📊 AUDIT_STATISTICS TABLE
CREATE TABLE audit_statistics (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    date DATE NOT NULL,
    
    -- 📈 Daily metrics
    total_audits INTEGER DEFAULT 0,
    completed_audits INTEGER DEFAULT 0,
    pending_audits INTEGER DEFAULT 0,
    failed_audits INTEGER DEFAULT 0,
    
    -- 🚨 Vulnerability metrics
    total_vulnerabilities INTEGER DEFAULT 0,
    critical_vulnerabilities INTEGER DEFAULT 0,
    high_vulnerabilities INTEGER DEFAULT 0,
    medium_vulnerabilities INTEGER DEFAULT 0,
    low_vulnerabilities INTEGER DEFAULT 0,
    
    -- ⏱️ Performance metrics
    average_audit_duration INTERVAL,
    average_security_score DECIMAL(5,2),
    
    -- 🌍 Network distribution
    network_distribution JSONB DEFAULT '{}',
    
    -- 🕒 Timestamps
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    
    UNIQUE(date)
);

-- 🔔 NOTIFICATIONS TABLE
CREATE TABLE notifications (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    
    -- 📨 Notification details
    title VARCHAR(255) NOT NULL,
    message TEXT NOT NULL,
    type VARCHAR(50) NOT NULL, -- 'audit_complete', 'vulnerability_found', etc.
    
    -- 🔗 Related entities
    related_audit_id UUID REFERENCES audits(id) ON DELETE SET NULL,
    related_contract_id UUID REFERENCES contracts(id) ON DELETE SET NULL,
    
    -- 📱 Status
    is_read BOOLEAN DEFAULT FALSE,
    is_email_sent BOOLEAN DEFAULT FALSE,
    
    -- 🕒 Timestamps
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    read_at TIMESTAMP WITH TIME ZONE,
    
    -- 📊 Metadata
    metadata JSONB DEFAULT '{}'
);

-- 🔑 API_KEYS TABLE
CREATE TABLE api_keys (
    id UUID PRIMARY KEY DEFAULT uuid_generate_v4(),
    user_id UUID NOT NULL REFERENCES users(id) ON DELETE CASCADE,
    
    -- 🔑 Key details
    key_name VARCHAR(100) NOT NULL,
    key_hash VARCHAR(255) NOT NULL, -- Hashed API key
    key_prefix VARCHAR(20) NOT NULL, -- First few characters for identification
    
    -- 🔐 Permissions
    permissions JSONB DEFAULT '{}',
    rate_limit INTEGER DEFAULT 1000, -- Requests per hour
    
    -- 📊 Usage tracking
    total_requests BIGINT DEFAULT 0,
    last_used_at TIMESTAMP WITH TIME ZONE,
    
    -- 🔒 Security
    is_active BOOLEAN DEFAULT TRUE,
    expires_at TIMESTAMP WITH TIME ZONE,
    
    -- 🕒 Timestamps
    created_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    updated_at TIMESTAMP WITH TIME ZONE DEFAULT NOW(),
    
    UNIQUE(key_hash)
);

-- 📊 INDEXES FOR PERFORMANCE

-- Users indexes
CREATE INDEX idx_users_email ON users(email);
CREATE INDEX idx_users_username ON users(username);
CREATE INDEX idx_users_wallet_address ON users(wallet_address);
CREATE INDEX idx_users_is_auditor ON users(is_auditor);
CREATE INDEX idx_users_created_at ON users(created_at);

-- Contracts indexes
CREATE INDEX idx_contracts_address ON contracts(contract_address);
CREATE INDEX idx_contracts_deployer ON contracts(deployer_address);
CREATE INDEX idx_contracts_network ON contracts(network);
CREATE INDEX idx_contracts_type ON contracts(contract_type);
CREATE INDEX idx_contracts_quantum_safe ON contracts(quantum_safe);
CREATE INDEX idx_contracts_created_at ON contracts(created_at);
CREATE INDEX idx_contracts_name_trgm ON contracts USING gin(name gin_trgm_ops);

-- Audits indexes
CREATE INDEX idx_audits_contract_id ON audits(contract_id);
CREATE INDEX idx_audits_auditor_id ON audits(auditor_id);
CREATE INDEX idx_audits_status ON audits(status);
CREATE INDEX idx_audits_type ON audits(audit_type);
CREATE INDEX idx_audits_created_at ON audits(created_at);
CREATE INDEX idx_audits_completed_at ON audits(completed_at);
CREATE INDEX idx_audits_overall_score ON audits(overall_score);

-- Vulnerabilities indexes
CREATE INDEX idx_vulnerabilities_audit_id ON vulnerabilities(audit_id);
CREATE INDEX idx_vulnerabilities_severity ON vulnerabilities(severity);
CREATE INDEX idx_vulnerabilities_category ON vulnerabilities(category);
CREATE INDEX idx_vulnerabilities_is_fixed ON vulnerabilities(is_fixed);
CREATE INDEX idx_vulnerabilities_created_at ON vulnerabilities(created_at);

-- Full-text search indexes
CREATE INDEX idx_contracts_source_code_search ON contracts USING gin(to_tsvector('english', source_code));
CREATE INDEX idx_vulnerabilities_description_search ON vulnerabilities USING gin(to_tsvector('english', description));

-- Composite indexes for common queries
CREATE INDEX idx_audits_status_type ON audits(status, audit_type);
CREATE INDEX idx_vulnerabilities_severity_fixed ON vulnerabilities(severity, is_fixed);
CREATE INDEX idx_contracts_network_type ON contracts(network, contract_type);

-- 🔧 FUNCTIONS AND TRIGGERS

-- Function to update updated_at timestamp
CREATE OR REPLACE FUNCTION update_updated_at_column()
RETURNS TRIGGER AS $$
BEGIN
    NEW.updated_at = NOW();
    RETURN NEW;
END;
$$ language 'plpgsql';

-- Apply updated_at triggers to relevant tables
CREATE TRIGGER update_users_updated_at 
    BEFORE UPDATE ON users 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_contracts_updated_at 
    BEFORE UPDATE ON contracts 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_audits_updated_at 
    BEFORE UPDATE ON audits 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_vulnerabilities_updated_at 
    BEFORE UPDATE ON vulnerabilities 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

CREATE TRIGGER update_audit_reports_updated_at 
    BEFORE UPDATE ON audit_reports 
    FOR EACH ROW EXECUTE FUNCTION update_updated_at_column();

-- Function to calculate overall audit score
CREATE OR REPLACE FUNCTION calculate_overall_score(
    p_security_score INTEGER,
    p_gas_efficiency_score INTEGER,
    p_code_quality_score INTEGER
) RETURNS INTEGER AS $$
BEGIN
    -- Weighted average: Security 50%, Gas 25%, Code Quality 25%
    RETURN (
        COALESCE(p_security_score, 0) * 0.5 + 
        COALESCE(p_gas_efficiency_score, 0) * 0.25 + 
        COALESCE(p_code_quality_score, 0) * 0.25
    )::INTEGER;
END;
$$ LANGUAGE plpgsql;

-- Function to update audit statistics
CREATE OR REPLACE FUNCTION update_audit_statistics()
RETURNS TRIGGER AS $$
BEGIN
    INSERT INTO audit_statistics (date, total_audits, completed_audits, pending_audits, failed_audits)
    VALUES (CURRENT_DATE, 0, 0, 0, 0)
    ON CONFLICT (date) DO NOTHING;
    
    -- Update statistics based on audit status changes
    IF TG_OP = 'INSERT' THEN
        UPDATE audit_statistics 
        SET total_audits = total_audits + 1
        WHERE date = CURRENT_DATE;
    ELSIF TG_OP = 'UPDATE' AND OLD.status != NEW.status THEN
        -- Handle status changes
        UPDATE audit_statistics SET
            pending_audits = (SELECT COUNT(*) FROM audits WHERE status = 'pending' AND DATE(created_at) = CURRENT_DATE),
            completed_audits = (SELECT COUNT(*) FROM audits WHERE status = 'completed' AND DATE(completed_at) = CURRENT_DATE),
            failed_audits = (SELECT COUNT(*) FROM audits WHERE status = 'failed' AND DATE(updated_at) = CURRENT_DATE)
        WHERE date = CURRENT_DATE;
    END IF;
    
    RETURN COALESCE(NEW, OLD);
END;
$$ LANGUAGE plpgsql;

-- Apply statistics trigger
CREATE TRIGGER update_audit_statistics_trigger
    AFTER INSERT OR UPDATE ON audits
    FOR EACH ROW EXECUTE FUNCTION update_audit_statistics();

-- 📊 VIEWS FOR COMMON QUERIES

-- Active audits view
CREATE VIEW active_audits AS
SELECT 
    a.id,
    a.title,
    a.status,
    a.audit_type,
    c.name AS contract_name,
    c.contract_address,
    u.username AS auditor_name,
    a.created_at,
    a.estimated_duration,
    a.overall_score
FROM audits a
JOIN contracts c ON a.contract_id = c.id
LEFT JOIN users u ON a.auditor_id = u.id
WHERE a.status IN ('pending', 'in_progress');

-- Vulnerability summary view
CREATE VIEW vulnerability_summary AS
SELECT 
    c.name AS contract_name,
    c.contract_address,
    COUNT(v.id) AS total_vulnerabilities,
    COUNT(CASE WHEN v.severity = 'critical' THEN 1 END) AS critical_count,
    COUNT(CASE WHEN v.severity = 'high' THEN 1 END) AS high_count,
    COUNT(CASE WHEN v.severity = 'medium' THEN 1 END) AS medium_count,
    COUNT(CASE WHEN v.severity = 'low' THEN 1 END) AS low_count,
    COUNT(CASE WHEN v.is_fixed = true THEN 1 END) AS fixed_count
FROM contracts c
JOIN audits a ON c.id = a.contract_id
JOIN vulnerabilities v ON a.id = v.audit_id
GROUP BY c.id, c.name, c.contract_address;

-- Auditor performance view
CREATE VIEW auditor_performance AS
SELECT 
    u.id,
    u.username,
    u.reputation_score,
    COUNT(a.id) AS total_audits,
    COUNT(CASE WHEN a.status = 'completed' THEN 1 END) AS completed_audits,
    AVG(a.overall_score) AS average_score,
    AVG(EXTRACT(EPOCH FROM a.actual_duration)) AS avg_duration_seconds
FROM users u
LEFT JOIN audits a ON u.id = a.auditor_id
WHERE u.is_auditor = true
GROUP BY u.id, u.username, u.reputation_score;

-- 🔐 ROW LEVEL SECURITY (RLS)

-- Enable RLS on sensitive tables
ALTER TABLE users ENABLE ROW LEVEL SECURITY;
ALTER TABLE api_keys ENABLE ROW LEVEL SECURITY;
ALTER TABLE notifications ENABLE ROW LEVEL SECURITY;

-- Users can only see their own data
CREATE POLICY user_own_data ON users
    FOR ALL
    TO authenticated_user
    USING (id = current_setting('app.current_user_id')::UUID);

-- API keys policy
CREATE POLICY api_key_own_data ON api_keys
    FOR ALL
    TO authenticated_user
    USING (user_id = current_setting('app.current_user_id')::UUID);

-- Notifications policy
CREATE POLICY notification_own_data ON notifications
    FOR ALL
    TO authenticated_user
    USING (user_id = current_setting('app.current_user_id')::UUID);

-- 📊 SAMPLE DATA FOR TESTING

-- Insert default tags
INSERT INTO tags (name, description, color) VALUES
('defi', 'Decentralized Finance', '#FF6B35'),
('nft', 'Non-Fungible Token', '#764BA2'),
('dao', 'Decentralized Autonomous Organization', '#667EEA'),
('bridge', 'Cross-chain Bridge', '#F093FB'),
('staking', 'Staking Protocol', '#4FACFE'),
('quantum-safe', 'Quantum-Safe Implementation', '#00D4FF');

-- 📝 COMMENTS FOR DOCUMENTATION

COMMENT ON DATABASE usdtgverse_audit IS 'USDTgVerse Quantum-Safe Smart Contract Audit System Database';

COMMENT ON TABLE users IS 'System users including auditors and contract developers';
COMMENT ON TABLE contracts IS 'Smart contracts submitted for audit';
COMMENT ON TABLE audits IS 'Audit sessions and their results';
COMMENT ON TABLE vulnerabilities IS 'Individual security vulnerabilities found during audits';
COMMENT ON TABLE audit_reports IS 'Generated audit reports in various formats';
COMMENT ON TABLE tags IS 'Tags for categorizing contracts';
COMMENT ON TABLE audit_statistics IS 'Daily aggregated statistics for reporting';
COMMENT ON TABLE notifications IS 'User notifications for audit events';
COMMENT ON TABLE api_keys IS 'API keys for programmatic access';

-- 🎉 SCHEMA CREATION COMPLETE
SELECT 'USDTgVerse Audit System Database Schema Created Successfully! 🎉' AS status;
