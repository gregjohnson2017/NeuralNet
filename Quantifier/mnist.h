#ifndef MNIST_H
#define MNIST_H
#include "gimage.h"
#include "quantify.h"
struct data_collection* read_mnist_data(char *image_file, char *label_file);
void write_mnist_data(struct data_collection *mnist_data, char *data_file);
void generate_mnist_images(struct data_collection *m);
#endif
