#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <string>

#include "common.h"
#include "fips.h"
#include "params.h"

typedef struct
{
    std::string test;
    size_t num_runs;
    size_t num_failures;
} test_status;

bool check_fips_run(int result, std::array<test_status, N_FIPS_TESTS> &statuses)
{
    bool success = true;
    for (int test = 0; test < N_FIPS_TESTS; ++test)
    {
        ++statuses[test].num_runs;
        if (result & fips_test_mask[test])
        {
            success = false;
            ++statuses[test].num_failures;
            std::cout << "INFO: "
                      << fips_test_names[test]
                      << " failed: ("
                      << statuses[test].num_failures
                      << " out of "
                      << statuses[test].num_runs
                      << " runs)\n";
        }
    }

    if (success)
    {
        std::cout << "INFO: Run "
                  << statuses[0].num_runs
                  << " succeeded\n";
    }

    return success;
}

int main(int argc, char **argv)
{
    fips::test_params params{};

    if (!fips::process_params(&argc, argv, params))
    {
        return EXIT_FAILURE;
    }

    std::unique_ptr<fips_ctx> ctx = std::make_unique<fips_ctx>();
    fips_init(ctx.get(), 0);

    std::vector<unsigned char> input_file_data =
        fips::LoadFile<unsigned char>(
            params.input_file.c_str());

    const size_t input_size{input_file_data.size()};

    if (input_size < FIPS_RNG_BUFFER_SIZE)
    {
        std::cerr << "ERROR: Not enough input bytes ("
                  << input_size << "< " << FIPS_RNG_BUFFER_SIZE
                  << ")\n";
        return EXIT_FAILURE;
    }

    std::array<test_status, N_FIPS_TESTS> statuses{};

    for (size_t test = 0; test < N_FIPS_TESTS; ++test)
    {
        statuses[test] = test_status{fips_test_names[test], 0, 0};
    }

    for (size_t used_bytes = 0; used_bytes + FIPS_RNG_BUFFER_SIZE < input_size; used_bytes += FIPS_RNG_BUFFER_SIZE)
    {
        const void *ptx = input_file_data.data() + used_bytes;
        auto result = fips_run_rng_test(ctx.get(), ptx);
        check_fips_run(result, statuses);
    }

    return EXIT_SUCCESS;
}
