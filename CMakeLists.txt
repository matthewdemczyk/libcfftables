#root cmake
cmake_minimum_required(VERSION 3.15)
project(libcfftables VERSION 0.1.4 LANGUAGES C)

include(GNUInstallDirs)
include(CTest)
include(CMakePackageConfigHelpers)

# Options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# -----------------------------
# Find FLINT via pkg-config in Conda
# -----------------------------
find_package(PkgConfig REQUIRED)

# If in a Conda environment, prioritize Conda's pkg-config path
if(DEFINED ENV{CONDA_PREFIX})
    set(ENV{PKG_CONFIG_PATH} "$ENV{CONDA_PREFIX}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")
    message(STATUS "Conda environment detected: $ENV{CONDA_PREFIX}")
    message(STATUS "PKG_CONFIG_PATH: $ENV{PKG_CONFIG_PATH}")
endif()

pkg_check_modules(FLINT REQUIRED IMPORTED_TARGET flint)

# Verify which FLINT was found
if(FLINT_FOUND)
    message(STATUS "FLINT found:")
    message(STATUS "  Version: ${FLINT_VERSION}")
    message(STATUS "  Include dirs: ${FLINT_INCLUDE_DIRS}")
    message(STATUS "  Library dirs: ${FLINT_LIBRARY_DIRS}")
    message(STATUS "  Libraries: ${FLINT_LIBRARIES}")
endif()

# Create alias for backward compatibility
if(NOT TARGET FLINT::FLINT)
    add_library(FLINT::FLINT ALIAS PkgConfig::FLINT)
endif()

# -----------------------------
# Add subdirectories
# -----------------------------

add_subdirectory(src)

# -----------------------------
# Install headers
# -----------------------------

install(DIRECTORY include/libcfftables
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

# -----------------------------
# Generate CMake config files
# -----------------------------

# Generate the config file
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/libcfftables-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/libcfftables-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libcfftables
)

# Generate version file
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/libcfftables-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install config files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libcfftables-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/libcfftables-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libcfftables
)

# -----------------------------
# pkg-config file
# -----------------------------

# Configure pkg-config file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/libcfftables.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/libcfftables.pc
    @ONLY
)

# Install pkg-config file
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libcfftables.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

# -----------------------------
# Testing
# -----------------------------

# Build tests if testing is enabled
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()