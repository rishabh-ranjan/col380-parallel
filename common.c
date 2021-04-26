#include "common.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void write_output(char fname[], double **arr, int n) {
	FILE *f = fopen(fname, "w");
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			fprintf(f, "%0.12f ", arr[i][j]);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

double *tmp[N];
void aux_write_output(const char *fname, double *arr, int n)
{
	for (int i = 0; i < n; ++i) {
		tmp[i] = &arr[n*i];
	}
	write_output((char *)fname, tmp, n);
}

float fscanfl(FILE *fp) {
	int c = fgetc_unlocked(fp);
	if (c == '\n') {
		c = fgetc_unlocked(fp);
	}
	int neg = 0;
	if (c == '-') {
		neg = 1;
		c = fgetc_unlocked(fp);
	}
	float y = 0;
	while (c >= '0' && c <= '9') {
		y = y*10 + (c-'0');
		c = fgetc_unlocked(fp);
	}
	assert(c == '.');
	float p = 0.1;
	do {
		c = fgetc_unlocked(fp);
		y += (c-'0')*p;
		p /= 10;
	} while (c >= '0' && c <= '9');
	return neg ? -y : y;
}

double fscand(FILE *fp) {
	int c = fgetc_unlocked(fp);
	if (c == '\n') {
		c = fgetc_unlocked(fp);
	}
	int neg = 0;
	if (c == '-') {
		neg = 1;
		c = fgetc_unlocked(fp);
	}
	double y = 0;
	while (c >= '0' && c <= '9') {
		y = y*10 + (c-'0');
		c = fgetc_unlocked(fp);
	}
	assert(c == '.');
	double p = 0.1;
	do {
		c = fgetc_unlocked(fp);
		y += (c-'0')*p;
		p /= 10;
	} while (c >= '0' && c <= '9');
	return neg ? -y : y;
}
