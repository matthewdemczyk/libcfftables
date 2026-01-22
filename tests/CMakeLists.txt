#tests cmake
# Create test executables
add_executable(test_cff test_cff.c)
add_executable(test_cff_direct_constructions test_cff_direct_constructions.c)

# Link against the cfftables target (not installed library)
target_link_libraries(test_cff PRIVATE libcfftables)
target_link_libraries(test_cff_direct_constructions PRIVATE libcfftables)

# Register test with CTest
add_test(NAME test_cff COMMAND test_cff)
add_test(NAME test_cff_direct_constructions COMMAND test_cff_direct_constructions)