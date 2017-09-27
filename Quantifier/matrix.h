#ifndef _MATRIX_H
#define _MATRIX_H 1
#include <stdlib.h>
typedef float **matrix;
matrix matrix_create(unsigned, unsigned);
void matrix_delete(matrix);
float matrix_convolve(matrix, matrix);
void matrix_print(matrix);
#endif