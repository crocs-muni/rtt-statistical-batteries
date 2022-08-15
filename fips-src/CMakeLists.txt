cmake_minimum_required(VERSION 3.10)

project(FIPS_BATTERY VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

find_package(nlohmann_json REQUIRED)

add_executable(fips_battery app/main.cpp src/fips.c)
target_include_directories(fips_battery PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/app/include
)
target_link_libraries(fips_battery PUBLIC
    nlohmann_json::nlohmann_json
)

install(TARGETS fips_battery RUNTIME DESTINATION bin)
