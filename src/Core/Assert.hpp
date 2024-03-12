//
// Created by Kulikov_AU on 12.03.2024.
//

#pragma once

#include <Logging/Log.hpp>
#include <filesystem>

#ifdef ENABLE_ASSERTS
    #define MR_INTERNAL_ASSERT_IMPL(type, check, msg, ...) { if(!(check)) { MR##type##ERROR(msg, __VA_ARGS__); MR_DEBUGBREAK(); } }
	#define MR_INTERNAL_ASSERT_WITH_MSG(type, check, ...) MR_INTERNAL_ASSERT_IMPL(type, check, "Assertion failed: {0}", __VA_ARGS__)
	#define MR_INTERNAL_ASSERT_NO_MSG(type, check) MR_INTERNAL_ASSERT_IMPL(type, check, "Assertion '{0}' failed at {1}:{2}", MR_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

	#define MR_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
	#define MR_INTERNAL_ASSERT_GET_MACRO(...) MR_EXPAND_MACRO( MR_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, MR_INTERNAL_ASSERT_WITH_MSG, MR_INTERNAL_ASSERT_NO_MSG) )

	// Currently accepts at least the condition and one additional parameter (the message) being optional
	#define MR_ASSERT(...) MR_EXPAND_MACRO( MR_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_, __VA_ARGS__) )
	#define MR_CORE_ASSERT(...) MR_EXPAND_MACRO( MR_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(_CORE_, __VA_ARGS__) )
#else
    #define MR_ASSERT(...)
    #define MR_CORE_ASSERT(...)
#endif