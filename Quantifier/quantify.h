#ifndef QUANTIFY_H
#define QUANTIFY_H
#include "gimage.h"
unsigned char** getData(image *i, int size);
int count_files(char *dir);
typedef struct image{
  int size;
  struct pixel ***data;
}image;
typedef struct pixel{
  unsigned char r, g, b, a;
}pixel;
typedef struct data_collection{
	int num_arrays, size, depth;
  struct pixel ***data, ***answers;
} data_collection;
struct data_collection* create_data(int num_arrays, int size);
void destroy_data(struct data_collection *d);
struct data_collection* read_data(char *data_file);
void write_data(char *dir, int size, char *data_file);
void print_data(struct data_collection *d);
#endif
