//
// Created by tosha on 09.03.2024.
//

#include "OpenGLVertexArray.hpp"


namespace multi_render_application {

    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }

        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray() {
        glCreateVertexArrays(1, &m_renderId);
    }

    void OpenGLVertexArray::Bind() const {
        glBindVertexArray(m_renderId);
    }

    void OpenGLVertexArray::Unbind() const {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertex_buffer) {

        //TODO: ASSERT IF NO LAYOUT

        glBindVertexArray(m_renderId);
        vertex_buffer->Bind();

        const auto& layout = vertex_buffer->GetLayout();
        for(const auto& element : layout)
        {
            switch(element.Type)
            {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4:
                {
                    glEnableVertexAttribArray(m_vertex_buffer_index);
                    glVertexAttribPointer(m_vertex_buffer_index,
                                          element.GetComponentCount(),
                                          ShaderDataTypeToOpenGLBaseType(element.Type),
                                          element.Normalized ? GL_TRUE : GL_FALSE,
                                          layout.GetStride(),
                                          (const void*)element.Offset);
                    m_vertex_buffer_index++;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool:
                {
                    glEnableVertexAttribArray(m_vertex_buffer_index);
                    glVertexAttribIPointer(m_vertex_buffer_index,
                                           element.GetComponentCount(),
                                           ShaderDataTypeToOpenGLBaseType(element.Type),
                                           layout.GetStride(),
                                           (const void*)element.Offset);
                    m_vertex_buffer_index++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        glEnableVertexAttribArray(m_vertex_buffer_index);
                        glVertexAttribPointer(m_vertex_buffer_index,
                                              count,
                                              ShaderDataTypeToOpenGLBaseType(element.Type),
                                              element.Normalized ? GL_TRUE : GL_FALSE,
                                              layout.GetStride(),
                                              (const void*)(element.Offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(m_vertex_buffer_index, 1);
                        m_vertex_buffer_index++;
                    }
                    break;
                }

                case ShaderDataType::None:
                default:
                    //TODO: Assert
                    break;
            }
        }

        m_vertex_buffers.push_back(vertex_buffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& index_buffer) {
        glBindVertexArray(m_renderId);
        index_buffer->Bind();

        m_index_buffer = index_buffer;
    }

    const std::vector<Ref<VertexBuffer>>& OpenGLVertexArray::GetVertexBuffers() const {
        return m_vertex_buffers;
    }

    const Ref<IndexBuffer>& OpenGLVertexArray::GetIndexBuffer() const {
        return m_index_buffer;
    }

}  // namespace multi_render_application