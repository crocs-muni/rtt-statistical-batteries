#include <exception>
#include <iostream>
#include <random>

#include "bis.hpp"

int main(int argc, char **argv) {
  std::vector<char> input(5140000);
  for (size_t i = 0; i < input.size(); ++i) {
    input[i] = (random() % 2) + '0';
  }

  try {
    bsi::monobit_test(input);

    bsi::poker_test(input);
  } catch (std::exception &e) {
    std::cout << "Error! " << e.what() << std::endl;
  }

  return 0;
}
