set(DIEHARDER_SOURCES
  add_ui_rngs.c
  add_ui_tests.c
  choose_rng.c
  configure.ac
  dct-g207g208g14g205-Y2-k2
  dct-g207g208g14g205-n8
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
  run_test.cruft
  set_globals.c
  testbits.c
  time_rng.c
  user_template.c
)

add_executable(dieharder_battery ${DIEHARDER_SOURCES})
target_link_libraries(dieharder_battery PUBLIC dieharder::dieharder m)
