#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void writeRandomMatricesToFile(const char *filename, int m, int k, int n, int range);
int readMatrixFromStdin(int **A, int **B, int **C, int *m, int *n, int *k, int *padded_m, int *padded_k, int *padded_n, int block_size);
int readMatrixFromFile(const char *filename, int **A, int **B, int **C, int *m, int *n, int *k, int *padded_m, int *padded_k, int *padded_n, int block_size);

