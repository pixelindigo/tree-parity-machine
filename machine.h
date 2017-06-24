#include <algorithm>
#include <random>
#include <vector>

using std::vector;

class TreeParityMachine {
 private:
    size_t length;
    size_t size;
    int key_bounds;
    vector<int> key;
    vector<int> hidden_neurons;
 public:
    TreeParityMachine(size_t length, size_t size, int key_bounds) :
        length(length),
        size(size),
        key_bounds(key_bounds),
        key(length * size),
        hidden_neurons(length) {
        // Using mersenne_twister_engine seeded with rd()
        std::random_device rd;
        std::mt19937 generator(rd());
        // The weights will be initialised to [-key_bounds;key_bounds] range
        std::uniform_int_distribution<> distribution(-key_bounds, key_bounds);
        // Init key to random values
        std::generate(key.begin(), key.end(), [&](){ return distribution(generator); });
    }

    int compute(const vector<int>& input) {
        int result = 1;
        for (size_t i = 0; i != length; ++i) {
            hidden_neurons[i] = 0;
            for (size_t j = 0; j != size; ++j) {
                hidden_neurons[i] += key[i*size + j] * input[i*size + j];
            }
            hidden_neurons[i] = hidden_neurons[i] > 0 ? 1 : -1;
            result *= hidden_neurons[i];
        }

        return result;
    }

    // Using Hebbian rule
    void sync(const vector<int>& input, int result) {
        for (size_t i = 0; i != length; ++i) {
            if (hidden_neurons[i] == result) {
                for (size_t j = 0; j != size; ++j) {
                    key[i*size + j] += input[i*size + j] * result; 
                    // Bounds fixup
                    if (key[i*size + j] > key_bounds) {
                        key[i*size + j] = key_bounds;
                    } else if (key[i*size + j] < -key_bounds) {
                        key[i*size + j] = -key_bounds;
                    }
                }
            }
        }
    }

    const vector<int>& get_key() const {
        return key;
    }
    
};
