//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

namespace gl_render_application{

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

}
