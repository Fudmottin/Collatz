#include <iostream>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

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
    std::vector<std::future<void>> futures;
    std::mutex io_mutex;

    for(uint64_t seed = 0; seed < 1'000'000'000; ++seed)
        futures.push_back(std::async(std::launch::async, [&, seed] {
            auto steps = collatz(seed);
            {
                std::scoped_lock lock(io_mutex); 
                std::cout << "Seed: " << seed << " Steps: " << steps << "\r";
            }
        }));
    
    std::cout << std::endl;

    return 0;
}

