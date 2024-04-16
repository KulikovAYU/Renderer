cmake_minimum_required(VERSION 3.14)

set(NAME OpenGlPlatformLibrary)

set(OPENGL_PLATFORM_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLBuffers.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLVertexArray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLEntityRenderObject.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLRenderApi.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLShader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLDebug.hpp
)

set(OPENGL_PLATFORM_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLBuffers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLTypeMapping.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLRenderApi.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLShader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLVertexArray.cpp
)

add_library(${NAME} STATIC
        ${OPENGL_PLATFORM_INCLUDES}
        ${OPENGL_PLATFORM_SOURCES})

#включаем неявно base.hpp во все translation units
#target_compile_options(OpenGlPlatformLibrary PRIVATE -include ${CMAKE_SOURCE_DIR}/src/Core/base.hpp)
target_precompile_headers(${NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Basepch.hpp)