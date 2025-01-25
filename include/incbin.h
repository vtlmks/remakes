

#define STR2(x) #x
#define STR(x) STR2(x)

#ifdef _WIN32
#define INCBIN_SECTION ".data, \"aw\""
#else
#define INCBIN_SECTION ".data"
#endif

/*
#ifdef _WIN32
#define INCBIN_SECTION ".rdata, \"dr\""
#else
#define INCBIN_SECTION ".rodata"
#endif
*/

#define INCBIN(name, file) \
	__asm__(".section " INCBIN_SECTION "\n" \
		".global " STR(name) "_data\n" \
		".balign 64\n" \
		STR(name) "_data:\n" \
		".incbin " STR(file) "\n" \
		".global " STR(name) "_end\n" \
		".balign 1\n" \
		STR(name) "_end:\n"); \
	extern __attribute__((aligned(64)))	char name##_data[]; \
	extern 										char name##_end[];

/*
// INCBIN_SHADER(vertexshader, "#version 140", "shader_header.glsl", "vertex_shader.glsl");
#define INCBIN_SHADER(name, version_str, header_file, shader_file) \
	__asm__(".section " INCBIN_SECTION "\n" \
		".global " STR(name) "_data\n" \
		".balign 64\n" \
		STR(name) "_data:\n" \
		".ascii \"" version_str "\\n\"\n" \
		".incbin \"" header_file "\"\n" \
		".incbin \"" shader_file "\"\n" \
		".byte 0\n" \
		".global " STR(name) "_end\n" \
		".balign 1\n" \
		STR(name) "_end:\n"); \
	extern __attribute__((aligned(64))) char name##_data[]; \
	extern                              char name##_end[];

*/
