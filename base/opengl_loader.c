// Common OpenGL types
#ifdef _WIN32
typedef __int64 GLintptr;
#else
typedef intptr_t GLintptr;
#endif
typedef void GLvoid;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;
typedef char GLchar;

typedef int GLint;
typedef int GLsizei;

typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef unsigned int GLbitfield;

typedef float GLfloat;
typedef double GLdouble;

typedef unsigned long long GLsizeiptr;  // Size for buffer data.

// Constants that are frequently used
#define GL_NO_ERROR 0
#define GL_INFO_LOG_LENGTH 0x8B84

#define GL_ZERO 0x0000
#define GL_ONE 0x0001
#define GL_ALPHA 0x1906
#define GL_BLEND 0x0BE2
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_COLOR_BUFFER_BIT 0x4000
#define GL_COMPILE_STATUS 0x8B81
#define GL_DEPTH_TEST 0x0B71
#define GL_FRAMEBUFFER_SRGB 0x8DB9
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_LINK_STATUS 0x8B82
#define GL_MODELVIEW 0x1700
#define GL_NEAREST 0x2600
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_PROJECTION 0x1701
#define GL_QUADS 0x0007
#define GL_RGBA 0x1908
#define GL_RGBA8 0x8058 // Add this line
#define GL_SCISSOR_TEST 0x0C11
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_SRC_ALPHA 0x0302
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_UNSIGNED_BYTE 0x1401
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_VERTEX_SHADER 0x8B31
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_STATIC_DRAW 0x88E4
#define GL_FLOAT 0x1406
#define GL_FALSE 0
#define GL_TRUE 1
#define GL_LINEAR 0x2601
#define GL_FUNC_ADD 0x8006
#define GL_CULL_FACE 0x0B44
#define GL_STREAM_DRAW 0x88E0
#define GL_WRITE_ONLY 0x88B9
#define GL_TRIANGLES 0x0004
#define GL_UNSIGNED_INT 0x1405
#define GL_LINEAR 0x2601
#define GL_FUNC_ADD 0x8006
#define GL_CULL_FACE 0x0B44
#define GL_STREAM_DRAW 0x88E0
#define GL_WRITE_ONLY 0x88B9
#define GL_UNSIGNED_SHORT 0x1403
#define GL_MULTISAMPLE 0x809D
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_RED 0x1903

#define GL_DEBUG_OUTPUT 0x92E0
#define GL_DEBUG_SOURCE_API             0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM   0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY     0x8249
#define GL_DEBUG_SOURCE_APPLICATION     0x824A
#define GL_DEBUG_SOURCE_OTHER           0x824B
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY       0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B

typedef void type_glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void type_glGenTextures(GLsizei n, GLuint *textures);
typedef void type_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
typedef void type_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
typedef void type_glBlendFunc(GLenum sfactor, GLenum dfactor);
typedef void type_glEnable(GLenum cap);
typedef void type_glDisable(GLenum cap);
typedef void type_glActiveTexture(GLenum texture);
typedef void type_glClear(GLbitfield mask);
typedef void type_glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void type_glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void type_glActiveTexture(GLenum texture);
typedef void type_glAttachShader(GLuint program, GLuint shader);
typedef void type_glBindBuffer(GLenum target, GLuint buffer);
typedef void type_glBindTexture(GLenum target, GLuint texture);
typedef void type_glBufferData(GLenum target, GLsizeiptr size, const GLvoid *data, GLenum usage);
typedef void type_glClear(GLbitfield mask);
typedef void type_glClearColor(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void type_glCompileShader(GLuint shader);
typedef GLuint type_glCreateProgram(void);
typedef GLuint type_glCreateShader(GLenum type);
typedef void type_glDeleteShader(GLuint shader);
typedef void type_glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
typedef void type_glEnableVertexAttribArray(GLuint index);
typedef void type_glGenBuffers(GLsizei n, GLuint *buffers);
typedef void type_glGenTextures(GLsizei n, GLuint *textures);
typedef void type_glGetShaderInfoLog(GLuint shader, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void type_glGetShaderiv(GLuint shader, GLenum pname, GLint *params);
typedef GLint type_glGetUniformLocation(GLuint program, const GLchar *name);
typedef void type_glLinkProgram(GLuint program);
typedef void type_glShaderSource(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
typedef void type_glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *data);
typedef void type_glTexParameteri(GLenum target, GLenum pname, GLint param);
typedef void type_glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *data);
typedef void type_glUniform1f(GLint location, GLfloat v0);
typedef void type_glUniform1i(GLint location, GLint v0);
typedef void type_glUniform2f(GLint location, GLfloat v0, GLfloat v1);
typedef void type_glUniform4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void type_glUseProgram(GLuint program);
typedef void type_glVertexAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
typedef void type_glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void type_glDeleteProgram(GLuint program);
typedef void type_glDeleteBuffers(GLsizei n, const GLuint *buffers);
typedef void type_glDeleteTextures(GLsizei n, const GLuint *textures);
typedef void type_glEnable(GLenum cap);
typedef void type_glGenerateMipmap(GLenum target);
typedef void type_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
typedef GLint type_glGetAttribLocation(GLuint program, const GLchar *name);
typedef void type_glDetachShader(GLuint program, GLuint shader);
typedef void type_glUniformMatrix4fv(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void* type_glMapBuffer(GLenum target, GLenum access);
typedef GLboolean type_glUnmapBuffer(GLenum target);
typedef void type_glBlendEquation(GLenum mode);
typedef void type_glBlendFunc(GLenum sfactor, GLenum dfactor);
typedef void type_glDisable(GLenum cap);
typedef void type_glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
typedef void type_glTexCoord2f(GLfloat s, GLfloat t);
typedef void type_glVertex2f(GLfloat x, GLfloat y);
typedef GLenum type_glGetError(void);
typedef void type_glGetProgramiv(GLuint program, GLenum pname, GLint *params);
typedef void type_glGetProgramInfoLog(GLuint program, GLsizei maxLength, GLsizei *length, GLchar *infoLog);
typedef void type_glGenVertexArrays(GLsizei n, GLuint *arrays);
typedef void type_glBindVertexArray(GLuint array);
typedef void type_glDeleteVertexArrays(GLsizei n, const GLuint *arrays);
typedef void type_glDrawArrays(GLenum mode, GLint first, GLsizei count);
typedef void type_glBufferSubData(GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid *data);
typedef void type_glDisableVertexAttribArray(GLuint index);
typedef void type_glDebugMessageCallback(void (*callback)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam), const void* userParam);

#define OpenGLFunction(Name) type_##Name *Name
struct opengl {
	OpenGLFunction(glActiveTexture);
	OpenGLFunction(glAttachShader);
	OpenGLFunction(glBindBuffer);
	OpenGLFunction(glBindTexture);
	OpenGLFunction(glBufferData);
	OpenGLFunction(glClear);
	OpenGLFunction(glClearColor);
	OpenGLFunction(glCompileShader);
	OpenGLFunction(glCreateProgram);
	OpenGLFunction(glCreateShader);
	OpenGLFunction(glDeleteShader);
	OpenGLFunction(glDrawElements);
	OpenGLFunction(glEnableVertexAttribArray);
	OpenGLFunction(glGenBuffers);
	OpenGLFunction(glGenTextures);
	OpenGLFunction(glGetShaderInfoLog);
	OpenGLFunction(glGetShaderiv);
	OpenGLFunction(glGetUniformLocation);
	OpenGLFunction(glLinkProgram);
	OpenGLFunction(glShaderSource);
	OpenGLFunction(glTexImage2D);
	OpenGLFunction(glTexParameteri);
	OpenGLFunction(glTexSubImage2D);
	OpenGLFunction(glUniform1f);
	OpenGLFunction(glUniform1i);
	OpenGLFunction(glUniform2f);
	OpenGLFunction(glUniform4f);
	OpenGLFunction(glUseProgram);
	OpenGLFunction(glVertexAttribPointer);
	OpenGLFunction(glViewport);
	OpenGLFunction(glDeleteProgram);
	OpenGLFunction(glDeleteBuffers);
	OpenGLFunction(glDeleteTextures);
	OpenGLFunction(glEnable);
	OpenGLFunction(glGenerateMipmap);
	OpenGLFunction(glGetProgramiv);
	OpenGLFunction(glGetAttribLocation);
	OpenGLFunction(glDetachShader);
	OpenGLFunction(glUniformMatrix4fv);
	OpenGLFunction(glMapBuffer);
	OpenGLFunction(glUnmapBuffer);
	OpenGLFunction(glBlendEquation);
	OpenGLFunction(glBlendFunc);
	OpenGLFunction(glDisable);
	OpenGLFunction(glScissor);
	OpenGLFunction(glTexCoord2f);
	OpenGLFunction(glVertex2f);
	OpenGLFunction(glGetError);
	OpenGLFunction(glGetProgramInfoLog);
	OpenGLFunction(glGenVertexArrays);
	OpenGLFunction(glBindVertexArray);
	OpenGLFunction(glDeleteVertexArrays);
	OpenGLFunction(glDrawArrays);
	OpenGLFunction(glBufferSubData);
	OpenGLFunction(glDisableVertexAttribArray);
	OpenGLFunction(glDebugMessageCallback);
};
#ifdef NUKLEAR_OPENGL_STRUCT
extern
#endif
struct opengl opengl;

#if defined(_WIN32)
static void *get_any_gl_address(const char *name) {
    void *p = (void *)wglGetProcAddress(name);
    if(!p) {
        HMODULE module = LoadLibraryA("opengl32.dll");
        if(module) {
            p = (void *)GetProcAddress(module, name);
        }
    }
    return p;
}

#define GetOpenGLFunction(Name) \
    opengl->Name = (type_##Name *)get_any_gl_address(#Name); \
    if(!opengl->Name) { \
        fprintf(stderr, "Failed to load OpenGL function: %s\n", #Name); \
        exit(EXIT_FAILURE); \
    }

#elif defined(__linux__)
#include <dlfcn.h>

// Declare glXGetProcAddress for function loading
static void *glXGetProcAddress(const GLubyte *procName) {
	static void *(*glxGetProcAddress)(const GLubyte *);
	if(!glxGetProcAddress) {
		void *libGL = dlopen("libGL.so.1", RTLD_LAZY | RTLD_GLOBAL);
		if(!libGL) {
			fprintf(stderr, "Error: Unable to load libGL.so.1\n");
			exit(EXIT_FAILURE);
		}
		glxGetProcAddress = dlsym(libGL, "glXGetProcAddress");
		if(!glxGetProcAddress) {
			fprintf(stderr, "Error: Unable to find glXGetProcAddress\n");
			exit(EXIT_FAILURE);
		}
	}
	return glxGetProcAddress(procName);
}

#define GetOpenGLFunction(Name) \
	opengl->Name = (type_##Name *)glXGetProcAddress((const GLubyte *)#Name); \
	if(!opengl->Name) { \
		fprintf(stderr, "Failed to load OpenGL function: %s\n", #Name); \
		exit(EXIT_FAILURE); \
	}

#endif

__attribute__((cold, noinline, section(".init_section")))
static void gl_loader(struct opengl *opengl) {
	GetOpenGLFunction(glActiveTexture);
	GetOpenGLFunction(glAttachShader);
	GetOpenGLFunction(glBindBuffer);
	GetOpenGLFunction(glBindTexture);
	GetOpenGLFunction(glBufferData);
	GetOpenGLFunction(glClear);
	GetOpenGLFunction(glClearColor);
	GetOpenGLFunction(glCompileShader);
	GetOpenGLFunction(glCreateProgram);
	GetOpenGLFunction(glCreateShader);
	GetOpenGLFunction(glDeleteShader);
	GetOpenGLFunction(glDrawElements);
	GetOpenGLFunction(glEnableVertexAttribArray);
	GetOpenGLFunction(glGenBuffers);
	GetOpenGLFunction(glGenTextures);
	GetOpenGLFunction(glGetShaderInfoLog);
	GetOpenGLFunction(glGetShaderiv);
	GetOpenGLFunction(glGetUniformLocation);
	GetOpenGLFunction(glLinkProgram);
	GetOpenGLFunction(glShaderSource);
	GetOpenGLFunction(glTexImage2D);
	GetOpenGLFunction(glTexParameteri);
	GetOpenGLFunction(glTexSubImage2D);
	GetOpenGLFunction(glUniform1f);
	GetOpenGLFunction(glUniform1i);
	GetOpenGLFunction(glUniform2f);
	GetOpenGLFunction(glUniform4f);
	GetOpenGLFunction(glUseProgram);
	GetOpenGLFunction(glVertexAttribPointer);
	GetOpenGLFunction(glViewport);
	GetOpenGLFunction(glDeleteProgram);
	GetOpenGLFunction(glDeleteBuffers);
	GetOpenGLFunction(glDeleteTextures);
	GetOpenGLFunction(glEnable);
	GetOpenGLFunction(glGenerateMipmap);
	GetOpenGLFunction(glGetProgramiv);
	GetOpenGLFunction(glGetAttribLocation);
	GetOpenGLFunction(glDetachShader);
	GetOpenGLFunction(glUniformMatrix4fv);
	GetOpenGLFunction(glMapBuffer);
	GetOpenGLFunction(glUnmapBuffer);
	GetOpenGLFunction(glBlendEquation);
	GetOpenGLFunction(glBlendFunc);
	GetOpenGLFunction(glDisable);
	GetOpenGLFunction(glScissor);
	GetOpenGLFunction(glTexCoord2f);
	GetOpenGLFunction(glVertex2f);
	GetOpenGLFunction(glGetError);
	GetOpenGLFunction(glGetProgramInfoLog);
	GetOpenGLFunction(glGenVertexArrays);
	GetOpenGLFunction(glBindVertexArray);
	GetOpenGLFunction(glDeleteVertexArrays);
	GetOpenGLFunction(glDrawArrays);
	GetOpenGLFunction(glBufferSubData);
	GetOpenGLFunction(glDisableVertexAttribArray);
	GetOpenGLFunction(glDebugMessageCallback);
};

#define glActiveTexture opengl.glActiveTexture
#define glAttachShader opengl.glAttachShader
#define glBindBuffer opengl.glBindBuffer
#define glBindTexture opengl.glBindTexture
#define glBufferData opengl.glBufferData
#define glClear opengl.glClear
#define glClearColor opengl.glClearColor
#define glCompileShader opengl.glCompileShader
#define glCreateProgram opengl.glCreateProgram
#define glCreateShader opengl.glCreateShader
#define glDeleteShader opengl.glDeleteShader
#define glDrawElements opengl.glDrawElements
#define glEnableVertexAttribArray opengl.glEnableVertexAttribArray
#define glGenBuffers opengl.glGenBuffers
#define glGenTextures opengl.glGenTextures
#define glGetShaderInfoLog opengl.glGetShaderInfoLog
#define glGetShaderiv opengl.glGetShaderiv
#define glGetUniformLocation opengl.glGetUniformLocation
#define glLinkProgram opengl.glLinkProgram
#define glShaderSource opengl.glShaderSource
#define glTexImage2D opengl.glTexImage2D
#define glTexParameteri opengl.glTexParameteri
#define glTexSubImage2D opengl.glTexSubImage2D
#define glUniform1f opengl.glUniform1f
#define glUniform1i opengl.glUniform1i
#define glUniform2f opengl.glUniform2f
#define glUniform4f opengl.glUniform4f
#define glUseProgram opengl.glUseProgram
#define glVertexAttribPointer opengl.glVertexAttribPointer
#define glViewport opengl.glViewport
#define glDeleteProgram opengl.glDeleteProgram
#define glDeleteBuffers opengl.glDeleteBuffers
#define glDeleteTextures opengl.glDeleteTextures
#define glEnable opengl.glEnable
#define glGenerateMipmap opengl.glGenerateMipmap
#define glGetProgramiv opengl.glGetProgramiv
#define glGetAttribLocation opengl.glGetAttribLocation
#define glDetachShader opengl.glDetachShader
#define glUniformMatrix4fv opengl.glUniformMatrix4fv
#define glMapBuffer opengl.glMapBuffer
#define glUnmapBuffer opengl.glUnmapBuffer
#define glBlendEquation opengl.glBlendEquation
#define glBlendFunc opengl.glBlendFunc
#define glDisable opengl.glDisable
#define glScissor opengl.glScissor
#define glTexCoord2f opengl.glTexCoord2f
#define glVertex2f opengl.glVertex2f
#define glGetError opengl.glGetError
#define glGetProgramInfoLog opengl.glGetProgramInfoLog
#define glGenVertexArrays opengl.glGenVertexArrays
#define glBindVertexArray opengl.glBindVertexArray
#define glDeleteVertexArrays opengl.glDeleteVertexArrays
#define glDrawArrays opengl.glDrawArrays
#define glBufferSubData opengl.glBufferSubData
#define glDisableVertexAttribArray opengl.glDisableVertexAttribArray
#define glDebugMessageCallback opengl.glDebugMessageCallback
