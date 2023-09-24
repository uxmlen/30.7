#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <utility>
#include <vector>

#include "request_handler.h"
bool make_thread = false;

RequestHandler pool;
void quicksort(int* array, int left, int right)
{
    if (left >= right)
        return;

    int left_bound = left;
    int right_bound = right;

    int middle = array[(left_bound + right_bound) / 2];

    do {
        while (array[left_bound] < middle) {
            left_bound++;
        }
        while (array[right_bound] > middle) {
            right_bound--;
        }
        if (left_bound <= right_bound) {
            std::swap(array[left_bound], array[right_bound]);
            left_bound++;
            right_bound--;
        }
    } while (left_bound <= right_bound);

    if (make_thread && (right_bound - left > 100000)) {
        pool.push_tasks(quicksort, array, left, right_bound);
        quicksort(array, left_bound, right);
    } else {
        // запускаем обе части синхронно
        quicksort(array, left, right_bound);
        quicksort(array, left_bound, right);
    }
}
void show_arr(int* arr, int arr_size)
{
    for (int i = 0; i < arr_size; i++) {
        std::cout << arr[i] << ' ';
    }
    std::cout << std::endl;
}

int main()
{
    std::srand(std::time(nullptr));

    int arr_size = 100;
    int* arr = new int[arr_size];
    // fill out array with random data
    for (int i = 0; i < arr_size; i++) {
        arr[i] = rand() / 100;
    }

    show_arr(arr, arr_size);
    std::cout << "start: " << std::endl;
    quicksort(arr, 0, arr_size - 1);

    std::cout << "end: " << std::endl;
    show_arr(arr, arr_size);
    return 0;
}
