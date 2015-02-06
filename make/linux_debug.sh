#!/bin/bash

pathes="-I./src/ -I./include/ -I/usr/include/freetype2 -L./build/libs/"
flags="-O0 -std=c++11 -g -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -lpthread -lX11 -lpthread -lrt -lm"
libs="-lGLEW -lGL -lGLU -lfreetype -lSOIL -ljpeg -lboost_system -lboost_filesystem -lsfml-graphics -lsfml-window -lsfml-system -lftgl
      -le2dit-ui -le2dit-gapi"
out="./build/e2dit"

maincpp="src/main.cpp"
systemcpp="src/system/application.cpp src/system/logger.cpp"
corecpp="src/core/events.cpp src/core/core.cpp"
renderercpp="src/renderer/base_object.cpp src/renderer/camera.cpp src/renderer/data_render.cpp
             src/renderer/material.cpp src/renderer/shader.cpp src/renderer/texture.cpp"

compiler="clang++"
analyzer="cppcheck"

# build GAPI library

gapiflags="-O0 -std=c++11 -shared -g -fPIC -Wall -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -lGLEW -lGL -lGLU -lSOIL -ljpeg -ldl"
gapicpp="src/renderer/base_object.cpp src/renderer/camera.cpp src/renderer/data_render.cpp
         src/renderer/material.cpp src/renderer/shader.cpp src/renderer/texture.cpp"

gapilibs="-lftgl -lfreetype"
gapiout="./build/libs/libe2dit-gapi.so"

if [ $1 = "-a" ]
then
	$analyzer $gapicpp
fi

$compiler $gapicpp $pathes $gapiflags $gapilibs -o $gapiout -lc

# build UI library

uiflags="-O0 -std=c++11 -shared -g -fPIC -Wall -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -ldl"
uicpp="src/ui/manager.cpp src/ui/element.cpp src/ui/image.cpp src/ui/panel.cpp src/ui/button.cpp
       src/ui/menus.cpp src/ui/edit.cpp src/ui/label.cpp"

uilibs="-lftgl -lfreetype -le2dit-gapi"
uiout="./build/libs/libe2dit-ui.so"

if [ $1 = "-a" ]
then
	$analyzer $uicpp
fi

$compiler $uicpp $pathes $uiflags $uilibs -o $uiout -lc

# build main program

if [ $1 = "-a" ]
then
	$analyzer $maincpp $systemcpp $renderercpp $corecpp
fi

$compiler $maincpp $pathes $systemcpp $corecpp $flags $libs -o $out
