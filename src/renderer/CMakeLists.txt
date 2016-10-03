cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(SOURCES base_object.cpp ../../include/renderer/base_object.h
		camera.cpp ../../include/renderer/camera.h
		data_render.cpp ../../include/renderer/data_render.h
		shader.cpp ../../include/renderer/shader.h
		texture.cpp ../../include/renderer/texture.h
		color.cpp ../../include/renderer/color.h
		../../include/renderer/sprite.h)

add_library(e2dit-renderer ${SOURCES})
target_link_libraries(e2dit-renderer e2dit-utility)
