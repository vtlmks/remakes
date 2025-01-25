/* Overlay state structure */
struct overlay {
	GLuint vao;
	GLuint vbo;
	GLuint ebo;
	GLuint program;
	GLint  loc_proj;
	GLint  loc_tex;
	GLint  loc_color;
	GLint  loc_pos_offset;
	GLuint font_texture;
	GLuint white_texture;
};

static struct overlay overlay_state;
static float mat[16];

/* ------------------------------------------------------------------------- */
/* Shader sources for a top-left orthographic approach                      */
/* ------------------------------------------------------------------------- */
static const char* overlay_vertex_shader_src =
"#version 140\n"
"in vec2 in_pos;\n"
"in vec2 in_uv;\n"
"uniform mat4 u_projection;\n"
"uniform vec2 u_pos_offset;\n"
"out vec2 v_uv;\n"
"void main() {\n"
"    vec2 pos = in_pos + u_pos_offset;\n"
"    gl_Position = u_projection * vec4(pos, 0.0, 1.0);\n"
"    v_uv = in_uv;\n"
"}\n";

static const char* overlay_fragment_shader_src =
"#version 140\n"
"uniform sampler2D u_font_texture;\n"
"uniform vec4 u_color;\n"
"in vec2 v_uv;\n"
"out vec4 frag_color;\n"
"void main() {\n"
"    float alpha = texture(u_font_texture, v_uv).r;\n"
"    frag_color = vec4(u_color.rgb, u_color.a * alpha);\n"
"}\n";

/* ------------------------------------------------------------------------- */
/* Shader helpers                                                            */
/* ------------------------------------------------------------------------- */
__attribute__((cold, noinline, section(".init_section")))
static GLuint overlay_compile_shader(const char *source, GLenum type) {
	GLuint shader = glCreateShader(type);
	if(shader == 0) {
		fprintf(stderr, "Error creating shader of type %d.\n", type);
		return 0;
	}

	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	/* Check for compilation errors */
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		/* Get and print log */
		GLint log_size = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_size);
		char *log = (char*)malloc(log_size);
		if(log) {
			glGetShaderInfoLog(shader, log_size, 0, log);
			fprintf(stderr, "Shader compilation failed:\n%s\n", log);
			free(log);
		} else {
			fprintf(stderr, "Shader compilation failed, but failed to allocate memory for log.\n");
		}
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

__attribute__((cold, noinline, section(".init_section")))
static GLuint overlay_create_program(const char *vs_source, const char *fs_source) {
	GLuint vs = overlay_compile_shader(vs_source, GL_VERTEX_SHADER);
	GLuint fs = overlay_compile_shader(fs_source, GL_FRAGMENT_SHADER);
	GLuint prog = glCreateProgram();

	glAttachShader(prog, vs);
	glAttachShader(prog, fs);
	glLinkProgram(prog);

	/* Check for linking errors */
	GLint success;
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if(!success) {
		/* Get and print log */
		GLint log_size = 0;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &log_size);
		char *log = (char*)malloc(log_size);
		if(log) {
			glGetProgramInfoLog(prog, log_size, 0, log);
			fprintf(stderr, "Program linking failed:\n%s\n", log);
			free(log);
		} else {
			fprintf(stderr, "Program linking failed, but failed to allocate memory for log.\n");
		}
		glDeleteProgram(prog);
		glDeleteShader(vs);
		glDeleteShader(fs);
		return 0;
	}

	/* Shaders can be deleted after linking */
	glDeleteShader(vs);
	glDeleteShader(fs);

	return prog;
}

static void overlay_make_ortho_top_left(float w, float h, float m[16]) {
	m[0] =  2.0f / w;
	m[5] = -2.0f / h;
	m[10] = -1.0f;
	m[12] = -1.0f;
	m[13] =  1.0f;
	m[15] =  1.0f;
}

__attribute__((cold, noinline, section(".init_section")))
static void overlay_init(void) {
	overlay_state.program = overlay_create_program(overlay_vertex_shader_src, overlay_fragment_shader_src);

	overlay_state.loc_proj       = glGetUniformLocation(overlay_state.program, "u_projection");
	overlay_state.loc_tex        = glGetUniformLocation(overlay_state.program, "u_font_texture");
	overlay_state.loc_color      = glGetUniformLocation(overlay_state.program, "u_color");
	overlay_state.loc_pos_offset = glGetUniformLocation(overlay_state.program, "u_pos_offset");

	glGenVertexArrays(1, &overlay_state.vao);
	glBindVertexArray(overlay_state.vao);

	glGenBuffers(1, &overlay_state.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, overlay_state.vbo);
	glBufferData(GL_ARRAY_BUFFER, 256 * 4 * 4 * sizeof(float), 0, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &overlay_state.ebo);

	// NOTE(peter): Generate ebo indices to handle up to 256 quads.
	unsigned short indices[256 * 6];
	size_t index_offset = 0;
	size_t vertex_offset = 0;

	for(size_t i = 0; i < 256; i++) {
		indices[index_offset++] = vertex_offset + 0;		// First triangle: top-left, top-right, bottom-right
		indices[index_offset++] = vertex_offset + 1;
		indices[index_offset++] = vertex_offset + 2;
		indices[index_offset++] = vertex_offset + 0;		// Second triangle: top-left, bottom-right, bottom-left
		indices[index_offset++] = vertex_offset + 2;
		indices[index_offset++] = vertex_offset + 3;
		vertex_offset += 4;
	}

	// Bind the EBO and upload the data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, overlay_state.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	GLint in_pos_attrib = glGetAttribLocation(overlay_state.program, "in_pos");
	GLint in_uv_attrib  = glGetAttribLocation(overlay_state.program, "in_uv");

	glEnableVertexAttribArray(in_pos_attrib);
	glVertexAttribPointer(in_pos_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(in_uv_attrib);
	glVertexAttribPointer(in_uv_attrib, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// The font data is 0 or 1, so boost values to 0 or 255, else the characters will hardly be visible
	for(uint32_t i = 0; i < font_texture_data->width * font_texture_data->height; ++i) {
		font_texture_data->data[i] = font_texture_data->data[i] ? 255 : 0;
	}

	glGenTextures(1, &overlay_state.font_texture);
	glBindTexture(GL_TEXTURE_2D, overlay_state.font_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 128, 128, 0, GL_RED, GL_UNSIGNED_BYTE, font_texture_data->data);

	glGenTextures(1, &overlay_state.white_texture);
	glBindTexture(GL_TEXTURE_2D, overlay_state.white_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	uint32_t white[] = { 0xffffffff };
	glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, white);

	glBindTexture(GL_TEXTURE_2D, 0);
}

__attribute__((cold, noinline, section(".init_section")))
static void overlay_shutdown(void) {
	glDeleteProgram(overlay_state.program);
	glDeleteBuffers(1, &overlay_state.vbo);
	glDeleteBuffers(1, &overlay_state.ebo);
	glDeleteVertexArrays(1, &overlay_state.vao);
	glDeleteTextures(1, &overlay_state.font_texture);
}

static void overlay_render_rect(float x1, float y1, float x2, float y2, float r, float g, float b, float a) {
	float vertices[4 * 4] = {
		/* pos.x, pos.y, u, v */
		x1, y1,  0.f, 0.f, // Top-left
		x2, y1,  1.f, 0.f, // Top-right
		x2, y2,  1.f, 1.f, // Bottom-right
		x1, y2,  0.f, 1.f  // Bottom-left
	};

	glBindVertexArray(overlay_state.vao);
	glBindBuffer(GL_ARRAY_BUFFER, overlay_state.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glUseProgram(overlay_state.program);
	glUniform4f(overlay_state.loc_color, r, g, b, a);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, overlay_state.white_texture); // No texture bound
	glUniform1i(overlay_state.loc_tex, 0);
	glUniform2f(overlay_state.loc_pos_offset, 0.0f, 0.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

static float vertices[256 * 4 * 4];
static void overlay_render_text_line(uint8_t *text, float x, float y, float r, float g, float b, float a, float offset_x, float offset_y) {
	size_t glyph_count = 0;
	size_t offset = 0;

	// Process the string using a do-while loop
	while(1) {
		uint8_t c = *text++;
		if(c == 0) break;

		float u0 = glyph_data[c].x / 128.f;
		float v0 = glyph_data[c].y / 128.f;
		float u1 = (glyph_data[c].x + 7) / 128.f;
		float v1 = (glyph_data[c].y + 14) / 128.f;

		// Calculate vertices for this glyph
		vertices[offset + 0] = x;				// x0
		vertices[offset + 1] = y;				// y0
		vertices[offset + 2] = u0;				// u0
		vertices[offset + 3] = v0;				// v0

		vertices[offset + 4] = x + 7.f;		// x1
		vertices[offset + 5] = y;				// y1
		vertices[offset + 6] = u1;				// u1
		vertices[offset + 7] = v0;				// v0

		vertices[offset + 8] = x + 7.f;		// x2
		vertices[offset + 9] = y + 14.f;		// y2
		vertices[offset + 10] = u1;			// u2
		vertices[offset + 11] = v1;			// v1

		vertices[offset + 12] = x;     	 	// x3
		vertices[offset + 13] = y + 14.f;	// y3
		vertices[offset + 14] = u0;			// u3
		vertices[offset + 15] = v1;			// v1

		x += 7.f;
		offset += 16;
		glyph_count++;
	}

	// Upload all vertices at once
	glBindVertexArray(overlay_state.vao);
	glBindBuffer(GL_ARRAY_BUFFER, overlay_state.vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, glyph_count * 4 * 4 * sizeof(float), vertices);

	// Set shader uniforms
	glUseProgram(overlay_state.program);
	glUniform4f(overlay_state.loc_color, r, g, b, a);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, overlay_state.font_texture);
	glUniform1i(overlay_state.loc_tex, 0);
	glUniform2f(overlay_state.loc_pos_offset, offset_x, offset_y);

	// Render glyphs in one draw call
	glDrawElements(GL_TRIANGLES, glyph_count * 6, GL_UNSIGNED_SHORT, 0);
	glBindVertexArray(0);
}

static void overlay_render(float rect_x, float rect_y, float rect_w, float rect_h, uint8_t **lines, int window_w, int window_h) {
	overlay_make_ortho_top_left((float)window_w, (float)window_h, mat);
	glViewport(0.f, 0.f, window_w, window_h);

	glUseProgram(overlay_state.program);
	glUniformMatrix4fv(overlay_state.loc_proj, 1, GL_FALSE, mat);

	overlay_render_rect(rect_x, rect_y, rect_x + rect_w, rect_y + rect_h, .01f, .013f, .04f, .6f); // Semi-transparent black

	float line_spacing = 14.0f;
	float x = rect_x + 4.0f;
	float y = rect_y + 4.0f;

	for(int i = 0; i < MAX_PROFILING_ENTRIES; i++) {
		uint8_t *line = lines[i];
		if(!line) continue;

		overlay_render_text_line(line, x + 1.0f, y + 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);	// Shadow: black, offset by 1 pixel down and right
		overlay_render_text_line(line, x, y, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);						// Main text: white, no offset

		y += line_spacing;
	}
}

