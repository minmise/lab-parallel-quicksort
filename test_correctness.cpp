#include <cassert>
#include <iostream>

#include "sort.hpp"
#include "test_generator.hpp"

bool test_arr_is_sorted(int *arr, int size) {
    for (int i = 1; i < size; ++i) {
        if (arr[i - 1] > arr[i]) {
            return false;
        }
    }
    return true;
}

void get_sort(int *arr, int size) {
    int *arr_copy_left = new int[size];
    int *arr_copy_mid = new int[size];
    int *arr_copy_right = new int[size];
    int *arr_sums_left = new int[size];
    int *arr_sums_mid = new int[size];
    int *arr_sums_right = new int[size];
    int *arr_result_left = new int[size];
    int *arr_result_mid = new int[size];
    int *arr_result_right = new int[size];
    sort(arr, size, arr_copy_left, arr_copy_mid, arr_copy_right, arr_sums_left, arr_sums_mid, arr_sums_right,
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

bool test_simple_0() {
    // [] -> []
    const int size = 0;
    int arr[size] = {};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_1() {
    // [0] -> [0]
    const int size = 1;
    int arr[size] = {0};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_2() {
    // [1, 2, 3, 4, 5] -> [1, 2, 3, 4, 5]
    const int size = 5;
    int arr[size] = {1, 2, 3, 4, 5};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_3() {
    // [5, 4, 3, 2, 1] -> [1, 2, 3, 4, 5]
    const int size = 5;
    int arr[size] = {5, 4, 3, 2, 1};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_4() {
    // [3, 5, 4, 1, 2] -> [1, 2, 3, 4, 5]
    const int size = 5;
    int arr[size] = {3, 5, 4, 1, 2};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_5() {
    // [-1, 2, -3, 4, -5] -> [-5, -3, -1, 2, 4]
    const int size = 5;
    int arr[size] = {-1, 2, -3, 4, -5};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_6() {
    // [3, 2, 2, 2, 2, 2] -> [2, 2, 2, 2, 2, 3]
    const int size = 6;
    int arr[size] = {3, 2, 2, 2, 2, 2};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_7() {
    // [2, 2, 2, 2, 2, 1] -> [1, 2, 2, 2, 2, 2]
    const int size = 6;
    int arr[size] = {2, 2, 2, 2, 2, 1};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_8() {
    // [3, 2, 2, 2, 2, 2, 1] -> [1, 2, 2, 2, 2, 2, 3]
    const int size = 7;
    int arr[size] = {3, 2, 2, 2, 2, 2, 1};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_9() {
    // [2, 2, 2, 2, 2] -> [2, 2, 2, 2, 2]
    const int size = 5;
    int arr[size] = {2, 2, 2, 2, 2};
    get_sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

void print_arr(int *arr, int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << "\n";
}

bool test_big_random() {
    // [...] (1e8) -> [...] (1e8) sorted
    const int size = 100000;
    int *arr = new int[size];
    //int arr[size] = {49, 21, 62, 27, 90, 59, 63, 26};
    const int cnt = 1;
    for (int i = 0; i < cnt; ++i) {
        gen_test_random(arr, size);
        //print_arr(arr, size);
        get_sort(arr, size);
        //print_arr(arr, size);
        if (!test_arr_is_sorted(arr, size)) {
            return false;
        }
    }
    delete[] arr;
    return true;
}

int main() {
    /*assert(test_simple_0());
    assert(test_simple_1());
    assert(test_simple_2());
    assert(test_simple_3());
    assert(test_simple_4());
    assert(test_simple_5());
    assert(test_simple_6());
    assert(test_simple_7());
    assert(test_simple_8());
    assert(test_simple_9());*/
    assert(test_big_random());
    std::cout << "Correct!\n";
    return 0;    
}