#include <chrono>
#include <iostream>

#include "seq_sort.hpp"
#include "par_sort.hpp"
#include "test_generator.hpp"

int test_time_seq(int *arr, int size) {
    auto start = std::chrono::high_resolution_clock::now();
    seq_sort(arr, size);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

int test_time_par(int *arr, int size) {
    int *arr_copy_left = new int[size];
    int *arr_copy_mid = new int[size];
    int *arr_copy_right = new int[size];
    int *arr_sums_left = new int[size];
    int *arr_sums_mid = new int[size];
    int *arr_sums_right = new int[size];
    int *arr_result_left = new int[size];
    int *arr_result_mid = new int[size];
    int *arr_result_right = new int[size];
    auto start = std::chrono::high_resolution_clock::now();
    par_sort(arr, size, arr_copy_left, arr_copy_mid, arr_copy_right, arr_sums_left, arr_sums_mid, arr_sums_right,
        arr_result_left, arr_result_mid, arr_copy_right);
    auto end = std::chrono::high_resolution_clock::now();
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
    int sum_seq = 0;
    int sum_par = 0;
    for (int i = 0; i < t; ++i) {
        int *arr = new int[size];
        gen_test_random(arr, size);
        int *arr_copy = new int[size];
        for (int j = 0; j < size; ++j) {
            arr_copy[j] = arr[j];
        }
        int value_seq = test_time_seq(arr, size);
        int value_par = test_time_par(arr_copy, size);
        delete[] arr;
        delete[] arr_copy;
        sum_seq += value_seq;
        sum_par += value_par;
        std::cout << "test " << i + 1 << " seq time: " << value_seq << " ms; par time: " << value_par << " ms\n";
    }
    std::cout << "seq average time: " << sum_seq / t << " ms; par average time: " << sum_par / t << " ms\n";
    return 0;
}