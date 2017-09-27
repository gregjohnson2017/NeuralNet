#include "quantify.h"
#include "gimage.h"
#include <math.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

image* get_image_data(pngimage *pngi, int size){
	if(pngi->width != size || pngi->height != size) {
		printf("Size is not %dx%d!\n", size, size);
		return NULL;
	}
  image *i = (pixelarr*)malloc(sizeof(image));
	i->size = size;
	pixel ***data = (pixel***)malloc(sizeof(pixel**) * size);
	for(int j = 0; j < size; j++){
		data[j] = (pixel**)malloc(sizeof(pixel*) * size);
	}
	
	for(int y = 0; y < i->height; y++) {
		for(int x = 0; x < i->width; x++) {
			data[y][x] = (pixel*)malloc(sizeof(pixel));
			data[y][x]->r = pngi->px[0];
			data[y][x]->g = pngi->px[1];
			data[y][x]->b = pngi->px[2];
			data[y][x]->a = pngi->px[3];
		}
	}
	
	i->data = data;
	return i;
}

/*
Taken from https://stackoverflow.com/questions/12075864/how-to-get-the-number-of-files-in-the-specific-folder-using-c-or-c-function
*/
int count_files(char *dir){
	int count = 0;
	struct dirent *dp;
	DIR *fd;
	if((fd = opendir(dir)) == NULL){
		fprintf(stderr, "listdir: can't open %s\n", dir);
		return -1;
	}
	while((dp = readdir(fd)) != NULL){
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
			continue;		/* skip self and parent */
		}
		count++;
	}
	closedir(fd);
	return count;
}

image* allocate_image(int size){
	image *img = (image*)malloc(sizeof(image));
	pixel ***data = (pixel***)malloc(sizeof(pixel**) * size);
	for (i = 0; i < size; i++){
		data[i] = (pixel**)malloc(sizeof(pixel*) * size);
		for (j = 0; j < size; j++){
		  data[i][j] = (pixel*)malloc(sizeof(pixel));
		}
	}
	img->data = data;
	return img;
}

void free_image(image *img){
  for (i = 0; i < img->size; i++){
    for (j = 0; j < img->size; j++){
      free(img->data[i][j])
    }
    free(img->data[i])
  }
  free(img->data);
  free(img);
}

data_collection* create_data(int num_arrays, int size){
	data_collection *data_c = (data_collection*)malloc(sizeof(data_collection));
	image **data = (image**)malloc(sizeof(image*) * num_arrays);
	image **answers = (image**)malloc(sizeof(image*) * num_arrays);
	for(int i = 0; i < num_arrays; i++){
		data[i] = allocate_image(size);
		answers[i] = allocate_image(size);
	}
	data_c->data = data;
	data_c->answers = answers;
	data_c->num_arrays = num_arrays;
	data_c->size = size;
	return data_c;
}

void destroy_data( data_collection *d){
  printf("attempting to destroy data\n");
	for(int i = 0; i < d->num_arrays; i++){
	  free_image(data[i]);
	  free_image(answers[i]);
	}
	free(d);
	printf("data destroyed successfully\n");
}

/*
read struct data_collection from file.
format:
int (num_arrays) number of samples
int (size) square X by X
struct image (data[0]) pixel by pixel
struct image (answers[0]) pixel by pixel
...
struct image (data[n])
struct image (answers[n])
EOF
*/
data_collection* read_data(char *data_file){
	FILE *fp = fopen(data_file, "rb");
	if(!fp){
		fprintf(stderr, "FILE %s NOT FOUND\n", data_file);
		abort();
	}
	int num_arrays, size;
	size_t gcc = 0;
	gcc += fread(&num_arrays, sizeof(int), 1, fp) * sizeof(int);
	gcc += fread(&size, sizeof(int), 1, fp) * sizeof(int);
	data_collection *data_c = create_data(num_arrays, size);
	
	for(int i = 0; i < num_arrays; ++){
	  // reading pixel by pixel is sizeXsize image, sample then answer
    for (j = 0; j < size; j++){
      for (k = 0; k < size; k++){
        gcc += fread(data_c->data[i]->data[j][k], sizeof(*data_c->data[i]->data[j][k]), 1, fp);
      }
    }
		for (j = 0; j < size; j++){
      for (k = 0; k < size; k++){
        gcc += fread(data_c->answers[i]->data[j][k], sizeof(*data_c->answers[i]->data[j][k]), 1, fp);
      }
    }
	}
	printf("Read %d arrays of size %d (%lu bytes) from %s\n", num_arrays, size, gcc, data_file); 
	fclose(fp);
	return data_c;
}

/*
Every sample in sampleDir must have corresponding answer "a_<samplename>.png" in answerDir
*/
void create_data_file(char *sampleDir, char *answerDir, int size, char *data_file){
	int num_files = count_files(sampleDir);
	FILE *fp = fopen(data_file, "wb");
	
	fwrite(&num_files, sizeof(num_files), 1, fp);
	fwrite(&size, sizeof(size), 1, fp);
	
	struct dirent *dp;
	DIR *fd;
	if(!fp) printf("Error opening file\n");
	if((fd = opendir(sampleDir)) == NULL){
		fprintf(stderr, "listdir: can't open %s\n", sampleDir);
		return;
	}
	while((dp = readdir(fd)) != NULL){
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
			continue;		/* skip self and parent */
		}
		char *data_path = (char*)malloc(sizeof(char) * (strlen(sampleDir) + strlen(dp->d_name) + 20));
		sprintf(data_path, "%s/%s", sampleDir, dp->d_name);
		pngimage *data = extract_from_png(data_path);
		image *data_img = get_image_data(data, size);
		
    for (i = 0; i < size; i++){
      for (j = 0; j < size; j++){
        fwrite(data_img->data[i][j], sizeof(*data_img->data[i][j]), 1, fp);
      }
    }
		
		char *answer_path = (char*)malloc(sizeof(char) * (strlen(answerDir) + strlen(dp->d_name) + 20));
		sprintf(answer_path, "../%s/a_%s", answerDir, dp->d_name);
		pngimage *answer = extract_from_png(answer_path);
		image *answer_img = get_image_data(answer, size);
		
		 for (i = 0; i < size; i++){
      for (j = 0; j < size; j++){
        fwrite(answer_img->data[i][j], sizeof(*answer_img->data[i][j]), 1, fp);
      }
    }
		
		/* Freeing section */
		// XXX not freeing data or answer, dont have a free method yet for those
		printf("attempting free in create_data_file\n");
		free_image(answer_img);
		free_image(data_img);
		free(data_path);
		free(answer_path);
		printf("free successful\n");
	}
	closedir(fd);
	fclose(fp);
}


