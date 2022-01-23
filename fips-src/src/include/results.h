#pragma once

#include <array>
#include <fstream>
#include <iostream>
#include <string>

#include "fips.h"
#include "params.h"

namespace fips {
typedef struct {
  std::string test;
  size_t num_runs;
  size_t num_failures;
} test_status;

void save_fips_run(int result, std::array<test_status, N_FIPS_TESTS> &statuses) {
  for (int test = 0; test < N_FIPS_TESTS; ++test) {
    ++statuses[test].num_runs;
    if (result & fips_test_mask[test]) {
      ++statuses[test].num_failures;
    }
  }
}

void dump_test_result_to_txt(std::ofstream &out, const test_status &status) {
  out << "Single Test Result:" << std::endl;
  out << status.test << std::endl;
  out << status.num_failures << " failed out of " << status.num_runs << std::endl;
  out << std::endl;
}

void dump_test_result_to_json(std::ofstream &out, const test_status &status, bool last) {
  out << "\t\t{" << std::endl;
  out << "\t\t\t\"name\": \"" << status.test << "\"," << std::endl;
  out << "\t\t\t\"num_runs\": \"" << status.num_runs << "\"," << std::endl;
  out << "\t\t\t\"num_failures\": \"" << status.num_failures << "\"" << std::endl;
  out << "\t\t}";

  if (!last) {
    out << ",";
  }
  out << std::endl;
}

void save_per_test_results(std::ofstream &out, const fips::test_params &params,
                           const std::array<test_status, N_FIPS_TESTS> &statuses) {
  std::vector<test_status> statuses_to_print{};

  if (!params.skip_monobit_test) {
    statuses_to_print.emplace_back(statuses[0]);
  }
  if (!params.skip_poker_test) {
    statuses_to_print.emplace_back(statuses[1]);
  }
  if (!params.skip_runs_test) {
    statuses_to_print.emplace_back(statuses[2]);
  }
  if (!params.skip_long_run_test) {
    statuses_to_print.emplace_back(statuses[3]);
  }
  if (!params.skip_continuous_run_test) {
    statuses_to_print.emplace_back(statuses[4]);
  }

  for (size_t i = 0; i < statuses_to_print.size(); ++i) {
    if (params.json) {
      dump_test_result_to_json(out, statuses_to_print[i], i == statuses_to_print.size() - 1);
    } else {
      dump_test_result_to_txt(out, statuses_to_print[i]);
    }
  }
}

void save_to_json(std::ofstream &out, const fips::test_params &params, bool battery_success,
                  const std::array<test_status, N_FIPS_TESTS> &statuses) {
  out << "{" << std::endl;
  out << "\t\"status\": ";
  if (battery_success) {
    out << "\"Accepted\",";
  } else {
    out << "\"Rejected\",";
  }

  out << std::endl;

  if (!params.skip_all_tests) {
    out << "\t\"tests\": [" << std::endl;
    save_per_test_results(out, params, statuses);
    out << "\t]" << std::endl;
  }
  out << "}" << std::endl;
}

void save_to_txt(std::ofstream &out, const fips::test_params &params, bool battery_success,
                 const std::array<test_status, N_FIPS_TESTS> &statuses) {
  out << "FIPS 140-2 battery: ";
  if (battery_success) {
    out << "Accepted";
  } else {
    out << "Rejected";
  }
  out << std::endl << std::endl;

  if (!params.skip_all_tests) {
    save_per_test_results(out, params, statuses);
  }
}

void generate_report(const fips::test_params &params, bool battery_success,
                     const std::array<test_status, N_FIPS_TESTS> &statuses) {
  std::ofstream out(params.output_file);
  if (!out.is_open()) {
    std::cerr << "Error: Cannot open output file for writing." << std::endl;
    return;
  }

  if (params.json) {
    save_to_json(out, params, battery_success, statuses);
  } else {
    save_to_txt(out, params, battery_success, statuses);
  }
}

} // namespace fips
