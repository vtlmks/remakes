
/* [=]===^=====================================================================================^===[=] */
static void framebuffer_callback(GLFWwindow *window, int width, int height) {
	state.screen_width = width;
	state.screen_height = height;

	const float wanted_aspect = 4.f / 3.f;
	float current_aspect = (float)width / (float)height;

	// Reset viewport dimensions to full window
	state.viewport.x = 0.0f;
	state.viewport.y = 0.0f;
	state.viewport.w = width;
	state.viewport.h = height;

	if(current_aspect > wanted_aspect) {			// Window is wider than the desired aspect ratio
		float new_width = height * wanted_aspect; // Compute new width based on the height and the desired aspect ratio
		state.viewport.x = (width - new_width) / 2;
		state.viewport.w = new_width;
	} else if(current_aspect < wanted_aspect) {	// Window is taller than the desired aspect ratio
		float new_height = width / wanted_aspect; // Compute new height based on the width and the desired aspect ratio
		state.viewport.y = (height - new_height) / 2;
		state.viewport.h = new_height;
	}
}

/* [=]===^=====================================================================================^===[=] */
// NOTE(peter): We can steal F11 and F12 here, and the shift, ctrl, alt, version of them, they are not on the Amiga keyboard.

struct window_state {
	bool isFullscreen;
	int windowedWidth;
	int windowedHeight;
	int windowedPosX;
	int windowedPosY;
	GLFWmonitor* currentMonitor;
};

static struct window_state w_state;

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if(key < 0) return;

	if(action == GLFW_PRESS || action == GLFW_RELEASE) {
		state.keystate[key] = (action == GLFW_PRESS) ? 1 : 0;
	}

	if(key == GLFW_KEY_ESCAPE) {
		if(action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, true);
		}
	}

	if(action == GLFW_RELEASE) {
		switch(key) {
			// Handle shader CRT emulation toggle
			case GLFW_KEY_F12: {
				if(!mods) {  // Handle F12 without any modifiers
					state.toggle_crt_emulation = !state.toggle_crt_emulation;
				}
			} break;

			// Handle fullscreen toggle
			case GLFW_KEY_F11: {
				if(!mods) {
					if(w_state.isFullscreen) {
						glfwSetWindowMonitor(window, 0, w_state.windowedPosX, w_state.windowedPosY, w_state.windowedWidth, w_state.windowedHeight, GLFW_DONT_CARE);
						glfwSetWindowSize(window, w_state.windowedWidth, w_state.windowedHeight);
						w_state.isFullscreen = false;
					} else {
						glfwGetWindowPos(window, &w_state.windowedPosX, &w_state.windowedPosY);
						glfwGetWindowSize(window, &w_state.windowedWidth, &w_state.windowedHeight);
						w_state.currentMonitor = glfwGetPrimaryMonitor();
						const GLFWvidmode *mode = glfwGetVideoMode(w_state.currentMonitor);

						glfwSetWindowMonitor(window, w_state.currentMonitor, 0, 0, mode->width, mode->height, mode->refreshRate);
						w_state.isFullscreen = true;
					}
				} else if((mods & GLFW_MOD_SHIFT) == GLFW_MOD_SHIFT) {
					state.overlay = !state.overlay;
				}
			} break;

			default: break;
		}
	}
}

/* [=]===^=====================================================================================^===[=] */
static void mouse_move_callback(GLFWwindow* window, double xpos, double ypos) {
	static double mouseSensitivity = 1.0;
	static double mouseDX = 0;
	static double mouseDY = 0;
	static double lastX = 0;
	static double lastY = 0;
	static int firstMouse = 1;

	if(firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = 0;
	}

	mouseDX = (xpos - lastX) * mouseSensitivity;
	mouseDY = (ypos - lastY) * mouseSensitivity;
	lastX = xpos;
	lastY = ypos;

	state.mouse_dx += mouseDX;
	state.mouse_dy += mouseDY;
}

/* [=]===^=====================================================================================^===[=] */
static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {

	// NOTE(peter): Update the mouse_button_state
	if(action == GLFW_PRESS) {
		state.mousebuttonstate[button] = 1;
	} else if(action == GLFW_RELEASE) {
		state.mousebuttonstate[button] = 0;
	}

	if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS) {
		state.grab_cursor = !state.grab_cursor;
	}
}

/* [=]===^=====================================================================================^===[=] */
static void error_callback(int e, const char *d) {
	printf("Error %d: %s\n", e, d);
}


/* [=]===^=====================================================================================^===[=] */
static uint8_t *debug_lines[MAX_PROFILING_ENTRIES];
static void debug_render() {
	if(state.overlay) {
		size_t offset = 0;

		for(int i = 0; i < MAX_PROFILING_ENTRIES; ++i) {
			if(state.debug.timings[i].count > 0) {
				debug_lines[i] = &debug_line_buffer[offset];
				offset += stbsp_sprintf((char *)&debug_line_buffer[offset], "%25s: cycles=%7" PRIu64 ", count=%2u, cycles/count=%7" PRIu64, state.debug.timings[i].name, state.debug.timings[i].cycles, state.debug.timings[i].count, state.debug.timings[i].cycles / state.debug.timings[i].count) + 1;
			} else {
				debug_lines[i] = 0;
			}
		}
		overlay_render(20.f, 20.f, 530.f, 200.f, debug_lines, state.screen_width, state.screen_height);
	}
	// memset(&state.debug, 0, sizeof(state.debug));
}
