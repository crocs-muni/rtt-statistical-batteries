include(${CMAKE_CURRENT_LIST_DIR}/zig.cmake)

set(CMAKE_C_FLAGS_INIT "-target x86_64-macos-none" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS_INIT "-target x86_64-macos-none" CACHE STRING "" FORCE)
