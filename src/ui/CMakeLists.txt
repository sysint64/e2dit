cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(PRECOMPUTE precompute/button.cpp precompute/edit.cpp precompute/element.cpp precompute/image.cpp precompute/label.cpp precompute/menus.cpp precompute/panel.cpp precompute/tabs.cpp)
set(SOURCES ${PRECOMPUTE} button.cpp cursor.cpp edit.cpp element.cpp image.cpp label.cpp manager.cpp menus.cpp panel.cpp tabs.cpp theme.cpp)
add_library(libe2dit-ui ${SOURCES})
target_link_libraries(libe2dit-ui libe2dit-utility libe2dit-renderer)
