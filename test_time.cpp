#include <chrono>
#include <iostream>

#include "sort.hpp"
#include "test_generator.hpp"

int test_time(const int size) {
    int *arr = new int[size];
    int *arr_copy_left = new int[size];
    int *arr_copy_mid = new int[size];
    int *arr_copy_right = new int[size];
    int *arr_sums_left = new int[size];
    int *arr_sums_mid = new int[size];
    int *arr_sums_right = new int[size];
    int *arr_result_left = new int[size];
    int *arr_result_mid = new int[size];
    int *arr_result_right = new int[size];
    gen_test_random(arr, size);
    auto start = std::chrono::high_resolution_clock::now();
    sort(arr, size, arr_copy_left, arr_copy_mid, arr_copy_right, arr_sums_left, arr_sums_mid, arr_sums_right,
        arr_result_left, arr_result_mid, arr_copy_right);
    auto end = std::chrono::high_resolution_clock::now();
    delete[] arr;
    delete[] arr_copy_left;
    delete[] arr_copy_mid;
    delete[] arr_copy_right;
    delete[] arr_sums_left;
    delete[] arr_sums_mid;
    delete[] arr_sums_right;
    delete[] arr_result_left;
    delete[] arr_result_mid;
    delete[] arr_result_right;
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