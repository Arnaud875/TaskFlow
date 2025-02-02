find_package(Catch2 CONFIG REQUIRED)

enable_testing()
include(CTest)
include(Catch)

# Get all test files
file(GLOB TEST_FILES CONFIGURE_DEPENDS "test_*.cpp")

# Function to create a test with Catch2
function(create_test test_file)
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name} PRIVATE
        TaskFlow_lib
        Catch2::Catch2
        Catch2::Catch2WithMain
    )

    catch_discover_tests(${test_name})
endfunction()

foreach(test_file ${TEST_FILES})
    create_test(${test_file})
endforeach()
