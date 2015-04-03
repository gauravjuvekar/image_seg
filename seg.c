#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<inttypes.h>
#include<stddef.h>
#include"interface.h"
#include"region.h"

/* Usage:
 * ./a.out file threshold minimum_segment_pixels segment
 *
 */

int main(int argc, char *argv[]) {
	char *file;
	if (argc == 5) {
		file = argv[1];
	}
	else {
		fprintf(
			stderr,
		"Usage: %s file threshold minimum_pixels_per_segment segment_no\n",
			argv[0]
		);
		exit(-1);
	}

	unsigned int threshold = (unsigned int) atoi(argv[2]);
	unsigned int min_pixels_per_segment = (unsigned int) atoi(argv[3]);
	unsigned int segment_number_input = (unsigned int) atoi(argv[4]);

	FILE *input = fopen(file, "r+b");
	fseek(input, 0, SEEK_SET);

	assert(input != NULL);

	uint16_t buffer16;
	fread(&buffer16, 1, sizeof(buffer16), input);
	assert( buffer16 == 0x4d42); // Check if file is a bmp

	uint32_t buffer32;
	ptrdiff_t pixel_array_offset;
	
	fseek(input,10,SEEK_SET);
	fread(&buffer32, 1, sizeof(buffer32), input);
	pixel_array_offset = (ptrdiff_t)buffer32;

	/* Get dimensions from file
	 */
	int height, width;
	
	fseek(input,0x12,SEEK_SET);
	fread(&buffer32, 1, sizeof(buffer32), input);
	width = (int)buffer32;

	fseek(input,0x16,SEEK_SET);
	fread(&buffer32, 1, sizeof(buffer32), input);
	height = (int)buffer32;

	struct pixel *pixel_array = (struct pixel*)calloc(
			width*height,
			sizeof(struct pixel)
	);
	assert(pixel_array != NULL);

	fseek(input, pixel_array_offset, SEEK_SET);
	int i,j;
	for (i = 0 ; i < height ; ++i) {
		for (j = 0 ; j < width ; ++j) {
			fread(&pixel_array[i*width+j], 3, sizeof(uint8_t), input);
			pixel_array[i*width+j].x=j;
			pixel_array[i*width+j].y=i;
		}
		fseek(input,(4-((width*3)%4))%4,SEEK_CUR);
	}




	/* Actual grow called.
	 */

	int segment_map[ (height*width)/min_pixels_per_segment + 10][2];
	//TODO:find exact
	// keep segment number and segment count
	// index translation for valid segments + pixels in valid segments

	int segment_count = 1;
	int valid_segment_count = 0;
	unsigned int pixels_in_segment = 0;

	for (i = 0 ; i < height ; ++i) {
		for (j = 0 ; j < width ; ++j) {

			struct pixel *current = get(pixel_array, j, i, width, height);

			if(current -> segment == 0) {
				pixels_in_segment = grow(
						current,
						segment_count,
						pixel_array,
						width,
						height,
						threshold
				); 

				if (pixels_in_segment >= min_pixels_per_segment) {
					valid_segment_count++;
					segment_map[valid_segment_count][0] = segment_count;
					segment_map[valid_segment_count][1] = pixels_in_segment;
				}
				segment_count++;
			}
		}
	}


	/* Print in ascii
	 */
	for (i = height - 1; i >= 0 ; --i) {
		for (j = 0 ; j < width ; ++j) {
			if (get(
						pixel_array,
						j, i,
						width, height
				)->segment == segment_map[segment_number_input][0]) {
				printf("#");
			}
			else {
				printf(".");
			}
		}
			printf("\n");
	}


	/*Stats*/
	puts("\n\n\n--------------------------------------------\n");
	printf("Total segments: %d\n", valid_segment_count );
	for (i = 1; i <= valid_segment_count ; i++, segment_count = 0 ) {
		printf("Segment %d \t has %d pixels\n", i, segment_map[i][1]);
	}
}
