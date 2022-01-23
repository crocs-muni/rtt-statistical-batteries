#pragma once

#include <array>
#include <exception>
#include <iostream>
#include <vector>

namespace bsi {
size_t do_t1(const char *seq) {
  size_t sum{0};
  size_t tmp{0};
  for (size_t i = 0; i < 20000; ++i) {
    tmp += *seq - '0';
    if (tmp < 0 || tmp > 1) {
      throw std::runtime_error("Invalid input sequence");
    }
    ++seq;
  }

  return sum;
}

size_t t1_monobit(const std::vector<char> &input_sequence) {
  std::array<size_t, 257> results{};
  const size_t input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }

  const char *seq = input_sequence.data();
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
  int i{0};
  int number{0};
  double x{0.0};
  std::array<int, 16> numoccur{};
  std::fill(numoccur.begin(), numoccur.end(), 0);

  // 20000 bits, 1 number consists from 4 bits => 5000 iterations
  for (i = 0; i < 5000; i++) {
    number = ((seq[0]) << 3) + ((seq[1]) << 2) + ((seq[2]) << 1) + ((seq[3]) << 0);
    seq += 4;
    numoccur[number]++;
  }

  // compute test value
  for (i = 0; i < 16; i++)
    x += numoccur[i] * numoccur[i];
  x = x * (16.0 / 5000.0) - 5000;

  return x;
}

int t2_poker(const std::vector<char> &input_sequence) {
  int inputLen = 0, i, failed;
  std::array<double, 257> results{};
  const char *seq = input_sequence.data();

  const size_t input_size = input_sequence.size() * 8;
  if (input_size < 5140000) {
    std::cerr << "Not enough bytes" << std::endl;
    return -1;
  }

  std::fill(results.begin(), results.end(), 0.0);

  // do T2 evaluation for 257 sequences, each consists from 20 000 bits (2500 Bytes)
  for (i = 0; i < 257; i++) {
    results[i] = do_t2(seq);
    seq += 20000;
  }

  // evaluation
  failed = 0;
  for (i = 0; i < 257; i++) {
    if ((results[i] < 1.03) || (results[i] > 57.4))
      failed++;
  }

  return 0;
}

} // namespace bsi
