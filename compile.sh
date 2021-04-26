#!/bin/bash
set -e

mpicc -Wall -DNDEBUG -o main_mpi main_mpi.c common.c
gcc -fopenmp -Wall -DNDEBUG -o main_omp main_omp.c common.c
