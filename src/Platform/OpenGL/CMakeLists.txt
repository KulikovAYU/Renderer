cmake_minimum_required(VERSION 3.14)

set(OPENGL_PLATFORM_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLBuffers.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLVertexArray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLEntityRenderObject.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLRenderApi.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLShader.hpp
        OpenGLDebug.hpp
)

set(OPENGL_PLATFORM_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLBuffers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLTypeMapping.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLRenderApi.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLShader.cpp
        OpenGLVertexArray.cpp
)

add_library(OpenGlPlatformLibrary STATIC
        ${OPENGL_PLATFORM_INCLUDES}
        ${OPENGL_PLATFORM_SOURCES})

set(GLM_DIR ${THIRDPARTY_DIR}/glm)
add_subdirectory(${GLM_DIR} ${CMAKE_BINARY_DIR}/glm)

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
set(GLAD_DIR ${THIRDPARTY_DIR}/glad)
add_library(Glad ${GLAD_DIR}/src/glad.c)

add_subdirectory(${THIRDPARTY_DIR}/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_include_directories(OpenGlPlatformLibrary PUBLIC ${THIRDPARTY_DIR}/glfw/include)

target_include_directories(Glad PRIVATE ${GLAD_DIR}/include)
target_include_directories(OpenGlPlatformLibrary PUBLIC ${GLAD_DIR}/include)
target_include_directories(OpenGlPlatformLibrary PUBLIC ${CMAKE_SOURCE_DIR}/src/core)
target_include_directories(OpenGlPlatformLibrary PUBLIC ${GLM_DIR})
target_link_libraries(OpenGlPlatformLibrary PRIVATE Glad)

#включаем неявно base.hpp во все translation units
#target_compile_options(OpenGlPlatformLibrary PRIVATE -include ${CMAKE_SOURCE_DIR}/src/Core/base.hpp)
target_precompile_headers(OpenGlPlatformLibrary PRIVATE ${CMAKE_SOURCE_DIR}/src/Core/Basepch.hpp)