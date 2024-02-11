#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <numeric>
#include <omp.h>

int main() {
    const int SIZE_EXPONENT = 28; // Size of array is 2^28
    const int SIZE = 1 << SIZE_EXPONENT; // Equivalent to 2^28
    std::vector<int> arr(SIZE);

    // Seed the random number generator
    std::srand(std::time(nullptr));

    // Fill the array with random integers
    #pragma omp parallel for
    for (int i = 0; i < SIZE; ++i) {
        arr[i] = std::rand() % 1000000000; // Random number between 0 and 10^9
    }

    // Set the desired number of threads
    int num_threads = 4; // Change this value as needed
    omp_set_num_threads(num_threads);

    // Measure execution time
    double start_time = omp_get_wtime();

    // Find the minimum element
    int minElement = arr[0];
    #pragma omp parallel for reduction(min:minElement)
    for (int i = 1; i < SIZE; ++i) {
        if (arr[i] < minElement) {
            minElement = arr[i];
        }
    }

    // Find the maximum element
    int maxElement = arr[0];
    #pragma omp parallel for reduction(max:maxElement)
    for (int i = 1; i < SIZE; ++i) {
        if (arr[i] > maxElement) {
            maxElement = arr[i];
        }
    }

    // Calculate the mean
    unsigned long long sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < SIZE; ++i) {
        sum += arr[i];
    }
    double mean = static_cast<double>(sum) / SIZE;

    // Output results
    std::cout << "Minimum: " << minElement << std::endl;
    std::cout << "Maximum: " << maxElement << std::endl;
    std::cout << "Mean: " << mean << std::endl;

    // Measure execution time
    double end_time = omp_get_wtime();
    double execution_time = end_time - start_time;
    std::cout << "Execution Time: " << execution_time << " seconds" << std::endl;

    return 0;
}