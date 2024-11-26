#include <chrono>
#include <iostream>

#include "sort.hpp"
#include "test_generator.hpp"

int test_time(const int size) {
    int *arr = new int[size];
    gen_test_random(arr, size);
    auto start = std::chrono::high_resolution_clock::now();
    sort(arr, size);
    auto end = std::chrono::high_resolution_clock::now();
    delete arr;
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int main() {
    const int t = 5;
    const int size = 1e8;
    int sum = 0;
    for (int i = 0; i < t; ++i) {
        int value = test_time(size);
        sum += value;
        std::cout << "launch " << i + 1 << " time: " << value << " ms\n";
    }
    std::cout << "average time: " << sum / t << " ms\n";
    return 0;
}