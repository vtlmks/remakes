/*
 * GLSL Shader Code Embedded as C Header
 * Generated by: ../bin/shader2h
 * GLSL Version: 140
 */

const char *vertex_shader_start =
	"#version 140\n"
	"\n"
	"in vec2 position;\n"
	"in vec2 texture_coord;\n"
	"\n"
	"out vec2 frag_texture_coord;\n"
	"\n"
	"void main() {\n"
	"	frag_texture_coord = texture_coord;\n"
	"	gl_Position = vec4(position, 0.0, 1.0);\n"
	"}\n";

