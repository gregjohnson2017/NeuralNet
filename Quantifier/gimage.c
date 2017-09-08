/* Adapted from Guillaume Cottenceau's and Yoshimasa Niwa's work */
#include "gimage.h"
#include <stdio.h>
image* create_image(int width, int height, pxinfo **px, png_bytep *row_pointers){
	image *i = malloc(sizeof(image));
	i->width = width;
	i->height = height;
	i->px = px;
	i->row_pointers = row_pointers;
	return i;
}

image* extract_from_png(const char* filename){
	int width, height;
	png_byte color_type, bit_depth;
	png_bytep *row_pointers;
	FILE *fp = fopen(filename, "rb");
	if(!fp) return NULL;
	
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png) return NULL;
	png_infop info = png_create_info_struct(png);
	if(!info) return NULL;
	if(setjmp(png_jmpbuf(png))) return NULL;

	png_init_io(png, fp);
	png_read_info(png, info);

	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	color_type = png_get_color_type(png, info);
	bit_depth = png_get_bit_depth(png, info);

	if(bit_depth == 16){
		png_set_strip_16(png);
	}
	if(color_type == PNG_COLOR_TYPE_PALETTE){
		png_set_palette_to_rgb(png);
	}

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8){
		png_set_expand_gray_1_2_4_to_8(png);
	}
	if(png_get_valid(png, info, PNG_INFO_tRNS)){
		png_set_tRNS_to_alpha(png);
	}

	// These color_type don't have an alpha channel then fill it with 0xff.
	if(color_type == PNG_COLOR_TYPE_RGB || color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_PALETTE){
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	}

	if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
		png_set_gray_to_rgb(png);
	}

	png_read_update_info(png, info);

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for(int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
	}
	png_read_image(png, row_pointers);
	fclose(fp);
	
	pxinfo **px = malloc(sizeof(pxinfo*) * height);
	for(int i = 0; i < height; i++){
		px[i] = malloc(sizeof(pxinfo) * width);
	}
	for(int y = 0; y < height; y++) {
		png_bytep row = row_pointers[y];
		for(int x = 0; x < width; x++) {
			png_bytep data = &(row[x * 4]);
			px[y][x].r = data[0];
			px[y][x].g = data[1];
			px[y][x].b = data[2];
			px[y][x].a = data[3];
			px[y][x].lum = 0.2126f * data[0] + 0.7152f * data[1] + 0.0722f * data[2];
		}
	}
	image *image = create_image(width, height, px, row_pointers);
	return image;
}

bool write_to_png(image* image, const char* filename){
	FILE *fp = fopen(filename, "wb");
	if(!fp) return NULL;

	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png) return NULL;

	png_infop info = png_create_info_struct(png);
	if (!info) return NULL;
	if (setjmp(png_jmpbuf(png))) return NULL;

	png_init_io(png, fp);

	// Output is 8bit depth, RGBA format.
	png_set_IHDR(
		png,
		info,
		image->width, image->height,
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);
	png_write_info(png, info);
	for(int y = 0; y < image->height; y++) {
		png_bytep row = image->row_pointers[y];
		for(int x = 0; x < image->width; x++) {
			png_bytep data = &(row[x * 4]);
			data[0] = image->px[y][x].r;
			data[1] = image->px[y][x].g;
			data[2] = image->px[y][x].b;
			data[3] = image->px[y][x].a;
		}
	}
	png_write_image(png, image->row_pointers);
	png_write_end(png, NULL);

	for(int y = 0; y < image->height; y++) {
		free(image->px[y]);
	}
	free(image->px);
	fclose(fp);
	return true;
}

void process(image* image){
	for(int y = 0; y < image->height; y++) {
		png_bytep row = image->row_pointers[y];
		for(int x = 0; x < image->width; x++) {
			png_bytep px = &(row[x * 4]);
			float luminance = 0.2126f * px[0] + 0.7152f * px[1] + 0.0722f * px[2];
			if(px[3] > 0){
				px[0] = 0;
				px[1] = 0;
				px[2] = 0;
				px[3] = 255-luminance;
			}
			//printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
		}
	}
}
