#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[]) {
	if(argc != 4) {
		fprintf(stderr, "\nUsage: %s <input_bmp> <output_bmp> <glyph_width>\n\n", argv[0]);
		fprintf(stderr, "This tool converts an Amiga font stored as multiple rows of characters\n");
		fprintf(stderr, "(e.g., a 32x32 font arranged in a bitmap with rows and columns defined\n");
		fprintf(stderr, "by the dimensions of the BMP file and glyph width) into a vertical\n");
		fprintf(stderr, "layout with one character per row. The resulting font is saved in the\n");
		fprintf(stderr, "specified output BMP file.\n\n");
		return 1;
	}
	uint32_t glyph_width = (uint32_t)strtoul(argv[3], NULL, 10);
	uint32_t glyph_height = glyph_width;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface *fontSurface = SDL_LoadBMP(argv[1]);
	if(!fontSurface) {
		fprintf(stderr, "Error loading BMP file: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	uint32_t bmp_width = fontSurface->w;
	uint32_t bmp_height = fontSurface->h;
	uint32_t glyphs_per_row = bmp_width / glyph_width;
	uint32_t total_glyphs = (bmp_width / glyph_width) * (bmp_height / glyph_height);

	SDL_Surface *outputSurface = SDL_CreateRGBSurfaceWithFormat(0, glyph_width, glyph_height * total_glyphs, fontSurface->format->BitsPerPixel, fontSurface->format->format);
	if(SDL_ISPIXELFORMAT_INDEXED(fontSurface->format->format)) {
		SDL_SetPaletteColors(outputSurface->format->palette, fontSurface->format->palette->colors, 0, fontSurface->format->palette->ncolors);
	}
	for(uint32_t i = 0; i < total_glyphs; i++) {
		uint32_t src_x = (i % glyphs_per_row) * glyph_width;
		uint32_t src_y = (i / glyphs_per_row) * glyph_height;
		SDL_Rect srcRect = { src_x, src_y, glyph_width, glyph_height };
		SDL_Rect destRect = { 0, i * glyph_height, glyph_width, glyph_height };
		SDL_BlitSurface(fontSurface, &srcRect, outputSurface, &destRect);
	}
	SDL_SaveBMP(outputSurface, argv[2]);
	SDL_FreeSurface(fontSurface);
	SDL_FreeSurface(outputSurface);
	SDL_Quit();
	return 0;
}

