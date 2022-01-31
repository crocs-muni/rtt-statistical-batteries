#pragma once
#include <cstddef>
#include <vector>

namespace bsi {

/// t0_words
void words_test(const std::vector<unsigned char> &input_sequence);

/// t1_monobit
void monobit_test(const std::vector<unsigned char> &input_sequence);

/// t2_poker
void poker_test(const std::vector<unsigned char> &input_sequence);

/// t3_runs
void runs_test(const std::vector<unsigned char> &input_sequence);

/// t4_long_run
void long_run_test(const std::vector<unsigned char> &input_sequence);

/// t5_autocorellation
void autocorrelation_test(const std::vector<unsigned char> &input_sequence);

/// t6_uniform_distribution
void uniform_test(const std::vector<unsigned char> &input_sequence, const size_t K, const size_t N, const double A);

/// t7_homogenity
void homogenity_test(const std::vector<unsigned char> &input_sequence);

/// t8_entropy
void entropy_test(const std::vector<unsigned char> &input_sequence);

} // namespace bsi
