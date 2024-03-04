//
// Created by Anton Kulikov on 03.03.2024.
//
#pragma once
#include "glad/glad.h"

template <typename T>
struct gl_type_map { static constexpr GLuint annotation = GL_DEBUG_TYPE_ERROR; };

#define DECLARE_GL_TYPE_MAP(Type, GlType) \
template <> \
constexpr const GLuint gl_type_map<Type>::annotation = GlType;

DECLARE_GL_TYPE_MAP(float, GL_FLOAT)
DECLARE_GL_TYPE_MAP(int, GL_INT)
DECLARE_GL_TYPE_MAP(unsigned int, GL_UNSIGNED_INT)