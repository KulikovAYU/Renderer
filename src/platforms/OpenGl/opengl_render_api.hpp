//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include "core/renderer/renderer_api.hpp"

namespace gl_render_application {
    class opengl_render_api : public renderer_api {

    public:
        void init() override;
        void set_viewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
        void set_clear_color(const glm::vec4& color) override;
        void clear() override;

        void draw_indexed(const Ref<vertex_array>& vertexArray, uint32_t indexCount = 0) override;
        void draw_lines(const Ref<vertex_array>& vertexArray, uint32_t vertexCount) override;

        virtual void set_line_width(float width) override;
    };
}

