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
option(USDTGVERSE_BUILD_WALLET_SDK "Build USDTgWallet C++ SDK" ON)
option(USDTGVERSE_USE_ROCKSDB "Use RocksDB for storage" ON)
option(USDTGVERSE_USE_LIBSODIUM "Use libsodium for crypto" ON)
option(USDTGVERSE_USE_SECP256K1 "Use libsecp256k1" ON)
option(USDTGVERSE_ENABLE_METRICS "Enable Prometheus metrics" ON)
option(USDTGVERSE_ENABLE_FUZZING "Enable fuzzing support" OFF)

# ============================================================================
# EXTERNAL DEPENDENCIES
# ============================================================================

# Required system libraries
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

# Optional: RocksDB
if(USDTGVERSE_USE_ROCKSDB)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(ROCKSDB QUIET rocksdb)
    endif()
    
    if(ROCKSDB_FOUND)
        add_compile_definitions(USDTGVERSE_HAS_ROCKSDB)
        message(STATUS "✅ RocksDB found: ${ROCKSDB_VERSION}")
    else()
        message(STATUS "⚠️  RocksDB not found, using built-in storage")
    endif()
endif()

# Optional: libsodium
if(USDTGVERSE_USE_LIBSODIUM)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SODIUM QUIET libsodium)
    endif()
    
    if(SODIUM_FOUND)
        add_compile_definitions(USDTGVERSE_HAS_SODIUM)
        message(STATUS "✅ libsodium found: ${SODIUM_VERSION}")
    else()
        message(STATUS "⚠️  libsodium not found, using built-in crypto")
    endif()
endif()

# Optional: libsecp256k1
if(USDTGVERSE_USE_SECP256K1)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SECP256K1 QUIET libsecp256k1)
    endif()
    
    if(SECP256K1_FOUND)
        add_compile_definitions(USDTGVERSE_HAS_SECP256K1)
        message(STATUS "✅ libsecp256k1 found: ${SECP256K1_VERSION}")
    else()
        message(STATUS "⚠️  libsecp256k1 not found, using built-in implementation")
    endif()
endif()

# ============================================================================
# INCLUDE DIRECTORIES
# ============================================================================

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external
)

# ============================================================================
# SOURCE FILES ORGANIZATION
# ============================================================================

# Common utilities
set(COMMON_SOURCES
    src/common/bytes.cpp
    src/common/varint.cpp
    src/common/endian.cpp
    src/common/span.cpp
    src/common/logging.cpp
    src/common/time.cpp
)

# Cryptography layer
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

# Codec layer (USDTg-Bin)
set(CODEC_SOURCES
    src/codec/encode.cpp
    src/codec/decode.cpp
    src/codec/varint.cpp
    src/codec/deterministic.cpp
)

# Key-Value storage
set(KV_SOURCES
    src/kv/ikv.cpp
    src/kv/mem/memory_kv.cpp
    src/kv/mmap_log/mmap_kv.cpp
)

# RocksDB backend (optional)
if(ROCKSDB_FOUND)
    list(APPEND KV_SOURCES src/kv/rocksdb/rocks_kv.cpp)
endif()

# Merkle tree & SMT
set(MERKLE_SOURCES
    src/merkle/smt.cpp
    src/merkle/merkle_tree.cpp
    src/merkle/proof.cpp
)

# Core blockchain
set(CORE_SOURCES
    src/core/tx.cpp
    src/core/account.cpp
    src/core/block.cpp
    src/core/state.cpp
    src/core/mempool.cpp
    src/core/chain.cpp
)

# Virtual Machine (Phase 2)
set(VM_SOURCES
    src/core/vm/vm.cpp
    src/core/vm/instruction.cpp
    src/core/vm/gas.cpp
)

# Consensus (HotStuff BFT-PoS)
set(CONSENSUS_SOURCES
    src/consensus/hotstuff/engine.cpp
    src/consensus/hotstuff/pacemaker.cpp
    src/consensus/hotstuff/safety.cpp
    src/consensus/hotstuff/liveness.cpp
    src/consensus/messages.cpp
    src/consensus/staking.cpp
    src/consensus/validator.cpp
)

# P2P Networking
set(P2P_SOURCES
    src/p2p/peer.cpp
    src/p2p/wire.cpp
    src/p2p/gossip.cpp
    src/p2p/rpc.cpp
    src/p2p/discovery.cpp
    src/p2p/noise_transport.cpp
)

# Node orchestration
set(NODE_SOURCES
    src/node/config.cpp
    src/node/node.cpp
    src/node/roles/validator.cpp
    src/node/roles/sentry.cpp
    src/node/roles/rpc_node.cpp
)

# OdixPay++ native messages
set(ODIXPAY_SOURCES
    src/core/odixpay/invoice.cpp
    src/core/odixpay/escrow.cpp
    src/core/odixpay/subscription.cpp
    src/core/odixpay/batch_pay.cpp
    src/core/odixpay/spending_limits.cpp
    src/core/odixpay/fee_sponsor.cpp
)

# Tools
set(TOOLS_SOURCES
    src/tools/keytool.cpp
    src/tools/genesis.cpp
    src/tools/cli.cpp
)

# All sources combined
set(ALL_SOURCES
    ${COMMON_SOURCES}
    ${CRYPTO_SOURCES}
    ${CODEC_SOURCES}
    ${KV_SOURCES}
    ${MERKLE_SOURCES}
    ${CORE_SOURCES}
    ${CONSENSUS_SOURCES}
    ${P2P_SOURCES}
    ${NODE_SOURCES}
    ${ODIXPAY_SOURCES}
)

# ============================================================================
# MAIN EXECUTABLE
# ============================================================================

add_executable(usdtgverse
    src/main.c
)

# Link libraries
target_link_libraries(usdtgverse PRIVATE
    Threads::Threads
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Optional libraries
if(ROCKSDB_FOUND)
    target_link_libraries(usdtgverse PRIVATE ${ROCKSDB_LIBRARIES})
    target_include_directories(usdtgverse PRIVATE ${ROCKSDB_INCLUDE_DIRS})
endif()

if(SODIUM_FOUND)
    target_link_libraries(usdtgverse PRIVATE ${SODIUM_LIBRARIES})
    target_include_directories(usdtgverse PRIVATE ${SODIUM_INCLUDE_DIRS})
endif()

if(SECP256K1_FOUND)
    target_link_libraries(usdtgverse PRIVATE ${SECP256K1_LIBRARIES})
    target_include_directories(usdtgverse PRIVATE ${SECP256K1_INCLUDE_DIRS})
endif()

# ============================================================================
# CLI TOOLS
# ============================================================================

if(USDTGVERSE_BUILD_TOOLS)
    # Key management tool
    add_executable(usdtg-keytool
        ${COMMON_SOURCES}
        ${CRYPTO_SOURCES}
        ${CODEC_SOURCES}
        src/tools/keytool.cpp
        src/tools/keytool_main.cpp
    )
    
    target_link_libraries(usdtg-keytool PRIVATE
        Threads::Threads
        OpenSSL::SSL
        OpenSSL::Crypto
    )
    
    # Genesis generator
    add_executable(usdtg-genesis
        ${COMMON_SOURCES}
        ${CRYPTO_SOURCES}
        ${CODEC_SOURCES}
        ${CORE_SOURCES}
        src/tools/genesis.cpp
        src/tools/genesis_main.cpp
    )
    
    target_link_libraries(usdtg-genesis PRIVATE
        Threads::Threads
        OpenSSL::SSL
        OpenSSL::Crypto
    )
endif()

# ============================================================================
# USDTGWALLET C++ SDK
# ============================================================================

if(USDTGVERSE_BUILD_WALLET_SDK)
    add_library(usdtgwallet SHARED
        ${COMMON_SOURCES}
        ${CRYPTO_SOURCES}
        ${CODEC_SOURCES}
        src/wallet/sdk.cpp
        src/wallet/account_manager.cpp
        src/wallet/transaction_builder.cpp
        src/wallet/odixpay_client.cpp
    )
    
    target_link_libraries(usdtgwallet PRIVATE
        Threads::Threads
        OpenSSL::SSL
        OpenSSL::Crypto
    )
    
    # Install SDK
    install(TARGETS usdtgwallet
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
    )
    
    install(DIRECTORY src/wallet/
        DESTINATION include/usdtgwallet
        FILES_MATCHING PATTERN "*.hpp"
    )
endif()

# ============================================================================
# TESTING
# ============================================================================

if(USDTGVERSE_BUILD_TESTS)
    enable_testing()
    
    # Find Google Test
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(GTEST QUIET gtest)
    endif()
    
    if(GTEST_FOUND)
        set(TEST_SOURCES
            tests/test_common.cpp
            tests/test_crypto.cpp
            tests/test_codec.cpp
            tests/test_merkle.cpp
            tests/test_tx.cpp
            tests/test_account.cpp
            tests/test_block.cpp
            tests/test_state.cpp
            tests/test_consensus.cpp
            tests/test_p2p.cpp
            tests/test_odixpay.cpp
            tests/test_integration.cpp
        )
        
        add_executable(usdtgverse_tests
            ${ALL_SOURCES}
            ${TEST_SOURCES}
            tests/test_main.cpp
        )
        
        target_link_libraries(usdtgverse_tests PRIVATE
            ${GTEST_LIBRARIES}
            Threads::Threads
            OpenSSL::SSL
            OpenSSL::Crypto
        )
        
        target_include_directories(usdtgverse_tests PRIVATE ${GTEST_INCLUDE_DIRS})
        target_compile_definitions(usdtgverse_tests PRIVATE TESTING_MODE)
        
        add_test(NAME USDTgVerseTests COMMAND usdtgverse_tests)
        
        message(STATUS "✅ Tests enabled with Google Test")
    else()
        message(STATUS "⚠️  Google Test not found, tests disabled")
    endif()
endif()

# ============================================================================
# FUZZING (LibFuzzer)
# ============================================================================

if(USDTGVERSE_ENABLE_FUZZING AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(FUZZ_SOURCES
        tests/fuzz/fuzz_tx_decode.cpp
        tests/fuzz/fuzz_block_decode.cpp
        tests/fuzz/fuzz_consensus_msg.cpp
        tests/fuzz/fuzz_p2p_msg.cpp
    )
    
    foreach(FUZZ_TARGET ${FUZZ_SOURCES})
        get_filename_component(TARGET_NAME ${FUZZ_TARGET} NAME_WE)
        add_executable(${TARGET_NAME}
            ${ALL_SOURCES}
            ${FUZZ_TARGET}
        )
        
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=fuzzer,address)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=fuzzer,address)
        
        target_link_libraries(${TARGET_NAME} PRIVATE
            Threads::Threads
            OpenSSL::SSL
            OpenSSL::Crypto
        )
    endforeach()
    
    message(STATUS "✅ Fuzzing targets enabled")
endif()

# ============================================================================
# CODE FORMATTING & LINTING
# ============================================================================

# clang-format
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        src/*.cpp src/*.hpp
        tests/*.cpp tests/*.hpp
    )
    
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i ${ALL_CXX_SOURCE_FILES}
        COMMENT "🎨 Formatting code with clang-format"
    )
    
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT} --dry-run --Werror ${ALL_CXX_SOURCE_FILES}
        COMMENT "🔍 Checking code format"
    )
endif()

# clang-tidy
find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY})
    message(STATUS "✅ clang-tidy enabled")
endif()

# ============================================================================
# INSTALLATION
# ============================================================================

# Install main executable
install(TARGETS usdtgverse
    RUNTIME DESTINATION bin
)

# Install tools
if(USDTGVERSE_BUILD_TOOLS)
    install(TARGETS usdtg-keytool usdtg-genesis
        RUNTIME DESTINATION bin
    )
endif()

# Install headers for development
install(DIRECTORY src/
    DESTINATION include/usdtgverse
    FILES_MATCHING PATTERN "*.hpp"
)

# Install CMake config
install(FILES cmake/USDTgVerseConfig.cmake
    DESTINATION lib/cmake/USDTgVerse
)

# ============================================================================
# PACKAGING
# ============================================================================

set(CPACK_PACKAGE_NAME "USDTgVerse")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_DESCRIPTION}")
set(CPACK_PACKAGE_VENDOR "USDTgVerse Team")
set(CPACK_PACKAGE_CONTACT "dev@usdtgverse.com")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB;RPM;TGZ")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "USDTgVerse Team <dev@usdtgverse.com>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "net")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_RPM_PACKAGE_GROUP "Applications/Internet")
    set(CPACK_RPM_PACKAGE_LICENSE "MIT")
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop;TGZ")
endif()

include(CPack)

# ============================================================================
# BUILD SUMMARY
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
message(STATUS "   Wallet SDK: ${USDTGVERSE_BUILD_WALLET_SDK}")
message(STATUS "   Metrics: ${USDTGVERSE_ENABLE_METRICS}")
message(STATUS "   Fuzzing: ${USDTGVERSE_ENABLE_FUZZING}")
message(STATUS "")
message(STATUS "📦 Dependencies:")
if(ROCKSDB_FOUND)
    message(STATUS "   RocksDB: ✅ ${ROCKSDB_VERSION}")
else()
    message(STATUS "   RocksDB: ❌ Using built-in storage")
endif()
if(SODIUM_FOUND)
    message(STATUS "   libsodium: ✅ ${SODIUM_VERSION}")
else()
    message(STATUS "   libsodium: ❌ Using built-in crypto")
endif()
if(SECP256K1_FOUND)
    message(STATUS "   libsecp256k1: ✅ ${SECP256K1_VERSION}")
else()
    message(STATUS "   libsecp256k1: ❌ Using built-in implementation")
endif()
message(STATUS "   OpenSSL: ✅ ${OPENSSL_VERSION}")
message(STATUS "")
message(STATUS "🚀 Ready to build the fastest enterprise blockchain!")
message(STATUS "💎 Native USDTg Token - No Contract Address")
message(STATUS "🎯 1 USD = 1 USDTg - Algorithmic Stability")
message(STATUS "⚡ Pure C++ Performance - Zero Framework Dependencies")
message(STATUS "")
message(STATUS "🌌 =============================================== 🌌")
message(STATUS "")
