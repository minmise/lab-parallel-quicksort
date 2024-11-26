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

bool test_simple_0() {
    // [] -> []
    const int size = 0;
    int arr[size] = {};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_1() {
    // [0] -> [0]
    const int size = 1;
    int arr[size] = {0};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_2() {
    // [1, 2, 3, 4, 5] -> [1, 2, 3, 4, 5]
    const int size = 5;
    int arr[size] = {1, 2, 3, 4, 5};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_3() {
    // [5, 4, 3, 2, 1] -> [1, 2, 3, 4, 5]
    const int size = 5;
    int arr[size] = {5, 4, 3, 2, 1};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_4() {
    // [3, 5, 4, 1, 2] -> [1, 2, 3, 4, 5]
    const int size = 5;
    int arr[size] = {3, 5, 4, 1, 2};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_5() {
    // [-1, 2, -3, 4, -5] -> [-5, -3, -1, 2, 4]
    const int size = 5;
    int arr[size] = {-1, 2, -3, 4, -5};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_6() {
    // [3, 2, 2, 2, 2, 2] -> [2, 2, 2, 2, 2, 3]
    const int size = 6;
    int arr[size] = {3, 2, 2, 2, 2, 2};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_7() {
    // [2, 2, 2, 2, 2, 1] -> [1, 2, 2, 2, 2, 2]
    const int size = 6;
    int arr[size] = {2, 2, 2, 2, 2, 1};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_8() {
    // [3, 2, 2, 2, 2, 2, 1] -> [1, 2, 2, 2, 2, 2, 3]
    const int size = 7;
    int arr[size] = {3, 2, 2, 2, 2, 2, 1};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_simple_9() {
    // [2, 2, 2, 2, 2] -> [2, 2, 2, 2, 2]
    const int size = 5;
    int arr[size] = {2, 2, 2, 2, 2};
    sort(arr, size);
    return test_arr_is_sorted(arr, size);
}

bool test_big_random() {
    // [...] (1e8) -> [...] (1e8) sorted
    const int size = 1e8;
    int *arr = new int[size];
    const int cnt = 1;
    for (int i = 0; i < cnt; ++i) {
        gen_test_random(arr, size);
        sort(arr, size);
        if (!test_arr_is_sorted(arr, size)) {
            return false;
        }
    }
    delete arr;
    return true;
}

int main() {
    assert(test_simple_0());
    assert(test_simple_1());
    assert(test_simple_2());
    assert(test_simple_3());
    assert(test_simple_4());
    assert(test_simple_5());
    assert(test_simple_6());
    assert(test_simple_7());
    assert(test_simple_8());
    assert(test_simple_9());
    assert(test_big_random());
    std::cout << "Correct!\n";
    return 0;    
}