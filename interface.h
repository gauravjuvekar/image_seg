#pragma once
#include<inttypes.h>

struct pixel {
	uint8_t b,g,r;
	int segment;
	unsigned int x,y;
};


struct pixel *get(
		struct pixel array[],
		int x,
		int y,
		const unsigned int width,
		const unsigned int height
); 

