set(LIBDIEHARDER_SOURCES
  bits.c
  chisq.c
  countx.c
  dab_bytedistrib.c
  dab_dct.c
  dab_filltree.c
  dab_filltree2.c
  dab_monobit2.c
  diehard_2dsphere.c
  diehard_3dsphere.c
  diehard_birthdays.c
  diehard_bitstream.c
  diehard_count_1s_byte.c
  diehard_count_1s_stream.c
  diehard_craps.c
  diehard_dna.c
  diehard_operm5.c
  diehard_opso.c
  diehard_oqso.c
  diehard_parking_lot.c
  diehard_rank_32x32.c
  diehard_rank_6x8.c
  diehard_runs_working.c
  diehard_squeeze.c
  diehard_sums.c
  dieharder_rng_types.c
  dieharder_test_types.c
  histogram.c
  kstest.c
  marsaglia_tsang_gcd.c
  marsaglia_tsang_gorilla.c
  parse.c
  prob.c
  random_seed.c
  rank.c
  rgb_bitdist.c
  rgb_kstest_test.c
  rgb_lagged_sums.c
  # rgb_lmn.c
  rgb_minimum_distance.c
  rgb_operm.c
  rgb_permutations.c
  rgb_persist.c
  rgb_timing.c
  rijndael-alg-fst.c
  rng_XOR.c
  rng_aes.c
  rng_ca.c
  rng_dev_arandom.c
  rng_dev_random.c
  rng_dev_urandom.c
  rng_file_input.c
  rng_file_input_raw.c
  rng_jenkins.c
  rng_kiss.c
  rng_stdin_input_raw.c
  rng_superkiss.c
  rng_threefish.c
  rng_uvag.c
  rngav.c
  rngs_gnu_r.c
  sample.c
  skein_block64.c
  skein_block_ref.c
  std_test.c
  sts_monobit.c
  sts_runs.c
  sts_serial.c
  timing.c
  version.c
  Vtest.c
  Xtest.c
)

add_library(dieharder ${LIBDIEHARDER_SOURCES})
target_link_libraries(dieharder PUBLIC GSL::gsl)

add_library(dieharder::dieharder ALIAS dieharder)

install(TARGETS dieharder RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
