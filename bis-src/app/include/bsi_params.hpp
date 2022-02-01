#pragma once

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

#include "clipp.h"

namespace bsi {
namespace args {

class Configuration {
public:
  // IO
  std::string input_file;
  std::string output_file;
  bool json;

  // Tests
  bool skip_all_tests;
  bool skip_words_test;
  bool skip_monobit_test;
  bool skip_poker_test;
  bool skip_runs_test;
  bool skip_long_run_test;
  bool skip_autocorrelation_test;
  bool skip_uniform_test;
  bool skip_homogenity_test;
  bool skip_entropy_test;

  // uniform distribution attributes
  size_t K;
  size_t N;
  double A;

  Configuration()
      : input_file{""}, output_file{""}, json{false}, skip_all_tests{false}, skip_words_test{false},
        skip_monobit_test{false}, skip_poker_test{false}, skip_runs_test{false}, skip_long_run_test{false},
        skip_autocorrelation_test{false}, skip_uniform_test{false}, skip_homogenity_test{false},
        skip_entropy_test{false}, K{20}, N{500000}, A{0.1}, cli{}, man_page_requested{false} {
    initialize_arguments();
  }

  void parse(const std::vector<std::string> &arguments) const {
    auto parsing_result = clipp::parse(arguments.begin(), arguments.end(), cli);
    if (man_page_requested) {
      print_man_page();
      throw int{1};
    } else if (!parsing_result) {
      print_usage();
      throw std::runtime_error("Required arguments were not provided");
    } else {
      validate_args();
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
         clipp::option("--skip_words_test").set(skip_words_test, true) % "T0 Words test will be skipped",
         clipp::option("--skip_monobit_test").set(skip_monobit_test, true) % "T1 Monobit test will be skipped",
         clipp::option("--skip_poker_test").set(skip_poker_test, true) % "T2 Poker test will be skipped",
         clipp::option("--skip_runs_test").set(skip_runs_test, true) % "T3 Runs test will be skipped",
         clipp::option("--skip_long_run_test").set(skip_long_run_test, true) % "T4 Long Run test will be skipped",
         clipp::option("--skip_autocorrelation_test").set(skip_autocorrelation_test, true) %
             "T5 Autocorrelation test will be skipped",
         clipp::option("--skip_uniform_test").set(skip_uniform_test, true) %
             "T6 Uniform Distribution test will be skipped",
         clipp::option("--skip_homogenity_test").set(skip_homogenity_test, true) % "T7 Homogenity test will be skipped",
         clipp::option("--skip_entropy_test").set(skip_entropy_test, true) % "T8 Entropy test will be skipped");

    const auto uniform_distribution_options =
        ("Uniform Distribution Test attributes" %
         ((clipp::option("-K") & clipp::value("length", K)) % "length of the vectors to be tested",
          (clipp::option("-N") & clipp::value("length", N)) % "length of the sequence to be tested",
          (clipp::option("-A") & clipp::value("length", A)) % "length of the sequence to be tested"));

    const auto help_options =
        (clipp::option("-h").set(man_page_requested, true), clipp::option("--help").set(man_page_requested, true));

    cli = (required_group, optional_group, uniform_distribution_options, help_options);
  }

  void print_usage() const { std::cout << clipp::usage_lines(cli, app_name.c_str()) << std::endl; }

  void print_man_page() const { std::cout << clipp::make_man_page(cli, app_name.c_str()) << std::endl; }

  void validate_args() const {
    if (!std::filesystem::is_regular_file(input_file)) {
      throw std::runtime_error("Input file is not a regular file");
    }
  }

  // private members
private:
  clipp::group cli;
  const std::string app_name{"bsi_battery"};
  bool man_page_requested;
};

} // namespace args
} // namespace bsi
