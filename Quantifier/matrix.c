#include "matrix.h"
#include <stdio.h>
#include <math.h>

matrix matrix_create(unsigned rows, unsigned cols){
	matrix mat = malloc(sizeof(float*) * (rows + 1));
	mat[rows] = NULL;
	for(int r = 0; r < rows; r++){
		mat[r] = malloc(sizeof(float) * (cols + 1));
		mat[r][cols] = NAN;
		for(int c = 0; c < cols; c++){
			mat[r][c] = r == c ? 1.0 : 0.0; // identity matrix
		}
	}
	return mat;
}

void matrix_delete(matrix mat){
	for(int r = 0; mat[r]; r++){
		free(mat[r]);
	}
	free(mat);
}

void matrix_print(matrix mat){
	int rows = 0;
	for(rows = 0; mat[rows]; rows++);
	for(int r = 0; mat[r]; r++){
		for(int c = 0; mat[r][c] == mat[r][c]; c++){
			if(c == 0){
				printf("%d : { %f, ", r, mat[r][c]);
			}else if (mat[r][c + 1] != mat[r][c + 1]){
				printf("%f }", mat[r][c]);
			}else{
				printf("%f, ", mat[r][c]);
			}
		}
		if(mat[r + 1]){
			printf(",\n");
		}else{
			printf("\n");
		}
	}
}

float matrix_convolve(matrix kernel, matrix sample){
	int kernel_rows = 0;
	int sample_rows = 0;
	for(int r = 0; kernel[r]; r++){
		kernel_rows++;
	}
	for(int r = 0; sample[r]; r++){
		sample_rows++;
	}	
	int kernel_cols = kernel_rows;
	if(kernel_rows != sample_rows) {
  	return NAN; // not equal
	}
	if(kernel_rows % 2 == 0){
	  return NAN; // not odd
	}
	if(kernel_rows < 3){
	  return NAN; // too small
	}
	float accumulator = 0, total = 0;
	for(int r = -(kernel_rows - 1) / 2; r < (kernel_rows + 1) / 2; r++){
		for(int c = -(kernel_cols - 1) / 2; c < (kernel_cols + 1) / 2; c++){
			total += kernel[r + ((kernel_rows - 1) / 2)][c + ((kernel_cols - 1) / 2)];
			accumulator += kernel[r + ((kernel_rows - 1) / 2)][c + ((kernel_cols - 1) / 2)] 
			* sample[-r + ((kernel_rows - 1) / 2)][-c + ((kernel_cols - 1) / 2)];
		}
	}
	if(total == 0) return accumulator;
	else return accumulator / total;
}
