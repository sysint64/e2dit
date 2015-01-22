#!/bin/bash
#
flags="-w -Wfatal-errors -D_linux_ -D_x86_ -O3 -ffast-math -funsafe-math-optimizations -m32 -march=native -fwhole-program -mfpmath=sse -funroll-loops -flto -ftree-vectorize -lpthread -lX11 -lpthread -lrt -lm"
libs="-lGLEW -lGL -lGLU -lfreetype -lSOIL -lsfml-graphics -lsfml-window -lsfml-system -lftgl"
out="core"
gapicpp=""
uicpp=""
corecpp=""
#
g++ MainSFML.cpp $gapicpp $uicpp $corecpp $flags $libs -o $out
