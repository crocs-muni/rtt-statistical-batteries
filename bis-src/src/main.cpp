#include <array>
#include <exception>
#include <iostream>
#include <random>

#include "bis.hpp"

/// @param characters input sequence containing numbers from 0 to 255
/// @returns Consider N as number of elements of input vector characters.
/// The output vector will be N*8 large containing only 0s and 1s
/// @brief Converts vector of uchars in range 0..255
/// to vector of zeros and ones.
/// this function comes from paranoYa sources
/// see function void MainWidget::diehardToNist() declared in main.cpp
std::vector<unsigned char> convert_to_bits(const std::vector<unsigned char> &characters) {

  const auto input_size = characters.size();
  std::vector<unsigned char> output(input_size * 8, 0);

  auto output_ptr = output.data();

  uint8_t mask{0};
  for (const auto &character : characters) {
    mask = 1u<<7;
    for (size_t i = 0; i < 8; ++i) {
      *output_ptr = ((character & mask) != 0);
      output_ptr++;
      mask >>= 1;
    }
  }

  return output;
}

int main(int argc, char **argv) {
  constexpr size_t InputSize{5140000};
  std::random_device rd{};
  std::mt19937 gen{rd()};
  std::uniform_int_distribution<int> dis(0, 255);

  std::vector<unsigned char> uchar_sequence(InputSize);
  for (size_t i = 0; i < InputSize; ++i) {
    uchar_sequence[i] = dis(gen);
  }
  const auto nist_sequence = convert_to_bits(uchar_sequence);
  try {
    bsi::words_test(uchar_sequence);

    bsi::monobit_test(nist_sequence);

    bsi::poker_test(nist_sequence);
  } catch (std::exception &e) {
    std::cout << "Error! " << e.what() << std::endl;
  }

  return 0;
}
