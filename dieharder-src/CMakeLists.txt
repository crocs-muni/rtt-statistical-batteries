cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)

project(DIEHARDER VERSION 3.31.1 LANGUAGES C)

# it's easier to detect endianness via CMake
include(TestBigEndian)
test_big_endian(IS_BIG_ENDIAN)
set(ENDIAN_HELPER_DEFINITION)
if(IS_BIG_ENDIAN)
  set(ENDIAN_HELPER_DEFINITION "BIG_ENDIAN")
else()
  set(ENDIAN_HELPER_DEFINITION "LITTLE_ENDIAN")
endif()

add_compile_definitions(${ENDIAN_HELPER_DEFINITION})


include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_BINARY_DIR}")

configure_file(config.h.in config.h @ONLY)

find_package(GSL REQUIRED)

add_subdirectory(libdieharder)

add_subdirectory(dieharder)
