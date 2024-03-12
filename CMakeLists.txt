cmake_minimum_required(VERSION 3.14)

project(Renderer)

set(CMAKE_CXX_STANDARD 20)

set(CORE_DIR ${CMAKE_SOURCE_DIR}/src/core)
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

#set(GLM_DIR ${THIRDPARTY_DIR}/glm)
#add_subdirectory(${GLM_DIR} ${CMAKE_BINARY_DIR}/glm)

set(SPIRVCROSS_DIR ${THIRDPARTY_DIR}/SPIRV-Cross)
add_subdirectory(${SPIRVCROSS_DIR} ${CMAKE_BINARY_DIR}/SPIRV-Cross)

set(SPDLOG_DIR ${THIRDPARTY_DIR}/spdlog)
add_subdirectory(${SPDLOG_DIR} ${CMAKE_BINARY_DIR}/spdlog)

set(IMGUI_DIR  ${THIRDPARTY_DIR}/imgui)
set(IMGUI_INCLUDES
        ${IMGUI_DIR}/imgui.h
        ${IMGUI_DIR}/backends/imgui_impl_glfw.h
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.h
)
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
)
add_library(ImGui STATIC
        ${IMGUI_INCLUDES}
        ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${THIRDPARTY_DIR})
target_include_directories(ImGui PUBLIC ${THIRDPARTY_DIR}/imgui)
target_link_libraries(ImGui PRIVATE Glad glfw)

add_subdirectory(src/Platform/Opengl)
target_include_directories(OpenGlPlatformLibrary PRIVATE ${GLAD_INCLUDE_DIR})
#target_include_directories(OpenGlPlatformLibrary PRIVATE ${GLM_DIR})

set(HEADERS_DIR
        src/Core/Sample/core.hpp
        src/Core/PlatformDetection.hpp
        src/Logging/Log.hpp
        src/Core/Assert.hpp
        #src/Core/Buffers.hpp
        #src/Core/renderer/Buffer.hpp
        #src/Core/renderer/VertexArray.hpp
        #src/Core/renderer/Shader.hpp
        #src/Core/renderer/ShaderData.hpp
        #src/Core/renderer/RendererApi.hpp
        #src/Core/Log.cpp
        #src/Core/Log.h
)

set(SOURCES_DIR
        src/main.cpp
        src/Core/Sample/sample_shader.cpp
        src/Platform/PlatformProvider/PlatformShader.cpp
        src/Platform/PlatformProvider/PlatformRendererApi.cpp
        src/Logging/Log.cpp
)

add_executable(${PROJECT_NAME} ${HEADERS_DIR} ${SOURCES_DIR})

#target_include_directories(MyOpenGLExecutable PRIVATE ${CORE_DIR})

#find_package(OpenGL REQUIRED)
#if(OPENGL_FOUND)
#    include_directories(${OPENGL_INCLUDE_DIRS})
#    target_link_libraries(MyOpenGLExecutable PRIVATE ${OPENGL_LIBRARIES})
#endif()

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
endif()

if(COCOA_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${COCOA_LIBRARY})
endif()

if(IOKIT_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${IOKIT_LIBRARY})
endif()


target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${GLAD_INCLUDE_DIR}
        #${GLM_DIR}
        ${PROJECT_SOURCE_DIR}/src
        #${CORE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        spirv-cross-core
        spirv-cross-glsl
        spdlog
        ImGui
        Glad
        glfw
        OpenGlPlatformLibrary
)

#target_compile_options(${PROJECT_NAME} PRIVATE -include ${CORE_DIR}/Basepch.hpp)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CORE_DIR}/Basepch.hpp)

include(cmake/Sanitize.cmake)