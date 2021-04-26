#include "common.h"
#include <assert.h>
#include <omp.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

inline static void sect(int j, int l, int u, void (*fn)(int, int)) {
	#pragma omp parallel sections
	{
		#pragma omp section
		for (int i = l+0; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+1; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+2; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+3; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+4; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+5; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+6; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+7; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+8; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+9; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+10; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+11; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+12; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+13; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+14; i < u; i += 16) (*fn)(i, j);
		#pragma omp section
		for (int i = l+15; i < u; i += 16) (*fn)(i, j);
	}
}

double A[N*N], L[N*N], UT[N*N], U[N*N];
int n;

inline static void pre(int i, int j) {
	for (int j = 0; j <= i; ++j) {
		L[n*i+j] = A[n*i+j];
		UT[n*i+j] = A[n*j+i];
	}
}

inline static void comp(int i, int j) {
	double ls = 0, us = 0;
	for (int k = 0; k < j; ++k) {
		ls += L[n*i+k] * UT[n*j+k];
		us += L[n*j+k] * UT[n*i+k];
	}
	L[n*i+j] -= ls;
	UT[n*i+j] = (UT[n*i+j] - us)/L[n*j+j];
	L[n*i+i] -= L[n*i+j] * UT[n*i+j];
}

inline static void post(int i, int j) {
	U[n*i+i] = 1;
	for (int j = i+1; j < n; ++j) {
		U[n*i+j] = UT[n*j+i];
	}
}

static void _serial(void)
{
	for (int i = 0; i < n; ++i) pre(i, 0);
	for (int j = 0; j < n; ++j) {
		for (int i = j+1; i < n; ++i) comp(i, j);
	}
	for (int i = 0; i < n; ++i) post(i, 0);
}

static void _for(void)
{
	#pragma omp parallel for schedule(static, 1) default(none) shared(n, A, L, UT)
	for (int i = 0; i < n; ++i) pre(i, 0);
	for (int j = 0; j < n; ++j) {
		#pragma omp parallel for default(none) shared(n, A, L, UT, j)
		for (int i = j+1; i < n; ++i) {
			if (L[n*j+j] == 0) {
				exit(0);
			}
			comp(i, j);
		}
	}
	#pragma omp parallel for schedule(static, 1) default(none) shared(n, U, UT)
	for (int i = 0; i < n; ++i) post(i, 0);
}

static void _sections(void)
{
	sect(0, 0, n, pre);
	for (int j = 0; j < n; ++j) sect(j, j+1, n, comp);
	sect(0, 0, n, post);
}

static void _for_sections(void)
{
	sect(0, 0, n, pre);
	for (int j = 0; j < n; ++j) {
		#pragma omp parallel for default(none) shared(n, A, L, UT, j)
		for (int i = j+1; i < n; ++i) comp(i, j);
	}
	sect(0, 0, n, post);
}

const void (*prcs[4])(void) = {_serial, _for, _sections, _for_sections};

int main(int argc, char **argv)
{

#ifndef NDEBUG
	double tic, toc;
	tic = omp_get_wtime();
#endif

	assert(argc == 6);
	int s = atoi(argv[1]);
	n = atoi(argv[2]);
	const char *A_fname = argv[3];
	const char *L_fname = argv[4];
	const char *U_fname = argv[5];

	FILE *fp = NULL;
	fp = fopen(A_fname, "r");
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			A[n*i+j] = fscand(fp);
		}
	}
	fclose(fp);

#ifndef NDEBUG
	toc = omp_get_wtime();
	printf("inp:\t%0.3lf s\n", toc-tic);
	tic = omp_get_wtime();
#endif

	(*prcs[s])();

#ifndef NDEBUG
	toc = omp_get_wtime();
	printf("prc:\t%0.3lf s\n", toc-tic);
	tic = omp_get_wtime();
#endif

	aux_write_output(L_fname, L, n);
	aux_write_output(U_fname, U, n);

#ifndef NDEBUG
	toc = omp_get_wtime();
	printf("out:\t%0.3lf s\n", toc-tic);
#endif

	return 0;
}
