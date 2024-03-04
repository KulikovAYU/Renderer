//
// Created by Anton Kulikov on 03.03.2024.
//

#include "opengl_buffers.hpp"
#include "opengl_vertex_array.hpp"

namespace gl_render_application{

    open_gl_vertex_buffer::open_gl_vertex_buffer(uint32_t size) {
        glCreateBuffers(1, &m_renderId);
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    open_gl_vertex_buffer::open_gl_vertex_buffer(float *vertices, uint32_t size) {
        glCreateBuffers(1, &m_renderId);
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    void open_gl_vertex_buffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
    }

    void open_gl_vertex_buffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void open_gl_vertex_buffer::set_data(const void *data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }


    open_gl_index_buffer::open_gl_index_buffer(uint32_t *indices, uint32_t count) : m_count{count}{
        glCreateBuffers(1, &m_renderId);

        // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
        // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    void open_gl_index_buffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderId);
    }

    void open_gl_index_buffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t open_gl_index_buffer::get_count() const {
        return m_count;
    }

    opengl_vertex_array::opengl_vertex_array() {
        glCreateVertexArrays(1, &m_renderId);
    }

    void opengl_vertex_array::bind() const{
        glBindVertexArray(m_renderId);
    }

    void opengl_vertex_array::unbind() const{
        glBindVertexArray(0);
    }

    void opengl_vertex_array::add_vertex_buffer(const Ref<vertex_buffer> &vertexBuffer) {
        glBindVertexArray(m_renderId);
        vertexBuffer->bind();

        //TODO: parse shaider layout

        m_vertex_buffers.push_back(vertexBuffer);
    }

    void opengl_vertex_array::set_index_buffer(const Ref<index_buffer> &indexBuffer) {

        glBindVertexArray(m_renderId);
        indexBuffer->bind();

        m_index_buffer = indexBuffer;
    }

    const std::vector<Ref<vertex_buffer>> &opengl_vertex_array::get_vertex_buffers() const {
        return m_vertex_buffers;
    }

    const Ref<index_buffer> &opengl_vertex_array::get_index_buffer() const {
        return m_index_buffer;
    }
}
