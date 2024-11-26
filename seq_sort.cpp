#include <algorithm>
#include <random>

#include "sort.hpp"

void quicksort(int *arr, int size) {
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
    ++left_pointer;
    for (int i = 0; i <= left_pivot_eq; ++i, --right_pointer) {
        std::swap(arr[i], arr[right_pointer]);
    }
    for (int i = size - 1; i >= right_pivot_eq; --i, ++left_pointer) {
        std::swap(arr[i], arr[left_pointer]);
    }
    quicksort(arr, right_pointer + 1);
    quicksort(arr + left_pointer, size - left_pointer);
}

void sort(int *arr, int size) {

}