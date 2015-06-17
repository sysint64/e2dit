cmake_minimum_required(VERSION 2.8)

project(e2dit)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
#set(Boost_NO_SYSTEM_PATHS ON)
set(SFML_NO_SYSTEM_PATHS ON)

if (UNIX)
	set(PLATFORM _linux_)
endif (UNIX)

# Required Libs

find_package(Freetype REQUIRED)
find_package(FTGL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(SOIL REQUIRED)
find_package(JPEG REQUIRED)

#set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.2 COMPONENTS graphics window system REQUIRED)
find_package(Boost COMPONENTS system filesystem REQUIRED)

#message(SEND_ERROR ${SFML_GRAPHICS_LIBRARY_DEBUG})

set(SFML_LIBS ${SFML_GRAPHICS_LIBRARY_DEBUG} ${SFML_WINDOW_LIBRARY_DEBUG} ${SFML_SYSTEM_LIBRARY_DEBUG})
set(GL_LIBS ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
set(IMAGE_LIBS ${SOIL_LIBRARIES} ${JPEG_LIBRARIES})
set(FONT_LIBS ${FREETYPE_LIBRARIES} ${FTGL_LIBRARIES})
set(BOOST_LIBS ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY})

set(libs ${FONT_LIBS} ${IMAGE_LIBS} ${GL_LIBS} ${BOOST_LIBS} ${SFML_LIBS})
set(CMAKE_CXX_FLAGS "-std=gnu++1y -stdlib=libc++ -D${PLATFORM} ${CMAKE_CXX_FLAGS} -pipe -ggdb -g3 -pg -O0 -ftest-coverage -fprofile-arcs")
set(SOURCE_EXE src/main.cpp src/core/core.cpp src/core/events.cpp)

add_executable(main ${SOURCE_EXE})
include_directories(include /usr/include/freetype2/ /home/int64/dev/boost/ src/utility src/renderer src/ui)
add_subdirectory(src/utility)
add_subdirectory(src/renderer)
add_subdirectory(src/ui)
target_link_libraries(main libe2dit-utility libe2dit-renderer libe2dit-ui ${libs})
