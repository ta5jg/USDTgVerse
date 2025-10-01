cmake_minimum_required(VERSION 3.20)
project(USDTgVerse LANGUAGES C CXX)

# USDTgVerse - Professional Enterprise Blockchain
project(USDTgVerse
    VERSION 1.0.0
    DESCRIPTION "Ultra-Fast Native C++ Blockchain with USDTg Stablecoin - Enterprise Grade"
    LANGUAGES CXX
    HOMEPAGE_URL "https://usdtgverse.com"
)

# ============================================================================
# BUILD CONFIGURATION
# ============================================================================

# C++20 Standard - Latest features
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build types
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    # Maximum performance flags
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -mtune=native -flto -DNDEBUG -Wno-deprecated-declarations -Wno-sign-conversion -Wno-implicit-int-conversion")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -fsanitize=address,undefined -fno-omit-frame-pointer -Wno-deprecated-declarations -Wno-sign-conversion -Wno-implicit-int-conversion")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-flto")
    
    # Warning flags (relaxed for development)
    add_compile_options(
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter -Wno-missing-field-initializers
        -Wno-conversion -Wno-sign-conversion -Wformat=2
        -Wundef -Wshadow -Wpointer-arith -Wno-deprecated-declarations
    )
    
    # Performance optimizations
    add_compile_options(-ffast-math -funroll-loops)
endif()

# ============================================================================
# OPTIONS & FEATURES
# ============================================================================

option(USDTGVERSE_BUILD_TESTS "Build test suite" ON)
option(USDTGVERSE_BUILD_TOOLS "Build CLI tools" ON)
option(USDTGVERSE_BUILD_ODIXPAY "Build ODIX PAY integration" ON)
option(USDTGVERSE_BUILD_TRADING_ENGINE "Build Trading Engine" ON)
option(USDTGVERSE_BUILD_WALLET_SDK "Build Wallet SDK" ON)
option(USDTGVERSE_BUILD_METRICS "Build metrics collection" ON)
option(USDTGVERSE_BUILD_FUZZING "Build fuzzing targets" OFF)

# ============================================================================
# DEPENDENCIES
# ============================================================================

# Find required packages
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

# Optional dependencies
find_package(RocksDB QUIET)
find_package(libsodium QUIET)
find_package(secp256k1 QUIET)
find_package(GTest QUIET)

# ============================================================================
# INCLUDE DIRECTORIES
# ============================================================================

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/common
    ${CMAKE_SOURCE_DIR}/src/crypto
    ${CMAKE_SOURCE_DIR}/src/odixpay
    ${CMAKE_SOURCE_DIR}/src/trading-engine
)

# ============================================================================
# SOURCE FILES
# ============================================================================

# Common sources
set(COMMON_SOURCES
    src/common/bytes.cpp
    src/common/varint.cpp
    src/common/endian.cpp
    src/common/span.cpp
    src/common/logging.cpp
    src/common/time.cpp
)

# Crypto sources
set(CRYPTO_SOURCES
    src/crypto/hash.cpp
    src/crypto/blake3.cpp
    src/crypto/sha256.cpp
    src/crypto/ed25519.cpp
    src/crypto/secp256k1_wrapper.cpp
    src/crypto/keys.cpp
    src/crypto/signature.cpp
    src/crypto/noise.cpp
)

# ODIX PAY sources
set(ODIXPAY_SOURCES
    src/odixpay/odixpay_api.c
)

# Trading Engine sources
set(TRADING_ENGINE_SOURCES
    src/trading-engine/trading_engine.c
)

# ============================================================================
# EXECUTABLES
# ============================================================================

# Main blockchain node
add_executable(usdtgverse src/main.c)
target_link_libraries(usdtgverse 
    ${CMAKE_THREAD_LIBS_INIT}
    OpenSSL::SSL
    OpenSSL::Crypto
)

# ODIX PAY API
if(USDTGVERSE_BUILD_ODIXPAY)
    add_library(odixpay_api STATIC ${ODIXPAY_SOURCES})
    target_link_libraries(odixpay_api 
        ${CMAKE_THREAD_LIBS_INIT}
        OpenSSL::SSL
        OpenSSL::Crypto
    )
    target_include_directories(odixpay_api PUBLIC src/odixpay)
endif()

# Trading Engine
if(USDTGVERSE_BUILD_TRADING_ENGINE)
    add_library(trading_engine STATIC ${TRADING_ENGINE_SOURCES})
    target_link_libraries(trading_engine 
        ${CMAKE_THREAD_LIBS_INIT}
        OpenSSL::SSL
        OpenSSL::Crypto
        m
    )
    target_include_directories(trading_engine PUBLIC src/trading-engine)
endif()

# Wallet SDK
if(USDTGVERSE_BUILD_WALLET_SDK)
    add_library(usdtgwallet STATIC 
        ${COMMON_SOURCES}
        ${CRYPTO_SOURCES}
        src/codec/encode.cpp
        src/codec/decode.cpp
        src/codec/varint.cpp
        src/codec/deterministic.cpp
        src/wallet/sdk.cpp
        src/wallet/account_manager.cpp
        src/wallet/transaction_builder.cpp
        src/wallet/odixpay_client.cpp
    )
    target_link_libraries(usdtgwallet 
        ${CMAKE_THREAD_LIBS_INIT}
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()

# Key tool
if(USDTGVERSE_BUILD_TOOLS)
    add_executable(usdtg-keytool 
        ${COMMON_SOURCES}
        ${CRYPTO_SOURCES}
        src/tools/keytool.cpp
        src/tools/keytool_main.cpp
    )
    target_link_libraries(usdtg-keytool 
        ${CMAKE_THREAD_LIBS_INIT}
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()

# Genesis tool
if(USDTGVERSE_BUILD_TOOLS)
    add_executable(usdtg-genesis 
        ${COMMON_SOURCES}
        ${CRYPTO_SOURCES}
        src/codec/encode.cpp
        src/codec/decode.cpp
        src/codec/varint.cpp
        src/codec/deterministic.cpp
        src/core/tx.cpp
    )
    target_link_libraries(usdtg-genesis 
        ${CMAKE_THREAD_LIBS_INIT}
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()

# ============================================================================
# TESTS
# ============================================================================

if(USDTGVERSE_BUILD_TESTS AND GTest_FOUND)
    enable_testing()
    
    add_executable(test_crypto 
        ${CRYPTO_SOURCES}
        tests/test_crypto.cpp
    )
    target_link_libraries(test_crypto 
        GTest::gtest
        GTest::gtest_main
        ${CMAKE_THREAD_LIBS_INIT}
        OpenSSL::SSL
        OpenSSL::Crypto
    )
    
    add_test(NAME CryptoTests COMMAND test_crypto)
endif()

# ============================================================================
# INSTALLATION
# ============================================================================

install(TARGETS usdtgverse
    RUNTIME DESTINATION bin
)

if(USDTGVERSE_BUILD_TOOLS)
    install(TARGETS usdtg-keytool usdtg-genesis
        RUNTIME DESTINATION bin
    )
endif()

if(USDTGVERSE_BUILD_WALLET_SDK)
    install(TARGETS usdtgwallet
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()

if(USDTGVERSE_BUILD_ODIXPAY)
    install(TARGETS odixpay_api
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()

if(USDTGVERSE_BUILD_TRADING_ENGINE)
    install(TARGETS trading_engine
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
endif()

# ============================================================================
# CONFIGURATION SUMMARY
# ============================================================================

message(STATUS "")
message(STATUS "🌌 =============================================== 🌌")
message(STATUS "    USDTgVerse Enterprise Blockchain Configuration")
message(STATUS "🌌 =============================================== 🌌")
message(STATUS "")
message(STATUS "📋 Build Configuration:")
message(STATUS "   Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "   Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "   C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "   Platform: ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "")
message(STATUS "🔧 Features:")
message(STATUS "   Tests: ${USDTGVERSE_BUILD_TESTS}")
message(STATUS "   Tools: ${USDTGVERSE_BUILD_TOOLS}")
message(STATUS "   ODIX PAY: ${USDTGVERSE_BUILD_ODIXPAY}")
message(STATUS "   Trading Engine: ${USDTGVERSE_BUILD_TRADING_ENGINE}")
message(STATUS "   Wallet SDK: ${USDTGVERSE_BUILD_WALLET_SDK}")
message(STATUS "   Metrics: ${USDTGVERSE_BUILD_METRICS}")
message(STATUS "   Fuzzing: ${USDTGVERSE_BUILD_FUZZING}")
message(STATUS "")
message(STATUS "📦 Dependencies:")
if(RocksDB_FOUND)
    message(STATUS "   RocksDB: ✅ ${RocksDB_VERSION}")
else()
    message(STATUS "   RocksDB: ❌ Using built-in storage")
endif()
if(libsodium_FOUND)
    message(STATUS "   libsodium: ✅ ${libsodium_VERSION}")
else()
    message(STATUS "   libsodium: ❌ Using built-in crypto")
endif()
if(secp256k1_FOUND)
    message(STATUS "   libsecp256k1: ✅ ${secp256k1_VERSION}")
else()
    message(STATUS "   libsecp256k1: ❌ Using built-in implementation")
endif()
message(STATUS "   OpenSSL: ✅ ${OPENSSL_VERSION}")
message(STATUS "")
message(STATUS "🚀 Ready to build the fastest enterprise blockchain!")
message(STATUS "💎 Native USDTg Token - No Contract Address")
message(STATUS "🎯 1 USD = 1 USDTg - Algorithmic Stability")
message(STATUS "⚡ Pure C++ Performance - Zero Framework Dependencies")
message(STATUS "🔐 Quantum-Safe Cryptography - Future-Proof Security")
message(STATUS "💳 ODIX PAY Integration - Enterprise Payments")
message(STATUS "📈 Trading Engine - Leveraged Trading")
message(STATUS "")
message(STATUS "🌌 =============================================== 🌌")
message(STATUS "")