#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "fips.h"
#include "fips_params.hpp"
#include "results.hpp"

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

int main(int argc, char **argv) {
  const std::vector<std::string> arguments(argv + 1, argv + argc);
  fips::args::Configuration configuration{};
  try {
    configuration.parse(arguments);
  } catch (int &exit_status) {
    return exit_status;
  } catch (std::exception &e) {
    std::cout << "Error ! " << e.what() << std::endl;
    return 1;
  }

  std::unique_ptr<fips_ctx> ctx = std::make_unique<fips_ctx>();
  fips_init(ctx.get(), 0);

  std::vector<unsigned char> input_file_data{};
  try {
    input_file_data = LoadFile<unsigned char>(configuration.input_file.c_str());
  } catch (std::exception &e) {
    std::cout << "Error (" << configuration.input_file << ") ! " << e.what() << std::endl;
  }

  const size_t input_size{input_file_data.size()};

  if (input_size < FIPS_RNG_BUFFER_SIZE) {
    std::cerr << "ERROR: Not enough input bytes (" << input_size << "< " << FIPS_RNG_BUFFER_SIZE << ")\n";
    return EXIT_FAILURE;
  }

  std::array<fips::test_status, N_FIPS_TESTS> statuses{};

  for (size_t test = 0; test < N_FIPS_TESTS; ++test) {
    statuses[test] = fips::test_status{fips_test_names[test], 0, 0};
  }

  bool succeeded = true;

  auto data_ptr = input_file_data.data();
  for (size_t used_bytes = 0; used_bytes + FIPS_RNG_BUFFER_SIZE < input_size; used_bytes += FIPS_RNG_BUFFER_SIZE) {
    const void *buffer = reinterpret_cast<void*>(data_ptr + used_bytes);
    auto result = fips_run_rng_test(ctx.get(), buffer);
    if (result) {
      succeeded = false;
    }
    fips::save_fips_run(result, statuses);
  }

  if (!configuration.skip_all_tests) {
    fips::generate_report(configuration, succeeded, statuses);
  }

  return EXIT_SUCCESS;
}
