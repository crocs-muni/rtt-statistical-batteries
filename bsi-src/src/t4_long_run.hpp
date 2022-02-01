#pragma once
#include <cstddef>
#include <cstdint>

namespace bsi {

/// @returns This function returns TRUE in case of a failure
bool do_long_run_test(const unsigned char *seq) {
  uint8_t current_bit = *seq++;
  size_t current_run_length = 1;
  size_t max_run_length = 1;
  for (size_t i = 1; i < 20000; i++) {
    if (*seq++ == current_bit) {
      ++current_run_length;
    } else {
      if (current_run_length > max_run_length) {
        max_run_length = current_run_length;
      }
      current_bit = *(seq - 1);
      current_run_length = 1;
    }
  }
  if (current_run_length > max_run_length) {
    max_run_length = current_run_length;
  }
  return max_run_length >= 34;
}

} // namespace bsi
