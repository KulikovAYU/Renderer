cmake_minimum_required(VERSION 3.27)

project(MyOpenGLExecutable)

set(CMAKE_CXX_STANDARD 20)

set(CORE_DIR ${PROJECT_SOURCE_DIR}/src/core)
set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

message("Building GLM")
#Установка пути к каталогу, где лежит glm
set(GLM_DIR ${THIRDPARTY_DIR}/glm)
add_subdirectory(${GLM_DIR})

set(HEADERS
        src/core/Sample/core.hpp
        src/core/Buffers.hpp
        src/core/renderer/Buffer.hpp
        src/core/renderer/VertexArray.hpp
        src/core/renderer/Shader.hpp
        src/core/renderer/ShaderData.hpp
        src/core/renderer/RendererApi.hpp
)

# Установка переменной для хранения списка исходных файлов вашего проекта
set(SOURCES
        src/main.cpp
        src/core/Sample/sample_shader.cpp
        src/Platform/PlatformProvider/PlatformShader.cpp
        src/Platform/PlatformProvider/PlatformRendererApi.cpp
)


# Создание исполняемого файла из исходных файлов
add_executable(MyOpenGLExecutable ${HEADERS} ${SOURCES})

# Поиск и подключение пакета OpenGL
#find_package(OpenGL REQUIRED)
#if(OPENGL_FOUND)
#    include_directories(${OPENGL_INCLUDE_DIRS})
#    target_link_libraries(MyOpenGLExecutable PRIVATE ${OPENGL_LIBRARIES})
#endif()

# Link against Cocoa framework
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    if(COCOA_LIBRARY)
        target_link_libraries(MyOpenGLExecutable PRIVATE ${COCOA_LIBRARY})
    endif()

    # Link against IOKit framework
    find_library(IOKIT_LIBRARY IOKit)
    if(IOKIT_LIBRARY)
        target_link_libraries(MyOpenGLExecutable PRIVATE ${IOKIT_LIBRARY})
    endif()
endif()

set(IMGUI_INCLUDES
        ${THIRDPARTY_DIR}/imgui/imgui.h
        ${THIRDPARTY_DIR}/imgui/backends/imgui_impl_glfw.h
        ${THIRDPARTY_DIR}/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
        ${THIRDPARTY_DIR}/imgui/imgui.cpp
        ${THIRDPARTY_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${THIRDPARTY_DIR}/imgui/backends/imgui_impl_opengl3.cpp
        ${THIRDPARTY_DIR}/imgui/imgui_demo.cpp
        ${THIRDPARTY_DIR}/imgui/imgui_draw.cpp
        ${THIRDPARTY_DIR}/imgui/imgui_tables.cpp
        ${THIRDPARTY_DIR}/imgui/imgui_widgets.cpp
)

add_library(ImGui STATIC
        ${IMGUI_INCLUDES}
        ${IMGUI_SOURCES})

target_include_directories(ImGui PUBLIC ${THIRDPARTY_DIR})
target_include_directories(ImGui PUBLIC ${THIRDPARTY_DIR}/imgui)
target_link_libraries(ImGui PRIVATE Glad glfw)

#включаем неявно base.hpp во все translation units
#target_compile_options(MyOpenGLExecutable PRIVATE -include ${CORE_DIR}/base.hpp)


# Указание директив для поиска заголовочных файлов
target_include_directories(
        MyOpenGLExecutable
        PUBLIC
        ${GLAD_INCLUDE_DIR}
        ${GLM_DIR}
        ${PROJECT_SOURCE_DIR}/src
        ${CORE_DIR}
)

add_subdirectory(src/Platform/Opengl)

target_include_directories(OpenGlPlatformLibrary PRIVATE ${GLAD_INCLUDE_DIR})
target_include_directories(OpenGlPlatformLibrary PRIVATE ${GLM_DIR})
target_include_directories(MyOpenGLExecutable PRIVATE ${GLM_DIR})

target_link_libraries(MyOpenGLExecutable PRIVATE ImGui Glad glfw OpenGlPlatformLibrary)
#target_compile_options(MyOpenGLExecutable PRIVATE -include ${CORE_DIR}/Basepch.hpp)
target_precompile_headers(MyOpenGLExecutable PRIVATE ${CMAKE_SOURCE_DIR}/src/Core/Basepch.hpp)


include(cmake/Sanitize.cmake)