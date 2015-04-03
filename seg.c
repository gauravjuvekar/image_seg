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
		fprintf(stderr, "Usage: %s file threshold minimum_pixels_per_segment segment_no\n", argv[0]);
		exit(-1);
	}

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

	int32_t height, width;

	fseek(input,0x12,SEEK_SET);
	fread(&buffer32, 1, sizeof(buffer32), input);
	width = (int32_t)buffer32;

	fseek(input,0x16,SEEK_SET);
	fread(&buffer32, 1, sizeof(buffer32), input);
	height = (int32_t)buffer32;

	struct pixel *pixel_array = (struct pixel*)calloc(width * height, sizeof(struct pixel));
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




/*Actual grow called.	*/
	int segment_count = 1;
	for (i = 0 ; i < height ; ++i) {
		for (j = 0 ; j < width ; ++j) {
			struct pixel *current = get(pixel_array, j, i , width, height);
			if(current -> segment == 0) {
				grow(current ,segment_count, pixel_array, width, height, (unsigned int)atoi(argv[2]));
				segment_count++;
			}
		}
	}

	/* prune tiny segments */
	int tmp_seg_count = 0;
	int new_count = 1;

	for (i = 1; i <=segment_count; i++, tmp_seg_count = 0 ) {
		for (j = 0 ; j < width * height ; j++) {
			if (pixel_array[j].segment == i) {
				tmp_seg_count++;
			}
		}
		if (tmp_seg_count < atoi(argv[3])) {	
			for (j = 0 ; j < width * height ; j++) {
				if (pixel_array[j].segment == i) {
					pixel_array[j].segment = 0;
				}
			}
		}
		else {
			for (j = 0 ; j < width * height ; j++) {
				if (pixel_array[j].segment == i) {
					pixel_array[j].segment = new_count;
				}
			}
			new_count++;
		}
	}

/*Print in ascii*/
	for (i = height - 1; i >= 0 ; --i) {
		for (j = 0 ; j < width ; ++j) {
			if (get(pixel_array, j, i , width, height) -> segment == atoi(argv[4])) {
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
	printf("Total segments: %d\n", new_count - 1);
	for (i = 1; i < new_count ; i++, segment_count = 0 ) {
		for (j = 0 ; j < width * height ; j++) {
			if (pixel_array[j].segment == i) {
				segment_count++;
			}
		}
		printf("Segment %d \t has %d pixels\n", i, segment_count);
	}
}
