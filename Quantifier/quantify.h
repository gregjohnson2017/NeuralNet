#ifndef QUANTIFY_H
#define QUANTIFY_H
#include "gimage.h"
unsigned char** get_image_data(image *i, int size);
int count_files(char *dir);
typedef struct image{
  struct pixel ***data;
}image;
typedef struct pixel{
  unsigned char r, g, b, a;
}pixel;
typedef struct data_collection{
	int num_arrays, size;
  struct image **data, **answers;
} data_collection;
struct image* allocate_image(int size);
struct data_collection* create_data(int num_arrays, int size);
struct data_collection* read_data(char *data_file);
void destroy_data(struct data_collection *d);
void create_data_file(char *dir, int size, char *data_file);
//void print_data(struct data_collection *d);
#endif
