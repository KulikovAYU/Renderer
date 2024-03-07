//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include "Renderer/Buffer.hpp"
#include "OpenGLEntityRenderObject.hpp"

namespace multi_render_application {

    class OpenGLVertexBuffer :
            public OpenglEntityRenderObjectId,
            public VertexBuffer{

    public:
        OpenGLVertexBuffer(uint32_t size);

        OpenGLVertexBuffer(float* vertices, uint32_t size);

        void Bind() const override;

        void Unbind() const override;

        void SetData(const void* data, uint32_t size) override;
    };

    class OpenglIndexBuffer :
            public OpenglEntityRenderObjectId,
            public IndexBuffer{

    public:
        OpenglIndexBuffer(uint32_t* indices, uint32_t count);

        void Bind() const override;

        void Unbind() const override;

        [[nodiscard]] uint32_t GetCount() const override;

    private:
        uint32_t m_count;
    };
}