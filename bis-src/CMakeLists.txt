cmake_minimum_required(VERSION 3.17)

project(BIS_STS VERSION 0.1 LANGUAGES CXX)
add_executable(bis app/main.cpp src/bsi.cpp)

target_include_directories(bis
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(bis
    PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 17
)
