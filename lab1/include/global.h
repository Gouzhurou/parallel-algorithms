#ifndef GLOBAL_H
#define GLOBAL_H

#include <vector>
#include <string>

void parser_matrix(std::vector<std::vector<int>>& matrix, const std::string& fileName);

void parser_matrices(std::vector<std::vector<int>>& matrix1, const std::string& fileName1, 
                    std::vector<std::vector<int>>& matrix2, const std::string& fileName2);

void multiply_matrices(std::vector<std::vector<int>>& matrix1, 
                    std::vector<std::vector<int>>& matrix2, 
                    std::vector<std::vector<int>>& result_matrix,
                    const int size);

void multiply_rows(std::vector<std::vector<int>>& matrix1, 
                std::vector<std::vector<int>>& matrix2, 
                std::vector<std::vector<int>>& result_matrix, 
                const int size, const int start_row, const int row_count);

void write_matrix(std::vector<std::vector<int>>& matrix, const int size, const std::string& fileName);

void generateMatrix(int start, int end, int size, const std::string& fileName);

#endif 
