//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include "ShaderData.hpp"

namespace multi_render_application {

    class VertexBuffer {
    public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetData(const void* data, uint32_t size) = 0;

//        virtual const buffer_layout& GetLayout() const = 0;
//        virtual void SetLayout(const buffer_layout& layout) = 0;

//        static Ref<vertex_buffer> Create(uint32_t size);
//        static Ref<vertex_buffer> Create(float* vertices, uint32_t size);
    };

    // supports 32-bit index buffers
    class IndexBuffer
    {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        [[nodiscard]] virtual uint32_t GetCount() const = 0;

        //static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };

    struct BufferElement
    {
        std::string Name;
        shader_data_type Type;
        uint32_t Size;
        size_t Offset;
        bool Normalized;

        BufferElement() = default;

        BufferElement(shader_data_type type, std::string  name, bool normalized = false)
                : Name(std::move(name)), Type(type), Size(shader_data_type_size(type)), Offset(0), Normalized(normalized)
        {
        }

        [[nodiscard]] uint32_t GetComponentCount() const
        {
            switch (Type)
            {
                case shader_data_type::Float:   return 1;
                case shader_data_type::Float2:  return 2;
                case shader_data_type::Float3:  return 3;
                case shader_data_type::Float4:  return 4;
                case shader_data_type::Mat3:    return 3; // 3* float3
                case shader_data_type::Mat4:    return 4; // 4* float4
                case shader_data_type::Int:     return 1;
                case shader_data_type::Int2:    return 2;
                case shader_data_type::Int3:    return 3;
                case shader_data_type::Int4:    return 4;
                case shader_data_type::Bool:    return 1;
                case shader_data_type::None:
                    break;
            }

            return 0;
        }
    };

    class BufferLayout
    {
    public:
        BufferLayout() {}

        BufferLayout(std::initializer_list<BufferElement> elements)
                : m_elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        [[nodiscard]] uint32_t GetStride() const { return m_Stride; }
        [[nodiscard]] const std::vector<BufferElement>& GetElements() const { return m_elements; }

        std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_elements.end(); }
        [[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
        [[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }
    private:
        void CalculateOffsetsAndStride()
        {
            size_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_elements)
            {
                element.Offset = offset;
                offset += element.Size;
                m_Stride += element.Size;
            }
        }
    private:
        std::vector<BufferElement> m_elements;
        uint32_t m_Stride = 0;
    };
}