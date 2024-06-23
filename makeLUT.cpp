#include <iostream>

constexpr int collatz(uint64_t seed) {
    int steps = 0;

    while (seed > 1) {
        (seed & 1) ? seed = 3 * seed + 1 : seed >>= 1;
        ++steps;
    }

    return steps;
}

int main() {
    std::cout << "#include<array>\n\n";
    std::cout << "constexpr std::array<int, 1'000'000> memo { ";

    for (int i = 0; i < 1'000'000; ++i) {
        std::cout << collatz(i);
        if (i < 999'999) std::cout << ", ";
        if ((i % 8) == 0 && (i > 1)) std::cout << "\n                                            ";
    }

    std::cout << " };\n";

    return 0;
}

