#include "quantify.h"
#include "gimage.h"
#include <math.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

struct image* getData(pngimage *pngi, int size){
	if(pngi->width != size || pngi->height != size) {
		printf("Size is not %dx%d!\n", size, size);
		return NULL;
	}
	struct image *i = (struct pixelarr*)malloc(sizoef(struct image));
	i->size = size;
	struct pixel ***data = (struct pixel***)malloc(sizeof(struct pixel**) * size);
	for(int j = 0; j < size; j++){
		data[j] = (struct pixel**)malloc(sizeof(struct pixel*) * size);
	}
	
	for(int y = 0; y < i->height; y++) {
		for(int x = 0; x < i->width; x++) {
			data[y][x] = (struct pixel*)malloc(sizeof(struct pixel));
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
			continue;    /* skip self and parent */
		}
		count++;
	}
	closedir(fd);
	return count;
}

struct data_collection* create_data(int num_arrays, int size, int depth){
	struct data_collection *data_c = (struct data_collection*)malloc(sizeof(struct data_collection));
	struct pixel ***data = (struct pixel***)malloc(sizeof(struct pixel**) * num_arrays);
	struct pixel ***answers = (struct pixel***)malloc(sizeof(struct pixel**) * num_arrays);
	for(int i = 0; i < num_arrays; i++){
		data[i] = (struct pixel**)malloc(sizeof(struct pixel*) * size);
		answers[i] = (struct pixel**)malloc(sizeof(struct pixel*) * size);
		for(int j = 0; j < size; j++){
			data[i][j] = (struct pixel*)malloc(sizeof(struct pixel) * size);
			answers[i][j] = (struct pixel*)malloc(sizeof(struct pixel) * size);
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
	int num_arrays, size, depth;
	size_t gcc = 0;
	gcc += fread(&num_arrays, sizeof(int), 1, fp) * sizeof(int);
	gcc += fread(&size, sizeof(int), 1, fp) * sizeof(int);
	gcc += fread(&depth, size(int), 1, fp) * sizeof(int);
	struct data_collection *data_c = create_data(num_arrays, size);
	
	int total = 0;
	for(int i = 0; i < num_arrays; i++){
		for(int j = 0; j < size; j++){
			gcc += fread(data_c->data[i][j], sizeof(*data_c->data[i][j]), size, fp) * sizeof(*data_c->data[i][j]);
		}
		unsigned char answer;
		gcc += fread(&answer, sizeof(unsigned char), 1, fp) * sizeof(unsigned char); 
		data_c->answers[i] = answer;
		if(answer == 0) total++;
	}
	//printf("read total zero answers = %d\n", total);
	printf("Read %d arrays of size %d (%lu bytes) from %s\n", num_arrays, size, gcc, data_file); 
	fclose(fp);
	return data_c;
}

void write_data(char *dir, int size, char *data_file, int depth){
	int num_files = count_files(dir);
	FILE *fp = fopen(data_file, "wb");
	fwrite(&num_files, sizeof(num_files), 1, fp);
	fwrite(&size, sizeof(size), 1, fp);
  fwrite(&depth, sizeof(depth), 1, fp));
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
		char *data_path = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(dp->d_name) + 20));
		sprintf(data_path, "%s/%s", dir, dp->d_name);
		pngimage *data = extract_from_png(data_path);
		
		char *answer_path = (char*)malloc(sizeof(char) * (strlen(dir) + strlen(dp->d_name) + 20));
		sprintf(answer_path, "%s/a_%s", dir, dp->d_name);
		pngimage *answer = extract_from_png(answer_path);
		
		
		
		
		// write data
		unsigned char **data = getData(data, size);
		for(int i = 0; i < size; i++){
			fwrite(data[i], sizeof(*data[i]), size, fp);
		}
		
		
		
		unsigned char answer = atoi(dp->d_name);
		printf("reading answer as %d\n", answer);
		fwrite(&answer, sizeof(unsigned char), 1, fp);
		
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
	  if(a%10!=0)continue;
		printf("ARRAY %d: (Answer = %c)\n", a, d->answers[a]+'a');
		for(int i = 0; i < d->size; i++){
			printf("{");
			for(int j = 0; j < d->size; j++){
				if(d->data[a][i][j] == 0){
					printf(j == d->size - 1 ? "-}" : "-, ");
				}else{
					printf(j == d->size - 1 ? "X}" : "X, ");
				}
			}
			printf("\n");
		}
		getchar();
	}
	printf("End of file.\n");
}













