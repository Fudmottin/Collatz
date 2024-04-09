#include <iostream>
#include <vector>
#include <set>
#include <future>
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

cpp_int findNode(const std::vector<cpp_int>& vec1, const std::vector<cpp_int>& vec2) {
    auto it = std::mismatch(vec1.begin(), vec1.end(), vec2.begin(), vec2.end());
    if (it.first == vec1.end() || it.second == vec2.end()) {
        // One vector is a prefix of the other, return the last element of the shorter vector
        return vec1.size() < vec2.size() ? vec1.back() : vec2.back();
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
    if (max_seed <= 4) {
        std::cerr << "Second argument must be greater than 4." << std::endl;
        return -1;
    }

    std::vector<std::future<void>> futures;
    std::mutex cout_mutex, nodes_mutex;
    std::vector<cpp_int> prev_seq {};
    std::set<cpp_int> nodes {};

    for (int i = 0; i < seeds; ++i) {
        futures.push_back(std::async(std::launch::async, [&]() {
            boost::random::mt19937_64 rng(std::random_device {}());                         
            boost::multiprecision::cpp_int seed = 
                boost::random::uniform_int_distribution<cpp_int>
                (4, max_seed - 3)(rng);

            seed += 3 - seed % 3;        // This forces the seed to be a leaf node
            while ((seed & 1) == 0)      // by making it evenly divisible by 3 but
                seed >>= 1;              // not by 2. No 3n+1 number will be evenly
                                         // divisible by 3.

            auto seq = runCollatz(seed);

            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "collatz:\n";
                for (auto n : seq)
                    std::cout << n << "\n";
                std::cout << std::endl;
            }
            {
                std::lock_guard<std::mutex> lock(nodes_mutex);
                if (!prev_seq.empty()) {
                    auto node = findNode(prev_seq, seq);
                    nodes.insert(node);
                }
                prev_seq = seq;
            }

        }));
    }

    for (auto& future : futures)
        future.get();

    std::cout << "nodes:\n";
    for (auto n : nodes)
        std::cout << n << "\n";
    std::cout << std::endl;


    return 0;
}

