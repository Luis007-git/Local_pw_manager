cmake_minimum_required(VERSION 3.20)
project(PasswordManager)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Find libsodium
find_package(PkgConfig REQUIRED)
pkg_check_modules(SODIUM REQUIRED libsodium)
include_directories(${SODIUM_INCLUDE_DIRS})
link_directories(${SODIUM_LIBRARY_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/PasswordManager.cpp
    src/VaultCrypto.cpp
)

# Main executable
add_executable(password_manager ${SOURCES})
target_link_libraries(password_manager ${SODIUM_LIBRARIES})
