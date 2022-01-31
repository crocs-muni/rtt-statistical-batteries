cmake_minimum_required(VERSION 3.17)

project(BSI_BATTERY VERSION 0.1 LANGUAGES CXX)
add_executable(bsi_battery app/main.cpp src/bsi.cpp)

target_include_directories(bsi_battery
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(bsi_battery
    PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 17
)
