//
// Created by Kulikov_AU on 05.03.2024.
//

#include <fstream>
#include <filesystem>
#include "OpenGLShader.hpp"
#include "Timer.h"
#include <shaderc/shaderc.hpp>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

namespace multi_render_application {

    namespace utils {

        static GLenum ShaderTypeFromString(const std::string& type)
        {
            if (type == "vertex")
                return GL_VERTEX_SHADER;
            if (type == "fragment" || type == "pixel")
                return GL_FRAGMENT_SHADER;

            MR_CORE_ASSERT(false, "Unknown shader type!");
            return 0;
        }

        static const char* GLShaderStageToString(GLenum stage) {
            switch (stage)
            {
                case GL_VERTEX_SHADER:   return "GL_VERTEX_SHADER";
                case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
            }
            MR_CORE_ASSERT(false);
            return nullptr;
        }

        static const char* GetCacheDirectory() {
            // TODO: make sure the assets directory is valid
            return "assets/cache/shader/opengl";
        }

        static void CreateCacheDirectoryIfNeeded() {
            std::string cache_directory = GetCacheDirectory();
            if (!std::filesystem::exists(cache_directory)) {
                std::filesystem::create_directories(cache_directory);
            }
        }

        static const char* GLShaderStageCachedOpenGLFileExtension(uint32_t stage) {
            switch (stage)
            {
                case GL_VERTEX_SHADER:    return ".cached_opengl.vert";
                case GL_FRAGMENT_SHADER:  return ".cached_opengl.frag";
            }
            MR_CORE_ASSERT(false);
            return "";
        }

        static const char* GLShaderStageCachedVulkanFileExtension(uint32_t stage) {
            switch (stage)
            {
                case GL_VERTEX_SHADER:    return ".cached_vulkan.vert";
                case GL_FRAGMENT_SHADER:  return ".cached_vulkan.frag";
            }
            MR_CORE_ASSERT(false);
            return "";
        }

        static shaderc_shader_kind GLShaderStageToShaderC(GLenum stage) {
            switch (stage)
            {
                case GL_VERTEX_SHADER:   return shaderc_glsl_vertex_shader;
                case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
            }
            MR_CORE_ASSERT(false);
            return (shaderc_shader_kind)0;
        }

    }  // namespace utils

    OpenGLShader::OpenGLShader(const std::string& filepath)
        : m_file_path{filepath} {

        utils::CreateCacheDirectoryIfNeeded();

        std::string source = ReadFile(filepath);
        auto shader_sources = PreProcess(source);

        {
            Timer timer;
            CompileOrGetVulkanBinaries(shader_sources);
            CompileOrGetOpenGLBinaries();
            CreateProgram();
            MR_LOG_CORE_WARN("Shader creation took {0} ms", timer.ElapsedMillis());
        }

        // Extract name from filepath
        auto last_slash = filepath.find_last_of("/\\");
        last_slash = last_slash == std::string::npos ? 0 : last_slash + 1;
        auto last_dot = filepath.rfind('.');
        auto count = last_dot == std::string::npos ? filepath.size() - last_slash : last_dot - last_slash;
        m_name = filepath.substr(last_slash, count);
    }

    OpenGLShader::OpenGLShader(const std::string& name,
                               const std::string& vertex_src,
                               const std::string& fragment_src)
        : m_name{name} {

        std::unordered_map<GLenum, std::string> sources;
        sources[GL_VERTEX_SHADER] = vertex_src;
        sources[GL_FRAGMENT_SHADER] = fragment_src;

        CompileOrGetVulkanBinaries(sources);
        CompileOrGetOpenGLBinaries();
        CreateProgram();
    }

    void OpenGLShader::Bind() const {
        glUseProgram(m_renderId);
    }

    void OpenGLShader::Unbind() const {
        glUseProgram(0);
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
            MR_LOG_CORE_ERROR("Could not open file '{0}'", filepath);
        }

        return result;
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(
        const std::string& source) {

        std::unordered_map<GLenum, std::string> shader_sources;

        const char* type_token = "#type";
        size_t type_token_length = strlen(type_token);
        size_t pos = source.find(type_token, 0); //Start of shader type declaration line
        while (pos != std::string::npos)
        {
            size_t eol = source.find_first_of("\r\n", pos); //End of shader type declaration line
            MR_CORE_ASSERT(eol != std::string::npos, "Syntax error");
            size_t begin = pos + type_token_length + 1; //Start of shader type name (after "#type " keyword)
            std::string type = source.substr(begin, eol - begin);
            MR_CORE_ASSERT(utils::ShaderTypeFromString(type), "Invalid shader type specified");

            size_t next_line_pos = source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
            MR_CORE_ASSERT(next_line_pos != std::string::npos, "Syntax error");
            pos = source.find(type_token, next_line_pos); //Start of next shader type declaration line

            shader_sources[utils::ShaderTypeFromString(type)] = (pos == std::string::npos) ? source.substr(next_line_pos) : source.substr(next_line_pos, pos - next_line_pos);
        }

        return shader_sources;
    }

    void OpenGLShader::CompileOrGetVulkanBinaries(
        const std::unordered_map<GLenum, std::string>& shader_sources) {

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_2);
        options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cache_directory = utils::GetCacheDirectory();

        auto& shader_data = m_vulkan_spirv;
        shader_data.clear();
        for (auto&& [stage, source] : shader_sources)
        {
            std::filesystem::path shader_file_path = m_file_path;
            std::filesystem::path cached_path = cache_directory / (shader_file_path.filename().string() + utils::GLShaderStageCachedVulkanFileExtension(stage));

            std::ifstream in(cached_path, std::ios::in | std::ios::binary);
            if (in.is_open())
            {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                auto& data = shader_data[stage];
                data.resize(size / sizeof(uint32_t));
                in.read((char*)data.data(), size);
            }
            else
            {
                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, utils::GLShaderStageToShaderC(stage), m_file_path.c_str(), options);
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    MR_LOG_CORE_ERROR(module.GetErrorMessage());
                    MR_CORE_ASSERT(false);
                }

                shader_data[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                std::ofstream out(cached_path, std::ios::out | std::ios::binary);
                if (out.is_open())
                {
                    auto& data = shader_data[stage];
                    out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }

        for (auto&& [stage, data]: shader_data) {
            Reflect(stage, data);
        }

    }

    void OpenGLShader::CompileOrGetOpenGLBinaries() {
        auto& shaderData = m_open_glspirv;

        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
        options.SetTargetEnvironment(shaderc_target_env_opengl, shaderc_env_version_opengl_4_5);
        const bool optimize = false;
        if (optimize)
            options.SetOptimizationLevel(shaderc_optimization_level_performance);

        std::filesystem::path cacheDirectory = utils::GetCacheDirectory();

        shaderData.clear();
        m_open_gl_source_code.clear();
        for (auto&& [stage, spirv] : m_vulkan_spirv)
        {
            std::filesystem::path shader_file_path = m_file_path;
            std::filesystem::path cached_path = cacheDirectory / (shader_file_path.filename().string() + utils::GLShaderStageCachedOpenGLFileExtension(stage));

            std::ifstream in(cached_path, std::ios::in | std::ios::binary);
            if (in.is_open())
            {
                in.seekg(0, std::ios::end);
                auto size = in.tellg();
                in.seekg(0, std::ios::beg);

                auto& data = shaderData[stage];
                data.resize(size / sizeof(uint32_t));
                in.read((char*)data.data(), size);
            }
            else
            {
                spirv_cross::CompilerGLSL glsl_compiler(spirv);
                m_open_gl_source_code[stage] = glsl_compiler.compile();
                auto& source = m_open_gl_source_code[stage];

                shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, utils::GLShaderStageToShaderC(stage), m_file_path.c_str());
                if (module.GetCompilationStatus() != shaderc_compilation_status_success)
                {
                    MR_LOG_CORE_ERROR(module.GetErrorMessage());
                    MR_CORE_ASSERT(false);
                }

                shaderData[stage] = std::vector<uint32_t>(module.cbegin(), module.cend());

                std::ofstream out(cached_path, std::ios::out | std::ios::binary);
                if (out.is_open())
                {
                    auto& data = shaderData[stage];
                    out.write((char*)data.data(), data.size() * sizeof(uint32_t));
                    out.flush();
                    out.close();
                }
            }
        }
    }

    void OpenGLShader::CreateProgram() {
        GLuint program = glCreateProgram();

        std::vector<GLuint> shader_i_ds;
        for (auto&& [stage, spirv] : m_open_glspirv)
        {
            GLuint shader_id = shader_i_ds.emplace_back(glCreateShader(stage));
            glShaderBinary(1, &shader_id, GL_SHADER_BINARY_FORMAT_SPIR_V, spirv.data(), spirv.size() * sizeof(uint32_t));
            glSpecializeShader(shader_id, "main", 0, nullptr, nullptr);
            glAttachShader(program, shader_id);
        }

        glLinkProgram(program);

        GLint is_linked;
        glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
        if (is_linked == GL_FALSE)
        {
            GLint max_length;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);

            std::vector<GLchar> info_log(max_length);
            glGetProgramInfoLog(program, max_length, &max_length, info_log.data());
            MR_LOG_CORE_ERROR("Shader linking failed ({0}):\n{1}", m_file_path, info_log.data());

            glDeleteProgram(program);

            for (auto id : shader_i_ds) {
                glDeleteShader(id);
            }
        }

        for (auto id : shader_i_ds)
        {
            glDetachShader(program, id);
            glDeleteShader(id);
        }

        m_renderId = program;
    }


    void OpenGLShader::Reflect(GLenum stage,
                               const std::vector<uint32_t>& shader_data) {
        spirv_cross::Compiler compiler(shader_data);
        spirv_cross::ShaderResources resources = compiler.get_shader_resources();

        MR_LOG_CORE_TRACE("OpenGLShader::Reflect - {0} {1}", utils::GLShaderStageToString(stage), m_file_path);
        MR_LOG_CORE_TRACE("    {0} uniform buffers", resources.uniform_buffers.size());
        MR_LOG_CORE_TRACE("    {0} resources", resources.sampled_images.size());

        MR_LOG_CORE_TRACE("Uniform buffers:");
        for (const auto& resource : resources.uniform_buffers)
        {
            const auto& buffer_type = compiler.get_type(resource.base_type_id);
            uint32_t buffer_size = compiler.get_declared_struct_size(buffer_type);
            uint32_t binding = compiler.get_decoration(resource.id, spv::DecorationBinding);
            int member_count = buffer_type.member_types.size();

            MR_LOG_CORE_TRACE("  {0}", resource.name);
            MR_LOG_CORE_TRACE("    Size = {0}", buffer_size);
            MR_LOG_CORE_TRACE("    Binding = {0}", binding);
            MR_LOG_CORE_TRACE("    Members = {0}", member_count);
        }
    }


}  // namespace multi_render_application