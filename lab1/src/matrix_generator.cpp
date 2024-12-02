#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>

#include "../include/global.h"

int main() 
{
    std::srand(time(0));
    int start = 0;
    int end = 9;
    int size = 1000;
    std::string fileName = "./data/matrix5.txt";

    generateMatrix(start, end, size, fileName);

    return 0;
}