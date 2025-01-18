cmake_minimum_required(VERSION 3.13)
project(TaskFlow LANGUAGES CXX)

# Dooes not build projet if is executed in MacOS
if (APPLE)
    message(FATAL_ERROR "This project does not support MacOS")
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

# Add cmake modules folder
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(SOURCES_FILES
    "src/main.cpp"
    "src/Database/Database.cpp"
    "src/Utils/Logger/Logger.cpp"
)

include(compileFlags)

find_package(Threads REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(TaskFlow ${SOURCES_FILES})

target_include_directories(TaskFlow PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(TaskFlow PRIVATE 
    Threads::Threads
    Crow::Crow
    unofficial::sqlite3::sqlite3
    nlohmann_json::nlohmann_json
)

# Precompile Static header
target_precompile_headers(TaskFlow PRIVATE "src/Static.hpp")

if (USE_TESTS)
    add_subdirectory(tests)
endif()