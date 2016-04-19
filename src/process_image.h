#ifndef PROCESS_IMAGE_H
#define PROCESS_IMAGE_H

#include "imgutil.h"

void process_image_grayscale(imgu_image *img) {
	for (int y = 0; y < img->height; ++y) {
		for (int x = 0; x < img->width; ++x) {
			unsigned char r = imgu_getchannel(img, x, y, 0);
			unsigned char g = imgu_getchannel(img, x, y, 1);
			unsigned char b = imgu_getchannel(img, x, y, 2);

			unsigned char gray = (r + g + b) / 3;
			img->rgba[((x + y * img->width) * 4) + 0] = gray;
			img->rgba[((x + y * img->width) * 4) + 1] = gray;
			img->rgba[((x + y * img->width) * 4) + 2] = gray;
			img->rgba[((x + y * img->width) * 4) + 3] = 255;
		}
	}
}

void process_image_removedark(imgu_image *img, unsigned char max) {	
	for (int y = 0; y < img->height; ++y) {
		for (int x = 0; x < img->width; ++x) {
			unsigned char r = imgu_getchannel(img, x, y, 0);
			unsigned char g = imgu_getchannel(img, x, y, 1);
			unsigned char b = imgu_getchannel(img, x, y, 2);

			unsigned char gray = (r + g + b) / 3;
			if (gray < max) {
				img->rgba[((x + y * img->width) * 4) + 0] = 0;
				img->rgba[((x + y * img->width) * 4) + 1] = 0;
				img->rgba[((x + y * img->width) * 4) + 2] = 0;
				img->rgba[((x + y * img->width) * 4) + 3] = 255;
			}
		}
	}
}

void process_image_tobits(imgu_image *img, unsigned char max) {
	for (int y = 0; y < img->height; ++y) {
		for (int x = 0; x < img->width; ++x) {
			unsigned char r = imgu_getchannel(img, x, y, 0);
			unsigned char g = imgu_getchannel(img, x, y, 1);
			unsigned char b = imgu_getchannel(img, x, y, 2);

			unsigned char gray = (r + g + b) / 3;
			if (gray < max) {
				img->rgba[((x + y * img->width) * 4) + 0] = 0;
				img->rgba[((x + y * img->width) * 4) + 1] = 0;
				img->rgba[((x + y * img->width) * 4) + 2] = 0;
				img->rgba[((x + y * img->width) * 4) + 3] = 255;
			} else {
				img->rgba[((x + y * img->width) * 4) + 0] = 255;
				img->rgba[((x + y * img->width) * 4) + 1] = 255;
				img->rgba[((x + y * img->width) * 4) + 2] = 255;
				img->rgba[((x + y * img->width) * 4) + 3] = 255;
			}
		}
	}
}

/* perform edge detection, defaults: a = 1, b = -4 */
void process_image_edgedetect(imgu_image *img, float a, float b) {
	float edge_detect[] = {
		0, a, 0,
		a, b, a,
		0, a, 0
	};

	imgu_filter(img, edge_detect);
}

/* lighten / darken image, defaults: a = 1.3 */
void process_image_lighten(imgu_image *img, float a) {
	float lighten[] = {
		0, 0, 0,
		0, a, 0,
		0, 0, 0
	};

	imgu_filter(img, lighten);
}

/* blur image, defaults: a = 1/9 */
void process_image_blur(imgu_image *img, float a) {
	float blur[] = {
		a, a, a,
		a, a, a,
		a, a, a
	};

	imgu_filter(img, blur);
}

/* sharpen image, defaults: a = -0.33, b = 2.33 */
void process_image_sharpen(imgu_image *img, float a, float b) {
	float blur[] = {
		0, a, 0,
		a, b, a,
		0, a, 0
	};

	imgu_filter(img, blur);
}

void process_image_crop(imgu_image *img) {
	/* find left border pos and length */
	int length = 0;
	int xstart = 0,
	    ystart = 0;
	
	for (int x = 0; x < img->width; ++x) {
		int long_enough = 0;
		length = 0;
		xstart = x;

		for (int y = 0; y < img->height; ++y) {
			if (length == 0)
				ystart = y;

			int r = img->rgba[(x + y * img->width) * 4 + 0];
			int g = img->rgba[(x + y * img->width) * 4 + 1];
			int b = img->rgba[(x + y * img->width) * 4 + 2];
			
			int grayscale = r + g + b;
			if (grayscale > 254 * 3) {
				++length;
			} else if (length > 200) {
				long_enough = 1;
				break;
			}
		}
		
		if (long_enough)
			break;
	}
	
	for (int y = ystart; y < ystart + length; ++y) {
		img->rgba[(xstart - 1 + y * img->width) * 4 + 0] = 255;
		img->rgba[(xstart - 1+ y * img->width) * 4 + 1] = 0;
		img->rgba[(xstart - 1+ y * img->width) * 4 + 2] = 0;
		
		img->rgba[(xstart + 1 + y * img->width) * 4 + 0] = 255;
		img->rgba[(xstart + 1 + y * img->width) * 4 + 1] = 0;
		img->rgba[(xstart + 1 + y * img->width) * 4 + 2] = 0;
	}

}

#endif
