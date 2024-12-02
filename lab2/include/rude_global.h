#ifndef RUDE_GLOBAL_H
#define RUDE_GLOBAL_H

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <fstream>
#include <string>
#include <vector>

#define SIZE 30
#define MATRICES_COUNT 2
#define MAX_QUEUE_SIZE 100

template<typename T>
class QueueRude 
{
private:
    mutable std::mutex mut;
    std::queue<T> my_queue;
    std::condition_variable cv;
    int size;
public:
    QueueRude() : size(0) {}

    void push(T new_value) 
    {
        std::lock_guard<std::mutex> lk(mut);
        my_queue.push(std::move(new_value));
        cv.notify_one();
    }

    void wait_and_pop(T& value) 
    {
        std::unique_lock<std::mutex> lk(mut);
        cv.wait(lk, [this] {return !my_queue.empty();});
        value = std::move(my_queue.front());
        my_queue.pop();
    }

    // int size() {return my_queue.size();}
};


std::vector<std::vector<int>> generate_matrix(int rows, int columns);

void generate_matrices(QueueRude<std::vector<std::vector<int>>*>& qgenerate, int calc_count, int producer_count);

void multiply_matrices(QueueRude<std::vector<std::vector<int>>*>& qgenerate,
                       QueueRude<std::vector<std::vector<int>>>& qmult, int calc_count, int consumer_count);

void write_result(QueueRude<std::vector<std::vector<int>>>& qmult, int calc_count, const std::string& file_name);

#endif