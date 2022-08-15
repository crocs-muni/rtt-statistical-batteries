set(DIEHARDER_SOURCES
  add_ui_rngs.c
  add_ui_tests.c
  choose_rng.c
  dieharder.c
  dieharder_exit.c
  help.c
  list_rand.c
  list_rngs.c
  list_tests.c
  output.c
  output_rnds.c
  parsecl.c
  rdieharder.c
  rng_empty_random.c
  run_all_tests.c
  run_test.c
  set_globals.c
  testbits.c
  time_rng.c
  user_template.c
)

add_executable(dieharder_battery ${DIEHARDER_SOURCES})
target_link_libraries(dieharder_battery PUBLIC GSL::gsl dieharder::dieharder m)

install(TARGETS dieharder_battery RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
