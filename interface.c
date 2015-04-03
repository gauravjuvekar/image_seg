#include"interface.h"
#include<stdlib.h>

struct pixel *get(
		struct pixel array[],
		int x,
		int y,
		const unsigned int width,
		const unsigned int height) {

	if (x < 0 || y < 0 || x >= width || y >= height) {
		return NULL;
	}
	
	return &array[y*width + x];
}


