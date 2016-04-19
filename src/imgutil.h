#ifndef IMGUTIL_H
#define IMGUTIL_H

typedef struct {
	unsigned char *rgba;
	unsigned width, height;
} imgu_image;

imgu_image imgu_new(unsigned char *img, unsigned w, unsigned h) {
	return (imgu_image) {
		.rgba = img,
		.width = w,
		.height = h
	};
}

void imgu_destroy(imgu_image *img) {
	free(img->rgba);
	img->width = 0;
	img->height = 0;
}

/*
 * get channel value at x,y from RGBA image
 */
unsigned char imgu_getchannel(imgu_image *i, int x, int y, int channel) {
	int idx = ((x + y * i->width) * 4) + channel;
	return i->rgba[idx];
}

unsigned char imgu_filter_pixel_channel(imgu_image *img, float filter[], int x, int y, int channel) {
	float dest = 0.0;

	for (int dy = -1; dy < 2; ++dy) {
		for (int dx = -1; dx < 2; ++dx) {
			int xp = x + dx;
			int yp = y + dy;
			
			if (xp < 0 || yp < 0 || xp >= img->width || yp >= img->height) {
				continue;
			}
			
			float result = filter[(1 + dx) + (1 + dy) * 3] * (float)imgu_getchannel(img, xp, yp, channel);
			dest += result;
		}
	}
	
	if (dest > 255.)
		dest = 255.;
	else if (dest < 0.)
		dest = 0.;
	
	return (unsigned char)dest;
}

void imgu_filter(imgu_image *img, float filter[]) {
	unsigned char *new_img = malloc(img->width * img->height * 4);
	
	for (int y = 0; y < img->height; ++y) {
		for (int x = 0; x < img->width; ++x) {
			unsigned char nr = imgu_filter_pixel_channel(img, filter, x, y, 0);
			unsigned char ng = imgu_filter_pixel_channel(img, filter, x, y, 1);
			unsigned char nb = imgu_filter_pixel_channel(img, filter, x, y, 2);
			
			int idx = ((x + y * img->width) * 4);
			new_img[idx + 0] = nr;
			new_img[idx + 1] = ng;
			new_img[idx + 2] = nb;
			new_img[idx + 3] = 255;
		}
	}

	free(img->rgba);
	img->rgba = new_img;
}

#endif
