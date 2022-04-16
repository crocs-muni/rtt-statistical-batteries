#include <exception>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>

#include "fips.h"
#include "fips_params.hpp"

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

typedef struct {
  std::string test;
  size_t num_runs;
  size_t num_failures;
} test_status;

int main(int argc, char **argv) {
  const std::vector<std::string> arguments(argv + 1, argv + argc);
  fips::args::Configuration configuration{};
  try {
    configuration.parse(arguments);
  } catch (int &exit_status) {
    return exit_status;
  } catch (std::exception &e) {
    std::cerr << "Error ! " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  if (configuration.skip_all_tests) {
    return EXIT_SUCCESS;
  }

  std::unique_ptr<fips_ctx> ctx = std::make_unique<fips_ctx>();
  fips_init(ctx.get(), 0);

  std::vector<unsigned char> input_file_data{};
  try {
    input_file_data = LoadFile<unsigned char>(configuration.input_file.c_str(), configuration.bytes_count);
  } catch (std::exception &e) {
    std::cerr << "Error (" << configuration.input_file << ") ! " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  const auto input_size = input_file_data.size();

  if (input_size < FIPS_RNG_BUFFER_SIZE) {
    std::cerr << "ERROR: Not enough input bytes (" << input_size << "< " << FIPS_RNG_BUFFER_SIZE << ")\n";
    return EXIT_FAILURE;
  }

  std::array<test_status, N_FIPS_TESTS> statuses{};

  for (size_t test = 0; test < N_FIPS_TESTS; ++test) {
    statuses[test] = test_status{fips_test_names[test], 0, 0};
  }

  bool accepted = true;

  auto data_ptr = input_file_data.data();
  for (size_t used_bytes = 0; used_bytes + FIPS_RNG_BUFFER_SIZE < input_size; used_bytes += FIPS_RNG_BUFFER_SIZE) {
    const void *buffer = reinterpret_cast<void *>(data_ptr + used_bytes);
    auto result = fips_run_rng_test(ctx.get(), buffer);
    if (result) {
      accepted = false;
    }
    for (size_t test = 0; test < N_FIPS_TESTS; ++test) {
      ++statuses[test].num_runs;
      if (result & fips_test_mask[test]) {
        ++statuses[test].num_failures;
      }
    }
  }

  nlohmann::json results = {
      {"sequence", configuration.input_file}, {"accepted", accepted}, {"tests", nlohmann::json::array({})}};

  for (const auto &status : statuses) {
    results["tests"].push_back(
        {{"name", status.test}, {"num_runs", status.num_runs}, {"num_failures", status.num_failures}});
  }

  std::cout << std::setw(2) << results << std::endl;

  if (configuration.output_to_file) {
    std::ofstream out(configuration.output_file);
    if (not out.is_open()) {
      std::cerr << "Error ! Unable to open output file " << configuration.output_file << std::endl;
      return EXIT_FAILURE;
    }
    out << std::setw(2) << results;
  }

  return EXIT_SUCCESS;
}
