#include <array>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <string_view>

#include <nlohmann/json.hpp>

#include "bsi.hpp"
#include "bsi_params.hpp"

/// Utility for loading files
/// @param file_name location
/// @param bytes_count amount of bytes to read from a file
/// @returns vector of type T containing binary data from the file
/// Caller ensures presence of the file
template <typename T> std::vector<T> LoadFile(const std::string &file_name, const size_t bytes_count) {
  auto file_size = std::filesystem::file_size(file_name);
  if (file_size < bytes_count) {
    throw std::runtime_error("File is not big enough");
  }
  std::ifstream in(file_name.c_str(), std::ios::binary);
  std::vector<T> file_data{};
  std::copy_n(std::istreambuf_iterator<char>(in), bytes_count, std::back_inserter(file_data));
  return file_data;
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
    std::cerr << "Error! " << e.what() << std::endl;
    return 1;
  }

  if (configuration.skip_all_tests) {
    return 0;
  }

  const auto uchar_sequence = LoadFile<unsigned char>(configuration.input_file, configuration.bytes_count);
  const auto nist_sequence = convert_to_bits(uchar_sequence);

  nlohmann::json results = {{"sequence", configuration.input_file}, {"tests", nlohmann::json::array({})}};

  auto run_test_for_nist_sequence = [&](auto &test_fnc, std::string_view test_name) {
    try {
      auto [num_failed, num_iterations] = test_fnc(nist_sequence);
      results["tests"].push_back(
          {{"name", test_name}, {"error", false}, {"num_runs", num_iterations}, {"num_failures", num_failed}});
    } catch (std::exception &e) {
      std::cerr << "Error(" << test_name << ") ! " << e.what() << std::endl;
      results["tests"].push_back({{"name", test_name}, {"error", true}});
    }
  };

  if (!configuration.skip_words_test) {
    try {
      auto [num_failed, num_iterations] = bsi::words_test(uchar_sequence);
      results["tests"].push_back(
          {{"name", bsi::WordsName}, {"error", false}, {"num_runs", num_iterations}, {"num_failures", num_failed}});
    } catch (std::exception &e) {
      std::cerr << "Error(" << bsi::WordsName << ") ! " << e.what() << std::endl;
      results["tests"].push_back({{"name", bsi::WordsName}, {"error", true}});
    }
  }
  if (!configuration.skip_monobit_test) {
    run_test_for_nist_sequence(bsi::monobit_test, bsi::MonobitName);
  }
  if (!configuration.skip_poker_test) {
    run_test_for_nist_sequence(bsi::poker_test, bsi::PokerName);
  }
  if (!configuration.skip_runs_test) {
    run_test_for_nist_sequence(bsi::runs_test, bsi::RunsName);
  }
  if (!configuration.skip_long_run_test) {
    run_test_for_nist_sequence(bsi::long_run_test, bsi::LongRunName);
  }
  if (!configuration.skip_autocorrelation_test) {
    run_test_for_nist_sequence(bsi::autocorrelation_test, bsi::AutocorrelationName);
  }
  if (!configuration.skip_uniform_test) {
    auto uniform =
        std::bind(bsi::uniform_test, std::placeholders::_1, configuration.K, configuration.N, configuration.A);
    run_test_for_nist_sequence(uniform, bsi::UniformDistributionName);
  }
  if (!configuration.skip_homogenity_test) {
    run_test_for_nist_sequence(bsi::homogenity_test, bsi::HomogenityName);
  }
  if (!configuration.skip_entropy_test) {
    run_test_for_nist_sequence(bsi::entropy_test, bsi::EntropyName);
  }

  std::cout << std::setw(2) << results << std::endl;

  if (configuration.output_to_file) {
    std::ofstream out(configuration.output_file);
    if (!out.is_open()) {
      std::cerr << "Error ! Unable to open output file " << configuration.output_file << std::endl;
      return 1;
    }
    out << std::setw(2) << results;
  }

  return 0;
}
