//
// Created by Kulikov_AU on 05.03.2024.
//

#include "Renderer/RendererApi.hpp"
#include <Platform/OpenGL/OpenGLRenderApi.hpp>

namespace multi_render_application {
    RendererApi::API RendererApi::s_api = RendererApi::API::OpenGL;

    Scope<RendererApi> RendererApi::Create() {
        switch (s_api)
        {
            case RendererApi::API::None:   return nullptr;
            case RendererApi::API::OpenGL: return CreateScope<OpenGLRenderApi>();
            //Implement other api
        }

        return nullptr;
    }
}