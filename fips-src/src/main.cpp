#include <iostream>
#include <iomanip>
#include <memory>
#include <random>

#include "fips.h"

int main(int argc, char **argv)
{

    std::random_device rd{};

    std::vector<unsigned char> sequence(FIPS_RNG_BUFFER_SIZE);

    for (int i = 0; i < FIPS_RNG_BUFFER_SIZE; ++i)
    {
        sequence[i] = static_cast<unsigned char>(rand() % 255);
    }

    // constexpr std::array<unsigned char, FIPS_RNG_BUFFER_SIZE> rng_buffer{0};

    std::unique_ptr<fips_ctx> ctx = std::make_unique<fips_ctx>();
    fips_init(ctx.get(), 0);

    auto ret = fips_run_rng_test(ctx.get(), sequence.data());
    if (ret)
    {
        for (int i = 0; i < N_FIPS_TESTS; ++i)
        {
            if (ret & fips_test_mask[i])
            {
                std::cout << "FIPS test " << fips_test_names[i] << " failed" << std::endl;
            }
        }
    }

    std::cout << "Result: " << ret << std::endl;
    return 0;
}
