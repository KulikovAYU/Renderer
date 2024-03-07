//
// Created by Kulikov_AU on 05.03.2024.
//

#pragma once
#include <glm/glm.hpp>

namespace multi_render_application {

    struct Shader {

    public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;

        virtual void Unbind() const = 0;

        virtual void SetInt(const std::string &name, int value) = 0;

        virtual void SetIntArray(const std::string &name, int *values, uint32_t count) = 0;

        virtual void SetFloat(const std::string &name, float value) = 0;

        virtual void SetFloat2(const std::string &name, const glm::vec2 &value) = 0;

        virtual void SetFloat3(const std::string &name, const glm::vec3 &value) = 0;

        virtual void SetFloat4(const std::string &name, const glm::vec4 &value) = 0;

        virtual void SetMat4(const std::string &name, const glm::mat4 &value) = 0;

        [[nodiscard]] virtual std::string GetName() const = 0;

        static Ref <Shader> Create(const std::string &filepath);

        static Ref <Shader> Create(const std::string &name, const std::string &vertexSrc, const std::string &fragmentSrc);
    };
}