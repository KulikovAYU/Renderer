//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include "Renderer/VertexArray.hpp"
#include "OpenGLEntityRenderObject.hpp"

namespace multi_render_application {

    class OpenGLVertexArray :
            public OpenGLEntityRenderObject<GLuint>,
            public VertexArray {

    public:
        OpenGLVertexArray();

        void Bind() const override;

        void Unbind() const override;

        void AddVertexBuffer(const Ref<VertexBuffer> &vertex_buffer) override;

        void SetIndexBuffer(const Ref<IndexBuffer> &index_buffer) override;

        [[nodiscard]] const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const override;

        [[nodiscard]] const Ref<IndexBuffer> &GetIndexBuffer() const override;

    private:
        uint32_t m_vertex_buffer_index = 0;
        std::vector<Ref<VertexBuffer>> m_vertex_buffers;
        Ref<IndexBuffer> m_index_buffer;
    };
}