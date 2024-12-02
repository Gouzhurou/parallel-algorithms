#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <functional>  // std::ref

#include "../include/global.h"

#define N 100  // matrix size
#define RESULT_FILE_NAME "./data/result2_100.txt"
#define MATRIX1_FILE_NAME "./data/matrix1_100.txt"
#define MATRIX2_FILE_NAME "./data/matrix2_100.txt"

int main() {
    std::vector<std::vector<int>> matrix1;
    std::vector<std::vector<int>> matrix2;
    std::vector<std::vector<int>> result_matrix;

    std::thread parser_thread(parser_matrices, std::ref(matrix1), MATRIX1_FILE_NAME, 
                            std::ref(matrix2), MATRIX2_FILE_NAME);
    parser_thread.join();


    std::thread mult_thread(multiply_matrices, std::ref(matrix1), std::ref(matrix2), 
                            std::ref(result_matrix), N);
    mult_thread.join();

    std::thread write_thread(write_matrix, std::ref(result_matrix), N, RESULT_FILE_NAME);
    write_thread.join();

    return 0;
}

