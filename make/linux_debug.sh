#!/bin/bash
#
flags="-O0 -g -w -Wfatal-errors -D_linux_ -D_x86_64_ -funroll-loops -lpthread -lX11 -lpthread -lrt -lm"
libs="-lGLEW -lGL -lGLU -lfreetype -lSOIL -ljpeg -lsfml-graphics -lsfml-window -lsfml-system -lftgl"
out="core"
gapicpp=""
uicpp=""
corecpp=""
#
g++ MainSFML.cpp $gapicpp $uicpp $corecpp $flags $libs -I/usr/include/freetype2 -o $out
