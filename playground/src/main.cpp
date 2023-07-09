#include "playground.hpp"

int main()
{
    spk::RandomGenerator<float> rng;

    rng.setDistributionRange(0.0f, 100.0f);
    
    for (size_t i = 0; i < 15; i++)
    {
        spk::cout << "RNG[" << i << "] -> " << rng() << std::endl;
    }

    return (0);
}
