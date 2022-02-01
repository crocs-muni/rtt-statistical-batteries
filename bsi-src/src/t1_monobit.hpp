#pragma once
#include <cstddef>
namespace bsi {

size_t do_monobit_test(const unsigned char *seq) {
  size_t sum{0};
  for (size_t i = 0; i < 20000; ++i) {
    sum += *seq;
    ++seq;
  }

  return sum;
}

} // namespace bsi
