cmake_minimum_required(VERSION 3.27)
project(untitled2)

set(CMAKE_CXX_STANDARD 20)

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

message("Creating GLAD library")
#Установка пути к каталогу, где лежит glad
set(GLAD_DIR ${THIRDPARTY_DIR}/glad)
#добавляем glad
set(GLAD_INCLUDE_DIR ${GLAD_DIR}/include)
# Создание библиотеки Glad
add_library(Glad ${GLAD_DIR}/src/glad.c)


message("Creating GLFW library")

# Установка пути к каталогу, где лежит GLFW
add_subdirectory(${THIRDPARTY_DIR}/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)

# Настройка пути к заголовочным файлам и библиотекам GLFW
set(GLFW_INCLUDE_DIR ${GLFW_DIR}/include)

message("Building GLM")
#Установка пути к каталогу, где лежит glm
set(GLM_DIR ${THIRDPARTY_DIR}/glm)
add_subdirectory(${GLM_DIR})

# Указание директив для поиска заголовочных файлов
include_directories(
        ${GLFW_INCLUDE_DIR}
        ${GLAD_INCLUDE_DIR}
        ${GLM_DIR}
        ${PROJECT_SOURCE_DIR}/src )

# Установка переменной для хранения списка исходных файлов вашего проекта
set(SOURCES src/main.cpp
        src/core/Sample/core.hpp
        src/core/base.hpp
        src/core/buffers.hpp
        src/platforms/OpenGl/opengl_buffers.hpp
        src/core/renderer/buffer.hpp
        src/platforms/OpenGl/opengl_buffers_impl.cpp
        src/core/renderer/vertex_array.hpp
        src/platforms/OpenGl/opengl_vertex_array.hpp
        src/platforms/OpenGl/opengl_entity_render_object.hpp
        src/core/renderer/renderer_api.cpp
        src/core/renderer/renderer_api.hpp
        src/platforms/OpenGl/opengl_render_api.cpp
        src/platforms/OpenGl/opengl_render_api.hpp
        src/core/renderer/shader_data.hpp
)

# Создание исполняемого файла из исходных файлов
add_executable(MyOpenGLExecutable ${SOURCES})

# Линковка с библиотекой Glad+glfw
#target_link_libraries(MyOpenGLExecutable PRIVATE Glad glfw)

# Поиск и подключение пакета OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
    target_link_libraries(MyOpenGLExecutable PRIVATE ${OPENGL_LIBRARIES})
endif()

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

target_link_libraries(MyOpenGLExecutable PRIVATE ImGui Glad glfw)