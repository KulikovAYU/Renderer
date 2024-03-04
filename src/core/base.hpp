//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once
#include <memory>

namespace gl_render_application {

    namespace noncopyable_ { // protection from unintended ADL
        class noncopyable {
        public:
            constexpr noncopyable() = default;
            ~noncopyable() = default;
            noncopyable(const noncopyable &) = delete;
            noncopyable & operator=(const noncopyable &) = delete;
        };
    }
    using noncopyable = noncopyable_::noncopyable;

    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    [[nodiscard]] constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    [[nodiscard]] constexpr Ref<T> CreateRef(Args&& ... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}