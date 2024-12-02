#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <functional>  // std::ref
#include <ctime>

#include "../include/global.h"

#define N 1000  // matrix size
#define P 15  // threads count
#define RESULT_FILE_NAME "./data/result3_1000.txt"
#define MATRIX1_FILE_NAME "./data/matrix1_1000.txt"
#define MATRIX2_FILE_NAME "./data/matrix2_1000.txt"

int main() {
    std::vector<std::vector<int>> matrix1;
    std::vector<std::vector<int>> matrix2;
    std::vector<std::vector<int>> result_matrix(N, std::vector<int>(N, 0));
    std::vector<std::thread> threads;
    std::ifstream fin;

    unsigned start_time = clock();

    parser_matrices(matrix1, MATRIX1_FILE_NAME,
                    matrix2, MATRIX2_FILE_NAME);

    int count_rows = N / P;
    int last_count_rows = N - count_rows * P;
    for (int i = 0; i < P - 1; ++i) {
        threads.emplace_back(multiply_rows, std::ref(matrix1), std::ref(matrix2), std::ref(result_matrix), 
                            N, i * count_rows, count_rows);
    }
    threads.emplace_back(multiply_rows, std::ref(matrix1), std::ref(matrix2), std::ref(result_matrix), 
                        N, (P - 1) * count_rows, count_rows + last_count_rows);

    for (auto &th : threads) {
        th.join();
    }

    write_matrix(result_matrix, N, RESULT_FILE_NAME);

    unsigned end_time = clock();
    std::cout << "work time (mc): " << end_time - start_time << std::endl;

    return 0;
}