#ifndef LOCK_FREE_GLOBAL_H
#define LOCK_FREE_GLOBAL_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <atomic>

#define SIZE 30
#define MATRICES_COUNT 2

template <typename T>
class QueueLockFree 
{
private:
    struct Node 
    {
        std::shared_ptr<T> data;
        // template claa for secure access to values
        // ensures that operations are performed seamlessly
        std::atomic<Node*> next = nullptr;
    };

    std::atomic<Node*> _head;
    std::atomic<Node*> _tail;

    static void delete_nodes(Node* nodes) 
    {
        while(nodes) 
        {
            Node* next = nodes->next;
            delete nodes;
            nodes = next;
        }
    }

    void try_delete(Node* old_head);

public:
    // std::atomic::load() reads data
    QueueLockFree() : _head(new Node()), _tail(_head.load()) 
    {}

    void push(T value);

    std::shared_ptr<T> pop();
    
};

std::vector<std::vector<int>> generate_matrix(int rows, int columns);

void generate_matrices(QueueLockFree<std::vector<std::vector<int>>*>& qgenerate, int calc_count, int producer_count);

void multiply_matrices(QueueLockFree<std::vector<std::vector<int>>*>& qgenerate,
                       QueueLockFree<std::vector<std::vector<int>>>& qmult, int calc_count, int consumer_count);

void write_result(QueueLockFree<std::vector<std::vector<int>>>& qmult, int calc_count, const std::string& file_name);

template <typename T>
void QueueLockFree<T>::push(T value) 
{
    Node* new_node = new Node();
    new_node->data = std::make_shared<T>(std::move(value));

    while (true) 
    {
        Node* old_tail = _tail.load();
        Node* next = old_tail->next.load();

        if (old_tail == _tail.load()) 
        {
            if (next == nullptr) 
            {
                // exchange values
                if (old_tail->next.compare_exchange_weak(next, new_node)) 
                {
                    _tail.compare_exchange_weak(old_tail, new_node);
                    return;
                }
            }
            else
                _tail.compare_exchange_weak(old_tail, next);
        }

    }
}

template <typename T>
std::shared_ptr<T> QueueLockFree<T>::pop() 
{
    while (true) 
    {
        Node* old_head = _head.load();
        Node* old_tail = _tail.load();
        Node* next = old_head->next.load();

        if (old_head == _head.load()) 
        {
            if (old_head == old_tail) 
            {
                if (next) 
                {
                    _tail.compare_exchange_weak(old_tail,  next);
                }
            }
            else
            {
                std::shared_ptr<T> data = next->data;
                if (_head.compare_exchange_weak(old_head, next)) 
                {
                    return data;
                }
            }
        }
    }
}

#endif