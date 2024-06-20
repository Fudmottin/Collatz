#include <iostream>

inline uint64_t collatz(uint64_t seed) {
    uint64_t steps = 0;
    while (seed > 1) {
        while ((seed & 1) == 0) {
            ++steps;
            seed >>= 1;
        }
        if (seed > 1) {
            ++steps;
            seed = seed * 3 + 1;
        }
    }
    return steps;
}

int main () {
    std::cout << "Starting Collatz run...\n";

    for(uint64_t seed = 0; seed <= 1'000'000'000; ++seed) {
        auto steps = collatz(seed);
        if ((seed % 100'000) == 0)
            std::cout << "Seed: " << seed << " Steps: " << steps << "\r";
    }
    
    std::cout << "\nFinished!" << std::endl;

    return 0;
}

