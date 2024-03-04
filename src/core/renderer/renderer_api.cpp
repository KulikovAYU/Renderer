//
// Created by Anton Kulikov on 03.03.2024.
//

#include "renderer_api.hpp"
#include "platforms/OpenGl/opengl_render_api.hpp"

namespace gl_render_application{

    renderer_api::API renderer_api::s_api = renderer_api::API::OpenGL;

    Scope<renderer_api> renderer_api::Create() {
        switch (s_api)
        {
            case renderer_api::API::None:   return nullptr;
            case renderer_api::API::OpenGL: return CreateScope<opengl_render_api>();
        }

        return nullptr;
    }
}

