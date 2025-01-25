
#include "fragment_shader.h"
#include "vertex_shader.h"



static const unsigned int indices[] = {
	0, 1, 2,
	0, 2, 3,
};

/* [=]===^=====================================================================================^===[=] */
__attribute__((cold, noinline, section(".init_section")))
static void setup_texture(GLuint texture, uint32_t width, uint32_t height) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, width, height, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

__attribute__((cold, noinline, section(".init_section")))
static GLuint compile_shader(GLenum shader_type, const GLchar *shader_source) {
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_source, 0);
	glCompileShader(shader);

	GLint success;
	GLchar info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(shader, sizeof(info_log), 0, info_log);
		printf("%s shader compilation failed:\n%s", shader_type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", info_log);
	}

	return shader;
}

__attribute__((cold, noinline, section(".init_section")))
static void setup_opengl(const char *vertex_shader_src, const char *fragment_shader_src, uint32_t setup_uniforms) {
	gl_loader(&opengl);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	// Shader setup
	GLuint vertex_shader = compile_shader(GL_VERTEX_SHADER, vertex_shader_src);
	GLuint fragment_shader = compile_shader(GL_FRAGMENT_SHADER, fragment_shader_src);

	state.shader_program = glCreateProgram();
	glAttachShader(state.shader_program, vertex_shader);
	glAttachShader(state.shader_program, fragment_shader);
	glLinkProgram(state.shader_program);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
	glUseProgram(state.shader_program);

	if(setup_uniforms) {
		// Cache uniform locations (only if required)
		state.uniform_resolution = glGetUniformLocation(state.shader_program, "resolution");
		state.uniform_src_image_size = glGetUniformLocation(state.shader_program, "src_image_size");
		state.uniform_brightness = glGetUniformLocation(state.shader_program, "brightness");
		state.uniform_tone = glGetUniformLocation(state.shader_program, "tone_data");
		state.uniform_crt_emulation = glGetUniformLocation(state.shader_program, "crt_emulation");
		state.uniform_sampler_location = glGetUniformLocation(state.shader_program, "iChannel0");
	}

	// VAO/VBO/EBO setup
	glGenVertexArrays(1, &state.vao);
	glGenBuffers(1, &state.vbo);
	glGenBuffers(1, &state.ebo);

	glBindVertexArray(state.vao);

	// Vertex data: Position (x, y) and Texture Coordinates (u, v)
	static const float vertices[] = {
		-1.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
		 1.0f, -1.0f, 1.0f, 0.0f, // Bottom-right
		 1.0f,  1.0f, 1.0f, 1.0f, // Top-right
		-1.0f,  1.0f, 0.0f, 1.0f  // Top-left
	};

	glBindBuffer(GL_ARRAY_BUFFER, state.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, state.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);							// Position
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));	// Texture coords
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Texture setup
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &state.texture);

	setup_texture(state.texture, state.screen_width, state.screen_height);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// NOTE(peter): This is used testing for opengl problems when adding more opengl functionality
#if 0
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam) {
	fprintf(stderr, "OpenGL Debug Message:\n");

	fprintf(stderr, "  Source: ");
	switch (source) {
		case GL_DEBUG_SOURCE_API:             fprintf(stderr, "API"); break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   fprintf(stderr, "Window System"); break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: fprintf(stderr, "Shader Compiler"); break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     fprintf(stderr, "Third Party"); break;
		case GL_DEBUG_SOURCE_APPLICATION:     fprintf(stderr, "Application"); break;
		case GL_DEBUG_SOURCE_OTHER:           fprintf(stderr, "Other"); break;
		default:                            fprintf(stderr, "Unknown"); break;
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "  Type: ");
	switch (type) {
		case GL_DEBUG_TYPE_ERROR:               fprintf(stderr, "Error"); break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: fprintf(stderr, "Deprecated Behavior"); break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  fprintf(stderr, "Undefined Behavior"); break;
		case GL_DEBUG_TYPE_PORTABILITY:       fprintf(stderr, "Portability"); break;
		case GL_DEBUG_TYPE_PERFORMANCE:       fprintf(stderr, "Performance"); break;
		case GL_DEBUG_TYPE_OTHER:               fprintf(stderr, "Other"); break;
		default:                            fprintf(stderr, "Unknown"); break;
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "  ID: %u\n", id);

	fprintf(stderr, "  Severity: ");
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         fprintf(stderr, "High"); break;
		case GL_DEBUG_SEVERITY_MEDIUM:       fprintf(stderr, "Medium"); break;
		case GL_DEBUG_SEVERITY_LOW:          fprintf(stderr, "Low"); break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: fprintf(stderr, "Notification"); break;
		default:                            fprintf(stderr, "Unknown"); break;
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "  Message: %s\n", message);
	fprintf(stderr, "\n");
}
#endif

// static GLuint create_font_texture(uint8_t* font_data, uint32_t width, uint32_t height) {
// 	GLuint texture;
// 	glGenTextures(1, &texture);
// 	setup_texture(texture, width, height);
// 	glBindTexture(GL_TEXTURE_2D, texture);
// 	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, font_data);
// 	return texture;
// }
