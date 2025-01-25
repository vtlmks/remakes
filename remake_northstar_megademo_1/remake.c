/*
 *
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define NOSERVICE
#define NOMINMAX
#define NOKERNEL
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOTEXTMETRIC
#define NOWH
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#undef NOCRYPT
#include <windows.h>
#include <wincrypt.h>
#endif

#include <base.c>

#include <ugg.h>		// The ultimate graphics format
#include <blit.h>
#include <generic_scroller.h>
#include <protracker2.c>

#define MKS_RESAMPLER_IMPLEMENTATION
#include <resampler.h>

INCBIN(_part1_small_font,	"data/p1/small_font.ugg");
INCBIN(_part2_ns_logo,		"data/p2/ns_logo.ugg");
INCBIN(_part2_text,			"data/p2/text.ugg");
INCBIN(_part4_atom_logo_bg,"data/p4/atom_logo_bg.ugg");

INCBIN(_part1_audio8,		"data/p1/audio8.raw");
INCBIN(_part3_audio8,		"data/p3/audio8.raw");
INCBIN(_part7_audio8,		"data/p7/audio8.raw");
INCBIN(_part2_n_s_beat,		"data/p2/n.s_beat.mod");
INCBIN(_part4_n_s_quiz,		"data/p4/n.s_quiz.mod");
INCBIN(_part5_n_s_speedo,	"data/p5/n.s_speedo.mod");
INCBIN(_part6_tune13,		"data/p6/tune13.mod");
INCBIN(_part8_n_s_dreamer,	"data/p8/n.s_dreamer.mod");

// Cast the INCBIN data to usable struct pointers
static struct ugg *part1_small_font_data		= (struct ugg*)_part1_small_font_data;
static struct ugg *part2_ns_logo_data			= (struct ugg*)_part2_ns_logo_data;
static struct ugg *part2_text_data				= (struct ugg*)_part2_text_data;
static struct ugg *part4_atom_logo_bg_data	= (struct ugg*)_part4_atom_logo_bg_data;

static int8_t *part1_audio8_data					= (int8_t*)_part1_audio8_data;
static int8_t *part3_audio8_data					= (int8_t*)_part3_audio8_data;
static int8_t *part7_audio8_data					= (int8_t*)_part7_audio8_data;

static uint8_t *part2_n_s_beat_data				= (uint8_t*)_part2_n_s_beat_data;
static uint8_t *part4_n_s_quiz_data				= (uint8_t*)_part4_n_s_quiz_data;
static uint8_t *part5_n_s_speedo_data			= (uint8_t*)_part5_n_s_speedo_data;
static uint8_t *part6_tune13_data				= (uint8_t*)_part6_tune13_data;
static uint8_t *part8_n_s_dreamer_data			= (uint8_t*)_part8_n_s_dreamer_data;

static struct pt_state part2_song;
static struct pt_state part4_song;
static struct pt_state part5_song;
static struct pt_state part6_song;
static struct pt_state part8_song;

#include "part1.c"
#include "part2.c"
#include "part3.c"
#include "part4.c"
#include "part5.c"
#include "part6.c"
#include "part7.c"
#include "part8.c"

static uint32_t load_frame_count = 0;

struct sample_state {
	int16_t *data;
	uint32_t size;
	uint32_t position;
	bool done;				// Used for one-shot playback
};

static struct sample_state part1_sample;
static struct sample_state part3_sample;
static struct sample_state part7_sample;

static void process_sampled_audio(int16_t *audio_buffer, size_t frames, struct sample_state *sample) {
	for(size_t i = 0; i < frames; ++i) {
		int16_t processed_sample = (int16_t)(sample->data[sample->position] * 0.707f);
		*audio_buffer++ = processed_sample;
		*audio_buffer++ = processed_sample;
		if(++sample->position == sample->size) {
			sample->position = 0;
		}
	}
}

static void p1_audio(int16_t *audio_buffer, size_t frames) { process_sampled_audio(audio_buffer, frames, &part1_sample); }
static void p3_audio(int16_t *audio_buffer, size_t frames) { process_sampled_audio(audio_buffer, frames, &part3_sample); }
static void p7_audio(int16_t *audio_buffer, size_t frames) { process_sampled_audio(audio_buffer, frames, &part7_sample); }

static void p2_audio(int16_t *audio_buffer, size_t frames) { pt2play_FillAudioBuffer(&part2_song, audio_buffer, frames); };
static void p4_audio(int16_t *audio_buffer, size_t frames) { pt2play_FillAudioBuffer(&part4_song, audio_buffer, frames); };
static void p5_audio(int16_t *audio_buffer, size_t frames) { pt2play_FillAudioBuffer(&part5_song, audio_buffer, frames); };
static void p6_audio(int16_t *audio_buffer, size_t frames) { pt2play_FillAudioBuffer(&part6_song, audio_buffer, frames); };
static void p8_audio(int16_t *audio_buffer, size_t frames) { pt2play_FillAudioBuffer(&part8_song, audio_buffer, frames); };

static struct remake_callbacks callbacks[] = {
	{ p1_update, p1_audio }, // Part 1 - fireworks				- done
	{ p2_update, p2_audio }, // Part 2 - sine scroll			- done
	{ p3_update, p3_audio }, // Part 3 - bouncing logo+logos	- done
	{ p4_update, p4_audio }, // Part 4 - 8 scrollers			- done
	{ p5_update, p5_audio }, // Part 5 - warhammer				- done
	{ p6_update, p6_audio }, // Part 6 - omega part				- done
	{ p7_update, p7_audio }, // Part 7 - daus part				- done
	{ p8_update, p8_audio }, // Part 8 - endpart					- done
};

static void switch_to_part(struct remake_callbacks *next_part) {
	current_part = next_part;
}

static void render_callback() {
	const uint8_t keymap[] = {
		'1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
		'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'
	};

	for(uint32_t i = 0; i < sizeof(keymap); ++i) {
		if(state.keystate[keymap[i]] && !state.keyprevstate[keymap[i]]) {
			if(i < ARRAY_SIZE(callbacks)) {
				if(current_part != &callbacks[i]) {
					switch_to_part(&callbacks[i]);
				}
			}
		}
	}

	if((state.keystate[GLFW_KEY_DOWN] && !state.keyprevstate[GLFW_KEY_DOWN]) || (state.keystate[GLFW_KEY_LEFT] && !state.keyprevstate[GLFW_KEY_LEFT])) {
		if(current_part > callbacks) {
			switch_to_part(current_part - 1);
		}
	}

	if((state.keystate[GLFW_KEY_UP] && !state.keyprevstate[GLFW_KEY_UP]) || (state.keystate[GLFW_KEY_RIGHT] && !state.keyprevstate[GLFW_KEY_RIGHT])) {
		if(current_part < &callbacks[ARRAY_SIZE(callbacks) - 1]) {
			switch_to_part(current_part + 1);
		}
	}

	memset(buffer, 0, sizeof(buffer));
	if(current_part->render()) {
		if(current_part < &callbacks[ARRAY_SIZE(callbacks) - 1]) {
			switch_to_part(current_part + 1);
		} else {
			switch_to_part(&callbacks[0]);
		}
	}
}

__attribute__((constructor(190)))
static void init_remake() {
	part1_sample.data = resample_audio(part1_audio8_data, _part1_audio8_end - _part1_audio8_data, 416, &part1_sample.size);
	part3_sample.data = resample_audio(part3_audio8_data, _part3_audio8_end - _part3_audio8_data, 428, &part3_sample.size);
	part7_sample.data = resample_audio(part7_audio8_data, _part7_audio8_end - _part7_audio8_data, 428, &part7_sample.size);

	pt2play_initPlayer(48000);
	pt2play_PlaySong(&part2_song, part2_n_s_beat_data,		CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&part4_song, part4_n_s_quiz_data,		CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&part5_song, part5_n_s_speedo_data,	CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&part6_song, part6_tune13_data,		CIA_TEMPO_MODE, 48000);
	pt2play_PlaySong(&part8_song, part8_n_s_dreamer_data,	CIA_TEMPO_MODE, 48000);

	switch_to_part(&callbacks[0]);
}
