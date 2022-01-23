#pragma once

#include <array>
#include <exception>
#include <iostream>
#include <vector>

namespace bsi {
namespace utils {

inline uint8_t char_to_bit(const char *bit) { return *bit - '0'; }
inline uint8_t hexadecimal_number(const char bits[4]) {
  size_t number{0};
  for (size_t i = 0; i < 4; ++i) {
    number += char_to_bit(&bits[i]) << (3 - i);
  }
  return number;
}

} // namespace utils

size_t do_t1(const char *seq) {
  size_t sum{0};
  size_t tmp{0};
  for (size_t i = 0; i < 20000; ++i) {
    tmp = utils::char_to_bit(seq);
    if (tmp < 0 || tmp > 1) {
      throw std::runtime_error("Invalid input sequence");
    }
    sum += tmp;
    ++seq;
  }

  return sum;
}

size_t t1_monobit(const std::vector<char> &input_sequence) {
  std::array<size_t, 257> results{};
  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }

  auto *seq = input_sequence.data();
  const size_t sequence_stride{20000};
  size_t num_failed{0};
  for (auto &&result : results) {
    result = do_t1(seq);
    if (result < 9654 || result > 10346) {
      ++num_failed;
    }
    seq += sequence_stride;
  }

  return 0;
}

double do_t2(const char *seq) {
  int number{0};
  double x{0.0};
  std::array<int, 16> numoccur{};
  std::fill(numoccur.begin(), numoccur.end(), 0);

  // 20000 bits, 1 number consists from 4 bits => 5000 iterations
  for (size_t i = 0; i < 5000; i++) {
    number = utils::hexadecimal_number(seq);
    seq += 4;
    numoccur[number]++;
  }

  for (const auto &number : numoccur) {
    x += std::pow(number, 2);
  }

  x = x * (16.0 / 5000.0) - 5000;

  return x;
}

int t2_poker(const std::vector<char> &input_sequence) {
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
  // do T2 evaluation for 257 sequences, each consists from 20 000 bits (2500 Bytes)
  for (auto &&result : results) {
    result = do_t2(seq);
    if ((result < 1.03) || (result > 57.4))
      num_failed++;
    seq += sequence_stride;
  }

  // if (num_failed) {
  //   char message[32];
  //   snprintf(message, 32, "Number of failed tests: %lu", num_failed);
  //   throw std::runtime_error(message);
  // }

  return 0;
}

} // namespace bsi
