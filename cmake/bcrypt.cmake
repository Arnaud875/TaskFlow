# Get bcrypt library from Github
FetchContent_Declare(
    bcrypt
    GIT_REPOSITORY https://github.com/hilch/Bcrypt.cpp
    GIT_TAG master
)

FetchContent_MakeAvailable(bcrypt)

if(NOT bcrypt_POPULATED)  
    add_library(bcrypt STATIC 
        ${bcrypt_SOURCE_DIR}/src/bcrypt.cpp
        ${bcrypt_SOURCE_DIR}/src/blowfish.cpp
    )
    
    target_include_directories(bcrypt
        PUBLIC 
            $<BUILD_INTERFACE:${bcrypt_SOURCE_DIR}/include>
    )
endif()