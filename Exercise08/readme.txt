# Exercise 8 - Compile and Run All MPI Programs

This folder contains a Makefile that compiles and runs all six MPI array-sum programs from Exercises 1 to 6.

## Run the Makefile

Open a terminal in the Exercise08 folder and run:

    make run

This compiles all six programs and runs each program with 4 MPI processes.

## Other commands

Compile all programs only:

    make

Run with a different number of processes (the number must evenly divide 1,000,000):

    make run NP=2
    make run NP=8

Remove the compiled executable files:

    make clean

## Requirements

On Windows, install Microsoft MPI and Microsoft MPI SDK before running the Makefile. The Makefile uses GCC together with Microsoft MPI.

On Linux, install an MPI implementation such as OpenMPI, which provides `mpicc` and `mpirun`.
