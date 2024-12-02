#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>

#include "../include/global.h"

#define N 500  // matrix size
#define RESULT_FILE_NAME "./data/result1_500.txt"
#define MATRIX1_FILE_NAME "./data/matrix1_500.txt"
#define MATRIX2_FILE_NAME "./data/matrix2_500.txt"

int main() {
    std::vector<std::vector<int>> matrix1;
    std::vector<std::vector<int>> matrix2;
    std::vector<std::vector<int>> result_matrix;

    pid_t pid1;
    // first process creation for parser matrices
    switch (pid1 = fork()) 
    {
        // error
        case -1:
            exit(-1);
        // child code
        case 0:
            parser_matrices(matrix1, MATRIX1_FILE_NAME, matrix2, MATRIX2_FILE_NAME);

            pid_t pid2;
            // second process creation for multiplication matrices
            switch (pid2 = fork()) 
            {
                // error
                case -1:
                    exit(-1);
                // child code
                case 0:
                    multiply_matrices(matrix1, matrix2, result_matrix, N);

                    pid_t pid3;
                    // third process creation for write result in file
                    switch (pid3 = fork()) 
                    {
                        // error
                        case -1:
                            exit(-1);
                        // child code
                        case 0:
                            write_matrix(result_matrix, N, RESULT_FILE_NAME);
                        // parent code
                        default: 
                            waitpid(pid3, nullptr, 0);
                    }
                // parent code
                default:
                    waitpid(pid2, nullptr, 0);
            }
        // parent code
        default:
            waitpid(pid1, nullptr, 0);
    }
    
    return 0;
}

