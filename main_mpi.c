#include "common.h"
#include <assert.h>
#include <mpi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

double A[N*N], L[N*N], UT[N*N], U[N*N];

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	int p, r;
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &r);

#ifndef NDEBUG
	double tic, toc;
	tic = MPI_Wtime();
#endif

	assert(argc == 5);
	int n = atoi(argv[1]);
	const char *A_fname = argv[2];
	const char *L_fname = argv[3];
	const char *U_fname = argv[4];

	FILE *fp = NULL;
	fp = fopen(A_fname, "r");
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			A[n*i+j] = fscand(fp);
		}
	}
	fclose(fp);

#ifndef NDEBUG
	MPI_Barrier(MPI_COMM_WORLD);
	toc = MPI_Wtime();
	if (r == 0) {
		printf("inp:\t%0.3lf s\n", toc-tic);
	}
	tic = MPI_Wtime();
#endif

	for (int j = 0; j < n; ++j) {
		const int root = j%p;
		MPI_Request reqs[3];
		MPI_Ibcast(&L[n*j], j, MPI_DOUBLE, root, MPI_COMM_WORLD, &reqs[0]);
		MPI_Ibcast(&UT[n*j], j, MPI_DOUBLE, root, MPI_COMM_WORLD, &reqs[1]);
		if (r == root) {
			double ls = 0;
			for (int k = 0; k < j; ++k) {
				ls += L[n*j+k] * UT[n*j+k];
			}
			L[n*j+j] = A[n*j+j] - ls;
		}
		MPI_Ibcast(&L[n*j+j], 1, MPI_DOUBLE, root, MPI_COMM_WORLD, &reqs[2]);
		MPI_Waitall(3, reqs, MPI_STATUSES_IGNORE);
		const double d = L[n*j+j];
		if (d == 0) {
			MPI_Abort(MPI_COMM_WORLD, 0);
		}
		for (int i = j+(r-root?r-root:p); i < n; i += p) {
			double ls = 0, us = 0;
			for (int k = 0; k < j; ++k) {
				ls += L[n*i+k] * UT[n*j+k];
				us += L[n*j+k] * UT[n*i+k];
			}
			L[n*i+j] = A[n*i+j] - ls;
			UT[n*i+j] = (A[n*j+i] - us)/d;
		}
	}

#ifndef NDEBUG
	MPI_Barrier(MPI_COMM_WORLD);
#endif

	if (r == 0) {
		for (int i = 0; i < n; ++i) {
			U[n*i+i] = 1;
			for (int j = i+1; j < n; ++j) {
				U[n*i+j] = UT[n*j+i];
			}
		}

#ifndef NDEBUG
		toc = MPI_Wtime();
		printf("prc:\t%0.3lf s\n", toc-tic);
		tic = MPI_Wtime();
#endif

		aux_write_output(L_fname, L, n);
		aux_write_output(U_fname, U, n);

#ifndef NDEBUG
		toc = MPI_Wtime();
		printf("out:\t%0.3lf s\n", toc-tic);
#endif

	}

	MPI_Finalize();
	return 0;
}
