cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 99)

project(DIEHARDER VERSION 3.31.1 LANGUAGES C)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_subdirectory(libdieharder)

add_subdirectory(dieharder)
