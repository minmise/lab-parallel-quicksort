#include <algorithm>
#include <cassert>
#include <iostream>
#include <omp.h>
#include <random>

#include "seq_sort.hpp"
#include "par_sort.hpp"

static int get_block_size() {
    static const int BLOCK_SIZE = 50'000'000;
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
    #pragma omp taskloop 
    for (int i = 0; i < size; ++i) {
        arr[i] = func(arr[i]);
    }
}

static void scan(int *arr, int size) {
    int blocked_size = (size + get_block_size() - 1) / get_block_size();
    int *sums = new int[blocked_size];
    #pragma omp taskloop 
    for (int block = 0; block < blocked_size; ++block) {
        sums[block] = 0;
        for (int i = block * get_block_size(); i < size && i < (block + 1) * get_block_size(); ++i) {
            sums[block] += arr[i];
        }
    }
    for (int i = 1; i < blocked_size; ++i) {
        sums[i] += sums[i - 1];
    }
    #pragma omp taskloop 
    for (int block = 0; block < blocked_size; ++block) {
        if (block != 0) {
            arr[block * get_block_size()] += sums[block - 1];
        }
        for (int i = block * get_block_size() + 1; i < size && i < (block + 1) * get_block_size(); ++i) {
            arr[i] += arr[i - 1];
        }
    }
    delete[] sums;
}

template<typename P>
static void filter(int *arr, int size, P &&predicate, int &new_size, int *sums, int *result) {
    #pragma omp taskloop 
    for (int i = 0; i < size; ++i) {
        sums[i] = arr[i];
    }
    map(sums, size, predicate);
    scan(sums, size);
    new_size = sums[size - 1];
    #pragma omp taskloop
    for (int i = 0; i < size; ++i) {
        if ((i == 0 ? sums[i] == 1 : sums[i] != sums[i - 1])) {
            result[sums[i] - 1] = arr[i];
        }
    }
    #pragma omp taskloop 
    for (int i = 0; i < new_size; ++i) {
        arr[i] = result[i];
    }
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

void par_sort(int *arr, int size, int *arr_copy_left, int *arr_copy_mid, int *arr_copy_right,
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