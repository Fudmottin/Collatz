#include <iostream>
#include <future>
#include <mutex>

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
    std::mutex m;
    unsigned num_threads = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;

    std::cout << "Starting Collatz...\n";

    for (int i = 0; i < num_threads; ++i)
        futures.push_back(std::async(std::launch::async, [&] {
            for(uint64_t seed = 0; seed <= 1'000'000'000; seed += num_threads) {
                auto steps = collatz(seed);
                if ((seed % 100'000) == 0) {
                    std::lock_guard<std::mutex> lk(m);
                    std::cout << "Seed: " << seed << " Steps: " << steps << "\r";
                }
            }
        }));

    for(auto& f : futures)
        f.wait();

    std::cout << "\nFinished!" << std::endl;

    return 0;
}

