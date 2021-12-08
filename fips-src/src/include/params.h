#pragma once

#include <string>

#include <gflags/gflags.h>

#include "common.h"

#define FIPS_STS_VERSION "0.0.1"
#define FIPS_STS_EXE_USAGE "FIPS statistical tests battery"

// Parameters defining IO
DEFINE_bool(binary_file,
            false,
            "Specify input file format");
DEFINE_bool(json,
            false,
            "Save test results to json");
DEFINE_string(input_file,
              "",
              "Specify a path to an input file");
DEFINE_string(output_file,
              "",
              "Specify a path to an output file");

// Parameters defining test results
DEFINE_bool(skip_monobit_test,
            false,
            "Monobit test results are not stored");
DEFINE_bool(skip_poker_test,
            false,
            "Poker test results are not stored");
DEFINE_bool(skip_runs_test,
            false,
            "Runs test results are not stored");
DEFINE_bool(skip_long_run_test,
            false,
            "Long Run test results are not stored");
DEFINE_bool(skip_continuous_run_test,
            false,
            "Continuous Run test results are not stored");
DEFINE_bool(skip_all_tests,
            false,
            "All results are skipped");

namespace fips
{

    typedef struct
    {
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
    } test_params;

    bool validate_params(const test_params &params)
    {
        if (!fs::is_regular_file(params.input_file))
        {
            std::cerr << "ERROR: Input file path does not exist\n";
            return false;
        }

        return true;
    }

    bool process_params(int *argc, char **argv, test_params &out_params)
    {
        gflags::SetUsageMessage(FIPS_STS_EXE_USAGE);
        gflags::SetVersionString(FIPS_STS_VERSION);
        gflags::ParseCommandLineFlags(argc, &argv, false);

        // IO
        out_params.is_binary = FLAGS_binary_file;
        out_params.json = FLAGS_json;
        out_params.input_file = FLAGS_input_file;
        out_params.output_file = FLAGS_output_file;

        // Tests
        out_params.skip_all_tests = FLAGS_skip_all_tests;
        out_params.skip_runs_test = FLAGS_skip_runs_test;
        out_params.skip_poker_test = FLAGS_skip_poker_test;
        out_params.skip_monobit_test = FLAGS_skip_monobit_test;
        out_params.skip_long_run_test = FLAGS_skip_long_run_test;
        out_params.skip_continuous_run_test = FLAGS_skip_continuous_run_test;

        return validate_params(out_params);
    }

} // namespace fips
