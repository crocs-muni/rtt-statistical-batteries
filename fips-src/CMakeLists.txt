cmake_minimum_required(VERSION 3.12)

project(fips VERSION 0.1)

add_executable(fips_battery
    src/main.cpp
    src/fips.c
)

set_target_properties(fips_battery
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    C_STANDARD 11
    CXX_STANDARD 17
)

target_include_directories(fips_battery
    PRIVATE
    src/include
)

target_link_libraries(fips_battery
    PUBLIC
    stdc++fs
)
