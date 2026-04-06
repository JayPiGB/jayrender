cmake_minimum_required(VERSION 3.20)

project(jayrender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/glfw)

add_library(glad STATIC external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external/glm)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE external/stb_image)

set(IMGUI_SRC external/imgui/imgui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui.cpp
    	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_demo.cpp
    	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_tables.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/imgui_widgets.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_glfw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/external/imgui/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
	external/imgui 
	external/imgui/backends
)
target_link_libraries(imgui PUBLIC
	glfw
	glad
	OpenGL::GL)

find_package(OpenGL REQUIRED)

add_executable(${CMAKE_PROJECT_NAME}
	src/main.cpp
	src/shader/shader.cpp
	src/resource_manager/resource_manager.cpp
	src/texture/texture.cpp
	src/data/material/material.cpp
	src/data/light/light.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include)

target_link_libraries(${CMAKE_PROJECT_NAME}
	PRIVATE
	glfw
	glad
	glm
	stb_image
	OpenGL::GL
	imgui
)

#copying resources to executable working directory
#TODO: fix for release/debug builds
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}/Debug)
