#include <iostream>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <thread>

#define SIZE 30
#define MATRICES_COUNT 2
#define RESULT_FILE_NAME "thin_result.txt"
#define MAX_QUEUE_SIZE 100

template<typename T>
class QueueThin 
{
private:
    struct Node 
    {
        std::shared_ptr<T> data;
        std::unique_ptr<Node> next;
    };

    // Synchronizing access to shared resources in multithreaded applications
    std::mutex head_mutex;
    // Unique property
    std::unique_ptr<Node> head;
    std::mutex tail_mutex;
    Node* tail;
    // notifications between threads
    std::condition_variable cvEmpty;
    std::condition_variable cvFull;
    int size;

    std::unique_ptr<Node> wait_pop_head(T& value) 
    {
        // more flexible locking than std::lock_guard<std::mutex>
        std::unique_lock<std::mutex> head_lock(head_mutex);
        cvEmpty.wait(head_lock, [&]{ return !is_empty(); });

        value = std::move(*head->data);
        std::unique_ptr<Node> old_head = std::move(head);
        head = std::move(old_head->next);
        size--;
        return old_head;
    }

public:
    QueueThin() : head(new Node()), tail(head.get()), size(0) {}

    void push(T new_value) 
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<Node> p(new Node());
        Node* const new_tail = p.get();
        {
            std::unique_lock<std::mutex> tail_lock(tail_mutex);
            cvFull.wait(tail_lock, [&]{ return size < MAX_QUEUE_SIZE; });
            tail->data = new_data;
            tail->next = std::move(p);
            tail = new_tail;
            size++;
        }
        cvEmpty.notify_one();
    }

    void wait_and_pop(T& value) 
    {
        std::unique_ptr<Node> const old_head = wait_pop_head(value);
        cvFull.notify_one();
    }

    bool is_empty() 
    {
        // std::unique_lock<std::mutex> tail_lock(tail_mutex);
        return (head.get() == tail);
    }

};


std::vector<std::vector<int>> generate_matrix(int rows, int columns)
{
    std::vector<std::vector<int>> matrix(rows, std::vector<int>(columns, 0));

    for (auto i = 0; i < rows; i++)
        for (auto j = 0; j < columns; j++)
            matrix[i][j] = rand() % 100;

    return matrix;
}

void generate_matrices(QueueThin<std::vector<std::vector<int>>*>& qgenerate, int calc_count, int producer_count)
{
    for (auto i = 0; i < calc_count; i += producer_count)
    {
        std::vector<std::vector<int>>* matrices = new std::vector<std::vector<int>>[MATRICES_COUNT];

        matrices[0] = generate_matrix(SIZE, SIZE);
        matrices[1] = generate_matrix(SIZE, SIZE);

        qgenerate.push(matrices);
    }
}

void multiply_matrices(QueueThin<std::vector<std::vector<int>>*>& qgenerate,
                       QueueThin<std::vector<std::vector<int>>>& qmult, int calc_count, int consumer_count)
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

void write_result(QueueThin<std::vector<std::vector<int>>>& qmult, int calc_count, const std::string& file_name)
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

int main()
{
    int calc_count = 100;
    int repeat_count = 100;
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
