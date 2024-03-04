//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include <vector>
#include "core/renderer/vertex_array.hpp"

namespace gl_render_application {

    class opengl_vertex_array :
            public opengl_entity_render_object<GLuint>,
            public vertex_array {

    public:
        opengl_vertex_array();

        void bind() const override;

        void unbind() const override;

        void add_vertex_buffer(const Ref<vertex_buffer> &vertexBuffer) override;

        void set_index_buffer(const Ref<index_buffer> &indexBuffer) override;

        const std::vector<Ref<vertex_buffer>> &get_vertex_buffers() const override;

        const Ref<index_buffer> &get_index_buffer() const override;

    private:
        uint32_t m_vertex_buffer_index = 0;
        std::vector<Ref<vertex_buffer>> m_vertex_buffers;
        Ref<index_buffer> m_index_buffer;
    };
}



