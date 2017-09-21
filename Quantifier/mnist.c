#include "mnist.h"
#include "quantify.h"
#include "gimage.h"
#include <png.h>
#include <byteswap.h>
#include <sys/stat.h>
#include <sys/types.h>

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
	printf("files opened\n");
	size_t image_gcc = 0, label_gcc = 0;
	int image_magic, image_count, image_rows, image_cols;
	image_gcc += fread(&image_magic, sizeof(int), 1, image_fp) * sizeof(int);
	image_gcc += fread(&image_count, sizeof(int), 1, image_fp) * sizeof(int);
	image_gcc += fread(&image_rows, sizeof(int), 1, image_fp) * sizeof(int);
	image_gcc += fread(&image_cols, sizeof(int), 1, image_fp) * sizeof(int);
	// big endian -> host endian
	image_magic = bswap_32(image_magic);
	image_count = bswap_32(image_count);
	image_rows = bswap_32(image_rows);
	image_cols = bswap_32(image_cols);
	int label_magic, label_count;
	label_gcc += fread(&label_magic, sizeof(int), 1, label_fp) * sizeof(int);
	label_gcc += fread(&label_count, sizeof(int), 1, label_fp) * sizeof(int);
	// big endian -> host endian
	label_magic = bswap_32(label_magic);
	label_count = bswap_32(label_count);
	printf("image count = %d rows = %d cols = %d\n", image_count, image_rows, image_cols);
	printf("label count = %d\n", image_count);
	getchar();
	if(image_count != label_count){
	  fprintf(stderr, "mismatched image and label count (%d and %d)\n", image_count, label_count);
	}
	if(image_rows != image_cols){
	  fprintf(stderr, "image rows and cols not equal (%d and %d)\n", image_rows, image_cols);
	}
	struct data_collection *data_c = create_data(image_count, image_rows);
	int totalZeros = 0;
	for(int i = 0; i < image_count; i++){
		for(int j = 0; j < image_rows; j++){
			image_gcc += fread(data_c->data[i][j], sizeof(*data_c->data[i][j]), image_cols, image_fp) * sizeof(*data_c->data[i][j]);
		}
		unsigned char answer;
		label_gcc += fread(&answer, sizeof(unsigned char), 1, label_fp) * sizeof(unsigned char); 
		data_c->answers[i] = answer;
		if(answer == 0) totalZeros++;
		printf("read image %d / %d\n", i, image_count);
	}
	printf("total zeros ans = %d\n", totalZeros);
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
		for(int j = 0; j < mnist_data->size; j++){
			fwrite(mnist_data->data[i][j], sizeof(*mnist_data->data[i][j]), mnist_data->size, fp);
		}
		fwrite(&mnist_data->answers[i], sizeof(mnist_data->answers[i]), 1, fp);
  }
  fclose(fp);
}

void generate_mnist_images(struct data_collection *m){
  pxinfo **new = malloc(sizeof(pxinfo*) * m->size);
	for(int i = 0; i < m->size; i++){
		new[i] = malloc(sizeof(pxinfo) * m->size);
	}
	for(int pic = 0; pic < m->num_arrays; pic++){
	  for(int r = 0; r < m->size; r++){
	    for(int c = 0; c < m->size; c++){
	      new[r][c].r = 255 - m->data[pic][r][c];
			  new[r][c].g = 255 - m->data[pic][r][c];
			  new[r][c].b = 255 - m->data[pic][r][c];
			  new[r][c].a = 255;
	    }
	  }
	  
	  char *template_filename = "template.png";
	  image *i;
	  if(!(i = extract_from_png(template_filename))){
		  fprintf(stderr, "CANNOT READ FILE %s\n", template_filename);
	  }
    i->px = new;
	  char *filename = malloc(sizeof(char) * 60);
	  mkdir("./Testing_Samples", 0777);
	  sprintf(filename, "./Testing_Samples/training_sample%d_%u.png", pic, m->answers[pic]);
	  write_to_png(i, filename);
	  //free(filename);
	  //free(i);
	}
	
}
