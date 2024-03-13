//
// Created by Kulikov_AU on 12.03.2024.
//

#pragma once

#ifdef _WIN32
    #ifdef _WIN64
    /* Windows x64  */
    #define MR_PLATFORM_WINDOWS
    #else
    /* Windows x86 */
		#error "x86 Builds are not supported!"
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#define HZ_PLATFORM_IOS
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#define MR_PLATFORM_MACOS
	#else
		#error "Unknown Apple platform!"
	#endif
    #elif defined(__ANDROID__)
        #define MR_PLATFORM_ANDROID
	    #error "Android is not supported!"
    #elif defined(__linux__)
        #define MR_PLATFORM_LINUX
	    #error "Linux is not supported!"//TODO:make to support
    #else
        /* Unknown compiler/platform */
	    #error "Unknown platform!"
#endif