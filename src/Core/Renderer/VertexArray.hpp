//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once
#include "Buffer.hpp"

namespace multi_render_application {

    struct VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) = 0;

        virtual void SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) = 0;

        [[nodiscard]] virtual const std::vector<Ref<VertexBuffer>> &GetVertexBuffers() const = 0;

        [[nodiscard]] virtual const Ref<IndexBuffer> &GetIndexBuffer() const = 0;

        //static Ref<vertex_array> Create();
    };
}