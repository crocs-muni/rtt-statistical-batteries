cmake_minimum_required(VERSION 3.10)

project(BSI_BATTERY VERSION 0.1 LANGUAGES CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_executable(bsi_battery app/main.cpp src/bsi.cpp)
target_include_directories(bsi_battery PRIVATE
    ${CMAKE_SOURCE_DIR}/app/include
    ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(bsi_battery PUBLIC
    stdc++fs
)
