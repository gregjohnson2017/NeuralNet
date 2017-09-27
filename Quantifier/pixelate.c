#include "gimage.h"
#include <math.h>

bool pixelate(pngimage *i, int divisor){
	if(divisor < 2) return false;
	//if(i->width != i->height) return false;
	for(int y = 0; y + divisor <= i->height; y += divisor) {
		for(int x = 0; x + divisor <= i->width; x += divisor) {
			int total_red = 0, total_green = 0, total_blue = 0, total_alpha = 0;
			for(int yy = 0; yy < divisor; yy++){
				png_bytep row = i->row_pointers[y + yy];
				for(int xx = 0; xx < divisor; xx++){
					png_bytep px = &(row[(xx + x) * 4]);
					total_red += px[0];
					total_green += px[1];
					total_blue += px[2];
					total_alpha += px[3];
				}
			}
			float avg_red = powf(powf(total_red / (divisor * divisor), 2.0f), 0.5f);
			float avg_green = powf(powf(total_green / (divisor * divisor), 2.0f), 0.5f);
			float avg_blue = powf(powf(total_blue / (divisor * divisor), 2.0f), 0.5f);
			float avg_alpha = powf(powf(total_alpha / (divisor * divisor), 2.0f), 0.5f);
			for(int yy = 0; yy < divisor; yy++){
				for(int xx = 0; xx < divisor; xx++){
					i->px[y+yy][x+xx].r = avg_red;
					i->px[y+yy][x+xx].g = avg_green;
					i->px[y+yy][x+xx].b = avg_blue;
					i->px[y+yy][x+xx].a = avg_alpha;
				}
			}
			//float luminance = 0.2126f * px[0] + 0.7152f * px[1] + 0.0722f * px[2];
		}
	}
	return true;
}

int main(int argc, char **argv){
	if(argc != 4){
		printf("Usage: pixelate <original> <new> <divisor>\n");
		return 0;
	}
	pngimage *i = extract_from_png(argv[1]);
	if(pixelate(i, atoi(argv[3]))){
		printf("pixelation was successful\n");
	}else{
		printf("pixelation failed\n");
	}
	if(write_to_png(i, argv[2])){
		printf("write was successful\n");
	}else{
		printf("write failed\n");
	}
}
