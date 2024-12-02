#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <functional>  // std::ref

#define N 500  // matrix size
#define RESULT_FILE_NAME "result2.2.txt"
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

    std::thread parser_thread(parser_matrices, std::ref(matrix1), MATRIX1_FILE_NAME, 
                            std::ref(matrix2), MATRIX2_FILE_NAME);
    parser_thread.join();


    std::thread mult_thread(multiply_matrices, std::ref(matrix1), std::ref(matrix2), 
                            std::ref(result_matrix));
    mult_thread.join();

    std::thread write_thread(write_matrix, std::ref(result_matrix), RESULT_FILE_NAME);
    write_thread.join();

    return 0;
}

