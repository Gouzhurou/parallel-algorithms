#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>

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

int main() 
{
    std::srand(time(0));
    int start = 0;
    int end = 9;
    int size = 1000;
    std::string fileName = "matrix5.txt";

    generateMatrix(start, end, size, fileName);

    return 0;
}