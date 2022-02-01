#pragma once
#include <cstdlib>

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
