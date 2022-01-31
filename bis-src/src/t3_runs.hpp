#pragma once
#include <array>
#include <cstdint>
#include <cstddef>

namespace bsi {

char do_runs_test(const unsigned char *seq) {
  constexpr size_t MaxRunLength{6};
  constexpr std::array<size_t, MaxRunLength> lnruns{2267, 1079, 502, 223, 90, 90};
  constexpr std::array<size_t, MaxRunLength> hnruns{2733, 1421, 748, 402, 223, 223};

  std::array<size_t, MaxRunLength> numruns0{};
  std::fill(numruns0.begin(), numruns0.end(), 0);
  std::array<size_t, MaxRunLength> numruns1{};
  std::fill(numruns1.begin(), numruns1.end(), 0);

  uint8_t current_bit{*seq++};
  int current_run_length{1};
  for (size_t i = 1; i < 20000; i++) {
    if (*seq++ == current_bit) {
      ++current_run_length;
    } else {
      if (current_run_length > MaxRunLength) {
        current_run_length = MaxRunLength;
      }
      if (current_bit == 1) {
        numruns1[current_run_length - 1]++;
      } else {
        numruns0[current_run_length - 1]++;
      }
      current_bit = *(seq - 1);
      current_run_length = 1;
    }
  }

  // encounter the last run of bits
  if (current_run_length > MaxRunLength) {
    current_run_length = MaxRunLength;
  }
  if (current_bit == 1) {
    numruns1[current_run_length - 1]++;
  } else {
    numruns0[current_run_length - 1]++;
  }
  // evaluation
  size_t failed{0};
  for (size_t i = 0; i < MaxRunLength; i++) {
    if ((numruns0[i] < lnruns[i]) || (numruns0[i] > hnruns[i]))
      failed++;
    if ((numruns1[i] < lnruns[i]) || (numruns1[i] > hnruns[i]))
      failed++;
  }

  return failed;
}

} // namespace bsi
