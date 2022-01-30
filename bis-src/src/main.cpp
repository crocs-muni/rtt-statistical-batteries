#include <exception>
#include <iostream>
#include <random>

#include "bis.hpp"

int main(int argc, char **argv) {
  constexpr size_t InputSize{5140000};
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<int> dis(0, 127);

  std::vector<char> input(InputSize);
  std::vector<char> words_input(InputSize);
  for (size_t i = 0; i < InputSize; ++i) {
    input[i] = (random() % 2) + '0';
    words_input[i] = dis(gen);
  }

  try {
    bsi::words_test(words_input);

    bsi::monobit_test(input);

    bsi::poker_test(input);
  } catch (std::exception &e) {
    std::cout << "Error! " << e.what() << std::endl;
  }

  return 0;
}
