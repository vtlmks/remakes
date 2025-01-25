/* ===^=====================================================================================^===[=]
 *
 *  Author: Peter Fors  aka  ViTAL/MKS
 *
 *  (C) Copyright 2023 by Mindkiller Systems, inc.
 *      All rights reserved.
 *
 * “The ships hung in the sky in much the same way that bricks don't.” ― Douglas Adams
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>
#include <x86intrin.h>

#define STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_NOFLOAT
#define STB_SPRINTF_STATIC
#include "stb_sprintf.h"

// NOTE(peter): The Width is calculated by (270*4)/3 (4:3 aspect ratio)
#define MAIN_BUFFER_HEIGHT 270
#define MAIN_BUFFER_WIDTH 360

#define FPS (50.f)
#define FRAMETIME (1.f/FPS)

static uint32_t buffer[MAIN_BUFFER_WIDTH * MAIN_BUFFER_HEIGHT] __attribute__((section(".bss"), aligned(64)));

#define SCALE (3.f)

#include "incbin.h"

// utils.h - xor_rand, mks_rand
#define RAND_IMPLEMENTATION
#include "rand.h"


#include "opengl_loader.c"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "common.h"
#include "base.h"
#include "opengl.c"
#include "cpu.c"
#include "audio.c"
#include "shader.c"

#include "ugg.h"

#include "font_texture.h"
INCBIN(_font_texture, "../base/font_texture.ugg");
struct ugg *font_texture_data = (struct ugg*)_font_texture_data;

#include "overlay.c"

#include "base_internal.c"
struct rng_state base_rand;

#define DEVICE_SAMPLE_RATE 48000

__attribute__((constructor(200)))
static void init() {
	xor_init_rng(&base_rand, 187481201);
}

/* [=]===^=====================================================================================^===[=] */
// int base_run(struct remake_state *remake, int argc, char **argv) {
int main(int argc, char **argv) {
	GLFWwindow *window;

	state.frametime = FRAMETIME;
	// TODO(peter): Get commandline arguments to see if crt_emulation should be turned off. and other things...
	state.toggle_crt_emulation = true;

	check_cpu_features();

#ifdef _WIN32
	timeBeginPeriod(1);
	SetPriorityClass(0, HIGH_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#else
	setbuf(stdout, 0);	// flush immediately to stdout
#endif

	glfwSetErrorCallback(error_callback);

	if(glfwInit()) {
		// Set X11 class and instance names before creating the window
		glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "remake_instance");
		glfwWindowHintString(GLFW_X11_CLASS_NAME, "remake_class");
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

// glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		uint32_t scaled_window_width = MAIN_BUFFER_WIDTH * SCALE;
		uint32_t scaled_window_height = MAIN_BUFFER_HEIGHT * SCALE;
		uint32_t min_window_width = MAIN_BUFFER_WIDTH * 2;
		uint32_t min_window_height = MAIN_BUFFER_HEIGHT * 2;

		window = glfwCreateWindow(scaled_window_width, scaled_window_height, "North Star - Megademo - 1988-03-31", 0, 0);
		if(window) {
			glfwMakeContextCurrent(window);
			glfwSwapInterval(1);

			if(glfwRawMouseMotionSupported()) {
				glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
			}

			glfwSetKeyCallback(window, key_callback);
			glfwSetCursorPosCallback(window, mouse_move_callback);
			glfwSetMouseButtonCallback(window, mouse_button_callback);
			glfwSetFramebufferSizeCallback(window, framebuffer_callback);
			glfwSetWindowAspectRatio(window, 4, 3);
			glfwSetWindowSizeLimits(window, min_window_width, min_window_height, -1, -1);
			glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);  // NOTE: Sticky mouse buttons, so we don't miss any events..

			// Set viewport dimensions and screen size initially
			int framebuffer_width, framebuffer_height;
			glfwGetFramebufferSize(window, &framebuffer_width, &framebuffer_height);
			state.screen_width = framebuffer_width;
			state.screen_height = framebuffer_height;
			state.viewport.w = scaled_window_width;
			state.viewport.h = scaled_window_height;

			setup_opengl(vertex_shader_start, fragment_shader_start, true);
			setup_texture(state.texture, MAIN_BUFFER_WIDTH, MAIN_BUFFER_HEIGHT);


// glEnable(GL_DEBUG_OUTPUT);
// glDebugMessageCallback(glDebugOutput, 0);

			bool running = true;

			float contrast = 1.0f;
			float saturation = 0.0f;
			float brightness = 1.0f;

			/*
			 * Move this into the mainloop if change of contrast/saturation is added as an interactive thing.
			 */
			float tone_dat[4] = {0.f};
			CrtsTone(tone_dat, contrast, saturation, INPUT_THIN, INPUT_MASK);

			overlay_init();
			audio_initialize();

			double accumulated_time = 0.0;
			double last_time = glfwGetTime();
			while(running && !glfwWindowShouldClose(window)) {
				double current_time = glfwGetTime();
				double delta_time = current_time - last_time;
				last_time = current_time;
				accumulated_time += delta_time;

				glfwPollEvents();

				if(state.grab_cursor != state.cursor_locked) {
					state.cursor_locked = state.grab_cursor;
					if(state.grab_cursor) {
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					} else {
						glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					}
				}

				if(accumulated_time >= state.frametime) {
					memset(&state.debug, 0, sizeof(state.debug));
					render_callback();
					memcpy(state.mousebuttonprevstate, state.mousebuttonstate, sizeof(state.mousebuttonstate));
					memcpy(state.keyprevstate, state.keystate, sizeof(state.keystate));

					// accumulated_time -= state.frametime;

					while(accumulated_time >= state.frametime) { accumulated_time -= state.frametime; }
					state.frame_number++;
				}

				glClearColor(0.f, 0.f, 0.f, 0.f);
				glClear(GL_COLOR_BUFFER_BIT);

				glUseProgram(state.shader_program);
				glBindVertexArray(state.vao);
				glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.ebo);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, state.texture);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, MAIN_BUFFER_WIDTH, MAIN_BUFFER_HEIGHT, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, buffer);
				glUniform2f(state.uniform_src_image_size, (float)MAIN_BUFFER_WIDTH, (float)MAIN_BUFFER_HEIGHT);
				glUniform2f(state.uniform_resolution, state.viewport.w, state.viewport.h);
				glUniform1f(state.uniform_brightness, brightness);
				glUniform4f(state.uniform_tone, tone_dat[0], tone_dat[1], tone_dat[2], tone_dat[3]);
				glUniform1i(state.uniform_crt_emulation, state.toggle_crt_emulation);
				glViewport(state.viewport.x, state.viewport.y, state.viewport.w, state.viewport.h);
				glDrawElements(GL_TRIANGLES, ARRAY_SIZE(indices), GL_UNSIGNED_INT, 0);

				debug_render();

				glfwSwapBuffers(window);

			}

			overlay_shutdown();
			glDeleteProgram(state.shader_program);
			glDeleteTextures(1, &state.texture);
			glfwDestroyWindow(window);
		} else {
			printf("ERROR: GLFW could not create window!\n");
		}
		glfwTerminate();
	} else {
		printf("ERROR: Could not initialize glfw!\n");
	}
	audio_shutdown();

#ifdef _WIN32
	timeEndPeriod(1);
#endif
	return 0;
}

