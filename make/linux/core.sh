#!/bin/bash

boost_include="/home/int64/dev/boost/"
boosl_libs="/home/int64/dev/boost/stage/lib/"

stdcpp="-std=gnu++1y -stdlib=libc++"
pathes="-I./src/ -I./include/ -I/usr/include/freetype2/ -L./build/libs/ -I$boost_include -L$boosl_libs"
flags="-O0 $stdcpp -g -Wfatal-errors -D_linux_ -D_x86_64_ -pipe"
libs="-lGLEW -lGL -lGLU -lfreetype -lSOIL -ljpeg -lftgl -lsfml-graphics-2-2 -lsfml-window-2-2 -lsfml-system-2-2 -lboost_system -lboost_filesystem
      -le2dit-ui -le2dit-gapi -le2dit-utility"
out="./build/e2dit"

maincpp="src/main.cpp"
corecpp="src/core/events.cpp src/core/core.cpp"

compiler="clang++-3.6"
analyzer="cppcheck"

# build main program

if [ $1 = "-a" ]
then
	$analyzer $maincpp $corecpp
fi

$compiler $maincpp $pathes $corecpp $flags $libs -o $out
