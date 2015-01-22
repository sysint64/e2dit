#!/bin/bash
#
flags="-O3 -w -Wfatal-errors -D_linux_ -D_x86_64_ -ffast-math -funsafe-math-optimizations -m64 -march=native -fwhole-program -mfpmath=sse -funroll-loops -flto -ftree-vectorize -lpthread -lX11 -lpthread -lrt -lm"
libs="-lGLEW -lGL -lGLU -lfreetype -lSOIL -lsfml-graphics -lsfml-window -lsfml-system -lftgl"
out="core"
gapicpp=""
uicpp=""
corecpp=""
#
g++ MainSFML.cpp $gapicpp $uicpp $corecpp $flags $libs -I/usr/include/freetype2 -o $out