#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "clipp.h"

namespace fips {
namespace args {
class Configuration {
public:
  // IO
  std::string input_file;
  std::string output_file;
  bool json;

  // Tests
  bool skip_all_tests;
  bool skip_runs_test;
  bool skip_poker_test;
  bool skip_monobit_test;
  bool skip_long_run_test;
  bool skip_continuous_run_test;

  Configuration()
      : input_file{""}, output_file{""}, json{false}, skip_all_tests{false}, skip_runs_test{false},
        skip_poker_test{false}, skip_monobit_test{false}, skip_long_run_test{false},
        skip_continuous_run_test{false}, cli{}, man_page_requested{false} {
    initialize_arguments();
  }

  void parse(const std::vector<std::string> &arguments) const {
    const auto parsing_result = clipp::parse(arguments.cbegin(), arguments.cend(), cli);
    if (man_page_requested) {
      print_man_page();
      throw int{1};
    } else if (!parsing_result) {
      print_usage();
      throw std::runtime_error("Wrong arguments provided");
    }
  }

  // private functions
private:
  void initialize_arguments() {
    const auto required_group = ((clipp::required("-i", "--input_file") & clipp::value("path", input_file)) %
                                     "Path to an input sequence to be tested",
                                 (clipp::required("-o", "--output_file") & clipp::value("path", output_file)) %
                                     "Path to a file where results will be stored");

    const auto optional_group =
        (clipp::option("-j", "--json").set(json, true) % "JSON output will be generated",
         clipp::option("--skip_all_tests").set(skip_all_tests, true) % "All tests will be skipped",
         clipp::option("--skip_runs_test").set(skip_runs_test, true) % "Runs test will be skipped",
         clipp::option("--skip_poker_test").set(skip_poker_test, true) % "Poker test will be skipped",
         clipp::option("--skip_monobit_test").set(skip_monobit_test, true) % "Monobit test will be skipped",
         clipp::option("--skip_long_run_test").set(skip_long_run_test, true) % "Long Run test will be skipped",
         clipp::option("--skip_continuous_run_test").set(skip_continuous_run_test, true) %
             "Continuous Run test will be skipped");

    const auto help_options =
        (clipp::option("-h", "--help").set(man_page_requested, true) % "Print this man page");

    cli = (required_group, optional_group, help_options);
  }

  void print_usage() const { std::cout << clipp::usage_lines(cli, app_name.c_str()) << std::endl; }

  void print_man_page() const { std::cout << clipp::make_man_page(cli, app_name.c_str()) << std::endl; }

  // private members
private:
  clipp::group cli;
  const std::string app_name{"fips_battery"};
  bool man_page_requested;
};
} // namespace args

} // namespace fips
