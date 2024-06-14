#include <iostream>

inline void collatz(uint64_t seed, uint64_t prev_seed) {
    while ((seed > 1) && (seed > prev_seed)) {
        while ((seed & 1) == 0) {
            seed >>= 1;
        }
        if (seed > 1) {
            seed = seed * 3 + 1;
        }
    }
}

int main () {
    for(uint64_t seed = 1; seed < 1'000'000'001; ++seed) {
        collatz(seed, seed - 1);
        std::cout << "Seed: " << seed << "\r";
    }
    
    std::cout << std::endl;

    return 0;
}

