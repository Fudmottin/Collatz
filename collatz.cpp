#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using boost::multiprecision::cpp_int;

const int MILLERRABINITERATIONS = 40;

cpp_int powMod(cpp_int base, cpp_int exp, const cpp_int& mod) {
    cpp_int result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

bool millerRabinTest(const cpp_int& n) {
    if (n <= 1 || n == 4) return false;
    if (n <= 5) return true;

    cpp_int d = n - 1;
    while ((d & 1) == 0) {
        d >>= 1;
    }

    boost::random::mt19937_64 rng(std::random_device{}());
    for (int i = 0; i < MILLERRABINITERATIONS; i++) {
        cpp_int a = boost::random::uniform_int_distribution<cpp_int>(2, n - 2)(rng);
        cpp_int x = powMod(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool isPrime = false;
        for (cpp_int r = 1; r < d; r *= 2) {
            x = powMod(x, 2, n);
            if (x == 1) return false;
            if (x == n - 1) {
                isPrime = true;
                break;
            }
        }

        if (!isPrime) return false;
    }

    return true;
}

cpp_int findFactor(const cpp_int& n) {
    if ((n & 1) == 0)
        return cpp_int(2);
    if (millerRabinTest(n))
        return n;
    for (cpp_int factor = 3; factor * factor <= n; factor += 2)
        if (n % factor == 0)
            return factor;
    return n;
}

void primeFactorization(cpp_int n) {
    cpp_int fac;
    do {
        fac = findFactor(n);
        cpp_int exp = 0;

        if (fac == n) {
            std::cout << n;
            return;
        }

        do {
            ++exp;
            n /= fac;
        } while ((n % fac) == 0);

        std::cout << fac;

        if (exp > 1) {
            std::cout << "^";
            if (!millerRabinTest(exp)) {
                std::cout << "(";
                primeFactorization(exp);
                std::cout << ")";
            } else std::cout << exp;
        }

        if (n > 2) std::cout << " * ";
        std::cout.flush();
    } while (n > 1);
}


inline cpp_int collatz(const cpp_int& n) {
    if ((n & 1) == 0)
        return n / 2;
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

            // Load the dice. We will make the seed evenly divisible
            // by 3 but not by 2. This should result in a sequence that
            // only the seed is divisible by 3. 3n+1 can't be divisible
            // by 3. We will leave any powers of 2 remain and see what
            // happens. When the collatz function hits a power of 2, it
            // should dive straight to 1.
        
            seed += 3 - seed % 3; // Yeah, that's all we have to do.

            auto seq = runCollatz(seed);

            std::cout << "collatz:\n";
            for (auto n : seq) {
                std::cout << n << "\t\t\t\t\t=\t";
                primeFactorization(n);
                std::cout << "\n";
            }
            std::cout << std::endl;

            if (!prev_seq.empty()) {
                auto node = findNode(prev_seq, seq);
                nodes.insert(node);
            }
            prev_seq = seq;
        }

        std::cout << "nodes:\n";
        for (auto n : nodes) {
            std::cout << n << "\t\t\t\t\t=\t";
            primeFactorization(n);
            std::cout << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
    }

    std::cout << std::endl;

    return 0;
}

