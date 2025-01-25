#define _CRT_SECURE_NO_WARNINGS
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "ugg.h"

#define bmp_magic (uint16_t)('B' | 'M' << 8)

#pragma pack(push,1)
struct bmp_header {
	uint16_t type;							// Magic identifier
	uint32_t size;							// File size in bytes
	uint16_t res1;							//
	uint16_t res2;							//
	uint32_t offset_to_image_data;	// Offset to image data in bytes
	uint32_t info_size;					// Header size in bytes
	int32_t width;							// Width of image
	int32_t height;						// Height of image, if negative we will flip the image.
	uint16_t planes;						// Number of color planes
	uint16_t bits;							// Bits per pixel    NOTE(peter): We will only be handling 1, 4 and 8 bits per pixel
	uint32_t compression;				// Compression type  NOTE(peter): We will handle: (0) BI_RGB, (1) BI_RLE8 (2) BI_RLE4 and (3) BI_BITFIELDS without HUFFMAN 1D
	uint32_t imagesize;					// Image size in bytes
	int32_t x_ppm;							// Pixels per meter
	int32_t y_ppm;							// Pixels per meter
	uint32_t n_colors;					// Number of colors
	uint32_t important_colors;			// Important colors
};
#pragma pack(pop)

// - -=-==<{[ print_usage ]}>==-=- -
static void print_usage(char **argv) {
	printf("\033[0;31mUsage:\033[0m %s -i inputfile -o outputfile [-c number_of_colors] [-p]\n", argv[0]);
	printf("\n");
	printf("\033[0;31mOptions:\033[0m\n");
	printf("  -i inputfile         Specify the input file to be processed\n");
	printf("  -o outputfile        Specify the output file to be created\n");
	printf("  -c number of colors  Set the number of colors to use\n");
	printf("                         (default: Automatically detected based on the\n");
	printf("                                   highest index used in the image.)\n");
	printf("  -p                   Display a preview of the output image\n");
}

// - -=-==<{[ bmp_read_pixels_1 ]}>==-=- -
void bmp_read_pixels_1(struct bmp_header *header, uint8_t *filedata, uint8_t *output) {
	// printf("bmp_read_pixels_1()...\n");
	uint8_t *src = filedata + header->offset_to_image_data;

	uint32_t image_height = (header->height < 0) ? -header->height : header->height;
	uint32_t pixels_per_row = (header->width + 7) >> 3;		// aligned to byte, we are counting bits here.
	uint32_t bytes_per_row = (pixels_per_row + 3) & ~0x3;

#if 0
	printf("pixels_per_row: %d\n", pixels_per_row);
	printf("bytes_per_row: %d\n", bytes_per_row);
#endif

	for(uint32_t y = 0; y < image_height; y++) {
		for(uint32_t x = 0; x < header->width; x++) {
			uint8_t bit = (x & 0x7);
			uint8_t byte = src[x >> 3];
			uint8_t index = byte >> (7 - bit);
			index &= 0x1;
			*output++ = index;
		}
		src += bytes_per_row;
	}
}

// - -=-==<{[ bmp_read_pixels_4 ]}>==-=- -
void bmp_read_pixels_4(struct bmp_header *header, uint8_t *filedata, uint8_t *output) {
	// printf("bmp_read_pixels_4()...\n");
	uint8_t *src = filedata + header->offset_to_image_data;

	uint32_t image_height = (header->height < 0) ? -header->height : header->height;
	uint32_t pixels_per_row = (header->width + 1) >> 1;		// aligned to byte, we are counting nibbles here.
	uint32_t bytes_per_row = (pixels_per_row + 3) & ~0x3;

	for(uint32_t y = 0; y < image_height; y++) {
		for(uint32_t x = 0; x < header->width; x++) {
			uint8_t byte = src[x >> 1];
			uint8_t index = (x & 0x1) ? byte & 0xf : byte >> 4;
			*output++ = index;
		}
		src += bytes_per_row;
	}
}

// - -=-==<{[ bmp_read_pixels_8 ]}>==-=- -
void bmp_read_pixels_8(struct bmp_header *header, uint8_t *filedata, uint8_t *output) {
	// printf("bmp_read_pixels_8()...\n");
	uint32_t bytes_per_row = (header->width + 3) & ~0x3;
	uint32_t image_height = (header->height < 0) ? -header->height : header->height;

	uint8_t *src = filedata + header->offset_to_image_data;
	for(uint32_t y = 0; y < image_height; ++y) {
		for(uint32_t x = 0; x < header->width; ++x) {
			*output++ = src[x];
		}
		src += bytes_per_row;
	}
}

// - -=-==<{[ bmp_read_pixels_rle4 ]}>==-=- -
void bmp_read_pixels_rle4(struct bmp_header *header, uint8_t *filedata, uint8_t *output) {
	// printf("bmp_read_pixels_rle4()...\n");
	uint32_t image_height = (header->height < 0) ? -header->height : header->height;

	bool keep_going = true;
	uint32_t current_line = 0;
	uint8_t *src = filedata + header->offset_to_image_data;
	uint8_t *dst = output;

	while(keep_going) {
		uint8_t byte1 = *src++;
		if(byte1) {
			uint8_t byte2 = *src++;
			for(uint32_t i = 0; i < (byte1 >>1); ++i) {
				*dst++ = byte2 >> 4;
				*dst++ = byte2 & 0xf;
			}
			if(byte1&1) {
				*dst++ = byte2 >> 4;
			}

		} else {
			uint8_t byte2 = *src++;
			switch(byte2) {
				case 0: {
					current_line++;
					dst = output + current_line * header->width;
				} break;
				case 1: {
					keep_going = false;
				} break;
				case 2: {
					uint8_t x_offset = *src++;
					uint8_t y_offset = *src++;
					current_line += y_offset;
					dst += y_offset * header->width + x_offset;
				} break;
				default: {
					for(uint32_t i = 0; i < (byte2 >> 1); ++i) {
						byte1 = *src++;
						*dst++ = byte1 >> 4;
						*dst++ = byte1 & 0xf;
					}
					if(byte2 & 0x1) {
						byte1 = *src++;
						*dst++ = byte1 >> 4;
					}
					src += ((byte2+1)>>1) & 0x1;

				} break;
			}
		}
		keep_going = (dst < output + (header->width * image_height));
	}
}

// - -=-==<{[ bmp_read_pixels_rle8 ]}>==-=- -
void bmp_read_pixels_rle8(struct bmp_header *header, uint8_t *filedata, uint8_t *output) {
	// printf("bmp_read_pixels_rle8()...\n");
	uint32_t image_height = (header->height < 0) ? -header->height : header->height;

	bool keep_going = true;
	uint32_t current_line = 0;
	uint8_t *src = filedata + header->offset_to_image_data;
	uint8_t *dst = output;

	while(keep_going) {
		uint8_t byte = *src++;
		if(byte) {
			uint8_t index = *src++;
			for(uint32_t i = 0; i < byte; ++i) {
				*dst++ = index;
			}
		} else {
			byte = *src++;
			switch(byte) {
				case 0: {
					current_line++;
					dst = output + current_line*header->width;
				} break;
				case 1: {
					keep_going = false;
				} break;
				case 2: {
					uint8_t x_offset = *src++;
					uint8_t y_offset = *src++;
					current_line += y_offset;
					dst += y_offset * header->width + x_offset;
				} break;
				default: {
					for(uint32_t i = 0; i < byte; ++i) {
						uint8_t index = *src++;
						*dst++ = index;
					}
					src += byte & 0x1;
				} break;
			}
		}

		keep_going = (dst < output + (header->width * image_height));
	}
}

void handle_errno(char *filename) {
	switch(errno) {
		case ENOENT: { printf("The file `%s` does not exist.\n", filename); } break;
		case EACCES: { printf("The file `%s` cannot be accessed due to insufficient permissions.\n", filename); } break;
	}
}

// - -=-==<{[ main ]}>==-=- -
int main(int argc, char **argv) {
	int32_t opt;
	char *outfile = 0;
	char *infile = 0;
	int32_t color_value = 0;
	bool preview_flag = false;

	if(argc == 1) {
		print_usage(argv);
		return 0;
	}

	while((opt = getopt(argc, argv, "i:o:c:p")) != -1) {
		switch (opt) {
			case 'i': {
				infile = optarg;
			} break;
			case 'o':
				outfile = optarg;
				break;
			case 'c': {
				color_value = atoi(optarg);
			} break;
			case 'p': {
				preview_flag = true;
			} break;
		}
	}

	if(!infile  || !outfile) {
		print_usage(argv);
		return 0;
	}

	errno = 0;
	FILE *f = fopen(infile, "rb");
	if(errno) {
		handle_errno(infile);
		exit(1);
	}
	fseek(f, 0, SEEK_END);
	uint32_t filesize = ftell(f);
	rewind(f);
	uint8_t *filedata = malloc(filesize);
	(void)fread(filedata, filesize, 1, f);
	fclose(f);
	struct bmp_header *header = (struct bmp_header*)filedata;

	if(header->type == bmp_magic) {
// NOTE(peter): DEBUG INFORMATION
#if 0
	printf("bmp_header:\n");
	printf("                type: %2.2s\n", (char*)&header->type);
	printf("                size: %u\n", header->size);
	printf("offset_to_image_data: %u\n", header->offset_to_image_data);
	printf("           info_size: %u\n", header->info_size);
	printf("\n");
	printf("bmp_info_header:\n");
	printf("       size: %u\n", header->size);
	printf("      width: %u\n", header->width);
	printf("     height: %u\n", header->height);
	printf("     planes: %u\n", header->planes);
	printf("       bits: %u\n", header->bits);
	printf("compression: %u\n", header->compression);
	printf("  imagesize: %u\n", header->imagesize);
	printf("      x-ppm: %u\n", header->x_ppm);
	printf("      y-ppm: %u\n", header->y_ppm);
	printf("     colors: %u\n", header->n_colors);
	printf(" imp.colors: %u\n", header->important_colors);
#endif

		size_t output_image_size = header->width * header->height;
		uint8_t *output = malloc(output_image_size);	// image output

		switch(header->bits) {
			case 1: {
				bmp_read_pixels_1(header, filedata, output);
			} break;
			case 4: {
				if(header->compression == 2) {
					bmp_read_pixels_rle4(header, filedata, output);
				} else {
					bmp_read_pixels_4(header, filedata, output);
				}
			} break;
			case 8: {
				if(header->compression == 1) {
					bmp_read_pixels_rle8(header, filedata, output);
				} else {
					bmp_read_pixels_8(header, filedata, output);
				}
			} break;
			default: {
				printf("WARN: Not a supported bit depth!\n");
			} break;
		}

		if(header->height > 0) {	// if positive height, we need to flip the image upside-down
			// NOTE(peter): Slow as fuck, but does the job.
			uint32_t row_size = header->width;
			uint32_t half_height = header->height / 2;
			for(uint32_t y = 0; y < half_height; y++) {
				uint8_t *row1 = output + y * row_size;
				uint8_t *row2 = output + (header->height - 1 - y) * row_size;
				for(uint32_t x = 0; x < header->width; x++) {
					uint8_t temp_pixel = row1[x];
					row1[x] = row2[x];
					row2[x] = temp_pixel;
				}
			}
		}

		if(preview_flag) {
			uint8_t *src = output;
			for(uint32_t y = 0; y < header->height; ++y) {
				for(uint32_t x = 0; x < header->width; ++x) {
					printf("%c", " abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrst"[*src++]);
				}
				printf("\n");
			}
		}

		uint8_t highest_color_index = 0;
		if(!color_value) {
			for(size_t i = 0; i < output_image_size; ++i) {
				if(output[i] > highest_color_index) {
					highest_color_index = output[i];
				}
			}
			highest_color_index++;		// NOTE(peter): We add one, as the index start on 0(zero), we need to count that for array-size.
		} else {
			highest_color_index = color_value;
		}
		/*
		* Generate file header
		*/

		// Extract the filename from the path
		char *filename = strrchr(outfile, '/');
		if(!filename) {
			filename = outfile; // No directory in the path
		} else {
			filename++; // Move past the '/'
		}

		// Find the last dot in the filename and remove the extension
		char *extension = strrchr(filename, '.');
		if(extension) {
			*extension = '\0'; // Truncate at the dot
		}

		// Replace invalid characters in the filename with '_'
		char *fix_filename = filename;
		while(*fix_filename) {
			if((*fix_filename == '-') || (*fix_filename == ' ') || (*fix_filename == '.')) {
				*fix_filename = '_';
			}
			++fix_filename;
		}

		// Construct the final path with the sanitized filename
		char buffer[512];
		snprintf(buffer, sizeof(buffer), "%.*s%s.ugg", (int)(filename - outfile), outfile, filename);

		FILE *f = fopen(buffer, "wb");

		struct ugg ugg_data = {0};

		ugg_data.width = header->width;
		ugg_data.height = header->height;

		size_t num_colors = header->n_colors ? header->n_colors : (1 << header->bits);

		uint8_t *color_data = filedata + sizeof(struct bmp_header);
		for(size_t i = 0; i < num_colors; ++i) {
			uint8_t b = *color_data++;
			uint8_t g = *color_data++;
			uint8_t r = *color_data++;
			uint8_t a = i ? 0xff : 0x00;	// NOTE(peter): This is to have all colors except color zero have 0xff as alpha for optimized functions in remakes!
			color_data++; // NOTE(peter): this has to happen as we skip the real alpha value!

			ugg_data.palette[i] = r << 24 | g << 16 | b << 8 | a;
		}

		fwrite(&ugg_data, sizeof(ugg_data), 1, f);

		// Write Image Data
		fwrite(output, sizeof(uint8_t), output_image_size, f);
		fclose(f);

	}
	// NOTE(peter): No need to free the loaded data, the OS will handle that when we exit.
}
