add_executable(testu01_cli
  src/TestU01.c
  src/FileStreamGen.c
  src/TestU01Settings.c
  src/Utils.c
)
target_include_directories(testu01_cli PRIVATE include)
target_link_libraries(testu01_cli PRIVATE testu01::testu01)
set_target_properties(testu01_cli PROPERTIES
  INSTALL_RPATH $ORIGIN/../lib
)

install(TARGETS testu01_cli RUNTIME DESTINATION bin)
