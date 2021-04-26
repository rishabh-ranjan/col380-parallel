#!/bin/bash
set -e

N="$1"
A="$2"
T="$3"
S="$4"
L="output_L_${S}_${T}.txt"
U="output_U_${S}_${T}.txt"

if [[ "$S" -eq "4" ]]
then
	mpiexec -n "$T" ./main_mpi "$N" "$A" "$L" "$U"
else
	OMP_NUM_THREADS="$T" ./main_omp "$S" "$N" "$A" "$L" "$U"
fi
