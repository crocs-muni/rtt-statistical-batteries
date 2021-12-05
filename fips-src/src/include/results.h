#pragma once

#include <array>
#include <fstream>
#include <iostream>
#include <string>

#include "fips.h"
#include "params.h"

namespace fips
{
    typedef struct
    {
        std::string test;
        size_t num_runs;
        size_t num_failures;
    } test_status;

    void save_fips_run(int result, std::array<test_status, N_FIPS_TESTS> &statuses)
    {
        for (int test = 0; test < N_FIPS_TESTS; ++test)
        {
            ++statuses[test].num_runs;
            if (result & fips_test_mask[test])
            {
                ++statuses[test].num_failures;
            }
        }
    }

    void dump_test_result(std::ofstream &out, const test_status &status) {
        out << "Single Test Result:" << std::endl;
        out << status.test << std::endl;
        out << status.num_failures << " failed out of " << status.num_runs << std::endl;
        out << std::endl;
    }

    void generate_report(const fips::test_params &params, bool battery_success, const std::array<test_status, N_FIPS_TESTS> &statuses)
    {
        std::ofstream out(params.output_file);
        if (!out.is_open()) {
            std::cerr << "Error: Cannot open output file for writing." << std::endl;
            return;
        }

        out << "FIPS 140-2 battery: ";
        if (battery_success) {
            out << "Accepted";
        } else {
            out << "Rejected";
        }
        out << std::endl << std::endl;

        if (!params.skip_all_tests)
        {
            if (!params.skip_monobit_test)
            {
                dump_test_result(out, statuses[0]);
            }
            if (!params.skip_poker_test)
            {
                dump_test_result(out, statuses[1]);
            }
            if (!params.skip_runs_test)
            {
                dump_test_result(out, statuses[2]);
            }
            if (!params.skip_long_run_test)
            {
                dump_test_result(out, statuses[3]);
            }
            if (!params.skip_continuous_run_test)
            {
                dump_test_result(out, statuses[4]);
            }
        }
    }

} // namespace fips
