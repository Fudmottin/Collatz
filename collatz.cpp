#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <random>
#include <utility>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

using boost::multiprecision::cpp_int;
using NodePair = std::pair<cpp_int, cpp_int>;
using Nodes = std::set<cpp_int>;
using NodePairs = std::set<NodePair>;

// Define a comparator for std::vector<cpp_int>
struct VectorCompare {
    bool operator()(const std::vector<cpp_int>& lhs, const std::vector<cpp_int>& rhs) const {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }
};

using CollatzSequences = std::set<std::vector<cpp_int>, VectorCompare>;

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

CollatzSequences makeCollatzSequences(int seeds, const cpp_int& max_seed) {
    CollatzSequences collatzSequences {};
    boost::random::mt19937_64 rng(std::random_device {}());                         

    for (int i = 0; i < seeds; ++i) {
        boost::multiprecision::cpp_int seed = 
        boost::random::uniform_int_distribution<cpp_int>
        (4, max_seed)(rng);

        // Load the dice. 3n+1 will always produce a number that is
        // not divisible by 3. We will simply make sure that the seeed
        // is also not divisible by 3.
        if (seed % 3 == 0) ++seed; // Yeah, that's all we have to do.

        auto seq = runCollatz(seed);
        collatzSequences.insert(seq);
    }

    return collatzSequences;
}

Nodes makeNodes(const CollatzSequences& collatzSequences) {
    Nodes nodes {};

    if (!collatzSequences.empty()) {
        auto it1 = collatzSequences.begin();
        while (it1 != std::prev(collatzSequences.end())) {
            auto it2 = std::next(it1);
            while (it2 != collatzSequences.end()) {
                auto node = findNode(*it1, *it2);
                if (node != 0) nodes.insert(node);
                ++it2;
            }
            ++it1;
        }
    }

    return nodes; 
}

NodePairs makeNodePairs(const Nodes& nodes) {
    NodePairs pairs {};
    for (auto it = nodes.rbegin(); it != nodes.rend(); ++it) {
        cpp_int parent = collatz(*it);
        do {
            auto ptr = nodes.find(parent);
            if (ptr != nodes.end()) {
                pairs.insert(NodePair(parent, *it));
                break;
            }
            parent = collatz(parent);
        } while (parent > 4);
    }
    return pairs;
}

void printCollatzSequence(const std::vector<cpp_int>& seq) {
    long long odds = 0;
    long long evens = 0;

    if (seq.empty()) return;

    for (auto n : seq)
        if ((n & 1) == 0) ++evens; else ++odds;

    std::cout << "collatz: " << seq.back() << " " << seq.size() << " " << evens << " " << odds << "\n";
    for (auto n : seq)
        std::cout << n << "\n";
    std::cout << std::endl;
}

void printNodes(const Nodes& nodes) {
    if (nodes.empty()) return;
    std::cout << "nodes: " << nodes.size() << "\n";
    for (auto n : nodes)
        std::cout << n << "\n";
    std::cout << std::endl;
}

void printHierarchy(const NodePairs& pairs) {
    if (pairs.empty()) return;
    std::cout << "hierarchy:\n";
    for (auto p : pairs)
        std::cout << p.first << " " << p.second << "\n";
    std::cout << std::endl;
}

void createDOTFile(const NodePairs& pairs) {
    // Print the tree in DOT format
    std::ofstream dotFile("tree.dot");
    if (dotFile.is_open()) {
        dotFile << "digraph Tree {\n";
        for (const auto& [parent, child] : pairs) {
            dotFile << "  " << parent << " -> " << child << ";\n";
        }
        dotFile << "}\n";
        dotFile.close();
        std::cout << "DOT file generated successfully." << std::endl;
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

void printSequences(const CollatzSequences& collatzSequences) {
    if (!collatzSequences.empty())
        for (const auto& seq : collatzSequences)
            printCollatzSequence(seq);
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
    if (max_seed < 4) {
        std::cerr << "Second argument must be greater than 4." << std::endl;
        return -1;
    }

    try {
        auto collatzSequences = makeCollatzSequences(seeds, max_seed);
        auto nodes = makeNodes(collatzSequences);
        auto hierarchy = makeNodePairs(nodes);

        std::cout << "collatz sequence data v1\n\n";
        printNodes(nodes);
        printHierarchy(hierarchy);
        printSequences(collatzSequences);
        createDOTFile(hierarchy);
    }
    catch (const std::exception& e) {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
    }

    std::cout << std::endl;

    return 0;
}

