#include "quantify.h"
#include "gimage.h"
#include <math.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

int** getData(image *i, int size){
	int **data = (int**)malloc(sizeof(int*) * size);
	for(int i = 0; i < size; i++){
		data[i] = (int*)malloc(sizeof(int) * size);
	}
	if(i->width != size || i->height != size) {
		printf("Size is not %dx%d!\n", size, size);
		return NULL;
	}
	for(int y = 0; y < i->height; y++) {
		for(int x = 0; x < i->width; x++) {
			png_bytep px = &(i->row_pointers[y][(x) * 4]);
			if(px[0] == 255 && px[1] == 255 && px[2] == 255){
				data[y][x] = 0;
			}else{
				data[y][x] = 1;
			}
//			printf("r = %d, g = %d, b = %d, a = %d\n", px[0], px[1], px[2], px[3]);
		}
	}
	return data;
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
			continue;    /* skip self and parent */
		}
		count++;
	}
	closedir(fd);
	return count;
}

struct data_collection* create_data(int num_arrays, int size){
	struct data_collection *data_c = (struct data_collection*)malloc(sizeof(struct data_collection));
	unsigned char ***data = (unsigned char***)malloc(sizeof(unsigned char**) * num_arrays);
	unsigned char *answers = (unsigned char*)malloc(sizeof(unsigned char) * num_arrays);
	for(int i = 0; i < num_arrays; i++){
		data[i] = (unsigned char**)malloc(sizeof(unsigned char*) * size);
		for(int j = 0; j < size; j++){
			data[i][j] = (unsigned char*)malloc(sizeof(unsigned char) * size);
		}
	}
	data_c->data = data;
	data_c->answers = answers;
	data_c->num_arrays = num_arrays;
	data_c->size = size;
	return data_c;
}

void destroy_data(struct data_collection *d){
	free(d->answers);
	for(int i = 0; i < d->num_arrays; i++){
		for(int j = 0; j < d->size; j++){
			free(d->data[i][j]);
		}
		free(d->data[i]);
	}
	free(d->data);
}

struct data_collection* read_data(char *data_file){
	FILE *fp = fopen(data_file, "rb");
	if(!fp){
		fprintf(stderr, "FILE %s NOT FOUND\n", data_file);
		abort();
	}
	int num_arrays, size;
	size_t gcc = 0;
	gcc += fread(&num_arrays, sizeof(int), 1, fp) * sizeof(int);
	gcc += fread(&size, sizeof(int), 1, fp) * sizeof(int);
	struct data_collection *data_c = create_data(num_arrays, size);
	for(int i = 0; i < num_arrays; i++){
		for(int j = 0; j < size; j++){
			gcc += fread(data_c->data[i][j], sizeof(*data_c->data[i][j]), size, fp) * sizeof(*data_c->data[i][j]);
		}
		int answer;
		gcc += fread(&answer, sizeof(int), 1, fp) * sizeof(int); 
		data_c->answers[i] = answer;
	}
	printf("Read %d arrays of size %d (%lu bytes) from %s\n", num_arrays, size, gcc, data_file); 
	fclose(fp);
	return data_c;
}

void write_data(char *dir, int size, char *data_file){
	int num_files = count_files(dir);
	FILE *fp = fopen(data_file, "wb");
	fwrite(&num_files, sizeof(num_files), 1, fp);
	fwrite(&size, sizeof(size), 1, fp);

	struct dirent *dp;
	DIR *fd;
	if(!fp) printf("Error opening file\n");
	if((fd = opendir(dir)) == NULL){
		fprintf(stderr, "listdir: can't open %s\n", dir);
		return;
	}
	while((dp = readdir(fd)) != NULL){
		if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")){
			continue;    /* skip self and parent */
		}
		char *path = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(dp->d_name) + 20));
		sprintf(path, "%s/%s", dir, dp->d_name);
		image *i = extract_from_png(path);
		
		// write data
		int **data = getData(i, size);
		for(int i = 0; i < size; i++){
			fwrite(data[i], sizeof(*data[i]), size, fp);
		}
		int answer = atoi(dp->d_name);
		fwrite(&answer, sizeof(int), 1, fp);
		
		/* Freeing section */
		for(int i = 0; i < size; i++){
			free(data[i]);
		}
		free(data);
		free(path);
		free(i);
	}
	closedir(fd);
	fclose(fp);
}

// only for square arrays
void print_data(struct data_collection *d){
	for(int a = 0; a < d->num_arrays; a++){
		printf("ARRAY %d: (Answer = %d)\n", a, d->answers[a]);
		for(int i = 0; i < d->size; i++){
			printf("{");
			for(int j = 0; j < d->size; j++){
				if(d->data[a][i][j] == 0){
					printf(j == d->size - 1 ? "-}" : "-, ");
				}else{
					printf(j == d->size - 1 ? "%d}" : "%d, ", d->data[a][i][j]);
				}
			}
			printf("\n");
		}
		getchar();
	}
	printf("End of file.\n");
}













