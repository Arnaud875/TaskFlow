cmake_minimum_required(VERSION 3.13)
project(TaskFlow LANGUAGES CXX)

# Dooes not build projet if is executed in Windows or MacOS
if (WIN32 OR APPLE)
    message(FATAL_ERROR "This project does not support Windows or MacOS")
endif()

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Do not build tests by default
set(USE_TESTS OFF CACHE BOOL "Enable tests")

# C++ configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SOURCES_FILES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

set(sqlite3_INCLUDE_DIRS /usr/include/sqlite3.h)
set(sqlite3_LIBRARIES /usr/local/lib/libsqlite3.so)

find_package(Threads REQUIRED)
find_package(Crow CONFIG REQUIRED)

find_library(JANSSON_LIBRARIES jansson HINTS /usr/local)
find_library(SQLITE3_LIBRARIES sqlite3 HINTS /usr/local)

add_executable(TaskFlow ${SOURCES_FILES})

target_include_directories(TaskFlow PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(TaskFlow PRIVATE
    Threads::Threads
    Crow::Crow
    ${JANSSON_LIBRARIES}
    ${SQLITE3_LIBRARIES}
)

# Check if the compiler is GNU
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(TaskFlow PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wpedantic
        $<$<CONFIG:Release>:-O3> # Optimization level 3
        $<$<CONFIG:Debug>:-O0 -g> # No optimization and debug symbols
    )
endif()

# Enable tests
if (USE_TESTS)
    include(CTest)

    enable_testing()
    add_subdirectory(tests)
endif()