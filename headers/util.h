#pragma once

#include <glad/glad.h>
#include <iostream>
/*
void gl_check_error(const char* function, const char* file, int line);

#define CHECK_GL_ERRORS

#ifdef CHECK_GL_ERRORS
#define GCE gl_check_error(__FUNCTION__, __FILE__, __LINE__);
#else
#define GCE
#endif
*/

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    while (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error]: " << error << " : @" << function << ":" << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))