set(testu01_GEN_INCLUDE_DIR "${CMAKE_BINARY_DIR}/include/testu01" CACHE PATH "Path containing all testu01 headers once generated")

file(MAKE_DIRECTORY ${testu01_GEN_INCLUDE_DIR})

set(testu01_SOURCES
  src/bbattery.c
  src/fcho.c
  src/fcong.c
  src/ffam.c
  src/ffsr.c
  src/fftc.c
  src/fknuth.c
  src/fmarsa.c
  src/fmultin.c
  src/fnpair.c
  src/fres.c
  src/fspectral.c
  src/fstring.c
  src/ftab.c
  src/fvaria.c
  src/fwalk.c
  src/rijndael-alg-fst.c
  src/scatter.c
  src/scomp.c
  src/sentrop.c
  src/sknuth.c
  src/smarsa.c
  src/smultin.c
  src/snpair.c
  src/sres.c
  src/sspacings.c
  src/sspectral.c
  src/sstring.c
  src/svaria.c
  src/swalk.c
  src/swrite.c
  src/tu01_sha1.c
  src/uautomata.c
  src/ubrent.c
  src/ucarry.c
  src/ucrypto.c
  src/ucryptoIS.c
  src/ucubic.c
  src/udeng.c
  src/ufile.c
  src/ugfsr.c
  src/ugranger.c
  src/uinv.c
  src/uknuth.c
  src/ulcg.c
  src/ulec.c
  src/umarsa.c
  src/umrg.c
  src/unif01.c
  src/unumrec.c
  src/uquad.c
  src/usoft.c
  src/utaus.c
  src/utezuka.c
  src/utouzin.c
  src/uvaria.c
  src/uweyl.c
  src/uwu.c
  src/uxorshift.c
  src/vectorsF2.c
)

add_library(testu01 ${testu01_SOURCES})
target_include_directories(testu01
  PUBLIC
  $<BUILD_INTERFACE:${testu01_GEN_INCLUDE_DIR}>
  $<INSTALL_INTERFACE:include>
  $<INSTALL_INTERFACE:include/TestU01>
)
target_link_libraries(testu01 PUBLIC testu01::mylib testu01::probdist m)

set(testu01_GENERATED_HEADERS)

foreach(source_file ${testu01_SOURCES})
  get_filename_component(source_file_we ${source_file} NAME_WE)
  set(tex_file "src/tex/${source_file_we}.tex")
  set(header_file "${testu01_GEN_INCLUDE_DIR}/${source_file_we}.h")
  set(output)
  message(STATUS "Generating header from file: ${tex_file}; outputing it to: ${header_file}")
  add_custom_command(
    OUTPUT ${header_file}
    COMMAND "$<TARGET_FILE:tcode>"
    ARGS ${tex_file} ${header_file}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  )
  list(APPEND testu01_GENERATED_HEADERS ${header_file})
endforeach()

add_custom_target(testu01_gens DEPENDS ${testu01_GENERATED_HEADERS})
add_dependencies(testu01 testu01_gens)

set_target_properties(testu01 PROPERTIES
  VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
  SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
  PUBLIC_HEADER include/TestU01.h
  POSITION_INDEPENDENT_CODE ON
)

add_library(testu01::testu01 ALIAS testu01)

install(TARGETS testu01
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  PUBLIC_HEADER DESTINATION include/TestU01
)
install(FILES ${testu01_GENERATED_HEADERS} DESTINATION include/TestU01)
