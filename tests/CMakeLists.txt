# tests/CMakeLists.txt

# Collect all .c files in this directory
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/construction_tests/*.c"
)
# Make a test output folder
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_output)

foreach(test_src ${TEST_SOURCES})
    # Get filename without extension (e.g. test_cff.c -> test_cff)
    get_filename_component(test_name ${test_src} NAME_WE)

    # Create executable
    add_executable(${test_name} ${test_src})

    # Link against libcfftables
    target_link_libraries(${test_name} PRIVATE libcfftables::libcfftables)

    # Register test with CTest
    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()