//
// Created by Kulikov_AU on 05.03.2024.
//

#include "../Core/Renderer/Shader.hpp"
#include "../Core/Renderer/RendererApi.hpp"
#include <Platform/OpenGL/OpenGLShader.hpp>

namespace multi_render_application{

    Ref <Shader> Shader::Create(const std::string &filepath){
        switch (RendererApi::GetAPI())
        {
            case RendererApi::API::None:    /*HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
            case RendererApi::API::OpenGL:  return CreateRef<OpenGLShader>(filepath);
        }
        return nullptr;
    }

    Ref <Shader> Shader::Create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc){
        switch (RendererApi::GetAPI())
        {
            case RendererApi::API::None:    /*HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");*/ return nullptr;
            case RendererApi::API::OpenGL:  return CreateRef<OpenGLShader>(name, vertexSrc, fragmentSrc);
        }

        return nullptr;
    }
}

