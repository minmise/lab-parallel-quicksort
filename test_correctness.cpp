#include <cassert>
#include <iostream>

#include "par_sort.hpp"
#include "seq_sort.hpp"
#include "test_generator.hpp"

bool test_arr_is_sorted(int *arr, int size) {
    for (int i = 1; i < size; ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

void par_sort_launch(int *arr, int size) {
    int *arr_copy_left = new int[size];
    int *arr_copy_mid = new int[size];
    int *arr_copy_right = new int[size];
    int *arr_sums_left = new int[size];
    int *arr_sums_mid = new int[size];
    int *arr_sums_right = new int[size];
    int *arr_result_left = new int[size];
    int *arr_result_mid = new int[size];
    int *arr_result_right = new int[size];
    par_sort(arr, size, arr_copy_left, arr_copy_mid, arr_copy_right, arr_sums_left, arr_sums_mid, arr_sums_right,
        arr_result_left, arr_result_mid, arr_copy_right);
    delete[] arr_copy_left;
    delete[] arr_copy_mid;
    delete[] arr_copy_right;
    delete[] arr_sums_left;
    delete[] arr_sums_mid;
    delete[] arr_sums_right;
    delete[] arr_result_left;
    delete[] arr_result_mid;
    delete[] arr_result_right;    
}

void test_sorts(int *arr, int size) {
    int *arr_copy = new int[size];
    for (int i = 0; i < size; ++i) {
        arr_copy[i] = arr[i];
    }
    seq_sort(arr, size);
    assert(test_arr_is_sorted(arr, size));
    par_sort_launch(arr_copy, size);
    assert(test_arr_is_sorted(arr_copy, size));
    delete[] arr_copy;
}

void test_simple_0() {
    // [] -> []
    const int size = 0;
    int arr[size] = {};
    test_sorts(arr, size);
}

void test_simple_1() {
    // [0] -> [0]
    const int size = 1;
    int arr[size] = {0};
    test_sorts(arr, size);
}

void test_simple_2() {
    // [1, 2, 3, 4, 5] -> [1, 2, 3, 4, 5]
    const int size = 5;
    int arr[size] = {1, 2, 3, 4, 5};
    test_sorts(arr, size);
}

void test_simple_3() {
    // [5, 4, 3, 2, 1] -> [1, 2, 3, 4, 5]
    const int size = 5;
    int arr[size] = {5, 4, 3, 2, 1};
    test_sorts(arr, size);
}

void test_simple_4() {
    // [3, 5, 4, 1, 2] -> [1, 2, 3, 4, 5]
    const int size = 5;
    int arr[size] = {3, 5, 4, 1, 2};
    test_sorts(arr, size);
}

void test_simple_5() {
    // [-1, 2, -3, 4, -5] -> [-5, -3, -1, 2, 4]
    const int size = 5;
    int arr[size] = {-1, 2, -3, 4, -5};
    test_sorts(arr, size);
}

void test_simple_6() {
    // [3, 2, 2, 2, 2, 2] -> [2, 2, 2, 2, 2, 3]
    const int size = 6;
    int arr[size] = {3, 2, 2, 2, 2, 2};
    test_sorts(arr, size);
}

void test_simple_7() {
    // [2, 2, 2, 2, 2, 1] -> [1, 2, 2, 2, 2, 2]
    const int size = 6;
    int arr[size] = {2, 2, 2, 2, 2, 1};
    test_sorts(arr, size);
}

void test_simple_8() {
    // [3, 2, 2, 2, 2, 2, 1] -> [1, 2, 2, 2, 2, 2, 3]
    const int size = 7;
    int arr[size] = {3, 2, 2, 2, 2, 2, 1};
    test_sorts(arr, size);
}

void test_simple_9() {
    // [2, 2, 2, 2, 2] -> [2, 2, 2, 2, 2]
    const int size = 5;
    int arr[size] = {2, 2, 2, 2, 2};
    test_sorts(arr, size);
}

void print_arr(int *arr, int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

void test_big_random() {
    // [...] (1e8) -> [...] (1e8) sorted
    const int size = 1e8;
    int *arr = new int[size];
    const int cnt = 1;
    for (int i = 0; i < cnt; ++i) {
        gen_test_random(arr, size);
        test_sorts(arr, size);
    }
    delete[] arr;
}

int main() {
    test_simple_0();
    test_simple_1();
    test_simple_2();
    test_simple_3();
    test_simple_4();
    test_simple_5();
    test_simple_6();
    test_simple_7();
    test_simple_8();
    test_simple_9();
    test_big_random();
    std::cout << "Correct!\n";
    return 0;    
}