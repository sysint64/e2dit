#!/bin/bash

boost_include="/home/int64/dev/boost/"
boosl_libs="/home/int64/dev/boost/stage/lib/"

stdcpp="-std=gnu++1y -stdlib=libc++"
pathes="-I./src/ -I./include/ -I/usr/include/freetype2/ -L./build/libs/ -I$boost_include -L$boosl_libs"
flags="-O0 $stdcpp -g -Wfatal-errors -D_linux_ -D_x86_64_ -pipe"

compiler="clang++-3.6"
analyzer="cppcheck"

# build UI library

uiflags="-O0 $stdcpp -shared -g -fPIC -Wall -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -ldl -pipe"

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
