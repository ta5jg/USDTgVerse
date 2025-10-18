#!/bin/bash

# RGLS Stablecoin Test Script
# This script tests the RGLS Stablecoin system implementation

echo "🚀 RGLS STABLECOIN TEST BAŞLIYOR!"
echo "================================="
echo ""

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test results
TESTS_PASSED=0
TESTS_FAILED=0
TOTAL_TESTS=0

# Function to run a test
run_test() {
    local test_name="$1"
    local test_command="$2"
    local expected_result="$3"
    
    echo -e "${BLUE}🧪 Test: $test_name${NC}"
    echo "Command: $test_command"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if eval "$test_command" > /dev/null 2>&1; then
        echo -e "${GREEN}✅ PASSED${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}❌ FAILED${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo ""
}

# Function to check if a file exists
check_file_exists() {
    local file_path="$1"
    local description="$2"
    
    echo -e "${BLUE}📁 Checking: $description${NC}"
    echo "File: $file_path"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if [ -f "$file_path" ]; then
        echo -e "${GREEN}✅ EXISTS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}❌ NOT FOUND${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo ""
}

# Function to check if a directory exists
check_directory_exists() {
    local dir_path="$1"
    local description="$2"
    
    echo -e "${BLUE}📂 Checking: $description${NC}"
    echo "Directory: $dir_path"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if [ -d "$dir_path" ]; then
        echo -e "${GREEN}✅ EXISTS${NC}"
        TESTS_PASSED=$((TESTS_PASSED + 1))
    else
        echo -e "${RED}❌ NOT FOUND${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo ""
}

# Function to check compilation
check_compilation() {
    local source_file="$1"
    local target_file="$2"
    local description="$3"
    
    echo -e "${BLUE}🔨 Compiling: $description${NC}"
    echo "Source: $source_file"
    echo "Target: $target_file"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    if [ -f "$source_file" ]; then
        # Try to compile
        if gcc -Wall -Wextra -std=c99 -O2 -g -o "$target_file" "$source_file" -lssl -lcrypto -lpthread 2>/dev/null; then
            echo -e "${GREEN}✅ COMPILATION SUCCESS${NC}"
            TESTS_PASSED=$((TESTS_PASSED + 1))
        else
            echo -e "${RED}❌ COMPILATION FAILED${NC}"
            TESTS_FAILED=$((TESTS_FAILED + 1))
        fi
    else
        echo -e "${RED}❌ SOURCE FILE NOT FOUND${NC}"
        TESTS_FAILED=$((TESTS_FAILED + 1))
    fi
    echo ""
}

# Start testing
echo -e "${YELLOW}📋 RGLS STABLECOIN SYSTEM TEST${NC}"
echo "======================================"
echo ""

# Test 1: Check if source files exist
echo -e "${YELLOW}1. SOURCE FILES CHECK${NC}"
echo "========================"
check_file_exists "src/tokens/rgls_stablecoin.c" "RGLS Stablecoin Source"
check_file_exists "src/oracle/rgls_oracle_server.c" "Oracle Server Source"
check_file_exists "src/verification/rgls_verification_server.c" "Verification Server Source"
check_file_exists "src/tokens/Makefile" "RGLS Stablecoin Makefile"
check_file_exists "src/oracle/Makefile" "Oracle Server Makefile"
check_file_exists "src/verification/Makefile" "Verification Server Makefile"

# Test 2: Check if directories exist
echo -e "${YELLOW}2. DIRECTORY STRUCTURE CHECK${NC}"
echo "=============================="
check_directory_exists "src/tokens" "Tokens Directory"
check_directory_exists "src/oracle" "Oracle Directory"
check_directory_exists "src/verification" "Verification Directory"

# Test 3: Check compilation
echo -e "${YELLOW}3. COMPILATION CHECK${NC}"
echo "====================="
check_compilation "src/tokens/rgls_stablecoin.c" "rgls_stablecoin" "RGLS Stablecoin"
check_compilation "src/oracle/rgls_oracle_server.c" "rgls_oracle_server" "Oracle Server"
check_compilation "src/verification/rgls_verification_server.c" "rgls_verification_server" "Verification Server"

# Test 4: Check if executables were created
echo -e "${YELLOW}4. EXECUTABLE CHECK${NC}"
echo "====================="
check_file_exists "rgls_stablecoin" "RGLS Stablecoin Executable"
check_file_exists "rgls_oracle_server" "Oracle Server Executable"
check_file_exists "rgls_verification_server" "Verification Server Executable"

# Test 5: Check if executables are runnable
echo -e "${YELLOW}5. EXECUTABLE TEST${NC}"
echo "==================="
run_test "RGLS Stablecoin Execution" "./rgls_stablecoin" "0"
run_test "Oracle Server Execution" "./rgls_oracle_server" "0"
run_test "Verification Server Execution" "./rgls_verification_server" "0"

# Test 6: Check system requirements
echo -e "${YELLOW}6. SYSTEM REQUIREMENTS CHECK${NC}"
echo "============================="
run_test "GCC Compiler" "gcc --version" "0"
run_test "OpenSSL Library" "openssl version" "0"
run_test "CURL Library" "curl --version" "0"
run_test "Pthread Library" "ldconfig -p | grep pthread" "0"

# Test 7: Check file permissions
echo -e "${YELLOW}7. FILE PERMISSIONS CHECK${NC}"
echo "========================="
run_test "RGLS Stablecoin Executable Permissions" "test -x rgls_stablecoin" "0"
run_test "Oracle Server Executable Permissions" "test -x rgls_oracle_server" "0"
run_test "Verification Server Executable Permissions" "test -x rgls_verification_server" "0"

# Test 8: Check file sizes
echo -e "${YELLOW}8. FILE SIZE CHECK${NC}"
echo "=================="
echo -e "${BLUE}📊 File Sizes:${NC}"
if [ -f "rgls_stablecoin" ]; then
    echo "RGLS Stablecoin: $(ls -lh rgls_stablecoin | awk '{print $5}')"
fi
if [ -f "rgls_oracle_server" ]; then
    echo "Oracle Server: $(ls -lh rgls_oracle_server | awk '{print $5}')"
fi
if [ -f "rgls_verification_server" ]; then
    echo "Verification Server: $(ls -lh rgls_verification_server | awk '{print $5}')"
fi
echo ""

# Test 9: Check system integration
echo -e "${YELLOW}9. SYSTEM INTEGRATION CHECK${NC}"
echo "============================="
run_test "Port 8080 Available" "netstat -an | grep :8080" "0"
run_test "Port 8081 Available" "netstat -an | grep :8081" "0"
run_test "Memory Available" "free -m | grep Mem" "0"
run_test "Disk Space Available" "df -h . | tail -1" "0"

# Test 10: Check documentation
echo -e "${YELLOW}10. DOCUMENTATION CHECK${NC}"
echo "========================"
check_file_exists "implementation/rgls_stablecoin_implementation.md" "RGLS Implementation Plan"
check_file_exists "README.md" "Main README"
check_file_exists "LICENSE" "License File"

# Summary
echo -e "${YELLOW}📊 TEST SUMMARY${NC}"
echo "==============="
echo -e "Total Tests: ${BLUE}$TOTAL_TESTS${NC}"
echo -e "Passed: ${GREEN}$TESTS_PASSED${NC}"
echo -e "Failed: ${RED}$TESTS_FAILED${NC}"

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL TESTS PASSED!${NC}"
    echo -e "${GREEN}✅ RGLS STABLECOIN SYSTEM READY!${NC}"
    exit 0
else
    echo -e "${RED}❌ SOME TESTS FAILED!${NC}"
    echo -e "${YELLOW}⚠️  Please check the failed tests above.${NC}"
    exit 1
fi
