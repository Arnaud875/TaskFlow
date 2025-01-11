find_package(Catch2 CONFIG REQUIRED)

enable_testing()

include(CTest)
include(Catch)

# Find all test files
file(GLOB TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/test_*.cpp")

# Function to create a test with Catch2
function(create_test test_file)
    get_filename_component(test_name ${test_file} NAME_WE)
    add_executable(${test_name} ${test_file})

    target_link_libraries(${test_name} PRIVATE
        Catch2::Catch2
        Catch2::Catch2WithMain
    )

    target_include_directories(${test_name} PRIVATE
        ${CMAKE_SOURCE_DIR}/src
    )

    catch_discover_tests(${test_name})
endfunction()

foreach(test_file ${TEST_FILES})
    create_test(${test_file})
endforeach()
