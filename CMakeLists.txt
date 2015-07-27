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

if (APPLE)
	find_library(CORE_FOUNDATION NAMES CoreFoundation)
	find_library(APPLICATION_SERVICES NAMES ApplicationServices)
	find_library(IO_KIT NAMES IOKit)
	find_library(CORE_SERVICES NAMES CoreServices)
	find_library(COCOA NAMES Cocoa)
	find_library(CARBON NAMES Carbon)

	set(DEPENDENCY_LIBS ${CORE_FOUNDATION} ${APPLICATION_SERVICES} ${IO_KIT} ${CORE_SERVICES} ${COCOA} ${CARBON})
endif()

include_directories(${FTGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIR})
include_directories(${SOIL_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIR})
include_directories(${FREETYPE_INCLUDE_DIRS})

add_definitions(-DGLM_FORCE_RADIANS)
add_definitions(-D${PLATFORM})

#set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.2 COMPONENTS graphics window system REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_LIBS ${SFML_GRAPHICS_LIBRARY_DEBUG} ${SFML_WINDOW_LIBRARY_DEBUG} ${SFML_SYSTEM_LIBRARY_DEBUG})
set(GL_LIBS ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
set(IMAGE_LIBS ${SOIL_LIBRARIES} ${JPEG_LIBRARIES})
set(FONT_LIBS ${FREETYPE_LIBRARIES} ${FTGL_LIBRARIES})
set(BOOST_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

set(libs ${FONT_LIBS} ${IMAGE_LIBS} ${GL_LIBS} ${BOOST_LIBS} ${SFML_LIBS} ${DEPENDENCY_LIBS})
#set(CMAKE_CXX_FLAGS "-std=gnu++1y -stdlib=libc++ -D${PLATFORM} ${CMAKE_CXX_FLAGS} -pipe -ggdb -g3 -pg -O0")
#set(CMAKE_CXX_FLAGS "-std=gnu++1y -stdlib=libc++ -D${PLATFORM} ${CMAKE_CXX_FLAGS}")

if (APPLE)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -stdlib=libc++")
	include_directories(~/Documents/libcxx/include)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -stdlib=libc++")
endif()

set(SOURCE_EXE src/main.cpp src/core/core.cpp src/core/events.cpp)

add_executable(main ${SOURCE_EXE})
include_directories(include /usr/include/freetype2/ /home/int64/dev/boost/ src/utility src/renderer src/ui)
add_subdirectory(src/utility)
add_subdirectory(src/renderer)
add_subdirectory(src/ui)

target_link_libraries(main libe2dit-utility libe2dit-renderer libe2dit-ui ${libs})
