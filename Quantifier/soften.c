#include "gimage.h"
#include "matrix.h"
#include <stdbool.h>
#include <math.h>

bool soften(const char*, const char*, unsigned, float);
matrix get_gaussian_filter(unsigned, float);

int main(int argc, char **argv){
	if(argc != 5){
		printf("Usage: soften <original.png> <new.png> <filter size> <intensity>\n");
		return 0;
	}
	if(soften(argv[1], argv[2], atoi(argv[3]), atof(argv[4]))){
		printf("\"%s\" softened successfully to \"%s\"\n", argv[1], argv[2]);
	}else{
		printf("Error: invalid file name(s)\n");
	}
}

bool soften(const char *src, const char *dst, unsigned size, float sigma){
	if(size < 3) return false;
	if(size % 2 == 0) return false;
	pngimage *src_img;
	if(!(src_img = extract_from_png(src))){
		return false;
	}
	pxinfo **new = malloc(sizeof(pxinfo*) * (src_img->height + size - 1));
	for(int i = 0; i < src_img->height; i++){
		new[i] = malloc(sizeof(pxinfo) * (src_img->width + size - 1));
	}
	matrix filter = get_gaussian_filter(size, sigma);
	for(int r = 0; r < src_img->height; r++){
		for(int c = 0; c < src_img->width; c++){
			if(r < ((size - 1) / 2) || r > (src_img->height - 1 - ((size - 1) / 2)) ||
			 c < ((size - 1) / 2) || c > (src_img->width - 1 - ((size - 1) / 2))){
        new[r][c].r = 255;
        new[r][c].g = 255;
        new[r][c].b = 255;
        new[r][c].a = 255;
			  continue;
			}
			for(int i = 0; i < 4; i++){
				matrix sample = matrix_create(size, size);
				switch(i){
					case 0:
						for(int sample_r = 0; sample[sample_r]; sample_r++){
							for(int sample_c = 0; sample[sample_r][sample_c] == sample[sample_r][sample_c]; sample_c++){
								sample[sample_r][sample_c] =
								src_img->px[r - ((size - 1) / 2) + sample_r]
								[c - ((size - 1) / 2) + sample_c].r;
							}				
						}
						new[r][c].r = matrix_convolve(filter, sample);
						break;
					case 1:
						for(int sample_r = 0; sample[sample_r]; sample_r++){
							for(int sample_c = 0; sample[sample_r][sample_c] == sample[sample_r][sample_c]; sample_c++){
								sample[sample_r][sample_c] = 
								src_img->px[r - ((size - 1) / 2) + sample_r]
								[c - ((size - 1) / 2) + sample_c].g;
							}				
						}		
						new[r][c].g = matrix_convolve(filter, sample);
						break;
					case 2:
						for(int sample_r = 0; sample[sample_r]; sample_r++){
							for(int sample_c = 0; sample[sample_r][sample_c] == sample[sample_r][sample_c]; sample_c++){
								sample[sample_r][sample_c] = 
								src_img->px[r - ((size - 1) / 2) + sample_r]
								[c - ((size - 1) / 2) + sample_c].b;
							}				
						}		
						new[r][c].b = matrix_convolve(filter, sample);
						break;
					case 3:
						for(int sample_r = 0; sample[sample_r]; sample_r++){
							for(int sample_c = 0; sample[sample_r][sample_c] == sample[sample_r][sample_c]; sample_c++){
								sample[sample_r][sample_c] = 
								src_img->px[r - ((size - 1) / 2) + sample_r]
								[c - ((size - 1) / 2) + sample_c].a;
							}				
						}	
						new[r][c].a = 255;//(1 / 159) * matrix_convolve(filter, sample);
						break;
				}
				
				matrix_delete(sample);
			}
			new[r][c].lum = 0.2126f * new[r][c].r + 0.7152f * new[r][c].g + 0.0722f * new[r][c].b;
		}
	}
	matrix_delete(filter);
	src_img->px = new;
	if(write_to_png(src_img, dst)){
		return true;
	}else{
		printf("\"%s\" could not be written to.\n", dst);
		return false;
	}
}

matrix get_gaussian_filter(unsigned size, float sigma){
	printf("building filter with size: %u and sigma %f\n", size, sigma);
	matrix filter = matrix_create(size, size);
	if(size % 2 == 0) return NULL;
	float k = (size - 1) / 2;
	for(int r = 0; filter[r]; r++){
		for(int c = 0; filter[r][c] == filter[r][c]; c++){
			float a = (1.0 / ( 2.0 * M_PI * powf(sigma, 2.0))) * 
			powf(M_E, -1.0 * (powf(r + 1 - (k + 1.0), 2.0) +
			powf(c + 1 - (k + 1.0), 2.0)) /
			(2.0 * powf(sigma, 2.0)));
			filter[r][c] = a;
		}
	}
	float divisor = filter[0][0];
	for(int r = 0; filter[r]; r++){
		for(int c = 0; filter[r][c] == filter[r][c]; c++){
			filter[r][c] = (int)(filter[r][c]/divisor);
			
		}
	}
	return filter;
}

