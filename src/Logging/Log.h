//
// Created by tosha on 11.03.2024.
//

#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace multi_render_application {
    class Log
    {
        public:
            static void Init();

            static Ref<spdlog::logger>& GetCoreLogger() { return s_core_logger; }
            static Ref<spdlog::logger>& GetClientLogger() { return s_client_logger; }
        private:
            static Ref<spdlog::logger> s_core_logger;
            static Ref<spdlog::logger> s_client_logger;
    };
}  // namespace multi_render_application


template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
    return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
    return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
    return os << glm::to_string(quaternion);
}

// Core log macros
#define LOG_CORE_TRACE(...)    ::multi_render_application::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_CORE_INFO(...)     ::multi_render_application::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_CORE_WARN(...)     ::multi_render_application::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_CORE_ERROR(...)    ::multi_render_application::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_CORE_CRITICAL(...) ::multi_render_application::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define LOG_TRACE(...)         ::multi_render_application::Log::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...)          ::multi_render_application::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...)          ::multi_render_application::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)         ::multi_render_application::Log::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)      ::multi_render_application::Log::GetClientLogger()->critical(__VA_ARGS__)
