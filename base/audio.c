#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define SAMPLE_RATE 48000
#define NUM_CHANNELS 2
#define FRAME_SIZE (NUM_CHANNELS * sizeof(short))

// static inline float smoothstep(float edge0, float edge1, float x) {
// 	x = (x - edge0) / (edge1 - edge0); // Scale x to [0, 1]
// 	x = x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x); // Clamp to [0, 1]
// 	return x * x * (3.0f - 2.0f * x); // Smooth interpolation
// }

// static inline float smootherstep(float edge0, float edge1, float x) {
//     x = (x - edge0) / (edge1 - edge0); // Scale x to [0, 1]
//     x = x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x); // Clamp to [0, 1]
//     return x * x * x * (x * (x * 6 - 15) + 10); // Modified curve
// }

static inline float fast_cos(float x) {
	float x2 = x * x;
	return 1.0f - x2 * (0.5f - x2 * 0.04166667f); // Approximation of cos(x)
}

static inline float cosine_smooth(float edge0, float edge1, float x) {
	x = (x - edge0) / (edge1 - edge0);					// Scale x to [0, 1]
	x = x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x);	// Clamp to [0, 1]
	return 0.5f * (1.0f - fast_cos(x * M_PI));			// Cosine smoothing
}

static float filter_phase = 0.0f;
static float prev_output_sample_L = 0.0f;
static float prev_output_sample_R = 0.0f;

static void audio_callback(int16_t *audio_buffer, size_t frames) {
	int filter_override = state.filter_override;										// Manual override: -1 = auto, 0 = off, 1 = on
	float filter_frequency = state.filter_frequency;									// Frequency in Hz for squarewave toggle

	if(current_part->audio_callback != 0) {
		current_part->audio_callback(audio_buffer, frames);

		if(filter_override) {
			float a = 1.0f * M_PI * 4000.0f / (SAMPLE_RATE + 1.0f * M_PI * 4000.0f);
			float phase_increment = filter_frequency / SAMPLE_RATE;

			for(size_t i = 0; i < frames * 2; i += 2) {
				float led_filter_active;

				if(filter_override == -1) {
					filter_phase += phase_increment;
					if(filter_phase >= 1.0f) filter_phase -= 1.0f;

					led_filter_active = cosine_smooth(0.45f, 0.50f, filter_phase) - cosine_smooth(0.95f, 1.00f, filter_phase);

				} else {
					led_filter_active = 1.0f;					// Manual override (1 = on)
				}

				float input_sample_L = (float)audio_buffer[i] / 32767.0f;
				float input_sample_R = (float)audio_buffer[i + 1] / 32767.0f;

				float filtered_sample_L = a * input_sample_L + (1.0f - a) * prev_output_sample_L;
				float filtered_sample_R = a * input_sample_R + (1.0f - a) * prev_output_sample_R;

				prev_output_sample_L = filtered_sample_L;
				prev_output_sample_R = filtered_sample_R;

				audio_buffer[i] = (int16_t)((1.0f - led_filter_active) * input_sample_L * 32767.0f + led_filter_active * filtered_sample_L * 32767.0f);
				audio_buffer[i + 1] = (int16_t)((1.0f - led_filter_active) * input_sample_R * 32767.0f + led_filter_active * filtered_sample_R * 32767.0f);
			}
		}
	} else {
		memset(audio_buffer, 0, frames * FRAME_SIZE);
	}
}

#ifdef __linux__

#include <pthread.h>
#include <alsa/asoundlib.h>

#define BUFFER_SIZE (512 * FRAME_SIZE)
#define NUM_CHANNELS 2

snd_pcm_t *pcm;
pthread_t audio_thread;
static __attribute__((aligned(64))) int16_t alsa_buffer[BUFFER_SIZE];

static void *audio_thread_func(void *arg) {
	while(1) {
		snd_pcm_wait(pcm, -1);
		audio_callback(alsa_buffer, BUFFER_SIZE / FRAME_SIZE);
		snd_pcm_writei(pcm, alsa_buffer, BUFFER_SIZE / FRAME_SIZE);
		pthread_testcancel();
	}
	return 0;
}

static int set_hw_params(snd_pcm_t *handle) {
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t period_size = 128;  // Period size (frames)
	snd_pcm_uframes_t buffer_size = 512;  // Buffer size (frames)
	unsigned int rate = SAMPLE_RATE;
	int err;

	snd_pcm_hw_params_alloca(&params);
	err = snd_pcm_hw_params_any(handle, params);
	if(err < 0) { fprintf(stderr, "Cannot initialize hardware parameter structure (%s)\n", snd_strerror(err)); return err; }

	err = snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
	if(err < 0) { fprintf(stderr, "Cannot set access type (%s)\n", snd_strerror(err)); return err; }

	err = snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
	if(err < 0) { fprintf(stderr, "Cannot set sample format (%s)\n", snd_strerror(err)); return err; }

	err = snd_pcm_hw_params_set_channels(handle, params, NUM_CHANNELS);
	if(err < 0) { fprintf(stderr, "Cannot set channel count (%s)\n", snd_strerror(err)); return err; }

	err = snd_pcm_hw_params_set_rate_near(handle, params, &rate, 0);
	if(err < 0) { fprintf(stderr, "Cannot set sample rate (%s)\n", snd_strerror(err)); return err; }

	err = snd_pcm_hw_params_set_period_size_near(handle, params, &period_size, 0);
	if(err < 0) { fprintf(stderr, "Cannot set period size (%s)\n", snd_strerror(err)); return err; }

	err = snd_pcm_hw_params_set_buffer_size_near(handle, params, &buffer_size);
	if(err < 0) { fprintf(stderr, "Cannot set buffer size (%s)\n", snd_strerror(err)); return err; }

	err = snd_pcm_hw_params(handle, params);
	if(err < 0) { fprintf(stderr, "Cannot set parameters (%s)\n", snd_strerror(err)); return err; }

	return 0;
}

static void audio_initialize() {
	int err;
	err = snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);
	if(err < 0) { fprintf(stderr, "Error opening PCM device: %s\n", snd_strerror(err)); return; }

	err = set_hw_params(pcm);
	if(err < 0) { fprintf(stderr, "Error setting hardware parameters: %s\n", snd_strerror(err)); return; }

	err = snd_pcm_start(pcm);
	if(err < 0) { fprintf(stderr, "Error starting PCM device: %s\n", snd_strerror(err)); return; }

	pthread_create(&audio_thread, 0, audio_thread_func, 0);
}

static void audio_shutdown() {
	pthread_cancel(audio_thread);
	pthread_join(audio_thread, 0);
	snd_pcm_drop(pcm);
	snd_pcm_close(pcm);
}

#elif _WIN32

#include <windows.h>
#include <mmsystem.h>

#define BUFFER_COUNT 3
#define BUFFER_SIZE (1024 * FRAME_SIZE)

HWAVEOUT wave_out;
WAVEHDR wave_header[BUFFER_COUNT];
static __attribute__((aligned(64))) int8_t waveout_buffer[BUFFER_COUNT][BUFFER_SIZE];

static void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2) {
	if(uMsg == WOM_DONE) {
		WAVEHDR *wave_header = (WAVEHDR*)dwParam1;
		waveOutUnprepareHeader(hwo, wave_header, sizeof(WAVEHDR));
		audio_callback((int16_t*)wave_header->lpData, wave_header->dwBufferLength / FRAME_SIZE);
		waveOutPrepareHeader(hwo, wave_header, sizeof(WAVEHDR));
		waveOutWrite(hwo, wave_header, sizeof(WAVEHDR));
	}
}

static void audio_initialize() {
	WAVEFORMATEX wave_format = {0};
	wave_format.wFormatTag = WAVE_FORMAT_PCM;
	wave_format.nChannels = NUM_CHANNELS;
	wave_format.nSamplesPerSec = SAMPLE_RATE;
	wave_format.wBitsPerSample = 16;
	wave_format.nBlockAlign = wave_format.nChannels * (wave_format.wBitsPerSample / 8);
	wave_format.nAvgBytesPerSec = wave_format.nSamplesPerSec * wave_format.nBlockAlign;
	waveOutOpen(&wave_out, WAVE_MAPPER, &wave_format, (DWORD_PTR)waveOutProc, (DWORD_PTR)&state, (DWORD_PTR)CALLBACK_FUNCTION);

	for(uint32_t i = 0; i < BUFFER_COUNT; ++i) {
		memset(&wave_header[i], 0, sizeof(WAVEHDR));
		wave_header[i].lpData = (char*)waveout_buffer[i];
		wave_header[i].dwBufferLength = BUFFER_SIZE;
		waveOutPrepareHeader(wave_out, &wave_header[i], sizeof(WAVEHDR));
	}

	for(uint32_t i = 0; i < BUFFER_COUNT; ++i) {
		memset(wave_header[i].lpData, 0, BUFFER_SIZE);
		waveOutWrite(wave_out, &wave_header[i], sizeof(WAVEHDR));
	}
}

static void audio_shutdown() {
	waveOutReset(wave_out);
	for(uint32_t i = 0; i < BUFFER_COUNT; ++i) {
		waveOutUnprepareHeader(wave_out, &wave_header[i], sizeof(WAVEHDR));
	}
	waveOutClose(wave_out);
}

#else
	#error "Unsupported platform"
#endif
