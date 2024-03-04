//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include "core/renderer/buffer.hpp"
#include "glad/glad.h"
#include "opengl_entity_render_object.hpp"

namespace gl_render_application {

    class open_gl_vertex_buffer :
            public opengl_entity_render_object<GLuint>,
            public vertex_buffer{

    public:
        open_gl_vertex_buffer(uint32_t size);

        open_gl_vertex_buffer(float* vertices, uint32_t size);

        void bind() const override;

        void unbind() const override;

        void set_data(const void* data, uint32_t size) override;
    };

    class open_gl_index_buffer :
            public opengl_entity_render_object<GLuint>,
            public index_buffer{

    public:
        open_gl_index_buffer(uint32_t* indices, uint32_t count);

        void bind() const override;

        void unbind() const override;

        uint32_t get_count() const override;

    private:
        uint32_t m_count;
    };
}