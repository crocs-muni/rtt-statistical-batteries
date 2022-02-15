#include <exception>
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

/// Auxiliary function for file loading.
/// This function Throws exception if file is not present
/// @param file_name location
/// @returns vector of type T containing binary data from the file
template <typename T> std::vector<T> LoadFile(const std::string &file_name) {
  std::ifstream in(file_name.c_str(), std::ios::binary);
  if (!in.is_open()) {
    throw std::runtime_error("Unable to open file");
  }
  std::vector<char> raw_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  return std::vector<T>(reinterpret_cast<T *>(raw_data.data()),
                        reinterpret_cast<T *>(raw_data.data()) + raw_data.size());
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
    std::cout << "Error ! " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  if (configuration.skip_all_tests) {
    return EXIT_SUCCESS;
  }

  std::unique_ptr<fips_ctx> ctx = std::make_unique<fips_ctx>();
  fips_init(ctx.get(), 0);

  std::vector<unsigned char> input_file_data{};
  try {
    input_file_data = LoadFile<unsigned char>(configuration.input_file.c_str());
  } catch (std::exception &e) {
    std::cout << "Error (" << configuration.input_file << ") ! " << e.what() << std::endl;
  }

  const auto input_size = input_file_data.size();

  if (input_size < FIPS_RNG_BUFFER_SIZE) {
    std::cerr << "ERROR: Not enough input bytes (" << input_size << "< " << FIPS_RNG_BUFFER_SIZE << ")\n";
    return EXIT_FAILURE;
  }

  std::ofstream out(configuration.output_file);
  if (not out.is_open()) {
    std::cout << "Error ! Unable to open output file " << configuration.output_file << std::endl;
    return 1;
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

  out << std::setw(2) << results;

  return EXIT_SUCCESS;
}
