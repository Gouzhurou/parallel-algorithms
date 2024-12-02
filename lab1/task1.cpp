#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>

#define N 500  // matrix size
#define RESULT_FILE_NAME "result1.2.txt"
#define MATRIX1_FILE_NAME "matrix3.txt"
#define MATRIX2_FILE_NAME "matrix4.txt"

void parser_matrix(std::vector<std::vector<int>>& matrix, const std::string& fileName) 
{
    std::ifstream fin;
    fin.open(fileName);

    std::string text;
    std::string subtext;
    int start_index;
    int end_index;

    while(std::getline(fin, text)) {
        std::vector<int> row;
        start_index = 0;

        while ((end_index = text.find(' ')) != text.npos) {
            subtext = text.substr(start_index, end_index - start_index);
            row.push_back(std::stoi(subtext));

            text[end_index] = '*';
            start_index = end_index + 1;
        }
        subtext = text.substr(start_index, text.length() - start_index);
        row.push_back(std::stoi(subtext));

        matrix.push_back(row);
    }

    fin.close();
}

void parser_matrices(std::vector<std::vector<int>>& matrix1, const std::string& fileName1, 
                    std::vector<std::vector<int>>& matrix2, const std::string& fileName2)
{
    parser_matrix(matrix1, fileName1);
    parser_matrix(matrix2, fileName2);
}

void multiply_matrices(std::vector<std::vector<int>>& matrix1, 
                    std::vector<std::vector<int>>& matrix2, 
                    std::vector<std::vector<int>>& result_matrix) 
{
    for (int i = 0; i < N; ++i) {
        std::vector<int> row;
        for (int j = 0; j < N; ++j) {
            int result = 0;
            for (int k = 0; k < N; ++k) {
                result += matrix1[i][k] * matrix2[k][j];
            }
            row.push_back(result);
        }
        result_matrix.push_back(row);
    }
}

void write_matrix(std::vector<std::vector<int>>& matrix, const std::string& fileName) 
{
    std::ofstream fout;
    fout.open(fileName);

    if(fout.is_open())
    {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N - 1; ++j) {
                fout << matrix[i][j] << " ";
            }
            fout << matrix[i][N - 1] << std::endl;
        }

        fout.close();
    }
}

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
                    multiply_matrices(matrix1, matrix2, result_matrix);

                    pid_t pid3;
                    // third process creation for write result in file
                    switch (pid3 = fork()) 
                    {
                        // error
                        case -1:
                            exit(-1);
                        // child code
                        case 0:
                            write_matrix(result_matrix, RESULT_FILE_NAME);
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

