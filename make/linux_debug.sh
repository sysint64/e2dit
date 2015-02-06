#!/bin/bash

flags="-O0 -std=c++11 -g -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -lpthread -lX11 -lpthread -lrt -lm"
libs="-lGLEW -lGL -lGLU -lfreetype -lSOIL -ljpeg -lboost_system -lboost_filesystem -lsfml-graphics -lsfml-window -lsfml-system -lftgl"
out="e2dit"

maincpp="main.cpp logger.cpp"
systemcpp="src/system/application.cpp"
corecpp="src/core/events.cpp src/core/core.cpp"
renderercpp="src/renderer/base_object.cpp src/renderer/camera.cpp src/renderer/data_render.cpp
             src/renderer/material.cpp src/renderer/shader.cpp src/renderer/texture.cpp"

compiler="clang++"
analyzer="cppcheck"

# build ui library

uicpp="src/ui/manager.cpp src/ui/element.cpp src/ui/image.cpp src/ui/panel.cpp src/ui/button.cpp
       src/ui/menus.cpp src/ui/edit.cpp src/ui/label.cpp"

if [ $1 = "-a" ]
then
	$analyzer $uicpp
fi

# build main program

if [ $1 = "-a" ]
then
	$analyzer $maincpp $systemcpp $renderercpp $corecpp
fi

$compiler $maincpp $systemcpp $renderercpp $corecpp $flags $libs -I/usr/include/freetype2 -o $out
