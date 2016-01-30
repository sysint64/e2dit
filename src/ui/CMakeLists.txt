cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(PRECOMPUTE precompute/button.cpp precompute/checkbox.cpp precompute/grouped.cpp precompute/treelist.cpp precompute/edit.cpp precompute/element.cpp precompute/label.cpp precompute/listmenu.cpp precompute/menuitems.cpp precompute/dropmenu.cpp precompute/panel.cpp precompute/toolbar.cpp precompute/dialog.cpp)
set(SOURCES ${PRECOMPUTE} button.cpp checkbox.cpp grouped.cpp treelist.cpp edit.cpp element.cpp image.cpp label.cpp manager.cpp listmenu.cpp menuitems.cpp dropmenu.cpp panel.cpp tabs.cpp theme.cpp toolbar.cpp dialog.cpp loader.cpp)

if (APPLE)
	set(SOURCES ${SOURCES} cursor.mm)
else()
	set(SOURCES ${SOURCES} cursor.cpp)
endif()

add_library(e2dit-ui ${SOURCES})
target_link_libraries(e2dit-ui e2dit-utility e2dit-renderer)
