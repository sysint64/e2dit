cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(SOURCES base_object.cpp camera.cpp data_render.cpp material.cpp shader.cpp texture.cpp)
add_library(e2dit-renderer ${SOURCES})
target_link_libraries(e2dit-renderer e2dit-utility)
