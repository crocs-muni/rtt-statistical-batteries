#pragma once
#include <algorithm>
#include <cstddef>
namespace bsi {

typedef struct data_s {
  short Z;
  short tau;
} data_t;

short do_autocorrelation_test(const unsigned char *seq, std::array<data_t, 5000> &data) {
  int ind, tau, numMaxTau;
  short xorSum, maxZ;
  auto origSeq = seq;
  for (tau = 1; tau <= 5000; tau++) {
    xorSum = 0;
    seq = origSeq;
    for (ind = 0; ind < 5000; ind++) {
      xorSum += *seq ^ *(seq + tau);
      seq++;
    }

    xorSum -= 2500;
    if (xorSum < 0)
      xorSum = -xorSum;
    data[tau - 1].Z = xorSum;
    data[tau - 1].tau = tau;
  }
  std::sort(data.begin(), data.end(), [](const auto &lhs, const auto &rhs) { return lhs.Z < rhs.Z; });
  maxZ = data[0].Z;
  numMaxTau = 1;
  while (data[numMaxTau].Z == maxZ)
    numMaxTau++;
  ind = rand() % numMaxTau;
  tau = data[ind].tau;
  seq = origSeq + 10000;
  xorSum = 0;
  for (ind = 0; ind < 5000; ind++) {
    xorSum += *seq ^ *(seq + tau);
    seq++;
  }
  return xorSum;
}

} // namespace bsi
