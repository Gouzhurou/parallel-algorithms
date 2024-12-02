#include <iostream>
#include <thread>

#include "../include/thin_global.h"

#define RESULT_FILE_NAME "./data/thin_result.txt"

int main()
{
    int calc_count = 100;
    int repeat_count = 10;
    int producer_count = 10;
    int consumer_count = 10;
    unsigned long duration_sum = 0;

    QueueThin<std::vector<std::vector<int>>*>* qgenerate = new QueueThin<std::vector<std::vector<int>>*>;
    QueueThin<std::vector<std::vector<int>>>* qmult = new QueueThin<std::vector<std::vector<int>>>;

    for (auto i = 0; i < repeat_count; ++i)
    {
        remove(RESULT_FILE_NAME);

        auto start_time = std::chrono::high_resolution_clock::now();

        std::vector<std::thread> threads;

        for (int j = 0; j < producer_count; j++)
            threads.push_back(std::thread(generate_matrices, std::ref(*qgenerate), calc_count, producer_count));

        for (int j = 0; j < consumer_count; j++)
            threads.push_back(std::thread(multiply_matrices, std::ref(*qgenerate), std::ref(*qmult), calc_count, consumer_count));

        threads.push_back(std::thread(write_result, std::ref(*qmult), calc_count, RESULT_FILE_NAME));

        for (auto j = 0; j < producer_count + consumer_count + 1; ++j) 
        {
            threads[j].join();
        }

        auto stop_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);

        duration_sum += duration.count();
    }

    std::cout << "Executing time:" << duration_sum / repeat_count << std::endl;

    delete qgenerate;
    delete qmult;
}
