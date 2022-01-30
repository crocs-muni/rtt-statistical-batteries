#pragma once

#include <algorithm>
#include <array>
#include <chrono>
#include <climits>
#include <compare>
#include <cstdlib>
#include <cstring>
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
void words_test(const std::vector<unsigned char> &input_sequence) {
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

size_t do_monobit_test(const unsigned char *seq) {
  size_t sum{0};
  for (size_t i = 0; i < 20000; ++i) {
    sum += *seq;
    ++seq;
  }

  return sum;
}

/// t1_monobit
void monobit_test(const std::vector<unsigned char> &input_sequence) {
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

/// t2_poker
void poker_test(const std::vector<unsigned char> &input_sequence) {
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

/// tbsi_t3_runs
void runs_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  std::array<char, 257> results{};
  std::fill(results.begin(), results.end(), 0);

  // test of input length, minimal is 5 140 000 bits (642 500 Bytes)
  const auto input_size = input_sequence.size(); // size of input string is in bits!!!
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }
  size_t failed{0};
  const size_t sequence_stride{20000};
  for (auto &&result : results) {
    result = do_runs_test(seq);
    if (result > 0) {
      ++failed;
    }
    seq += sequence_stride;
  }
  std::cout << "Runs failed: " << failed << std::endl;
}

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

void long_run_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  std::array<bool, 257> results{};
  std::fill(results.begin(), results.end(), 0);

  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }

  size_t failed{0};
  for (auto &&result : results) {
    result = do_long_run_test(seq);
    if (result) {
      ++failed;
    }
    seq += 20000;
  }

  std::cout << "Long Run failed: " << failed << std::endl;
}

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

void autocorrelation_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  std::array<short, 257> results{};
  std::fill(results.begin(), results.end(), 0);
  const auto input_size = input_sequence.size();
  if (input_size < 5140000) {
    throw std::runtime_error("Invalid size of input sequence");
  }
  srand(time(NULL));
  std::array<data_t, 5000> data{};
  std::fill(data.begin(), data.end(), data_t{0, 0});
  size_t failed{0};
  for (auto &&result : results) {
    result = do_autocorrelation_test(seq, data);
    if (result < 2326 || result > 2674) {
      ++failed;
    }
    seq += 20000;
  }

  std::cout << "Autocorellation failed: " << failed << std::endl;
}

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

void uniform_test(const std::vector<unsigned char> &input_sequence, const size_t K, const size_t N, const double A) {
  auto seq = input_sequence.data();
  const auto input_size = input_sequence.size();
  if (K * N > input_size) {
    throw std::runtime_error("Required length of the sequence to be tested is too large");
  }
  if (K < 1 || K > 24) {
    throw std::runtime_error("Length of the vectors to be tested must be in [1;24]");
  }
  if (A <= 0 || A >= 1) {
    throw std::runtime_error("Parameter A must be in (0;1)");
  }

  size_t size{1ull << K};
  std::vector<size_t> X(size, 0);

  size_t failed{0};
  const auto iterations = input_size / (N * K);
  for (size_t i = 0; i < iterations; i++) {
    std::fill(X.begin(), X.end(), 0);
    if (do_uniform_test(seq, X.data(), K, N, A) > 0) {
      ++failed;
    }
    seq += K * N;
  }
  std::cout << "Uniform failed: " << failed << std::endl;
}

unsigned long long **createTF(int K) {
  unsigned long long **tab = NULL;
  unsigned long size;
  long i;

  size = 1ul << K;
  tab = (unsigned long long **)calloc(size, sizeof(unsigned long long *));
  if (!tab)
    return NULL;
  for (i = 0; (unsigned long)i < size; i++) {
    tab[i] = (unsigned long long *)calloc(2, sizeof(unsigned long long));
    if (!tab[i]) {
      i--;
      while (i > 0)
        free(tab[i]);
      free(tab);
      return NULL;
    }
  } // for i allocate TF arrray
  return tab;
}

void destroyTF(unsigned long long **tab, int K) {
  unsigned long size, i;
  size = 1ul << K;
  for (i = 0; i < size; i++)
    free(tab[i]);
  free(tab);
}

long TF(const unsigned char *seq, long availableLen, unsigned long long **tab, int K, int N) {
  unsigned long long w;
  long size, working;
  int i;
  long j;

  size = 1ul << K;
  working = size;

  while ((availableLen > 0) && (working > 0)) {
    // create K-bit word
    w = 0;
    for (i = 0; i < K; i++) {
      w <<= 1;
      w |= *seq++;
      availableLen--;
      if (availableLen == 0)
        return -1;
    }
    // if the related counter is < N, increase the counter
    if (tab[w][0] + tab[w][1] < N)
      tab[w][*seq]++;
    seq++;
    availableLen--;
    // if the bound N is reached, we decrease the working subsequences
    // and we increase the sum of the subsequence by 2 to avoid again decreasing
    // so we have to decrease this number after the computing
    if (tab[w][0] + tab[w][1] == N) {
      working--;
      tab[w][0]++;
      tab[w][1]++;
    }
  } // while working > 0  && availableLen > 0

  // check if the computing has been finished correctly
  if (working > 0)
    return -1;

  // correct the vaules for all sum subsequences
  for (j = 0; j < size; j++) {
    tab[j][0]--;
    tab[j][1]--;
  }

  return availableLen;
}

int doEvaluationT7(unsigned long long **tab, int K, double bound) {
  unsigned long size, i;
  double sum;
  int failed = 0;

  size = 1ul << (K - 1);
  sum = 0.0;
  for (i = 0; i < size; i++) {
    sum += ((tab[i][0] - tab[i + size][0]) * (tab[i][0] - tab[i + size][0])) / (double)(tab[i][0] + tab[i + size][0]) +
           ((tab[i][1] - tab[i + size][1]) * (tab[i][1] - tab[i + size][1])) / (double)(tab[i][1] + tab[i + size][1]);
    if (sum > bound)
      failed++;
  }
  return failed;
}

void homogenity_test(const std::vector<unsigned char> &input_sequence) {
  constexpr size_t MIN_K{1};
  constexpr size_t MAX_K{3};
  auto seq = input_sequence.data();
  int failed, iterations, end;
  unsigned long long **tabTF;
  long ret{0};
  double result{0.0};
  auto input_size = input_sequence.size();
  size_t failedTotal{0};
  iterations = 0;
  end = 0;
  while (!end) {
    iterations++;
    failed = 0;
    for (size_t i = MIN_K; i <= MAX_K; i++) {
      tabTF = createTF(i);
      if (!tabTF) {
        throw std::runtime_error("Insufficient memory");
      }
      ret = TF(seq, input_size, tabTF, i, 100000);
      if (ret == -1) {
        destroyTF(tabTF, i);
        if (iterations == 1) {
          throw std::runtime_error("Bad input data");
        }
        end = 1;
        iterations--;
        break;
      }
      if (i == 1) {
        result = (tabTF[0][1] + tabTF[1][0]) / 100000.0 - 1;
        if (result < 0)
          result = -result;
        if (result >= 0.02) {
          failedTotal++;
        }
      }
      else {
        failed = doEvaluationT7(tabTF, i, 15.13);
        if (failed > 0) {
          failedTotal += failed;
        }
      }
      seq += input_size - ret;
      input_size = ret;
      destroyTF(tabTF, i);
    }
  }

  std::cout << "Homogenity failed: " << failedTotal << std::endl;
}

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
    G[i] /= log(2.0);
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

void entropy_test(const std::vector<unsigned char> &input_sequence) {
  auto seq = input_sequence.data();
  auto input_size = input_sequence.size();
  int ret{0};

  if (input_size < 8 * (2560 + 256000)) {
    throw std::runtime_error("Invalid size of input sequence");
  }
  size_t iterations{0};
  size_t failed{0};
  while (input_size >= 8 * (2560 + 256000)) {
    ret = do_entropy_test(seq, 8, 2560, 256000);
    if (ret == -1) {
      throw std::runtime_error("Insufficient memory");
    }
    iterations++;
    if (ret == 1) {
      failed++;
    }
    input_size -= 8 * (2560 + 256000);
    seq += 8 * (2560 + 256000);
  }

  std::cout << "Entropy test failed: " << failed << std::endl;
}

} // namespace bsi
