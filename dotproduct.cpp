#include <iostream>
#include <vector>
#include <omp.h>
#include <chrono>

// Function to calculate the dot product of two vectors
int dot_product(const std::vector<int>& vector_a, const std::vector<int>& vector_b) {
    int product = 0;
#pragma omp parallel for reduction(+:product)
    for (int i = 0; i < vector_a.size(); i++) {
        product += vector_a[i] * vector_b[i];
    }
    return product;
}

int main() {
    // Initialize two vectors with 109 random integers from the set {-1, 0, 1}
    std::vector<int> vector_a(109, 0);
    std::vector<int> vector_b(109, 0);
    for (int i = 0; i < 109; i++) {
        vector_a[i] = rand() % 3 - 1;
        vector_b[i] = rand() % 3 - 1;
    }

    // Measure the time taken for the compilation
    auto start_time = std::chrono::high_resolution_clock::now();

    // Set the number of threads
    #ifdef _OPENMP
    omp_set_num_threads(5); // Set the number of threads to 4
    #endif

    // Calculate the dot product of the two vectors
    int result = dot_product(vector_a, vector_b);

    // Measure the time taken for the compilation
    auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the time taken for the compilation
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);

    // Print the result and the time taken for the compilation
    std::cout << "Dot product: " << result << std::endl;
    std::cout << "Time taken for compilation: " << duration.count() << " nanoseconds" << std::endl;

    return 0;
}