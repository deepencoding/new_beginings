#pragma once

#include <glad/glad.h>
#include <iostream>

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line) {
    if (GLenum error = glGetError()) {
        std::cout << "[OpenGL Error]: " << error << " : @" << function << ":" << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))