cmake_minimum_required(VERSION 3.17)

# tell CMake to search for Find<Package>.cmake files also in build directory
# needed for conan integration
list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")

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

if(WIN32)
  if(MINGW)
    add_link_options("-Wl,--allow-multiple-definition")
  else()
    message(FATAL_ERROR "${CMAKE_C_COMPILER_ID} is not supported on Windows, please, use only MinGW GCC.")
  endif()
endif()

include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_BINARY_DIR}")

configure_file(config.h.in config.h @ONLY)

find_package(GSL REQUIRED)

add_subdirectory(libdieharder)

add_subdirectory(dieharder)

install(FILES
  AUTHORS
  ChangeLog
  dieharder/COPYING
  README
  dieharder/NOTES DESTINATION .)
