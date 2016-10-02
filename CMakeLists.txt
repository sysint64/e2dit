cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#set(Boost_NO_SYSTEM_PATHS ON)
#set(SFML_NO_SYSTEM_PATHS ON)

if (APPLE)
	set(PLATFORM _osx_)
elseif(UNIX)
	set(PLATFORM _linux_)
else()
	set(PLATFORM _win_)
endif()

# Required Libs

find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SOIL REQUIRED)
find_package(JPEG REQUIRED)
find_package(GLM REQUIRED)

if (UNIX)
	find_package(X11 REQUIRED)
	set(DEPENDENCY_LIBS ${X11_LIBRARIES})
endif()

if (APPLE)
	find_library(CORE_FOUNDATION NAMES CoreFoundation)
	find_library(APPLICATION_SERVICES NAMES ApplicationServices)
	find_library(IO_KIT NAMES IOKit)
	find_library(CORE_SERVICES NAMES CoreServices)
	find_library(COCOA NAMES Cocoa)
	find_library(CARBON NAMES Carbon)

	set(DEPENDENCY_LIBS ${CORE_FOUNDATION} ${APPLICATION_SERVICES} ${IO_KIT} ${CORE_SERVICES} ${COCOA} ${CARBON})
endif()

#set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.3 COMPONENTS system window graphics REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

include_directories(${FTGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${SOIL_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${X11_INCLUDE_DIRS})

#set(GLEW_LIBRARIES "F:/mingw64/lib/libglew32.dll.a")

add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-D${PLATFORM})

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_LIBS ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY})
set(GL_LIBS ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
set(IMAGE_LIBS ${SOIL_LIBRARIES} ${JPEG_LIBRARIES})
set(FONT_LIBS ${FREETYPE_LIBRARIES} ${FTGL_LIBRARIES})
set(BOOST_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

message(STATUS "Find GLEW: ${GLEW_LIBRARIES}")

set(libs ${FONT_LIBS} ${IMAGE_LIBS} ${GL_LIBS} ${BOOST_LIBS} ${SFML_LIBS} ${DEPENDENCY_LIBS})
#set(CMAKE_CXX_FLAGS "-std=gnu++1y -stdlib=libc++ -D${PLATFORM} ${CMAKE_CXX_FLAGS} -pipe -ggdb -g3 -pg -O0")
#set(CMAKE_CXX_FLAGS "-std=gnu++1y -stdlib=libc++ -D${PLATFORM} ${CMAKE_CXX_FLAGS}")

set(DEBUG_FLAGS "${CMAKE_CXX_FLAGS} -g3 -O0 -Wno-deprecated-declarations")
set(RELEASE_FLAGS_32 "${CMAKE_CXX_FLAGS} -m32 -mfpmath=sse -Ofast -flto -march=native -funroll-loops -Wno-deprecated-declarations")
set(RELEASE_FLAGS_64 "${CMAKE_CXX_FLAGS} -m64 -mfpmath=sse -Ofast -flto -march=native -funroll-loops -Wno-deprecated-declarations")

if (APPLE)
	set(CMAKE_CXX_FLAGS "${DEBUG_FLAGS} -std=c++14 -stdlib=libc++")
	include_directories(~/Documents/libcxx/include)
else()
	#set(CMAKE_CXX_FLAGS "${DEBUG_FLAGS} -std=c++14 -stdlib=libc++")
	set(CMAKE_CXX_FLAGS "${DEBUG_FLAGS} -std=c++14")
	#add_definitions(-DGLEW_STATIC)
	#add_definitions(-DSFML_STATIC)
endif()

set(SOURCE_EXE
		src/resources.cpp
		include/resources.h
		src/main.cpp
		src/editor/core.cpp
		src/editor/core.h
		src/editor/events.cpp
		src/editor/manipulators/manipulator.cpp
		src/editor/manipulators/manipulator.h
		src/editor/commands/command.cpp
		src/editor/commands/command.h
		src/editor/settings.cpp
		src/editor/settings.h
		src/editor/session.cpp
		src/editor/session.h
		src/editor/shortkey.cpp
		src/editor/shortkey.h
		src/editor/scene.cpp
		src/editor/scene.h
		src/editor/scene/sceneobject.cpp
		src/editor/scene/sceneobject.h
		src/editor/scene/sprite.cpp
		src/editor/scene/sprite.h
		src/editor/scene/camera.cpp
		src/editor/scene/camera.h
		src/editor/scene/light.cpp
		src/editor/scene/light.h
		src/editor/scene/sound.cpp
		src/editor/scene/sound.h
		src/editor/scene/tilemap.cpp
		src/editor/scene/tilemap.h
		src/editor/scene/polygon.cpp
		src/editor/scene/polygon.h
		src/editor/scene/terrain.cpp
		src/editor/scene/terrain.h
		src/editor/scene/prefab.cpp
		src/editor/scene/prefab.h
		src/editor/workspace.cpp
		src/editor/workspace.h
		src/editor/scene/point.cpp
		src/editor/scene/point.h
		src/editor/modificators/knife.cpp
		src/editor/modificators/knife.h
		src/editor/modificators/cut.cpp
		src/editor/modificators/cut.h
		src/editor/modificators/modificator.cpp
		src/editor/modificators/modificator.h
		src/editor/modificators/retopology.cpp
		src/editor/modificators/retopology.h
		src/editor/modificators/boolean.cpp
		src/editor/modificators/boolean.h
		src/editor/modificators/partition.cpp
		src/editor/modificators/partition.h
		src/editor/modificators/extrude.cpp
		src/editor/modificators/extrude.h
		src/editor/transformators/transformator.cpp
		src/editor/transformators/transformator.h
		src/editor/transformators/bend.cpp
		src/editor/transformators/bend.h
		src/editor/transformators/wave.cpp
		src/editor/transformators/wave.h
		src/editor/transformators/screw.cpp
		src/editor/transformators/screw.h
		src/editor/transformators/widen.cpp
		src/editor/transformators/widen.h
		src/editor/transformators/arc.cpp
		src/editor/transformators/arc.h
		src/editor/transformators/arctovertex.cpp
		src/editor/transformators/arctovertex.h
		src/editor/physic/join.cpp
		src/editor/physic/join.h
		src/editor/physic/shape.cpp
		src/editor/physic/shape.h
		src/editor/physic/body.cpp
		src/editor/physic/body.h
		src/editor/mapeditor.cpp
		src/editor/mapeditor.h)

add_executable(main ${SOURCE_EXE})
include_directories(include
		/usr/include/freetype2/
		/home/int64/dev/boost/
		src/utility
		src/renderer
		src/ui)

add_subdirectory(src/utility)
add_subdirectory(src/renderer)
add_subdirectory(src/ui)

target_link_libraries(main e2dit-utility e2dit-renderer e2dit-ui ${libs})
