#src cmake
# ==============================================================================
# Core Library (libcfftables)
# ==============================================================================

# Collect source files
set(CORE_SOURCES
    cff.c
    cff_tables.c
    internal_cff_utils.c
)

set(CONSTRUCTION_SOURCES
    constructions/additive.c
    constructions/doubling.c
    constructions/ext_by_one.c
    constructions/finite_fields_wrapper.c
    constructions/fixed_cffs.c
    constructions/id.c
    constructions/kronecker.c
    constructions/optimized_kronecker.c
    constructions/porat_rothschild.c
    constructions/reed_solomon.c
    constructions/short_reed_solomon.c
    constructions/sperner.c
    constructions/sts.c
)

# Create the library
add_library(libcfftables ${CORE_SOURCES} ${CONSTRUCTION_SOURCES})
add_library(libcfftables::libcfftables ALIAS libcfftables)

# Include directories
target_include_directories(libcfftables
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/constructions
)

# Link libraries
target_link_libraries(libcfftables
    PUBLIC PkgConfig::FLINT
    PRIVATE m
)

# Compiler flags
target_compile_options(libcfftables PRIVATE
    $<$<C_COMPILER_ID:GNU,Clang>:-Wall -Wextra>
    $<$<C_COMPILER_ID:GNU,Clang>:-fno-fast-math>
    $<$<C_COMPILER_ID:GNU,Clang>:-fno-unsafe-math-optimizations>
    $<$<C_COMPILER_ID:GNU>:-ffloat-store>
    $<$<C_COMPILER_ID:GNU,Clang>:-frounding-math>
)

# Enable LTO (Link Time Optimization) if supported
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
    set_property(TARGET libcfftables PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Set library version
set_target_properties(libcfftables PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME cfftables  # This creates libcfftables.so on Unix
)

# Set rpath for installed library
set_target_properties(libcfftables PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

# Install the library
install(TARGETS libcfftables
    EXPORT libcfftables-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install CMake config files
install(EXPORT libcfftables-targets
    FILE libcfftables-targets.cmake
    NAMESPACE libcfftables::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libcfftables
)