# Parallel-Matrix-Multiplication
Program to multiply two matrices together in parallel using Pthreads

To compile:
gcc -g -Wall -o main main.c -lpthread -lm

To run:
./main [thread_number]

where [thread_number] is replaced by the number of threads you want to use.
