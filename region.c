#include"region.h"
#include<stdlib.h>
#include<stdio.h>

bool is_similar(
		struct pixel *pixel1,
		struct pixel *pixel2,
		unsigned int threshold) {
	unsigned int diff = abs(pixel1 -> r - pixel2 -> r) + 
	abs(pixel1 -> g - pixel2 -> g) + 
	abs(pixel1 -> b - pixel2 -> b);
	return diff<threshold;
}

void grow(struct pixel *pixel, unsigned int segment,
		struct pixel *array,
		const unsigned int width,
		const unsigned int height,
		unsigned int threshold) {
	int x,y;
	for(x = -1; x < 2; x++){
		for(y=-1;y<2;y++) {
			struct pixel *adjacent = get(
					array,
					pixel->x + x,
					pixel -> y + y,
					width,
					height);
			if(adjacent != NULL){
				if (adjacent-> segment == 0) {
					if (is_similar(pixel, adjacent, threshold)) {
						adjacent -> segment = segment;
						/*printf("%d %d added to %d\n", adjacent->x, adjacent->y, segment);*/
						grow(adjacent, segment, array, width, height, threshold);
					}
				}
			}
		}
	}
}

