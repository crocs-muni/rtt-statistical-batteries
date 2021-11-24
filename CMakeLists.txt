cmake_minimum_required(VERSION 3.12)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(CONAN_PATHS_FILE "${CMAKE_BINARY_DIR}/conan_paths.cmake")

if(NOT EXISTS ${CONAN_PATHS_FILE})
    message(FATAL_ERROR "No such file: ${CONAN_PATHS_FILE}. Please run conan install command inside the build directory!")
endif()
include(${CONAN_PATHS_FILE})

add_subdirectory(fips-src)
