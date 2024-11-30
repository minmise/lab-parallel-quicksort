#include <algorithm>
#include <cassert>
#include <iostream>
#include <omp.h>
#include <random>

#include "sort.hpp"

static int get_block_size() {
    static const int BLOCK_SIZE = 5e7;
    return BLOCK_SIZE;
}

static void seq_sort(int *arr, int size) {
    if (size <= 1) {
        return;
    }
    std::swap(arr[rand() % size], arr[size - 1]);
    int pivot = arr[size - 1];
    int left_pointer = 0;
    int left_pivot_eq = -1;
    int right_pointer = size - 2;
    int right_pivot_eq = size - 1;
    while (left_pointer <= right_pointer) {
        while (left_pointer < size && arr[left_pointer] < pivot) {
            ++left_pointer;
        }
        while (right_pointer > left_pointer && arr[right_pointer] > pivot) {
            --right_pointer;
        }
        if (left_pointer >= right_pointer) {
            break;
        }
        std::swap(arr[left_pointer], arr[right_pointer]);
        if (arr[left_pointer] == pivot) {
            std::swap(arr[++left_pivot_eq], arr[left_pointer]);
        }
        if (arr[right_pointer] == pivot) {
            std::swap(arr[--right_pivot_eq], arr[right_pointer]);
        }
        ++left_pointer;
        --right_pointer;
    }
    if (arr[left_pointer] == pivot && left_pointer < right_pivot_eq) {
        std::swap(arr[--right_pivot_eq], arr[left_pointer]);
    }
    right_pointer = left_pointer - 1;
    for (int i = 0; i <= left_pivot_eq; ++i, --right_pointer) {
        std::swap(arr[i], arr[right_pointer]);
    }
    for (int i = size - 1; i >= right_pivot_eq; --i, ++left_pointer) {
        std::swap(arr[i], arr[left_pointer]);
    }
    seq_sort(arr, right_pointer + 1);
    seq_sort(arr + left_pointer, size - left_pointer);
}

template<typename F1, typename F2>
static void fork2join(F1 &&func_lhs, F2 &&func_rhs) {
    #pragma omp task
    {
        func_lhs();
    }
    #pragma omp task
    {
        func_rhs();
    }
    #pragma omp taskwait
}

template<typename F>
static void map(int *arr, int size, F &&func) {
    #pragma omp taskloop
    for (int i = 0; i < size; ++i) {
        arr[i] = func(arr[i]);
    }
}

template<typename F>
static void down(int *sumLeft, int v, int *arr, int size, F &&func, int left) {
    if (size <= get_block_size()) {
        for (int i = 0; i < size; ++i) {
            arr[i] = func(left, arr[i]);
            left = arr[i];
        }
        return;
    }
    int mid = size / 2;
    fork2join([&]() {
        down(sumLeft, v * 2 + 1, arr, mid, func, left);
    }, [&]() {
        down(sumLeft, v * 2 + 2, arr + mid, size - mid, func, func(left, sumLeft[v * 2 + 1]));
    });
}

template<typename F>
static int up(int *sumLeft, int v, int *arr, int size, F &&func) {
    if (size <= get_block_size()) {
        sumLeft[v] = arr[0];
        for (int i = 1; i < size; ++i) {
            sumLeft[v] = func(sumLeft[v], arr[i]);
        }
        return sumLeft[v];
    }
    int mid = size / 2;
    int left, right;
    fork2join([&]() {
        left = up(sumLeft, v * 2 + 1, arr, mid, func);
    }, [&]() {
        right = up(sumLeft, v * 2 + 2, arr + mid, size - mid, func);
    });
    sumLeft[v] = func(left, right);
    return sumLeft[v];
}

template<typename F>
static void scan(int *arr, int size, F &&func, int neutral) {
    int *sumLeft = new int[size * 4];
    up(sumLeft, 0, arr, size, func);
    down(sumLeft, 0, arr, size, func, neutral);
    delete[] sumLeft;
}

template<typename P>
static void filter(int *arr, int size, P &&predicate, int &new_size, int *sums, int *result) {
    #pragma omp taskloop
    for (int i = 0; i < size; ++i) {
        sums[i] = arr[i];
    }
    // #pragma omp taskwait
    map(sums, size, predicate);
    scan(sums, size, [&](int lhs, int rhs) {
        return lhs + rhs;
    }, 0);
    new_size = sums[size - 1];
    #pragma omp taskloop
    for (int i = 0; i < size; ++i) {
        if ((i == 0 ? sums[i] == 1 : sums[i] != sums[i - 1])) {
            result[sums[i] - 1] = arr[i];
        }
    }
    // #pragma omp taskwait
    #pragma omp taskloop
    for (int i = 0; i < new_size; ++i) {
        arr[i] = result[i];
    }
    // #pragma omp taskwait
}

static void quicksort(int *arr, int size, int *arr_copy_left, int *arr_copy_mid, int *arr_copy_right,
int *arr_sums_left, int *arr_sums_mid, int *arr_sums_right, int *arr_result_left, int *arr_result_mid, int *arr_result_right) {
    if (size <= get_block_size()) {
        seq_sort(arr, size);
        return;
    }
    int pivot = arr[rand() % size];
    #pragma omp taskloop
    for (int i = 0; i < size; ++i) {
        arr_copy_left[i] = arr[i];
        arr_copy_mid[i] = arr[i];
        arr_copy_right[i] = arr[i];
    }
    int left_size, mid_size, right_size;
    //#pragma omp task shared(left_size)
    {
        filter(arr_copy_left, size, [&](int value) {
            return (value < pivot ? 1 : 0);
        }, left_size, arr_sums_left, arr_result_left);
    }
    //#pragma omp task shared(mid_size)
    {
        filter(arr_copy_mid, size, [&](int value) {
            return (value == pivot ? 1 : 0);
        }, mid_size, arr_sums_mid, arr_result_mid);
    }
    //#pragma omp task shared(right_size)
    {
        filter(arr_copy_right, size, [&](int value) {
            return (value > pivot ? 1 : 0);
        }, right_size, arr_sums_right, arr_result_right);
    }
    //#pragma omp taskwait
    #pragma omp taskloop
    for (int i = 0; i < size; ++i) {
        int index = i;
        if (index < left_size) {
            arr[i] = arr_copy_left[index];
        } else {
            index -= left_size;
            if (index < mid_size) {
                arr[i] = arr_copy_mid[index];
            } else {
                index -= mid_size;
                arr[i] = arr_copy_right[index];
            }
        }
    }
    // #pragma omp taskwait
    int delta = left_size + mid_size;
    fork2join([&]() {
        quicksort(arr, left_size, arr_copy_left, arr_copy_mid, arr_copy_right,
            arr_sums_left, arr_sums_mid, arr_sums_right,
            arr_result_left, arr_result_mid, arr_result_right);
    }, [&]() {
        quicksort(arr + delta, right_size, arr_copy_left + delta, arr_copy_mid + delta, arr_copy_right + delta,
            arr_sums_left + delta, arr_sums_mid + delta, arr_sums_right + delta,
            arr_result_left + delta, arr_result_mid + delta, arr_result_right + delta);
    });
}

void sort(int *arr, int size, int *arr_copy_left, int *arr_copy_mid, int *arr_copy_right,
int *arr_sums_left, int *arr_sums_mid, int *arr_sums_right, int *arr_result_left, int *arr_result_mid, int *arr_result_right) {
    static const int NUM_THREADS = 4;
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        #pragma omp single
        {
            quicksort(arr, size, arr_copy_left, arr_copy_mid, arr_copy_right, arr_sums_left, arr_sums_mid, arr_sums_right,
                arr_result_left, arr_result_mid, arr_result_right);
        }
    }

}