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

    void OpenGLRenderApi::DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount) {
        vertexArray->bind();
        uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }

    void OpenGLRenderApi::DrawLines(const Ref<VertexArray> &vertexArray, uint32_t vertexCount) {
        vertexArray->bind();
        glDrawArrays(GL_LINES, 0, vertexCount);
    }

    void OpenGLRenderApi::SetLineWidth(float width) {
        glLineWidth(width);
    }
}