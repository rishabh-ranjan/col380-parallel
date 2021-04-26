#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
	srand48(time(NULL));
	int n = atoi(argv[1]);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < n; ++j) {
			 double d = rand() & 1 ? drand48() : -drand48();
			 printf("%0.12lf ", d);
		}
		printf("\n");
	}
}
