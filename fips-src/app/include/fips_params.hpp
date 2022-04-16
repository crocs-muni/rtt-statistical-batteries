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
  size_t bytes_count;
  bool output_to_file;
  bool json;

  // Tests
  bool skip_all_tests;

  Configuration()
      : input_file{""}, output_file{""}, bytes_count{0}, output_to_file{false}, skip_all_tests{false}, cli{},
        man_page_requested{false} {
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
                                 (clipp::required("-b", "--bytes_count") & clipp::value("bytes", bytes_count)) %
                                     "Number of bytes to be read from input file (must be less than file size)");

    const auto optional_group =
        ((clipp::option("-o", "--output_file").set(output_to_file, true) & clipp::value("path", output_file)) %
             "Path to a file where results will be stored",
         clipp::option("--skip_all_tests").set(skip_all_tests, true) % "All tests will be skipped");

    const auto help_options = (clipp::option("-h", "--help").set(man_page_requested, true) % "Print this man page");

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
