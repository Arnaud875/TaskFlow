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

set(DEV_MODE OFF CACHE BOOL "Enable dev mode")

# C++ configuration
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add cmake modules folder
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Get all project files
file(GLOB_RECURSE ALL_SOURCE_FILES 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.hpp
)

# Add clang-format target
find_program(CLANG_FORMAT_EXE "clang-format")
if(CLANG_FORMAT_EXE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${ALL_SOURCE_FILES}
        COMMENT "Formatting source code with clang-format"
        VERBATIM
    )
endif()

include(compileFlags)

find_package(Threads REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(TaskFlow ${ALL_SOURCE_FILES})

target_include_directories(TaskFlow PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(TaskFlow PRIVATE 
    Threads::Threads
    Crow::Crow
    unofficial::sqlite3::sqlite3
    nlohmann_json::nlohmann_json
)

target_precompile_headers(TaskFlow PRIVATE "src/Static.hpp")

if (DEV_MODE)
    find_program(CLANG_TIDY_EXE "clang-tidy")
    if (CLANG_TIDY_EXE)
        set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" 
            "-p=${CMAKE_BINARY_DIR}"
            "-extra-arg=-Wno-unknown-warning-option"
            "--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
            "-header-filter=${CMAKE_SOURCE_DIR}/*"        # Modifié pour ne filtrer que les fichiers du projet
            "-extra-arg=-I/usr/include/c++/13"           # Ajout du chemin des headers C++
            "-extra-arg=-I/usr/include"                  # Ajout du chemin des headers système
            "-extra-arg=-I/usr/include/x86_64-linux-gnu" # Ajout du chemin des headers spécifiques à l'architecture
            "-extra-arg=-I/usr/include/x86_64-linux-gnu/c++/13"     # Ajout de ce chemin
            "-extra-arg=-I/usr/lib/gcc/x86_64-linux-gnu/13/include" # Ajout de ce chemin
        )

        add_custom_target(clang 
            DEPENDS TaskFlow
            COMMAND ${CLANG_TIDY_COMMAND} ${ALL_SOURCE_FILES}
            COMMENT ""
        )
    endif()
endif()

if (USE_TESTS)
    add_subdirectory(tests)
endif()