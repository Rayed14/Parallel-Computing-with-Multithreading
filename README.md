# Parallel Computing with Multithreading
## CPU Information and Array Operations
## Project Overview
This project explores the parallel computing capabilities of multithreading by executing commands to gather information about the CPU and performing array operations concurrently.

### Task 1
* CPU Information Retrieval:

The program extracts details about the CPU, memory, and logical cores using commands like cat /proc/cpuinfo, cat /proc/meminfo, and lscpu.
* Array Operations:

A 1D array of size 2^16 is populated.
The square root of each array element is calculated concurrently, replacing the original item in the array.
* Multithreading:

Threads are created equal to the number of logical cores using pthread_create().
The array items are distributed block-wise to each thread in the populate() and calcSqrt() functions.
Pthreads affinity or taskset is used for block-wise distribution.

### Task 2
* Array Population and Sum Calculation:

A 2D array of size 2^16 * 2^16 is populated with random integers.
The sum of all elements in the array is computed and stored in a global variable.
* Multithreading:

* Two versions of multithreading are implemented:

Version 1 (Block Wise): Threads operate on blocks of the 2D array with varying block sizes.
Version 2 (Cyclic): Threads operate cyclically on fixed rows of the 2D array.
* Performance Evaluation:

GFlops are computed for each version and block size.
Plots are generated for various scenarios (V1 and V2), illustrating the relationship between block size/number of rows and GFlops.
### Getting Started
* Clone the Repository:

git clone https://github.com/your-username/parallel-multithreading-cpu-array-ops.git
* Build and Run:

Navigate to the project directory and compile the code.

gcc -o main main.c -lpthread -lm
Execute the compiled program.
bash
Copy code
./main
* Review Results:

Examine the output to see CPU information and the results of array operations.
Check the generated plots for performance evaluation.
### Dependencies
pthread library for multithreading
math library for mathematical operations
### Contributing
Feel free to contribute by opening issues or submitting pull requests. Your feedback and improvements are highly appreciated.

