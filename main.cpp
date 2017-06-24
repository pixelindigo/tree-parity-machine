#include <cstdio>
#include <iostream>
#include "machine.h"

double sync_status(const std::vector<int>& alice,
                    const std::vector<int>& bob) {
    double matching = 0;
    for (size_t i = 0; i != alice.size(); ++i) {
        if (alice[i] == bob[i]) { ++matching; }
    }
    return (matching / alice.size()) * 100.0;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "Usage: " << argv[0] << " key_size multiplier key_bounds\n";
        return 0;
    }
    size_t size, multiplier, bounds;
    sscanf(argv[1], "%zu", &size);
    sscanf(argv[2], "%zu", &multiplier);
    sscanf(argv[3], "%zu", &bounds);
    // Using mersenne_twister_engine seeded with rd()
    std::random_device rd;
    std::mt19937 generator(rd());
    // Generates true with p = 0.5 and false with the same 1-p = 0.5
    std::bernoulli_distribution distribution(0.5);
    TreeParityMachine alice(size, multiplier, bounds),
                      bob(size, multiplier, bounds);
    std::vector<int> input(size * multiplier);
    size_t epochs = 0; 
    while (alice.get_key() != bob.get_key()) {
        std::cout << "\rSynchronization: " << sync_status(alice.get_key(), bob.get_key()) << "% "
            << "Iterations: " << epochs;
        ++epochs;
        std::generate(input.begin(), input.end(),
                [&](){ return distribution(generator) ? 1 : -1; });

        int output_alice = alice.compute(input);
        int output_bob = bob.compute(input);
        if (output_alice == output_bob) {
            alice.sync(input, output_alice);
            bob.sync(input, output_bob);
        }
    }
    std::cout << "\nSync completed!\n";
    std::cout << "Key: ";
    for (int value : alice.get_key()) {
        std::cout << (value + bounds) << " ";
    }
    std::cout << "\n";
    return 0;
}
