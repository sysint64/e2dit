cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(SOURCES base_object.cpp camera.cpp data_render.cpp material.cpp shader.cpp texture.cpp)
add_library(libe2dit-renderer ${SOURCES})
target_link_libraries(libe2dit-renderer libe2dit-utility)
