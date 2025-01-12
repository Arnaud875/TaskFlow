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
set(USE_TESTS ON CACHE BOOL "Enable tests")

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
 "src/Utils/Logger.cpp")

include(compileFlags)

find_package(Threads REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG)
find_package(unofficial-sqlite3 CONFIG)

if (WIN32)
    if (NOT unofficial-sqlite3_FOUND)
        message(FATAL_ERROR "sqlite3 not found")
    endif()

    if (NOT nlohmann_json_FOUND)
        message(FATAL_ERROR "nlohmann_json not found")
    endif()
endif()

add_executable(TaskFlow ${SOURCES_FILES})

target_include_directories(TaskFlow PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(TaskFlow PRIVATE Threads::Threads Crow::Crow)

if (WIN32)
    target_link_libraries(TaskFlow PRIVATE unofficial::sqlite3::sqlite3 nlohmann_json::nlohmann_json)
elseif (UNIX)
    # Check if nlohmann_json was found
    if (NOT nlohmann_json_FOUND)
        # Find jansson library in usr/local folder
        find_library(JANSSON_LIBRARIES jansson HINTS /usr/local)

        target_link_libraries(CMAKE_TASKING_TOOLSET PRIVATE ${JANSSON_LIBRARIES})
    else()
        target_link_libraries(TaskFlow PRIVATE nlohmann_json::nlohmann_json)
    endif()

    # Check if sqlite3 was found
    if (not unofficial-sqlite3_FOUND)
        # Find sqlite3 library in usr/local folder
        set(sqlite3_INCLUDE_DIRS /usr/include/sqlite3.h)
        set(sqlite3_LIBRARIES /usr/local/lib/libsqlite3.so)

        find_library(SQLITE3_LIBRARIES sqlite3 HINTS /usr/local)

        target_link_libraries(TaskFlow PRIVATE ${SQLITE3_LIBRARIES})
    else()
        target_link_libraries(TaskFlow PRIVATE unofficial::sqlite3::sqlite3)
    endif()
endif()

# Precompile Static header
target_precompile_headers(TaskFlow PRIVATE "src/Static.hpp")

if (USE_TESTS)
    add_subdirectory(tests)
endif()