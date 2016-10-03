cmake_minimum_required(VERSION 2.8)
project(e2dit)
set(PRECOMPUTE precompute/button.cpp
		precompute/checkbox.cpp
		precompute/grouped.cpp
		precompute/treelist.cpp
		precompute/edit.cpp
		precompute/element.cpp
		precompute/label.cpp
		precompute/listmenu.cpp
		precompute/menuitems.cpp
		precompute/dropmenu.cpp
		precompute/panel.cpp
		precompute/toolbar.cpp
		precompute/dialog.cpp
		precompute/tooltip.cpp)

set(DIALOGS dialogs/color_dialog.cpp ../../include/ui/dialogs/color_dialog.h
		dialogs/color_dialog_view.cpp)

set(SOURCES ${PRECOMPUTE} ${DIALOGS} button.cpp ../../include/ui/button.h
		checkbox.cpp ../../include/ui/checkbox.h
		grouped.cpp ../../include/ui/grouped.h
		treelist.cpp ../../include/ui/treelist.h
		edit.cpp ../../include/ui/edit.h
		widget.cpp ../../include/ui/widget.h
		image.cpp ../../include/ui/image.h
		label.cpp ../../include/ui/label.h
		manager.cpp ../../include/ui/manager.h
		listmenu.cpp ../../include/ui/listmenu.h
		menuitems.cpp ../../include/ui/menuitems.h
		dropmenu.cpp ../../include/ui/dropmenu.h
		panel.cpp ../../include/ui/panel.h
		tabs.cpp ../../include/ui/tabs.h
		theme.cpp ../../include/ui/theme.h
		toolbar.cpp ../../include/ui/toolbar.h
		dialog.cpp ../../include/ui/dialog.h
		loader.cpp ../../include/ui/loader.h
		stack_layout.cpp ../../include/ui/stack_layout.h
		tooltip.cpp ../../include/ui/tooltip.h
		colorpanel.cpp ../../include/ui/colorpanel.h
		../../include/ui/cursor.h)

if (APPLE)
	set(SOURCES ${SOURCES} cursor.mm)
else()
	set(SOURCES ${SOURCES} cursor.cpp)
endif()

add_library(e2dit-ui ${SOURCES})
target_link_libraries(e2dit-ui e2dit-utility e2dit-renderer)
