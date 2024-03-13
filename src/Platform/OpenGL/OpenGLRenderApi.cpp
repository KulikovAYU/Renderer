//
// Created by Anton Kulikov on 03.03.2024.
//

#include "OpenGLRenderApi.hpp"
#include <iostream>
#include <glad/glad.h>

namespace multi_render_application{

    void OpenGLMessageCallback(
            unsigned source,
            unsigned type,
            unsigned id,
            unsigned severity,
            int length,
            const char* message,
            const void* userParam)
    {
        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:         std::cout <<"CRITICAL: "<<(message) << std::endl; return;
            case GL_DEBUG_SEVERITY_MEDIUM:       std::cout <<"ERROR: "<<(message) << std::endl; return;
            case GL_DEBUG_SEVERITY_LOW:          std::cout <<"WARNING: "<<(message) << std::endl; return;
            case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout <<"TRACE: "<<(message) << std::endl; return;
        }
    }



    void OpenGLRenderApi::Init() {
#ifdef DEBUG
        glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LINE_SMOOTH);
    }

    void OpenGLRenderApi::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderApi::SetClearColor(const glm::vec4 &color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGLRenderApi::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLRenderApi::DrawIndexed(const Ref<VertexArray> &vertex_array, uint32_t index_count) {
        vertex_array->Bind();
        uint32_t count = index_count != 0u ? index_count : vertex_array->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRenderApi::DrawLines(const Ref<VertexArray> &vertex_array, uint32_t vertex_count) {
        vertex_array->Bind();
        glDrawArrays(GL_LINES, 0, vertex_count);
    }

    void OpenGLRenderApi::SetLineWidth(float width) {
        glLineWidth(width);
    }
}