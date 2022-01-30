#pragma once

#include <algorithm>
#include <array>
#include <climits>
#include <compare>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <vector>

namespace bsi {
namespace utils {
inline uint8_t char_to_bit(const char bit) { return bit - '0'; }
inline size_t chars_to_number(const char *chars, const size_t num_chars) {
  size_t number{0};
  for (size_t i = 0; i < num_chars; ++i) {
    number += char_to_bit(chars[i]) << (num_chars - 1 - i);
  }
  return number;
}
} // namespace utils

/// t0_words
void words_test(const std::vector<char> &input_sequence) {
  auto input_ptr = input_sequence.data();
  constexpr size_t ArraySize{65536};

  auto inputLen = input_sequence.size() / CHAR_BIT;
  if (inputLen < ArraySize * 6) {
    throw std::runtime_error("Invalid input sequence");
  }

  std::vector<size_t> storage(ArraySize, 0);

  size_t failed{0};
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
        failed++;
        break;
      }
    }
  }

  std::cout << "Words failed: " << failed << std::endl;
}

size_t do_monobit_test(const char *seq) {
  size_t sum{0};
  size_t tmp{0};
  for (size_t i = 0; i < 20000; ++i) {
    tmp = utils::char_to_bit(*seq);
    if (tmp < 0 || tmp > 1) {
      throw std::runtime_error("Invalid input sequence");
    }
    sum += tmp;
    ++seq;
  }

  return sum;
}

/// t1_monobit
void monobit_test(const std::vector<char> &input_sequence) {
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

  std::cout << "Monobit failed: " << num_failed << std::endl;
}

double do_poker_test(const char *seq) {
  int number{0};
  double x{0.0};
  std::array<int, 16> numoccur{};
  std::fill(numoccur.begin(), numoccur.end(), 0);

  // 20000 bits, 1 number consists from 4 bits => 5000 iterations
  for (size_t i = 0; i < 5000; i++) {
    number = utils::chars_to_number(seq, 4);
    seq += 4;
    numoccur[number]++;
  }

  for (const auto &number : numoccur) {
    x += std::pow(number, 2);
  }

  x = x * (16.0 / 5000.0) - 5000;

  return x;
}

/// t2_poker
void poker_test(const std::vector<char> &input_sequence) {
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

  std::cout << "Poker failed: " << num_failed << std::endl;
}

} // namespace bsi
