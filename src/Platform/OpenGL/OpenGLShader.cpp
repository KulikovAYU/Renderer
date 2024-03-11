//
// Created by Kulikov_AU on 05.03.2024.
//

#include <fstream>
#include <filesystem>
#include "OpenGLShader.hpp"

namespace multi_render_application {

    namespace utils {

        static const char* GetCacheDirectory() {
            // TODO: make sure the assets directory is valid
            return "assets/cache/shader/opengl";
        }

        static void CreateChasheDirectoryIfNotExist() {
            std::string cacheDirectory = GetCacheDirectory();
            if (!std::filesystem::exists(cacheDirectory))
                std::filesystem::create_directories(cacheDirectory);
        }

    }  // namespace utils

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_file_path{filepath} {
        // TODO: do smth !
        utils::CreateChasheDirectoryIfNotExist();

        std::string source = ReadFile(filepath);
    }

    OpenGLShader::OpenGLShader(const std::string& name,
                               const std::string& vertexSrc,
                               const std::string& fragmentSrc)
        : m_name{name} {
        // TODO: do smth !!!
    }

    std::string OpenGLShader::ReadFile(const std::string& filepath) {
        std::string result;
        std::ifstream in(
            filepath,
            std::ios::in |
                std::ios::binary);  // ifstream closes itself due to RAII
        if (in) {
            in.seekg(0, std::ios::end);
            size_t size = in.tellg();
            if (size != -1) {
                result.resize(size);
                in.seekg(0, std::ios::beg);
                in.read(&result[0], size);
            } else {
                // HZ_CORE_ERROR("Could not read from file '{0}'", filepath);
            }
        } else {
            // HZ_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

}  // namespace multi_render_application