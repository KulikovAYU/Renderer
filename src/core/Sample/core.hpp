//
// Created by Anton Kulikov on 28.02.2024.
//

#pragma once

#include <vector>
#include <utility>
#include "glad/glad.h"
#include "platforms/OpenGl/opengl_debug.hpp"
#include "platforms/OpenGl/opengl_type_mapping.hpp"
#include "core/base.hpp"

namespace gl_render_application{

    class vertex_array_object;

    template<typename TEntity>
    struct opengl_entity_render_object{
    public:
        [[nodiscard]] TEntity get_render_id() const noexcept { return m_renderId; }
        virtual ~opengl_entity_render_object(){
            if (m_renderId != 0) {
                glDeleteBuffers(1, &m_renderId);
            }
        }

    protected:
        TEntity m_renderId = 0;
    };

    struct base_buffer_object : opengl_entity_render_object<GLuint> {
    public:
        virtual void init(GLenum usage) = 0;
    };


    template<typename TDataType>
    class base_typed_buffer_object : public base_buffer_object{
    public:
        explicit base_typed_buffer_object(std::vector<TDataType> data) noexcept : m_data{std::move(data)}{

        }

        [[nodiscard]] size_t get_size() const noexcept{
            return m_data.size() * sizeof(TDataType);
        }

        [[nodiscard]] const TDataType *const get_data() const noexcept{
            return m_data.data();
        }

        [[nodiscard]] constexpr const TDataType GetType() const{
            return gl_type_map<TDataType>::annotation;
        }

        virtual void bind() = 0;
        virtual void unbind() = 0;

    protected:
        base_typed_buffer_object()=default;

        std::vector<TDataType> m_data;
    };

    template<class T>
    class vertex_buffer_object final :
            public base_typed_buffer_object<T>,
            private noncopyable {

    public:

        explicit vertex_buffer_object(std::vector<T> data) noexcept
            : base_typed_buffer_object<T>(data){

        }

        vertex_buffer_object(vertex_buffer_object&& rhs)  noexcept
            : base_typed_buffer_object<float>(std::move(rhs.m_data)){
            base_typed_buffer_object<T>::m_renderId = rhs.m_renderId;
            rhs.m_renderId = 0;
        }

        vertex_buffer_object& operator = (vertex_buffer_object&& rhs) noexcept{
            if (this == &rhs)
                return *this;

            base_typed_buffer_object<T>::m_data = std::move(rhs.m_data);
            base_typed_buffer_object<T>::m_renderId = rhs.m_renderId;
            rhs.m_renderId = 0;

            return *this;
        }

        void bind() override{
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, base_typed_buffer_object<T>::m_renderId));
        }

        void unbind() override{
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

    protected:
         void init(GLenum usage) override {
            GLCall(glGenBuffers(1, &(base_typed_buffer_object<T>::m_renderId)));
            //Буфер вершин делается текущим активным буфером.
             bind();
            //Данные вершин загружаются в буфер вершин. GL_STATIC_DRAW указывает, что данные не будут изменяться в процессе рендеринга.
            GLCall(glBufferData(GL_ARRAY_BUFFER, base_typed_buffer_object<T>::get_size(), base_typed_buffer_object<T>::get_data(), usage));
        }

    private:
        friend class vertex_array_object;
    };

    template<class T>
    class element_buffer_object final :
            public base_typed_buffer_object<T>,
            private noncopyable{

    public:
        explicit element_buffer_object(std::vector<T> data) noexcept
            : base_typed_buffer_object<unsigned int>(data){

        }

        element_buffer_object(element_buffer_object&& rhs) noexcept
            : base_typed_buffer_object<unsigned int>(std::move(rhs.m_data)){
            element_buffer_object<T>::m_renderId = rhs.m_renderId;
            rhs.m_renderId = 0;
        }

        element_buffer_object& operator = (element_buffer_object&& rhs) noexcept{
            if (this == &rhs)
                return *this;

            element_buffer_object<T>::m_renderId = rhs.m_renderId;
            rhs.m_renderId = 0;

            return *this;
        }

        void bind() override{
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object<T>::m_renderId));
        }

        void unbind() override{
            GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        }

    protected:
        void init(GLenum usage) override {
            GLCall(glGenBuffers(1, &(element_buffer_object<T>::m_renderId)));
            //Привязывается буфер элементов.
            bind();
            //Загружаются данные элементов (индексов) в буфер элементов.
            GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object<T>::get_size(), element_buffer_object<T>::get_data(), usage));
        }

    private:
        void move_construct_from(element_buffer_object&& rhs){
            this->swap(rhs);
        }

        void swap(element_buffer_object& rhs) noexcept{
            using std::swap;
            swap(this->m_renderId, rhs.m_renderId);
            swap(this->m_data, rhs.m_data);
        }

        friend class vertex_array_object;
    };

    class vertex_array_object final :
            public base_buffer_object,
            private noncopyable{

        using base_buffer_object_ptr = Scope<vertex_buffer_object<float>>;
        using base_array_object_ptr = Scope<element_buffer_object<unsigned int>>;
    public:
        vertex_array_object(
                base_buffer_object_ptr vbo,
                base_array_object_ptr ebo) :
                m_vbo{std::move(vbo)},
                m_ebo{std::move(ebo)}
                {

                }

        void init(GLenum usage) override{
            GLCall(glGenVertexArrays(1, &m_renderId));
            GLCall(glBindVertexArray(m_renderId));

            m_vbo->init(usage);
            m_ebo->init(usage);

            /*
             * Устанавливается указатель на данные вершин в VBO.
             * Это означает, что данные вершин будут использоваться для атрибута с индексом 0 (первый аргумент),
             * имеющего 3 компоненты типа GL_FLOAT, смещение между каждой вершиной
             * равно 3 * sizeof(float), и начало данных - смещение 0.
             * stride = 3 * sizeof(float) (т.к. x, y, z)
             * [ VERTEX 1] [ VERTEX 2] [ VERTEX 3 ]
             *  [x][y][z]   [x][y][z]   [x][y][z]
             *  первый аргумент index используется в вертексном шейдере layout (location = 0)
            */
            //GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
            GLCall(glVertexAttribPointer(m_vaptr.VertexShaderLocation,
                                   m_vaptr.Size,
                                   m_vbo->GetType(),
                                   m_vaptr.Normalize,
                                   m_vaptr.StrideIndex * sizeof(m_vbo->GetType()) ,
                                   m_vaptr.Pointer));

            //включаем использование атрибута вершин с индексом 0.
            GLCall(glEnableVertexAttribArray(0));
            // Отвязывает буфер вершин. Это безопасно делать, так как glVertexAttribPointer зарегистрировал
            // буфер вершин как текущий активный для атрибута вершин.
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        }

        void SetVertexAttribPointer(GLuint vertexShaderLocation,
                              GLuint size,
                              GLuint strideIndex,
                              void* pointer = nullptr){
            m_vaptr.VertexShaderLocation = vertexShaderLocation;
            m_vaptr.Size = size;
            m_vaptr.StrideIndex = strideIndex;
            m_vaptr.Pointer = pointer;
        }

    private:
        struct VattrPtr{
            GLuint VertexShaderLocation;
            GLuint Size;
            GLuint StrideIndex;
            GLuint Normalize = GL_FALSE;
            void* Pointer = nullptr;
        } m_vaptr;

        base_buffer_object_ptr m_vbo;
        base_array_object_ptr m_ebo;
    };
}