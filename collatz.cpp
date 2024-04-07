#include <iostream>
#include <vector>
#include <future>
#include <thread>
#include <algorithm>
#include <random>
#include <mutex>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using boost::multiprecision::cpp_int;

inline cpp_int collatz(const cpp_int& n) {
    if ((n & 1) == 0)
        return n / 2;
    else
        return n * 3 + 1;
}

std::vector<cpp_int> runCollatz(cpp_int seed) {
    std::vector<cpp_int> seq;

    for (auto n = seed; n > 3; n = collatz(n))
         seq.push_back(n);

    std::reverse(seq.begin(), seq.end());

    return seq;
}

int main (int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <seeds max_seed>" << std::endl;
        return -1;
    }

    int seeds;
    cpp_int max_seed;

    try {
        seeds = std::stoi(argv[1]);
        max_seed = cpp_int(argv[2]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return -1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Out of range: " << e.what() << std::endl;
        return -1;
    }

    if (seeds < 0) {
        std::cerr << "Seeds must be a non-negative integer." << std::endl;
        return -1;
    }
    if (max_seed <= 4) {
        std::cerr << "Second argument must be greater than 4." << std::endl;
        return -1;
    }

    unsigned int num_cores = std::thread::hardware_concurrency();
    std::vector<std::future<void>> futures;
    std::mutex cout_mutex;

    for (int i = 0; i < seeds; ++i) {
        futures.push_back(std::async(std::launch::async, [i, max_seed, &cout_mutex]() {
            boost::random::mt19937_64 rng(std::random_device {}());                         
            boost::multiprecision::cpp_int seed = 
                boost::random::uniform_int_distribution<cpp_int>
                (4, max_seed)(rng);

            auto seq = runCollatz(seed);

            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "collatz(" << seed << ")\n";
                for (auto n : seq) {
                    std::cout << n << "\n";
                }
                std::cout << std::endl;
            }

        }));
    }

    for (auto& future : futures)
        future.get();

    return 0;
}

