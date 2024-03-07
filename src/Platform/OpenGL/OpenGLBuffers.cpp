//
// Created by Anton Kulikov on 03.03.2024.
//

#include "OpenGLBuffers.hpp"
#include "OpenGLVertexArray.hpp"

namespace multi_render_application{

    OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
        glCreateBuffers(1, &m_renderId);
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size) {
        glCreateBuffers(1, &m_renderId);
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    void OpenGLVertexBuffer::Bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
    }

    void OpenGLVertexBuffer::Unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenGLVertexBuffer::SetData(const void *data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    OpenglIndexBuffer::OpenglIndexBuffer(uint32_t *indices, uint32_t count) : m_count{count}{
        glCreateBuffers(1, &m_renderId);

        // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
        // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state.
        glBindBuffer(GL_ARRAY_BUFFER, m_renderId);
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    void OpenglIndexBuffer::Bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderId);
    }

    void OpenglIndexBuffer::Unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    uint32_t OpenglIndexBuffer::GetCount() const {
        return m_count;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        glCreateVertexArrays(1, &m_renderId);
    }

    void OpenGLVertexArray::bind() const{
        glBindVertexArray(m_renderId);
    }

    void OpenGLVertexArray::Unbind() const{
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer> &vertexBuffer) {
        glBindVertexArray(m_renderId);
        vertexBuffer->Bind();

        //TODO: parse shaider layout

        m_vertex_buffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer> &indexBuffer) {

        glBindVertexArray(m_renderId);
        indexBuffer->Bind();

        m_index_buffer = indexBuffer;
    }

    const std::vector<Ref<VertexBuffer>> &OpenGLVertexArray::GetVertexBuffers() const {
        return m_vertex_buffers;
    }

    const Ref<IndexBuffer> &OpenGLVertexArray::GetIndexBuffer() const {
        return m_index_buffer;
    }
}
