cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
set(CMAKE_POLICY_DEFAULT_CMP0148 OLD)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

set(NAME Renderer)

project(${NAME})

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)

set(CORE_DIR ${CMAKE_SOURCE_DIR}/src/core)
include_directories(${CORE_DIR})

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

set(SPDLOG_DIR ${THIRDPARTY_DIR}/spdlog)
add_subdirectory(${SPDLOG_DIR} ${CMAKE_BINARY_DIR}/spdlog)
include_directories(${THIRDPARTY_DIR}/spdlog/include)

set(GLM_DIR ${THIRDPARTY_DIR}/glm)
add_subdirectory(${GLM_DIR} ${CMAKE_BINARY_DIR}/glm)
include_directories(${GLM_DIR})

set(GLAD_DIR ${THIRDPARTY_DIR}/glad)
add_library(Glad ${GLAD_DIR}/src/glad.c)
target_include_directories(Glad PUBLIC ${GLAD_DIR}/include)

add_subdirectory(${THIRDPARTY_DIR}/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)

set(IMGUI_DIR  ${THIRDPARTY_DIR}/imgui)
set(IMGUI_INCLUDES
        ${IMGUI_DIR}/imgui.h
        ${IMGUI_DIR}/backends/imgui_impl_glfw.h
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.h)
set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp)
add_library(ImGui STATIC
        ${IMGUI_INCLUDES}
        ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${THIRDPARTY_DIR})
target_include_directories(ImGui PUBLIC ${THIRDPARTY_DIR}/imgui)
target_link_libraries(ImGui PRIVATE Glad glfw)

option(SPIRV_CROSS_ENABLE_TESTS OFF)
set(SPIRVCROSS_DIR ${THIRDPARTY_DIR}/SPIRV-Cross)
add_subdirectory(${SPIRVCROSS_DIR} ${CMAKE_BINARY_DIR}/SPIRV-Cross)

set(SPRIRV_TOOLS_DIR ${THIRDPARTY_DIR}/SPIRV-Tools)
add_subdirectory(${SPRIRV_TOOLS_DIR} ${CMAKE_BINARY_DIR}/SPIRV-Tools)

set(GLSLLANG_DIR ${THIRDPARTY_DIR}/glslang)
add_subdirectory(${GLSLLANG_DIR} ${CMAKE_BINARY_DIR}/glslang)

set(SHADERC_SKIP_TESTS TRUE)
set(SHADERC_DIR ${THIRDPARTY_DIR}/shaderc)
add_subdirectory(${SHADERC_DIR} ${CMAKE_BINARY_DIR}/shaderc)


add_subdirectory(src/Platform/Opengl)

set(SOURCES_DIR
        src/main.cpp
        src/Platform/PlatformProvider/PlatformShader.cpp
        src/Platform/PlatformProvider/PlatformRendererApi.cpp
        src/Logging/Log.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES_DIR})

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
        PRIVATE
        ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(OpenGlPlatformLibrary PRIVATE
        spirv-cross-core
        spirv-cross-glsl
        Glad
        shaderc_shared
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        ImGui
        Glad
        glfw
        OpenGlPlatformLibrary
)

set(INCLUDES_DIR ${CMAKE_SOURCE_DIR}/includes)
# Получите список всех заголовочных файлов в директории Core
file(GLOB_RECURSE CORE_HEADERS ${CORE_DIR}/*.h)
# Команда для копирования заголовочных файлов
foreach(HEADER ${CORE_HEADERS})
    # Вычислите путь назначения
    file(RELATIVE_PATH REL_HEADER ${CMAKE_SOURCE_DIR}/Core ${HEADER})
    set(DEST_HEADER ${INCLUDES_DIR}/${REL_HEADER})

    # Команда копирования
    add_custom_command(
            OUTPUT ${DEST_HEADER}
            COMMAND ${CMAKE_COMMAND} -E copy ${HEADER} ${DEST_HEADER}
            DEPENDS ${HEADER}
            COMMENT "Copying ${HEADER} to ${DEST_HEADER}"
    )

    # Группируйте все операции копирования в одну цель
    list(APPEND COPIED_HEADERS ${DEST_HEADER})
endforeach()

add_custom_target(CopyCoreHeaders ALL DEPENDS ${COPIED_HEADERS})

#target_compile_options(${PROJECT_NAME} PRIVATE -include ${CORE_DIR}/Basepch.hpp)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src/Basepch.hpp)

include(cmake/Sanitize.cmake)