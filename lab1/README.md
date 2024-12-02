# Lab1: Basics of working with processes and threads
The lab consists of 3 subtasks that perform the same task using processes or threads. The implementation is done in C/C++. Perform the multiplication of 2 matrices. Input matrices are entered from a file (or generated). The result is written to the file.
1.1.
Perform the task, dividing it into 3 processes. Choose a data exchange mechanism between the processes.
Process 1: fills the input matrices with data (reads from a file or generates them in some way).
Process 2: performs the multiplication
Process 3: outputs the result
1.2.1
Similar to 1.1, using threads (std::threads)
1.2.2
Divide the multiplication into P threads (you can use the “naive” method by rows and columns).
Test by comparing the calculation result with the results from 1.2.1 / 1.1
In the report:
Investigate the relationship between the number of threads, the size of the input data, and the parameters of the target computing system. Formulate restrictions on the number of threads.

# loadind program
- tast_num: g++ -o task src/task_num.cpp src/global.cpp
- matrix_generator: g++ -o generator src/matrix_generator.cpp src/global.cpp