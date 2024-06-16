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
    for(uint64_t seed = 1; seed < 1'000'000'001; ++seed)
        std::cout << "Seed: " << seed << " Steps: " << collatz(seed) << "\r";
    
    std::cout << std::endl;

    return 0;
}

