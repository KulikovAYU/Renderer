//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

#include <glad/glad.h>

namespace multi_render_application{

    template<typename TEntity>
    struct OpenGLEntityRenderObject{
    public:
        [[nodiscard]] TEntity GetRenderId() const noexcept { return m_renderId; }

        virtual ~OpenGLEntityRenderObject(){
            if (m_renderId != 0) {
                glDeleteBuffers(1, &m_renderId);
            }
        }

    protected:
        TEntity m_renderId = 0;
    };

    struct OpenglEntityRenderObjectId : public  OpenGLEntityRenderObject<GLuint>{};

}
