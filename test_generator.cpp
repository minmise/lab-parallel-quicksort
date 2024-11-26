#include <random>

#include "test_generator.hpp"

void gen_test_random(int *arr, int size) {
    for (int i = 0; i < size; ++i) {
        arr[i] = rand();
    }
}