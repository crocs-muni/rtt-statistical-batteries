add_executable(bis src/main.cpp)

target_include_directories(bis
    PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

set_target_properties(bis
    PROPERTIES
    C_STANDARD 11
    CXX_STANDARD 17
)
