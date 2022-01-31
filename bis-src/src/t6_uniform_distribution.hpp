#pragma once
#include <cmath>
#include <cstddef>
namespace bsi {

int do_uniform_test(const unsigned char *seq, size_t *X, const size_t K, const size_t N, const double A) {
  size_t w{0};
  for (size_t i = 0; i < N; i++) {
    w = 0;
    for (size_t j = 0; j < K; j++) {
      w <<= 1;
      w |= *seq++;
    }
    X[w]++;
  }
  const auto power = std::pow(2.0, -K);
  const auto low = power - A;
  const auto high = power + A;

  size_t failed{0};
  double ratio{0.0};
  for (size_t i = 0; i < (1ull << K); i++) {
    ratio = static_cast<double>(X[i]) / static_cast<double>(N);
    if (ratio < low || ratio > high) {
      failed++;
    }
  }

  return failed;
}

} // namespace bsi
