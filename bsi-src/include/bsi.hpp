#pragma once
#include <cstddef>
#include <string_view>
#include <vector>

namespace bsi {

inline constexpr std::string_view WordsName{"T0 - Words"};
inline constexpr std::string_view MonobitName{"T1 - Monobit"};
inline constexpr std::string_view PokerName{"T2 - Poker"};
inline constexpr std::string_view RunsName{"T3 - Runs"};
inline constexpr std::string_view LongRunName{"T4 - Long run"};
inline constexpr std::string_view AutocorrelationName{"T5 - Autocorrelation"};
inline constexpr std::string_view UniformDistributionName{"T6 - Uniform Distribution"};
inline constexpr std::string_view HomogenityName{"T7 - Homogenity"};
inline constexpr std::string_view EntropyName{"T8 - Entropy"};

/// test result
typedef struct {
  size_t num_failed;
  size_t num_iterations;
} test_result;

/// t0_words
test_result words_test(const std::vector<unsigned char> &input_sequence);

/// t1_monobit
test_result monobit_test(const std::vector<unsigned char> &input_sequence);

/// t2_poker
test_result poker_test(const std::vector<unsigned char> &input_sequence);

/// t3_runs
test_result runs_test(const std::vector<unsigned char> &input_sequence);

/// t4_long_run
test_result long_run_test(const std::vector<unsigned char> &input_sequence);

/// t5_autocorellation
test_result autocorrelation_test(const std::vector<unsigned char> &input_sequence);

/// t6_uniform_distribution
test_result uniform_test(const std::vector<unsigned char> &input_sequence, const size_t K, const size_t N,
                         const double A);

/// t7_homogenity
test_result homogenity_test(const std::vector<unsigned char> &input_sequence);

/// t8_entropy
test_result entropy_test(const std::vector<unsigned char> &input_sequence);

} // namespace bsi
