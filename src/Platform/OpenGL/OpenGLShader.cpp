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

        static void CreateChashDirectoryIfNotExist() {
            std::string cache_directory = GetCacheDirectory();
            if (!std::filesystem::exists(cache_directory)) {
                std::filesystem::create_directories(cache_directory);
            }
        }

    }  // namespace utils

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_file_path{filepath} {
        // TODO: do smth !
        utils::CreateChashDirectoryIfNotExist();

        std::string source = ReadFile(filepath);
    }

    OpenGLShader::OpenGLShader(const std::string& name,
                               const std::string& vertex_src,
                               const std::string& fragment_src)
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
               MR_LOG_CORE_ERROR("Could not read from file '{0}'", filepath);
            }
        } else {
           // MR_LOG_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

}  // namespace multi_render_application