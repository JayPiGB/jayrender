cmake_minimum_required(VERSION 3.20)

project(jayrender)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(external/glfw)

add_library(glad external/glad/src/glad.c)
target_include_directories(glad PUBLIC external/glad/include)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE external/glm)

add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE external/stb_image)

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
)

#copying resources to executable working directory
#TODO: fix for release/debug builds
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}/Debug)
