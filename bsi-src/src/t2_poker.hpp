#pragma once
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
namespace bsi {

double do_poker_test(const unsigned char *seq) {
  int number{0};
  double x{0.0};
  std::array<int, 16> numoccur{};
  std::fill(numoccur.begin(), numoccur.end(), 0);

  // 20000 bits, 1 number consists from 4 bits => 5000 iterations
  for (size_t i = 0; i < 5000; i++) {
    number = ((seq[0]) << 3) + ((seq[1]) << 2) + ((seq[2]) << 1) + ((seq[3]) << 0);
    seq += 4;
    numoccur[number]++;
  }

  for (const auto &number : numoccur) {
    x += std::pow(number, 2);
  }

  x = x * (16.0 / 5000.0) - 5000;

  return x;
}

} // namespace bsi
