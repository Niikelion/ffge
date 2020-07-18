cmake_minimum_required(VERSION 3.9.2)

project(ffge VERSION 2.0.0 DESCRIPTION "Graphics engine")

find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
find_package(freetype REQUIRED)

set(SOURCES
	src/basics.cpp
	src/buffer.cpp
	src/camera.cpp
	src/font.cpp
	src/frame.cpp
	src/material.cpp
	src/mesh.cpp
	src/obj.cpp
	src/object.cpp
	src/renderer.cpp
	src/scene.cpp
	src/shader.cpp
	src/sprite.cpp
	src/text.cpp
	src/texture.cpp
	src/uniform.cpp
)

set(HEADERS
	include/ffge/basics.hpp
	include/ffge/buffer.hpp
	include/ffge/camera.hpp
	include/ffge/ff_glew.hpp
	include/ffge/ffge.hpp
	include/ffge/font.hpp
	include/ffge/frame.hpp
	include/ffge/ftguard.hpp
	include/ffge/material.hpp
	include/ffge/mesh.hpp
	include/ffge/obj.hpp
	include/ffge/object.hpp
	include/ffge/renderer.hpp
	include/ffge/scene.hpp
	include/ffge/shader.hpp
	include/ffge/sprite.hpp
	include/ffge/text.hpp
	include/ffge/texture.hpp
	include/ffge/uniform.hpp
)

add_library(ffge ${SOURCES} ${HEADERS})
target_include_directories(ffge PUBLIC ./include)
target_include_directories(ffge PRIVATE include/ffge)
target_link_libraries(ffge PUBLIC FreeImage glm GLEW freetype)