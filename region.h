#pragma once
#include<stdbool.h>
#include"interface.h"

bool is_similar(
		struct pixel *pixel1,
		struct pixel *pixel2,
		unsigned int threshold);

void grow(struct pixel *pixel, unsigned int segment,
		struct pixel *array,
		const unsigned int width,
		const unsigned int height, unsigned int threshold);
