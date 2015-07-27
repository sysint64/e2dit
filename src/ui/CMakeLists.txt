cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(PRECOMPUTE precompute/button.cpp precompute/checkbox.cpp precompute/edit.cpp precompute/element.cpp precompute/image.cpp precompute/label.cpp precompute/listmenu.cpp precompute/dropmenu.cpp precompute/panel.cpp precompute/tabs.cpp)
set(SOURCES ${PRECOMPUTE} button.cpp checkbox.cpp edit.cpp element.cpp image.cpp label.cpp manager.cpp listmenu.cpp dropmenu.cpp panel.cpp tabs.cpp theme.cpp)

if (APPLE)
	set(SOURCES ${SOURCES} cursor.mm)
else()
	set(SOURCES ${SOURCES} cursor.cpp)
endif()

add_library(libe2dit-ui ${SOURCES})
target_link_libraries(libe2dit-ui libe2dit-utility libe2dit-renderer)
