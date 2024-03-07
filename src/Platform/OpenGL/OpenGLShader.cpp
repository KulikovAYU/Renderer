//
// Created by Kulikov_AU on 05.03.2024.
//

#include "OpenGLShader.hpp"

namespace multi_render_application{


    OpenGLShader::OpenGLShader(const std::string &filepath) : m_file_path{filepath}{
        //TODO: do smth !
    }

    OpenGLShader::OpenGLShader(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc)
            : m_name{name} {
        //TODO: do smth !!!
    }
}