#include "mnist.h"

struct data_collection* read_mnist_data(char *image_file, char *label_file){
	FILE *image_fp = fopen(image_file, "rb");
	FILE *label_fp = fopen(label_file, "rb");
	if(!image_fp){
		fprintf(stderr, "FILE %s NOT FOUND\n", image_file);
		abort();
	}else if (!label_fp){
	  fprintf(stderr, "FILE %s NOT FOUND\n", label_file);
		abort();
	}
	size_t image_gcc = 0, label_gcc = 0;
	int image_magic, image_count, image_rows, image_cols;
	image_gcc += fread(&image_magic, sizeof(int), 1, image_fp) * sizeof(int);
	image_gcc += fread(&image_count, sizeof(int), 1, image_fp) * sizeof(int);
	image_gcc += fread(&image_rows, sizeof(int), 1, image_fp) * sizeof(int);
	image_gcc += fread(&image_cols, sizeof(int), 1, image_fp) * sizeof(int);
	int label_magic, label_count;
	label_gcc += fread(&label_magic, sizeof(int), 1, label_fp) * sizeof(int);
	label_gcc += fread(&label_count, sizeof(int), 1, label_fp) * sizeof(int);
	if(image_count != label_count){
	  fprintf(stderr, "mismatched image and label count (%d and %d)\n", image_count, label_count);
	}
	if(image_rows != image_cols){
	  fprintf(stderr, "image rows and cols not equal (%d and %d)\n", image_rows, image_cols);
	}
	struct data_collection *data_c = create_data(image_count, image_rows);
	for(int i = 0; i < image_count; i++){
		for(int j = 0; j < image_rows; j++){
			image_gcc += fread(data_c->data[i][j], sizeof(*data_c->data[i][j]), image_cols, image_fp) * sizeof(*data_c->data[i][j]);
		}
		unsigned char answer;
		label_gcc += fread(&answer, sizeof(unsigned char), 1, label_fp) * sizeof(unsigned char); 
		data_c->answers[i] = answer;
	}
	printf("Read %d arrays of size %dx%d (%lu bytes) from %s\n", image_count, image_rows, image_cols,  image_gcc, image_file);
		printf("Read %d labels (%lu bytes) from %s\n", label_count, label_gcc, label_file);
	fclose(image_fp);
	fclose(label_fp);
	return data_c;
}

void write_mnist_data(struct data_collection *mnist_data, char *data_file){
  FILE *fp = fopen(data_file, "wb");
  if(!fp){
    fprintf(stderr, "CANNOT OPEN FILE %s\n", data_file);
    abort();
  }
  fwrite(&mnist_data->num_arrays, sizeof(mnist_data->num_arrays), 1, fp);
  fwrite(&mnist_data->size, sizeof(mnist_data->size), 1, fp);
  for(int i = 0; i < mnist_data->num_arrays; i++){
		for(int i = 0; i < mnist_data->size; i++){
			fwrite(mnist_data->data[i], sizeof(*mnist_data->data[i]), mnist_data->size, fp);
		}
		fwrite(&mnist_data->answers[i], sizeof(mnist_data->answers[i]), 1, fp);
  }
  fclose(fp);
}

void generate_mnist_images(struct data_collection *m){
  pxinfo **new = malloc(sizeof(pxinfo*) * m->size);
	for(int i = 0; i < src_img->height; i++){
		new[i] = malloc(sizeof(pxinfo) * src_img->size);
	}
	for(int pic = 0; pic < m->num_arrays; pic++){
	  for(int r = 0; r < m->size; r++){
	    for(int c = 0; c < m->size; c++){
	      new[r][c].r = m->data[pic][r][c];
			  new[r][c].g = m->data[pic][r][c];
			  new[r][c].b = m->data[pic][r][c];
			  new[r][c].a = 255;
	    }
	  }
	  image *i = create_image(m->size, m->size, new);
	  char *filename = malloc(sizeof(char) * 25);
	  sprintf(filename, "./Training_Samples/training_sample%d.png", pic);
	  write_to_png(i, filename);
	  free(filename);
	  free(i);
	}
	
}
