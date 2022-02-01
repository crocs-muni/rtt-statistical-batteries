#pragma once

#include <string>

#include "clipp.h"

namespace fips {

struct test_params {
  // IO
  std::string input_file;
  std::string output_file;
  bool is_binary;
  bool json;

  // Tests
  bool skip_all_tests;
  bool skip_runs_test;
  bool skip_poker_test;
  bool skip_monobit_test;
  bool skip_long_run_test;
  bool skip_continuous_run_test;

  test_params(const std::vector<std::string> &arguments) {
    input_file = "";
    output_file = "";
    is_binary = false;
    json = false;
    skip_all_tests = false;
    skip_runs_test = false;
    skip_poker_test = false;
    skip_monobit_test = false;
    skip_long_run_test = false;
    skip_continuous_run_test = false;

    cli = (clipp::required("-i", "--input-file") & clipp::value("path", input_file),
           clipp::required("-o", "--output-file") & clipp::value("path", output_file),
           clipp::option("-b", "--binary-file").set(is_binary, true), clipp::option("-j", "--json").set(json, true),
           clipp::option("-sa", "--skip-all-tests").set(is_binary, true),
           clipp::option("-sr", "--skip-runs-test").set(is_binary, true),
           clipp::option("-sp", "--skip-poker-test").set(is_binary, true),
           clipp::option("-sm", "--skip-monobit-test").set(is_binary, true),
           clipp::option("-sl", "--skip-long_run-test").set(is_binary, true),
           clipp::option("-sc", "--skip-continuous-run-test").set(is_binary, true));

    const auto result = clipp::parse(arguments.begin(), arguments.end(), cli);
    if (!result) {
      std::cout << clipp::usage_lines(cli, "fips_battery") << std::endl;
      exit(1);
    } else {
      if (!fs::is_regular_file(input_file)) {
        exit(-1);
      }
    }
  }

private:
  clipp::group cli;
};

} // namespace fips
