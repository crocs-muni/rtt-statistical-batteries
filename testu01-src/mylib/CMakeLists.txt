add_executable(tcode src/tcode.c)

set(mylib_GEN_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include/mylib" CACHE PATH "Path containing all mylib headers once generated")

file(MAKE_DIRECTORY ${mylib_GEN_INCLUDE_DIR})

set(mylib_SOURCES
  src/addstr.c
  src/bitset.c
  src/chrono.c
  src/gdef.c
  src/mystr.c
  src/num2.c
  src/num.c
  src/tables.c
  src/util.c
)

add_library(mylib STATIC ${mylib_SOURCES})
target_include_directories(mylib PUBLIC ${mylib_GEN_INCLUDE_DIR})
set_target_properties(mylib PROPERTIES POSITION_INDEPENDENT_CODE ON)
if(WIN32)
  target_link_libraries(mylib PUBLIC wsock32)
endif()

set(mylib_GENERATED_HEADERS)
foreach(source_file ${mylib_SOURCES})
  get_filename_component(source_file_we ${source_file} NAME_WE)
  set(tex_file "src/tex/${source_file_we}.tex")
  set(header_file "${mylib_GEN_INCLUDE_DIR}/${source_file_we}.h")
  message(STATUS "Generating header from file: ${tex_file}")
  add_custom_command(
    OUTPUT ${header_file}
    COMMAND "$<TARGET_FILE:tcode>"
    ARGS ${tex_file} ${header_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
  list(APPEND mylib_GENERATED_HEADERS ${header_file})
endforeach()

add_custom_target(mylib_gens DEPENDS ${mylib_GENERATED_HEADERS})
add_dependencies(mylib mylib_gens)

add_library(testu01::mylib ALIAS mylib)
