#include"region.h"
#include<stdlib.h>
#include<stdio.h>


bool is_similar(
		struct pixel *pixel1,
		struct pixel *pixel2,
		unsigned int threshold) {

	unsigned int diff = 
		abs(pixel1->r - pixel2->r) + 
		abs(pixel1->g - pixel2->g) + 
		abs(pixel1->b - pixel2->b);

	return diff < threshold;
}


unsigned int grow(
		struct pixel *pixel,
		unsigned int segment,
		struct pixel *array,
		const unsigned int width,
		const unsigned int height,
		unsigned int threshold) {

	unsigned int count = 1;
	int x,y;
	for(    x = -1; x <= 1; x++) {
		for(y = -1; y <= 1; y++) {
			struct pixel *adjacent = get(
					array,
					(pixel->x + x),
					(pixel->y + y),
					width,
					height
					);

			if(
					adjacent       != NULL && //adjacent pixel is within image
					adjacent->segment == 0 && //adjacent pixel 
					                          //is not in a segment
					is_similar(pixel, adjacent, threshold)) {
				adjacent->segment = segment; //include adjacent in the segment
				count += grow(               //and grow from it
						adjacent,
						segment,
						array,
						width, height,
						threshold
				);
			}
		}
	}
	return count;
}

