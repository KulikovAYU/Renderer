//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once

namespace multi_render_application {

    enum class shader_data_type
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32_t shader_data_type_size(shader_data_type type)
    {
        switch (type)
        {
            case shader_data_type::Float:    return 4;
            case shader_data_type::Float2:   return 4 * 2;
            case shader_data_type::Float3:   return 4 * 3;
            case shader_data_type::Float4:   return 4 * 4;
            case shader_data_type::Mat3:     return 4 * 3 * 3;
            case shader_data_type::Mat4:     return 4 * 4 * 4;
            case shader_data_type::Int:      return 4;
            case shader_data_type::Int2:     return 4 * 2;
            case shader_data_type::Int3:     return 4 * 3;
            case shader_data_type::Int4:     return 4 * 4;
            case shader_data_type::Bool:     return 1;
            case shader_data_type::None:
                break;
        }

        return 0;
    }
}