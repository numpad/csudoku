#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"

#include "imgutil.h"
#include "process_image.h"

/* print usage information */
void usage(const char *arg0) {
	puts  ("usage:");
	printf("  %s <filename>\n", arg0);
}

int main(int argc, char *argv[]) {
	if (argc == 1)
		return usage(argv[0]), 1;

	/* load image (hopefully) of sudoku field + margin containing unecessary data*/
	unsigned error;
	unsigned char *image;
	unsigned width, height;

	if ( (error = lodepng_decode32_file(&image, &width, &height, argv[1])) )
		return printf("error %u: %s\n", error, lodepng_error_text(error)), error;
	
	/* process image */
	imgu_image img_data = imgu_new(image, width, height);
	char input[128];
	while (printf("operation: "), scanf("%s", input) != EOF) {
		if (!strcmp(input, "edge")) {
			printf("Factor (default=1,-4): ");
			float a, b;
			scanf("%f,%f", &a, &b);
			process_image_edgedetect(&img_data, a, b);
			puts("Operation complete!\n");
		} else if (!strcmp(input, "lighten")) {
			printf("Factor (default=1.3): ");
			float factor;
			scanf("%f", &factor);
			process_image_lighten(&img_data, factor);
			puts("Operation complete!\n");
		} else if (!strcmp(input, "blur")) {
			printf("Factor (default=1.11): ");
			float factor;
			scanf("%f", &factor);
			process_image_blur(&img_data, factor);
			puts("Operation complete!\n");
		} else if (!strcmp(input, "sharpen")) {
			printf("Factor (default=-0.33,2.33): ");
			float a, b;
			scanf("%f,%f", &a, &b);
			process_image_sharpen(&img_data, a, b);
			puts("Operation complete!\n");
		} else if (!strcmp(input, "grayscale")) {
			process_image_grayscale(&img_data);
			puts("Operation complete!\n");
		} else if (!strcmp(input, "tobits")) {
			unsigned char val;
			scanf("%hhu", &val);
			process_image_tobits(&img_data, val);
			puts("Operation complete!\n");
		} else {
			puts("custom data:");
			float filter[9];
			for (int i = 0; i < 9; i += 3) {
				printf("data (%d-%d): ", i + 1, i + 3);
				scanf("%f,%f,%f", &filter[i], &filter[i + 1], &filter[i + 2]);
			}
			
			imgu_filter(&img_data, filter);
		}
	}
	//process_image_crop(&img_data);

	printf("\nDone!\n");
	
	lodepng_encode32_file("test_result.png", img_data.rgba, img_data.width, img_data.height);
	
	imgu_destroy(&img_data);
	return 0;
}
