#include <immintrin.h>

struct rect {
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
};

__attribute__((hot, always_inline, target("sse4.1")))
static inline void _blit_clipped(struct ugg * restrict image, struct rect src_rect, int32_t dest_x, int32_t dest_y, struct rect clip_rect) {
	// Calculate intersection rectangle
	int32_t src_x2 = src_rect.x + src_rect.width;
	int32_t src_y2 = src_rect.y + src_rect.height;

	int32_t dest_x2 = dest_x + src_rect.width;
	int32_t dest_y2 = dest_y + src_rect.height;

	int32_t clip_x2 = clip_rect.x + clip_rect.width;
	int32_t clip_y2 = clip_rect.y + clip_rect.height;

	int32_t intersect_x1 = MAX(dest_x, clip_rect.x);
	int32_t intersect_y1 = MAX(dest_y, clip_rect.y);
	int32_t intersect_x2 = MIN(dest_x2, clip_x2);
	int32_t intersect_y2 = MIN(dest_y2, clip_y2);

	if(intersect_x1 >= intersect_x2 || intersect_y1 >= intersect_y2) return;

	// Calculate clipped source coordinates
	int32_t clipped_src_x = src_rect.x + (intersect_x1 - dest_x);
	int32_t clipped_src_y = src_rect.y + (intersect_y1 - dest_y);

	int32_t clipped_width = intersect_x2 - intersect_x1;
	int32_t clipped_height = intersect_y2 - intersect_y1;

	// Clamp source coordinates to image boundaries
	clipped_src_x = MAX(0, MIN((int32_t)(image->width - 1), clipped_src_x));
	clipped_src_y = MAX(0, MIN((int32_t)(image->height - 1), clipped_src_y));

	// Adjust clipped width and height based on image boundaries
	int32_t clipped_source_width = MIN(clipped_width, (int32_t)(image->width - clipped_src_x));
	int32_t clipped_source_height = MIN(clipped_height, (int32_t)(image->height - clipped_src_y));

	if(clipped_source_width <= 0 || clipped_source_height <= 0) return;

	// Initialize pointers
	uint32_t dest_x_offset = intersect_x1;
	uint32_t dest_y_offset = intersect_y1;

	uint32_t buffer_index = (dest_y_offset) * MAIN_BUFFER_WIDTH + (dest_x_offset);
	uint8_t *pixel_ptr = &image->data[(clipped_src_y) * image->width + (clipped_src_x)];
	uint32_t *buffer_ptr = &buffer[buffer_index];

	__m128i zero = _mm_setzero_si128();

	for(uint32_t iy = 0; iy < clipped_source_height; iy++) {
		uint32_t x = 0;

		for(; x + 3 < clipped_source_width; x += 4) {											// Process in blocks of 4
			uint8_t idx0 = pixel_ptr[x + 0];															// Manual gather of 4 pixel indices
			uint8_t idx1 = pixel_ptr[x + 1];
			uint8_t idx2 = pixel_ptr[x + 2];
			uint8_t idx3 = pixel_ptr[x + 3];

			__m128i new_colors		= _mm_setr_epi32(image->palette[idx0], image->palette[idx1], image->palette[idx2], image->palette[idx3]);
			__m128i old_colors		= _mm_loadu_si128((__m128i*)(buffer_ptr + x));		// Load existing colors from the destination
			__m128i mask				= _mm_cmpeq_epi32(new_colors, zero);					// Compare colors with zero
			__m128i blended_colors	= _mm_blendv_epi8(new_colors, old_colors, mask); 	// Blend based on mask
			_mm_storeu_si128((__m128i*)(buffer_ptr + x), blended_colors);					// Store results
		}

		for(; x < clipped_source_width; x++) {														// Handle remaining pixels
			uint8_t color_id = pixel_ptr[x];
			if(color_id != 0) {
				buffer_ptr[x] = image->palette[color_id];
			}
		}
		buffer_ptr += MAIN_BUFFER_WIDTH;
		pixel_ptr += image->width;
	}
}

__attribute__((hot, always_inline))
static inline void blit_clipped(struct ugg * restrict image, struct rect src_rect, int32_t dest_x, int32_t dest_y, struct rect clip_rect) {
	PROFILE_FUNCTION();
	_blit_clipped(image, src_rect, dest_x, dest_y, clip_rect);
}

__attribute__((hot, always_inline))
static inline void blit_full_to_clipped(struct ugg * restrict image, int32_t dest_x, int32_t dest_y, struct rect clip_rect) {
	PROFILE_FUNCTION();
	struct rect source_rect = {0, 0, image->width, image->height};
	_blit_clipped(image, source_rect, dest_x, dest_y, clip_rect);
}

__attribute__((hot, always_inline))
static inline void blit_clipped_to_full(struct ugg * restrict image, struct rect src_rect, int32_t dest_x, int32_t dest_y) {
	PROFILE_FUNCTION();
	struct rect buffer_rect = {0, 0, MAIN_BUFFER_WIDTH, MAIN_BUFFER_HEIGHT};
	_blit_clipped(image, src_rect, dest_x, dest_y, buffer_rect);
}

__attribute__((hot, always_inline))
static inline void blit_full(struct ugg * restrict image, int32_t dest_x, int32_t dest_y) {
	PROFILE_FUNCTION();
	struct rect source_rect = {0, 0, image->width, image->height};
	struct rect buffer_rect = {0, 0, MAIN_BUFFER_WIDTH, MAIN_BUFFER_HEIGHT};
	_blit_clipped(image, source_rect, dest_x, dest_y, buffer_rect);
}
