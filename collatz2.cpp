#include <iostream>
#include <random>
#include <set>
#include <utility>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using boost::multiprecision::cpp_int;
using SeedSteps = std::pair<cpp_int, uint64_t>;

SeedSteps superCollatz(const cpp_int& n) {
    uint64_t steps = 0;
    cpp_int m {n};

    while (m > 1) {
        while ((m & 1) == 0) {
            ++steps;
            m >>= 1;
        }
        if (m > 1) {
            ++steps;
            m = m * 3 + 1;
        }
    }

    return SeedSteps(n, steps);
}

std::set<SeedSteps> makeCollatzSequences(int seeds, const cpp_int& max_seed) {
    std::set<SeedSteps> seedSteps {};
    boost::random::mt19937_64 rng(std::random_device {}());                         

    for (int i = 0; i < seeds; ++i) {
        boost::multiprecision::cpp_int seed = 
        boost::random::uniform_int_distribution<cpp_int>
        (2, max_seed)(rng);

        seedSteps.insert(superCollatz(seed));
    }

    return seedSteps;
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

    if (seeds < 1) {
        std::cerr << "Seeds must be greater than zero." << std::endl;
        return -1;
    }
    if (max_seed < 2) {
        std::cerr << "Second argument must be greater than 1." << std::endl;
        return -1;
    }

    try {
        std::cout << "seed\tsteps\n";
        auto seedSteps = makeCollatzSequences(seeds, max_seed);
        uint64_t minVal = std::numeric_limits<uint64_t>::max();
        uint64_t maxVal = std::numeric_limits<uint64_t>::min();

        for (const auto& ss : seedSteps) {
            std::cout << ss.first << "\t" << ss.second << "\n";
            minVal = std::min(minVal, ss.second);
            maxVal = std::max(maxVal, ss.second);
        }

        std::cout << "\nMin setps: " << minVal << "\tMax steps: " << maxVal << std::endl;
        std::cout << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
    }

    std::cout << std::endl;

    return 0;
}

