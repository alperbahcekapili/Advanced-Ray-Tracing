#include <iostream>
#include <vector>
#include <chrono>

const int N = 1'000'000;

void benchmarkVector() {
    std::vector<int> vec(N, 0);
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        vec[i] = i; // Example operation
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken for vector: " << duration.count() << " seconds\n";
}

void benchmarkArray() {
    int arr[N] = {0};
    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < N; ++i) {
        arr[i] = i; // Example operation
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken for array: " << duration.count() << " seconds\n";
}

int main() {
    benchmarkVector();
    benchmarkArray();
    return 0;
}
