#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>

#include "../include/global.h"

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
                    std::vector<std::vector<int>>& result_matrix,
                    const int size) 
{
    for (int i = 0; i < size; ++i) {
        std::vector<int> row;
        for (int j = 0; j < size; ++j) {
            int result = 0;
            for (int k = 0; k < size; ++k) {
                result += matrix1[i][k] * matrix2[k][j];
            }
            row.push_back(result);
        }
        result_matrix.push_back(row);
    }
}

void multiply_rows(std::vector<std::vector<int>>& matrix1, 
                std::vector<std::vector<int>>& matrix2, 
                std::vector<std::vector<int>>& result_matrix, 
                const int size, const int start_row, const int row_count) {
    for (int i = start_row; i < start_row + row_count; ++i) {
        for (int j = 0; j < size; ++j) {
            int result = 0;
            for (int k = 0; k < size; ++k) {
                result += matrix1[i][k] * matrix2[k][j];
            }
            result_matrix[i][j] = result;
        }
    }
}

void write_matrix(std::vector<std::vector<int>>& matrix, const int size, const std::string& fileName) 
{
    std::ofstream fout;
    fout.open(fileName);

    if(fout.is_open())
    {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size - 1; ++j) {
                fout << matrix[i][j] << " ";
            }
            fout << matrix[i][size - 1] << std::endl;
        }

        fout.close();
    }
}

void generateMatrix(int start, int end, int size, const std::string& fileName)
{
    std::ofstream fout;
    fout.open(fileName);

    if (fout.is_open())
    {
        for (auto i = 0; i < size; ++i)
        {
            for (auto j = 0; j < size - 1; ++j)
            {
                fout << std::rand() % (end - start + 1) + start << " ";
            }
            fout << std::rand() % (end - start + 1) + start;
            if (i < size - 1)
                fout << std::endl;
        }

        fout.close();
    }
    else 
    {
        std::cerr << "open file error" << std::endl;
        return;
    }
}
