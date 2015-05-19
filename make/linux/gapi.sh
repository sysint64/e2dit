#!/bin/bash

boost_include="/home/int64/dev/boost/"
boosl_libs="/home/int64/dev/boost/stage/lib/"

stdcpp="-std=gnu++1y -stdlib=libc++"
pathes="-I./src/ -I./include/ -I/usr/include/freetype2/ -L./build/libs/ -I$boost_include -L$boosl_libs"
flags="-O0 $stdcpp -g -Wfatal-errors -D_linux_ -D_x86_64_ -pipe"

compiler="clang++-3.6"
analyzer="cppcheck"

# build GAPI library

gapiflags="-O0 $stdcpp -shared -g -fPIC -Wall -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -lGLEW -lGL -lGLU -lSOIL -ljpeg -ldl -pipe"
gapicpp="src/renderer/base_object.cpp src/renderer/camera.cpp src/renderer/data_render.cpp
         src/renderer/material.cpp src/renderer/shader.cpp src/renderer/texture.cpp"

gapilibs="-lftgl -lfreetype -lboost_system -lboost_filesystem -le2dit-utility"
gapiout="./build/libs/libe2dit-gapi.so"

if [ $1 = "-a" ]
then
	$analyzer $gapicpp
fi

$compiler $gapicpp $pathes $gapiflags $gapilibs -o $gapiout
