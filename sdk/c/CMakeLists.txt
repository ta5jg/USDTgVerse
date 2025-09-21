# 🔧 USDTgVerse C SDK - CMake Configuration
# =========================================
# 
# ⚛️ QUANTUM-SAFE BLOCKCHAIN SDK
# 🌍 CROSS-PLATFORM BUILD SYSTEM
# 🔐 ENTERPRISE-GRADE COMPILATION

cmake_minimum_required(VERSION 3.16)

# 🚀 PROJECT CONFIGURATION
project(USDTgVerse-SDK
    VERSION 1.0.0
    DESCRIPTION "World's First Quantum-Safe Blockchain SDK"
    LANGUAGES C CXX
)

# 📊 BUILD INFORMATION
message(STATUS "🚀 Building USDTgVerse C SDK v${PROJECT_VERSION}")
message(STATUS "⚛️ Quantum Security Level: 256-bit")
message(STATUS "🌍 Target Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "🔧 Compiler: ${CMAKE_C_COMPILER_ID}")

# 🎯 C STANDARD
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 🔧 BUILD TYPE
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "🏗️ Build Type: ${CMAKE_BUILD_TYPE}")

# 📁 DIRECTORY STRUCTURE
set(USDTG_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(USDTG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(USDTG_EXAMPLES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/examples)
set(USDTG_TESTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)

# 📦 FIND REQUIRED PACKAGES
find_package(Threads REQUIRED)

# OpenSSL for cryptographic functions (optional)
find_package(OpenSSL)
if(OPENSSL_FOUND)
    message(STATUS "🔐 OpenSSL Found: ${OPENSSL_VERSION}")
    add_definitions(-DUSDTG_USE_OPENSSL)
else()
    message(STATUS "⚠️ OpenSSL Not Found - Using built-in crypto")
endif()

# cURL for HTTP requests (optional)
find_package(CURL)
if(CURL_FOUND)
    message(STATUS "🌐 cURL Found: ${CURL_VERSION_STRING}")
    add_definitions(-DUSDTG_USE_CURL)
else()
    message(STATUS "⚠️ cURL Not Found - Using basic HTTP client")
endif()

# 🔧 COMPILER FLAGS
set(USDTG_C_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wformat=2
    -Wformat-security
    -Wnull-dereference
    -Wstack-protector
    -Wtrampolines
    -Walloca
    -Wvla
    -Warray-bounds=2
    -Wimplicit-fallthrough=3
    -Wtraditional-conversion
    -Wshift-overflow=2
    -Wcast-qual
    -Wstringop-overflow=4
    -Wlogical-op
    -Wduplicated-cond
    -Wduplicated-branches
    -Wformat-overflow=2
    -Wformat-truncation=2
    -Wstringop-truncation
    -Wcast-align=strict
)

# 🔐 SECURITY FLAGS
set(USDTG_SECURITY_FLAGS
    -fstack-protector-strong
    -fPIE
    -D_FORTIFY_SOURCE=2
    -Wformat
    -Wformat-security
)

# ⚡ OPTIMIZATION FLAGS
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(USDTG_OPTIMIZATION_FLAGS
        -O3
        -DNDEBUG
        -ffunction-sections
        -fdata-sections
        -flto
    )
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(USDTG_OPTIMIZATION_FLAGS
        -O0
        -g3
        -DDEBUG
        -fsanitize=address
        -fsanitize=undefined
    )
endif()

# 🏗️ PLATFORM-SPECIFIC SETTINGS
if(WIN32)
    message(STATUS "🪟 Windows Build Configuration")
    add_definitions(-DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0601)
    set(PLATFORM_LIBS ws2_32 crypt32 advapi32)
    
    # Windows security flags
    list(APPEND USDTG_SECURITY_FLAGS
        -fstack-protector-strong
        -Wformat
        -Wformat-security
    )
    
elseif(APPLE)
    message(STATUS "🍎 macOS Build Configuration")
    set(PLATFORM_LIBS)
    
    # macOS security flags
    list(APPEND USDTG_SECURITY_FLAGS
        -fstack-protector-strong
        -Wformat
        -Wformat-security
    )
    
elseif(UNIX)
    message(STATUS "🐧 Linux Build Configuration")
    set(PLATFORM_LIBS dl m)
    
    # Linux security flags
    list(APPEND USDTG_SECURITY_FLAGS
        -fstack-protector-strong
        -Wformat
        -Wformat-security
        -pie
        -Wl,-z,relro
        -Wl,-z,now
        -Wl,-z,noexecstack
    )
endif()

# 📚 MAIN LIBRARY
add_library(usdtgverse SHARED
    ${USDTG_SOURCE_DIR}/usdtgverse.c
)

# Static library version
add_library(usdtgverse_static STATIC
    ${USDTG_SOURCE_DIR}/usdtgverse.c
)

# Set library properties
set_target_properties(usdtgverse PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER ${USDTG_INCLUDE_DIR}/usdtgverse.h
    OUTPUT_NAME usdtgverse
)

set_target_properties(usdtgverse_static PROPERTIES
    OUTPUT_NAME usdtgverse
)

# 🎯 INCLUDE DIRECTORIES
target_include_directories(usdtgverse PUBLIC
    $<BUILD_INTERFACE:${USDTG_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

target_include_directories(usdtgverse_static PUBLIC
    $<BUILD_INTERFACE:${USDTG_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# 🔗 LINK LIBRARIES
target_link_libraries(usdtgverse PRIVATE
    Threads::Threads
    ${PLATFORM_LIBS}
)

target_link_libraries(usdtgverse_static PRIVATE
    Threads::Threads
    ${PLATFORM_LIBS}
)

# Link OpenSSL if available
if(OPENSSL_FOUND)
    target_link_libraries(usdtgverse PRIVATE OpenSSL::SSL OpenSSL::Crypto)
    target_link_libraries(usdtgverse_static PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()

# Link cURL if available
if(CURL_FOUND)
    target_link_libraries(usdtgverse PRIVATE CURL::libcurl)
    target_link_libraries(usdtgverse_static PRIVATE CURL::libcurl)
endif()

# 🚩 COMPILER FLAGS
target_compile_options(usdtgverse PRIVATE
    ${USDTG_C_FLAGS}
    ${USDTG_SECURITY_FLAGS}
    ${USDTG_OPTIMIZATION_FLAGS}
)

target_compile_options(usdtgverse_static PRIVATE
    ${USDTG_C_FLAGS}
    ${USDTG_SECURITY_FLAGS}
    ${USDTG_OPTIMIZATION_FLAGS}
)

# 🔐 COMPILE DEFINITIONS
target_compile_definitions(usdtgverse PRIVATE
    USDTGVERSE_EXPORTS
    USDTG_VERSION_STRING="${PROJECT_VERSION}"
)

target_compile_definitions(usdtgverse_static PRIVATE
    USDTG_VERSION_STRING="${PROJECT_VERSION}"
)

# 🧪 EXAMPLES (Optional)
option(USDTG_BUILD_EXAMPLES "Build example programs" ON)

if(USDTG_BUILD_EXAMPLES)
    message(STATUS "🧪 Building examples")
    
    # Example programs
    add_executable(wallet_example
        ${USDTG_EXAMPLES_DIR}/wallet_example.c
    )
    
    add_executable(transaction_example
        ${USDTG_EXAMPLES_DIR}/transaction_example.c
    )
    
    add_executable(contract_example
        ${USDTG_EXAMPLES_DIR}/contract_example.c
    )
    
    # Link examples to library
    target_link_libraries(wallet_example PRIVATE usdtgverse)
    target_link_libraries(transaction_example PRIVATE usdtgverse)
    target_link_libraries(contract_example PRIVATE usdtgverse)
    
    # Copy examples to build directory
    set_target_properties(wallet_example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    )
    set_target_properties(transaction_example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    )
    set_target_properties(contract_example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
    )
endif()

# 🧪 TESTS (Optional)
option(USDTG_BUILD_TESTS "Build test suite" OFF)

if(USDTG_BUILD_TESTS)
    message(STATUS "🧪 Building tests")
    enable_testing()
    
    add_executable(usdtg_tests
        ${USDTG_TESTS_DIR}/test_main.c
        ${USDTG_TESTS_DIR}/test_wallet.c
        ${USDTG_TESTS_DIR}/test_crypto.c
        ${USDTG_TESTS_DIR}/test_network.c
    )
    
    target_link_libraries(usdtg_tests PRIVATE usdtgverse_static)
    
    add_test(NAME USDTgVerse_Tests COMMAND usdtg_tests)
endif()

# 📦 INSTALLATION
include(GNUInstallDirs)

install(TARGETS usdtgverse usdtgverse_static
    EXPORT USDTgVerseTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(DIRECTORY ${USDTG_INCLUDE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

# Install CMake config files
install(EXPORT USDTgVerseTargets
    FILE USDTgVerseTargets.cmake
    NAMESPACE USDTgVerse::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/USDTgVerse
)

# Create config file
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/USDTgVerseConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/USDTgVerseConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/USDTgVerse
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/USDTgVerseConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/USDTgVerseConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/USDTgVerseConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/USDTgVerse
)

# 📄 PACKAGE CONFIGURATION
set(CPACK_PACKAGE_NAME "USDTgVerse-SDK")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "World's First Quantum-Safe Blockchain SDK")
set(CPACK_PACKAGE_VENDOR "USDTgVerse Development Team")
set(CPACK_PACKAGE_CONTACT "developers@usdtgverse.com")
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)

# Platform-specific packaging
if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
    set(CPACK_NSIS_DISPLAY_NAME "USDTgVerse SDK")
    set(CPACK_NSIS_PACKAGE_NAME "USDTgVerse SDK")
elseif(APPLE)
    set(CPACK_GENERATOR "ZIP;DragNDrop")
else()
    set(CPACK_GENERATOR "TGZ;DEB;RPM")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "USDTgVerse Team")
    set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
endif()

include(CPack)

# 📊 BUILD SUMMARY
message(STATUS "")
message(STATUS "🎉 USDTgVerse C SDK Configuration Complete")
message(STATUS "==========================================")
message(STATUS "📦 Library: usdtgverse (shared & static)")
message(STATUS "🔧 Version: ${PROJECT_VERSION}")
message(STATUS "⚛️ Quantum Security: 256-bit")
message(STATUS "🌍 Platform: ${CMAKE_SYSTEM_NAME}")
message(STATUS "🏗️ Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "🧪 Examples: ${USDTG_BUILD_EXAMPLES}")
message(STATUS "🧪 Tests: ${USDTG_BUILD_TESTS}")
message(STATUS "🔐 OpenSSL: ${OPENSSL_FOUND}")
message(STATUS "🌐 cURL: ${CURL_FOUND}")
message(STATUS "")
message(STATUS "🚀 Ready to build with: make -j$(nproc)")
message(STATUS "📦 Install with: make install")
message(STATUS "")
