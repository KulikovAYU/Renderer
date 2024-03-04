//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include <vector>
#include <string>
#include "core/base.hpp"
#include "shader_data.hpp"

namespace gl_render_application {

    class vertex_buffer
    {
    public:
        virtual ~vertex_buffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void set_data(const void* data, uint32_t size) = 0;

//        virtual const buffer_layout& GetLayout() const = 0;
//        virtual void SetLayout(const buffer_layout& layout) = 0;

//        static Ref<vertex_buffer> Create(uint32_t size);
//        static Ref<vertex_buffer> Create(float* vertices, uint32_t size);
    };

    // supports 32-bit index buffers
    class index_buffer
    {
    public:
        virtual ~index_buffer() = default;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t get_count() const = 0;

        //static Ref<IndexBuffer> Create(uint32_t* indices, uint32_t count);
    };

    struct buffer_element
    {
        std::string Name;
        shader_data_type Type;
        uint32_t Size;
        size_t Offset;
        bool Normalized;

        buffer_element() = default;

        buffer_element(shader_data_type type, const std::string& name, bool normalized = false)
                : Name(name), Type(type), Size(shader_data_type_size(type)), Offset(0), Normalized(normalized)
        {
        }

        uint32_t GetComponentCount() const
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
            }

            return 0;
        }
    };

    class buffer_layout
    {
    public:
        buffer_layout() {}

        buffer_layout(std::initializer_list<buffer_element> elements)
                : m_elements(elements)
        {
            CalculateOffsetsAndStride();
        }

        uint32_t GetStride() const { return m_Stride; }
        const std::vector<buffer_element>& GetElements() const { return m_elements; }

        std::vector<buffer_element>::iterator begin() { return m_elements.begin(); }
        std::vector<buffer_element>::iterator end() { return m_elements.end(); }
        std::vector<buffer_element>::const_iterator begin() const { return m_elements.begin(); }
        std::vector<buffer_element>::const_iterator end() const { return m_elements.end(); }
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
        std::vector<buffer_element> m_elements;
        uint32_t m_Stride = 0;
    };
}