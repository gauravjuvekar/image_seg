#pragma once
#include<stdbool.h>
#include"interface.h"

bool is_similar(
		struct pixel *pixel1,
		struct pixel *pixel2,
		unsigned int threshold
);

unsigned int grow(
		struct pixel *pixel,       // The pixel to grow from 
		unsigned int segment,      // The segment to assign to grown pixels
		struct pixel *array,       // Pointer to pixel buffer passed to get
		const unsigned int width,  // Image width, passed to get
		const unsigned int height, // Image height, passed to get
		unsigned int threshold     // Intensity diff threshold passed to
		                           // is_similar
);
