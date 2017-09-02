#ifndef _G_IMAGE_H
#define _G_IMAGE_H 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <png.h>
typedef struct image{
	int width, height;
	png_bytep *row_pointers;
} image;

image* create_image(int, int, png_bytep*);
image* extract_from_png(char*);
void process(image*);
bool write_to_png(image*, char*);

#endif