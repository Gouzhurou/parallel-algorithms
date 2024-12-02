#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../include/rude_global.h"

std::vector<std::vector<int>> generate_matrix(int rows, int columns)
{
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(columns, 0));

    for (auto i = 0; i < rows; i++)
        for (auto j = 0; j < columns; j++)
            matrix[i][j] = rand() % 100;

    return matrix;
}


void generate_matrices(QueueRude<std::vector<std::vector<int>>*>& qgenerate, int calc_count, int producer_count)
{
    for (auto i = 0; i < calc_count; i += producer_count)
    {
        std::vector<std::vector<int>>* matrices = new std::vector<std::vector<int>>[MATRICES_COUNT];

        matrices[0] = generate_matrix(SIZE, SIZE);
        matrices[1] = generate_matrix(SIZE, SIZE);

        qgenerate.push(matrices);
    }
}

void multiply_matrices(QueueRude<std::vector<std::vector<int>>*>& qgenerate,
                       QueueRude<std::vector<std::vector<int>>>& qmult, int calc_count, int consumer_count)
{
    for (auto i = 0; i < calc_count; i += consumer_count)
    {
        std::vector<std::vector<int>>* matrices;
        qgenerate.wait_and_pop(matrices);

        std::vector<std::vector<int>> matrix_res(SIZE, std::vector<int>(SIZE, 0));

        for(auto i = 0; i < SIZE; i++)
            for(auto j = 0; j < SIZE; j++)
                for(auto k = 0; k < SIZE; k++)
                    matrix_res[i][j] += matrices[0][i][k] * matrices[1][k][j];

        qmult.push(matrix_res);
        delete[] matrices;
    }
}

void write_result(QueueRude<std::vector<std::vector<int>>>& qmult, int calc_count, const std::string& file_name)
{
    for (auto i = 0; i < calc_count; ++i)
    {
        std::vector<std::vector<int>> matrix_res;
        qmult.wait_and_pop(matrix_res);

        std::ofstream fout;
        // All output operations happen at the end of the file, appending to its existing contents
        fout.open(file_name, std::ios::app);

        if (!fout){
            std::cerr << "Can't open fout to write result" << std::endl;
            return;
        }

        fout << "--------------------------------" << std::endl;

        for (auto i = 0; i < SIZE; ++i)
        {
            for(auto j = 0; j < SIZE - 1; ++j)
            {
                fout << matrix_res[i][j] << ' ';
            }
            fout << matrix_res[i][SIZE - 1] << std::endl;
        }

        fout.close();
    }
}