#ifndef _G_IMAGE_H
#define _G_IMAGE_H 1

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <png.h>
typedef struct pxinfo{
	float r, g, b, a, lum;
} pxinfo;
typedef struct pngimage{
	int width, height;
	pxinfo **px;
	png_bytep *row_pointers;
} pngimage;
image* create_image(int width, int height, pxinfo **px, png_bytep *row_pointers);
image* extract_from_png(const char* filename);
void process(pngimage*);
bool write_to_png(pngimage* image, const char* filename);
#endif
