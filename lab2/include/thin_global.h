#ifndef THIN_GLOBAL_H
#define THIN_GLOBAL_H

#include <iostream>
#include <condition_variable>
#include <mutex>
#include <string>
#include <vector>

#define SIZE 30
#define MATRICES_COUNT 2
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
    std::mutex _head_mutex;
    // Unique property
    std::unique_ptr<Node> _head;
    std::mutex _tail_mutex;
    Node* _tail;
    // notifications between threads
    std::condition_variable _cv_empty;
    std::condition_variable _cv_full;
    int _size;

    std::unique_ptr<Node> wait_pop_head(T& value) 
    {
        // more flexible locking than std::lock_guard<std::mutex>
        std::unique_lock<std::mutex> head_lock(_head_mutex);
        _cv_empty.wait(head_lock, [&]{ return !is_empty(); });

        value = std::move(*_head->data);
        std::unique_ptr<Node> old_head = std::move(_head);
        _head = std::move(old_head->next);
        _size--;
        return old_head;
    }

public:
    QueueThin() : _head(new Node()), _tail(_head.get()), _size(0) {}

    void push(T new_value) 
    {
        std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<Node> p(new Node());
        Node* const new_tail = p.get();
        {
            std::unique_lock<std::mutex> tail_lock(_tail_mutex);
            _cv_full.wait(tail_lock, [&]{ return _size < MAX_QUEUE_SIZE; });
            _tail->data = new_data;
            _tail->next = std::move(p);
            _tail = new_tail;
            _size++;
        }
        _cv_empty.notify_one();
    }

    void wait_and_pop(T& value) 
    {
        std::unique_ptr<Node> const old_head = wait_pop_head(value);
        _cv_full.notify_one();
    }

    bool is_empty() 
    {
        return (_head.get() == _tail);
    }

};

std::vector<std::vector<int>> generate_matrix(int rows, int columns);

void generate_matrices(QueueThin<std::vector<std::vector<int>>*>& qgenerate, int calc_count, int producer_count);

void multiply_matrices(QueueThin<std::vector<std::vector<int>>*>& qgenerate,
                       QueueThin<std::vector<std::vector<int>>>& qmult, int calc_count, int consumer_count);

void write_result(QueueThin<std::vector<std::vector<int>>>& qmult, int calc_count, const std::string& file_name);

#endif