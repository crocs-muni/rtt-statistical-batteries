#include <array>
#include <exception>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>

#include "bsi_params.hpp"

#include "bsi.hpp"

/// Utility for loading files
/// @param file_name location
/// @returns vector of type T containing binary data from the file
/// Caller ensures presence of the file
template <typename T> std::vector<T> LoadFile(const std::string &file_name) {
  std::ifstream in(file_name.c_str(), std::ios::binary);
  std::vector<char> raw_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  return std::vector<T>(reinterpret_cast<T *>(raw_data.data()),
                        reinterpret_cast<T *>(raw_data.data()) + raw_data.size());
}

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
    mask = 1u << 7;
    for (size_t i = 0; i < 8; ++i) {
      *output_ptr = ((character & mask) != 0);
      output_ptr++;
      mask >>= 1;
    }
  }

  return output;
}

int main(int argc, char **argv) {
  const std::vector<std::string> arguments(argv + 1, argv + argc);
  bsi::args::Configuration configuration{};
  try {
    configuration.parse(arguments);
  } catch (int &exit_status) {
    return exit_status;
  } catch (std::exception &e) {
    std::cout << "Error! " << e.what() << std::endl;
    return 1;
  }

  const auto uchar_sequence = LoadFile<unsigned char>(configuration.input_file);
  const auto nist_sequence = convert_to_bits(uchar_sequence);

  auto run_test_for_nist_sequence = [&](auto &test, const std::string &test_name) {
    try {
      test(nist_sequence);
    } catch (std::exception &e) {
      std::cout << "Error(" << test_name << ") ! " << e.what() << std::endl;
    }
  };

  if (configuration.skip_all_tests) {
    return 0;
  }
  if (!configuration.skip_words_test) {
    bsi::words_test(uchar_sequence);
  }
  if (!configuration.skip_monobit_test) {
    run_test_for_nist_sequence(bsi::monobit_test, "T1-Monobit");
  }
  if (!configuration.skip_poker_test) {
    run_test_for_nist_sequence(bsi::poker_test, "T2-Poker");
  }
  if (!configuration.skip_runs_test) {
    run_test_for_nist_sequence(bsi::runs_test, "T3-Runs");
  }
  if (!configuration.skip_long_run_test) {
    run_test_for_nist_sequence(bsi::long_run_test, "T4-LongRun");
  }
  if (!configuration.skip_autocorrelation_test) {
    run_test_for_nist_sequence(bsi::autocorrelation_test, "T5-Autocorrelation");
  }
  if (!configuration.skip_uniform_test) {
    auto uniform =
        std::bind(bsi::uniform_test, std::placeholders::_1, configuration.K, configuration.N, configuration.A);
    run_test_for_nist_sequence(uniform, "T6-UniformDistribution");
  }
  if (!configuration.skip_homogenity_test) {
    run_test_for_nist_sequence(bsi::homogenity_test, "T7-Homogenity");
  }
  if (!configuration.skip_entropy_test) {
    run_test_for_nist_sequence(bsi::entropy_test, "T8-Entropy");
  }
  return 0;
}
