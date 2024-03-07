//
// Created by Kulikov_AU on 05.03.2024.
//

#pragma once
#include "Renderer/Shader.hpp"
#include "OpenGLEntityRenderObject.hpp"
#include <unordered_map>
#include <vector>

namespace multi_render_application {
    class OpenGLShader final:
            public Shader,
            public OpenglEntityRenderObjectId {

    public:
        explicit OpenGLShader(const std::string& filepath);

        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);

        void Bind() const override{}

        void Unbind() const override{}

        void SetInt(const std::string &name, int value) override{}

        void SetIntArray(const std::string &name, int *values, uint32_t count) override{}

        void SetFloat(const std::string &name, float value) override{}

        void SetFloat2(const std::string &name, const glm::vec2 &value) override{}

        void SetFloat3(const std::string &name, const glm::vec3 &value) override{}

        void SetFloat4(const std::string &name, const glm::vec4 &value) override{}

        void SetMat4(const std::string &name, const glm::mat4 &value) override{}

        [[nodiscard]] std::string GetName() const override{return m_name;}

    private:
        std::string m_file_path;
        std::string m_name;

        std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
        std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

        std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;

    };
}
