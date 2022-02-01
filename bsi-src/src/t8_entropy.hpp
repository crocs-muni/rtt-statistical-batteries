#pragma once
#include <cmath>
#include <cstddef>
#include <cstring>
namespace bsi {

int do_entropy_test(const unsigned char *seq, const int L, const int Q, const int K) {
  size_t w{0};
  const size_t size{1ul << L};
  int *lastPosition = new int[size];
  std::memset(lastPosition, -1, size * sizeof(lastPosition[0]));
  double *G = new double[K + Q + 1];
  G[0] = G[1] = 0.0;
  for (size_t i = 1; i <= K + Q - 1; i++) {
    G[i + 1] = G[i] + 1.0 / (i);
  }
  for (size_t i = 1; i <= K + Q; i++) {
    G[i] /= std::log(2.0);
  }

  for (size_t i = 0; i < Q; i++) {
    w = 0;
    for (size_t j = 0; j < L; j++)
      w += seq[L * i + j] << j;
    lastPosition[w] = i;
  }

  double TG{0.0};
  size_t distance{0};
  for (size_t i = Q; i < K + Q; i++) {
    w = 0;
    for (size_t j = 0; j < L; j++)
      w += seq[L * i + j] << j;
    if (lastPosition[w] >= 0) {
      distance = i - lastPosition[w];
      TG += G[distance];
    }
    lastPosition[w] = i;
  }

  delete[] G;
  delete[] lastPosition;

  TG /= (double)K;
  if (TG > 7.976) {
    return 0;
  } else {
    return 1;
  }
}

} // namespace bsi
