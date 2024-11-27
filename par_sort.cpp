#include <algorithm>
#include <iostream>
#include <omp.h>
#include <random>

#include "sort.hpp"

static int get_block_size() {
    static const int BLOCK_SIZE = 4;
    return BLOCK_SIZE;
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
    if (size <= get_block_size()) {
        for (int i = 0; i < size; ++i) {
            arr[i] = func(arr[i]);
        }
        return;
    }
    int mid = size / 2;
    fork2join([&](void) {
        map(arr, mid, func);
    }, [&](void) {
        map(arr + mid, size - mid, func);
    });
}

template<typename F>
static void down(int *sumLeft, int v, int *arr, int size, F &&func, int left) {
    if (size == 1) {
        arr[0] = func(left, arr[0]);
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
    if (size == 1) {
        sumLeft[v] = arr[0];
        return arr[0];
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
static void filter(int *arr, int size, P &&predicate, int &new_size) {
    int *sums = new int[size];
    #pragma omp taskloop
    for (int i = 0; i < size; ++i) {
        sums[i] = arr[i];
    }
    map(sums, size, predicate);
    scan(sums, size, [&](int lhs, int rhs) {
        return lhs + rhs;
    }, 0);
    new_size = sums[size - 1];
    int *result = new int[new_size];
    #pragma omp taskloop
    for (int i = 0; i < size; ++i) {
        if (i == 0 && sums[i] == 1 || sums[i] != sums[i - 1]) {
            result[sums[i] - 1] = arr[i];
        }
    }
    delete[] sums;
    #pragma omp taskloop
    for (int i = 0; i < new_size; ++i) {
        arr[i] = result[i];
    }
    delete[] result;
}

static void quicksort(int *arr, int size, int *arr_copy_left, int *arr_copy_mid, int *arr_copy_right) {
    if (size <= get_block_size()) {
        std::sort(arr, arr + size);
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
    filter(arr_copy_left, size, [&](int value) {
        return (value < pivot ? 1 : 0);
    }, left_size);
    filter(arr_copy_mid, size, [&](int value) {
        return (value == pivot ? 1 : 0);
    }, mid_size);
    filter(arr_copy_right, size, [&](int value) {
        return (value > pivot ? 1 : 0);
    }, right_size);
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
    fork2join([&]() {
        quicksort(arr, left_size, arr_copy_left, arr_copy_mid, arr_copy_right);
    }, [&]() {
        quicksort(arr + left_size + mid_size, right_size, arr_copy_left, arr_copy_mid, arr_copy_right);
    });
}

void sort(int *arr, int size) {
    static const int NUM_THREADS = 4;
    int *arr_copy_left = new int[size];
    int *arr_copy_mid = new int[size];
    int *arr_copy_right = new int[size];
    #pragma omp parallel num_threads(NUM_THREADS)
    {
        #pragma omp single
        {
            quicksort(arr, size, arr_copy_left, arr_copy_mid, arr_copy_right);
        }
    }
    delete[] arr_copy_left;
    delete[] arr_copy_mid;
    delete[] arr_copy_right;
}