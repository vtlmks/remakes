#pragma once

#ifdef _WIN32
#include <intrin.h>
#include <windows.h>
#include <malloc.h>   // For _aligned_malloc and _aligned_free on Windows
#define aligned_alloc(align, size) _aligned_malloc(size, align)
#define aligned_free _aligned_free

#elif __linux__
#include <sys/prctl.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdlib.h>   // For aligned_alloc on Linux
#define aligned_free free
#endif

enum cpu_features {
	REMAKE_CPU_SSE3		= 1 << 0,
	REMAKE_CPU_SSSE3		= 1 << 1,
	REMAKE_CPU_SSE4_1		= 1 << 2,
	REMAKE_CPU_SSE4_2		= 1 << 3,
	REMAKE_CPU_AVX			= 1 << 4,
	REMAKE_CPU_AVX2		= 1 << 5,
	REMAKE_CPU_FMA			= 1 << 6,
	REMAKE_CPU_BMI1		= 1 << 7,
	REMAKE_CPU_BMI2		= 1 << 8,
	REMAKE_CPU_AES			= 1 << 9,
	REMAKE_CPU_AVX512F	= 1 << 10,
	REMAKE_CPU_SHA			= 1 << 11,
};

#define MAX_PROFILING_ENTRIES (64)

struct function_cycles {
	const char *name;
	uint64_t cycles;
	uint32_t count;
};

struct debug_state {
	struct function_cycles timings[MAX_PROFILING_ENTRIES];
};

/*
 * -=[*]=- base_state struct documentation
 */
struct base_state {
	double frametime;
	uint8_t keystate[512];
	uint8_t keyprevstate[512];
	uint8_t mousebuttonstate[8];
	uint8_t mousebuttonprevstate[8];
	float mouse_dx;
	float mouse_dy;
	uint32_t screen_width;
	uint32_t screen_height;
	uint32_t frames_per_second;
	uint32_t frame_number;
	int32_t filter_override;					// Manual override: -1 = automatic, 0 = off, 1 = on
	float filter_frequency;						// Frequency in Hz for squarewave toggle
	enum cpu_features cpu_features;
	struct {
		float x, y, w, h;
	} viewport;
	// OpenGL
	GLuint texture;
	GLuint shader_program;
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint uniform_resolution;
	GLuint uniform_src_image_size;
	GLuint uniform_brightness;
	GLuint uniform_tone;
	GLuint uniform_crt_emulation;
	GLuint uniform_sampler_location;
	// Misc
	bool toggle_crt_emulation;
	bool cursor_locked;
	bool grab_cursor;
	bool overlay;						// Render debug_overlay
	// Debug/Information
	struct debug_state debug;
};
struct base_state state;


struct remake_callbacks {
	bool (*render)();
	void (*audio_callback)(int16_t *audio_buffer, size_t frames);
};

static struct remake_callbacks *current_part = 0;
static void render_callback();

struct profiling_context {
	uint32_t func_id;
};

static uint8_t debug_line_buffer[MAX_PROFILING_ENTRIES * 256];	// NOTE(peter): for storing all performance strings in an array to print to the debug-output

static inline uint64_t read_tsc() {
	unsigned int aux;
	return __rdtscp(&aux);
}

static void end_profiling(struct profiling_context *ctx) __attribute__((unused));
static void end_profiling(struct profiling_context *ctx) {
	state.debug.timings[ctx->func_id].cycles += read_tsc();
	state.debug.timings[ctx->func_id].count++;
}

#define PROFILE_NAMED(name)																			\
	uint32_t func_id = __COUNTER__;																	\
	if((state).debug.timings[func_id].count == 0)												\
		(state).debug.timings[func_id] = (struct function_cycles){(name), 0, 0};		\
	(state).debug.timings[func_id].cycles -= read_tsc();										\
	struct profiling_context ctx __attribute__((cleanup(end_profiling))) = { func_id };

#define PROFILE_FUNCTION()																				\
	uint32_t func_id = __COUNTER__;																	\
	if((state).debug.timings[func_id].count == 0)												\
		(state).debug.timings[func_id] = (struct function_cycles){__func__, 0, 0};		\
	(state).debug.timings[func_id].cycles -= read_tsc();										\
	struct profiling_context ctx __attribute__((cleanup(end_profiling))) = { func_id };
