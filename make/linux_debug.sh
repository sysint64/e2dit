#!/bin/bash

boost_include="/home/int64/dev/boost/"
boosl_libs="/home/int64/dev/boost/stage/lib/"

stdcpp="-std=gnu++1y -stdlib=libc++"
pathes="-I./src/ -I./include/ -I/usr/include/freetype2/ -L./build/libs/ -I$boost_include -L$boosl_libs"
flags="-O0 $stdcpp -g -Wfatal-errors -D_linux_ -D_x86_64_"
libs="-lGLEW -lGL -lGLU -lfreetype -lSOIL -ljpeg -lftgl -lsfml-graphics-2-2 -lsfml-window-2-2 -lsfml-system-2-2 -lboost_system -lboost_filesystem
      -le2dit-ui -le2dit-gapi -le2dit-utility"
out="./build/e2dit"

maincpp="src/main.cpp"
#systemcpp="src/system/application.cpp src/system/logger.cpp"
corecpp="src/core/events.cpp src/core/core.cpp"
renderercpp="src/renderer/base_object.cpp src/renderer/camera.cpp src/renderer/data_render.cpp
             src/renderer/material.cpp src/renderer/shader.cpp src/renderer/texture.cpp"

compiler="clang++-3.6"
analyzer="cppcheck"

# build Utility Library

utilityflags="-O0 $stdcpp -shared -g -fPIC -Wall -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops"
utilitycpp="src/utility/data_map.cpp src/utility/data_map_lexer.cpp src/utility/data_map_parser.cpp src/utility/logger.cpp src/utility/application.cpp"

utilitylibs="-lboost_system -lboost_filesystem"
utilityout="./build/libs/libe2dit-utility.so"

if [ $1 = "-a" ]
then
	$analyzer $utilitycpp
fi

$compiler $utilitycpp $pathes $utilityflags $utilitylibs -o $utilityout

# build GAPI library

gapiflags="-O0 $stdcpp -shared -g -fPIC -Wall -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -lGLEW -lGL -lGLU -lSOIL -ljpeg -ldl"
gapicpp="src/renderer/base_object.cpp src/renderer/camera.cpp src/renderer/data_render.cpp
         src/renderer/material.cpp src/renderer/shader.cpp src/renderer/texture.cpp"

gapilibs="-lftgl -lfreetype -lboost_system -lboost_filesystem -le2dit-utility"
gapiout="./build/libs/libe2dit-gapi.so"

if [ $1 = "-a" ]
then
	$analyzer $gapicpp
fi

$compiler $gapicpp $pathes $gapiflags $gapilibs -o $gapiout

# build UI library

uiflags="-O0 $stdcpp -shared -g -fPIC -Wall -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -ldl"

# Precomputed

uipcpp="src/ui/precompute/element.cpp src/ui/precompute/image.cpp src/ui/precompute/panel.cpp
        src/ui/precompute/button.cpp src/ui/precompute/menus.cpp src/ui/precompute/edit.cpp
        src/ui/precompute/label.cpp"

uicpp="src/ui/manager.cpp src/ui/element.cpp src/ui/image.cpp src/ui/panel.cpp src/ui/button.cpp
       src/ui/menus.cpp src/ui/edit.cpp src/ui/label.cpp src/ui/theme.cpp src/ui/cursor.cpp"

uilibs="-lftgl -lfreetype -lboost_system -lboost_filesystem -le2dit-gapi -le2dit-utility"
uiout="./build/libs/libe2dit-ui.so"

if [ $1 = "-a" ]
then
	$analyzer $uicpp
fi

$compiler $uipcpp $uicpp $pathes $uiflags $uilibs -o $uiout

# build main program

if [ $1 = "-a" ]
then
	$analyzer $maincpp $renderercpp $corecpp
fi

$compiler $maincpp $pathes $corecpp $flags $libs -o $out
