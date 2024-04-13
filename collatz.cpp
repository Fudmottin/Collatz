#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using boost::multiprecision::cpp_int;

inline cpp_int collatz(const cpp_int& n) {
    if ((n & 1) == 0)
        return n >> 1;
    else
        return n * 3 + 1;
}

std::vector<cpp_int> runCollatz(const cpp_int& seed) {
    std::vector<cpp_int> seq;

    auto n = seed;
    do {
        seq.push_back(n);
        n = collatz(n);
    } while (n > 3);

    std::reverse(seq.begin(), seq.end());

    return seq;
}

cpp_int findNode(const std::vector<cpp_int>& vec1, const std::vector<cpp_int>& vec2) {
    auto it = std::mismatch(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    if (it.first == vec1.end() || it.second == vec2.end()) {
        // One vector is a prefix of the other, return 0 
        return 0; 
    } else {
        // Return the last common element
        return *(it.first - 1);
    }
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
    if (max_seed < 4) {
        std::cerr << "Second argument must be greater than 4." << std::endl;
        return -1;
    }

    try {
        std::vector<cpp_int> prev_seq {};
        std::set<cpp_int> nodes {};

        for (int i = 0; i < seeds; ++i) {
            boost::random::mt19937_64 rng(std::random_device {}());                         
            boost::multiprecision::cpp_int seed = 
            boost::random::uniform_int_distribution<cpp_int>
            (4, max_seed)(rng);

            // Load the dice. 3n+1 will always produce a number that is
            // not divisible by 3. We will simply make sure that the seeed
            // is also not divisible by 3.
            if (seed % 3 == 0) ++seed; // Yeah, that's all we have to do.

            auto seq = runCollatz(seed);
            long long odds = 0;
            long long evens = 0;

            for (auto n : seq)
                if ((n & 1) == 0) ++evens; else ++odds;

            std::cout << "collatz: " << seed << " " << seq.size() << " " << evens << " " << odds << "\n";
            for (auto n : seq)
                std::cout << n << "\n";
            std::cout << std::endl;

            if (!prev_seq.empty()) {
                auto node = findNode(prev_seq, seq);
                if (node != 0) nodes.insert(node);
            }
            prev_seq = seq;
        }

        std::cout << "nodes: " << nodes.size() << "\n";
        for (auto n : nodes)
            std::cout << n << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
    }

    std::cout << std::endl;

    return 0;
}

