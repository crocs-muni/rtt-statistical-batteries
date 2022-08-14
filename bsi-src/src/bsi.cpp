#include <algorithm>
#include <array>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iostream>

#include "bsi.hpp"

#include "t1_monobit.hpp"
#include "t2_poker.hpp"
#include "t3_runs.hpp"
#include "t4_long_run.hpp"
#include "t5_autocorrelation.hpp"
#include "t6_uniform_distribution.hpp"
#include "t7_homogenity.hpp"
#include "t8_entropy.hpp"

namespace bsi {

/// t0_words
test_result words_test(const std::vector<unsigned char> &input_sequence) {
  auto input_ptr = input_sequence.data();
  constexpr size_t ArraySize{65536};

  auto inputLen = input_sequence.size();
  if (inputLen < ArraySize * 6) {
    throw std::runtime_error("Invalid input sequence");
  }

  std::vector<size_t> storage(ArraySize, 0);

  size_t num_failed{0};
  std::array<char, 8> temp{};
  std::fill(temp.begin(), temp.end(), 0);
  auto storage_ptr = storage.data();
  const auto tmp_ptr = reinterpret_cast<size_t *>(temp.data());
  const auto num_iterations = inputLen / (ArraySize * 6);
  for (size_t iteration = 0; iteration < num_iterations; iteration++) {
    for (size_t i = 0; i < ArraySize; i++) {
      for (size_t j = 0; j < 6; j++) {
        temp[j] = input_ptr[i * 6 + j];
      }
      storage_ptr[i] = *tmp_ptr;
    }

    std::sort(storage_ptr, storage_ptr + ArraySize, std::less<decltype(storage_ptr[0])>());

    for (size_t i = 0; i < ArraySize - 1; i++) {
      if (storage_ptr[i] == storage_ptr[i + 1]) {
        num_failed++;
        break;
      }
    }
  }

  return test_result{num_failed, num_iterations};
}

/// t1_monobit
test_result monobit_test(const std::vector<unsigned char> &input_sequence) {
  std::array<size_t, 257> results{};
  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }

  auto *seq = input_sequence.data();
  const size_t sequence_stride{20000};
  size_t num_failed{0};
  for (auto &&result : results) {
    result = do_monobit_test(seq);
    if (result < 9654 || result > 10346) {
      ++num_failed;
    }
    seq += sequence_stride;
  }

  return test_result{num_failed, results.size()};
}

/// t2_poker
test_result poker_test(const std::vector<unsigned char> &input_sequence) {
  int inputLen = 0, i, failed;
  std::array<double, 257> results{};
  auto seq = input_sequence.data();

  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }

  std::fill(results.begin(), results.end(), 0.0);
  const size_t sequence_stride{20000};
  size_t num_failed{0};
  // do T2 evaluation for 257 sequences, each consists from 20 000 bits (2500
  // Bytes)
  for (auto &&result : results) {
    result = do_poker_test(seq);
    if ((result < 1.03) || (result > 57.4))
      num_failed++;
    seq += sequence_stride;
  }

  return test_result{num_failed, results.size()};
}

/// t3_runs
test_result runs_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  std::array<char, 257> results{};
  std::fill(results.begin(), results.end(), 0);

  // test of input length, minimal is 5 140 000 bits (642 500 Bytes)
  const auto input_size = input_sequence.size(); // size of input string is in bits!!!
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }
  size_t num_failed{0};
  const size_t sequence_stride{20000};
  for (auto &&result : results) {
    result = do_runs_test(seq);
    if (result > 0) {
      ++num_failed;
    }
    seq += sequence_stride;
  }

  return test_result{num_failed, results.size()};
}

/// t4_long_run
test_result long_run_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  std::array<bool, 257> results{};
  std::fill(results.begin(), results.end(), 0);

  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }

  size_t num_failed{0};
  for (auto &&result : results) {
    result = do_long_run_test(seq);
    if (result) {
      ++num_failed;
    }
    seq += 20000;
  }

  return test_result{num_failed, results.size()};
}

/// t5_autocorellation
test_result autocorrelation_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  std::array<short, 257> results{};
  std::fill(results.begin(), results.end(), 0);
  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }
  srand(time(NULL));
  std::array<data_t, 5000> data{};
  std::fill(data.begin(), data.end(), data_t{0, 0});
  size_t num_failed{0};
  for (auto &&result : results) {
    result = do_autocorrelation_test(seq, data);
    if (result < 2326 || result > 2674) {
      ++num_failed;
    }
    seq += 20000;
  }

  return test_result{num_failed, results.size()};
}

/// t6_uniform_distribution
test_result uniform_test(const std::vector<unsigned char> &input_sequence, const size_t K, const size_t N,
                         const double A) {
  auto seq = input_sequence.data();
  const auto input_size = input_sequence.size();
  if (K * N > input_size) {
    throw std::runtime_error("Required length of the sequence to be tested is too large");
  }
  if (K < 1 || K > 24) {
    throw std::runtime_error("Length of the vectors to be tested must be in [1;24]");
  }
  if (A <= 0 || A >= 1) {
    throw std::runtime_error("Parameter A must be in (0;1)");
  }

  size_t size{1ull << K};
  std::vector<size_t> X(size, 0);

  size_t num_failed{0};
  const auto iterations = input_size / (N * K);
  for (size_t i = 0; i < iterations; i++) {
    std::fill(X.begin(), X.end(), 0);
    if (do_uniform_test(seq, X.data(), K, N, A) > 0) {
      ++num_failed;
    }
    seq += K * N;
  }

  return test_result{num_failed, iterations};
}

/// t7_homogenity
test_result homogenity_test(const std::vector<unsigned char> &input_sequence) {
  constexpr size_t MIN_K{1};
  constexpr size_t MAX_K{3};
  auto seq = input_sequence.data();
  int failed, end;
  unsigned long long **tabTF;
  long ret{0};
  double result{0.0};
  auto input_size = input_sequence.size();
  size_t failedTotal{0};
  size_t iterations{0};
  end = 0;
  while (!end) {
    iterations++;
    failed = 0;
    for (size_t i = MIN_K; i <= MAX_K; i++) {
      tabTF = createTF(i);
      if (!tabTF) {
        throw std::runtime_error("Insufficient memory");
      }
      ret = TF(seq, input_size, tabTF, i, 100000);
      if (ret == -1) {
        destroyTF(tabTF, i);
        if (iterations == 1) {
          throw std::runtime_error("Bad input data");
        }
        end = 1;
        iterations--;
        break;
      }
      if (i == 1) {
        result = (tabTF[0][1] + tabTF[1][0]) / 100000.0 - 1;
        if (result < 0)
          result = -result;
        if (result >= 0.02) {
          failedTotal++;
        }
      } else {
        failed = doEvaluationT7(tabTF, i, 15.13);
        if (failed > 0) {
          failedTotal += failed;
        }
      }
      seq += input_size - ret;
      input_size = ret;
      destroyTF(tabTF, i);
    }
  }

  return test_result{failedTotal, iterations};
}

/// t8_entropy
test_result entropy_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  auto input_size = input_sequence.size();
  int ret{0};

  if (input_size < 8 * (2560 + 256000)) {
    throw std::runtime_error("Invalid size of input sequence");
  }
  size_t num_iterations{0};
  size_t num_failed{0};
  while (input_size >= 8 * (2560 + 256000)) {
    ret = do_entropy_test(seq, 8, 2560, 256000);
    if (ret == -1) {
      throw std::runtime_error("Insufficient memory");
    }
    num_iterations++;
    if (ret == 1) {
      num_failed++;
    }
    input_size -= 8 * (2560 + 256000);
    seq += 8 * (2560 + 256000);
  }

  return test_result{num_failed, num_iterations};
}

} // namespace bsi
