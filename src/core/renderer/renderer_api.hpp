//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include <cstdint>
#include "vertex_array.hpp"
#include "glm/vec4.hpp"

namespace gl_render_application{
    class renderer_api {
    public:
        enum class API
        {
            None = 0, OpenGL = 1
        };

    public:
        virtual ~renderer_api() = default;

        virtual void init() = 0;
        virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void set_clear_color(const glm::vec4& color) = 0;
        virtual void clear() = 0;

        virtual void draw_indexed(const Ref<vertex_array>& vertexArray, uint32_t indexCount = 0) = 0;
        virtual void draw_lines(const Ref<vertex_array>& vertexArray, uint32_t vertexCount) = 0;

        virtual void set_line_width(float width) = 0;

        [[nodiscard]] static API GetAPI() { return s_api; }
        [[nodiscard]] static Scope<renderer_api> Create();
    private:
        static API s_api;
    };
}



