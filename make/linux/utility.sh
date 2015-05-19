#!/bin/bash

boost_include="/home/int64/dev/boost/"
boosl_libs="/home/int64/dev/boost/stage/lib/"

stdcpp="-std=gnu++1y -stdlib=libc++"
pathes="-I./src/ -I./include/ -I/usr/include/freetype2/ -L./build/libs/ -I$boost_include -L$boosl_libs"
flags="-O0 $stdcpp -g -Wfatal-errors -D_linux_ -D_x86_64_ -pipe"

compiler="clang++-3.6"
analyzer="cppcheck"

# build Utility Library

utilityflags="-O0 $stdcpp -shared -g -fPIC -Wall -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -pipe"
utilitycpp="src/utility/data_map.cpp src/utility/data_map_lexer.cpp src/utility/data_map_parser.cpp src/utility/logger.cpp src/utility/application.cpp"

utilitylibs="-lboost_system -lboost_filesystem"
utilityout="./build/libs/libe2dit-utility.so"

if [ $1 = "-a" ]
then
	$analyzer $utilitycpp
fi

$compiler $utilitycpp $pathes $utilityflags $utilitylibs -o $utilityout