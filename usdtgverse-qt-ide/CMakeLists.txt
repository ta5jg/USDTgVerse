cmake_minimum_required(VERSION 3.16)
project(USDTgVerseIDE VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable Qt6 features
qt6_standard_project_setup()

# Source files
set(SOURCES
    src/main.cpp
    src/Core/USDTgVerseIDE.cpp
    src/Editor/CodeEditor.cpp
    src/Terminal/Terminal.cpp
    src/AI/AIAssistant.cpp
    src/FileManager/FileExplorer.cpp
    src/UI/MainWindow.cpp
)

# Header files
set(HEADERS
    src/Core/USDTgVerseIDE.h
    src/Editor/CodeEditor.h
    src/Terminal/Terminal.h
    src/AI/AIAssistant.h
    src/FileManager/FileExplorer.h
    src/UI/MainWindow.h
)

# Create executable
qt6_add_executable(USDTgVerseIDE ${SOURCES} ${HEADERS})

# Link Qt6 libraries
target_link_libraries(USDTgVerseIDE
    Qt6::Core
    Qt6::Widgets
)

# Set target properties
set_target_properties(USDTgVerseIDE PROPERTIES
    MACOSX_BUNDLE TRUE
    MACOSX_BUNDLE_BUNDLE_NAME "USDTgVerse IDE"
    MACOSX_BUNDLE_BUNDLE_VERSION "1.0.0"
    MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0.0"
    MACOSX_BUNDLE_IDENTIFIER "com.usdtgverse.ide"
)

# Include directories
target_include_directories(USDTgVerseIDE PRIVATE src)

# Install target
install(TARGETS USDTgVerseIDE
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)
