#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define BUFFER_WIDTH (368)
#define BUFFER_HEIGHT (BUFFER_WIDTH * (3.0 / 4.0))


#include "../opengl_loader.c"

struct base_state {
	void (*audio_callback)int16_t *audio_buffer, size_t frames);
	void (*render_callback)();
	double frametime;
	uint8_t keystate[512];
	uint8_t keyprevstate[512];
	uint8_t mousebuttonstate[8];
	uint8_t mousebuttonprevstate[8];
	uint32_t *buffer;
	char *title;
	uint32_t buffer_width;
	uint32_t buffer_height;
	float mouse_dx;
	float mouse_dy;
	uint32_t screen_width;
	uint32_t screen_height;
	uint32_t frames_per_second;
	uint32_t frame_number;
	struct {
		float x, y, w, h;
	} viewport;
	GLuint texture;
	GLuint shader_program;
	GLuint vao, vbo, ebo; // Add VAO, VBO, and EBO here
	GLuint uniform_resolution;
	GLuint uniform_src_image_size;
	GLuint uniform_brightness;
	GLuint uniform_tone;
	GLuint uniform_crt_emulation;
	GLuint uniform_sampler_location;
	bool toggle_crt_emulation;
	bool cursor_locked;
	bool grab_cursor;
};

#include "../opengl.c"

static const char* vertexShaderSource = "#version 330 core\n"
	"layout(location = 0) in vec2 aPos;\n"
	"layout(location = 1) in vec2 aTexCoord;\n"
	"out vec2 TexCoord;\n"
	"void main() {\n"
	"    gl_Position = vec4(aPos, 0.0, 1.0);\n"
	"    TexCoord = aTexCoord;\n"
	"}\n";

static const char* fragmentShaderSource = "#version 330 core\n"
	"in vec2 TexCoord;\n"
	"out vec4 FragColor;\n"
	"uniform sampler2D screenTexture;\n"
	"void main() {\n"
	"    FragColor = texture(screenTexture, TexCoord);\n"
	"    FragColor = vec4(TexCoord, 0.0, 1.0);//texture(screenTexture, TexCoord);\n"
	"}\n";

int main() {
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Render Buffer", 0, 0);
	glfwMakeContextCurrent(window);
	struct base_state state = {
		.buffer_width = BUFFER_WIDTH,
		.buffer_height = BUFFER_HEIGHT
	};

	setup_opengl(vertexShaderSource, fragmentShaderSource, false);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, state.texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, state.buffer_width, state.buffer_height, GL_RGBA, GL_UNSIGNED_BYTE, 0);

		glUseProgram(state.shader_program);
		glBindVertexArray(state.vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &state.vao);
	glDeleteBuffers(1, &state.vbo);
	glDeleteBuffers(1, &state.ebo);
	glDeleteTextures(1, &state.texture);
	glDeleteProgram(state.shader_program);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

