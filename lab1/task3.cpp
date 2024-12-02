#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <string>
#include <functional>  // std::ref
#include <ctime>

#define N 1000  // matrix size
#define P 1  // threads count
#define RESULT_FILE_NAME "result3.txt"
#define MATRIX1_FILE_NAME "matrix5.txt"
#define MATRIX2_FILE_NAME "matrix6.txt"

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

void multiply_rows(std::vector<std::vector<int>>& matrix1, 
                std::vector<std::vector<int>>& matrix2, 
                std::vector<std::vector<int>>& result_matrix, 
                int start_row, int row_count) {
    for (int i = start_row; i < start_row + row_count; ++i) {
        for (int j = 0; j < N; ++j) {
            int result = 0;
            for (int k = 0; k < N; ++k) {
                result += matrix1[i][k] * matrix2[k][j];
            }
            result_matrix[i][j] = result;
        }
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
                            i * count_rows, count_rows);
    }
    threads.emplace_back(multiply_rows, std::ref(matrix1), std::ref(matrix2), std::ref(result_matrix), 
                        (P - 1) * count_rows, count_rows + last_count_rows);

    for (auto &th : threads) {
        th.join();
    }

    write_matrix(result_matrix, RESULT_FILE_NAME);

    unsigned end_time = clock();
    std::cout << "work time (mc): " << end_time - start_time << std::endl;

    return 0;
}