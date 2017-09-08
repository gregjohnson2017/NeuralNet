#ifndef QUANTIFY_H
#define QUANTIFY_H
#include "gimage.h"
int** getData(image *i, int size);
int count_files(char *dir);
typedef struct data_collection{
  unsigned char  ***data, *answers
	int num_arrays, size;
} data_collection;
struct data_collection* create_data(int num_arrays, int size);
void destroy_data(struct data_collection *d);
struct data_collection* read_data(char *data_file);
void write_data(char *dir, int size, char *data_file);
void print_data(struct data_collection *d);
#endif
