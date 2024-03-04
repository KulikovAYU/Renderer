//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once
#include "../base.hpp"
#include "buffer.hpp"

namespace gl_render_application {
    class vertex_array {
    public:
        virtual ~vertex_array() = default;

        virtual void bind() const = 0;

        virtual void unbind() const = 0;

        virtual void add_vertex_buffer(const Ref<vertex_buffer> &vertexBuffer) = 0;

        virtual void set_index_buffer(const Ref<index_buffer> &indexBuffer) = 0;

        virtual const std::vector<Ref<vertex_buffer>> &get_vertex_buffers() const = 0;

        virtual const Ref<index_buffer> &get_index_buffer() const = 0;

        //static Ref<vertex_array> Create();
    };
}