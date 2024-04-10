cmake_minimum_required(VERSION 3.14)

set(OPENGL_PLATFORM_INCLUDES
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLBuffers.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLVertexArray.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLEntityRenderObject.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLRenderApi.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLShader.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLDebug.hpp
)

set(OPENGL_PLATFORM_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLBuffers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLTypeMapping.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLRenderApi.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/OpenGLShader.cpp
        OpenGLVertexArray.cpp
)

add_library(OpenGlPlatformLibrary STATIC
        ${OPENGL_PLATFORM_INCLUDES}
        ${OPENGL_PLATFORM_SOURCES})

set(GLM_DIR ${THIRDPARTY_DIR}/glm)
add_subdirectory(${GLM_DIR} ${CMAKE_BINARY_DIR}/glm)

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
set(GLAD_DIR ${THIRDPARTY_DIR}/glad)
add_library(Glad ${GLAD_DIR}/src/glad.c)

add_subdirectory(${THIRDPARTY_DIR}/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_include_directories(OpenGlPlatformLibrary PUBLIC ${THIRDPARTY_DIR}/glfw/include)

target_include_directories(Glad PRIVATE ${GLAD_DIR}/include)
target_include_directories(OpenGlPlatformLibrary PUBLIC ${GLAD_DIR}/include)
target_include_directories(OpenGlPlatformLibrary PUBLIC ${CMAKE_SOURCE_DIR}/src/core)
target_include_directories(OpenGlPlatformLibrary PUBLIC ${GLM_DIR})
target_link_libraries(OpenGlPlatformLibrary PRIVATE Glad)


add_definitions(-D_CRT_SECURE_NO_WARNINGS)
option(SPIRV_CROSS_ENABLE_TESTS OFF)
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)
set(SPIRVCROSS_DIR ${THIRDPARTY_DIR}/SPIRV-Cross)
add_subdirectory(${SPIRVCROSS_DIR} ${CMAKE_BINARY_DIR}/SPIRV-Cross)
target_link_libraries(OpenGlPlatformLibrary PRIVATE spirv-cross-core spirv-cross-glsl)
#target_include_directories(OpenGlPlatformLibrary PUBLIC ${SPIRVCROSS_DIR})

set(SPRIRV_TOOLS_DIR ${THIRDPARTY_DIR}/SPIRV-Tools)
add_subdirectory(${SPRIRV_TOOLS_DIR} ${CMAKE_BINARY_DIR}/SPIRV-Tools)

set(GLSLLANG_DIR ${THIRDPARTY_DIR}/glslang)
add_subdirectory(${GLSLLANG_DIR} ${CMAKE_BINARY_DIR}/glslang)

set(SHADERC_SKIP_TESTS TRUE)
set(SHADERC_DIR ${THIRDPARTY_DIR}/shaderc)
add_subdirectory(${SHADERC_DIR} ${CMAKE_BINARY_DIR}/shaderc)
target_link_libraries(OpenGlPlatformLibrary PRIVATE shaderc_shared)

#find vulkan sdk for cross shader building
find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
    message("Vulkan SDK found: ${Vulkan_INCLUDE_DIRS}")
    #target_include_directories(OpenGlPlatformLibrary PRIVATE ${Vulkan_INCLUDE_DIRS})

    get_filename_component(VULKAN_SDK_LIBS ${Vulkan_LIBRARIES} DIRECTORY)
    message("Vulkan libraries found: ${VULKAN_SDK_LIBS}")
#    target_link_libraries(OpenGlPlatformLibrary PRIVATE
#            #optimized ${VULKAN_SDK_LIBS}/shaderc_sharedd.lib
#             ${VULKAN_SDK_LIBS}/shaderc_sharedd.lib
#       #     ${VULKAN_SDK_LIBS}/spirv-cross-cored.lib
#      #       ${VULKAN_SDK_LIBS}/spirv-cross-glsld.lib
##             ${VULKAN_SDK_LIBS}/SPIRV-Toolsd.lib
#    )



#    find_library(SHADERC_SHARED_LIBRARY shaderc_shared HINTS ${VULKAN_SDK_LIBS})
#    find_library(SPIRV_CROSS_CORE_LIBRARY spirv-cross-core HINTS ${VULKAN_SDK_LIBS})
#    find_library(SPIRV_CROSS_GLSL_LIBRARY spirv-cross-glsl HINTS ${VULKAN_SDK_LIBS})
#    target_link_libraries(OpenGlPlatformLibrary PRIVATE
#            ${SHADERC_SHARED_LIBRARY}
#            ${SPIRV_CROSS_CORE_LIBRARY}
#            ${SPIRV_CROSS_GLSL_LIBRARY}
#    )
else ()
    message(FATAL_ERROR "Vulkan SDK not found.")
endif ()



target_include_directories(OpenGlPlatformLibrary PRIVATE ${THIRDPARTY_DIR}/spdlog/include)


#включаем неявно base.hpp во все translation units
#target_compile_options(OpenGlPlatformLibrary PRIVATE -include ${CMAKE_SOURCE_DIR}/src/Core/base.hpp)
target_precompile_headers(OpenGlPlatformLibrary PRIVATE ${CMAKE_SOURCE_DIR}/src/Basepch.hpp)