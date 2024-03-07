//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include "Renderer/RendererApi.hpp"

namespace multi_render_application {

    class OpenGLRenderApi : public RendererApi {

    public:
        void Init() override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;
        void Clear() override;

        void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
        void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;

        virtual void SetLineWidth(float width) override;
    };
}

