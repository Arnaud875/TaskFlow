cmake_minimum_required(VERSION 3.20)
project(TaskFlow LANGUAGES CXX)

# Dooes not build projet if is executed in MacOS
if (APPLE)
    message(FATAL_ERROR "This project does not support MacOS")
endif()

# Set build type to Release if not set
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Do not build tests by default
set(USE_TESTS OFF CACHE BOOL "Enable tests")

# Activate dev function
set(DEV_MODE OFF CACHE BOOL "Enable dev mode")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

# Set database file name and schema file source
set(DATABASE_FILE_NAME "task_flow.db")
set(DATABASE_SCHEMA_FILE_NAME "${CMAKE_SOURCE_DIR}/data/database.sql")

file(GLOB_RECURSE ALL_SOURCE_FILES 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.hpp
)

# Remove main.cpp from the list of source files
list(FILTER ALL_SOURCE_FILES EXCLUDE REGEX ".*main.cpp$")

include(FetchContent)
include(compileFlags)
include(bcrypt)

find_package(Threads REQUIRED)
find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

# Créer une bibliothèque avec les sources
add_library(TaskFlow_lib STATIC ${ALL_SOURCE_FILES})

# Configurer la bibliothèque
target_include_directories(TaskFlow_lib PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(TaskFlow_lib PUBLIC 
    Threads::Threads
    Crow::Crow
    unofficial::sqlite3::sqlite3
    nlohmann_json::nlohmann_json
    bcrypt
)

# Ajouter les définitions à la bibliothèque
target_compile_definitions(TaskFlow_lib PUBLIC 
    DATABASE_FILE_NAME="${DATABASE_FILE_NAME}"
    DATABASE_SCHEMA_FILE_NAME="${DATABASE_SCHEMA_FILE_NAME}"
    DEV_MODE="${DEV_MODE}"
)

target_precompile_headers(TaskFlow_lib PUBLIC "src/Static.hpp")

# Modifier l'exécutable pour utiliser la bibliothèque
add_executable(TaskFlow src/main.cpp)
target_link_libraries(TaskFlow PRIVATE TaskFlow_lib)

if (DEV_MODE)
    find_program(CLANG_TIDY_EXE "clang-tidy")

    # Add clang target
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

    # Add clang-format target
    find_program(CLANG_FORMAT_EXE "clang-format")
    if(CLANG_FORMAT_EXE)
        add_custom_target(format
            COMMAND ${CLANG_FORMAT_EXE} -i -style=file ${ALL_SOURCE_FILES}
            COMMENT "Formatting source code with clang-format"
            VERBATIM
        )
    endif()
endif()

if (USE_TESTS)
    # Add tests targets
    add_subdirectory(tests)
endif()