cmake_minimum_required(VERSION 3.17)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(TestU01 VERSION 1.2.3 LANGUAGES C)

set(PACKAGE_VERSION "${PROJECT_VERSION}")
set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")
configure_file(config.h.in config.h @ONLY)

include_directories(${CMAKE_BINARY_DIR})

add_subdirectory(mylib)
add_subdirectory(probdist)
add_subdirectory(testu01)
add_subdirectory(cli_wrapper)
