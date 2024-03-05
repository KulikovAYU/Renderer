//
// Created by Anton Kulikov on 28.02.2024.
//
#pragma once

#include <iostream>
#include "glad/glad.h"

#define GL_C_ASSERT(x) if(!(x)) __builtin_trap();
#define GLCall(x) GlClearError(); \
    x;\
    GL_C_ASSERT(GlLogCall(#x, __FILE__, __LINE__))

static void GlClearError(){
    while (glGetError() != GL_NO_ERROR);
}

static bool GlLogCall(const char* function, const char* file, int line){
    while (GLenum glError = glGetError())
    {
        std::cout << "[OpenGL Error] (" << glError << "): "
                  << function << " " << file << ":" << line << std::endl;
        return false;
    }

    return true;
}
