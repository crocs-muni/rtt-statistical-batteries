set(probdist_GEN_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include/probdist" CACHE PATH "Path containing all probdist headers once generated")

file(MAKE_DIRECTORY ${probdist_GEN_INCLUDE_DIR})

set(probdist_SOURCES
  src/fbar.c
  src/fdist.c
  src/finv.c
  src/fmass.c
  src/gofs.c
  src/gofw.c
  src/statcoll.c
  src/wdist.c
)

add_library(probdist STATIC ${probdist_SOURCES})
target_include_directories(probdist PUBLIC ${probdist_GEN_INCLUDE_DIR})
target_link_libraries(probdist PUBLIC testu01::mylib)
set_target_properties(probdist PROPERTIES POSITION_INDEPENDENT_CODE ON)

set(probdist_GENERATED_HEADERS)
foreach(source_file ${probdist_SOURCES})
  get_filename_component(source_file_we ${source_file} NAME_WE)
  set(tex_file "src/tex/${source_file_we}.tex")
  set(header_file "${probdist_GEN_INCLUDE_DIR}/${source_file_we}.h")
  set(output)
  message(STATUS "Generating header from file: ${tex_file}; outputing it to: ${header_file}")
  add_custom_command(
    OUTPUT ${header_file}
    COMMAND "$<TARGET_FILE:tcode>"
    ARGS ${tex_file} ${header_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
  list(APPEND probdist_GENERATED_HEADERS ${header_file})
endforeach()

add_custom_target(probdist_gens DEPENDS ${probdist_GENERATED_HEADERS})
add_dependencies(probdist probdist_gens)

add_library(testu01::probdist ALIAS probdist)
