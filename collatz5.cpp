#include <iostream>
#include <mutex>
#include <future>
#include "memo.h"

constexpr int collatz(uint64_t seed) {
    int steps = 0;

    while (seed > 1) {
        if (seed < 1'000'000) return steps + memo[seed];
        (seed & 1) ? seed = 3 * seed + 1 : seed >>= 1;
        ++steps;
    }

    return steps;
}

int main () {
    std::mutex m;
    unsigned num_threads = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;

    std::cout << "Starting Collatz run...\n";

    for (int i = 0; i < num_threads; ++i)
        futures.push_back(std::async(std::launch::async, [&, i] {
            for(int seed = i; seed <= 1'000'000'000; seed += num_threads) {
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
