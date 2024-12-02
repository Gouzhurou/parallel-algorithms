# Lab2: Implementing Thread-Safe Data Structures with Locks
Implement iterative (potentially infinite) execution of data preparation, processing and output using the producer-consumer pattern (based on Lab. 1 (parts 1.2.1 and 1.2.2)).
- The number of producers and consumers must be variable.
- Ensure parallel execution of the threads for processing the finished portion of data, preparing the next portion of data and outputting the previously obtained results.
- Use the “condition variable” mechanism.

2.1 Use a queue with “coarse” locking.

2.2 Use a queue with “fine” locking.

- The queue must have an upper limit on the number of elements.

Perform testing of items 2.1 and 2.2, ensure the correctness of the results.

In the report:
Compare the performance of 2.1 and 2.2 depending on the number of producers and consumers.
# loading program
- rude: g++ -o rude src/rude_lock.cpp src/rude_global.cpp
- thin: g++ -o thin src/thin_lock.cpp src/thin_global.cpp 