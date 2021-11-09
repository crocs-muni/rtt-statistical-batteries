cmake_minimum_required(VERSION 3.12)

project(fips VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(fips_battery
    src/main.cpp
    src/fips.c
)

set_target_properties(fips_battery
    PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 14
)

target_include_directories(fips_battery
    PRIVATE
    src/include
)
