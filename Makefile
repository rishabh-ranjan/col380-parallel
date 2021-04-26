all: main_omp main_mpi
.PHONY: tc clean

main_omp: main_omp.c common.c
	gcc -Wall -fopenmp -o $@ $^

main_mpi: main_mpi.c common.c
	mpicc -Wall -o $@ $^

tc: gen
	./gen $(N) > $(N).A

gen: gen.c
	gcc -Wall -o $@ $^

clean:
	rm main_omp main_mpi gen *.A *.L *.U output_*
