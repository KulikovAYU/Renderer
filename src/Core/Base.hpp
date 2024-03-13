//
// Created by Anton Kulikov on 03.03.2024.
//

#pragma once
#include <memory>
#include <PlatformDetection.hpp>

#ifdef DEBUG
#if defined(MR_PLATFORM_WINDOWS)
		    #define MR_DEBUGBREAK() __debugbreak()
	    #elif defined(MR_PLATFORM_LINUX)
		    #include <signal.h>
		    #define MR_DEBUGBREAK() raise(SIGTRAP)
        #elif defined(MR_PLATFORM_MACOS)
            #include <signal.h>
		    #define MR_DEBUGBREAK() raise(SIGTRAP)
	    #else
		    #error "Platform doesn't support debugbreak yet!"
	    #endif
	    #define MR_ENABLE_ASSERTS
#else
#define MR_DEBUGBREAK()
#endif

#define MR_EXPAND_MACRO(x) x
#define MR_STRINGIFY_MACRO(x) #x

namespace multi_render_application {

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
    [[nodiscard]] constexpr Scope<T> CreateScope(Args&& ... args){
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    [[nodiscard]] constexpr Ref<T> CreateRef(Args&& ... args){
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
}

#include "Logging/Log.hpp"
#include "Assert.hpp"