#pragma once

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void write_output(char [], double **, int);
void aux_write_output(const char *, double *, int);
float fscanfl(FILE *);
double fscand(FILE *);

#define N 5000
